<?php
/**
 * @file
 * Resource definition and callbacks for translations.
 */

function _qtrServices_translations() {
  $file = array('type' => 'inc', 'module' => 'qtrServices', 'name' => 'resources/translations');

  // Return a verse, along with its translations,
  // likes and other details.
  $translations['operations']['retrieve'] = array(
    'help' => 'Return a verse, along with its translations, likes and other details.',
    'file' => $file,
    'callback' => '_qtrServices_translations_retrieve',
    'args' => array(
      array(
        'name' => 'selector',
        'optional' => FALSE,
        'source' => array('path' => 0),
        'description' => 'Translation selector, in the form lng:chapter:verse',
        'type' => 'string',
      ),
    ),
    'access callback' => 'user_access',
    'access arguments' => array('access content'),
  );

  // Search verses and translations
  // according to the given parameters.
  $translations['operations']['index'] = array(
    'help' => 'Search verses and translations.',
    'file' => $file,
    'callback' => '_qtrServices_translations_index',
    'args' => array(
      array(
        'name' => 'params',
        'optional' => FALSE,
        'source' => 'param',
        'description' => 'Filter parameters (lng, words, etc.)',
        'type' => 'array',
      ),
    ),
    'access callback' => 'user_access',
    'access arguments' => array('access content'),
  );

  // Get a verse, along with its translations, likes and other details.
  $translations['actions']['get'] = array(
    'help' => 'Return a verse, along with its translations, likes and other details.',
    'file' => $file,
    'callback' => '_qtrServices_translations_get',
    'args' => array(
      array(
        'name' => 'params',
        'optional' => FALSE,
        'source' => 'data',
        'description' => 'Parameters (lng, chapter, verse).',
        'type' => 'array',
      ),
    ),
    'access callback' => 'user_access',
    'access arguments' => array('access content'),
  );

  // Search verses and translations using various filters.
  $translations['actions']['search'] = array(
    'help' => 'Search verses and translations.',
    'file' => $file,
    'callback' => '_qtrServices_translations_search',
    'args' => array(
      array(
        'name' => 'params',
        'optional' => FALSE,
        'source' => 'data',
        'description' => 'Filters (lng, words, project, etc.)',
        'type' => 'array',
      ),
    ),
    'access callback' => 'user_access',
    'access arguments' => array('access content'),
  );

  // Add a new translation to a verse.
  $translations['actions']['add'] = array(
    'help' => 'Add a new translation to a verse.',
    'file' => $file,
    'callback' => '_qtrServices_translations_add',
    'args' => array(
      array(
        'name' => 'params',
        'optional' => FALSE,
        'source' => 'data',
        'description' => 'Translation data (lng, chapter, verse, translation).',
        'type' => 'array',
      ),
    ),
    'access callback' => 'user_access',
    'access arguments' => array('qtranslate-translate'),
  );

  // Add a like for a translation.
  $translations['actions']['like'] = array(
    'help' => 'Add a like for a translation.',
    'file' => $file,
    'callback' => '_qtrServices_translations_like',
    'args' => array(
      array(
        'name' => 'params',
        'optional' => FALSE,
        'source' => 'data',
        'description' => 'Translation that is liked (tguid).',
        'type' => 'array',
      ),
    ),
    'access callback' => 'user_access',
    'access arguments' => array('qtranslate-like'),
  );

  // Delete a translation.
  $translations['actions']['del'] = array(
    'help' => 'Delete a translation.',
    'file' => $file,
    'callback' => '_qtrServices_translations_del',
    'args' => array(
      array(
        'name' => 'params',
        'optional' => FALSE,
        'source' => 'data',
        'description' => 'Translation (tguid) to be deleted.',
        'type' => 'array',
      ),
    ),
    'access callback' => 'user_access',
    'access arguments' => array('qtranslate-translate'),
  );

  // Submit multiple actions at once.
  $translations['actions']['submit'] = array(
    'help' => 'Submit multiple actions at once.',
    'file' => $file,
    'callback' => '_qtrServices_translations_submit',
    'args' => array(
      array(
        'name' => 'params',
        'optional' => FALSE,
        'source' => 'data',
        'description' => 'Array of actions to be executed.',
        'type' => 'array',
      ),
    ),
    'access callback' => 'user_access',
    'access arguments' => array('qtranslate-like'),
  );

  // Import translations from file.
  $translations['actions']['import'] = array(
    'help' => 'Import translations from file.',
    'file' => $file,
    'callback' => '_qtrServices_import',
    'args' => array(
      array(
        'name' => 'params',
        'optional' => FALSE,
        'source' => 'data',
        'description' => 'Assoc array of parameters.',
        'type' => 'array',
      ),
    ),
    'access callback' => 'user_access',
    'access arguments' => array('qtranslate-like'),
  );

  return $translations;
}

