/*
    Copyright (C) 2001  ITK
    Author  : Alexey M. Tkachenko <alexey@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg.h"

#include <gtk/gtk.h>
#include <string.h>

#include "ci_clip-gtk.ch"
#include "ci_clip-gtk.h"

/**********************************************************/
/* Signal handlers */
static gint
handle_checkmenuitem_draw_indicator_signal(GtkWidget * widget, GdkRectangle * area, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_X, area->x);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_Y, area->y);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_WIDTH, area->width);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_HEIGHT, area->height);
   INVOKESIGHANDLER(widget, cs, cv);
}

static int
emit_draw_indicator_signal(C_widget * cwid, const gchar * signal_name)
{
   ClipMachine *ClipMachineMemory = cwid->cmachine;
   ClipVar *marea = _clip_spar(cwid->cmachine, 3);
   GdkRectangle area;
   double d;
   CHECKARG(3, MAP_type_of_ClipVarType);
   _clip_mgetn(cwid->cmachine, marea, HASH_X, &d);
   area.x = d;
   _clip_mgetn(cwid->cmachine, marea, HASH_Y, &d);
   area.y = d;
   _clip_mgetn(cwid->cmachine, marea, HASH_WIDTH, &d);
   area.width = d;
   _clip_mgetn(cwid->cmachine, marea, HASH_HEIGHT, &d);
   area.height = d;
   gtk_signal_emit_by_name(GTK_OBJECT(cwid->widget), signal_name, &area, cwid);
   return 0;
 err:
   return 1;
}

/* Signals table */
static SignalTable check_menu_item_signals[] = {
   {"toggled", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_TOGGLED_SIGNAL},
   {"draw-indicator", GSF(handle_checkmenuitem_draw_indicator_signal),
    ESF(emit_draw_indicator_signal), GTK_DRAW_INDICATOR_SIGNAL},
   {"", NULL, NULL, 0}
};

/* Register check menu item signals table in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_check_menu_item()
{
   return GTK_TYPE_CHECK_MENU_ITEM;
}

long
_clip_type_check_menu_item()
{
   return GTK_WIDGET_CHECK_MENU_ITEM;
}

const char *
_clip_type_name_check_menu_item()
{
   return "GTK_WIDGET_CHECK_MENU_ITEM";
}

int
clip_INIT___CHECK_MENU_ITEM(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_check_menu_item, _clip_type_name_check_menu_item, _gtk_type_check_menu_item, _gtk_type_container, check_menu_item_signals);
   return 0;
}

/**********************************************************/

/**** Check menu item constructor ****/
int
clip_GTK_CHECKMENUITEMNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   char *label = _clip_parc(ClipMachineMemory, 2);
   char *pchar = _clip_parc(ClipMachineMemory, 3);
   GtkWidget *wid = NULL;
   C_widget *cwid;
   guint accel_key = 0;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT(3, CHARACTER_type_of_ClipVarType);

   if (pchar)
      {
	 char *pc;
	 for (pc = label; pc && *pc; pc++)
	    if (*pc == *pchar)
	       *pc = '_';
      }
   if (_clip_parinfo(ClipMachineMemory, 2) != CHARACTER_type_of_ClipVarType)
      wid = gtk_check_menu_item_new();
   else
      {
	 LOCALE_TO_UTF(label);
	 wid = gtk_check_menu_item_new_with_label(label);
	 if (pchar)
	    accel_key = gtk_label_parse_uline(GTK_LABEL(GTK_BIN(&(GTK_ITEM(wid)->bin))->child), label);
	 FREE_TEXT(label);
      }
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   _clip_mputn(ClipMachineMemory, &cwid->obj, HASH_ACCELKEY, accel_key);
   return 0;
 err:
   return 1;
}

/****  ------------------ ****/

int
clip_GTK_CHECKMENUITEMSETACTIVE(ClipMachine * ClipMachineMemory)
{
   C_widget *citm = _fetch_cw_arg(ClipMachineMemory);
   gboolean is_active = _clip_parl(ClipMachineMemory, 2);
   CHECKCWID(citm, GTK_IS_CHECK_MENU_ITEM);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      is_active = TRUE;
   gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(citm->widget), is_active);
   return 0;
 err:
   return 1;
}

int
clip_GTK_CHECKMENUITEMGETACTIVE(ClipMachine * ClipMachineMemory)
{
   C_widget *citm = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(citm, GTK_IS_CHECK_MENU_ITEM);
   _clip_retl(ClipMachineMemory, GTK_CHECK_MENU_ITEM(citm->widget)->active);
   return 0;
 err:
   return 1;
}

int
clip_GTK_CHECKMENUITEMSETSHOWTOGGLE(ClipMachine * ClipMachineMemory)
{
   C_widget *citm = _fetch_cw_arg(ClipMachineMemory);
   gboolean always = _clip_parl(ClipMachineMemory, 2);
   CHECKCWID(citm, GTK_IS_CHECK_MENU_ITEM);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      always = TRUE;
   gtk_check_menu_item_set_show_toggle(GTK_CHECK_MENU_ITEM(citm->widget), always);
   return 0;
 err:
   return 1;
}

int
clip_GTK_CHECKMENUITEMTOGGLED(ClipMachine * ClipMachineMemory)
{
   C_widget *citm = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(citm, GTK_IS_CHECK_MENU_ITEM);
   gtk_check_menu_item_toggled(GTK_CHECK_MENU_ITEM(citm->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_CHECKMENUITEMSETSTYLE(ClipMachine * ClipMachineMemory)
{
   C_widget *citm = _fetch_cw_arg(ClipMachineMemory);
   ClipVar *mstyle = _clip_spar(ClipMachineMemory, 2);
   GtkStyle *style;
   GtkWidget *wid;
   CHECKCWID(citm, GTK_IS_ITEM);
   CHECKARG(2, MAP_type_of_ClipVarType);
   wid = GTK_BIN(&(GTK_ITEM(citm->widget)->bin))->child;
   style = gtk_style_copy(wid->style);
  //gtk_style_unref(wid->style);
   _map_to_style(ClipMachineMemory, mstyle, style);
   gtk_widget_set_style(wid, style);
   return 0;
 err:
   return 1;
}
