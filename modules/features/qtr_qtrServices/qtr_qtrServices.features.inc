<?php
/**
 * @file
 * qtr_qtrServices.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function qtr_qtrServices_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "services" && $api == "services") {
    return array("version" => "3");
  }
}
