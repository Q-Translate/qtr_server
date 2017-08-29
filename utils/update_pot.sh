#/bin/bash
### Extract translatable strings of qtr_server
### and update the file 'qtrserver.pot'.
###
### Run it on a copy of qtr_server that is just
### cloned from git, don't run it on an installed
### copy of qtr_server, otherwise 'potx-cli.php'
### will scan also the other modules that are on
### the directory 'modules/'.

### go to the qtr_server directory
cd $(dirname $0)
cd ..

### extract translatable strings
utils/potx-cli.php

### concatenate files 'general.pot' and 'installer.pot' into 'qtr_server.pot'
msgcat --output-file=qtrserver.pot general.pot installer.pot
rm -f general.pot installer.pot
mv -f qtrserver.pot l10n/

### merge/update with previous translations
for po_file in $(ls l10n/qtrserver.*.po)
do
    msgmerge --update --previous $po_file l10n/qtrserver.pot
done

