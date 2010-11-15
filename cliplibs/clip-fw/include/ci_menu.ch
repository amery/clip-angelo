#ifndef _MENU_CH
#define _MENU_CH

//----------------------------------------------------------------------------//
// Creating PullDown Menus from source code

#xcommand MENU [ <oMenu> ] ;
	     [ <popup: POPUP> ] ;
       => ;
	  [ <oMenu> := ] MenuBegin( <.popup.> )

#xcommand MENUITEM [ <oMenuItem> PROMPT ] [<cPrompt>] ;
	     [ MESSAGE <cMsg> ] ;
	     [ <checked: CHECK, CHECKED, MARK> ] ;
	     [ <enable: ENABLED, DISABLED> ] ;
	     [ <file: FILE, FILENAME, DISK> <cBmpFile> ] ;
	     [ <resource: RESOURCE, RESNAME, NAME> <cResName> ] ;
	     [ ACTION <uAction,...> ] ;
	     [ WHEN <wAction> ] ;
	     [ BLOCK <bAction> ] ;
	     [ <of: OF, MENU, SYSMENU> <oMenu> ] ;
	     [ ACCELERATOR <nState>, <nVirtKey> ] ;
	     [ <help: HELP> ] ;
	     [ <HelpId: HELP ID, HELPID> <nHelpId> ] ;
       => ;
	  [ <oMenuItem> := ] MenuAddItem( <cPrompt>, <cMsg>,;
	     <.checked.>, [ Upper(<(enable)>) == "ENABLED" ],;
	     [\{|oMenuItem|<uAction>\}],;
	     <cBmpFile>, <cResName>, <oMenu>, <bAction>, <nState>, <nVirtKey>,;
	     <.help.>, <nHelpId>)

	     //, [\{|oMenuItem|<wAction> \}])

// New MRU technology in FiveWin. See SAMPLES\TestMru.prg !!!

#xcommand MRU <oMru> ;
	     [ <Ini: INI, ININAME, FILENAME, NAME, DISK> <cIniFile> ] ;
	     [ SECTION <cSection> ] ;
	     [ <size: SIZE, ITEMS> <nItems> ] ;
	     [ MESSAGE <cMsg> ] ;
	     [ ACTION <uAction> ] ;
       => ;
	  <oMru> := TMru():New( <cIniFile>, <cSection>, <nItems>, <cMsg>,;
	     [{|cMruItem,oMenuItem|<uAction>}] )

#xcommand SEPARATOR => MenuAddItem()

#xcommand ENDMENU => MenuEnd()

//----------------------------------------------------------------------------//
// Creating PullDown Menus from resources

#xcommand DEFINE MENU <oMenu> ;
	     [ <res: RESOURCE, NAME, RESNAME> <cResName> ] ;
	     [ <popup: POPUP> ] ;
       => ;
	  <oMenu> := TMenu():ReDefine( <cResName>, <.popup.> )

#xcommand REDEFINE MENUITEM [ <oMenuItem> PROMPT <cPrompt> ] ;
	     [ ID <nId> <of: OF, MENU> <oMenu> ] ;
	     [ ACTION <uAction> ] ;
	     [ BLOCK  <bAction> ] ;
	     [ MESSAGE <cMsg> ] ;
	     [ <checked: CHECK, CHECKED, MARK> ] ;
	     [ <enable: ENABLED, DISABLED> ] ;
	     [ <file: FILE, FILENAME, DISK> <cBmpFile> ] ;
	     [ <resource: RESOURCE, RESNAME, NAME> <cResName> ] ;
	     [ ACCELERATOR <nState>, <nVirtKey> ] ;
	     [ <HelpId: HELP ID, HELPID> <nHelpId> ] ;
       => ;
	  [ <oMenuItem> := ] TMenuItem():ReDefine( <cPrompt>, <cMsg>,;
	     <.checked.>, [ Upper(<(enable)>) == "ENABLED" ], <{uAction}>,;
	     <cBmpFile>, <cResName>, <oMenu>, <bAction>, <nId>,;
	     <nState>, <nVirtKey>, <nHelpId>  )

//----------------------------------------------------------------------------//

#xcommand DEFINE MENU <oMenu> OF <oWnd> ;
       => ;
	  <oMenu> := TMenu():New( .f., <oWnd> )

#xcommand SET MENU OF <oWnd> TO <oMenu> => <oWnd>:SetMenu( <oMenu> )

//----------------------------------------------------------------------------//
// PopUps Management

#xcommand ACTIVATE <menu:POPUP,MENU> <oMenu> ;
	     [ AT <nRow>, <nCol> ] ;
	     [ <of: OF, WINDOW, DIALOG> <oWnd> ] ;
       => ;
	  <oMenu>:Activate( <nRow>, <nCol>, <oWnd> )

//----------------------------------------------------------------------------//
// Using Window System Menu

#xcommand REDEFINE SYSMENU [<oMenu>] ;
	     [ <of: OF, WINDOW, DIALOG> <oWnd> ] ;
       => ;
	  [<oMenu> :=] MenuBegin( .f., .t., <oWnd> )

#xcommand ENDSYSMENU => MenuEnd()

//----------------------------------------------------------------------------//

#endif
