<?php
/**
 * @file
 * qtr_qtr.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function qtr_qtr_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'qtranslate-admin'.
  $permissions['qtranslate-admin'] = array(
    'name' => 'qtranslate-admin',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'qtrUser',
  );

  // Exported permission: 'qtranslate-flag'.
  $permissions['qtranslate-flag'] = array(
    'name' => 'qtranslate-flag',
    'roles' => array(
      'contributor' => 'contributor',
      'moderator' => 'moderator',
      'translator' => 'translator',
    ),
    'module' => 'qtrUser',
  );

  // Exported permission: 'qtranslate-import'.
  $permissions['qtranslate-import'] = array(
    'name' => 'qtranslate-import',
    'roles' => array(
      'translator' => 'translator',
    ),
    'module' => 'qtrUser',
  );

  // Exported permission: 'qtranslate-resolve'.
  $permissions['qtranslate-resolve'] = array(
    'name' => 'qtranslate-resolve',
    'roles' => array(
      'moderator' => 'moderator',
    ),
    'module' => 'qtrUser',
  );

  // Exported permission: 'qtranslate-translate'.
  $permissions['qtranslate-translate'] = array(
    'name' => 'qtranslate-translate',
    'roles' => array(
      'contributor' => 'contributor',
      'moderator' => 'moderator',
      'translator' => 'translator',
    ),
    'module' => 'qtrUser',
  );

  // Exported permission: 'qtranslate-like'.
  $permissions['qtranslate-like'] = array(
    'name' => 'qtranslate-like',
    'roles' => array(
      'contributor' => 'contributor',
      'moderator' => 'moderator',
      'translator' => 'translator',
    ),
    'module' => 'qtrUser',
  );

  return $permissions;
}
