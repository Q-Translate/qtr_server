<?php
/**
 * @file
 * Function: import_translations()
 */

namespace QTranslate;
use \qtr;

/**
 * Import translations of the Quran from the given text data file.
 *
 * @param $lng
 *   The language (code) of the translation.
 *
 * @param $file
 *   The text file of the translations,
 *   having each translation in a separate line.
 *
 * @param $uid
 *   Id of the user that is importing these translations.
 */
function import_translations($lng, $file, $uid = NULL, $notime = FALSE) {
  $handle = fopen($file, 'r') or die("Error: Cannot open file: $file\n");

  // Get the email of the author of the translation, etc.
  $uid = qtr::user_check($uid);
  $account = user_load($uid);
  $umail = ($uid==1 ?  $umail = '' : $account->init);
  $time = ($notime ? NULL : date('Y-m-d H:i:s', REQUEST_TIME));

  // Get a list of chapters and their structure.
  $chapters = qtr::db_query('SELECT * FROM {qtr_chapters}')->fetchAllAssoc('cid');

  $count = 0;  // used to show progress outout
  while(!feof($handle)){
    $line = fgets($handle);
    $line = trim($line);
    if ($line == '')  continue;

    // Show progress output.
    $count++;
    if ($count % 20 == 1) print ".";

    // Get chapter id, verse nr and the translation.
    list($chapter_id, $verse_nr, $translation) = explode('|', $line, 3);
    if (!is_numeric($chapter_id)) continue;
    if (!is_numeric($verse_nr)) continue;

    // Check the chapter and verse numbers.
    if ($chapter_id < 1 || $chapter_id > 114) {
      qtr::messages(t("Chapter id !cid is not between 1 and 114.", ['!cid' => $chapter_id]), 'error');
      continue;
    }
    if ($verse_nr < 1 || $verse_nr > $chapters[$chapter_id]->verses) {
      qtr::messages(t("Verse number !nr is not valid.", ['!nr' => $verse_nr]), 'error');
      continue;
    }

    // Get the verse id and translation id.
    $vid = $chapters[$chapter_id]->start + $verse_nr;
    $tguid = sha1($translation . $lng . $vid);

    // Check whether such a translation exists already.
    $exists = qtr::db_query(
      'SELECT tguid FROM {qtr_translations} WHERE tguid = :tguid',
      [':tguid' => $tguid]
    )->fetchField();

    // If it exists, just add a like for it.
    if ($exists) {
      qtr::like_add($tguid, $uid, !$notime);
      continue;
    }

    // Add a new translation.
    $fields = array(
      'tguid' => $tguid,
      'vid' => $vid,
      'lng' => $lng,
      'translation' => $translation,
      'count' => 1,
      'umail' => $umail,
      'ulng' => $lng,
    );
    if ($time != NULL)  $fields['time'] = $time;
    qtr::db_insert('qtr_translations')->fields($fields)->execute();

    // Clean any previous like.
    include_once(drupal_get_path('module', 'qtrCore') . '/lib/fn/like/del_previous.php');
    _like_del_previous($tguid, $umail, $vid, $lng);

    // Add a like for it.
    $fields = array(
      'tguid' => $tguid,
      'umail' => $umail,
      'ulng' => $lng,
    );
    if ($time != NULL)  $fields['time'] = $time;
    qtr::db_insert('qtr_likes')->fields($fields)->execute();
  }
  fclose($handle);
}
