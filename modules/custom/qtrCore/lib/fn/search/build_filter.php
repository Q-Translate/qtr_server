<?php
/**
 * @file
 * Function: search_build_filter()
 */

namespace QTranslate;
use \qtr;

/**
 * Build the filter data from the given params.
 *
 * Check and sanitize the data, and put default values where missing.
 *
 * @param $params
 *   Assoc array with filter parameters.
 *
 * @return
 *   Assoc array with filter data.
 */
function search_build_filter($params) {
  // Get language.
  $filter['lng'] = $params['lng'];

  // Number of results to be returned.
  $limit = isset($params['limit']) ? (int)trim($params['limit']) : 5;
  if ($limit < 1)  $limit = 1;
  if ($limit > 100) $limit = 100;
  $filter['limit'] = $limit;

  // Search can be done either by similarity of words, or by matching words
  // according to a certain logic.
  $type = isset($params['type']) ? $params['type'] : '';
  $filter['type'] = in_array($type, ['similar', 'logical']) ? $type : 'similar';

  // Search can be performed either on translations or on the verses.
  $what = isset($params['what']) ? $params['what'] : '';
  $filter['what'] = in_array($what, ['translations', 'verses']) ? $what : 'translations';

  // Words to search for and chapter to search in.
  $filter['words'] = isset($params['words']) ? $params['words'] : '';
  $filter['chapter'] = isset($params['chapter']) ? $params['chapter'] : '';

  // Limit search only to the strings touched (translated or liked)
  // by the current user.
  $filter['only_mine'] = isset($params['only_mine']) && (int)$params['only_mine'] ? 1 : 0;

  // Limit search by the editing users (used by admins).
  $filter['translated_by'] = isset($params['translated_by']) ? trim($params['translated_by']) : '';
  $filter['liked_by'] = isset($params['liked_by']) ? trim($params['liked_by']) : '';

  // Limit by date of translation or likes (used by admins).
  $date_filter_options = array('translations', 'likes');
  $date_filter = isset($params['date_filter']) ? trim($params['date_filter']) : '';
  $filter['date_filter'] = in_array($date_filter, $date_filter_options) ? $date_filter : 'translations';

  // from_date
  $filter['from_date'] = isset($params['from_date']) ? trim($params['from_date']) : '';

  // to_date
  $filter['to_date'] = isset($params['to_date']) ? trim($params['to_date']) : '';

  return $filter;
}
