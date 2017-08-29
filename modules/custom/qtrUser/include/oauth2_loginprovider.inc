<?php
/**
 * @file
 * Customizing the user profile that is served by oauth2_loginprovider.
 */

/**
 * Implements hook_oauth2_loginprovider_userprofile_alter().
 */
function qtrUser_oauth2_loginprovider_userprofile_alter(&$data) {
  // Remove uninteresting fields.
  unset($data['uuid']);
  unset($data['disqus']);
  unset($data['rdf_mapping']);

  // These fields are duplicated
  // (without 'field_' and without the deep array structure).
  unset($data['field_translation_lng']);

  // Add to profile data the Q-Translate permissions of the user.
  $data['permissions'] = _qtrUser_get_qtr_permissions();
}

/**
 * Return an array of the Q-Translate permissions of the current user.
 */
function _qtrUser_get_qtr_permissions() {
  global $user;
  $perms_arr = user_role_permissions($user->roles);
  $all_perms = array();
  foreach ($perms_arr as $rid => $perms) {
    $all_perms += $perms;
  }
  $all_perms = array_keys($all_perms);
  $qtr_perms = array_keys(module_invoke('qtrUser', 'permission'));
  $perms = array_intersect($all_perms, $qtr_perms);

  return array_values($perms);
}
