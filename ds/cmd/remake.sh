cmd_remake_help() {
    cat <<_EOF
    remake
        Reconstruct again the container, preserving the existing data.

_EOF
}

cmd_remake() {
    # backup
    ds backup data

    # reinstall
    ds remove
    ds make
    ds restart

    # restore
    local backup_file="backup-data-$(date +%Y%m%d).tgz"
    ds restore $backup_file

    # re-import the data
    ds exec data/import.sh
}
