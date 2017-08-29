<?php
/**
 * @file
 * Function user_send_mail()
 */

namespace QTranslate;

/**
 * Return true if we can send notification emails to the given account.
 */
function user_send_mail($account) {
  if (is_int($account)) {
    $account = user_load($account);
  }
  else {
    // Invalid account parameter.
    return FALSE;
  }

  // Don't send to admin.
  if ($account->uid < 2)     return FALSE;

  // Don't send to the disabled accounts,
  if ($account->status != 1) return FALSE;

  // Don't send to users that have never logged in.
  if ($account->login == 0)  return FALSE;

  return TRUE;
}
