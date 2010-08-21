************************************************
static function me_menu_Create()
local oTopBar, oPopUp, oPopUp1, oItem
local keys:=HK_get("edit"),kn

      oTopBar := TopBar( 0, 0, maxcol())
      oTopBar:ColorSpec := set("edit_colors_menu")

      oPopUp := PopUp()
      oPopUp :ColorSpec:= set("edit_colors_menu")
      oTopBar:AddItem( MenuItem ( [&File],oPopUp,,,HASH_ItemFile) )

      kn:=key_name(HK_get_key(keys,HASH_CreateNewFile))
      oItem :=MenuItem( [&New                ]+kn ,{|| .t. }, ,[Create a new file], HASH_CreateNewFile)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_OpenFile))
      oItem :=MenuItem( [&Open...            ]+kn ,{|| .t. }, ,[Open a file], HASH_OpenFile)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_SaveFile))
      oItem :=MenuItem( [&Save               ]+kn ,{|| .t. }, ,[Save a file], HASH_SaveFile)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_SaveAsFile))
      oItem :=MenuItem( [&Save as...         ]+kn ,{|| .t. }, ,[Save a file as...], HASH_SaveAsFile)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_Reload))
      oItem :=MenuItem( [&Reload file        ]+kn ,{|| .t. }, ,[Reload a file], HASH_Reload)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_ReadOnly))
      oItem :=MenuItem( [Read on&ly          ]+kn ,{|| .t. }, ,[Read only options], HASH_ReadOnly)
      oPopUp:AddItem( oItem)

      oPopUp:AddItem( MenuItem( MENU_SEPARATOR ) )

      kn:=key_name(HK_get_key(keys,HASH_Print))
      oItem :=MenuItem( [&Print...           ]+kn ,{|| .t. }, ,[Print a file], HASH_Print)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_PrintBlock))
      oItem :=MenuItem( [Print &block...     ]+kn ,{|| .t. }, ,[Print marked block], HASH_PrintBlock)
      oPopUp:AddItem( oItem)

      oPopUp:AddItem( MenuItem( MENU_SEPARATOR ) )

      kn:=key_name(HK_get_key(keys,HASH_ExitSave))
      oItem :=MenuItem( [E&xit with save     ]+kn ,{|| .t. }, ,[End of application and save options], HASH_ExitSave)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_ExitNoSave))
      oItem :=MenuItem( [Exit with-out sa&ve ]+kn ,{|| .t. }, ,[End of application and not save options], HASH_ExitNoSave)
      oPopUp:AddItem( oItem)

**************************************************************************

      oPopUp := PopUp()
      oPopUp :ColorSpec:= set("edit_colors_menu")
      oTopBar:AddItem( MenuItem ( [&Window],oPopUp,,,HASH_ItemWindow) )

      kn:=key_name(HK_get_key(keys,HASH_ListFiles))
      oItem :=MenuItem( [&List    ]+kn ,{|| .t. }, ,[List of windows], HASH_ListFiles)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_NextWindow))
      oItem :=MenuItem( [&Next    ]+kn ,{|| .t. }, ,[Next window], HASH_NextWindow)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_PrevWindow))
      oItem :=MenuItem( [&Prev    ]+kn ,{|| .t. }, ,[Prevoice window], HASH_PrevWindow)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_CloseWindow))
      oItem :=MenuItem( [&Close   ]+kn ,{|| .t. }, ,[Close window], HASH_CloseWindow)
      oPopUp:AddItem( oItem)

**************************************************************************

      oPopUp := PopUp()
      oPopUp :ColorSpec:= set("edit_colors_menu")
      oTopBar:AddItem( MenuItem ( [&Block],oPopUp,,, HASH_ItemBlock) )

      kn:=key_name(HK_get_key(keys,HASH_MarkBlockString))
      oItem :=MenuItem( [&Line block marker    ]+kn ,{|| .t. }, ,[Start/stop line block marker], HASH_MarkBlockString)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_MarkBlockRect))
      oItem :=MenuItem( [&Column block marker  ]+kn ,{|| .t. }, ,[Start/stop columns block marker], HASH_MarkBlockRect)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_CancelBlock))
      oItem :=MenuItem( [&Unmark               ]+kn ,{|| .t. }, ,[Unmark block marker], HASH_CancelBlock)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_CopyBlock))
      oItem :=MenuItem( [Co&py                 ]+kn ,{|| .t. }, ,[Copy block ], HASH_CopyBlock)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_MoveBlock))
      oItem :=MenuItem( [&Move                 ]+kn ,{|| .t. }, ,[Move block ], HASH_MoveBlock)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_DeleteBlock))
      oItem :=MenuItem( [&Delete               ]+kn ,{|| .t. }, ,[Delete block], HASH_DeleteBlock)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_CopyClipboard))
      oItem :=MenuItem( [Copy to clip&board    ]+kn ,{|| .t. }, ,[Copy block to clipboard], HASH_CopyClipboard)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_PasteClipboard))
      oItem :=MenuItem( [Pa&ste from clipboard ]+kn ,{|| .t. }, ,[Paste block from clipboard], HASH_PasteClipboard)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_SaveBlock))
      oItem :=MenuItem( [&Save to file         ]+kn ,{|| .t. }, ,[Save block to file], HASH_SaveBlock)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_LoadBlock))
      oItem :=MenuItem( [&Load from file       ]+kn ,{|| .t. }, ,[Load block from file], HASH_LoadBlock)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_PrintBlock))
      oItem :=MenuItem( [&Print...           ]+kn ,{|| .t. }, ,[Print marked block], HASH_PrintBlock)
      oPopUp:AddItem( oItem)


      //oPopUp:AddItem( MenuItem( MENU_SEPARATOR ) )
