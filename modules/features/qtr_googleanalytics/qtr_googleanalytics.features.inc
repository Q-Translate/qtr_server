<?php
/**
 * @file
 * qtr_googleanalytics.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function qtr_googleanalytics_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
