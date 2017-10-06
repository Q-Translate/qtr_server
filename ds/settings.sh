APP=qtr_server/ds

### Docker settings.
IMAGE=qtr_server
CONTAINER=qtr-example-org
DOMAIN="qtr.example.org"

### Uncomment if this installation is for development.
DEV=true

### Other domains.
DOMAINS="dev.qtr.example.org"

### DB settings
DBHOST=mariadb
DBPORT=3306
DBNAME=qtr
DBUSER=qtr
DBPASS=qtr

### Gmail account for notifications.
### Make sure to enable less-secure-apps:
### https://support.google.com/accounts/answer/6010255?hl=en
GMAIL_ADDRESS=qtr.example.org@gmail.com
GMAIL_PASSWD=

### Admin settings.
ADMIN_PASS=123456

### Translation languages supported by the Q-Translate Server.
LANGUAGES='en fr de it sq'
