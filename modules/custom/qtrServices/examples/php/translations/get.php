<?php
$path = dirname(dirname(__FILE__));
include_once($path . '/config.php');
include_once($path . '/http_request.php');

// POST api/translations/get
$url = $base_url . '/api/translations/get';
$options = array(
  'method' => 'POST',
  'data' => array(
    'lng' => 'en',
    'chapter' => 2,
    'verse' => 3,
  ),
  'headers' => array(
    'Content-type' => 'application/x-www-form-urlencoded',
  ),
);
$result = http_request($url, $options);
