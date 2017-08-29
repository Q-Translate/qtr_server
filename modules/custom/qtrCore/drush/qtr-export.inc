<?php
/**
 * Description of the command 'qtr-export'.
 */
function _qtr_export_drush_command() {
  return array(
    'description' => "Export the translations of a language.",
    'arguments' => array(
      'lng' => 'The language of translation (en, de, fr, sq, etc.)',
    ),
    'options' => array(
      'chapter' => 'The chapter to be exported.',
      'mode' => "'most_liked' (default) or 'preference'",
      'users' => 'Comma separated list of the preferred users.',
      'output' => 'The output file where the translations will be saved.',
    ),
    'examples' => array(
      "drush qtr-export en --chapter=1" => 'Export the most liked English translations of the first chapter.',
    ),
    'aliases' => array('qtr-exp'),
  );
}

/**
 * Help text for the command 'qtr-export'.
 */
function _qtr_export_drush_help() {
  return "Export the translations of a language..

The export mode 'most_liked' (which is the default one) exports the translations
with the bigest numberor likes.

The export mode 'preference' gives priority to translations that are liked by a
certain user or a group of users. It requires an additional option (users) to
specify the user (or a list of users) whose translations are preferred. If a
string has no translation that is liked by any of the preferred users, then the
most liked translation is exported.
";
}

/**
 * Callback function for the command 'qtr-export'.
 */
function drush_qtranslate_qtr_export($lng) {
  // Check $lng.
  if (!qtr::lng_check($lng)) {
    drush_log(t("The language '!lng' does not exist.", ['!lng' => "$lng"]), 'error');
    drupal_exit();
  }

  // Get the option 'chapter'.
  $chapter = drush_get_option('chapter', NULL);
  if ($chapter != NULL && ($chapter < 1 || $chapter > 114)) {
    drush_log(t("Chapter '!chapter' is out of range.", ['!chapter' => $chapter]), 'error');
    drupal_exit();
  }

  // Get the export mode.
  $mode = drush_get_option('mode', 'most_liked');
  if ($mode != 'most_liked' && $mode != 'preference') {
    drush_log(t("Unknown export mode '!mode'.", ['!mode' => $mode]), 'error');
    drupal_exit();
  }

  // Get the preference users.
  if ($mode == 'preference') {
    $users = drush_get_option('users', NULL);
    list($arr_emails, $error_messages) = qtr::utils_get_emails($users);
    if (!empty($error_messages)) {
      foreach ($error_messages as $msg)  drush_log($msg[0], $msg[1]);
      drupal_exit();
    }
  }

  // Get the option output.
  $output = drush_get_option('output');

  // Export the project.
  $file = qtr::export($lng, $chapter, $mode, $arr_emails);
  if ($output) {
    shell_exec("mv $file $output");
  }
  else {
    print(shell_exec("cat $file"));
    unlink($file);
  }
}
