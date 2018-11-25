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
    if [[ -f trc ]]; then
        cp trc /home/twitter/.trc
        chown twitter: /home/twitter/.trc
    fi
}

restore_custom_scripts() {
    if [[ ! -f /host/backup.sh ]] && [[ -f backup.sh ]]; then
        cp backup.sh /host/
    fi
    if [[ ! -f /host/restore.sh ]] && [[ -f restore.sh ]]; then
        cp restore.sh /host/
    fi
    if [[ ! -d /host/cmd ]] && [[ -d cmd ]]; then
        cp -a cmd /host/
    fi
    if [[ ! -d /host/scripts ]] && [[ -d scripts ]]; then
        cp -a scripts /host/
    fi
}

# go to the backup directory
backup=$1
cd /host/$backup

# restore
restore_data
restore_config

# restore any custom scripts
restore_custom_scripts

# custom restore script
[[ -f /host/restore.sh ]] && source /host/restore.sh
