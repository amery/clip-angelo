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

/*********************** SIGNALS **************************/
static gint
handle_move_current_signal(GtkWidget * widget, GtkMenuDirectionType direction, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_DIRECTION, direction);
   INVOKESIGHANDLER(widget, cs, cv);
}

static gint
handle_activate_current_signal(GtkWidget * widget, gboolean force_hide, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_FORCEHIDE, force_hide);
   INVOKESIGHANDLER(widget, cs, cv);
}

static SignalTable menushell_signals[] = {
   {"deactivate", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_DEACTIVATE_SIGNAL},
   {"selection-done", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_SELECTION_DONE_SIGNAL},
   {"move-current", GSF(handle_move_current_signal), ESF(object_emit_signal),
    GTK_MOVE_CURRENT_SIGNAL},
   {"activate-current", GSF(handle_activate_current_signal),
    ESF(object_emit_signal), GTK_ACTIVATE_CURRENT_SIGNAL},
   {"cancel", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_CANCEL_SIGNAL},
   {"", NULL, NULL, 0}
};

/* Register menu shell signals table in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_menu_shell()
{
   return GTK_TYPE_MENU_SHELL;
}

long
_clip_type_menu_shell()
{
   return GTK_WIDGET_MENU_SHELL;
}

const char *
_clip_type_name_menu_shell()
{
   return "GTK_WIDGET_MENU_SHELL";
}

int
clip_INIT___MENU_SHELL(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_menu_shell, _clip_type_name_menu_shell, _gtk_type_menu_shell, _gtk_type_container, menushell_signals);
   return 0;
}

/**********************************************************/

int
clip_GTK_MENUSHELLAPPEND(ClipMachine * ClipMachineMemory)
{
   C_widget *cmns = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKCWID(cmns, GTK_IS_MENU_SHELL);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_menu_shell_append(GTK_MENU_SHELL(cmns->widget), cwid->widget);
   return 0;
 err:
   return 1;
}

int
clip_GTK_MENUSHELLPREPEND(ClipMachine * ClipMachineMemory)
{
   C_widget *cmns = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKCWID(cmns, GTK_IS_MENU_SHELL);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_menu_shell_prepend(GTK_MENU_SHELL(cmns->widget), cwid->widget);
   return 0;
 err:
   return 1;
}

int
clip_GTK_MENUSHELLINSERT(ClipMachine * ClipMachineMemory)
{
   C_widget *cmns = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   int position = _clip_parni(ClipMachineMemory, 3);
   CHECKCWID(cmns, GTK_IS_MENU_SHELL);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   gtk_menu_shell_insert(GTK_MENU_SHELL(cmns->widget), cwid->widget, position);
   return 0;
 err:
   return 1;
}

int
clip_GTK_MENUSHELLDEACTIVATE(ClipMachine * ClipMachineMemory)
{
   C_widget *cmns = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(cmns, GTK_IS_MENU_SHELL);
   gtk_menu_shell_deactivate(GTK_MENU_SHELL(cmns->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_MENUSHELLSELECTITEM(ClipMachine * ClipMachineMemory)
{
   C_widget *cmns = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKCWID(cmns, GTK_IS_MENU_SHELL);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_menu_shell_select_item(GTK_MENU_SHELL(cmns->widget), cwid->widget);
   return 0;
 err:
   return 1;
}

int
clip_GTK_MENUSHELLDESELECT(ClipMachine * ClipMachineMemory)
{
   C_widget *cmns = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(cmns, GTK_IS_MENU_SHELL);
   gtk_menu_shell_deselect(GTK_MENU_SHELL(cmns->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_MENUSHELLACTIVATETITEM(ClipMachine * ClipMachineMemory)
{
   C_widget *cmns = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   gboolean force_deactivate = _clip_parl(ClipMachineMemory, 3);
   CHECKCWID(cmns, GTK_IS_MENU_SHELL);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKOPT(3, LOGICAL_type_of_ClipVarType);
   gtk_menu_shell_activate_item(GTK_MENU_SHELL(cmns->widget), cwid->widget, force_deactivate);
   return 0;
 err:
   return 1;
}
