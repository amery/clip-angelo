#!/bin/bash -u
while [[ -n "$*" ]] ; do
	flawfinder --columns -m 1 -i $1
	shift
done
