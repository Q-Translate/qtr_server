cmd_create_help() {
    cat <<_EOF
    create
        Create the container '$CONTAINER'.

_EOF
}

rename_function cmd_create orig_cmd_create
cmd_create() {
    local code_dir=$(dirname $(realpath $APP_DIR))
    mkdir -p var-www drush-cache
    orig_cmd_create \
        --mount type=bind,src=$code_dir,dst=/usr/local/src/qtr_server \
        --mount type=bind,src=$(pwd)/var-www,dst=/var/www \
        --mount type=bind,src=$(pwd)/drush-cache,dst=/root/.drush/cache \
        --workdir /var/www \
        --env CODE_DIR=/usr/local/src/qtr_server \
        --env DRUPAL_DIR=/var/www/qtr \
        "$@"    # accept additional options, e.g.: -p 2201:22

    rm -f qtr_server
    ln -s var-www/qtr/profiles/qtr_server .

    # create the databases and the user
    local dbdata="${DBNAME}_data"
    ds @$DBHOST exec mysql -e "
        create database if not exists $DBNAME;
        create database if not exists ${DBNAME}_data;
        grant all privileges on *.* to '$DBUSER'@'$CONTAINER.$NETWORK' identified by '$DBPASS';
        flush privileges;
    "

    # copy local commands
    mkdir -p cmd/
    [[ -f cmd/remake.sh ]] || cp $APP_DIR/misc/remake.sh cmd/
}
