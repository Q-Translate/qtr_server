<?php
/**
 * @file
 * Function translation_add().
 */

namespace QTranslate;
use \qtr;

/**
 * Add a new translation to a source string.
 *
 * @param $vid
 *   The verse ID for which a new translation should be added.
 *
 * @param $lng
 *   The language (code) of the new translation.
 *
 * @param $translation
 *   The new translation as a string.
 *
 * @param $uid (optional)
 *   Id of the user that is adding the string.
 *
 * @param $notify (optional)
 *   It TRUE, notify relevant users about the new translation.
 *
 * @return
 *   ID of the new translation, or NULL if no translation was added.
 */
function translation_add($vid, $lng, $translation, $uid = NULL, $notify = TRUE) {
  // Get the user account.
  $uid = qtr::user_check($uid);
  $account = user_load($uid);

  // Check that the language matches translation_lng of the user.
  if ($lng != $account->translation_lng && $uid != 1) {
    $msg = t('Not allowed to make translations for the language: !lng.', ['!lng' => $lng]);
    qtr::messages($msg, 'warning');
    return FALSE;
  }

  // Don't add empty translations.
  $translation = str_replace(t('<New translation>'), '', $translation);
  if (trim($translation) == '')  {
    $msg = t('The given translation is empty.');
    qtr::messages($msg, 'warning');
    return NULL;
  }

  // Look for an existing translation, if any.
  $tguid = sha1($translation . $lng . $vid);
  $existing = qtr::translation_get($tguid);

  // If this translation already exists, there is nothing to be added.
  if (!empty($existing))  {
    if ($notify) {
      $msg = t('The given translation already exists.');
      qtr::messages($msg, 'warning');
    }
    return $tguid;
  }

  // Get the email of the author of the translation.
  $umail = ($uid==1 ?  $umail = '' : $account->init);

  // Insert the new translation.
  qtr::db_insert('qtr_translations')
    ->fields(array(
        'vid' => $vid,
        'lng' => $lng,
        'translation' => $translation,
        'tguid' => $tguid,
        'count' => 0,
        'umail' => $umail,
        'ulng' => $lng,
        'time' => date('Y-m-d H:i:s', REQUEST_TIME),
      ))
    ->execute();

  // If there is another translation for the same verse, by the same user,
  // the new translation should replace the old one. This is useful when
  // the user wants to correct the translation, but it limits the user to
  // only one suggested translation per verse.
  // However, translators and admins (with the access rights 'qtranslate-import'
  // and 'qtranslate-admin') do not have this limitation and can suggest more
  // than one translation for the same verse.
  if ( !user_access('qtranslate-import', $account)
    && !user_access('qtranslate-admin', $account) && $uid != 1 )
  {
    _remove_old_translation($vid, $lng, $umail, $tguid);
  }

  // Add also a like for the new translation (but not if it is added by admin).
  if ($uid > 1) {
    qtr::like_add($tguid, $uid);
  }

  // Notify other users that a new translation has been suggested.
  if ($notify) {
    _notify_users_on_new_translation($vid, $lng, $tguid, $translation);
  }

  return $tguid;
}


/**
 * If there is another translation for the same verse, by the same user,
 * the new translation should replace the old one. This is useful when
 * the user wants to correct the translation, but it limits the user to
 * only one suggested translation per verse.
 *
 * @param $vid
 *   Id of the verse being translated.
 *
 * @param $lng
 *   Language of translation.
 *
 * @param $umail
 *   Email that identifies the user who made the translation.
 *
 * @param $tguid
 *   Id of the new translation.
 */
