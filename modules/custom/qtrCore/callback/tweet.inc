<?php
/**
 * @file
 * Return a random translation in a suitable form for the twitter.
 *
 * If the request header 'Accept: application/json' is set, then
 * the output will be in JSON.  This can be useful for adding your custom
 * hashtags, url, etc.
 *
 * Examples:
 *   curl -q -k https://qtranslate.org/tweet/en
 *   curl -q -k -H 'Accept: application/json' \
 *        https://qtranslate.org/tweet/en
 */

function qtrCore_tweet($lng =NULL) {
  if (empty($lng))  $lng = 'en';

  // Get a random translation.
  $vid = rand(1, 6236);
  $verse = qtr::db_query(
    'SELECT * FROM {qtr_verses} WHERE vid = :vid',
    [':vid' => $vid]
  )->fetch();
  $cid = $verse->cid;
  $nr = $verse->nr;
  $translations = qtr::db_query(
    'SELECT translation FROM {qtr_translations} WHERE vid = :vid AND lng = :lng',
    [':vid' => $vid, ':lng' => $lng ]
  )->fetchCol();

  // Build the tweet.
  $i = rand(0, sizeof($translations) - 1);
  $tweet = $translations[$i];
  if (empty($tweet)) {
    $tweet = t('Error: No string found!');
  }
  else {
    if (strlen($tweet) > 80) {
      $tweet = substr($tweet, 0, strrpos(substr($tweet, 0, 80-3), ' '));
      $tweet .= '...';
    }
  }

  // If the request has the header 'Accept: application/json'
  // return the output in JSON format.
  if ($_SERVER['HTTP_ACCEPT'] == 'application/json') {
    //drupal_add_http_header('Content-Type', 'application/json; utf-8');
    print drupal_json_encode(array(
        'lng' => $lng,
        'chapter' => $cid,
        'verse' => $nr,
        'tweet' => $tweet,
      ));
    exit;
  }

  // Append hash tags, url, etc.
  $tweet = "[Quran $cid:$nr] $tweet #quran_$lng";
  if (isset($_GET['url'])) {
    $url = $_GET['url'];
    $tweet .= ' ' . $url;
  }
  else {
    module_load_include('inc', 'qtrCore', 'includes/sites');
    $client_url = qtr::utils_get_client_url($lng);
    $tweet .= ' ' . $client_url . "/qtr/$lng/$cid/$nr";
  }

  print $tweet;
  exit(0);
}
