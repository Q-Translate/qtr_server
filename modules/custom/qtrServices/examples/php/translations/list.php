<?php
$path = dirname(dirname(__FILE__));
include_once($path . '/config.php');
include_once($path . '/http_request.php');

// GET api/translations
http_request($base_url . '/api/translations?lng=en&words=unseen&chapter=2');
