#!/bin/bash -x

source /host/settings.sh

### make sure that we have the right git branch on the make file
makefile="$CODE_DIR/build-qtrserver.make"
git_branch=$(git -C $CODE_DIR branch | cut -d' ' -f2)
sed -i $makefile \
    -e "/qtr_server..download..branch/ c projects[qtr_server][download][branch] = $git_branch"

### retrieve all the projects/modules and build the application directory
rm -rf $DRUPAL_DIR
drush make --prepare-install --force-complete \
           --contrib-destination=profiles/qtr_server \
           $makefile $DRUPAL_DIR

### Replace the profile qtr_server with a version
### that is a git clone, so that any updates
### can be retrieved easily (without having to
### reinstall the whole application).
cd $DRUPAL_DIR/profiles/
mv qtr_server qtr_server-bak
cp -a $CODE_DIR .
### copy contrib libraries and modules
cp -a qtr_server-bak/libraries/ qtr_server/
cp -a qtr_server-bak/modules/contrib/ qtr_server/modules/
cp -a qtr_server-bak/themes/contrib/ qtr_server/themes/
### cleanup
rm -rf qtr_server-bak/

### copy the bootstrap library to the custom theme, etc.
cd $DRUPAL_DIR/profiles/qtr_server/
cp -a libraries/bootstrap themes/contrib/bootstrap/
cp -a libraries/bootstrap themes/qtr_server/
cp libraries/bootstrap/less/variables.less themes/qtr_server/less/

### copy hybridauth provider GitHub.php to the right place
cd $DRUPAL_DIR/profiles/qtr_server/libraries/hybridauth/
cp additional-providers/hybridauth-github/Providers/GitHub.php \
   hybridauth/Hybrid/Providers/

### copy the logo file to the drupal dir
ln -s $DRUPAL_DIR/profiles/qtr_server/qtr_server.png $DRUPAL_DIR/logo.png

### set propper directory permissions
cd $DRUPAL_DIR
mkdir -p sites/all/translations
chown -R www-data: sites/all/translations
mkdir -p sites/default/files/
chown -R www-data: sites/default/files/
mkdir -p cache/
chown -R www-data: cache/

### put a link to the data directory on /var/www/data
rm -f /var/www/data
ln -s $DRUPAL_DIR/profiles/qtr_server/modules/custom/qtrCore/data /var/www/data

### create the downloads dir etc.
mkdir -p /var/www/downloads/
chown www-data /var/www/downloads/
mkdir -p /var/www/exports/
chown www-data: /var/www/exports/
mkdir -p /var/www/uploads/
chown www-data: /var/www/uploads/
