<?php
namespace QTranslate;
use \qtr;

/**
 * Clean any previous like by this user for this translation.
 *
 * @param $tguid
 *   ID of the translation.
 *
 * @param $umail
 *   The mail of the user.
 *
 * @param $vid
 *   ID of the verse.
 *
 * @param $lng
 *   Language code of the translation.
 *
 * @return
 *   Number of previous likes that were deleted.
 *   (Normally should be 0, but can also be 1. If it is >1,
 *   something must be wrong.)
 */
function _like_del_previous($tguid, $umail, $vid, $lng) {

  // Get the other sibling translations (translations of the same
  // string and the same language) which the user has liked.
  $arr_tguid = qtr::db_query(
    'SELECT DISTINCT t.tguid
       FROM {qtr_translations} t
       LEFT JOIN {qtr_likes} l ON (l.tguid = t.tguid)
       WHERE t.vid = :vid
         AND t.lng = :lng
         AND l.umail = :umail
         AND l.ulng = :ulng',
    array(
      ':vid' => $vid,
      ':lng' => $lng,
      ':umail' => $umail,
      ':ulng' => $lng,
    ))
             ->fetchCol();

  if (empty($arr_tguid))  return 0;

  // Insert to the trash table the likes that will be removed.
  $query = qtr::db_select('qtr_likes', 'l')
    ->fields('l', array('lid', 'tguid', 'umail', 'ulng', 'time', 'active'))
    ->condition('umail', $umail)
    ->condition('ulng', $lng)
    ->condition('tguid', $arr_tguid, 'IN');
  $query->addExpression('NOW()', 'd_time');
  qtr::db_insert('qtr_likes_trash')->from($query)->execute();

  // Remove any likes by the user for each translation in $arr_tguid.
  $num_deleted = qtr::db_delete('qtr_likes')
    ->condition('umail', $umail)
    ->condition('ulng', $lng)
    ->condition('tguid', $arr_tguid, 'IN')
    ->execute();

  // Decrement the like count for each translation in $arr_tguid.
  $num_updated = qtr::db_update('qtr_translations')
    ->expression('count', 'count - 1')
    ->condition('tguid', $arr_tguid, 'IN')
    ->execute();

  return $num_deleted;
}
