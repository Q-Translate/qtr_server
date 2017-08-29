<?php

/**
 * Menu callback for 'languages'.
 * Returns a json list of languages and their details.
 */
function qtrCore_languages() {
  $languages = qtr::lng_get_details();
  drupal_json_output($languages);
  drupal_exit();
}
