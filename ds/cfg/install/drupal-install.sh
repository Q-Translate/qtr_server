#!/bin/bash -x

source /host/settings.sh

### settings for the database and the drupal site
db_name=qtr
db_user=qtr
db_pass=qtr
site_name="Q-Translate"
site_mail="$GMAIL_ADDRESS"
account_name=admin
account_pass="$ADMIN_PASS"
account_mail="$GMAIL_ADDRESS"

### create the database and user
mysql='mysql --defaults-file=/etc/mysql/debian.cnf'
$mysql -e "
    DROP DATABASE IF EXISTS $db_name;
    CREATE DATABASE $db_name;
    GRANT ALL ON $db_name.* TO $db_user@localhost IDENTIFIED BY '$db_pass';
"

### start site installation
#sed -e '/memory_limit/ c memory_limit = -1' -i /etc/php/7.0/cli/php.ini
cd $DRUPAL_DIR
drush site-install --verbose --yes qtr_server \
      --db-url="mysql://$db_user:$db_pass@localhost/$db_name" \
      --site-name="$site_name" --site-mail="$site_mail" \
      --account-name="$account_name" --account-pass="$account_pass" --account-mail="$account_mail"

### install additional features
drush="drush --root=$DRUPAL_DIR --yes"
$drush pm-enable qtr_layout
$drush features-revert qtr_layout

$drush pm-enable qtr_content

$drush pm-enable qtr_captcha
$drush features-revert qtr_captcha

#$drush pm-enable qtr_invite
#$drush pm-enable qtr_simplenews
#$drush pm-enable qtr_mass_contact
#$drush pm-enable qtr_googleanalytics

### update to the latest version of core and modules
#$drush pm-refresh
#$drush pm-update

### refresh and update translations
if [[ -z $DEV ]]; then
    $drush l10n-update-refresh
    $drush l10n-update
fi
