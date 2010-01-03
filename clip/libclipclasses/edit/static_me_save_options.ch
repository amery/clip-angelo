***************************
static function me_save_options()
local nfile, str
	::Opt:FCASE             := ::Find:case
	::Opt:FWORDONLY         := ::Find:wordonly
	::Opt:FREGEXP           := ::Find:regexp
	::Opt:FALLW             := ::Find:allw
	::Opt:FDIRECT           := ::Find:direct
	::Opt:FWHERE            := ::Find:where
	dirmake(::__dirEdit)
	nfile := fcreate(::__dirEdit+PATH_DELIM+".edit")

	str := "Options := "+varToString(::Opt)+"&\n"
	fwrite(nfile, str, len(str))
	fclose(nfile)
return .t.
