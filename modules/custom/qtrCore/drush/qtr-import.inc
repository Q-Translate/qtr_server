<?php
/**
 * Description of the command 'qtr-import'.
 */
function _qtr_import_drush_command() {
  return array(
    'description' => "Import translation from a text file.",
    'arguments' => array(
      'lng' => 'The language of translation (de, fr, sq, en_GB, etc.)',
      'author' => 'The author of translation',
      'file' => 'A text file that has the translation of each verse on a separate line.',
    ),
    'options' => array(
      'force' => 'If true, then permissions checking will be skipped.',
    ),
    'examples' => array(
      "drush qtr-imp en pickthall \$(pwd)/pickthall.txt" => 'Import the Pickthall English translations.',
    ),
    'aliases' => array('qtr-imp'),
  );
}

/**
 * Help text for the command 'qtr-import'.
 */
function _qtr_import_drush_help() {
  return dt('Import translation from a text file.');
}

/**
 * Callback function for the command 'qtr-import'.
 */
function drush_qtranslate_qtr_import($lng, $author, $file) {
  // Create the user and get its id.
  $uid = qtr::user_add($author, $lng);

  // Check user permissions.
  $force = drush_get_option('force');
  if (!$force) {
    _drush_qtranslate_qtr_import_check($uid, $lng);
  }

  // Check that there is such a file.
  if (!file_exists($file)) {
    drush_log(t('File "!file" does not exist.', ['!file' => $file]), 'error');
    drupal_exit();
  }

  // Import translations.
  qtr::import_translations($lng, $file, $uid, TRUE);
  foreach(qtr::messages() as $msg)  drush_log($msg[0], $msg[1]);
}

/**
 * Check user permissions.
 */
function _drush_qtranslate_qtr_import_check($uid, $lng) {
  $user = user_load($uid);

  // Check user access permissions.
  if (!user_access('qtranslate-translate')) {
    drush_log(t('User "!user" has no rights for suggesting translations!', ['!user' => $user->name]), 'error');
    drupal_exit();
  }
  if (!user_access('qtranslate-like')) {
    drush_log(t('User "!user" has no rights for submiting likes!', ['!user' => $user->name]), 'error');
    drupal_exit();
  }

  // Get and check the parameter 'lng'.
  if (!qtr::lng_check($lng)) {
    drush_log(t('Language "!lng" is not supported.', array('!lng' => $lng)), 'error');
    drupal_exit();
  }

  // Make sure that this user can make translations for the given language.
  if ($lng != $user->translation_lng) {
    drush_log(t('User "!user" cannot submit translations for the language "!lng"',
        ['!user' => $user->name, '!lng' => $lng]
      ), 'error');
    drupal_exit();
  }
}
