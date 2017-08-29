<?php
/**
 * @file
 * Function: report_stats()
 */

namespace QTranslate;
use \qtr;

/**
 * Return an array of the statistics (number of likes
 * and translations) for the last week, month and year.
 *
 * @param $lng
 *   Language of translations.
 *
 * @return
 *   Array of general stats for the last week, month and year.
 */
function report_stats($lng = NULL) {

  // validate parameters
  if (!in_array($lng, qtr::lng_get())) {
    $lng = 'all';
  }

  // Return cache if possible.
  $cid = "report_statistics:$lng";
  $cache = cache_get($cid, 'cache_qtrCore');
  if (!empty($cache) && isset($cache->data) && !empty($cache->data)) {
    return $cache->data;
  }

  // Get the query condition and the arguments.
  $condition = 'time >= :from_date';
  $args = array();
  if ($lng != 'all') {
    $condition .= ' AND ulng = :ulng';
    $args[':ulng'] = $lng;
  }

  // Get the count queries.
  $sql_count_translations =
     "SELECT count(*) as cnt FROM {qtr_translations}
      WHERE $condition";
  $sql_count_likes =
     "SELECT count(*) as cnt FROM {qtr_likes}
      WHERE $condition";

  // Get the stats.
  $stats = array();
  foreach (array('week', 'month', 'year') as $period) {
    $from_date = date('Y-m-d', strtotime("-1 $period"));
    $args[':from_date'] = $from_date;
    $nr_likes = qtr::db_query($sql_count_likes, $args)->fetchField();
    $nr_translations = qtr::db_query($sql_count_translations, $args)->fetchField();

    $stats[$period] = array(
      'period' => $period,
      'from_date' => $from_date,
      'nr_likes' => $nr_likes,
      'nr_translations' => $nr_translations,
    );
  }

  // Cache for 12 hours.
  cache_set($cid, $stats, 'cache_qtrCore', time() + 12*60*60);

  return $stats;
}
