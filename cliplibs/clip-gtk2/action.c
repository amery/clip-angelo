
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
static    gint
handle_activate_signal(GtkAction * action, C_signal * cs)
{
   OBJECTPREPARECV(cs, cv);
   OBJECTINVOKESIGHANDLER(cs, cv);
}

static SignalTable action_signals[] = {
   {"activate", GSF(handle_activate_signal), ESF(object_emit_signal),
    GTK_ACTIVATE_SIGNAL},
   {"", NULL, NULL, 0}
};

/**********************************************************/
CLIP_DLLEXPORT GtkType
_gtk_type_action()
{
   return GTK_TYPE_ACTION;
}

long
_clip_type_action()
{
   return GTK_OBJECT_ACTION;
}

const char *
_clip_type_name_action()
{
   return "GTK_OBJECT_ACTION";
}

int
clip_INIT___ACTION(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_action, _clip_type_name_action, _gtk_type_action, NULL, action_signals);
   return 0;
}

int
clip_GTK_ACTIONNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   gchar    *name = _clip_parc(ClipMachineMemory, 2);

   gchar    *label = _clip_parc(ClipMachineMemory, 3);

   gchar    *tooltip = _clip_parc(ClipMachineMemory, 4);

   gchar    *stock_id = _clip_parc(ClipMachineMemory, 5);

   C_object *caction;

   GtkAction *action;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);
   CHECKARG(3, CHARACTER_type_of_ClipVarType);
   CHECKOPT(4, CHARACTER_type_of_ClipVarType);
   CHECKOPT(5, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(name);
   LOCALE_TO_UTF(label);
   if (tooltip)
      LOCALE_TO_UTF(tooltip);
   if (stock_id)
      LOCALE_TO_UTF(stock_id);

   action = gtk_action_new(name, label, tooltip, stock_id);

   if (action)
    {
       caction = _list_get_cobject(ClipMachineMemory, action);
       if (!caction)
	  caction = _register_object(ClipMachineMemory, action, GTK_TYPE_ACTION, cv, NULL);
       if (caction)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &caction->obj);
    }

   FREE_TEXT(name);
   FREE_TEXT(label);
   if (tooltip)
      FREE_TEXT(tooltip);
   if (stock_id)
      FREE_TEXT(stock_id);
   return 0;
 err:
   return 1;
}

int
clip_GTK_ACTIONGETNAME(ClipMachine * ClipMachineMemory)
{
   C_object *caction = _fetch_co_arg(ClipMachineMemory);

   gchar    *name;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(caction, GTK_IS_ACTION(caction->object));

   name = (gchar *) gtk_action_get_name(GTK_ACTION(caction->object));

   LOCALE_FROM_UTF(name);

   _clip_retc(ClipMachineMemory, name);

   FREE_TEXT(name);
   return 0;
 err:
   return 1;
}

