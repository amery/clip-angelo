/*
    Copyright (C) 2001  ITK
    Author  : Alexey M. Tkachenko <alexey@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg.h"

#include <gtk/gtk.h>

#include "ci_clip-gtk.ch"
#include "ci_clip-gtk.h"

/**********************************************************/
/* Signals table */
static SignalTable tree_item_signals[] = {
   {"collapse", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_COLLAPSE_SIGNAL},
   {"expand", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_EXPAND_SIGNAL},
   {"", NULL, NULL, 0}
};

/* Register item signals table in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_tree_item()
{
   return GTK_TYPE_TREE_ITEM;
}

long
_clip_type_tree_item()
{
   return GTK_WIDGET_TREE_ITEM;
}

const char *
_clip_type_name_tree_item()
{
   return "GTK_WIDGET_TREE_ITEM";
}

int
clip_INIT___TREEITEM(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_tree_item, _clip_type_name_tree_item,
		    _gtk_type_tree_item, _gtk_type_container, tree_item_signals);
   return 0;
}

/**********************************************************/

/**** TREE ITEM constructor ****/
int
clip_GTK_TREEITEMNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   char     *label = _clip_parc(ClipMachineMemory, 2);

   GtkWidget *wid = NULL;

   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 2) == CHARACTER_type_of_ClipVarType)
    {
       LOCALE_TO_UTF(label);
       wid = gtk_tree_item_new_with_label(label);
       FREE_TEXT(label);
    }
   else
      wid = gtk_tree_item_new();
   if (!wid)
      goto err;

   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

int
clip_GTK_TREEITEMSETSUBTREE(ClipMachine * ClipMachineMemory)
{
   C_widget *citm = _fetch_cw_arg(ClipMachineMemory);

   C_widget *csubtree = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(citm, GTK_IS_TREE_ITEM);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(csubtree, GTK_IS_WIDGET);
   gtk_tree_item_set_subtree(GTK_TREE_ITEM(citm->widget), GTK_WIDGET(csubtree->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEITEMGETSUBTREE(ClipMachine * ClipMachineMemory)
{
   C_widget *citm = _fetch_cw_arg(ClipMachineMemory);

   C_widget *csubtree = NULL;

   GtkWidget *subtree;

   CHECKCWID(citm, GTK_IS_TREE_ITEM);
   subtree = GTK_TREE_ITEM_SUBTREE(citm->widget);
   csubtree = _list_get_cwidget(ClipMachineMemory, subtree);
   if (!csubtree)
      csubtree = _register_widget(ClipMachineMemory, subtree, NULL);
   if (csubtree)
      _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &csubtree->obj);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEITEMREMOVESUBTREE(ClipMachine * ClipMachineMemory)
{
   C_widget *citm = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(citm, GTK_IS_TREE_ITEM);
   gtk_tree_item_remove_subtree(GTK_TREE_ITEM(citm->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEITEMSELECT(ClipMachine * ClipMachineMemory)
{
   C_widget *citm = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(citm, GTK_IS_TREE_ITEM);
   gtk_tree_item_select(GTK_TREE_ITEM(citm->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEITEMDESELECT(ClipMachine * ClipMachineMemory)
{
   C_widget *citm = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(citm, GTK_IS_TREE_ITEM);
   gtk_tree_item_deselect(GTK_TREE_ITEM(citm->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEITEMEXPAND(ClipMachine * ClipMachineMemory)
{
   C_widget *citm = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(citm, GTK_IS_TREE_ITEM);
   gtk_tree_item_expand(GTK_TREE_ITEM(citm->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEITEMCOLLAPSE(ClipMachine * ClipMachineMemory)
{
   C_widget *citm = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(citm, GTK_IS_TREE_ITEM);
   gtk_tree_item_collapse(GTK_TREE_ITEM(citm->widget));
   return 0;
 err:
   return 1;
}
