#!/bin/bash -u
if ! [[ "$USER" = "root" ]] ; then
	su -c 'init/compile.sh "usr.local"'
#	User=$(cat config/user)
#	chown -Rc $User *
fi
if [ -f ./usr.local ] ; then
	rm -fv ./usr.local
fi
#User=$(cat config/user)
#if [[ "$User" != "root" ]] ; then
#	chown -Rc $User *
#fi
if [[ "$RootMode" = "1" ]] ; then
	exit && exit
fi
