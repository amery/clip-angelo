/*
    Copyright (C) 2001  ITK
    Author  : Alexey M. Tkachenko <alexey@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include "hashcode.h"
#include "clip.h"
#include "clip-gtkcfg.h"

#include <gtk/gtk.h>

#include "clip-gtk.ch"
#include "clip-gtk.h"

/**********************************************************/
/* Signals table */
static SignalTable menu_item_signals[] =
{
	{"activate",  		GSF( widget_signal_handler ), ESF( object_emit_signal ), GTK_ACTIVATE_SIGNAL},
	{"activate-item",  	GSF( widget_signal_handler ), ESF( object_emit_signal ), GTK_ACTIVATE_ITEM_SIGNAL},
	{"", NULL, NULL, 0}
};

/* Register menu item signals table in global table */
CLIP_DLLEXPORT GtkType _gtk_type_menu_item() { return GTK_TYPE_MENU_ITEM; }
long _clip_type_menu_item() { return GTK_WIDGET_MENU_ITEM; }
const char * _clip_type_name_menu_item() { return "GTK_WIDGET_MENU_ITEM"; }

CLIP_DLLEXPORT GtkType _gtk_type_tearoff_menu_item() { return GTK_TYPE_TEAROFF_MENU_ITEM; }
long _clip_type_tearoff_menu_item() { return GTK_WIDGET_TEAROFF_MENU_ITEM; }
const char * _clip_type_name_tearoff_menu_item() { return "GTK_WIDGET_TEAROFF_MENU_ITEM"; }

int
clip_INIT___MENU_ITEM(ClipMachine *cm)
{
	_wtype_table_put(_clip_type_menu_item, _clip_type_name_menu_item, _gtk_type_menu_item, _gtk_type_item, menu_item_signals);
	return 0;
}

int
clip_INIT___TEAROFF_MENU_ITEM(ClipMachine *cm)
{
	_wtype_table_put(_clip_type_tearoff_menu_item, _clip_type_name_tearoff_menu_item, _gtk_type_tearoff_menu_item, _gtk_type_menu_item, NULL);
	return 0;
}
/**********************************************************/

/****  Menu item constructor ****/
int
clip_GTK_MENUITEMNEW(ClipMachine * cm)
{
	ClipVar * cv   = _clip_spar(cm, 1);
	char * label   = _clip_parc(cm, 2);
        char * pchar   = _clip_parc(cm, 3);
	GtkWidget *wid = NULL;
        C_widget *cwid;
        guint accel_key = 0;
	CHECKOPT(1,MAP_t);
	CHECKOPT(2,CHARACTER_t); CHECKOPT(3,CHARACTER_t);

	if (pchar)
        {
        	char *pc;
        	for (pc = label;pc && *pc; pc++)
                	if (*pc == *pchar)
                        	*pc='_';
        }
	if (_clip_parinfo(cm,2)!=CHARACTER_t)
		wid = gtk_menu_item_new();
        else
        {
		LOCALE_TO_UTF(label);
		wid = gtk_menu_item_new_with_label(label);
                if (pchar)
                	accel_key = gtk_label_parse_uline(GTK_LABEL(GTK_BIN(&(GTK_ITEM(wid)->bin))->child),
                        	label);
        	FREE_TEXT(label);
	}
        if (!wid) goto err;
	cwid = _register_widget(cm, wid, cv);
	_clip_mclone(cm,RETPTR(cm),&cwid->obj);
        _clip_mputn(cm,&cwid->obj,HASH_ACCELKEY,accel_key);
	return 0;
err:
	return 1;
}
/****  ------------------ ****/

/****  Tearoff menu item constructor ****/
int
clip_GTK_TEAROFFMENUITEMNEW(ClipMachine * cm)
{
	ClipVar * cv   = _clip_spar(cm, 1);
	GtkWidget *wid = NULL;
        C_widget *cwid;
	CHECKOPT(1,MAP_t);

	wid = gtk_tearoff_menu_item_new();
        if (!wid) goto err;
	cwid = _register_widget(cm, wid, cv);
	_clip_mclone(cm,RETPTR(cm),&cwid->obj);
	return 0;
err:
	return 1;
}
/****  ------------------ ****/

