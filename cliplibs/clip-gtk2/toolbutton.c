/*
    Copyright (C) 2004  ITK
    Author  : Elena V. Kornilova <alena@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include <string.h>
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg2.h"

#include <gtk/gtk.h>

#include "ci_clip-gtk2.ch"
#include "ci_clip-gtk2.h"

/*********************** SIGNALS **************************/

/* Signals table */
static SignalTable toolbutton_signals[] = {
   {"clicked", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_CLICKED_SIGNAL},
   {"", NULL, NULL, 0}
};

static SignalTable toggle_toolbutton_signals[] = {
   {"toggled", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_TOGGLED_SIGNAL},
   {"", NULL, NULL, 0}
};

/**********************************************************/
CLIP_DLLEXPORT GtkType
_gtk_type_tool_button()
{
   return GTK_TYPE_TOOL_BUTTON;
}

long
_clip_type_tool_button()
{
   return GTK_WIDGET_TOOL_BUTTON;
}

const char *
_clip_type_name_tool_button()
{
   return "GTK_WIDGET_TOOL_BUTTON";
}

CLIP_DLLEXPORT GtkType
_gtk_type_toggle_tool_button()
{
   return GTK_TYPE_TOGGLE_TOOL_BUTTON;
}

long
_clip_type_toggle_tool_button()
{
   return GTK_WIDGET_TOGGLE_TOOL_BUTTON;
}

const char *
_clip_type_name_toggle_tool_button()
{
   return "GTK_WIDGET_TOGGLE_TOOL_BUTTON";
}

CLIP_DLLEXPORT GtkType
_gtk_type_radio_tool_button()
{
   return GTK_TYPE_RADIO_TOOL_BUTTON;
}

long
_clip_type_radio_tool_button()
{
   return GTK_WIDGET_TOGGLE_TOOL_BUTTON;
}

const char *
_clip_type_name_radio_tool_button()
{
   return "GTK_WIDGET_RADIO_TOOL_BUTTON";
}

/* Register toolitem in global table */
int
clip_INIT___TOOLBUTTON(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_tool_button, _clip_type_name_tool_button,
		    _gtk_type_tool_button, _gtk_type_tool_item, toolbutton_signals);
   _wtype_table_put(_clip_type_toggle_tool_button,
		    _clip_type_name_toggle_tool_button,
		    _gtk_type_toggle_tool_button, _gtk_type_tool_button, toggle_toolbutton_signals);
   _wtype_table_put(_clip_type_radio_tool_button,
		    _clip_type_name_radio_tool_button, _gtk_type_radio_tool_button, _gtk_type_tool_button, NULL);
   return 0;
}

