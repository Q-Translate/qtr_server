#!/bin/bash
### Export contributions from users (translations and likes)
### since a certain date.

source /host/settings.sh

### get the arguments
from_date=${1:-'0000-00-00'}    # in format YYYY-MM-DD

### mysqldump default options
dbname=${QTR_DATA:-${DBNAME}_data}
mysqldump="mysqldump --host=$DBHOST --port=$DBPORT --user=$DBUSER --password='$DBPASS' --databases $dbname"

### get the dump filename
date1=${from_date//-/}
date2=$(date +%Y%m%d)
dump_file=contributions-$date1-$date2.sql

### dump translations and likes
$mysqldump --tables qtr_translations \
    --where="time > '$from_date' AND umail != ''" > $dump_file
$mysqldump --tables qtr_likes \
    --where="time > '$from_date'" >> $dump_file

### dump also deleted translations and likes
$mysqldump --tables qtr_translations_trash \
    --where="d_time > '$from_date'" >> $dump_file
$mysqldump --tables qtr_likes_trash \
    --where="d_time > '$from_date'" >> $dump_file

### compress the dump file
gzip $dump_file
