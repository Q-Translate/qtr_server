<?php
/**
 * @file
 * qtr_qtr.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function qtr_qtr_default_rules_configuration() {
  $items = array();
  $items['rules_assign_contributor_role'] = entity_import('rules_config', '{ "rules_assign_contributor_role" : {
      "LABEL" : "Assign contributor role",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules" ],
      "ON" : { "user_insert" : [] },
      "DO" : [
        { "user_add_role" : { "account" : [ "account" ], "roles" : { "value" : { "4" : "4" } } } }
      ]
    }
  }');
  return $items;
}
