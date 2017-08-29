<?php
/**
 * @file
 * Form for backup/restore of Q-Translate data.
 */

/**
 * Backup and restore Q-Translate data.
 *
 * @return
 *   An array containing form items.
 */
function qtrCore_backup() {
  $form = [
    // Backup.
    'data_backup' => [
      '#type' => 'fieldset',
      '#title' => t('Backup Q-Translate Data'),
      '#description' => t('Backup users, translations and likes, etc.'),

      'submit' => [
        '#value' => t('Backup'),
        '#type' => 'submit',
      ],
    ],

    // Restore.
    'data_restore' => [
      '#type' => 'fieldset',
      '#title' => t('Restote Q-Translate Data'),
      '#description' => t('Restore users, translations and likes, etc.'),

      'file' => [
        '#type' => 'file',
        '#title' => t('Backup File'),
        '#description' => t('Upload a backup file.'),
      ],

      'submit' => [
        '#value' => t('Restore'),
        '#type' => 'submit',
      ],
    ],
  ];

  return $form;
}

/**
 * Implement of hook form_submit() for the form qtrCore_backup.
 */
function qtrCore_backup_submit($form, &$form_state) {
  $form_values = $form_state['values'];
  $op = $form_values['op'];

  if ($op == t('Backup')) {
    // Make a backup.
    _qtrCore_backup_make();
  }

  if ($op == t('Restore')) {
    // Restore a backup.
    _qtrCore_backup_restore();
  }
}

/**
 * Make a backup.
 */
function _qtrCore_backup_make() {
  // Allow the backup script to run until completion.
  set_time_limit(0);

  // Execute the backup script.
  $path = drupal_get_path('module', 'qtrCore');
  exec("$path/data/db/backup.sh 2>&1", $output);

  // Display any notification messages.
  if (!empty($output)) {
    foreach ($output as $line) {
      if (trim($line) == '') continue;
      if ($line == 'mysqldump: [Warning] Using a password on the command line interface can be insecure.') continue;
      drupal_set_message($line, 'error');
    }
  }

  // Get the name of the backup file.
  $date = date('Ymd');
  $backup_file = "/tmp/qtr-backup-$date.tgz";

  // Send the headers.
  $filename = basename($backup_file);
  header('Content-Type: application/x-compressed');
  header('Content-Disposition: attachment; filename="' . $filename . '"');
  header('Content-Transfer-Encoding: binary');
  header("Content-Length: " . filesize($backup_file));

  // Open the file in a binary mode and send it to the browser.
  $fp = fopen($backup_file, 'rb');
  fpassthru($fp);

  // Cleanup.
  unlink($backup_file);

  // Stop execution.
  drupal_exit();
}

/**
 * Restore a backup.
 */
function _qtrCore_backup_restore() {
  // Check that a file is uploaded.
  if (!$_FILES['files']['tmp_name']['file']) {
    drupal_set_message(t('No file is uploaded.'), 'error');
    return;
  }
  // Check for any other upload errors.
  if ($_FILES['files']['error']['file'] != 0) {
    drupal_set_message(t("Error !error happened during file upload.",
        ['!error' => $_FILES['files']['error']['file']]), 'error');
    return;
  }

  // Restore the backup.
  $filename = $_FILES['files']['tmp_name']['file'];
  $path = drupal_get_path('module', 'qtrCore');
  exec("$path/data/db/restore.sh $filename 2>&1", $output);

  // Display any notification messages.
  if (!empty($output)) {
    foreach ($output as $line) {
      if (trim($line) == '') continue;
      if ($line == 'mysql: [Warning] Using a password on the command line interface can be insecure.') continue;
      drupal_set_message($line, 'error');
    }
  }
  else {
    drupal_set_message(t('Backup restored successfully.'));
  }

  // Cleanup.
  unlink($filename);
}
