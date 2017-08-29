<?php
/**
 * Used for debug.
 */

/**
 * Implements hook_boot().
 *
 * Outputs debug information to the file: /tmp/qtr.log
 */
function qtrCore_boot() {
  function _qtrCore_log($var, $comment ='') {
    $file = '/tmp/qtr.log';
    $content = "\n==> $comment: " . print_r($var, true);
    file_put_contents($file, $content, FILE_APPEND);
  }

  _qtrCore_log('= = = = = = = = = = = = = = = = = = = = = = =');
  _qtrCore_log($_GET, '$_GET');
  _qtrCore_log($_POST, '$_POST');
  //_qtrCore_log($_COOKIE, '$_COOKIE');

  if (isset($_SESSION['oauth2_client'])) {
    _qtrCore_log($_SESSION['oauth2_client'], '$_SESSION[oauth2_client]');
  }
  /*
  if (isset($_SESSION['qtrCore'])) {
    _qtrCore_log($_SESSION['qtrCore'], '$_SESSION[qtrCore]');
  }
  if (isset($_SESSION['qtrServices'])) {
    _qtrCore_log($_SESSION['qtrServices'], '$_SESSION[qtrServices]');
  }
  if (isset($_SESSION['qtr'])) {
    _qtrCore_log($_SESSION['qtr'], '$_SESSION[qtr]');
  }
  */
}
