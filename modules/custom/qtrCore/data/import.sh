#!/bin/bash

### go to the directory of the script
cd $(dirname $0)

### make sure that the script is called with `nohup nice ...`
if [[ "$1" != "--dont-fork" ]]; then
    # this script has *not* been called recursively by itself
    datestamp=$(date +%F | tr -d -)
    nohup_out=/host/logs/nohup-import-$lng-$datestamp.out
    mkdir -p /host/logs/
    rm -f $nohup_out
    nohup nice "$(pwd)/$(basename $0)" --dont-fork $@ > $nohup_out &
    sleep 1
    echo "tail -f $(pwd)/$nohup_out"
    tail -f $nohup_out
    exit
else
    # this script has been called by itself
    shift # remove the flag $1 that is used as a termination condition
fi

### go to the directory of translations
mkdir -p translations/
cd translations/

### get the languages that will be imported
source /host/settings.sh
languages=${1:-$LANGUAGES}

for lng in $languages; do
    # get and import the translations of the language
    translations='https://github.com/Q-Translate/translations/raw/master'
    files=$(wget -qO- $translations/LIST.txt | grep "^$lng\.")
    for file in $files; do
        echo -e "\nGet    $file"
        rm -f $file
        wget -q $translations/$file

        echo "Import $file"
        author=${file%.txt}
        drush qtr-import $lng $author "$(pwd)/$file" --force
    done
done
echo -e "\nDONE"