/**
 * Get a verse, along with its translations, likes and other details.
 *
 * @param $selector
 *   Translation selector, in the form 'lng:chapter:verse'
 *
 * @return
 *   A nested structure containing the verse, its
 *   translations, likes, etc.
 *
 * @example
 *   curl -q -k -H 'Accept: application/json' \
 *        'https://qtranslate.org/api/translations/en:2:3'
 */
function _qtrServices_translations_retrieve($selector) {
  list($lng, $chapter, $verse) = explode(':', $selector, 3);
  $verse = _qtrServices_translations_get(array(
               'lng' => $lng,
               'chapter' => $chapter,
               'verse' => $verse,
             ));
  return $verse;
}


/**
 * Get a verse, along with its translations, likes and other details.
 *
 * @param $params
 *   Associative array of the POST data, which contains:
 *   - lng
 *       The language of translations, like: 'en', 'fr', 'sq', etc.
 *   - chapter
 *       Chapter number.
 *   - verse
 *       Verse number.
 *
 * @return
 *   A nested array containing the verse, its translations, likes, etc.
 *
 * @example
 *   curl -H 'Content-type: application/json' \
 *     -X POST https://qtranslate.org/api/translations/get
 *     -d '{"lng": "en", "chapter": "2", "verse": "3"}'
 */
function _qtrServices_translations_get($params) {
  $lng = $params['lng'];
  $chapter = $params['chapter'];
  $verse = $params['verse'];

  if (!qtr::lng_check($lng)) {
    return services_error('Language missing or not supported.', 406);
  }
  if ($chapter < 1 or $chapter > 114)
    return services_error('Chapter must be between 1 and 114.', 406);
  $vid = qtr::verse_get_vid($chapter, $verse);
  if (!$vid)
    return services_error('The verse number is not correct.', 406);

  $verses = qtr::verse_load([$vid], $lng);
  $verse = empty($verses) ? NULL : $verses[0];

  return [ 'verse' => $verse ];
}


/**
 * Filter verses and translations according to the given parameters.
 *
 * This provides the same functionality and has the same parameters
 * as the operation 'translations/search'. The only difference is that
 * the parameters are provided as a GET request (in the URL), whereas
 * the parameters of 'translations/search' are provided as a POST
 * request. Sometimes this may be more convenient.
 *
 * @param $params
 *   Parameters for filtering the verses etc.
 *
 * @return
 *   Array containing search results, along with the filters
 *   and the pager info (same as for the search case).
 *
 * @example
 *   curl -H 'Accept: application/json' \
 *     'https://qtranslate.org/api/translations?lng=en&words=unseen'
 */
function _qtrServices_translations_index($params) {
  return _qtrServices_translations_search($params);
}


/**
 * Search verses and translations using various filters.
 *
 * @param $params
 *   Associative array of the POST data, which contains
 *   the filter parameters. These parameters can be:
 *   - lng
 *       The language of translations.
 *   - limit (int)
 *       The number of results to be returned (min: 5, max: 50).
 *   - words
 *       Words to be searched for.
 *   - chapter (int)
 *       Chapter to search on.
 *   - type
 *       Type of search:
 *       - similar  Search words by similarity.
 *       - logical  Search using logical (boolean) rules.
 *   - what
 *       What to search.
 *       - translations  Search translations.
 *       - verses        Search verses.
 *   - page
 *       Page of results to be displayed.
 *   - only_mine (boolean)
 *       Limit search only to the strings touched (translated or liked)
 *       by the current user.
 *   - translated_by
 *       Limit search by the author of translations
 *       (can be used only by admins).
 *   - liked_by
 *       Limit search by a user (can be used only by admins).
 *   - date_filter
 *       Which date to filter (used only by admins). Can be one of:
 *       - translations  Filter Translations By Date (default)
 *       - likes         Filter Likes By Date
 *   - from_date
 *   - to_date
 *
 * @return
 *   Array containing search results, along with the filters
 *   and the pager info. Each result is a nested structure
 *   containing the string, its translations, likes, etc.
 */
