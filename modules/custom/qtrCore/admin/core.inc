<?php
/**
 * @file
 * Administrative pages for the qtrCore module.
 */

/**
 * General configuration settings for the module qtrCore.
 *
 * @return
 *   An array containing form items to place on the module settings page.
 */
function qtrCore_config() {

  $form['config'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Q-Translate Configuration'),
  );

  // qtr_languages
  $form['config']['qtr_languages'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Translation Languages'),
    '#default_value' => variable_get('qtr_languages', 'en'),
    '#description'   => t('The list of translation languages supported by the server. Enter a space separated list of language codes.'),
  );

  // qtr_client
  $form['config']['qtr_client'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Client URL'),
    '#default_value' => variable_get('qtr_client', 'https://qtranslate.net'),
    '#description'   => t('URL of the default client.'),
  );

  $form = system_settings_form($form);
  $form['#submit'][] = 'qtrCore_config_set_languages';
  return $form;
}

/**
 * Update cached values of languages.
 */
function qtrCore_config_set_languages() {
  // Update 'allowed_values' of 'field_translation_lng'.
  $info = field_info_field('field_translation_lng');
  $info['settings']['allowed_values'] = qtr::lng_get_list();
  field_update_field($info);
}
