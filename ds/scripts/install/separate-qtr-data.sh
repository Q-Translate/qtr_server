#!/bin/bash -x
### Create another database for the translation data
### and copy to it the relevant tables (of module qtrCore).

source /host/settings.sh

### database and user settings
db_name=qtr_data
db_user=qtr_data
db_pass=qtr_data

### create the database and user
mysql="mysql --defaults-file=/etc/mysql/debian.cnf -B"
$mysql -e "
    DROP DATABASE IF EXISTS $db_name;
    CREATE DATABASE $db_name;
    GRANT ALL ON $db_name.* TO $db_user@localhost IDENTIFIED BY '$db_pass';
"

### copy the tables of qtr_data to the new database
tables=$($mysql -D qtr -e "SHOW TABLES" | grep '^qtr_' )
for table in $tables; do
    echo "Copy: $table"
    $mysql -e "
        CREATE TABLE $db_name.$table LIKE qtr.$table;
        INSERT INTO $db_name.$table SELECT * FROM qtr.$table;
    "
done

### modify Drupal settings
drupal_settings=$DRUPAL_DIR/sites/default/settings.php
sed -e '/===== APPENDED BY INSTALLATION SCRIPTS =====/,$ d' -i $drupal_settings
cat << EOF >> $drupal_settings
//===== APPENDED BY INSTALLATION SCRIPTS =====

/**
 * Use a separate database for the translation data.
 * This provides more flexibility. For example the
 * drupal site and the translation data can be backuped
 * and restored separately. Or a test drupal site
 * (testing new drupal features) can connect to the
 * same translation database.
 */
\$databases['qtr_db']['default'] = array (
    'database' => '$db_name',
    'username' => '$db_user',
    'password' => '$db_pass',
    'host' => 'localhost',
    'port' => '',
    'driver' => 'mysql',
    'prefix' => '',
);

EOF
