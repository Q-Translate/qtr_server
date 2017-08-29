<?php

/**
 * Menu callback for 'chapters'.
 * Returns a json list of chapters and their details.
 */
function qtrCore_chapters() {
  drupal_json_output(qtr::chapters());
  drupal_exit();
}
