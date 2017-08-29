<?php
/**
 * @file
 * Function: utils_get_client_url()
 */

namespace QTranslate;

/**
 * Return the url of the client site of a language, or the default client url
 * if the site of that language is not defined.
 */
function utils_get_client_url($lng) {
  $sites = qtr_get_sites();
  if ( isset($sites[$lng]['url']) ) {
    $site_url = $sites[$lng]['url'];
  }
  else {
    $site_url = variable_get('qtr_client', 'https://qtranslate.net');
  }

  return $site_url;
}

/**
 * Return an array of sites for each language
 * and their metadata (like url etc.)
 */
function qtr_get_sites() {
  return [
    'en' => [
      //'url' => 'https://en.qtranslate.net',
    ],
    'sq' => [
      //'url' => 'https://sq.qtranslate.net',
    ],
  ];
}
