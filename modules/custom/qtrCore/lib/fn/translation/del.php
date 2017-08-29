<?php
/**
 * @file
 * Function: translation_del()
 */

namespace QTranslate;
use \qtr;

/**
 * Delete the translation with the given id and any related likes.
 *
 * @param $tguid
 *   ID of the translation.
 *
 * @param $notify
 *   Notify the author and likers of the deleted translation.
 *
 * @param $uid
 *   Id of the user that is deleting the translation.
 */
function translation_del($tguid, $notify = TRUE, $uid = NULL) {
  // Get the mail and lng of the user that is deleting the translation.
  $uid = qtr::user_check($uid);
  $account = user_load($uid);
  $umail = $account->init;    // email used for registration
  $ulng = $account->translation_lng;

  // Get the language of the translation.
  $lng = qtr::db_query(
    'SELECT lng FROM {qtr_translations} WHERE tguid = :tguid',
    [':tguid' => $tguid]
  )->fetchField();

  // Check that the language matches that of the user.
  if ($lng != $ulng && $uid != 1) {
    $msg = t('Not allowed to delete translations of the language: !lng.', ['!lng' => $lng]);
    qtr::messages($msg, 'warning');
    return FALSE;
  }

  // Get the author of the translation.
  $author = qtr::db_query(
    'SELECT u.uid, u.name, u.umail
     FROM {qtr_translations} t
     JOIN {qtr_users} u ON (u.umail = t.umail AND u.ulng = t.ulng)
     WHERE t.tguid = :tguid',
    array(':tguid' => $tguid))
    ->fetchObject();

  // Check that the current user has the right to delete translations.
  $is_author = ($umail == $author->umail);
  $is_admin = ($uid == 1);
  if ($is_author || user_access('qtranslate-resolve', $account)
    || user_access('qtranslate-admin', $account) || $is_admin)
  {
    _translation_del($tguid, $notify, $umail, $ulng, $author);
  }
  else {
    _notify_moderators_for_wrong_translation($tguid, $lng, $uid);
    $msg = t('You do not have permission to delete this translation. However the admins will be notified about it.');
    qtr::messages($msg);
  }
}

/**
 * Delete the translation.
 */
function _translation_del($tguid, $notify, $umail, $ulng, $author) {
  // Before deleting, get the likers, verse and translation (for notifications).
  $users = qtr::db_query(
    'SELECT u.uid, u.name, u.umail
     FROM {qtr_likes} l
     JOIN {qtr_users} u ON (u.umail = l.umail AND u.ulng = l.ulng)
     WHERE l.tguid = :tguid',
    array(':tguid' => $tguid))
    ->fetchAll();
  $vid = qtr::db_query(
    'SELECT vid FROM {qtr_translations} WHERE tguid = :tguid',
    [':tguid' => $tguid]
  )->fetchField();
  $verse = qtr::verse_get($vid);
  $translation = qtr::translation_get($tguid);

  // Copy to the trash table the translation that will be deleted.
  $query = qtr::db_select('qtr_translations', 't')
    ->fields('t', array('tguid', 'vid', 'lng', 'translation', 'count', 'umail', 'ulng', 'time', 'active'))
    ->condition('tguid', $tguid);
  $query->addExpression(':d_umail', 'd_umail', array(':d_umail' => $umail));
  $query->addExpression(':d_ulng', 'd_ulng', array(':d_ulng' => $ulng));
  $query->addExpression('NOW()', 'd_time');
  qtr::db_insert('qtr_translations_trash')->from($query)->execute();

  // Copy to the trash table the likes that will be deleted.
  $query = qtr::db_select('qtr_likes', 'l')
    ->fields('l', array('lid', 'tguid', 'umail', 'ulng', 'time', 'active'))
    ->condition('tguid', $tguid);
  $query->addExpression('NOW()', 'd_time');
  qtr::db_insert('qtr_likes_trash')->from($query)->execute();

  // Delete the translation and any likes related to it.
  qtr::db_delete('qtr_translations')->condition('tguid', $tguid)->execute();
  qtr::db_delete('qtr_likes')->condition('tguid', $tguid)->execute();

  // Notify the author of a translation and its users
  // that it has been deleted.
  if ($notify) {
    _notify_users_on_translation_del($vid, $ulng, $translation, $author, $users);
  }
}

/**
 * Notify the author of a translation and its users
 * that it has been deleted.
 */
function _notify_users_on_translation_del($vid, $lng, $translation, $author, $users) {

  // Get details of the verse.
  $verse = qtr::db_query(
    'SELECT * FROM {qtr_verses} WHERE vid = :vid',
    array(':vid' => $vid)
  )->fetch();
  $notifications = array();

  // Notify the author of the translation about the deletion.
  if ($author->uid && qtr::user_send_mail($author->uid)) {
    $notification = array(
      'type' => 'notify-author-on-translation-deletion',
      'uid' => $author->uid,
      'username' => $author->name,
      'recipient' => $author->name . ' <' . $author->umail . '>',
      'lng' => $lng,
      'chapter_id' => $verse->cid,
      'verse_nr' => $verse->nr,
      'verse' => $verse->verse,
      'translation' => $translation,
    );
    $notifications[] = $notification;
  }

  // Notify the users that have liked the translation as well.
  foreach ($users as $user) {
    if (!qtr::user_send_mail($user->uid))  continue;
    if ($user->name == $author->name)  continue;  // don't send a second message to the author

    $notification = array(
      'type' => 'notify-user-on-translation-deletion',
      'uid' => $user->uid,
      'username' => $user->name,
      'recipient' => $user->name . ' <' . $user->umail . '>',
      'lng' => $lng,
      'chapter_id' => $verse->cid,
      'verse_nr' => $verse->nr,
      'verse' => $verse->verse,
      'translation' => $translation,
    );
    $notifications[] = $notification;
  }

  qtr::queue('notifications', $notifications);
}

/**
 * Notify the moderators of a language about the wrong translation.
 */
function _notify_moderators_for_wrong_translation($tguid, $lng, $uid) {

  // Get details of the verse and the translation.
  $vid = qtr::db_query(
    'SELECT vid FROM {qtr_translations} WHERE tguid = :tguid',
    [':tguid' => $tguid]
  )->fetchField();
  $verse = qtr::db_query(
    'SELECT * FROM {qtr_verses} WHERE vid = :vid',
    array(':vid' => $vid)
  )->fetch();
  $translation = qtr::translation_get($tguid);

  // Get a list of moderators and admins of the language.
  $uids = \db_query('SELECT uid FROM users_roles WHERE rid IN (3, 5)')->fetchCol();
  $moderators = user_load_multiple($uids);

  // Notify the moderators.
  $notifications = array();
  foreach ($moderators as $moderator) {
    if (!qtr::user_send_mail($moderator->uid)) continue;
    if ($moderator->translation_lng != $lng) continue;

    $notification = array(
      'type' => 'notify-moderators-for-wrong-translation',
      'uid' => $moderator->uid,
      'username' => $moderator->name,
      'recipient' => $moderator->name . ' <' . $moderator->init . '>',
      'author_uid' => $uid,
      'lng' => $lng,
      'chapter_id' => $verse->cid,
      'verse_nr' => $verse->nr,
      'verse' => $verse->verse,
      'translation' => $translation,
    );
    $notifications[] = $notification;
  }
  if (empty($notifications)) return;
  qtr::queue('notifications', $notifications);
}
