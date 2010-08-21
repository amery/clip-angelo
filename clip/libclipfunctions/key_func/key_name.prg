function key_name(nkey)

	local _key_names,ret:="",i

	#include "ci_key_name.ch"

	for i=1 to len(_key_names)
		if nkey>=_key_names[i][1] .and. nkey<=_key_names[i][2]
			if nkey>=0
				nkey=nkey-_key_names[i][1]+1
			else
				nkey=abs(_key_names[i][2]-nkey)+1
			endif
			ret=_key_names[i][3][nkey]
			exit
		endif
	next
return ret
