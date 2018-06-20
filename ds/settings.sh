APP=qtr_server/ds

### Docker settings.
IMAGE=qtr_server
CONTAINER=qtr.example.org
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

### Gmail account for notifications. This will be used by ssmtp.
### You need to create an application specific password for your account:
### https://www.lifewire.com/get-a-password-to-access-gmail-by-pop-imap-2-1171882
GMAIL_ADDRESS=qtr.example.org@gmail.com
GMAIL_PASSWD=

### Admin settings.
ADMIN_PASS=123456

### Translation languages supported by the Q-Translate Server.
LANGUAGES='en fr de it sq'
