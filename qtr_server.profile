<?php
/**
 * @file
 * Installation steps for the profile qtr_server.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * Allows the profile to alter the site configuration form.
 */
function qtr_server_form_install_configure_form_alter(&$form, $form_state) {
  // Pre-populate the site name with the server name.
  $form['site_information']['site_name']['#default_value'] = 'Qtr_server';
}

/**
 * Implements hook_install_tasks().
 */
function qtr_server_install_tasks($install_state) {
  // Add our custom CSS file for the installation process
  drupal_add_css(drupal_get_path('profile', 'qtr_server') . '/qtr_server.css');

  module_load_include('inc', 'phpmailer', 'phpmailer.admin');
  //module_load_include('inc', 'qtr_server', 'qtr_server.admin');

  $tasks = array(
    'qtr_server_mail_config' => array(
      'display_name' => st('Mail Settings'),
      'type' => 'form',
      'run' => INSTALL_TASK_RUN_IF_NOT_COMPLETED,
      'function' => 'phpmailer_settings_form',
    ),
    /*
    'qtr_server_config' => array(
      'display_name' => st('Qtr_server Settings'),
      'type' => 'form',
      'run' => INSTALL_TASK_RUN_IF_NOT_COMPLETED,
      'function' => 'qtr_server_config',
    ),
    */
  );

  return $tasks;
}
