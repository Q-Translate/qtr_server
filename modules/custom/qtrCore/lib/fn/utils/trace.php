<?php
/**
 * @file
 * Function: utils_trace()
 */

namespace QTranslate;
use \qtr;

/**
 * Output the call stack up to the point where this function is called.
 */
function utils_trace($comment ='') {
  $e = new Exception();
  $trace = $e->getTraceAsString();
  $trace = preg_replace('/^#0 .*/', '', $trace);
  $trace = str_replace(DRUPAL_ROOT.'/', '', $trace);
  qtr::log($trace, $comment);
}
