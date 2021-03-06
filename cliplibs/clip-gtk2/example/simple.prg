#include <ci_clip-gtk2.ch>

static hb

function main()
	local w,f,vb,sw,qbtn,st, menu1

	/* Initialize GTK+ system */
	gtk_init()

	/* Create main window */
	/***************************************************/
	w:= gtk_WindowNew(,"Simple GTK+ application")
	gtk_WindowSetPosition(w,GTK_WIN_POS_CENTER_ON_PARENT)
	gtk_WindowSetDefaultSize(w,500,300)
	gtk_SignalConnect(w,"delete-event",{|wid,e|gtk_Quit()})

	/* Create vertical box container */
	vb:= gtk_VBoxNew()
	gtk_ContainerAdd (w, vb)

	/* Create horizontal box container */
	hb:= gtk_HBoxNew()
	gtk_WidgetSetSizeRequest(hb,600,25)
	gtk_BoxPackStart(vb,hb)

	/* Create scrolled window container */
	sw:= gtk_ScrolledWindowNew()
	gtk_ScrolledWindowSetPlacement(sw,GTK_CORNER_TOP_LEFT)
	gtk_BoxPackStart(vb,sw,.t.,.t.)

	/* Create layout container */
	f:= gtk_LayoutNew()
	gtk_WidgetSetSizeRequest(f,700,1000)
	gtk_ContainerSetBorderWidth(f,20)
	gtk_ScrolledWindowAddWithViewport (sw, f)

	/* Create quit button */
	qbtn:=gtk_ButtonNew(,"Quit from application")
	gtk_LayoutPut (f, qbtn, 160, 110)
	gtk_WindowSetDefault(w, qbtn)
	gtk_SignalConnect(qbtn,GTK_CLICKED_SIGNAL,{||gtk_Quit()})
	gtk_WidgetAddAccelerator( qbtn, "clicked", w, GDK_q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE )

	/* Menu */
	menu1 := CreateMenu()

	/* Statusbar */
	st := gtk_StatusBarNew()
	gtk_BoxPackEnd(vb,st,.F.,.F.)
	//gtk_SignalConnect(vb, "button-press-event", {|w, e| buttonPress(w, e)})
	//gtk_SignalConnect(vb, "button-release-event", {|w, e| buttonRelease(w, e)})
	//gtk_SignalConnect(vb, "event", {|w, e| event(w, e)})
	gtk_SignalConnect(vb, GTK_2BUTTON_PRESS, {|w, e| qout("press 2"), gtk_MenuPopUp(menu1)})
	gtk_SignalConnect(vb, "button-press-event", {|w, e| qout("press 1")})
	gtk_StatusBarSetText(st,"This is a status bar")

	/* Show main window */
	/***************************************************/
	gtk_WidgetShowAll( w )

	/* Do GTK+ events */
	gtk_main()

return
static function event(wid,  ev)
        do case

        case ev:EVENT == GTK_BUTTON_PRESS
        	qout("GTK_BUTTON_PRESS")
                return .T.
        case ev:EVENT == GTK_2BUTTON_PRESS
        	qout("GTK_2BUTTON_PRESS")
                return .t.
        case ev:EVENT == GTK_BUTTON_RELEASE
        	qout("GTK_BUTTON_RELEASE")
                return .f.
        endcase
return .t.
static function buttonPress(wid, ev)
	qout("event type", ev:TYPE)

return .t.
static function buttonRelease(wid, ev)
	qout("release event type", ev:TYPE)

return .t.
static function CreateMenu()
	local menubar, menubar2
	local menu1i, menu2i, menu1, menu2
	local item1, item2, item3, item4, item5, hitem
	local rmg, radioitem1, radioitem2, radioitem3, checkitem

	/***************************************************/
	/* Prepare menu bar */
	menubar:=gtk_MenuBarNew()
	//gtk_MenuBarSetShadowType(menubar, GTK_SHADOW_IN)
	gtk_BoxPackStart (hb, menubar)

	/* Create submenu item */
	menu1i:=gtk_MenuItemNew(,"menu1")

	/* Create submenu */
	menu1:=gtk_MenuNew()

	/* Create menu items */
	item1:=gtk_MenuItemNew(,"Item 1")
	item2:=gtk_MenuItemNew(,"Item 2")
	item3:=gtk_MenuItemNew(,"Item 3")
	item4:=gtk_MenuItemNew(,"Item 4")
	item5:=gtk_MenuItemNew(,"Item 5")

	/* Create radio menu group */
	rmg:=gtk_RadioMenuGroupNew()

	/* Create radio menu items */
	radioitem1:=gtk_RadioMenuItemNew(,rmg,"RadioItem1")
	radioitem2:=gtk_RadioMenuItemNew(,rmg,"RadioItem2")
	gtk_CheckMenuItemSetActive(radioitem2)
	radioitem3:=gtk_RadioMenuItemNew(,,"RadioItem3")
	gtk_RadioMenuItemSetGroup(radioitem3,rmg)

	/* Create check menu item */
	checkitem:=gtk_CheckMenuItemNew(,"Check Item")
	gtk_CheckMenuItemSetActive(checkitem)

	/* Create submenu 2 item */
	menu2i = gtk_MenuItemNew(,"menu2")

	/* Create submenu 2 */
	menu2:=gtk_MenuNew()
	gtk_MenuAppend(menu2, item4)
	gtk_MenuAppend(menu2, item5)

	/* Create menubar */
	gtk_MenuAppend(menu1, item1)
	gtk_MenuAppend(menu1, item2)
	gtk_MenuAppend(menu1, radioitem1)
	gtk_MenuAppend(menu1, radioitem2)
	gtk_MenuAppend(menu1, radioitem3)
	gtk_MenuAppend(menu1, checkitem)
	gtk_MenuAppend(menu1, menu2i)
	gtk_MenuItemSetSubmenu(menu2i,menu2)
	gtk_MenuBarAppend(menubar,menu1i)
	gtk_MenuItemSetSubmenu(menu1i,menu1)
	gtk_MenuBarAppend(menubar,item3)

	/* Create help menu item */
	hitem:=gtk_MenuItemNew(,"Help")

	/* Create menu bar 2 */
	menubar2:=gtk_MenuBarNew()
	gtk_MenuBarAppend(menubar2,hitem)
	gtk_BoxPackEnd (hb, menubar2)
return  menu1
