#!/bin/bash -x
### Set the admin password of Drupal.

source /host/settings.sh
admin_pass=${1:-$ADMIN_PASS}
drush @qtr user-password admin --password="$admin_pass"
