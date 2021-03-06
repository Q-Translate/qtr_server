#!/bin/bash -x

source /host/settings.sh

### settings for the database and the drupal site
site_name="Q-Translate"
site_mail="$GMAIL_ADDRESS"
account_name=admin
account_pass="$ADMIN_PASS"
account_mail="$GMAIL_ADDRESS"

### start site installation
sed -e '/memory_limit/ c memory_limit = -1' -i /etc/php/7.1/cli/php.ini
cd $DRUPAL_DIR
drush site-install --verbose --yes qtr_server \
      --db-url="mysql://$DBUSER:$DBPASS@$DBHOST:$DBPORT/$DBNAME" \
      --site-name="$site_name" --site-mail="$site_mail" \
      --account-name="$account_name" --account-pass="$account_pass" --account-mail="$account_mail"

### set the common options for drush
drush="drush --root=$DRUPAL_DIR --yes"

### set the list of supported languages
sed -i $DRUPAL_DIR/profiles/qtr_server/modules/custom/qtrCore/data/config.sh \
    -e "/^languages=/c languages=\"$LANGUAGES\""
$drush vset qtr_languages "$LANGUAGES"

### add these languages to drupal
drush dl drush_language
for lng in $LANGUAGES; do
    $drush language-add $lng
done

### fix the DB schema and install some test data
$DRUPAL_DIR/profiles/qtr_server/modules/custom/qtrCore/data/install.sh

### set the directory for uploads
$drush vset --exact file_private_path '/var/www/uploads/'

### install features
$drush pm-enable qtr_qtrServices
$drush features-revert qtr_qtrServices

$drush pm-enable qtr_qtr
$drush features-revert qtr_qtr

$drush pm-enable qtr_misc
$drush features-revert qtr_misc

$drush pm-enable qtr_layout
$drush features-revert qtr_layout

$drush pm-enable bootstrap
$drush pm-enable qtr_server

$drush pm-enable qtr_hybridauth
$drush features-revert qtr_hybridauth

#$drush pm-enable qtr_captcha
#$drush features-revert qtr_captcha

$drush pm-enable qtr_permissions
$drush features-revert qtr_permissions

### update to the latest version of core and modules
#$drush pm-refresh
#$drush pm-update
