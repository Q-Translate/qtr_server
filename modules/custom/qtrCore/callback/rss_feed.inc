<?php
/**
 * @file
 * Menu callbacks.
 */

/**
 * Callback for 'rss-feed'.
 */
function qtrCore_rss_feed($lng = 'en') {
  // Get the feed data from cache, if possible.
  $cache = cache_get("qtrCore_rss_feed:$lng", 'cache_qtrCore');
  if (!empty($cache) && isset($cache->data) && !empty($cache->data)) {
    $rss_feed = $cache->data;
    drupal_add_http_header('Content-Type', 'application/rss+xml; charset=utf-8');
    print $rss_feed;
    return;
  }

  // Get the latest translations.
  $translations = qtr::translation_latest($lng);

  // Get the url of the translation site.
  module_load_include('inc', 'qtrCore', 'lib/sites');
  $client_url = qtr::utils_get_client_url($lng);

  // Start building the RSS feed output.
  $rss_feed = '<?xml version="1.0" encoding="utf-8"?>' . "\n";
  $rss_feed .= '<rss version="2.0">';

  $link = url("rss-feed/$lng", array('absolute' => TRUE));
  $title = variable_get('site_name', 'Q-Translate');
  $description = t('Latest translations on Q-Translate');
  $ttl = 180;
  $site_mail = variable_get('site_mail');
  $site_name = variable_get('site_name');
  $web_master = "$site_mail ($site_name)";

  $last_build_date = date('r');
  $rss_feed .= "
    <channel>
      <title>$title</title>
      <link>$link</link>
      <description>$description</description>
      <category>Q-Translate</category>
      <language>$lng</language>
      <ttl>$ttl</ttl>
      <webMaster>$web_master</webMaster>
      <lastBuildDate>$last_build_date</lastBuildDate>
    ";

  // Append an item for each translation.
  foreach ($translations as $trans) {
    extract(get_object_vars($trans));
    $title = "[$chapter_id:$verse_nr] " . qtr::utils_shorten($translation, 50);
    $link = $client_url . "/qtr/$lng/$chapter_id/$verse_nr";
    $description = "[$chapter_id:$verse_nr] $translation";
    $pub_date = date('r', strtotime($time));
    $rss_feed .= "
      <item>
        <title>$title</title>
        <link>$link</link>
        <description><![CDATA[$description]]></description>
        <author>$usermail ($username)</author>
        <guid isPermaLink='false'>$tguid</guid>
        <pubDate>$pub_date</pubDate>
      </item>
    ";
  }
  // Close the XML elements.
  $rss_feed .= "</channel>\n";
  $rss_feed .= "</rss>\n";

  // Cache for 2 hours.
  cache_set("qtrCore_rss_feed:$lng", $rss_feed, 'cache_qtrCore', strtotime('+2 hours'));

  // Send the output.
  drupal_add_http_header('Content-Type', 'application/rss+xml; charset=utf-8');
  print $rss_feed;
}
