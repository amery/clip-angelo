if [ -f clip_msgfmt.in ]
then

printf "configure: creating clip_msgfmt$SCRIPTSUFF .."
exec 3>clip_msgfmt$SCRIPTSUFF

echo '#!/bin/sh ' >&3
echo "# Created automatically by 'configure'" >&3
echo >&3
echo '[ -z "$CLIPROOT" ] && '"CLIPROOT=$CLIPROOT" >&3
echo "NO_GETTEXT=$NO_GETTEXT" >&3
if [ "$PO_COMPAT" = yes ]
then
	echo "PO_COMPAT=yes" >&3
fi
echo "PO_FROM_COMPAT=\"$PO_FROM_COMPAT\"" >&3
echo "PO_TO_COMPAT=\"$PO_TO_COMPAT\"" >&3
cat clip_msgfmt.in >&3

exec 3>&-
echo ". done."

chmod +x clip_msgfmt$SCRIPTSUFF

fi

