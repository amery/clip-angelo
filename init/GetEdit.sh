#!/bin/bash -u
if [[ -f "$Clip_S_Dir/Edit.setup.ini" ]] ; then
	export Edit=$(cat "$Clip_S_Dir/Edit.setup.ini" )
else
	if [ -x /usr/bin/kwrite ] ; then
		export Edit="/usr/bin/kwrite"
		echo "$Edit" >"$Clip_S_Dir/Edit.setup.ini"
		echo "Y" >"$Clip_S_Dir/MultiFile.setup.ini"
	fi
fi
if [[ $Edit = "/usr/bin/kwrite" ]] ; then
		echo "Y" >"$Clip_S_Dir/MultiFile.setup.ini"
fi
if [[ 1"" = 1"$Edit" ]] || [[ 1"" = 1"$MultiFile" ]]; then
	export Edit=""
	while ! [ -x $Edit ] || [[ 1"" = 1"$Edit" ]] ; do
		echo ""
		echo ""
		echo "Please enter the name with full path of your favorite program editor"
		echo "e.g. : /usr/bin/kwrite"
		echo "Pay attention if you work under X of not!"
		read -p "The full name + <enter> \[ /usr/bin/kwrite \]: " Edit
		echo ""
		if [[ -z "$Edit" ]] ; then
			export Edit=/usr/bin/kwrite
			export MultiFile="Y"
		else
			if ! [ -x "$Edit" ] ; then
				echo "The editor : $Edit DOES NOT EXIST"
				export Edit=""
				continue
			fi
			echo "Does your favorite editor support multi files"
			echo "e.g. : /usr/bin/kwrite file1 file2 file..."
			read -n 1 -p "enter [Y/N] : " MultiFile
		fi
	done
	echo "$Edit" >"$Clip_S_Dir/Edit.setup.ini"
	if [[ $MultiFile = [Nn] ]] ; then
		echo "N" >"$Clip_S_Dir/MultiFile.setup.ini"
	else
		echo "Y" >"$Clip_S_Dir/MultiFile.setup.ini"
	fi
fi

