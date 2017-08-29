<?php
$path = dirname(dirname(__FILE__));
include_once($path . '/config.php');
include_once($path . '/http_request.php');

// GET api/report/topusers
$url = $base_url . '/api/report/topusers?lng=en&period=week';
$result = http_request($url);

// POST api/report/topusers
$url = $base_url . '/api/report/topusers';
$options = array(
  'method' => 'POST',
  'data' => array(
    'lng' => 'en',
    'period' => 'week',
  ),
  'headers' => array(
    'Content-type' => 'application/x-www-form-urlencoded',
  ),
);
$result = http_request($url, $options);
