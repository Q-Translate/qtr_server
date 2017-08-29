<?php
/**
 * @file
 * qtr_hybridauth.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function qtr_hybridauth_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
