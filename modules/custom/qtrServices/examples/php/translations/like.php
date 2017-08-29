<?php
$path = dirname(dirname(__FILE__));
include_once($path . '/config.php');
include_once($path . '/http_request.php');
include_once($path . '/get_access_token.php');

// Get a verse.
$url = $base_url . "/api/translations/get";
$options = array(
  'method' => 'POST',
  'data' => array(
    'lng' => 'en',
    'chapter' => 2,
    'verse' => 3,
  ),
  'headers' => array(
    'Content-type' => 'application/x-www-form-urlencoded',
    'Accept' => 'application/json',
  ),
);
$result = http_request($url, $options);

// Get the tguid of the first translation.
$tguid = $result['verse']['translations'][0]['tguid'];

// Get an access  token.
$access_token = get_access_token($auth);

// POST api/translations/like
$url = $base_url . '/api/translations/like';
$options = array(
  'method' => 'POST',
  'data' => array('tguid' => $tguid),
  'headers' => array(
    'Content-type' => 'application/x-www-form-urlencoded',
    'Authorization' => 'Bearer ' . $access_token,
  ),
);
$result = http_request($url, $options);

// Retrive the verse and check that the translation has been liked.
$url = $base_url . "/api/translations/get";
$options = array(
  'method' => 'POST',
  'data' => array(
    'lng' => 'en',
    'chapter' => 2,
    'verse' => 3,
  ),
  'headers' => array(
    'Content-type' => 'application/x-www-form-urlencoded',
    'Accept' => 'application/json',
  ),
);
$result = http_request($url, $options);
