proc GetApplyKey(get, key)

local cKey
local bKeyBlock


	if ( (bKeyBlock := SetKey(key)) <> NIL )
		GetDoSetKey(bKeyBlock, get)
		return
	end
	//dispbegin()
	do case
	case ( key == K_UP )
		get:exitState := GE_UP

	case ( key == K_SH_TAB )
		get:exitState := GE_UP

	case ( key == K_DOWN )
		get:exitState := GE_DOWN

	case ( key == K_TAB )
		get:exitState := GE_DOWN

	case ( key == K_ENTER )
		get:exitState := GE_ENTER

	case ( key == K_ESC )
		if ( Set(_SET_ESCAPE) )
			get:undo()
			get:exitState := GE_ESCAPE
		end

	case ( key == K_PGUP )
		get:exitState := GE_WRITE

	case ( key == K_PGDN )
		get:exitState := GE_WRITE

	case ( key == K_CTRL_HOME )
		get:exitState := GE_TOP


	case (key == K_CTRL_END)
		get:exitState := GE_BOTTOM

	case (key == K_CTRL_W)
		get:exitState := GE_WRITE

	case (key == K_INS)
		Set( _SET_INSERT, !Set(_SET_INSERT) )
		ShowScoreboard()

	case (key == K_HOME)
		get:Home()

	case (key == K_END)
		get:End()

	case (key == K_RIGHT)
		get:Right()
	case (key == K_CTRL_RIGHT)
		get:WordRight()

	case (key == K_LEFT)
		get:Left()
	case (key == K_CTRL_LEFT)
		get:WordLeft()

	case (key == K_BS)
		get:BackSpace()

	case (key == K_DEL)
		get:Delete()

	case (key == K_CTRL_Y)
		get:DelEnd()

	case (key == K_UNDO)
		get:Undo()

	case (key == K_CTRL_T)
		get:DelWordRight()

	case (key == K_CTRL_BS)
		get:DelWordLeft()

	otherwise

		if (key >= 32 .and. key <= 255)

			cKey := Chr(key)

			if (get:type == "N" .and. (cKey == "." .or. cKey == ","))
				/*
				IF ('- ' $ get:Buffer).OR.('-0' $ get:Buffer)
					get:ToDecPos()
					get:Left()
					get:Left()
					get:Insert(CHR(45))
					get:Insert(CHR(48))
				ELSE
					get:ToDecPos()
				ENDIF
				*/
				get:ToDecPos()
			else
				if ( Set(_SET_INSERT) )
					get:Insert(cKey)
				else
					get:Overstrike(cKey)
				end

				if (get:typeOut .and. !Set(_SET_CONFIRM) )
					if ( Set(_SET_BELL) )
						?? Chr(7)
					end

					get:exitState := GE_ENTER
				end

			end

		end
	endcase
	//dispend()
return
