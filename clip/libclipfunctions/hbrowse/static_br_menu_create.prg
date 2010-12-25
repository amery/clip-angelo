static function br_menu_Create(enable)
#define FA_NORMAL	0
#define FA_READONLY	1
#define FA_HIDDEN	2
#define FA_SYSTEM	4
#define FA_VOLUME	8
#define FA_DIRECTORY	16
#define FA_ARCHIVE	32
local oTopBar, oPopUp, oPopUp1, oItem
local keys:=HK_get("htmlBrowser"),kn
local fchs:="", a, b:={}, i, oPopUp2
	#ifdef OS_CYGWIN
	fchs := "C:/cygwin"
	#endif
	fchs += cliproot()+"/charsets/"+"*.tbl"
	a = fileseek(fchs, FA_ARCHIVE+FA_VOLUME+FA_SYSTEM+FA_HIDDEN+FA_READONLY)
	while !empty(a)
		a := left(a, len(a)-4)
		aadd(b, a)
		a = fileseek()
	enddo
	b := asort(b,,,{|x, y| x < y})

      oTopBar := TopBar( 0, 0, maxcol())
      oTopBar:ColorSpec := COLOR_MENU

      oPopUp := PopUp()
      oPopUp :ColorSpec:= COLOR_MENU
      oTopBar:AddItem( MenuItem ( [&File],oPopUp) )

      //oPopUp:AddItem( MenuItem( MENU_SEPARATOR ) )
      /*
      kn:=key_name(HK_get_key(keys,HASH_OpenNew))
      oItem :=MenuItem( [Load New URL   ]+kn ,{|| .t. }, ,[Load new URL], HASH_OpenNew)
      oPopUp:AddItem( oItem)
      */


      oPopUp2 := PopUp()
      oPopUp2:ColorSpec:= COLOR_MENU
      for i=1 to len(b)
	oItem :=MenuItem( b[i] ,{|| local(_1:=@b[i]), set("HV_CHARSET", _1) }, ,, HASH_SetCodePage)
	oPopUp2:AddItem( oItem)
      next
      oPopUp:AddItem( MenuItem([&Charset], oPopUp2) )

      kn:=key_name(HK_get_key(keys,HASH_Exit))
      oItem :=MenuItem( [E&xit   ]+kn ,{|| .t. }, ,[End of application], HASH_Exit)
      oPopUp:AddItem( oItem)

      oPopUp := PopUp()
      oPopUp :ColorSpec:= COLOR_MENU
      oTopBar:AddItem( MenuItem ( [&Control],oPopUp) )

      kn:=key_name(HK_get_key(keys,HASH_NextItem))
      oItem :=MenuItem( [&Next   ]+kn ,{|| .t. }, ,[Go next item], HASH_NextItem)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_PrevItem))
      oItem :=MenuItem( [&Prev   ]+kn ,{|| .t. }, ,[Go previous item], HASH_PrevItem)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_PrevUrl))
      oItem :=MenuItem( [P&revUrl   ]+kn ,{|| .t. }, ,[Go previous URL document], HASH_PrevUrl)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_NextUrl))
      oItem :=MenuItem( [Nex&tUrl   ]+kn ,{|| .t. }, ,[Go next URL document], HASH_NextUrl)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_InfoUrl))
      oItem :=MenuItem( [&InfoUrl   ]+kn ,{|| .t. }, ,[Get information about URL document], HASH_InfoUrl)
      oPopUp:AddItem( oItem)


      oPopUp := PopUp()
      oPopUp :ColorSpec:= COLOR_MENU
      oTopBar:AddItem( MenuItem ( [&Options],oPopUp) )

      kn:=key_name(HK_get_key(keys,HASH_Find))
      oItem :=MenuItem( [&Find   ]+kn ,{|| .t. }, ,[Find string], HASH_Find)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_ResumeFind))
      oItem :=MenuItem( [&Resume   ]+kn ,{|| .t. }, ,[Resume find string], HASH_ResumeFind)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_UnmarkBlock))
      oItem :=MenuItem( [&Unmark   ]+kn ,{|| .t. }, ,[Unmark selected block], HASH_UnmarkBlock)
      oPopUp:AddItem( oItem)

   return ( oTopBar)
