<?php
/**
 * @file
 * Functions that are used for sending notification emails.
 */

namespace QTranslate;
use \qtr;

/**
 * Sends by email a notification message.
 *
 * The subject and body of the message depend on the $params->type.
 * Other required attributes of $params are $params->recipient
 * and $params->uid. The other attributes are needed to build the
 * subject and body of the message. Some common attributes are:
 *   $params->uid, $params->lng, $params->chapter_id, $params->verse_nr,
 *   $params->username, $params->verse, $params->translation, etc.
 */
function email_send($params) {
  if (!$params->uid or !$params->recipient)  return;
  if (!qtr::user_send_mail($params->uid))  return;


  $account = user_load($params->uid);
  // See: http://api.drupal.org/api/drupal/includes%21mail.inc/function/drupal_mail/7
  drupal_mail(
    $module   = 'qtrCore',
    $key      = 'notifications',
    $to       = $params->recipient,
    $langcode = $account->language,
    $params   = get_subject_and_body($params),
    $from     = get_sender(),
    $send     = TRUE
  );
}


/**
 * Return the sender of the email notifications
 * (which is always the same, as defined on smtp variables)
 */
function get_sender() {
  $smtp_from = variable_get('smtp_from');
  $smtp_fromname = variable_get('smtp_fromname');
  return "$smtp_fromname <$smtp_from>";
}

/**
 * Returns the subject and body of the email notification:
 *    array('subject' => $subject, 'body' => $body)
 *
 * The subject and body of the message depend on the type
 * of the notification, defined by $params->type.
 *
 * The other attributes depend on the notification type.
 * Some common attributes are:
 * $params->uid, $params->lng, $params->chapter_id, $params->verse_nr,
 * $params->username, $params->verse, $params->translation, etc.
 */
function get_subject_and_body($params) {
  $account = user_load($params->uid);
  $lng = $account->translation_lng;
  $subject_prefix = "qtr-$lng";

  // Get the url of the translation site.
  module_load_include('inc', 'qtrCore', 'lib/sites');
  $client_url = qtr::utils_get_client_url($lng);

  // Get the url of the verse.
  if (isset($params->chapter_id) && isset($params->verse_nr)) {
    $url = $client_url . "/qtr/$lng/$params->chapter_id/$params->verse_nr";
  }

  // Include the subject and body of the message.
  include(dirname(__FILE__) . '/msg/' . $params->type . '.inc');

  // Return the subject and body of the message.
  return array(
    'subject' => $subject,
    'body' => $body,
  );
}

/**
 * From the given (possibly long) string, returns a short string
 * of the given length (that can be suitable for title, subject, etc.)
 */
function cut($str, $length) {
  $str = str_replace("\n", ' ', $str);
  $str = strip_tags($str);
  if (strlen($str) > $length) {
    $str = substr($str, 0, strrpos(substr($str, 0, $length - 3), ' '));
    $str .= '...';
  }
  $str = utf8_decode($str);
  $str = htmlentities($str);
  return $str;
}
