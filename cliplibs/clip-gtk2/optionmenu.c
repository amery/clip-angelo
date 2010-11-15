/*
    Copyright (C) 2004  ITK
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

/**********************************************************/
/* Signals table */
static SignalTable option_menu_signals[] = {
  /* signals */
   {"changed", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_CHANGED_SIGNAL},
   {"", NULL, NULL, 0}
};

/**********************************************************/

/* Register option menu in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_option_menu()
{
   return GTK_TYPE_OPTION_MENU;
}

long
_clip_type_option_menu()
{
   return GTK_WIDGET_OPTION_MENU;
}

const char *
_clip_type_name_option_menu()
{
   return "GTK_WIDGET_OPTION_MENU";
}

int
clip_INIT___OPTION_MENU(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_option_menu, _clip_type_name_option_menu, _gtk_type_option_menu, _gtk_type_button, option_menu_signals);
   return 0;
}

/**********************************************************/

/**** Option menu constructor ****/
int
clip_GTK_OPTIONMENUNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   GtkWidget *wid = NULL;
   C_widget *cwid;
   CHECKOPT(1, MAP_type_of_ClipVarType);

   wid = gtk_option_menu_new();
   if (!wid)
      goto err;

   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

/* Returns the GtkMenu associated with the GtkOptionMenu. */
int
clip_GTK_OPTIONMENUGETMENU(ClipMachine * ClipMachineMemory)
{
   C_widget *coptmenu = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cmenu = 0;
   GtkWidget *menu;
   CHECKCWID(coptmenu, GTK_IS_OPTION_MENU);
   menu = gtk_option_menu_get_menu(GTK_OPTION_MENU(coptmenu->widget));
   if (menu)
      {
	 cmenu = _list_get_cwidget(ClipMachineMemory, menu);
	 if (!cmenu)
	    cmenu = _register_widget(ClipMachineMemory, menu, NULL);
      }
   if (cmenu)
      _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cmenu->obj);
   return 0;
 err:
   return 1;
}

/* Provides the GtkMenu that is popped up to allow the user to choose a
 * new value. You should provide a simple menu avoiding the use of tearoff
 * menu items, submenus, and accelerators. */
int
clip_GTK_OPTIONMENUSETMENU(ClipMachine * ClipMachineMemory)
{
   C_widget *coptmenu = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cmenu = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKCWID(coptmenu, GTK_IS_OPTION_MENU);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cmenu, GTK_IS_MENU);
   gtk_option_menu_set_menu(GTK_OPTION_MENU(coptmenu->widget), cmenu->widget);
   return 0;
 err:
   return 1;
}

/* Removes the menu from the option menu. */
int
clip_GTK_OPTIONMENUREMOVEMENU(ClipMachine * ClipMachineMemory)
{
   C_widget *coptmenu = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(coptmenu, GTK_IS_OPTION_MENU);
   gtk_option_menu_remove_menu(GTK_OPTION_MENU(coptmenu->widget));
   return 0;
 err:
   return 1;
}

/* Selects the menu item specified by index making
 * it the newly selected value for the option menu. */
int
clip_GTK_OPTIONMENUSETHISTORY(ClipMachine * ClipMachineMemory)
{
   C_widget *coptmenu = _fetch_cw_arg(ClipMachineMemory);
   guint index = _clip_parni(ClipMachineMemory, 2);
   CHECKCWID(coptmenu, GTK_IS_OPTION_MENU);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      index = 1;
   gtk_option_menu_set_history(GTK_OPTION_MENU(coptmenu->widget), index - 1);
   return 0;
 err:
   return 1;
}

int
clip_GTK_OPTIONMENUGETHISTORY(ClipMachine * ClipMachineMemory)
{
   C_widget *coptmenu = _fetch_cw_arg(ClipMachineMemory);
   guint index;
   CHECKCWID(coptmenu, GTK_IS_OPTION_MENU);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      index = 1;
   index = gtk_option_menu_get_history(GTK_OPTION_MENU(coptmenu->widget));
   _clip_retni(ClipMachineMemory, index + 1);
   return 0;
 err:
   return 1;
}
