<?php
/**
 * @file
 * Functions called by cron.
 */

/**
 * Implements hook_cron_queue_info().
 */
function qtrCore_cron_queue_info() {

  $queues['notifications'] = array(
    'worker callback' => 'qtrCore_send_notification_by_email',
    'time' => 240,
  );
  $queues['import'] = array(
    'worker callback' => 'qtrCore_import',
    'time' => 2000,
  );
  $queues['delete_fake_users'] = array(
    'worker callback' => 'qtrCore_delete_fake_user',
    'time' => 30,
  );
  return $queues;
}

/**
 * Implements hook_cron().
 */
function qtrCore_cron() {
  qtrCore_cron_nightly();
}

/**
 * Make sure that cron functions runs only once per night,
 * no matter how often cron is called.
 */
function qtrCore_cron_nightly() {
  // check whether the crontime has been reached yet
  $crontime = variable_get('qtrCore_crontime', '0');
  if (time() < $crontime)  return;

  // set the crontime after the next midnight (after 03:00 AM)
  $crontime = strtotime(date('Y-m-d 02:55')) + 24*60*60;
  variable_set('qtrCore_crontime', $crontime);

  // Call cron functions.
  qtr::cron_cleanup_fake_users();
  qtr::cron_send_verses_for_review();
}

/**
 * The callback function called from cron_queue 'notifications'.
 */
function qtrCore_send_notification_by_email($params) {
  qtr::email_send($params);
}

/**
 * The callback function called from cron_queue 'import'.
 */
function qtrCore_import($params) {
  qtr::cron_import($params);
}

/**
 * The callback function called from cron_queue 'delete_fake_users'.
 */
function qtrCore_delete_fake_user($uid) {
  // Delete from the drupal table 'users'
  user_delete($uid);

  // Delete from the table 'qtr_users'
  qtr::db_delete('qtr_users')->condition('uid', $uid)->execute();
}
