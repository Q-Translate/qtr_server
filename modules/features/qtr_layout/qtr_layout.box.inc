<?php
/**
 * @file
 * qtr_layout.box.inc
 */

/**
 * Implements hook_default_box().
 */
function qtr_layout_default_box() {
  $export = array();

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'boxabout';
  $box->plugin_key = 'simple';
  $box->title = '';
  $box->description = 'About Q-Translate';
  $box->options = array(
    'body' => array(
      'value' => '<p><strong><a href="http://info.qtranslate.org" target="_blank">Q-Translate</a></strong> aims to improve the translations of the Quran, by getting review and feedback from many people. This may help to double check the translations, fix any mistakes or typos, and improve the translation quality. The original text of the Quran is downloaded from <a href="http://tanzil.net/download/" target="_blank">http://tanzil.net/download/</a> and the translations from <a href="http://tanzil.net/trans/" target="_blank">http://tanzil.net/trans/</a></p>',
      'format' => 'full_html',
    ),
    'additional_classes' => '',
  );
  $export['boxabout'] = $box;

  return $export;
}
