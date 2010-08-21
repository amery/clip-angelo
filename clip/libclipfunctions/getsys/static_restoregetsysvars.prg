STATIC PROCEDURE RestoreGetSysVars( aSavSysVars )
   LOCAL k,lUpdated         := oStatus:updated
   for k in aSavSysVars KEYS
	oStatus[k] := aSavSysVars[k]
   next
   oStatus:updated := lUpdated
   READVAR( aSavSysVars:readvar )
   GETACTIVE( aSavSysVars:activeget )
RETURN
