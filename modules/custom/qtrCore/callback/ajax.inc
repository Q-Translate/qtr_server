<?php
/**
 * @file
 * AJAX callback functions.
 */

/**
 * Retrieve a JSON list of autocomplete suggestions for users.
 */
function qtrCore_user_autocomplete($lng, $str = '') {
  $matches = array();
  if ($str) {
    $query = "SELECT name FROM {qtr_users}
              WHERE name LIKE :str AND ulng = :ulng
              ORDER BY name";
    $args = array(
      ':str' => $str . '%',
      ':ulng' => $lng,
    );
    $result = qtr::db_query_range($query, 0, 10, $args);
    foreach ($result as $user) {
      $matches[$user->name] = check_plain($user->name);
    }
  }
  print drupal_json_encode($matches);
  exit();
}
