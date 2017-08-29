api = 2
core = 7.x

;------------------------------
; Build Drupal core (with patches).
;------------------------------
includes[drupal] = drupal-org-core.make

;------------------------------
; Get profile qtr_server.
;------------------------------
projects[qtr_server][type] = profile
projects[qtr_server][download][type] = git
projects[qtr_server][download][url] = /usr/local/src/qtr_server
projects[qtr_server][download][branch] = master
