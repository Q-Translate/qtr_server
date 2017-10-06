#!/bin/bash
### Save the schema of the database qtr_data.

source /host/settings.sh

### go to the script directory
cd $(dirname $0)

### mysql and mysqldump options
dbname=${QTR_DATA:-${DBNAME}_data}
mysql="mysql --host=$DBHOST --port=$DBPORT --user=$DBUSER --password='$DBPASS' --database=$dbname"
mysqldump="mysqldump --host=$DBHOST --port=$DBPORT --user=$DBUSER --password='$DBPASS' --databases $dbname"

### get the list of the tables
tables=$($mysql -B -e "SHOW TABLES" | grep '^qtr_' )

### dump only the schema of the database
$mysqldump --no-data --compact --add-drop-table \
    --tables $tables > qtr_schema.sql

### fix a little bit the file
sed -e '/^SET /d' -i qtr_schema.sql

echo "
Schema saved to:
  $(pwd)/qtr_schema.sql
"
