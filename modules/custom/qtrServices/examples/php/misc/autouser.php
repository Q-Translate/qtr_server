<?php
$path = dirname(dirname(__FILE__));
include_once($path . '/config.php');
include_once($path . '/http_request.php');

// Autocomplete strings.
http_request('https://dev.qtranslate.org/auto/user/en/u');
