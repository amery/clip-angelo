static function db_menu_Create(enable)
local oTopBar, oPopUp, oPopUp1, oItem
local keys:=HK_get("dialog"),kn

      oTopBar := TopBar( 0, 0, maxcol())
      oTopBar:ColorSpec := set("dbox_colors_menu")

      oPopUp := PopUp()
      oPopUp :ColorSpec:= set("dbox_colors_menu")
      oTopBar:AddItem( MenuItem ( [&File],oPopUp) )

      /*
      kn:=key_name(HK_get_key(keys,HASH_Print))
      oItem :=MenuItem( [&Print...           ]+kn ,{|| .t. }, ,[Print a file], HASH_Print)
      oItem:Enabled := .f.//enable
      oPopUp:AddItem( oItem)
      */

      //oPopUp:AddItem( MenuItem( MENU_SEPARATOR ) )

      kn:=key_name(HK_get_key(keys,HASH_Exit))
      oItem :=MenuItem( [E&xit   ]+kn ,{|| .t. }, ,[End of application], HASH_Exit)
      oPopUp:AddItem( oItem)

      oPopUp := PopUp()
      oPopUp :ColorSpec:= set("dbox_colors_menu")
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
