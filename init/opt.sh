#!/bin/bash -u
export WaitTime=1
if ! [[ "$USER" = "root" ]] ; then
	su -c 'init/compile.sh "opt"'
#	User=$(cat config/user)
#	chown -Rc $User *
fi

if [ -f ./opt ] ; then
	rm -fv ./opt
fi
#User=$(cat config/user)
#if [[ "$User" != "root" ]] ; then
#	chown -Rc $User *
#fi
if [[ "$RootMode" = "1" ]] ; then
	exit && exit
fi
