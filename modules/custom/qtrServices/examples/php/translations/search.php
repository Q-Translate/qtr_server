<?php
$path = dirname(dirname(__FILE__));
include_once($path . '/config.php');
include_once($path . '/http_request.php');

// POST api/translations/search
$url = $base_url . '/api/translations/search';
$options = array(
  'method' => 'POST',
  'data' => array(
    'lng' => 'en',
    'words' => 'unseen',
    'chapter' => 2,
  ),
  'headers' => array(
    'Content-type' => 'application/x-www-form-urlencoded',
  ),
);
$result = http_request($url, $options);
