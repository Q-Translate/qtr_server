<?php
/**
 * @file
 * Function: search_build_query()
 */

namespace QTranslate;
use \qtr;

/**
 * Build the query that selects the verses that match the given filter.
 *
 * This query should return only the id-s of the matching verses and
 * the matching scores, ordered by the score in decreasing order.
 *
 * It should be something like this:
 *
 *    SELECT v.vid,
 *           MAX(MATCH (t.translation) AGAINST (:words)) AS score
 *    FROM {qtr_verses} v
 *    LEFT JOIN {qtr_translations} t ON (v.vid = t.vid)
 *    LEFT JOIN . . . . .
 *    . . . . . . . . . .
 *    WHERE (t.lng = :lng)
 *      AND (MATCH (t.translation) AGAINST (:words IN BOOLEAN MODE))
 *    . . . . . . . . . .
 *    GROUP BY v.vid
 *    ORDER BY score DESC
 *    LIMIT :limit;
 *
 * Tables that are joined and the select conditions are based on the
 * values of the filter.
 *
 * @param $filter
 *   Filter conditions that should be matched.
 *   It is an associated array with these keys:
 *      lng, limit, mode, words, chapter, only_mine, translated_by,
 *      liked_by, date_filter, from_date, to_date
 *
 * @return
 *   A query object that corresponds to the filter.
 *   NULL if there is nothing to select.
 */
function search_build_query($filter) {
  // Store the value of the language.
  _lng($filter['lng']);

  $query = qtr::db_select('qtr_verses', 'v')
    ->extend('PagerDefault')->limit($filter['limit']);
  $query->addField('v', 'vid');
  $query->groupBy('v.vid');

  _filter_by_content($query, $filter['type'], $filter['what'], $filter['words']);
  _filter_by_chapter($query, $filter['chapter']);
  _filter_by_author($query, $filter['lng'], $filter['only_mine'], $filter['translated_by'], $filter['liked_by']);
  _filter_by_date($query, $filter['date_filter'], $filter['from_date'], $filter['to_date']);

  // If nothing has been selected yet, then return NULL.
  if (sizeof($query->conditions()) == 1) return NULL;

  // Display results in verse order (unless order by translation time has been
  // specified above, which will take precedence).
  $query->orderBy('v.vid');

  // qtr::log(_get_query_string($query), '$query');  //debug
  return $query;
}

/**
 * Return the query as a string (for debug).
 */
function _get_query_string(\SelectQueryInterface $query) {
  $string = (string) $query;
  $arguments = $query->arguments();

  if (!empty($arguments) && is_array($arguments)) {
    foreach ($arguments as $placeholder => &$value) {
      if (is_string($value)) {
        $value = "'$value'";
      }
    }
    $arguments += ['{' => '', '}' => ''];

    $string = strtr($string, $arguments);
  }

  return $string;
}

/**
 * Keep and return the value of language (instead of using a global variable).
 */
function _lng($lang = NULL) {
  static $lng = NULL;
  if ($lang !== NULL) {
    $lng = $lang;
  }
  return $lng;
}

/**
 * Apply to the query conditions related to the content
 * (of strings and translations).
 * The first parameter, $query, is an object, so it is
 * passed by reference.
 */
function _filter_by_content($query, $type, $what, $search_words) {

  // If there are no words to be searched for, no condition can be added.
  if (trim($search_words) == '') {
    $query->addExpression('1', 'score');
    return;
  }

  // Get the match condition and the score field.
  $in_boolean_mode = ($type=='logical' ? ' IN BOOLEAN MODE' : '');
  if ($what=='verses') {
    $query->addExpression('MATCH (v.verse) AGAINST (:words)', 'score');
    $query->where(
      'MATCH (v.verse) AGAINST (:words' . $in_boolean_mode . ')',
      array(':words' => $search_words)
    );
  }
  else {   // ($content=='translations')
    _join_table($query, 'translations');
    $query->addExpression('MAX(MATCH (t.translation) AGAINST (:words))', 'score');
    $query->where(
      'MATCH (t.translation) AGAINST (:words' . $in_boolean_mode . ')',
      array(':words' => $search_words)
    );
  }

  // Order results by the field score.
  $query->orderBy('score', 'DESC');
}

/**
 * Apply to the query conditions related to chapter.
 * The first parameter, $query, is an object, so it is passed by reference.
 */
function _filter_by_chapter($query, $chapter) {
  if ($chapter == '')  return;
  _join_table($query, 'chapters');
  $query->condition('c.cid', $chapter);
}

