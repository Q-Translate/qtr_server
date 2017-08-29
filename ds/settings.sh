APP=qtr_server/ds

### Docker settings.
IMAGE=qtr_server
CONTAINER=qtr-example-org
SSHD_PORT=2201
#PORTS="80:80 443:443 $SSHD_PORT:22"    ## ports to be forwarded when running stand-alone
PORTS=""    ## no ports to be forwarded when running behind wsproxy

DOMAIN="qtr.example.org"
DOMAINS="dev.qtr.example.org tst.qtr.example.org"  # other domains

### Gmail account for notifications.
### Make sure to enable less-secure-apps:
### https://support.google.com/accounts/answer/6010255?hl=en
GMAIL_ADDRESS=qtr.example.org@gmail.com
GMAIL_PASSWD=

### Admin settings.
ADMIN_PASS=123456

### Translation languages supported by the Q-Translate Server.
LANGUAGES='en fr de it sq'

### Uncomment if this installation is for development.
DEV=true
