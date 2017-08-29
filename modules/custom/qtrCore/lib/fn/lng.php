<?php
/**
 * @file
 * Get the languages that are supported by the application.
 */

namespace QTranslate;
use \qtr;

/**
 * Return TRUE if the given language is supported.
 */
function lng_check($lng) {
  return in_array($lng, qtr::lng_get());
}

/**
 * Get a list of supported languages.
 *
 * @return
 *   Array of language codes.
 */
function lng_get() {
  $langs = array();

  $qtr_langs = variable_get('qtr_languages', '');
  foreach (explode(' ', $qtr_langs) as $lng) {
    if ($lng == '')  continue;
    $langs[] = $lng;
  }

  if (empty($langs)) {
    $langs = array('en');
  }

  return $langs;
}

/**
 * Get a list of supported languages to be used in selection options.
 *
 * @return
 *   Associated array with language codes as keys and language names as
 *   values.
 */
function lng_get_list() {
  $list = array();
  $all_langs = \db_query('SELECT * FROM {qtrLanguages}')->fetchAllAssoc('code');
  $langs = qtr::lng_get();
  foreach ($langs as $lng) {
    $list[$lng] = isset($all_langs[$lng]) ? $all_langs[$lng]->name : $lng;
  }

  return $list;
}

/**
 * Get an array of the supported languages and their details.
 *
 * @return
 *   Associated array with language codes as keys and language details as
 *   values.
 */
function lng_get_details() {
  $lng_details = array();
  $all_langs = \db_query('SELECT * FROM {qtrLanguages}')->fetchAllAssoc('code');
  $langs = qtr::lng_get();
  foreach ($langs as $lng) {
    if (isset($all_langs[$lng])) {
      $lng_details[$lng] = array(
        'code' => $lng,
        'name' => $all_langs[$lng]->name,
        'direction' => $all_langs[$lng]->direction,
      );
    }
    else {
      $lng_details[$lng] = array(
        'code' => $lng,
        'name' => "Unknown ($lng)",
        'direction' => LANGUAGE_LTR,
      );
    }
  }

  return $lng_details;
}