/**
 * Apply to the query conditions related to authors.
 * The first parameter, $query, is an object, so it is passed by reference.
 */
function _filter_by_author($query, $lng, $only_mine, $translated_by, $liked_by) {

  if ($only_mine) {
    _join_table($query, 'likes');

    global $user;
    $umail = $user->init;  // initial mail used for registration
    $query->condition(db_or()
      ->condition(db_and()
        ->condition('t.umail', $umail)
        ->condition('t.ulng', $lng)
      )
      ->condition(db_and()
        ->condition('l.umail', $umail)
        ->condition('l.ulng', $lng)
      )
    );
    //done, ignore $translated_by and $liked_by
    return;
  }

  //get the umail for $translated_by and $liked_by
  $get_umail = 'SELECT umail FROM {qtr_users} WHERE name = :name AND ulng = :ulng';
  $args = array();
  if ($translated_by == '') $t_umail = '';
  else {
    $account = user_load_by_name($translated_by);
    $args[':ulng'] = $account->translation_lng;
    $args[':name'] = $translated_by;
    $t_umail = qtr::db_query($get_umail, $args)->fetchField();
  }
  if ($liked_by == '') $l_umail = '';
  else {
    $account = user_load_by_name($liked_by);
    $args[':ulng'] = $account->translation_lng;
    $args[':name'] = $liked_by;
    $l_umail = qtr::db_query($get_umail, $args)->fetchField();
  }

  //if it is the same user, then search for strings
  //translated OR liked by this user
  if ($t_umail==$l_umail and $t_umail!='') {
    _join_table($query, 'likes');
    $query->condition(db_or()
      ->condition(db_and()
        ->condition('t.umail', $t_umail)
        ->condition('t.ulng', $lng)
      )
      ->condition(db_and()
        ->condition('l.umail', $l_umail)
        ->condition('l.ulng', $lng)
      )
    );
    return;
  }

  //if the users are different, then search for strings
  //translated by $t_umail AND liked by $l_umail
  if ($t_umail != '') {
    _join_table($query, 'translations');
    $query->condition('t.umail', $t_umail)->condition('t.ulng', $lng);
  }
  if ($l_umail != '') {
    _join_table($query, 'likes');
    $query->condition('l.umail', $l_umail)->condition('l.ulng', $lng);
  }
}

/**
 * Apply to the query conditions related to translation or like dates.
 *
 * The first parameter, $query, is an object, so it is passed
 * by reference.
 *
 * $date_filter has one of the values ('strings', 'translations', 'likes')
 */
function _filter_by_date($query, $date_filter, $from_date, $to_date) {
  // If both dates are empty, there is no condition to be added.
  if ($from_date == '' and $to_date == '')  return;

  //if the date of translations or likes has to be checked,
  //then the corresponding tables must be joined
  if ($date_filter == 'translations') {
    _join_table($query, 'translations');
  }
  elseif ($date_filter == 'likes') {
    _join_table($query, 'likes');
  }

  //get the alias (name) of the date field that has to be checked
  if  ($date_filter=='likes')    $field = 'l.time';
  else                           $field = 't.time';

  //add to query the propper date condition;
  //if none of the dates are given, no condition is added
  if ($from_date != '' and $to_date != '') {
    $query->condition($field, array($from_date, $to_date), 'BETWEEN');
    $query->orderBy($field, 'DESC');
  }
  elseif ($from_date == '' and $to_date != '') {
    $query->condition($field, $to_date, '<=');
    $query->orderBy($field, 'DESC');
  }
  elseif ($from_date != '' and $to_date == '') {
    $query->condition($field, $from_date, '>=');
    $query->orderBy($field, 'DESC');
  }
  else {
    //do nothing
  }

}

/**
 * Add a join for the given table to the $query.
 * Make sure that the join is added only once (by using tags).
 * $table can be one of: translations, likes, chapters
 */
function _join_table($query, $table) {
  $tag = "join-$table";
  if ($query->hasTag($tag))  return;
  $query->addTag($tag);

  switch ($table) {
    case 'translations':
      $query->leftJoin("qtr_translations", 't', 'v.vid = t.vid AND t.lng = :lng', [':lng' => _lng()]);
      break;
    case 'likes':
      _join_table($query, 'translations');
      $query->leftJoin("qtr_likes", 'l', 'l.tguid = t.tguid');
      break;
    case 'chapters':
      $query->leftJoin("qtr_chapters", 'c', 'c.cid = v.cid');
      break;
    default:
      debug("Error: _join_table(): table '$table' is unknown.");
      break;
  }
}
