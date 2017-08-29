<?php
/**
 * @file
 * Function: translation_latest()
 */

namespace QTranslate;
use \qtr;

/**
 * Get and return an array of the latest translation suggestions.
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
 */
function translation_latest($lng = NULL, $days = NULL, $limit = NULL) {

  if ($days == NULL) $days = 1;

  $get_latest_translations = "
    SELECT v.cid AS chapter_id, v.nr AS verse_nr, v.vid,
           t.lng, t.translation, t.tguid, t.time,
           u.name AS username, u.umail AS usermail
    FROM {qtr_translations} t
    LEFT JOIN {qtr_verses} v ON (v.vid = t.vid)
    LEFT JOIN {qtr_users} u ON (u.umail = t.umail AND u.ulng = t.ulng)
    LEFT JOIN {qtr_chapters} c ON (c.cid = v.cid)
    WHERE t.umail != ''";
  $args = array();

  if ($lng && $lng != 'all') {
    $get_latest_translations .= " AND t.lng = :lng";
    $args[':lng'] = $lng;
  }

  if ($limit == NULL) {
    $get_latest_translations .= " AND t.time > :from_date ORDER BY t.time DESC";
    $args[':from_date'] = date('Y-m-d', strtotime("-$days days"));
  }
  else {
    $get_latest_translations .= " ORDER BY t.time DESC LIMIT $limit";
  }

  // run the query and get the translations
  $translations = qtr::db_query($get_latest_translations, $args)->fetchAll();

  return $translations;
}
