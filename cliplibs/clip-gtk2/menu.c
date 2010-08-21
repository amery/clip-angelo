/*
    Copyright (C) 2001-2005  ITK
    Author  : Alexey M. Tkachenko <alexey@itk.ru>
    	      Elena V. Kornilova <alena@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg2.h"

#include <gtk/gtk.h>

#include "ci_clip-gtk2.ch"
#include "ci_clip-gtk2.h"

/*********************** SIGNALS **************************/

/****              Menu has no signals                 ****/

/**********************************************************/

/* Register menu signals table in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_menu()
{
   return GTK_TYPE_MENU;
}

long
_clip_type_menu()
{
   return GTK_WIDGET_MENU;
}

const char *
_clip_type_name_menu()
{
   return "GTK_WIDGET_MENU";
}

int
clip_INIT___MENU(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_menu, _clip_type_name_menu, _gtk_type_menu, _gtk_type_menu_shell, NULL);
   return 0;
}

/**********************************************************/

/****  MENU constructor ****/
int
clip_GTK_MENUNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   GtkWidget *wid = NULL;

   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);

   wid = gtk_menu_new();
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

/****  ------------------ ****/
int
clip_GTK_MENUAPPEND(ClipMachine * ClipMachineMemory)
{
   C_widget *cmnu = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(cmnu, GTK_IS_MENU);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_menu_append(GTK_MENU(cmnu->widget), cwid->widget);
   return 0;
 err:
   return 1;
}

int
clip_GTK_MENUPREPEND(ClipMachine * ClipMachineMemory)
{
   C_widget *cmnu = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(cmnu, GTK_IS_MENU);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_menu_prepend(GTK_MENU(cmnu->widget), cwid->widget);
   return 0;
 err:
   return 1;
}

int
clip_GTK_MENUINSERT(ClipMachine * ClipMachineMemory)
{
   C_widget *cmnu = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   int       position = _clip_parni(ClipMachineMemory, 3);

   CHECKCWID(cmnu, GTK_IS_MENU);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   gtk_menu_insert(GTK_MENU(cmnu->widget), cwid->widget, position);
   return 0;
 err:
   return 1;
}

int
clip_GTK_MENUREORDERCHILD(ClipMachine * ClipMachineMemory)
{
   C_widget *cmnu = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   int       position = _clip_parni(ClipMachineMemory, 3);

   CHECKCWID(cmnu, GTK_IS_MENU);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   gtk_menu_reorder_child(GTK_MENU(cmnu->widget), cwid->widget, position);
   return 0;
 err:
   return 1;
}

/* Display the menu onscreen */
int
clip_GTK_MENUPOPUP(ClipMachine * ClipMachineMemory)
{
   C_widget *cmnu = _fetch_cw_arg(ClipMachineMemory);

  /*
     C_widget *cparentshell = _fetch_cwidget(ClipMachineMemory, 2);
     C_widget *cparentitem  = _fetch_cwidget(ClipMachineMemory, 3);
     ClipVar        *func   = _clip_spar(ClipMachineMemory, 4);
   */
   CHECKCWID(cmnu, GTK_IS_MENU);
   gtk_menu_popup(GTK_MENU(cmnu->widget), NULL, NULL, NULL, NULL, 1, 0);
   return 0;
 err:
   return 1;
}

