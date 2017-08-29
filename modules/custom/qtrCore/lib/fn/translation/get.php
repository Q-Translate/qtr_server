<?php
/**
 * @file
 * Function: translation_get()
 */

namespace QTranslate;
use \qtr;

/**
 * Return a translation from its ID.
 */
function translation_get($tguid) {
  $translation = qtr::db_query(
    'SELECT translation FROM {qtr_translations} WHERE tguid = :tguid',
    array(':tguid' => $tguid)
  )->fetchField();
  return $translation;
}
