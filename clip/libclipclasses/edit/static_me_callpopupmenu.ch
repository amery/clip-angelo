************************************
static function me_CallPopupMenu(nWin)
local obj
local i,j, str, x1, x2, y1, y2, lb := .f., oPopup, oTopBar, oItem, hKey, a
	// mouse clicked into block
	if ::__l_tobj
		obj := ::__checkWindow(@nWin)
		if obj:strblock .or. obj:rectblock
			y1 := min(obj:koordblock[1], obj:koordblock[3])
			y2 := max(obj:koordblock[1], obj:koordblock[3])
			x1 := min(obj:koordblock[2], obj:koordblock[4])
			x2 := max(obj:koordblock[2], obj:koordblock[4])
		endif

		if obj:strblock .and. between(obj:line + mRow() - obj:rowWin, y1, y2 )
			lb := .t.
		elseif obj:rectblock  .and. between(obj:line + mRow() - obj:rowWin, y1, y2 );
			.and. between(obj:pos + mCol() - obj:colWin, x1, x2 )
			lb := .t.
		endif
	endif

	wopen(::nTop, ::nLeft, ::nBot, ::nRight, .f.)
	i := setcursor(0)
	a := {}
	if lb
		aadd(a, {[Copy to clipboard],[Copy to clipboard], HASH_CopyClipboard, .t.})
		aadd(a, {[Unmark],[Unmark block], HASH_CancelBlock, .t.})
		aadd(a, {[Cut],[Cut block from text], HASH_DeleteBlock, .t.})
	else
		aadd(a, {[Save    ],[Save file], HASH_SaveFile, iif(::__l_tobj,.t., .f.)})
		aadd(a, {[Open    ],[Open file], HASH_OpenFile, .t.})
		aadd(a, {[Close   ],[Close], HASH_CloseWindow, iif(::__l_tobj,.t., .f.)})
		aadd(a, {[List files],[List of opened files], HASH_ListFiles, .t.})
		aadd(a, {[Exit with save],[Exit with save status], HASH_ExitSave, .t.})
		aadd(a, {[Exit with out save],[Exit with out save status], HASH_ExitNoSave, .t.})
	endif
	oPopup := PopUp(mRow(), mCol())
	oPopUp :ColorSpec:= set("edit_colors_menu")

	for j=1 to len(a)
		oItem :=MenuItem(a[j][1],{|| .t. }, ,a[j][2], a[j][3])
		oPopup:AddItem( oItem)
		oPopup:EnableStatus(a[j][3], a[j][4])
	next
	oPopUp:__resize(mRow()-1, mCol())
	hKey := MenuModal(oPopup, 1)
	wclose()
	setcursor(i)
	if ::nWins >0
		wselect(::tobjinfo[::curwin]:curwin)
	endif

return hKey
