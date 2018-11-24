#!/bin/bash -x

#source /host/settings.sh

restore_data() {
    drush @qtr sql-query --file=$(pwd)/qtr.sql
    $(drush @qtr sql-connect --database=qtr_data) < $(pwd)/qtr_data.sql
}

restore_config() {
    # enable features
    while read feature; do
        drush --yes @qtr pm-enable $feature
        drush --yes @qtr features-revert $feature
    done < qtr_features.txt

    while read feature; do
        drush --yes @qtr_dev pm-enable $feature
        drush --yes @qtr_dev features-revert $feature
    done < qtr_dev_features.txt

    # restore private variables
    drush @qtr php-script $(pwd)/restore-private-vars-qtr.php
    drush @qtr_dev php-script $(pwd)/restore-private-vars-qtr-dev.php

    # twitter config
    [[ -f trc ]] && cp trc /home/twitter/.trc
}


# go to the backup directory
backup=$1
cd /host/$backup

# restore
restore_data
restore_config

# custom restore script
[[ -f /host/restore.sh ]] && source /host/restore.sh

# restore any custom scripts
[[ -f /host/backup.sh ]] || cp backup.sh /host/
[[ -f /host/restore.sh ]] || cp restore.sh /host/
[[ -d /host/cmd ]] || cp -a cmd /host/
[[ -d /host/scripts ]] || cp -a scripts /host/

