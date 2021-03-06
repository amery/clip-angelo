/*
    Copyright (C) 2005  ITK
    Author  : Elena V. Kornilova <alena@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg2.h"

#include <gtk/gtk.h>

#include "ci_clip-gtk2.ch"
#include "ci_clip-gtk2.h"

/*********************** SIGNALS **************************/

static SignalTable menu_tool_button_signals[] = {
   {"show-menu", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_SHOW_MENU_SIGNAL},
   {"", NULL, NULL, 0}
};

/* Register menu signals table in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_menu_tool_button()
{
   return GTK_TYPE_MENU_TOOL_BUTTON;
}

long
_clip_type_menu_tool_button()
{
   return GTK_WIDGET_MENU_TOOL_BUTTON;
}

const char *
_clip_type_name_menu_tool_button()
{
   return "GTK_WIDGET_MENU_TOOL_BUTTON";
}

int
clip_INIT___MENUTOOLBUTTON(ClipMachine * ClipMachineMemory)
{

   _wtype_table_put(_clip_type_menu_tool_button,
		    _clip_type_name_menu_tool_button,
		    _gtk_type_menu_tool_button, _gtk_type_tool_button, menu_tool_button_signals);
   return 0;
}

/**********************************************************/

int
clip_GTK_MENUTOOLBUTTONNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gchar    *label = _clip_parc(ClipMachineMemory, 3);

   GtkWidget *wid = NULL;

   GtkToolItem *item;

   C_widget *citem;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKCWIDOPT(cwid, GTK_IS_WIDGET);
   CHECKOPT(3, CHARACTER_type_of_ClipVarType);

   if (cwid)
      wid = GTK_WIDGET(cwid->widget);

   item = gtk_menu_tool_button_new(wid, label);
   citem = _register_widget(ClipMachineMemory, GTK_WIDGET(item), cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &citem->obj);
   return 0;
 err:
   return 1;
}

int
clip_GTK_MENUTOOLBUTTONNEWFROMSTOCK(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   gchar    *stock = _clip_parc(ClipMachineMemory, 2);

   GtkToolItem *item;

   C_widget *citem;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   item = gtk_menu_tool_button_new_from_stock(stock);
   citem = _register_widget(ClipMachineMemory, GTK_WIDGET(item), cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &citem->obj);
   return 0;
 err:
   return 1;
}

int
clip_GTK_MENUTOOLBUTTONSETMENU(ClipMachine * ClipMachineMemory)
{
   C_widget *cmenu_label = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cmnu = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(cmenu_label, GTK_IS_MENU_TOOL_BUTTON);
   CHECKCWID(cmnu, GTK_IS_WIDGET);

   gtk_menu_tool_button_set_menu(GTK_MENU_TOOL_BUTTON(cmenu_label->widget), GTK_WIDGET(cmnu->widget));

   return 0;
 err:
   return 1;
}

int
clip_GTK_MENUTOOLBUTTONGETMENU(ClipMachine * ClipMachineMemory)
{
   C_widget *cmenu_label = _fetch_cw_arg(ClipMachineMemory);

   GtkWidget *mnu;

   C_widget *cmnu;

   CHECKCWID(cmenu_label, GTK_IS_MENU_TOOL_BUTTON);

   mnu = gtk_menu_tool_button_get_menu(GTK_MENU_TOOL_BUTTON(cmenu_label->widget));

   if (mnu)
    {
       cmnu = _list_get_cwidget(ClipMachineMemory, mnu);
       if (!cmnu)
	  cmnu = _register_widget(ClipMachineMemory, mnu, NULL);
       if (cmnu)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cmnu->obj);
    }

   return 0;
 err:
   return 1;
}

int
clip_GTK_MENUTOOLBUTTONSETARROWTOOLTIP(ClipMachine * ClipMachineMemory)
{
   C_widget *cmenu_label = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cttips = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gchar    *text = _clip_parc(ClipMachineMemory, 3);

   gchar    *privatetext = _clip_parc(ClipMachineMemory, 4);

   CHECKCWID(cmenu_label, GTK_IS_MENU_TOOL_BUTTON);
   CHECKCWID(cttips, GTK_IS_WIDGET);
   CHECKARG(3, CHARACTER_type_of_ClipVarType);
   CHECKARG(4, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(text);
   LOCALE_TO_UTF(privatetext);
   gtk_menu_tool_button_set_arrow_tooltip(GTK_MENU_TOOL_BUTTON
					  (cmenu_label->widget), GTK_TOOLTIPS(cttips->widget), text, privatetext);
   FREE_TEXT(text);
   FREE_TEXT(privatetext);

   return 0;
 err:
   return 1;
}
