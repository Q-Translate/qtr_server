#!/bin/bash

### go to the script directory
cd $(dirname $0)

### create the DB tables
mysql=$(drush sql-connect)
$mysql < db/qtr_schema.sql

### import chapters and verses
data=$(pwd)
drush eval "qtr::import_chapters('$data/quran/index.xml');"
drush eval "qtr::import_verses('$data/quran/uthmani.xml');"

### add a few english translations
drush qtr-import en en.pickthall "$data/test/en.pickthall.txt" --force
drush qtr-import en en.maududi "$data/test/en.maududi.txt" --force
drush qtr-import en en.sahih "$data/test/en.sahih.txt" --force
drush qtr-import en en.itani "$data/test/en.itani.txt" --force
