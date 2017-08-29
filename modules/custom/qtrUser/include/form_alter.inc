<?php
/**
 * @file
 * Alter user_profile_form, user_register_form, etc.
 */

/**
 * Implements hook_form_FORM_ID_alter() for user_register_form.
 */
function qtrUser_form_user_register_form_alter(&$form, &$form_state, $form_id) {
  // Hide the language field if the language is set in the url.
  if (isset($_GET['lng'])) {
      $form['field_translation_lng']['#access'] = FALSE;
  }
}

/**
 * Validation function for user_register_form.
 */
function _qtrUser_user_register_validate($form, &$form_state) {
  $values = &$form_state['values'];

  // Set the value for the language field.
  if (isset($_GET['lng'])) {
      $values['field_translation_lng']['und'][0]['value'] = $_GET['lng'];
  }
}
