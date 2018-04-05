#!/bin/bash -x

### make a clone of /var/www/qtr to /var/www/qtr_dev
/usr/local/src/qtr_server/ds/cfg/dev/clone.sh qtr qtr_dev

### comment out the configuration of the database 'qtr_data' so that
### the internal test database can be used instead for translations
sed -i /var/www/qtr_dev/sites/default/settings.php \
    -e '/$databases..qtr_data/,+8 s#^/*#//#'

### add a test user
drush @qtr_dev user-create user1 --password=pass1 \
      --mail='user1@example.org' > /dev/null 2>&1

### register a test oauth2 client on qtr_server
drush @qtr_dev oauth2-client-add test1 12345 \
    'https://qtranslate.net/oauth2-client-php/authorized.php'
