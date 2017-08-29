<?php
/**
 * @file
 * qtr_qtrServices.services.inc
 */

/**
 * Implements hook_default_services_endpoint().
 */
function qtr_qtrServices_default_services_endpoint() {
  $export = array();

  $endpoint = new stdClass();
  $endpoint->disabled = FALSE; /* Edit this to true to make a default endpoint disabled initially */
  $endpoint->api_version = 3;
  $endpoint->name = 'qtranslate';
  $endpoint->server = 'rest_server';
  $endpoint->path = 'api';
  $endpoint->authentication = array(
    'oauth2_server' => array(
      'server' => 'oauth2',
    ),
  );
  $endpoint->server_settings = array(
    'formatters' => array(
      'bencode' => TRUE,
      'json' => TRUE,
      'jsonp' => TRUE,
      'php' => TRUE,
      'xml' => TRUE,
    ),
    'parsers' => array(
      'application/json' => TRUE,
      'application/vnd.php.serialized' => TRUE,
      'application/x-www-form-urlencoded' => TRUE,
      'application/xml' => TRUE,
      'multipart/form-data' => TRUE,
      'text/xml' => TRUE,
    ),
  );
  $endpoint->resources = array(
    'report' => array(
      'operations' => array(
        'retrieve' => array(
          'enabled' => '1',
          'settings' => array(
            'oauth2_server' => array(
              'scope' => '',
              'require_authentication' => NULL,
            ),
          ),
        ),
      ),
      'actions' => array(
        'stats' => array(
          'enabled' => '1',
          'settings' => array(
            'oauth2_server' => array(
              'scope' => '',
              'require_authentication' => NULL,
            ),
          ),
        ),
        'topusers' => array(
          'enabled' => '1',
          'settings' => array(
            'oauth2_server' => array(
              'scope' => '',
              'require_authentication' => NULL,
            ),
          ),
        ),
      ),
    ),
    'translations' => array(
      'operations' => array(
        'retrieve' => array(
          'enabled' => '1',
          'settings' => array(
            'oauth2_server' => array(
              'scope' => '',
              'require_authentication' => NULL,
            ),
          ),
        ),
        'index' => array(
          'enabled' => '1',
          'settings' => array(
            'oauth2_server' => array(
              'scope' => '',
              'require_authentication' => NULL,
            ),
          ),
        ),
      ),
      'actions' => array(
        'search' => array(
          'enabled' => '1',
          'settings' => array(
            'oauth2_server' => array(
              'scope' => '',
              'require_authentication' => NULL,
            ),
          ),
        ),
        'get' => array(
          'enabled' => '1',
          'settings' => array(
            'oauth2_server' => array(
              'scope' => '',
              'require_authentication' => NULL,
            ),
          ),
        ),
        'add' => array(
          'enabled' => '1',
          'settings' => array(
            'oauth2_server' => array(
              'require_authentication' => '1',
              'scope' => '',
            ),
          ),
        ),
        'like' => array(
          'enabled' => '1',
          'settings' => array(
            'oauth2_server' => array(
              'require_authentication' => '1',
              'scope' => '',
            ),
          ),
        ),
        'del' => array(
          'enabled' => '1',
          'settings' => array(
            'oauth2_server' => array(
              'require_authentication' => '1',
              'scope' => '',
            ),
          ),
        ),
        'submit' => array(
          'enabled' => '1',
          'settings' => array(
            'oauth2_server' => array(
              'require_authentication' => '1',
              'scope' => '',
            ),
          ),
        ),
        'import' => array(
          'enabled' => '1',
          'settings' => array(
            'oauth2_server' => array(
              'require_authentication' => '1',
              'scope' => '',
            ),
          ),
        ),
      ),
    ),
    'user' => array(
      'actions' => array(
        'profile' => array(
          'enabled' => '1',
          'settings' => array(
            'oauth2_server' => array(
              'require_authentication' => '1',
              'scope' => '',
            ),
          ),
        ),
      ),
    ),
    'user_profile' => array(
      'operations' => array(
        'index' => array(
          'enabled' => '1',
          'settings' => array(
            'oauth2_server' => array(
              'require_authentication' => '1',
              'scope' => '',
            ),
          ),
        ),
      ),
    ),
  );
  $endpoint->debug = 0;
  $export['qtranslate'] = $endpoint;

  return $export;
}