/**** ------------------ ****/
int
clip_GTK_TOOLBUTTONNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gchar    *label = _clip_parc(ClipMachineMemory, 3);

   GtkToolItem *item;

   C_widget *cbutton;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT(2, MAP_type_of_ClipVarType);
   CHECKCWIDOPT(cwid, GTK_IS_WIDGET);
   CHECKOPT(3, CHARACTER_type_of_ClipVarType);

   if (label)
      LOCALE_TO_UTF(label);
   item = gtk_tool_button_new((cwid) ? GTK_WIDGET(cwid->widget) : NULL, (label) ? label : NULL);
   if (label)
      FREE_TEXT(label);
   if (!item)
      goto err;
   cbutton = _register_widget(ClipMachineMemory, GTK_WIDGET(item), cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cbutton->obj);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TOOLBUTTONNEWFROMSTOCK(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   gchar    *label = _clip_parc(ClipMachineMemory, 2);

   GtkToolItem *item;

   C_widget *cbutton;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   item = gtk_tool_button_new_from_stock(label);

   if (!item)
      goto err;
   cbutton = _register_widget(ClipMachineMemory, GTK_WIDGET(item), cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cbutton->obj);
   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

int
clip_GTK_TOOLBUTTONSETLABEL(ClipMachine * ClipMachineMemory)
{
   C_widget *cbutton = _fetch_cw_arg(ClipMachineMemory);

   gchar    *label = _clip_parc(ClipMachineMemory, 2);

   CHECKCWID(cbutton, GTK_IS_TOOL_BUTTON);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(label);
   gtk_tool_button_set_label(GTK_TOOL_BUTTON(cbutton->widget), label);
   FREE_TEXT(label);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TOOLBUTTONGETLABEL(ClipMachine * ClipMachineMemory)
{
   C_widget *cbutton = _fetch_cw_arg(ClipMachineMemory);

   gchar    *label;

   CHECKCWID(cbutton, GTK_IS_TOOL_BUTTON);

   label = (gchar *) gtk_tool_button_get_label(GTK_TOOL_BUTTON(cbutton->widget));
   if (label)
    {
       LOCALE_FROM_UTF(label);
       _clip_retc(ClipMachineMemory, label);
       FREE_TEXT(label);
    }

   return 0;
 err:
   return 1;
}

int
clip_GTK_TOOLBUTTONSETUSEUNDERLINE(ClipMachine * ClipMachineMemory)
{
   C_widget *cbutton = _fetch_cw_arg(ClipMachineMemory);

   gboolean  set = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(cbutton, GTK_IS_TOOL_BUTTON);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);

   gtk_tool_button_set_use_underline(GTK_TOOL_BUTTON(cbutton->widget), set);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TOOLBUTTONGETUSEUNDERLINE(ClipMachine * ClipMachineMemory)
{
   C_widget *cbutton = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cbutton, GTK_IS_TOOL_BUTTON);

   _clip_retl(ClipMachineMemory, gtk_tool_button_get_use_underline(GTK_TOOL_BUTTON(cbutton->widget)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_TOOLBUTTONSETSTOCKID(ClipMachine * ClipMachineMemory)
{
   C_widget *cbutton = _fetch_cw_arg(ClipMachineMemory);

   gchar    *stock = _clip_parc(ClipMachineMemory, 2);

   CHECKCWID(cbutton, GTK_IS_TOOL_BUTTON);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);

   gtk_tool_button_set_stock_id(GTK_TOOL_BUTTON(cbutton->widget), stock);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TOOLBUTTONGETSTOCKID(ClipMachine * ClipMachineMemory)
{
   C_widget *cbutton = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cbutton, GTK_IS_TOOL_BUTTON);

   _clip_retc(ClipMachineMemory, (gchar *) gtk_tool_button_get_stock_id(GTK_TOOL_BUTTON(cbutton->widget)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_TOOLBUTTONSETICONWIDGET(ClipMachine * ClipMachineMemory)
{
   C_widget *cbutton = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(cbutton, GTK_IS_TOOL_BUTTON);
   CHECKCWID(cwid, GTK_IS_WIDGET);

   gtk_tool_button_set_icon_widget(GTK_TOOL_BUTTON(cbutton->widget), GTK_WIDGET(cwid->widget));

   return 0;
 err:
   return 1;
}

int
clip_GTK_TOOLBUTTONGETICONWIDGET(ClipMachine * ClipMachineMemory)
{
   C_widget *cbutton = _fetch_cw_arg(ClipMachineMemory);

   GtkWidget *wid;

   C_widget *cwid;

   CHECKCWID(cbutton, GTK_IS_TOOL_BUTTON);

   wid = gtk_tool_button_get_icon_widget(GTK_TOOL_BUTTON(cbutton->widget));
   if (wid)
    {
       cwid = _list_get_cwidget(ClipMachineMemory, wid);
       if (!cwid)
	  cwid = _register_widget(ClipMachineMemory, wid, NULL);
       if (cwid)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
    }

   return 0;
 err:
   return 1;
}

int
clip_GTK_TOOLBUTTONSETLABELWIDGET(ClipMachine * ClipMachineMemory)
{
   C_widget *cbutton = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(cbutton, GTK_IS_TOOL_BUTTON);
   CHECKCWID(cwid, GTK_IS_WIDGET);

   gtk_tool_button_set_label_widget(GTK_TOOL_BUTTON(cbutton->widget), GTK_WIDGET(cwid->widget));

   return 0;
 err:
   return 1;
}

int
clip_GTK_TOOLBUTTONGETLABELWIDGET(ClipMachine * ClipMachineMemory)
{
   C_widget *cbutton = _fetch_cw_arg(ClipMachineMemory);

   GtkWidget *wid;

   C_widget *cwid;

   CHECKCWID(cbutton, GTK_IS_TOOL_BUTTON);

   wid = gtk_tool_button_get_label_widget(GTK_TOOL_BUTTON(cbutton->widget));
   if (wid)
    {
       cwid = _list_get_cwidget(ClipMachineMemory, wid);
       if (!cwid)
	  cwid = _register_widget(ClipMachineMemory, wid, NULL);
       if (cwid)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
    }

   return 0;
 err:
   return 1;
}

/******************************************************************************/
/**************************** TOGGLE TOOL BUTTON ******************************/

int
clip_GTK_TOGGLETOOLBUTTONNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   GtkToolItem *item;

   C_widget *cbutton;

   CHECKOPT(1, MAP_type_of_ClipVarType);

   item = gtk_toggle_tool_button_new();
   if (!item)
      goto err;
   cbutton = _register_widget(ClipMachineMemory, GTK_WIDGET(item), cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cbutton->obj);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TOGGLETOOLBUTTONNEWFROMSTOCK(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   gchar    *label = _clip_parc(ClipMachineMemory, 2);

   GtkToolItem *item;

   C_widget *cbutton;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   item = gtk_toggle_tool_button_new_from_stock(label);

   if (!item)
      goto err;
   cbutton = _register_widget(ClipMachineMemory, GTK_WIDGET(item), cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cbutton->obj);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TOGGLETOOLBUTTONSETACTIVE(ClipMachine * ClipMachineMemory)
{
   C_widget *cbutton = _fetch_cw_arg(ClipMachineMemory);

   gboolean  set = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(cbutton, GTK_IS_TOGGLE_TOOL_BUTTON);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);

   gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(cbutton->widget), set);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TOGGLETOOLBUTTONGETACTIVE(ClipMachine * ClipMachineMemory)
{
   C_widget *cbutton = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cbutton, GTK_IS_TOGGLE_TOOL_BUTTON);

   _clip_retl(ClipMachineMemory, gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(cbutton->widget)));

   return 0;
 err:
   return 1;
}

/******************************************************************************/
/**************************** RADIO TOOL BUTTON *******************************/

int
clip_GTK_RADIOTOOLBUTTONNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   C_object *cgroup = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   GtkToolItem *item;

   C_widget *cbutton;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKCOBJOPT(cgroup, GTK_IS_OBJECT(cgroup->object));

   item = gtk_radio_tool_button_new((cgroup) ? (GSList *) (cgroup->object) : NULL);

   if (!item)
      goto err;
   cbutton = _register_widget(ClipMachineMemory, GTK_WIDGET(item), cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cbutton->obj);
   return 0;
 err:
   return 1;
}

int
clip_GTK_RADIOTOOLBUTTONNEWFROMSTOCK(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   C_object *cgroup = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gchar    *label = _clip_parc(ClipMachineMemory, 3);

   GtkToolItem *item;

   C_widget *cbutton;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKCOBJOPT(cgroup, GTK_IS_OBJECT(cgroup->object));
   CHECKARG(3, CHARACTER_type_of_ClipVarType);

   item = gtk_radio_tool_button_new_from_stock((cgroup) ? (GSList *) (cgroup->object) : NULL, label);

   if (!item)
      goto err;
   cbutton = _register_widget(ClipMachineMemory, GTK_WIDGET(item), cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cbutton->obj);
   return 0;
 err:
   return 1;
}

int
clip_GTK_RADIOTOOLBUTTONNEWFROMWIDGET(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   C_widget *cgroup = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   GtkToolItem *item;

   C_widget *cbutton;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKCWIDOPT(cgroup, GTK_IS_RADIO_TOOL_BUTTON);

   item = gtk_radio_tool_button_new_from_widget((cgroup) ? GTK_RADIO_TOOL_BUTTON(cgroup->widget) : NULL);

   if (!item)
      goto err;
   cbutton = _register_widget(ClipMachineMemory, GTK_WIDGET(item), cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cbutton->obj);
   return 0;
 err:
   return 1;
}

int
clip_GTK_RADIOTOOLBUTTONNEWWITHSTOCKFROMWIDGET(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   C_widget *cgroup = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gchar    *label = _clip_parc(ClipMachineMemory, 3);

   GtkToolItem *item;

   C_widget *cbutton;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKCWIDOPT(cgroup, GTK_IS_RADIO_TOOL_BUTTON);
   CHECKARG(3, CHARACTER_type_of_ClipVarType);

   item = gtk_radio_tool_button_new_with_stock_from_widget((cgroup) ? GTK_RADIO_TOOL_BUTTON(cgroup->widget) : NULL, label);

   if (!item)
      goto err;
   cbutton = _register_widget(ClipMachineMemory, GTK_WIDGET(item), cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cbutton->obj);
   return 0;
 err:
   return 1;
}

int
clip_GTK_RADIOTOOLBUTTONGETGROUP(ClipMachine * ClipMachineMemory)
{
   C_widget *cbutton = _fetch_cw_arg(ClipMachineMemory);

   GSList   *list;

   C_object *cgroup;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKCWIDOPT(cbutton, GTK_IS_RADIO_TOOL_BUTTON);

   list = gtk_radio_tool_button_get_group(GTK_RADIO_TOOL_BUTTON(cbutton->widget));

   if (list)
    {
       cgroup = _list_get_cobject(ClipMachineMemory, list);
       if (!cgroup)
	  cgroup = _register_object(ClipMachineMemory, list, GTK_TYPE_OBJECT, NULL, NULL);
       _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cgroup->obj);
    }
   return 0;
 err:
   return 1;
}

int
clip_GTK_RADIOTOOLBUTTONSETGROUP(ClipMachine * ClipMachineMemory)
{
   C_widget *cbutton = _fetch_cw_arg(ClipMachineMemory);

   C_object *cgroup = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKCWIDOPT(cbutton, GTK_IS_RADIO_TOOL_BUTTON);
   CHECKCOBJ(cgroup, GTK_IS_OBJECT(cgroup->object));

   gtk_radio_tool_button_set_group(GTK_RADIO_TOOL_BUTTON(cbutton->widget), (GSList *) (cgroup->object));

   return 0;
 err:
   return 1;
}