int
clip_GTK_MENUITEMSETSUBMENU(ClipMachine * cm)
{
	C_widget *citm = _fetch_cw_arg(cm);
	C_widget *cwid = _fetch_cwidget(cm,_clip_spar(cm,2));
        CHECKCWID(citm,GTK_IS_MENU_ITEM);
	CHECKARG2(2,MAP_t,NUMERIC_t); CHECKCWID(cwid,GTK_IS_WIDGET);
        gtk_menu_item_set_submenu(GTK_MENU_ITEM(citm->widget), cwid->widget);
	return 0;
err:
	return 1;
}

int
clip_GTK_MENUITEMREMOVESUBMENU(ClipMachine * cm)
{
	C_widget *citm = _fetch_cw_arg(cm);
        CHECKCWID(citm,GTK_IS_MENU_ITEM);
        gtk_menu_item_remove_submenu(GTK_MENU_ITEM(citm->widget));
	return 0;
err:
	return 1;
}

int
clip_GTK_MENUITEMSETPLACEMENT(ClipMachine * cm)
{
	C_widget *citm = _fetch_cw_arg(cm);
        gint placement = _clip_parni(cm,2);
        CHECKCWID(citm,GTK_IS_MENU_ITEM);
        CHECKOPT(2,NUMERIC_t);
        gtk_menu_item_set_placement(GTK_MENU_ITEM(citm->widget),placement);
	return 0;
err:
	return 1;
}

int
clip_GTK_MENUITEMCONFIGURE(ClipMachine * cm)
{
	C_widget *citm = _fetch_cw_arg(cm);
        gint show_toggle_indicator = _clip_parl(cm,2);
        gint show_submenu_indicator = _clip_parl(cm,3);
        CHECKCWID(citm,GTK_IS_MENU_ITEM);
        CHECKOPT(2,LOGICAL_t); CHECKOPT(3,LOGICAL_t);
        if (_clip_parinfo(cm,2)==UNDEF_t) show_toggle_indicator = TRUE;
        if (_clip_parinfo(cm,3)==UNDEF_t) show_submenu_indicator = TRUE;
        gtk_menu_item_configure(GTK_MENU_ITEM(citm->widget),show_toggle_indicator,show_submenu_indicator);
	return 0;
err:
	return 1;
}

int
clip_GTK_MENUITEMSELECT(ClipMachine * cm)
{
	C_widget *citm = _fetch_cw_arg(cm);
        CHECKCWID(citm,GTK_IS_ITEM);
        gtk_menu_item_select(GTK_MENU_ITEM(citm->widget));
	return 0;
err:
	return 1;
}

int
clip_GTK_MENUITEMDESELECT(ClipMachine * cm)
{
	C_widget *citm = _fetch_cw_arg(cm);
        CHECKCWID(citm,GTK_IS_ITEM);
        gtk_menu_item_deselect(GTK_MENU_ITEM(citm->widget));
	return 0;
err:
	return 1;
}

int
clip_GTK_MENUITEMACTIVATE(ClipMachine * cm)
{
	C_widget *citm = _fetch_cw_arg(cm);
        CHECKCWID(citm,GTK_IS_ITEM);
        gtk_menu_item_activate(GTK_MENU_ITEM(citm->widget));
	return 0;
err:
	return 1;
}

int
clip_GTK_MENUITEMRIGHTJUSTIFY(ClipMachine * cm)
{
	C_widget *citm = _fetch_cw_arg(cm);
        CHECKCWID(citm,GTK_IS_ITEM);
        gtk_menu_item_right_justify(GTK_MENU_ITEM(citm->widget));
	return 0;
err:
	return 1;
}

int
clip_GTK_MENUITEMSETSTYLE(ClipMachine * cm)
{
	C_widget   *citm = _fetch_cw_arg(cm);
	ClipVar  *mstyle = _clip_spar(cm,2);
	GtkStyle *style;
        GtkWidget *wid;
        CHECKCWID(citm,GTK_IS_ITEM);
	CHECKARG(2,MAP_t);
        wid = GTK_BIN(&(GTK_ITEM(citm->widget)->bin))->child;
	style = gtk_style_copy(wid->style);
        //gtk_style_unref(wid->style);
	_map_to_style(cm, mstyle, style);
	gtk_widget_set_style (wid, style);
	return 0;
err:
	return 1;
}

