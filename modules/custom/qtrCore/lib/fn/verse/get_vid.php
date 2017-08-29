<?php
/**
 * @file
 * Function: verse_get_vid()
 */

namespace QTranslate;
use \qtr;

/**
 * Get the vid from the chapter number and verse number.
 * Return NULL if chapter and verse numbers are not correct.
 */
function verse_get_vid($chapter, $verse) {
  if ($chapter < 1 or $chapter > 114) return NULL;
  if ($verse < 1) return NULL;
  $sql = 'SELECT * FROM {qtr_chapters} WHERE cid = :cid';
  $ch = qtr::db_query($sql, array(':cid' => $chapter))->fetchAssoc();
  if ($verse > $ch['verses']) return NULL;
  return $ch['start'] + $verse;
}