int
clip_GTK_ACTIONISSENSITIVE(ClipMachine * ClipMachineMemory)
{
   C_object *caction = _fetch_co_arg(ClipMachineMemory);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(caction, GTK_IS_ACTION(caction->object));

   _clip_retl(ClipMachineMemory, gtk_action_is_sensitive(GTK_ACTION(caction->object)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ACTIONGETSENSITIVE(ClipMachine * ClipMachineMemory)
{
   C_object *caction = _fetch_co_arg(ClipMachineMemory);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(caction, GTK_IS_ACTION(caction->object));

   _clip_retl(ClipMachineMemory, gtk_action_get_sensitive(GTK_ACTION(caction->object)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ACTIONISVISIBLE(ClipMachine * ClipMachineMemory)
{
   C_object *caction = _fetch_co_arg(ClipMachineMemory);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(caction, GTK_IS_ACTION(caction->object));

   _clip_retl(ClipMachineMemory, gtk_action_is_visible(GTK_ACTION(caction->object)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ACTIONGETVISIBLE(ClipMachine * ClipMachineMemory)
{
   C_object *caction = _fetch_co_arg(ClipMachineMemory);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(caction, GTK_IS_ACTION(caction->object));

   _clip_retl(ClipMachineMemory, gtk_action_get_visible(GTK_ACTION(caction->object)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ACTIONACTIVATE(ClipMachine * ClipMachineMemory)
{
   C_object *caction = _fetch_co_arg(ClipMachineMemory);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(caction, GTK_IS_ACTION(caction->object));

   gtk_action_activate(GTK_ACTION(caction->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ACTIONCREATEICON(ClipMachine * ClipMachineMemory)
{
   C_object *caction = _fetch_co_arg(ClipMachineMemory);

   GtkIconSize size = _clip_parni(ClipMachineMemory, 2);

   GtkWidget *wid;

   C_widget *cwid;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(caction, GTK_IS_ACTION(caction->object));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   wid = gtk_action_create_icon(GTK_ACTION(caction->object), size);

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
clip_GTK_ACTIONCREATEMENUITEM(ClipMachine * ClipMachineMemory)
{
   C_object *caction = _fetch_co_arg(ClipMachineMemory);

   GtkWidget *wid;

   C_widget *cwid;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(caction, GTK_IS_ACTION(caction->object));

   wid = gtk_action_create_menu_item(GTK_ACTION(caction->object));

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
clip_GTK_ACTIONCREATETOOLITEM(ClipMachine * ClipMachineMemory)
{
   C_object *caction = _fetch_co_arg(ClipMachineMemory);

   GtkWidget *wid;

   C_widget *cwid;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(caction, GTK_IS_ACTION(caction->object));

   wid = gtk_action_create_tool_item(GTK_ACTION(caction->object));

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
clip_GTK_ACTIONCONNECTPROXY(ClipMachine * ClipMachineMemory)
{
   C_object *caction = _fetch_co_arg(ClipMachineMemory);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(caction, GTK_IS_ACTION(caction->object));
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);

   gtk_action_connect_proxy(GTK_ACTION(caction->object), GTK_WIDGET(cwid->widget));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ACTIONDISCONNECTPROXY(ClipMachine * ClipMachineMemory)
{
   C_object *caction = _fetch_co_arg(ClipMachineMemory);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(caction, GTK_IS_ACTION(caction->object));
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);

   gtk_action_disconnect_proxy(GTK_ACTION(caction->object), GTK_WIDGET(cwid->widget));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ACTIONGETPROXIES(ClipMachine * ClipMachineMemory)
{
   C_object *caction = _fetch_co_arg(ClipMachineMemory);

   GSList   *list;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(caction, GTK_IS_ACTION(caction->object));

   list = gtk_action_get_proxies(GTK_ACTION(caction->object));

   if (list)
    {
       ClipVar  *cv = RETPTR(ClipMachineMemory);

       long      n = g_slist_length(list);

       _clip_array(ClipMachineMemory, cv, 1, &n);
       for (n = 0; list; list = g_slist_next(list))
	{
	   C_widget *cwid;

	   GtkWidget *wid;

	   wid = list->data;
	   cwid = _list_get_cwidget(ClipMachineMemory, wid);
	   if (!cwid)
	      cwid = _register_widget(ClipMachineMemory, wid, NULL);
	   if (cwid)
	      _clip_aset(ClipMachineMemory, cv, &cwid->obj, 1, &n);
	}
    }

   return 0;
 err:
   return 1;
}

int
clip_GTK_ACTIONCONNECTACCELERATOR(ClipMachine * ClipMachineMemory)
{
   C_object *caction = _fetch_co_arg(ClipMachineMemory);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(caction, GTK_IS_ACTION(caction->object));

   gtk_action_connect_accelerator(GTK_ACTION(caction->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ACTIONDISCONNECTACCELERATOR(ClipMachine * ClipMachineMemory)
{
   C_object *caction = _fetch_co_arg(ClipMachineMemory);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(caction, GTK_IS_ACTION(caction->object));

   gtk_action_disconnect_accelerator(GTK_ACTION(caction->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ACTIONBLOCKACTIVATEFROM(ClipMachine * ClipMachineMemory)
{
   C_object *caction = _fetch_co_arg(ClipMachineMemory);

   C_widget *cproxy = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(caction, GTK_IS_ACTION(caction->object));
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cproxy, GTK_IS_WIDGET);

   gtk_action_block_activate_from(GTK_ACTION(caction->object), GTK_WIDGET(cproxy->widget));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ACTIONUNBLOCKACTIVATEFROM(ClipMachine * ClipMachineMemory)
{
   C_object *caction = _fetch_co_arg(ClipMachineMemory);

   C_widget *cproxy = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(caction, GTK_IS_ACTION(caction->object));
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cproxy, GTK_IS_WIDGET);

   gtk_action_unblock_activate_from(GTK_ACTION(caction->object), GTK_WIDGET(cproxy->widget));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ACTIONSETACCELPATH(ClipMachine * ClipMachineMemory)
{
   C_object *caction = _fetch_co_arg(ClipMachineMemory);

   gchar    *accel_path = _clip_parc(ClipMachineMemory, 2);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(caction, GTK_IS_ACTION(caction->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   gtk_action_set_accel_path(GTK_ACTION(caction->object), accel_path);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ACTIONSETACCELGROUP(ClipMachine * ClipMachineMemory)
{
   C_object *caction = _fetch_co_arg(ClipMachineMemory);

   C_widget *cagroup = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(caction, GTK_IS_ACTION(caction->object));
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cagroup, GTK_IS_ACCEL_GROUP);

   gtk_action_set_accel_group(GTK_ACTION(caction->object), GTK_ACCEL_GROUP(cagroup->widget));

   return 0;
 err:
   return 1;
}

#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 6)
int
clip_GTK_ACTIONGETACCELPATH(ClipMachine * ClipMachineMemory)
{
   C_object *caction = _fetch_co_arg(ClipMachineMemory);

   gchar    *path;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(caction, GTK_IS_ACTION(caction->object));

   path = (gchar *) gtk_action_get_accel_path(GTK_ACTION(caction->object));
   LOCALE_FROM_UTF(path);
   _clip_retc(ClipMachineMemory, path);
   FREE_TEXT(path);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ACTIONSETSENSITIVE(ClipMachine * ClipMachineMemory)
{
   C_object *caction = _fetch_co_arg(ClipMachineMemory);

   gboolean  sensitive = _clip_parl(ClipMachineMemory, 2);;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(caction, GTK_IS_ACTION(caction->object));
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_action_set_sensitive(GTK_ACTION(caction->object), sensitive);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ACTIONSETVISIBLE(ClipMachine * ClipMachineMemory)
{
   C_object *caction = _fetch_co_arg(ClipMachineMemory);

   gboolean  visible = _clip_parl(ClipMachineMemory, 2);;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(caction, GTK_IS_ACTION(caction->object));
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_action_set_visible(GTK_ACTION(caction->object), visible);

   return 0;
 err:
   return 1;
}
#endif
