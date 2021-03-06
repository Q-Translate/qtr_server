#!/bin/bash -x

#source /host/settings.sh

backup_data() {
    ### set mysqldump options
    sql_connect=$(drush @qtr sql-connect --database=qtr_data | sed -e 's/^mysql //' -e 's/--database=/--databases /')
    mysqldump="mysqldump $sql_connect --skip-add-drop-table --replace"

    ### backup translations
    $mysqldump --tables qtr_translations --where="time IS NOT NULL" \
               > $(pwd)/qtr_data.sql

    ### backup likes
    $mysqldump --tables qtr_likes --where="time IS NOT NULL" \
               >> $(pwd)/qtr_data.sql

    ### backup other tables of qtr_data
    table_list="
    qtr_translations_trash
    qtr_likes_trash
    qtr_users
    "
    $mysqldump --tables $table_list >> $(pwd)/qtr_data.sql

    ### fix 'CREATE TABLE' on the sql file
    sed -i qtr_data.sql \
        -e 's/CREATE TABLE/CREATE TABLE IF NOT EXISTS/g'

    ### backup qtr tables
    mysqldump=$(drush @qtr sql-connect | sed -e 's/^mysql/mysqldump/' -e 's/--database=/--databases /')
    table_list="
        qtrLanguages
        users
        users_roles
        field_data_field_translation_lng
        field_revision_field_translation_lng
        hybridauth_identity
        hybridauth_session
    "
    $mysqldump --tables $table_list > $(pwd)/qtr.sql
}

backup_config() {
    # enabled features
    drush @qtr features-list --pipe --status=enabled \
          > $(pwd)/qtr_features.txt
    drush @qtr_dev features-list --pipe --status=enabled \
          > $(pwd)/qtr_dev_features.txt

    # drupal variables
    local dir
    dir=/var/www/qtr/profiles/qtr_server/modules/features
    $dir/save-private-vars.sh @qtr
    mv restore-private-vars.php restore-private-vars-qtr.php

    dir=/var/www/qtr_dev/profiles/qtr_server/modules/features
    $dir/save-private-vars.sh @qtr_dev
    mv restore-private-vars.php restore-private-vars-qtr-dev.php

    # twitter config
    [[ -f /home/twitter/.trc ]] && cp /home/twitter/.trc trc
}


# go to the backup dir
backup=$1
cd /host/$backup

# make the backup
backup_data
backup_config

# backup settings.sh
cp /host/settings.sh .

# custom backup script
[[ -f /host/backup.sh ]] && source /host/backup.sh

# backup any custom scripts
[[ -f /host/backup.sh ]] && cp /host/backup.sh .
[[ -f /host/restore.sh ]] && cp /host/restore.sh .
[[ -d /host/cmd ]] && cp -a /host/cmd .
[[ -d /host/scripts ]] && cp -a /host/scripts .

