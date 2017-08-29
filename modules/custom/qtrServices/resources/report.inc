<?php
/**
 * @file
 * Resource definition and callbacks for report.
 */

function _qtrServices_report() {
  $file = array('type' => 'inc', 'module' => 'qtrServices', 'name' => 'resources/report');
  $access = array(
    'access callback' => 'user_access',
    'access arguments' => array('access content'),
  );

  // Getting reports of different kinds.
  $report['operations']['retrieve'] = array(
    'help' => 'Reports of different kinds.',
    'file' => $file,
    'callback' => '_qtr_report_retrieve',
    'args' => array(
      array(
        'name' => 'report',
        'optional' => FALSE,
        'source' => array('path' => 0),
        'description' => 'The name/id of the report.',
        'type' => 'string',
      ),
      array(
        'name' => 'params',
        'optional' => FALSE,
        'source' => 'param',
        'description' => 'Parameters of the report.',
        'type' => 'array',
      ),
    ),
  ) + $access;

  // Getting general statistics (number of likes and
  // translations) for the last week, month and year.
  $report['actions']['stats'] = array(
    'help' => 'General statistics (number of likes and translations) for the last week, month and year.',
    'file' => $file,
    'callback' => '_qtr_report_stats',
    'args' => array(
      array(
        'name' => 'params',
        'optional' => FALSE,
        'source' => 'data',
        'description' => 'The parameters of the report.',
        'type' => 'array',
      ),
    ),
  ) + $access;

  // Getting statistics about top contributing users.
  $report['actions']['topusers'] = array(
    'help' => 'Statistics about top contributing users.',
    'file' => $file,
    'callback' => '_qtr_report_topusers',
    'args' => array(
      array(
        'name' => 'params',
        'optional' => FALSE,
        'source' => 'data',
        'description' => 'The parameters of the report.',
        'type' => 'array',
      ),
    ),
  ) + $access;

  return $report;
}

/**
 * Getting reports of different kinds. Anybody can
 * retrieve them, there are no access restrictions.
 *
 * @param $report
 *   The name/id of the report.
 *
 * @param $params
 *   Parameters of the report, comming from the
 *   GET request. They depend on the kind of report.
 *
 * @return
 *   The data of the requested report.
 *
 * @example
 *   curl 'https://dev.qtranslate.org/api/report/stats?lng=en'
 *   curl 'https://dev.qtranslate.org/api/report/topusers?lng=en&period=week'
 */
function _qtr_report_retrieve($report, $params) {
  $report_function = '_qtr_report_' . $report;
  if (! function_exists($report_function) ) {
    return services_error("Unknown report: $report", 406);
  }
  return $report_function($params);
}

/**
 * Getting general statistics (number of likes and
 * translations) for the last week, month and year.
 *
 * @param $params
 *   Parameters of the report, comming from the POST request.
 *   It contains these fields (variables):
 *   - lng: language of translations
 *
 * @return
 *   Array of general stats for week, month and year.
 *
 * @example
 *   curl -H "Content-type: application/json"  \
 *        -X POST https://dev.qtranslate.org/api/report/stats \
 *        -d '{"lng": "en"}'
 */
function _qtr_report_stats($params) {
  $params += array(
    'lng' => NULL,
  );

  // get parameters and validate them
  $lng = $params['lng'];
  if (!$lng)  $lng = 'all';
  if (!in_array($lng, qtr::lng_get()) and $lng != 'all') {
    return services_error('Language missing or not supported.', 406);
  }

  return qtr::report_stats($lng);
}

/**
 * Getting statistics about top contributing users.
 *
 * @param $params
 *   Parameters of the report, comming from the POST request.
 *   It contains these fields (variables):
 *   - lng: language of translations
 *   - period: period of report: day | week | month | year
 *   - size: number of results (top contributers), between 5 and 100
 *
 * @return
 *   Array of users, where each user is an object with these attributes:
 *   uid, name, umail, score, translations, likes
 *
 * @example
 *   curl -H "Content-type: application/json"  \
 *     -X POST https://dev.qtranslate.org/api/report/topusers \
 *     -d '{"lng": "en", "period": "week", "size": 10}'
 */
function _qtr_report_topusers($params) {
  $params += array(
    'lng' => NULL,
    'period' => 'week',
    'size' => 5,
  );

  // get parameters and validate them
  $period = $params['period'];
  if (!in_array($period, array('day', 'week', 'month', 'year'))) {
    return services_error('Period missing or not supported.', 406);
  }
  $size = (int)$params['size'];
  if ($size < 5 or $size > 100) {
    return services_error('Size must be between 5 and 100.', 406);
  }
  $lng = $params['lng'];
  if (!$lng)  $lng = 'all';
  if (!in_array($lng, qtr::lng_get()) and $lng != 'all') {
    return services_error('Language missing or not supported.', 406);
  }

  return qtr::report_topusers($period, $size, $lng);
}
