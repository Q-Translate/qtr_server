#!/bin/bash
### Update all projects at once.

for dir in /var/www/qtr*
do
    echo
    echo "===> $dir"
    cd $dir
    drush --yes vset update_check_disabled 1
    drush pm-refresh
    drush up
done