function _qtrServices_translations_search($params) {
  // Get and check the parameter 'lng'.
  $lng = $params['lng'];
  if (!qtr::lng_check($lng)) {
    return services_error('Language missing or not supported.', 406);
  }

  // The PagerDefault of Drupal7 queries gets the current page
  // from $_GET['page']. This is a trick to make it work.
  // See: https://api.drupal.org/api/drupal/includes%21pager.inc/function/pager_find_page/7
  $_GET['page'] = isset($params['page']) ? (int)trim($params['page']) : 0;

  // Build the filter and the query, and then get the strings.
  $filter = qtr::search_build_filter($params);
  $query = qtr::search_build_query($filter);
  $verses= ($query===NULL ? array() : qtr::verse_details($query, $lng));

  // The PagerDefault of Drupal7 defines the pager variables
  // as global variables (these variables are later used by theme_pager).
  // See: https://api.drupal.org/api/drupal/includes!pager.inc/function/pager_default_initialize/7
  $pager = array(
    'number_of_items' => $GLOBALS['pager_total_items'][0],
    'items_per_page' => $GLOBALS['pager_limits'][0],
    'number_of_pages' => $GLOBALS['pager_total'][0],
    'current_page' => $GLOBALS['pager_page_array'][0],
  );

  return array(
    'filter' => $filter,
    'pager' => $pager,
    'verses' => $verses,
  );
}


/**
 * Add a new translation to a verse.
 *
 * @param $params
 *   Associative array of the POST data, which contains:
 *   - lng
 *       The language (code) of the new translation.
 *   - chapter
 *       Chapter number.
 *   - verse
 *       Verse number.
 *   - translation
 *       The new translation as a string.
 *
 * @return
 *   Associative array containing:
 *   - tguid
 *       ID of the new translation, or NULL if no translation was added.
 *   - messages
 *       Array of notification messages; each notification message
 *       is an array of a message and a type, where type can be one of
 *       'status', 'warning', 'error'.
 */
function _qtrServices_translations_add($params) {
  // get parameters and validate them
  $lng = $params['lng'];
  $chapter = $params['chapter'];
  $verse = $params['verse'];
  $translation = $params['translation'];

  if (!qtr::lng_check($lng)) {
    return services_error('Language missing or not supported.', 406);
  }
  if ($chapter < 1 or $chapter > 114)
    return services_error('Chapter must be between 1 and 114.', 406);
  $vid = qtr::verse_get_vid($chapter, $verse);
  if (!$vid)
    return services_error('The verse number is not correct.', 406);

  // Check access permissions.
  if (!user_access('qtranslate-translate')) {
    $msg = t('No rights for making translations!');
    return ['tguid' => NULL, 'messages' => [[$msg, 'error']]];
  }
  // Check that the language matches the translation language of the user.
  $account = user_load($GLOBALS['user']->uid);
  if (!user_access('qtranslate-admin') and ($lng != $account->translation_lng)) {
    $msg = t('No rights for contributing to language <strong>!lng</strong>.', ['!lng' => $lng]);
    return ['tguid' => NULL, 'messages' => [[$msg, 'error']]];
  }

  // Add the translation.
  $tguid = qtr::translation_add($vid, $lng, $translation);
  return ['tguid' => $tguid, 'messages' => qtr::messages()];
}


/**
 * Delete a translation.
 *
 * @param $params
 *   Associative array of the POST data, which contains:
 *   - tguid
 *       ID of the translation
 *
 * @return
 *   Associative array containing:
 *   - messages
 *       Array of notification messages; each notification message
 *       is an array of a message and a type, where type can be one of
 *       'status', 'warning', 'error'.
 */
function _qtrServices_translations_del($params) {
  $tguid = $params['tguid'];
  qtr::translation_del($tguid);
  return ['messages' => qtr::messages()];
}


/**
 * Add a like for the given translation.
 *
 * @param $params
 *   Associative array of the POST data, which contains:
 *   - tguid
 *       ID of the translation
 *
 * @return
 *   Associative array containing:
 *   - lid
 *       ID of the new like, or NULL
 *   - messages
 *       Array of notification messages; each notification message
 *       is an array of a message and a type, where type can be one of
 *       'status', 'warning', 'error'.
 */
function _qtrServices_translations_like($params) {
  $tguid = $params['tguid'];

  // Check access permissions.
  if (!user_access('qtranslate-like')) {
    $msg = t('No rights for submitting likes!');
    return ['vid' => NULL, 'messages' => [[$msg, 'error']]];
  }

  // Add the like.
  $lid = qtr::like_add($tguid);
  return ['lid' => $lid, 'messages' => qtr::messages()];
}


/**
 * Submit multiple actions at once.
 *
 * @param $params
 *   Array of the POST data, which contains an item for each action.
 *   Each action is an associated array that contains the items:
 *   - action
 *       The action to be done (add, like, del)
 *   - params
 *       Associative array of the needed parameters for this action.
 *
 * @return
 *   Associative array containing:
 *   - messages
 *       Array of notification messages; each notification message
 *       is an array of a message and a type, where type can be one of
 *       'status', 'warning', 'error'.
 */
