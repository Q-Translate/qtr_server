<?php
/**
 * @file
 * Function: verse_get()
 */

namespace QTranslate;
use \qtr;

/**
 * Get a verse from its ID.
 */
function verse_get($vid) {
  $verse = qtr::db_query(
    'SELECT verse FROM {qtr_verses} WHERE vid = :vid',
    array(':vid' => $vid)
  )->fetchField();
  return $verse;
}
