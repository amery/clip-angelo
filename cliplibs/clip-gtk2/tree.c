/*
    Copyright (C) 2001  ITK
    Author  : Alexey M. Tkachenko <alexey@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include <string.h>
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg2.h"

#include <gtk/gtk.h>

#include "ci_clip-gtk2.ch"
#include "ci_clip-gtk2.h"

/**********************************************************/
/* Signal handlers */
static    gint
handle_sel_child_signal(GtkWidget * widget, GtkWidget * child, C_signal * cs)
{
   C_widget *cchild = _get_cwidget(cs->cw->cmachine, child);

   PREPARECV(cs, cv);
   if (cchild)
      _clip_madd(cs->cw->cmachine, &cv, HASH_CHILD, &cchild->obj);
   INVOKESIGHANDLER(child, cs, cv);
}

/* Signals table */
static SignalTable tree_signals[] = {
   {"selection-changed", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_SELECTION_CHANGED_SIGNAL},
   {"select-child", GSF(handle_sel_child_signal), ESF(object_emit_signal),
    GTK_SELECT_CHILD_SIGNAL},
   {"unselect-child", GSF(handle_sel_child_signal), ESF(object_emit_signal),
    GTK_UNSELECT_CHILD_SIGNAL},
   {"", NULL, NULL, 0}
};

/**********************************************************/

/* Register CTree in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_tree()
{
   return GTK_TYPE_TREE;
}

long
_clip_type_tree()
{
   return GTK_WIDGET_TREE;
}

const char *
_clip_type_name_tree()
{
   return "GTK_WIDGET_TREE";
}

int
clip_INIT___TREE(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_tree, _clip_type_name_tree, _gtk_type_tree, _gtk_type_container, tree_signals);
   return 0;
}

/**** Tree constructor ****/
int
clip_GTK_TREENEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   GtkWidget *wid = NULL;

   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   wid = gtk_tree_new();

   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