int
clip_GTK_MENUSETACCELGROUP(ClipMachine * ClipMachineMemory)
{
   C_widget *cmnu = _fetch_cw_arg(ClipMachineMemory);

   C_widget *caccelgroup = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(cmnu, GTK_IS_MENU);
   CHECKCWID(caccelgroup, GTK_IS_ACCEL_GROUP);

   gtk_menu_set_accel_group(GTK_MENU(cmnu->widget), GTK_ACCEL_GROUP(caccelgroup->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_MENUGETACCELGROUP(ClipMachine * ClipMachineMemory)
{
   C_widget *cmnu = _fetch_cw_arg(ClipMachineMemory);

   C_widget *caccelgroup;

   GtkAccelGroup *accelgroup;

   CHECKCWID(cmnu, GTK_IS_MENU);

   accelgroup = gtk_menu_get_accel_group(GTK_MENU(cmnu->widget));

   if (accelgroup)
    {
       caccelgroup = _list_get_cwidget(ClipMachineMemory, accelgroup);
       if (!caccelgroup)
	  _register_widget(ClipMachineMemory, GTK_WIDGET(accelgroup), NULL);
       if (accelgroup)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &caccelgroup->obj);
    }
   return 0;
 err:
   return 1;
}

int
clip_GTK_MENUSETACCELPATH(ClipMachine * ClipMachineMemory)
{
   C_widget *cmnu = _fetch_cw_arg(ClipMachineMemory);

   const gchar *path = _clip_parc(ClipMachineMemory, 2);

   CHECKCWID(cmnu, GTK_IS_MENU);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   gtk_menu_set_accel_path(GTK_MENU(cmnu->widget), path);
   return 0;
 err:
   return 1;
}

int
clip_GTK_MENUGETTEAROFFSTATE(ClipMachine * ClipMachineMemory)
{
   C_widget *cmnu = _fetch_cw_arg(ClipMachineMemory);

   gboolean  ret;

   CHECKCWID(cmnu, GTK_IS_MENU);

   ret = gtk_menu_get_tearoff_state(GTK_MENU(cmnu->widget));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

int
clip_GTK_MENUGETTITLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cmnu = _fetch_cw_arg(ClipMachineMemory);

   gchar    *title;

   CHECKCWID(cmnu, GTK_IS_MENU);

   title = (gchar *) gtk_menu_get_title(GTK_MENU(cmnu->widget));

   LOCALE_FROM_UTF(title);

   _clip_retc(ClipMachineMemory, title);

   FREE_TEXT(title);
   return 0;
 err:
   return 1;
}

int
clip_GTK_MENUPOPDOWN(ClipMachine * ClipMachineMemory)
{
   C_widget *cmnu = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cmnu, GTK_IS_MENU);
   gtk_menu_popdown(GTK_MENU(cmnu->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_MENUREPOSITION(ClipMachine * ClipMachineMemory)
{
   C_widget *cmnu = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cmnu, GTK_IS_MENU);
   gtk_menu_reposition(GTK_MENU(cmnu->widget));
   return 0;
 err:
   return 1;
}

/* Keep track of the last menu item selected. (For the purposes
 * of the option menu */
int
clip_GTK_MENUGETACTIVE(ClipMachine * ClipMachineMemory)
{
   C_widget *cmnu = _fetch_cw_arg(ClipMachineMemory);

   GtkWidget *item;

   C_widget *citem;

   CHECKCWID(cmnu, GTK_IS_MENU);
   item = gtk_menu_get_active(GTK_MENU(cmnu->widget));
   citem = _list_get_cwidget(ClipMachineMemory, item);
   if (!citem)
      citem = _register_widget(ClipMachineMemory, item, NULL);
   if (citem)
    {
       ClipVar  *ret = RETPTR(ClipMachineMemory);

       _clip_mclone(ClipMachineMemory, ret, &citem->obj);
    }
   return 0;
 err:
   return 1;
}

int
clip_GTK_MENUSETACTIVE(ClipMachine * ClipMachineMemory)
{
   C_widget *cmnu = _fetch_cw_arg(ClipMachineMemory);

   int       index = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(cmnu, GTK_IS_MENU);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_menu_set_active(GTK_MENU(cmnu->widget), index);
   return 0;
 err:
   return 1;
}

int
clip_GTK_MENUSETTEAROFFSTATE(ClipMachine * ClipMachineMemory)
{
   C_widget *cmnu = _fetch_cw_arg(ClipMachineMemory);

   gboolean  state = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(cmnu, GTK_IS_MENU);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   gtk_menu_set_tearoff_state(GTK_MENU(cmnu->widget), state);
   return 0;
 err:
   return 1;
}

/* This sets the window manager title for the window that
 * appears when a menu is torn off */
int
clip_GTK_MENUSETTITLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cmnu = _fetch_cw_arg(ClipMachineMemory);

   gchar    *title = _clip_parc(ClipMachineMemory, 2);

   CHECKCWID(cmnu, GTK_IS_MENU);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);
   LOCALE_TO_UTF(title);
   gtk_menu_set_title(GTK_MENU(cmnu->widget), title);
   FREE_TEXT(title);
   return 0;
 err:
   return 1;
}

#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 4)

int
clip_GTK_MENUATTACH(ClipMachine * ClipMachineMemory)
{
   C_widget *cmnu = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   guint     left_attach = _clip_parni(ClipMachineMemory, 3);

   guint     right_attach = _clip_parni(ClipMachineMemory, 4);

   guint     top_attach = _clip_parni(ClipMachineMemory, 5);

   guint     bottom_attach = _clip_parni(ClipMachineMemory, 6);

   CHECKCWID(cmnu, GTK_IS_MENU);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);
   CHECKARG(5, NUMERIC_type_of_ClipVarType);
   CHECKARG(6, NUMERIC_type_of_ClipVarType);

   gtk_menu_attach(GTK_MENU(cmnu->widget), GTK_WIDGET(cwid->widget), left_attach, right_attach, top_attach, bottom_attach);
   return 0;
 err:
   return 1;
}

int
clip_GTK_MENUSETMONITOR(ClipMachine * ClipMachineMemory)
{
   C_widget *cmnu = _fetch_cw_arg(ClipMachineMemory);

   guint     monitor_num = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(cmnu, GTK_IS_MENU);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_menu_set_monitor(GTK_MENU(cmnu->widget), monitor_num);
   return 0;
 err:
   return 1;
}
#endif
#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 6)
int
clip_GTK_MENUGETFORATTACHWIDGET(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   ClipVar  *cv = RETPTR(ClipMachineMemory);

   GList    *list;

   long      l;

   CHECKCWID(cwid, GTK_IS_WIDGET);

   list = gtk_menu_get_for_attach_widget(GTK_WIDGET(cwid->widget));
   l = g_list_length(list);
   _clip_array(ClipMachineMemory, cv, 1, &l);
   for (l = 0; list; list = g_list_next(list), l++)
    {
       C_widget *cmnu;

       GtkMenu  *menu;

       menu = GTK_MENU(list->data);

       if (menu)
	{
	   cmnu = _list_get_cwidget(ClipMachineMemory, menu);
	   if (!cmnu)
	      cmnu = _register_widget(ClipMachineMemory, GTK_WIDGET(menu), NULL);
	   if (cmnu)
	      _clip_aset(ClipMachineMemory, cv, &cmnu->obj, 1, &l);
	}
    }
   return 0;
 err:
   return 1;
}
#endif
