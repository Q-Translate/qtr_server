<?php
$path = dirname(dirname(__FILE__));
include_once($path . '/config.php');
include_once($path . '/http_request.php');

// GET api/report/statistics
$url = $base_url . '/api/report/stats?lng=en';
$result = http_request($url);

// POST api/report/statistics
$url = $base_url . '/api/report/stats';
$options = array(
  'method' => 'POST',
  'data' => array('lng' => 'en'),
  'headers' => array(
    'Content-type' => 'application/x-www-form-urlencoded',
  ),
);
$result = http_request($url, $options);
