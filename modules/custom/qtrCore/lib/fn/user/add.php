<?php
/**
 * @file
 * Function user_add()
 */

namespace QTranslate;

/**
 * Add a new user with the given username and translation_lng.
 * Return the uid of this user.
 */
function user_add($username, $lng = 'en') {
  // Check for an existing user.
  $user = user_load_by_name($username);
  if ($user)  return $user->uid;

  // Add a new user.
  $username = str_replace(' ', '', $username);
  $site_mail = variable_get('site_mail');
  $user_mail = preg_replace('/\+.*@/', '@', $site_mail);
  $user_mail = str_replace('@', "+${username}@", $user_mail);
  $new_user = array(
    'name' => $username,
    'mail' => $user_mail,
    'init' => $user_mail,
    'translation_lng' => $lng,
    'roles' => array(4),
    'status' => 1,
    'access' => time(),
  );
  $user = user_save(null, $new_user);
  return $user->uid;
}
