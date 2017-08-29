#!/bin/bash

### go to the directory of the script
cd $(dirname $0)

### make sure that the script is called with `nohup nice ...`
if [[ "$1" != "--dont-fork" ]]; then
    # this script has *not* been called recursively by itself

    ### get the language code
    if [[ -z $1 ]]; then
        echo "Usage: $0 <lng>
        where <lng> is the language code, like: en, fr, it, de, sq, etc."
        exit 1
    fi
    lng=$1

    datestamp=$(date +%F | tr -d -)
    nohup_out=logs/nohup-import-$lng-$datestamp.out
    mkdir -p logs/
    rm -f $nohup_out
    nohup nice $0 --dont-fork $@ > $nohup_out &
    sleep 1
    echo "tail -f $(pwd)/$nohup_out"
    tail -f $nohup_out
    exit
else
    # this script has been called by itself
    shift # remove the flag $1 that is used as a termination condition
fi

### get the language code
lng=$1

### go to the directory of translations
mkdir -p translations/
cd translations/

### get and import the translations of the given language
translations='https://github.com/Q-Translate/translations/raw/master'
files=$(wget -qO- $translations/LIST.txt | grep "^$lng\.")
for file in $files; do
    echo -e "\nGet    $file"
    rm -f $file
    wget -q $translations/$file

    echo "Import $file"
    author=${file%.txt}
    time drush qtr-import $lng $author "$(pwd)/$file" --force
done
echo 'DONE'
