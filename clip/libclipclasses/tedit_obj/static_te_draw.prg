static function te_draw(Draw_Mode, nkey, undo)
local draw_char, around_char[4], back_cur_ch, back_new_ch, string, direction
local A_Right:=1, A_Left:=2, A_Up:=3, A_Down:=4
local R_Mode, L_Mode, U_Mode, D_Mode
local p, ch_ret, line, pos, old_direction:=0, str, ch, i, j
	undo := iif(undo == NIL, .t., undo)
	draw_char := {{"�", "�"}, {"�", "�"}}
	back_cur_ch := {"������������",;
			"������������",;
			"������������",;
			"������������"}
	back_new_ch := {"������������������������",;
			"������������������������",;
			"������������������������",;
			"������������������������"}
	string :={;
   /* present_derection=A_Right*/;
   /*up   */       {{"����������������������","����������������������"},;
   /*down */        {"����������������������","����������������������"},;
   /*up down*/      {"����������������������","����������������������"};
		     },;
   /* present_derection=A_Left*/;
		   {{"����������������������","����������������������"},;
		    {"����������������������","����������������������"},;
		    {"����������������������","����������������������"};
		     },;
   /* present_derection=A_Up*/;
   /*left   */     {{"����������������������","����������������������"},;
   /*right  */      {"����������������������","����������������������"},;
   /*left right*/   {"����������������������","����������������������"};
		     },;
   /* present_derection=A_Down*/;
		   {{"����������������������","����������������������"},;
		    {"����������������������","����������������������"},;
		    {"����������������������","����������������������"};
		    };
		   }
	Draw_Mode := iif(Draw_Mode, 1, 2)
	R_Mode:=0; L_Mode:=0; U_Mode:=0; D_Mode:=0
	afill(around_char, NIL)

	do case
	case nkey == K_RIGHT
		direction := 1
	case nkey == K_LEFT
		direction := 2
	case nkey == K_UP
		direction := 3
	case nkey == K_DOWN
		direction := 4
	otherwise
		return .f.
	endcase
	line := ::line
	pos := ::pos
	::__check_line(line)
	ch_ret := substr(::edbuffer[line], pos, 1)
	::__around_check(@around_char, @R_Mode, @L_Mode, @U_Mode, @D_Mode)
	if undo
		aadd(around_char, ch_ret)
		::writeundo(HASH_DRAW, around_char)
	endif

	if direction == A_Right
		::right(.f.)
		ch := substr(::edbuffer[::line], ::pos-1, 1)
		p = at(ch, back_cur_ch[direction])
		if p>=1
			ch_ret := substr(back_new_ch[direction], (Draw_Mode-1) * 12 + p, 1)
		else
			if ((L_Mode > 0) .and. (D_Mode + U_Mode + R_Mode) == 0)
				ch_ret := draw_char[1][Draw_Mode]
			elseif ((D_Mode == 1 .and. U_Mode == 1 ) .or. (D_Mode == 2 .and. U_Mode == 2))
				ch_ret := substr(string[direction][3][D_Mode], L_Mode*8 + R_Mode*2 + Draw_Mode, 1)
				if ch != ch_ret
					::left(.f.)
				endif
			elseif (D_Mode == 1 .or. D_Mode == 2)
				ch_ret := substr(string[direction][2][D_Mode], L_Mode*8 + R_Mode*2 + Draw_Mode, 1)
				if ch != ch_ret
					::left(.f.)
				endif
			elseif (U_Mode == 2 .or. U_Mode==1)
				ch_ret := substr(string[direction][1][U_Mode], L_Mode*8 + R_Mode*2 + Draw_Mode, 1)
				if ch != ch_ret
					::left(.f.)
				endif
			else
				ch_ret := draw_char[1][Draw_Mode]
			endif
		endif
	endif

	if direction == A_Left
		  ::left(.f.)
		  ch := substr(::edbuffer[::line], ::pos+1, 1)
		  p := at(ch, back_cur_ch[direction])
		  if p>=1
			ch_ret := substr(back_new_ch[direction], (Draw_Mode-1) * 12 + p, 1)
		  else
			if ((R_Mode > 0) .and. (D_Mode + U_Mode + R_Mode) == 0)
				ch_ret := draw_char[1][Draw_Mode]
			elseif (((D_Mode == 1) .and. (U_Mode == 1) ) .or. (D_Mode == 2 .and. U_Mode == 2))
				ch_ret := substr(string[direction][3][D_Mode], L_Mode*8 + R_Mode*2 + Draw_Mode, 1)
				if ch != ch_ret
					::right(.f.)
				endif
			elseif (D_Mode == 1 .or. D_Mode == 2)
				ch_ret := substr(string[direction][2][D_Mode], L_Mode*8 + R_Mode*2 + Draw_Mode, 1)
				if ch != ch_ret
					::right(.f.)
				endif
			elseif (U_Mode == 2 .or. U_Mode==1)
				ch_ret := substr(string[direction][1][U_Mode], L_Mode*8 + R_Mode*2 + Draw_Mode, 1)
				if ch != ch_ret
					::right(.f.)
				endif
			else
				ch_ret := draw_char[1][Draw_Mode]
			endif
		  endif
	    endif
	 if direction == A_Up
	      ::up(.f.)
	      ch := substr(::edbuffer[::line+1], ::pos, 1)
	      p := at(ch, back_cur_ch[direction])
	      if p>=1
		   ch_ret := substr(back_new_ch[direction], (Draw_Mode-1) * 12 + p, 1)
	      else
			if (D_Mode > 0 .and. (R_Mode + U_Mode + L_Mode) == 0)
				ch_ret := draw_char[2][Draw_Mode]
			elseif ((L_Mode == 1 .and. R_Mode == 1) .or. L_Mode==2 .and. R_Mode==2)
				ch_ret := substr(string[direction][3][L_Mode], U_Mode*8 + D_Mode*2 + Draw_Mode, 1)
				if ch != ch_ret
					::down(.f.)
				endif
			elseif (L_Mode == 1 .or. L_Mode==2)
				ch_ret := substr(string[direction][1][L_Mode], U_Mode*8 + D_Mode*2 + Draw_Mode, 1)
				if ch != ch_ret
					::down(.f.)
				endif
			elseif (R_Mode == 2 .or. R_Mode==1)
				ch_ret := substr(string[direction][2][R_Mode], U_Mode*8 + D_Mode*2 + Draw_Mode, 1)
				if ch != ch_ret
					::down(.f.)
				endif
			else
				ch_ret := draw_char[2][Draw_Mode]
			endif
	      endif
	endif
	if direction == A_Down
	      ::down(.f.)
	      ch := substr(::edbuffer[::line-1], ::pos, 1)
	      p := at(ch, back_cur_ch[direction])
	      if p>=1
		   ch_ret := substr(back_new_ch[direction], (Draw_Mode-1) * 12 + p, 1)
	      else
			if (D_Mode > 0 .and. (R_Mode + U_Mode + L_Mode) == 0)
				ch_ret := draw_char[2][Draw_Mode]
			elseif ((R_Mode == 1 .and. L_Mode == 1) .or. L_Mode==2 .and. R_Mode==2)
				ch_ret := substr(string[direction][3][R_Mode], U_Mode*8 + D_Mode*2 + Draw_Mode, 1)
				if ch != ch_ret
					::up(.f.)
				endif
			elseif (L_Mode == 1 .or. L_Mode==2)
				ch_ret := substr(string[direction][1][L_Mode], U_Mode*8 + D_Mode*2 + Draw_Mode, 1)
				if ch != ch_ret
					::up(.f.)
				endif
			elseif (R_Mode == 2 .or. R_Mode==1)
				ch_ret := substr(string[direction][2][R_Mode], U_Mode*8 + D_Mode*2 + Draw_Mode, 1)
				if ch != ch_ret
					::up(.f.)
				endif
			else
				ch_ret := draw_char[2][Draw_Mode]
			endif
	      endif
	endif

	for i in around_char
		i := translate_charset( __CHARSET__,host_charset(), i)
	next
	if around_char[1] != NIL
		::edbuffer[line] := padr(substr(::edbuffer[line], 1, pos), pos) + around_char[1] + substr(::edbuffer[line], pos+2)
	endif
	if around_char[2] != NIL
		::edbuffer[line] := padr(substr(::edbuffer[line], 1, pos-2), pos-2) + around_char[2] + substr(::edbuffer[line], pos)
	endif
	if around_char[3] != NIL .and. line - 1 > 0
		::edbuffer[line-1] := padr(substr(::edbuffer[line-1], 1, pos-1), pos-1) + around_char[3] + substr(::edbuffer[line-1], pos+1)
	endif
	if around_char[4] != NIL .and. line + 1 <= ::lines
		::edbuffer[line+1] := padr(substr(::edbuffer[line+1], 1, pos-1), pos-1) + around_char[4] + substr(::edbuffer[line+1], pos+1)
	endif
	::edbuffer[line] := padr(substr(::edbuffer[line], 1, pos-1), pos-1) + ch_ret + substr(::edbuffer[line], pos+1)
	::updated := .t.
	if undo
		::refresh()
	endif
RETURN .t.
