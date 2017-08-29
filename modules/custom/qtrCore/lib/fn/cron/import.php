<?php
/**
 * @file
 * Function: cron_import()
 */

namespace QTranslate;
use \qtr;

/**
 * The callback function called from cron_queue 'import'.
 */
function cron_import($params) {

  // Make sure that imports do not run in parallel,
  // so that the server is not overloaded.
  if (!lock_acquire('import', 3000)) {
    // If we cannot get the lock, just stop the execution, do not return,
    // because after the callback function returns, the cron_queue will
    // remove the item from the queue, no matter whether it is processed or not.
    exit();
  }

  // Allow the import script to run until completion.
  set_time_limit(0);

  // Get the parameters.
  $lng = $params->lng;
  $account = user_load($params->uid);
  $file = file_load($params->fid);
  $dont_save_time = !$params->is_translator;

  // Copy the file to a tmp directory.
  $tmpdir = '/tmp/' . sha1_file($file->uri);
  mkdir($tmpdir, 0700);
  file_unmanaged_copy($file->uri, $tmpdir);
  $tmpfile =  $tmpdir . '/' . $file->filename;

  // Import the file.
  qtr::import_translations($lng, $tmpfile, $account->uid, $dont_save_time);
  $txt_messages = qtr::messages_cat(qtr::messages());

  // Get the url of the client site.
  module_load_include('inc', 'qtrCore', 'includes/sites');
  $client_url = qtr::utils_get_client_url($lng);

  // Notify the user that the import is done.
  if (qtr::user_send_mail($account)) {
    $params = array(
      'type' => 'notify-that-import-is-done',
      'uid' => $account->uid,
      'username' => $account->name,
      'recipient' => $account->name .' <' . $account->mail . '>',
      'filename' => $file->filename,
      'messages' => $txt_messages,
    );
    qtr::queue('notifications', array($params));
  }

  // Cleanup.
  exec("rm -rf $tmpdir/");
  file_delete($file, TRUE);

  // This import is done, allow any other imports to run.
  lock_release('import');
}
