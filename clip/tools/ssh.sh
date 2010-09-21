#!/bin/bash -u
ssh=`which ssh`
if [ -z "$ssh" ]
then
	ssh=ssh
fi
exec $ssh $*

