<?php
/**
 * @file
 * Function: import_chapters()
 */

namespace QTranslate;
use \qtr;

/**
 * Import the chapters of the Quran from the given xml data file.
 */
function import_chapters($file) {
  $quran = simplexml_load_file($file) or die("Error: Cannot load xml file: $file\n");

  qtr::db_query('TRUNCATE qtr_chapters')->execute();;
  foreach($quran->suras->sura as $chapter) {
    qtr::db_insert('qtr_chapters')
      ->fields(array(
          'cid' => $chapter['index'],
          'verses' => $chapter['ayas'],
          'start' => $chapter['start'],
          'name' => $chapter['name'],
          'tname' => $chapter['tname'],
        ))
      ->execute();
  }
}
