<?php
/**
 * @file
 * Function: report_topusers()
 */

namespace QTranslate;
use \qtr;

/**
 * Return a list of top contributing users from the last period.
 *
 * @param $period
 *     Period of report: day | week | month | year.
 *
 * @param $size
 *     Number of top users to return.
 *
 * @param $lng
 *     Language of contributions.
 *
 * @return
 *     Array of users, where each user is an object
 *     with these attributes:
 *         uid, name, umail, score, translations, likes
 */
function report_topusers($period = 'week', $size = 5, $lng = NULL) {

  // validate parameters
  if (!in_array($lng, qtr::lng_get())) {
    $lng = 'all';
  }
  if (!in_array($period, array('day', 'week', 'month', 'year'))) {
    $period = 'week';
  }
  $size = (int) $size;
  if ($size < 5) $size = 5;
  if ($size > 100) $size = 100;

  // Return cache if possible.
  $cid = "report_topusers:$period:$size:$lng";
  $cache = cache_get($cid, 'cache_qtrCore');
  if (!empty($cache) && isset($cache->data) && !empty($cache->data)) {
    return $cache->data;
  }

  // Get the query condition and the arguments.
  $condition = 'time >= :from_date';
  $args[':from_date'] = date('Y-m-d', strtotime("-1 $period"));
  if ($lng != 'all') {
    $condition .= ' AND ulng = :lng';
    $args[':lng'] = $lng;
  }

  // Give weight 5 to a translation and weight 1 to a like,
  // get the sum of all the weights grouped by user (umail),
  // order by this score, and get the top users.
  $sql_get_topusers = "
    SELECT u.uid, u.name, u.umail, sum(w.weight) AS score,
           sum(w.translation) AS translations, sum(w.likes) AS likes
    FROM (
       (
         SELECT t.umail, t.lng as ulng,
                5 AS weight, 1 AS translation, 0 AS likes
         FROM {qtr_translations} t
         WHERE $condition
       )
       UNION ALL
       (
         SELECT l.umail, l.ulng,
                1 AS weight, 0 AS translation, 1 AS likes
         FROM {qtr_likes} l
         WHERE $condition
       )
    ) AS w
    LEFT JOIN {qtr_users} u
           ON (u.ulng = w.ulng AND u.umail = w.umail)
    WHERE u.name != 'admin'
    GROUP BY w.umail
    ORDER BY score DESC
  ";
  $topusers = qtr::db_query_range($sql_get_topusers, 0, $size, $args)->fetchAll();

  // Cache for 12 hours.
  cache_set($cid, $topusers, 'cache_qtrCore', time() + 12*60*60);

  return $topusers;
}