function _remove_old_translation($vid, $lng, $umail, $tguid) {
  // Get the old translation (if any).
  $query = 'SELECT tguid, translation
            FROM {qtr_translations}
            WHERE vid = :vid AND lng = :lng
              AND umail = :umail AND ulng = :ulng
              AND tguid != :tguid';
  $args = array(
    ':vid' => $vid,
    ':lng' => $lng,
    ':umail' => $umail,
    ':ulng' => $lng,
    ':tguid' => $tguid);
  $old_trans = qtr::db_query($query, $args)->fetchObject();
  if (!$old_trans)  return;  // if there is no old translation, we are done

  // Copy to the trash table the old translation.
  $query = qtr::db_select('qtr_translations', 't')
    ->fields('t', array('vid', 'lng', 'translation', 'tguid', 'count', 'umail', 'ulng', 'time', 'active'))
    ->condition('tguid', $old_trans->tguid);
  $query->addExpression(':d_umail', 'd_umail', array(':d_umail' => $umail));
  $query->addExpression(':d_ulng', 'd_ulng', array(':d_ulng' => $lng));
  $query->addExpression('NOW()', 'd_time');
  qtr::db_insert('qtr_translations_trash')->from($query)->execute();

  // Remove the old translation.
  qtr::db_delete('qtr_translations')
    ->condition('tguid', $old_trans->tguid)
    ->execute();

  // Get the likes of the old translation.
  $query = "SELECT l.tguid, l.time, u.umail, u.ulng, u.uid,
                   u.name AS user_name, u.status AS user_status
            FROM {qtr_likes} l
            JOIN {qtr_users} u ON (u.umail = l.umail AND u.ulng = l.ulng)
            WHERE l.tguid = :tguid AND l.umail != :umail";
  $args = array(':tguid' => $old_trans->tguid, ':umail' => $umail);
  $likes = qtr::db_query($query, $args)->fetchAll();

  // Insert to the trash table the likes that will be deleted.
  $query = qtr::db_select('qtr_likes', 'l')
    ->fields('l', array('lid', 'tguid', 'umail', 'ulng', 'time', 'active'))
    ->condition('tguid', $old_trans->tguid);
  $query->addExpression('NOW()', 'd_time');
  qtr::db_insert('qtr_likes_trash')->from($query)->execute();

  // Delete the likes belonging to the old translation.
  qtr::db_delete('qtr_likes')->condition('tguid', $old_trans->tguid)->execute();

  // Associate these likes to the new translation.
  $notification_list = array();
  foreach ($likes as $like) {
    // Associate the like to the new translation.
    qtr::db_insert('qtr_likes')
      ->fields(array(
          'tguid' => $tguid,
          'umail' => $like->umail,
          'ulng' => $like->ulng,
          'time' => $like->time,
        ))
      ->execute();

    if ($like->user_status != 1)  continue;   // skip non-active users

    // Add user to the notification list.
    $notification_list[$like->uid] = array(
      'uid' => $like->uid,
      'name' => $like->user_name,
      'umail' => $like->umail,
    );
  }

  _notify_users_on_translation_change($notification_list, $vid, $lng, $old_trans->translation, $tguid);
}

/**
 * Notify the users that have liked a translation that the author has changed
 * the translation and their likes count now for the new translation.
 */
function _notify_users_on_translation_change($users, $vid, $lng, $old_translation, $tguid) {

  if (empty($users))  return;

  // Get details of the verse and the translation.
  $verse = qtr::db_query(
    'SELECT * FROM {qtr_verses} WHERE vid = :vid',
    array(':vid' => $vid)
  )->fetch();
  $new_translation = qtr::translation_get($tguid);

  $notifications = array();
  foreach ($users as $uid => $user) {
    if (!qtr::user_send_mail($uid)) continue;
    $notification = array(
      'type' => 'notify-user-on-translation-change',
      'uid' => $user['uid'],
      'username' => $user['name'],
      'recipient' => $user['name'] . ' <' . $user['umail'] . '>',
      'lng' => $lng,
      'chapter_id' => $verse->cid,
      'verse_nr' => $verse->nr,
      'verse' => $verse->verse,
      'old_translation' => $old_translation,
      'new_translation' => $new_translation,
    );
    $notifications[] = $notification;
  }

  qtr::queue('notifications', $notifications);
}

/**
 * Notify the users that have supported a translation that a new translation has been
 * submitted. Maybe they would like to review it and change their support.
 */
function _notify_users_on_new_translation($vid, $lng, $tguid, $translation) {
  $query = "SELECT u.umail, u.ulng, u.uid, u.name, u.status, t.translation
            FROM {qtr_translations} t
            JOIN {qtr_likes} l ON (l.tguid = t.tguid)
            JOIN {qtr_users} u ON (u.umail = l.umail AND u.ulng = l.ulng)
            WHERE t.vid = :vid AND t.lng = :lng AND t.tguid != :tguid";
  $args = array(':vid' => $vid, ':lng' => $lng, ':tguid' => $tguid);
  $users = qtr::db_query($query, $args)->fetchAll();

  if (empty($users))  return;

  // Get the details of the verse.
  $verse = qtr::db_query(
    'SELECT * FROM {qtr_verses} WHERE vid = :vid',
    array(':vid' => $vid)
  )->fetch();

  $notifications = array();
  foreach ($users as $user) {
    if (!qtr::user_send_mail($user->uid)) continue;
    $notification = array(
      'type' => 'notify-user-on-new-translation',
      'uid' => $user->uid,
      'username' => $user->name,
      'recipient' => $user->name . ' <' . $user->umail . '>',
      'lng' => $lng,
      'chapter_id' => $verse->cid,
      'verse_nr' => $verse->nr,
      'verse' => $verse->verse,
      'liked_translation' => $user->translation,
      'new_translation' => $translation,
    );
    $notifications[] = $notification;
  }

  qtr::queue('notifications', $notifications);
}
