#!/bin/bash
rm -rf /tmp/webserv_eval # re-create directory
cd /tmp/
mkdir webserv_eval
cd webserv_eval
mkdir YoupiBanane && cd YoupiBanane
touch youpi.bad_extension
touch youpi.bla
mkdir nop && cd nop
touch youpi.bad_extension
touch other.pouic
cd ..
mkdir Yeah
cd Yeah
touch not_happy.bad_extension
cd ../..

echo "Webserv eval directory created"
find . -print | sed -e 's;[^/]*/;|____;g;s;____|; |;g'
