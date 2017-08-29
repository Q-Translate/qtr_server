#!/bin/bash -x

### make a clone of /var/www/qtr to /var/www/qtr_dev
/usr/local/src/qtr_server/ds/cfg/dev/clone.sh qtr qtr_dev

### add a test user
drush @qtr_dev user-create user1 --password=pass1 \
      --mail='user1@example.org' > /dev/null 2>&1