**************************************************************************

      oPopUp := PopUp()
      oPopUp :ColorSpec:= set("edit_colors_menu")
      oTopBar:AddItem( MenuItem ( [&Service],oPopUp,,, HASH_ItemService) )

      kn:=key_name(HK_get_key(keys,HASH_InsMode))
      oItem :=MenuItem( [&Insert mode    ]+kn ,{|| .t. },,[On/off insert mode ], HASH_InsMode)
      oItem:Checked := Set(_SET_INSERT)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_FindString))
      oItem :=MenuItem( [Find &string    ]+kn ,{|| .t. }, ,[Find string ], HASH_FindString)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_FindNext))
      oItem :=MenuItem( [Find &next      ]+kn ,{|| .t. }, ,[Find next ], HASH_FindNext)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_FindPrev))
      oItem :=MenuItem( [Find pre&vious  ]+kn ,{|| .t. }, ,[Find previous ], HASH_FindPrev)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_FindReplace))
      oItem :=MenuItem( [&Replace        ]+kn ,{|| .t. }, ,[Find and replace string ], HASH_FindReplace)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_MatchSymbolForward))
      oItem :=MenuItem( [&Match symbol forward  ]+kn ,{|| .t. }, ,[Match identity symbol in forward ], HASH_MatchSymbolForward)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_MatchSymbolBackward))
      oItem :=MenuItem( [Match symbol &backward ]+kn ,{|| .t. }, ,[Match identity symbol in backward ], HASH_MatchSymbolBackward)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_MatchStructForward))
      oItem :=MenuItem( [Match structure forward  ]+kn ,{|| .t. }, ,[Match Clipper structure to forward ], HASH_MatchStructForward)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_MatchStructBackward))
      oItem :=MenuItem( [Match structure backward ]+kn ,{|| .t. }, ,[Match Clipper structure to backward ], HASH_MatchStructBackward)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_Undo))
      oItem :=MenuItem( [&Undo           ]+kn ,{|| .t. },,[Undo operation], HASH_Undo)
      oPopUp:AddItem( oItem)


**************************************************************************

      oPopUp := PopUp()
      oPopUp :ColorSpec:= set("edit_colors_menu")
      oTopBar:AddItem( MenuItem ( [Forma&t],oPopUp,,, HASH_ItemFormat) )

      kn:=key_name(HK_get_key(keys,HASH_FormatLine))
      oItem :=MenuItem( [Format &line    ]+kn ,{|| .t. },,[Format line], HASH_FormatLine)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_FormatPart))
      oItem :=MenuItem( [Format &part    ]+kn ,{|| .t. },,[Format part], HASH_FormatPart)
      oPopUp:AddItem( oItem)

**************************************************************************

      oPopUp := PopUp()
      oPopUp :ColorSpec:= set("edit_colors_menu")
      oTopBar:AddItem( MenuItem ( [O&therwise],oPopUp,,, HASH_ItemOtherwise) )

      kn:=key_name(HK_get_key(keys,HASH_Template))
      oItem :=MenuItem( [&Template       ]+kn ,{|| .t. }, ,[Template ], HASH_Template)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_Macro))
      oItem :=MenuItem( [Template &macro ] ,{|| .t. }, ,[Template macro ], HASH_Macro)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_SingleGraphic))
      oItem :=MenuItem( [&Single graphic ]+kn ,{|| .t. }, ,[On/Off single graphic mode ], HASH_SingleGraphic)
      oItem:Checked := ::single_mode
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_DoubleGraphic))
      oItem :=MenuItem( [&Double graphic ]+kn ,{|| .t. }, ,[On/Off double graphic mode  ], HASH_DoubleGraphic)
      oItem:Checked := ::double_mode
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_ChooseCodePage))
      oItem :=MenuItem( [&Choose code page ]+kn ,{|| .t. }, ,[Choose code page  ], HASH_ChooseCodePage)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_Options))
      oItem :=MenuItem( [&Options...     ]+kn ,{|| .t. },,[Set options], HASH_Options)
      oPopUp:AddItem( oItem)

**************************************************************************

      oPopUp := PopUp()
      oPopUp :ColorSpec:= set("edit_colors_menu")
      oTopBar:AddItem( MenuItem ( [Info],oPopUp,,, HASH_ItemInfo) )

      kn:=key_name(HK_get_key(keys,HASH_HelpEdit))
      oItem :=MenuItem( [&Help       ]+kn ,{|| .t. }, ,[Help page], HASH_HelpEdit)
      oPopUp:AddItem( oItem)

      kn:=key_name(HK_get_key(keys,HASH_About))
      oItem :=MenuItem( [&About ] ,{|| .t. }, ,[About program ], HASH_About)
      oPopUp:AddItem( oItem)

   return ( oTopBar)
