cmd_create_help() {
    cat <<_EOF
    create
        Create the container '$CONTAINER'.

_EOF
}

rename_function cmd_create orig_cmd_create
cmd_create() {
    local code_dir=$(dirname $(realpath $APP_DIR))
    mkdir -p var-www
    orig_cmd_create \
        --mount type=bind,src=$code_dir,dst=/usr/local/src/qtr_server \
        --mount type=bind,src=$(pwd)/var-www,dst=/var/www \
        --workdir /var/www \
        --env CODE_DIR=/usr/local/src/qtr_server \
        --env DRUPAL_DIR=/var/www/qtr \
        "$@"    # accept additional options, e.g.: -p 2201:22

    rm -f qtr_server
    ln -s var-www/qtr/profiles/qtr_server .
}
