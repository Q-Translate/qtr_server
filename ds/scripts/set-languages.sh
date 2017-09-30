#!/bin/bash -x

source /host/settings.sh
languages=${1:-$LANGUAGES}

### set the list of languages for import
sed -i /var/www/data/config.sh \
    -e "/^languages=/c languages=\"$languages\""

### update drupal configuration
drush @local_qtr --yes vset qtr_languages "$languages"
drush @local_qtr --yes php-eval "module_load_include('inc', 'qtrCore', 'admin/core'); qtrCore_config_set_languages();"

### add these languages to drupal and import their translations
for lng in $languages; do
    drush @local_qtr --yes language-add $lng
done
if [[ -z $DEV ]]; then
    drush @local_qtr --yes l10n-update-refresh
    drush @local_qtr --yes l10n-update
fi
