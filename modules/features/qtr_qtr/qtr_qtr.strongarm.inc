<?php
/**
 * @file
 * qtr_qtr.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function qtr_qtr_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'user_admin_role';
  $strongarm->value = '3';
  $export['user_admin_role'] = $strongarm;

  return $export;
}
