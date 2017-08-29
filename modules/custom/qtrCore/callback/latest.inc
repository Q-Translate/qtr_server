<?php
/**
 * @file
 * Return the latest translations in JSON format.
 */

/**
 * Return the latest translations in JSON format.
 *
 * If the option $days is given, return all the translations since that many
 * days ago. Otherwise, only the translations since yesterday.
 *
 * If the option $limit is given, then return that many translations from the
 * latest ones, regardless of their time. This takes precedence over the option
 * $days.
 *
 * Each of the returned translations has these fields:
 *   chapter_id, verse_nr, vid, lng, translation, tguid, time, username, usermail
 *
 * Results will be cached for 2 hours.
 */
function qtrCore_latest($lng ='en', $days = NULL, $limit = NULL) {
  // Get the data from cache, if possible.
  $cache_id = "qtrCore_latest:$lng:$days:$limit";
  $cache = cache_get($cache_id, 'cache_qtrCore');
  if (!empty($cache) && isset($cache->data) && !empty($cache->data)) {
    $latest = $cache->data;
  }
  else {
    // Get the latest translations from the database.
    $latest = qtr::translation_latest($lng, $days, $limit);
    // Cache them for 2 hours.
    cache_set($cache_id, $latest, 'cache_qtrCore', strtotime('+2 hours'));
  }

  // Output latest translations in JSON format and exit.
  print drupal_json_encode($latest);
  exit();
}
