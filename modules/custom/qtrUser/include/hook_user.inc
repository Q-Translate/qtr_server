<?php
/**
 * @file
 * Add hooks for user_load, user_insert, user_update, user_cancel, user_delete
 * and user_presave.
 */

/**
 * Implements hook_user_load().
 * Act on user objects when loaded from the database.
 */
function qtrUser_user_load($users) {
  foreach($users as $user) {
    _qtrUser_user_get_fields($user);
  }
}


/**
 * Implements hook_user_isert().
 * It is called when a new user account is created.
 */
function qtrUser_user_insert(&$edit, $account, $category) {
  _qtrUser_user_get_fields($account);
  _qtr_user_update(
    $umail = $account->init,  // initial email that the user used for registration
    $ulng = $account->translation_lng,
    $ulng_old = 'und',  // undefined
    $uid = $account->uid,
    $name = $account->name,
    $status = 1
  );
}


/**
 * Implements hook_user_update().
 * It is called after a user account has been updated.
 */
function qtrUser_user_update(&$edit, $account, $category) {
  _qtrUser_user_get_fields($account);
  _qtr_user_update(
    $umail = $account->init,
    $ulng = $account->translation_lng,
    $ulng_old = $account->original->translation_lng,
    $uid = $account->uid,
    $name = $account->name,
    $status = $account->status
  );
}


/**
 * Implements hook_user_cancel().
 */
function qtrUser_user_cancel($edit, $account, $category) {
  _qtrUser_user_get_fields($account);
  _qtr_user_disable(
    $umail = $account->init,
    $ulng = $account->translation_lng
  );
}


/**
 * Implements hook_user_delete().
 */
function qtrUser_user_delete($account) {
  _qtrUser_user_get_fields($account);
  _qtr_user_disable(
    $umail = $account->init,
    $ulng = $account->translation_lng
  );
}


/**
 * Implements hook_user_presave().
 * It is called before the user profile page is saved.
 */
function qtrUser_user_presave(&$edit, $account, $category) {
  _qtrUser_user_set_fields($edit, $account);
}


/**
 * Get the values of the fields and put them in the structure
 * of the $user.  This is called after a user account is loaded, in
 * hook_user_load().
 */
function _qtrUser_user_get_fields(&$user) {
  $lang = LANGUAGE_NONE;

  // field_translation_lng
  $user->translation_lng = 'en';
  if (isset($user->field_translation_lng[$lang][0]['value'])) {
    $user->translation_lng = $user->field_translation_lng[$lang][0]['value'];
  }
}

/**
 * Save the values of attributes in the structure of the $edit.
 * This is called before a user account is saved, in hook_user_presave().
 */
function _qtrUser_user_set_fields(&$edit, $account) {

  $lang = LANGUAGE_NONE;

  if (isset($edit['translation_lng'])) {
    $edit['field_translation_lng'][$lang][0]['value'] = $edit['translation_lng'];
  }
}


/**
 * It is called when a new user account is created,
 * or after a user account has been updated.
 *
 * We want to make sure that there is only one record
 * with the same (umail, ulng), so we delete first any
 * existing record like that, and then insert a new one.
 */
function _qtr_user_update($umail, $ulng, $ulng_old, $uid, $name, $status) {
  // Delete first any existing user which has the same umail and ulng.
  qtr::db_delete('qtr_users')
    ->condition('umail', $umail)
    ->condition(db_or()
      ->condition('ulng', $ulng)
      ->condition('ulng', $ulng_old)
    )
    ->execute();

  // Insert a new user with the given fields.
  qtr::db_insert('qtr_users')
    ->fields(array(
        'umail' => $umail,
        'ulng' => $ulng,
        'uid' => $uid,
        'name' => $name,
        'status' => $status,
      ))
    ->execute();
}


/**
 * It is called when a user is cancelled or deleted.
 */
function _qtr_user_disable($umail, $ulng) {
  qtr::db_update('qtr_users')
    ->fields(array(
        'status' => 0,
      ))
    ->condition('umail', $umail)
    ->condition('ulng', $ulng)
    ->execute();
}
