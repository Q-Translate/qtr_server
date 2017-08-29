<?php
/**
 * @file
 * Get an array of chapters and their details.
 */

namespace QTranslate;
use \qtr;

/**
 * Get an array of chapters and their details.
 *
 * @return
 *   Array of chapters and their details.
 */
function chapters() {
  $chapters = qtr::db_query('SELECT * FROM {qtr_chapters}')->fetchAll();
  array_unshift($chapters, NULL);
  return $chapters;
}
