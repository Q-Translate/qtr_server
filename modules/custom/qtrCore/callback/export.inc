<?php
/**
 * @file
 * Callback function for menu path: export
 */


/**
 * Callback function for the menu 'export'
 *
 * Return translations of a language as a text file.
 *
 * Does not allow concurrent exports because they may affect the performance
 * of the server.  Parameters are received from the request (either GET or
 * POST). They are:
 *
 *  - lng
 *      Translation to be exported.
 *
 *  - chapter
 *      Number of the chapter to be exported.
 *
 *  - mode
 *      Mode of export: most_liked | preference
 *
 *      Default is 'most_liked' which exports the translations with the biggest
 *      number of likes.
 *
 *      The export mode 'preference' gives priority to translations that are
 *      liked by a certain user or a group of users. It requires an additional
 *      argument (users) to specify the user (or a list of users) whose
 *      translations are preferred.
 *
 * - users
 *      Comma separated list of usernames of the preferred voters.
 *
 * Examples:
 *   curl -k 'https://qtranslate.org/export?lng=en' > quran_en.txt
 *   curl -k https://qtranslate.org/export \
 *        -d 'lng=en&chapter=1' > chapter1_en.txt
 *   curl -k https://qtranslate.org/export \
 *        -d chapter=2 -d lng=en \
 *        -d export_mode=preference \
 *        -d users=user1,user2 > chapter1_en.txt
 */
function qtrCore_export() {
  // Try to avoid concurrent exports because they
  // may affect the performance of the server.
  if (!lock_acquire('export')) {
    drupal_set_message(t("Error: Server is currently busy. Please try to export again later."), 'error');
    drupal_goto();
  }

  // Get the parameters.
  $lng = check_plain($_REQUEST['lng']);
  if ($lng == '') {
    drupal_set_message(t("The language parameter is required"), 'error');
    drupal_goto();
  }
  else if (!qtr::lng_check($lng)) {
    drupal_set_message(t("The language '!lng' does not exist.", ['!lng' => "$lng"]), 'error');
    drupal_goto();
  }

  // Get the option 'chapter'
  $chapter = check_plain($_REQUEST['chapter']);
  if ($chapter != '' && ($chapter < 1 || $chapter > 114)) {
    drupal_set_message(t("Chapter '!chapter' is out of range.", ['!chapter' => $chapter]), 'error');
    drupal_goto();
  }

  // Get the export mode.
  $mode = check_plain($_REQUEST['mode']);
  if (empty($mode))  $mode = 'most_liked';
  if ($mode != 'most_liked' && $mode != 'preference') {
    drupal_set_message(t("Unknown export mode '!mode'.", ['!mode' => $mode]), 'error');
    drupal_goto();
  }

  // Get and check the list of preferred voters.
  $users = check_plain($_REQUEST['users']);
  if ($mode == 'preference') {
    if (empty($users)) {
      if (user_is_anonymous()) {
        drupal_set_message(t("No users given."), 'error');
        drupal_goto();
      }
      else {
        $users = $GLOBALS['user']->name;
      }
    }
    list($emails, $messages) = qtr::utils_get_emails($users);
    if (!empty($messages)) {
      foreach ($messages as $msg) {
        drupal_set_message($msg[0], 'error');
      }
      drupal_goto();
    }
  }

  // Export translations of the project.
  set_time_limit(0);
  $file = qtr::export($lng, $chapter, $mode, $emails);

  // Send the headers.
  $filename = ($chapter ? "quran-$lng.$chapter.txt" : "quran-$lng.txt");
  header('Content-Type: text/plain; charset=utf-8');
  header('Content-Disposition: attachment; filename="' . $filename . '"');
  header('Content-Transfer-Encoding: binary');
  header("Content-Length: " . filesize($file));

  // Open the file in a binary mode and dump it to the browser.
  $fp = fopen($file, 'rb');
  fpassthru($fp);

  // Cleanup temporary file.
  unlink($file);

  // Stop execution.
  lock_release('export');
  drupal_exit();
}
