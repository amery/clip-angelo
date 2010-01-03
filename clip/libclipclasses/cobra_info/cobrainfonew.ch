function cobraInfoNew()
	local lang,ch,obj:=map()
	obj:classname	:= "COBRAINFO"
	obj:version	:= COBRA_VERSION
	obj:user	:= getenv("USER")
	obj:passwd	:= ""
	obj:lang	:= "en_EN"
	obj:langs	:= {"en_EN"}
	obj:charset	:= "CP437"
	obj:charsets	:= {"CP437"}
	obj:crypto	:= COBRA_CRYPTO
	obj:crypts	:= {COBRA_CRYPTO}
	obj:auth	:= COBRA_AUTH_SCRIPT

	obj:fromIni	:=@ ci_fromIni()

	/* lang environment detecting*/
	lang := getenv("CLIP_LANG")
	if empty(lang)
		lang := getenv("LANG")
	endif
	if !empty(lang)
		ch := atr(".",lang)
		if ch > 0
			obj:charset := substr(lang,ch+1)
			obj:lang := left(lang,ch-1)
			aadd(obj:charsets,obj:charset)
		else
			obj:lang := lang
		endif
		aadd(obj:langs,obj:lang)
	endif

return obj