/* Adds the GtkTreeItem in tree_item to the end of the items in tree. */
int
clip_GTK_TREEAPPEND(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   C_widget *citem = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(ctree, GTK_IS_TREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(citem, GTK_TREE_ITEM);

   gtk_tree_append(GTK_TREE(ctree->widget), citem->widget);
   return 0;
 err:
   return 1;
}

/* Adds the GtkTreeItem in tree_item to the start of the items in tree. */
int
clip_GTK_TREEPREPEND(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   C_widget *citem = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(ctree, GTK_IS_TREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(citem, GTK_TREE_ITEM);

   gtk_tree_prepend(GTK_TREE(ctree->widget), citem->widget);
   return 0;
 err:
   return 1;
}

/* Adds the GtkTreeItem in tree_item to the list of items in tree
 * at the position indicated by position. */
int
clip_GTK_TREEINSERT(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   C_widget *citem = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gint      position = INT_OPTION(ClipMachineMemory, 3, 1) - 1;

   CHECKCWID(ctree, GTK_IS_TREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(citem, GTK_TREE_ITEM);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);

   gtk_tree_insert(GTK_TREE(ctree->widget), citem->widget, position);
   return 0;
 err:
   return 1;
}

/* Removes a list of items from the GtkTree in tree.

 * If only one item is to be removed from the GtkTree,
 * gtk_container_remove() can be used instead.

 * Removing an item from a GtkTree dereferences the item, and thus
 * usually destroys the item and any subtrees it may contain. If the
 * item is not to be destroyed, use gtk_object_ref() before removing it. */
int
clip_GTK_TREEREMOVEITEMS(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   ClipVar  *items = _clip_spar(ClipMachineMemory, 2);

   CHECKCWID(ctree, GTK_IS_TREE);
   CHECKOPT3(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType, ARRAY_type_of_ClipVarType);

   if (items->t.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType
       || items->t.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
    {
       C_widget *citem = _fetch_cwidget(ClipMachineMemory, items);

       gtk_tree_remove_item(GTK_TREE(ctree->widget), citem->widget);
    }
   if (items->t.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
    {
       GList    *list = NULL;

       unsigned short i;

       ClipVar  *item;

       C_widget *citem;

       for (i = 0; i < items->a.count; i++)
	{
	   item = &items->a.items[i];
	   if (item->t.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType
	       || item->t.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
	    {
	       citem = _fetch_cwidget(ClipMachineMemory, item);
	       CHECKCWID(citem, GTK_IS_TREE_ITEM);
	       list = g_list_append(list, citem->widget);
	    }
	}
       if (list)
	{
	   gtk_tree_remove_items(GTK_TREE(ctree->widget), list);
	   g_list_free(list);
	}
    }
   return 0;
 err:
   return 1;
}

/* Removes the items at positions between start and end from the GtkTree tree.

 * Removing an item from a GtkTree dereferences the item, and thus usually
 * destroys the item and any subtrees it may contain. If the item is not
 * to be destroyed, use gtk_object_ref() before removing it. */
int
clip_GTK_TREECLEARITEMS(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   gint      start = INT_OPTION(ClipMachineMemory, 2, 1) - 1;

   gint      end = INT_OPTION(ClipMachineMemory, 3, 1) - 1;

   CHECKCWID(ctree, GTK_IS_TREE);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);

   gtk_tree_clear_items(GTK_TREE(ctree->widget), start, end);
   return 0;
 err:
   return 1;
}

/* Emits the select_item signal for the child at position item, and thus
 * selects it (unless it is unselected in a signal handler). */
int
clip_GTK_TREESELECTITEM(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   gint      item = INT_OPTION(ClipMachineMemory, 2, 1) - 1;

   CHECKCWID(ctree, GTK_IS_TREE);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);

   gtk_tree_select_item(GTK_TREE(ctree->widget), item);
   return 0;
 err:
   return 1;
}

/* Emits the unselect_item for the child at position item, and thus unselects it. */
int
clip_GTK_TREEUNSELECTITEM(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   gint      item = INT_OPTION(ClipMachineMemory, 2, 1) - 1;

   CHECKCWID(ctree, GTK_IS_TREE);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);

   gtk_tree_unselect_item(GTK_TREE(ctree->widget), item);
   return 0;
 err:
   return 1;
}

/* Emits the select_item signal for the child tree_item, and thus
 * selects it (unless it is unselected in a signal handler). */
int
clip_GTK_TREESELECTCHILD(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   C_widget *citem = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(ctree, GTK_IS_TREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(citem, GTK_TREE_ITEM);

   gtk_tree_select_child(GTK_TREE(ctree->widget), citem->widget);
   return 0;
 err:
   return 1;
}

/* Emits the unselect_item signal for the child tree_item, and thus unselects it. */
int
clip_GTK_TREEUNSELECTCHILD(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   C_widget *citem = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(ctree, GTK_IS_TREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(citem, GTK_TREE_ITEM);

   gtk_tree_unselect_child(GTK_TREE(ctree->widget), citem->widget);
   return 0;
 err:
   return 1;
}

/* Returns the position of child in the GtkTree tree.
 * If child is not a child of tree, then -1 is returned. */
int
clip_GTK_TREECHILDPOSITION(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   C_widget *citem = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(ctree, GTK_IS_TREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(citem, GTK_TREE_ITEM);

   _clip_retni(ClipMachineMemory, gtk_tree_child_position(GTK_TREE(ctree->widget), citem->widget));
   return 0;
 err:
   return 1;
}

/* Sets the selection mode for the GtkTree tree.
mode can be one of
    GTK_SELECTION_SINGLE for when only one item can be selected at a time.
    GTK_SELECTION_BROWSE for when one item must be selected.
    GTK_SELECTION_MULTIPLE for when many items can be selected at once.
    GTK_SELECTION_EXTENDED Reserved for later use.
The selection mode is only defined for a root tree, as the root tree "owns" the selection.
The default mode is GTK_SELECTION_SINGLE. */
int
clip_GTK_TREESETSELECTIONMODE(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   GtkSelectionMode mode = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(ctree, GTK_IS_TREE);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);

   gtk_tree_set_selection_mode(GTK_TREE(ctree->widget), mode);
   return 0;
 err:
   return 1;
}

/* Sets the 'viewmode' for the GtkTree in tree.
The 'viewmode' defines how the tree looks when an item is selected.
mode can be one of:
    GTK_TREE_VIEW_LINE : When an item is selected the entire GtkTreeItem is highlighted.
    GTK_TREE_VIEW_ITEM : When an item is selected only the selected item's child widget is highlighted.
The default mode is GTK_TREE_VIEW_LINE. */
int
clip_GTK_TREESETVIEWMODE(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   GtkTreeViewMode mode = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(ctree, GTK_IS_TREE);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);

   gtk_tree_set_view_mode(GTK_TREE(ctree->widget), mode);
   return 0;
 err:
   return 1;
}

/* Sets whether or not the connecting lines between branches and children are drawn. */
int
clip_GTK_TREESETVIEWLINES(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   gboolean  flag = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(ctree, GTK_IS_TREE);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);

   gtk_tree_set_view_lines(GTK_TREE(ctree->widget), flag);
   return 0;
 err:
   return 1;
}

/* Removes the item child from the GtkTree tree. */
int
clip_GTK_TREEREMOVEITEM(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   C_widget *citem = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(ctree, GTK_IS_TREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(citem, GTK_TREE_ITEM);

   gtk_tree_remove_item(GTK_TREE(ctree->widget), citem->widget);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEISROOTTREE(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(ctree, GTK_IS_TREE);
   _clip_retl(ClipMachineMemory, GTK_IS_ROOT_TREE(ctree->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEROOTTREE(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   C_widget *croot;

   CHECKCWID(ctree, GTK_IS_TREE);
   croot = _get_cwidget(ClipMachineMemory, GTK_WIDGET(GTK_TREE_ROOT_TREE(ctree->widget)));
   if (croot)
      _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &croot->obj);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEGETSELECTION(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   GList    *list;

   CHECKCWID(ctree, GTK_IS_TREE);
   list = GTK_TREE_SELECTION(ctree->widget);
   if (list)
    {
       C_widget *citem;

       long      l = g_list_length(list);

       ClipVar  *a = RETPTR(ClipMachineMemory);

       _clip_array(ClipMachineMemory, a, 1, &l);
       for (l = 0; list; list = g_list_next(list), l++)
	{
	   citem = _get_cwidget(ClipMachineMemory, list->data);
	   if (citem)
	      _clip_aset(ClipMachineMemory, a, &citem->obj, 1, &l);
	}
    }
   return 0;
 err:
   return 1;
}
