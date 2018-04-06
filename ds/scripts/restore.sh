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
}


# disable the site for maintenance
drush --yes @local_qtr vset maintenance_mode 1
drush --yes @local_qtr cache-clear all

# extract the backup archive
file=$1
cd /host/
tar --extract --gunzip --preserve-permissions --file=$file
backup=${file%%.tgz}
backup=$(basename $backup)
cd $backup/

# restore
restore_data
restore_config

# clean up
cd /host/
rm -rf $backup

# enable the site
drush --yes @local_qtr cache-clear all
drush --yes @local_qtr vset maintenance_mode 0