function _qtrServices_translations_submit($params) {
  $nr_likes = $nr_translations = $nr_deletions = 0;
  foreach ($params as $item) {
    $action = $item['action'];
    if (!$action) continue;
    switch ($action) {
    case 'add':
      $result = _qtrServices_translations_add($item['params']);
      if ($result['tguid'] != NULL)  $nr_translations++;
      qtr::messages($result['messages']);
      break;

    case 'like':
      $result = _qtrServices_translations_like($item['params']);
      if ($result['vid'] != NULL)  $nr_likes++;
      qtr::messages($result['messages']);
      break;

    case 'del':
      $result = _qtrServices_translations_del($item['params']);
      if (sizeof($result['messages'])==0) {
        $nr_deletions++;
      }
      else {
        qtr::messages($result['messages']);
      }
      break;

    default:
      $msg = t('Action !action is not supported!', ['!action' => $action]);
      qtr::messages($msg, 'error');
      break;
    }
  }

  if ($nr_translations > 0) {
    $msg = format_plural($nr_translations, '1 translation saved', '@count translations saved');
    qtr::messages($msg);
  }
  if ($nr_likes > 0) {
    $msg = format_plural($nr_likes, '1 like saved', '@count likes saved');
    qtr::messages($msg);
  }
  if ($nr_deletions > 0) {
    $msg = format_plural($nr_deletions, '1 translation deleted', '@count translations deleted');
    qtr::messages($msg);
  }

  return ['messages' => qtr::messages()];
}

/**
 * Import translations from file.
 *
 * @param $params
 *   Associative array of the POST data, which contains:
 *   - lng
 *       Language of translations.
 *   - author
 *       Author of translations (username).
 *
 * @return
 *   Associative array containing:
 *   - messages
 *       Array of notification messages; each notification message
 *       is an array of a message and a type, where type can be one of
 *       'status', 'warning', 'error'.
 */
function _qtrServices_import($params) {
  // Get and check the parameter 'lng'.
  $lng = $params['lng'];
  if (!qtr::lng_check($lng)) {
    return services_error('Language missing or not supported.', 406);
  }

  // Get the author of translations.
  if (user_access('qtranslate-admin')) {
    $author = $params['author'];
    $uid = ( $author ?
           qtr::user_add($author, $lng) :
           $uid = $GLOBALS['user']->uid );
  }
  elseif (user_access('qtranslate-import')) {
    $uid = $GLOBALS['user']->uid;
  }
  else {
    $msg = t('Only admins and translators can import translations.');
    return ['messages' => [[$msg, 'error']]];
  }
  $is_translator = ($uid == $GLOBALS['user']->uid);

  // Make sure that the user can make
  // translations for the given language.
  $user = user_load($GLOBALS['user']->uid);
  if ($lng != $user->translation_lng) {
    $msg = t('No rights for contributing to language <strong>!lng</strong>.', ['!lng' => $lng]);
    return ['messages' => [[$msg, 'error']]];
  }

  // Get the parameters.
  if (!isset($_FILES['file'])) {
    return services_error('No file uploaded.', 406);
  }
  $uploaded_file = $_FILES['file'];

  // Check that a file is uploaded.
  if (!$uploaded_file['tmp_name']) {
    return services_error('File upload failed.', 406);
  }

  // Check for any other upload errors.
  if ($uploaded_file['error'] != 0) {
    return services_error('Error ' . $uploaded_file['error'] . 'happened during file upload.', 406);
  }

  // Move the uploaded file to 'private://' (/var/www/uploads/).
  $file_uri = 'private://' . $uploaded_file['name'];
  if (!drupal_move_uploaded_file($uploaded_file['tmp_name'], $file_uri)) {
    $msg = [t('Failed to move uploaded file.'), 'error'];
    return ['messages' => [$msg]];
  }

  // Save the file data to the DB.
  $file = new stdClass();
  $file->uid = $GLOBALS['user']->uid;
  $file->status = FILE_STATUS_PERMANENT;
  $file->filename = trim(drupal_basename($uploaded_file['name']), '.');
  $file->uri = $file_uri;
  $file->filemime = file_get_mimetype($file->filename);
  $file->filesize = $uploaded_file['size'];
  try {
    $file = file_save($file);
  }
  catch (Exception $e) {
    $msg = [$e->getMessage(), 'error'];
    return ['messages' => [$msg]];
  }

  // Schedule the import.
  $queue_params = [
    'uid' => $uid,
    'fid' => $file->fid,
    'lng' => $lng,
    'is_translator' => $is_translator,
  ];
  qtr::queue('import', array($queue_params));

  // Return a notification message.
  $msg = t("Import of file '!filename' is scheduled. You will be notified by email when it is done.",
         ['!filename' => $file->filename]);
  return ['messages' => [[$msg, 'status']]];
}
