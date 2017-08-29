<?php
/**
 * @file
 * Function: cron_send_verses_for_review()
 */

namespace QTranslate;
use \qtr;

/**
 * Send by email a string for review to all the active users.
 */
function cron_send_verses_for_review() {
  $notifications = array();
  $accounts = entity_load('user');
  foreach ($accounts as $account) {
    if (!qtr::user_send_mail($account))  continue;

    // get a random verse
    $vid = rand(1, 6236);
    $verse = qtr::db_query('SELECT * FROM {qtr_verses} WHERE vid = :vid', [':vid' => $vid])->fetch();

    $message_params = array(
      'type' => 'verse-to-be-reviewed',
      'uid' => $account->uid,
      'username' => $account->name,
      'recipient' => $account->name .' <' . $account->mail . '>',
      'lng' => $account->translation_lng,
      'chapter_id' => $verse->cid,
      'verse_nr' => $verse->nr,
      'vid' => $vid,
    );
    $notifications[] = $message_params;
  }
  qtr::queue('notifications', $notifications);
}
