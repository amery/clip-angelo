static function __check_clnt_info(sInfo,qArgs,oAnswer)
	local i,j,x,a,ret := .t.
	local cInfo
	if ! __check_args(qArgs,1,{"O"})
		oAnswer:errno := COBRA_ERR_BADARG
		return .f.
	endif
	cInfo := qArgs[1]
	if ! (cInfo:classname == "COBRAINFO")
		oAnswer:errno := COBRA_ERR_BADINFO
		return .f.
	endif
	if cInfo:version != sInfo:version
		if cInfo:version < COBRA_COMPAT_VERSION
			oAnswer:errno := COBRA_ERR_BADVERSION
			return .f.
		else
			oAnswer:warning += [Need updating client]+"&\n"
		endif
	endif
	/* check langs */
	x := .f.
	for i=1 to len(cInfo:langs)
		j := ascan(sInfo:langs,{|a|upper(a)==upper(cInfo:langs[i])})
		if j<=0
			loop
		endif
		cInfo:lang := sInfo:langs[j]
		x := .t.
	next
	if empty(cInfo:lang)
		oAnswer:errno := COBRA_ERR_BADLANG
		return .f.
	else
		sInfo:lang := cInfo:lang
	endif
	if !x
		oAnswer:warning += [Client LANG value not supported]+"&\n"
	endif
	/* check charsets */
	x := .f.
	for i=1 to len(cInfo:charsets)
		j := ascan(sInfo:charsets,{|a|upper(a) == upper(cInfo:charsets[i])})
		if j<=0
			loop
		endif
		cInfo:charset := sInfo:charset[j]
		x := .t.
	next
	if empty(cInfo:charset)
		oAnswer:errno := COBRA_ERR_BADCHARSET
		return .f.
	else
		sInfo:charset := cInfo:charset
	endif
	if !x
		oAnswer:warning += [Client CHARSET value not supported]+"&\n"
	endif
	/* check crypts */
	x := .f.
	for i=1 to len(cInfo:crypts)
		j := ascan(sInfo:crypts,{|a|upper(a)==upper(cInfo:crypts[i])})
		if j<=0
			loop
		endif
		cInfo:crypto := sInfo:crypts[j]
		x := .t.
	next
	if empty(cInfo:crypto)
		oAnswer:errno := COBRA_ERR_BADCRYPTO
		return .f.
	else
		sInfo:crypto := cInfo:crypto
	endif
	if !x
		oAnswer:warning += [Client CRYPTO value not supported]+"&\n"
	endif
	oAnswer:return := cInfo
return .t.
