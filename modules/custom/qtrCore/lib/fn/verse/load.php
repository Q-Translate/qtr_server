<?php
/**
 * @file
 * Function: verse_load()
 */

namespace QTranslate;
use \qtr;

/**
 * Get details for a list of verses.
 *
 * @param $arr_vid
 *   List of verse IDs to be loaded.
 *
 * @param $lng
 *   Language of translations.
 *
 * @return
 *   An array of verses, translations and likes, where each verse
 *   is an associative array, with translations and likes as nested
 *   associative arrays.
 */
function verse_load($arr_vid, $lng) {
  $query = qtr::db_select('qtr_verses', 'v')
    ->fields('v', array('vid'))
    ->condition('v.vid', $arr_vid, 'IN');

  return qtr::verse_details($query, $lng);
}
