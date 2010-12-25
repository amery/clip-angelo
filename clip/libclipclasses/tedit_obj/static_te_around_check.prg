static function te_around_check(around_char, R_Mode, L_Mode, U_Mode, D_Mode)
local ch, p
	::__check_line(::line)
	//::__check_line(::line-1)
	//::__check_line(::line+1)

	/* right */
	around_char[1] := substr(::edbuffer[::line], ::pos+1, 1)
	p := at(around_char[1], "��������������������������")
	if p>=1 .and. p<14
		R_Mode=1
	endif
	if p >13
		R_Mode=2
	endif
	/* left */
	if ::pos-1 < 1
		around_char[2] := ""
	else
		around_char[2] := substr(::edbuffer[::line], ::pos-1, 1)
	endif
	p = at(around_char[2], "��������������������������")
	if p >=1 .and. p<14
		L_Mode=1
	endif
	if p >13
		L_Mode=2
	endif
	/* up */
	if ::line-1 < 1
		around_char[3] := " "
	else
		around_char[3] := substr(::edbuffer[::line-1], ::pos, 1)
	endif
	p := at(around_char[3], "��������������������������")
	if p >=1 .and. p <14
		U_Mode = 1
	endif
	if p >13
		U_Mode = 2
	endif
	/* down */
	if ::line+1 > ::lines
		around_char[4] := " "
	else
		around_char[4] := substr(::edbuffer[::line+1], ::pos, 1)
	endif
	p := at(around_char[4], "��������������������������")
	if p >=1 .and. p < 14
		D_Mode=1
	endif
	if p >13
		D_Mode=2
	endif

return
