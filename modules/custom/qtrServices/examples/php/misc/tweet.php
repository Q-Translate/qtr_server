<?php
$path = dirname(dirname(__FILE__));
include_once($path . '/config.php');
include_once($path . '/http_request.php');

function link_to($url) {
    print("<li>Click: <a href='$url' target='_blank'>$url</a></li>");
};

print "<br/><hr/><br/>\n";

// Get a random translation.
link_to('https://qtranslate.org/tweet/en');
link_to('https://qtranslate.net/qtr/tweet/en');

http_request('https://qtranslate.org/tweet/en',
  ['headers' => ['Accept' => 'application/json']]);
