
# Q-Translate Server

Drupal installation profile for Q-Translate Server.

Q-Translate is an application that helps to improve the translations
of the Quran, by getting review and feedback from lots of
people. http://info.qtranslate.org


## Installation

  - First install Docker:
    https://docs.docker.com/engine/installation/linux/docker-ce/ubuntu/#install-using-the-repository

  - Then install `ds`, `wsproxy` and `mariadb`:
     + https://github.com/docker-scripts/ds#installation
     + https://github.com/docker-scripts/wsproxy#installation
     + https://github.com/docker-scripts/mariadb#installation


  - Get the code from GitHub, like this:
    ```
    git clone https://github.com/Q-Translate/qtr_server /opt/docker-scripts/qtr_server
    ```

  - Create a directory for the container: `ds init qtr_server/ds @qtr-example-org`

  - Fix the settings:
    ```
    cd /var/ds/qtr-example-org/
    vim settings.sh
    ```

  - Build image, create the container and configure it: `ds make`


## Install Q-Translate Client

  - See: https://github.com/Q-Translate/qtr_client#installation

  - Setup oauth2 login between the client and the server: `ds @qcl-example-org setup-oauth2-login @qtr-example-org`
    or
    ```
    cd /var/ds/qcl-example-org/
    ds setup-oauth2-login @qtr-example-org
    ```


## Access the website

  - Tell `wsproxy` to manage the domain of this container: `ds wsproxy add`

  - Tell `wsproxy` to get a free letsencrypt.org SSL certificate for
    this domain (if it is a real one): `ds wsproxy ssl-cert`

  - If the domain is not a real one, add to `/etc/hosts` the lines
    `127.0.0.1 qtr.example.org` and `127.0.0.1 qcl.example.org` and
    then try in browser https://qtr.example.org and
    https://qcl.example.org


## Other commands

    ds help

    ds shell
    ds stop
    ds start
    ds snapshot

    ds inject set-adminpass.sh <new-drupal-admin-passwd>
    ds inject set-domain.sh <new.domain>
    ds inject set-emailsmtp.sh <gmail-user> <gmail-passwd>
    ds inject oauth2-client-add.sh <@alias> <client-key> <client-secret> <https://redirect-uri>
    ds inject set-languages.sh

    ds inject dev/clone.sh test
    ds inject dev/clone-del.sh test
    ds inject dev/clone.sh 01

    ds backup [proj1]
    ds restore <backup-file.tgz> [proj1]
