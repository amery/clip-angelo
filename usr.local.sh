#!/bin/bash -u
if ! [[ "$USER" = "root" ]] ; then
	su -c 'init/compile.sh "usr.local"'
#	User=$(cat config/user)
#	chown -Rc $User *
fi
[ -f ./usr.local ] && rm -fv ./usr.local
#User=$(cat config/user)
#if [[ "$User" != "root" ]] ; then
#	chown -Rc $User *
#fi
if [[ "$RootMode" = "1" ]] ; then
	exit && exit
fi
