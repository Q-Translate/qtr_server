<?php
/**
 * @file
 * Drush commands for importing/exporting *.po files.
 */

require_once(dirname(__FILE__) . '/qtr-import.inc');
require_once(dirname(__FILE__) . '/qtr-export.inc');

/**
 * Implementation of hook_drush_command().
 *
 * @return
 *   An associative array describing commands.
 */
function qtranslate_drush_command() {
  $items = array(
    'qtr-import' => _qtr_import_drush_command(),
    'qtr-export' => _qtr_export_drush_command(),
  );
  return $items;
}

/**
 * Implementation of hook_drush_help().
 *
 * @param
 *   A string with the help section (prepend with 'drush:')
 *
 * @return
 *   A string with the help text for your command.
 */
function qtranslate_drush_help($section) {
  switch ($section) {
    case 'drush:qtr-import':
      return _qtr_import_drush_help();
    case 'drush:qtr-export':
      return _qtr_export_drush_help();

    case 'meta:qtranslate:title':
      return dt("Q-Translate Commands");
    case 'meta:qtranslate:summary':
      return dt("Import and export Quran translations.");
  }
}
