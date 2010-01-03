*********************************
static function me___addMacroToMenu(par, mstr, fnc, cmd, hk)
static l_m := .f.
static pitem
local mitem, kn
	if empty(mstr)
		mstr := fnc
	endif
	if !l_m
		pitem := PopUp()
		pitem :ColorSpec:= set("edit_colors_menu")
		::oMenu:AddItem( MenuItem ( [Macros],pitem,,,HASH_ItemMacros) )
		l_m := .t.
	endif

	kn:=key_name(hk)
	mItem :=MenuItem( mstr+" "+kn ,{|| .t. }, ,::__iniMacro:getValue(par, "UMENUMSG"), cmd)
	pitem:AddItem( mItem)
return .t.
**********************************
