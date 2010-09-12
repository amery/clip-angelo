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

/****************** SIGNALS for List  *********************/

/* Signal handlers */
static    gint
handle_select_child_signal(GtkWidget * widget, GtkWidget * wid, C_signal * cs)
{
   C_widget *cwid;

   PREPARECV(cs, cv);
   cwid = _list_get_cwidget(cs->cw->cmachine, wid);
   if (!cwid)
      cwid = _register_widget(cs->cw->cmachine, wid, NULL);
   if (cwid)
      _clip_madd(cs->cw->cmachine, &cv, HASH_CHILD, &cwid->obj);
   INVOKESIGHANDLER(widget, cs, cv);
}

static    gint
handle_unselect_child_signal(GtkWidget * widget, GtkWidget * wid, C_signal * cs)
{
   C_widget *cwid;

   PREPARECV(cs, cv);
   cwid = _list_get_cwidget(cs->cw->cmachine, wid);
   if (!cwid)
      cwid = _register_widget(cs->cw->cmachine, wid, NULL);
   if (cwid)
      _clip_madd(cs->cw->cmachine, &cv, HASH_CHILD, &cwid->obj);
   INVOKESIGHANDLER(widget, cs, cv);
}

static    gint
list_emit_signal(C_widget * clist, const gchar * signal_name)
{
   ClipMachine *ClipMachineMemory = clist->cmachine;

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   CHECKARG2(3, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_signal_emit_by_name(GTK_OBJECT(clist->widget), signal_name, cwid->widget, clist);
   return 0;
 err:
   return 1;
}

/* Signals table */
static SignalTable list_signals[] = {
   {"selection-changed", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_SELECTION_CHANGED_SIGNAL},
   {"select-child", GSF(handle_select_child_signal), ESF(list_emit_signal),
    GTK_SELECT_CHILD_SIGNAL},
   {"unselect-child", GSF(handle_unselect_child_signal),
    ESF(list_emit_signal), GTK_UNSELECT_CHILD_SIGNAL},
   {"", NULL, NULL, 0}
};

/**********************************************************/

/* Register List in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_list()
{
   return GTK_TYPE_LIST;
}

long
_clip_type_list()
{
   return GTK_WIDGET_LIST;
}

const char *
_clip_type_name_list()
{
   return "GTK_WIDGET_LIST";
}

int
clip_INIT___LIST(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_list, _clip_type_name_list, _gtk_type_list, _gtk_type_container, list_signals);
   return 0;
}

/**********************************************************/

/****  LIST constructor ****/
int
clip_GTK_LISTNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   GtkWidget *wid = NULL;

   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);

   wid = gtk_list_new();
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);

   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

/****  ------------------ ****/

/* Inserts items into the list at the position position.
 * The GList items must not be freed after. */
int
clip_GTK_LISTINSERTITEMS(ClipMachine * ClipMachineMemory)
{
   C_widget *clst = _fetch_cw_arg(ClipMachineMemory);

   ClipVar  *cv = _clip_spar(ClipMachineMemory, 2);

   gint      position = _clip_parni(ClipMachineMemory, 3);

   GList    *items = NULL;

   CHECKCWID(clst, GTK_IS_LIST);
   CHECKARG4(2, ARRAY_type_of_ClipVarType, CHARACTER_type_of_ClipVarType, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      position = 1;
   switch (cv->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
    {
    case CHARACTER_type_of_ClipVarType:
       items =
	g_list_append(items,
		      gtk_list_item_new_with_label(cv->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf));
       break;

    case MAP_type_of_ClipVarType:
    case NUMERIC_type_of_ClipVarType:
       {
	  C_widget *citem = _fetch_cwidget(ClipMachineMemory, cv);

	  CHECKCWID(citem, GTK_IS_WIDGET);
	  if (GTK_IS_LIST_ITEM(citem->widget))
	     items = g_list_append(items, citem->widget);
	  else
	   {
	      GtkWidget *item = gtk_list_item_new();

	      gtk_container_add(GTK_CONTAINER(item), citem->widget);
	      items = g_list_append(items, item);
	   }
	  break;
       }

    case ARRAY_type_of_ClipVarType:
       {
	  C_widget *citem;

	  GtkWidget *item;

	  int       i;

	  for (i = 0; i < cv->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar; i++)
	   {
	      switch (cv->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar[i].ClipType_t_of_ClipVar.
		      ClipVartype_type_of_ClipType)
	       {
	       case CHARACTER_type_of_ClipVarType:
		  items =
		   g_list_append(items,
				 gtk_list_item_new_with_label(cv->ClipArrVar_a_of_ClipVar.
							      ClipVar_items_of_ClipArrVar[i].ClipStrVar_s_of_ClipVar.
							      ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf));
		  break;
	       case MAP_type_of_ClipVarType:
	       case NUMERIC_type_of_ClipVarType:
		  citem = _fetch_cwidget(ClipMachineMemory, &cv->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar[i]);
		  CHECKCWID(citem, GTK_IS_WIDGET);
		  if (GTK_IS_LIST_ITEM(citem->widget))
		     items = g_list_append(items, citem->widget);
		  else
		   {
		      item = gtk_list_item_new();
		      gtk_container_add(GTK_CONTAINER(item), citem->widget);
		      items = g_list_append(items, item);
		   }
		  break;
	       default:
		  break;
	       }
	   }

       }
    default:
       break;
    }
   gtk_list_insert_items(GTK_LIST(clst->widget), items, position - 1);
   return 0;
 err:
   return 1;
}

/* Adds items to the end of the list. */
int
clip_GTK_LISTAPPENDITEMS(ClipMachine * ClipMachineMemory)
{
   C_widget *clst = _fetch_cw_arg(ClipMachineMemory);

   ClipVar  *cv = _clip_spar(ClipMachineMemory, 2);

   GList    *items = NULL;

   CHECKCWID(clst, GTK_IS_LIST);
   CHECKARG4(2, ARRAY_type_of_ClipVarType, CHARACTER_type_of_ClipVarType, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   switch (cv->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
    {
    case CHARACTER_type_of_ClipVarType:
       items =
	g_list_append(items,
		      gtk_list_item_new_with_label(cv->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf));
       break;

    case MAP_type_of_ClipVarType:
    case NUMERIC_type_of_ClipVarType:
       {
	  C_widget *citem = _fetch_cwidget(ClipMachineMemory, cv);

	  CHECKCWID(citem, GTK_IS_WIDGET);
	  if (GTK_IS_LIST_ITEM(citem->widget))
	     items = g_list_append(items, citem->widget);
	  else
	   {
	      GtkWidget *item = gtk_list_item_new();

	      gtk_container_add(GTK_CONTAINER(item), citem->widget);
	      items = g_list_append(items, item);
	   }
	  break;
       }

    case ARRAY_type_of_ClipVarType:
       {
	  C_widget *citem;

	  GtkWidget *item;

	  int       i;

	  for (i = 0; i < cv->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar; i++)
	   {
	      switch (cv->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar[i].ClipType_t_of_ClipVar.
		      ClipVartype_type_of_ClipType)
	       {
	       case CHARACTER_type_of_ClipVarType:
		  items =
		   g_list_append(items,
				 gtk_list_item_new_with_label(cv->ClipArrVar_a_of_ClipVar.
							      ClipVar_items_of_ClipArrVar[i].ClipStrVar_s_of_ClipVar.
							      ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf));
		  break;
	       case MAP_type_of_ClipVarType:
	       case NUMERIC_type_of_ClipVarType:
		  citem = _fetch_cwidget(ClipMachineMemory, &cv->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar[i]);
		  CHECKCWID(citem, GTK_IS_WIDGET);
		  if (GTK_IS_LIST_ITEM(citem->widget))
		     items = g_list_append(items, citem->widget);
		  else
		   {
		      item = gtk_list_item_new();
		      gtk_container_add(GTK_CONTAINER(item), citem->widget);
		      items = g_list_append(items, item);
		   }
		  break;
	       default:
		  break;
	       }
	   }

       }
    default:
       break;
    }
   gtk_list_append_items(GTK_LIST(clst->widget), items);
   return 0;
 err:
   return 1;
}

/* Inserts items at the beginning of the list. */
int
clip_GTK_LISTPREPENDITEMS(ClipMachine * ClipMachineMemory)
{
   C_widget *clst = _fetch_cw_arg(ClipMachineMemory);

   ClipVar  *cv = _clip_spar(ClipMachineMemory, 2);

   GList    *items = NULL;

   CHECKCWID(clst, GTK_IS_LIST);
   CHECKARG4(2, ARRAY_type_of_ClipVarType, CHARACTER_type_of_ClipVarType, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   switch (cv->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
    {
    case CHARACTER_type_of_ClipVarType:
       items =
	g_list_append(items,
		      gtk_list_item_new_with_label(cv->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf));
       break;

    case MAP_type_of_ClipVarType:
    case NUMERIC_type_of_ClipVarType:
       {
	  C_widget *citem = _fetch_cwidget(ClipMachineMemory, cv);

	  CHECKCWID(citem, GTK_IS_WIDGET);
	  if (GTK_IS_LIST_ITEM(citem->widget))
	     items = g_list_append(items, citem->widget);
	  else
	   {
	      GtkWidget *item = gtk_list_item_new();

	      gtk_container_add(GTK_CONTAINER(item), citem->widget);
	      items = g_list_append(items, item);
	   }
	  break;
       }

    case ARRAY_type_of_ClipVarType:
       {
	  C_widget *citem;

	  GtkWidget *item;

	  int       i;

	  for (i = 0; i < cv->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar; i++)
	   {
	      switch (cv->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar[i].ClipType_t_of_ClipVar.
		      ClipVartype_type_of_ClipType)
	       {
	       case CHARACTER_type_of_ClipVarType:
		  items =
		   g_list_append(items,
				 gtk_list_item_new_with_label(cv->ClipArrVar_a_of_ClipVar.
							      ClipVar_items_of_ClipArrVar[i].ClipStrVar_s_of_ClipVar.
							      ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf));
		  break;
	       case MAP_type_of_ClipVarType:
	       case NUMERIC_type_of_ClipVarType:
		  citem = _fetch_cwidget(ClipMachineMemory, &cv->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar[i]);
		  CHECKCWID(citem, GTK_IS_WIDGET);
		  if (GTK_IS_LIST_ITEM(citem->widget))
		     items = g_list_append(items, citem->widget);
		  else
		   {
		      item = gtk_list_item_new();
		      gtk_container_add(GTK_CONTAINER(item), citem->widget);
		      items = g_list_append(items, item);
		   }
		  break;
	       default:
		  break;
	       }
	   }

       }
    default:
       break;
    }
   gtk_list_prepend_items(GTK_LIST(clst->widget), items);
   return 0;
 err:
   return 1;
}

/* Removes the items from the list. */
int
clip_GTK_LISTREMOVEITEMS(ClipMachine * ClipMachineMemory)
{
   C_widget *clst = _fetch_cw_arg(ClipMachineMemory);

   ClipVar  *cv = _clip_spar(ClipMachineMemory, 2);

   GList    *items = NULL;

   C_widget *citem;

   int       i;

   CHECKCWID(clst, GTK_IS_LIST);
   CHECKARG(2, ARRAY_type_of_ClipVarType);
   for (i = 0; i < cv->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar; i++)
    {
       switch (cv->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar[i].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
	{
	case MAP_type_of_ClipVarType:
	case NUMERIC_type_of_ClipVarType:
	   citem = _fetch_cwidget(ClipMachineMemory, &cv->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar[i]);
	   CHECKCWID(citem, GTK_IS_LIST_ITEM);
	   items = g_list_append(items, citem->widget);
	   break;
	default:
	   break;
	}
    }
   gtk_list_remove_items(GTK_LIST(clst->widget), items);
   return 0;
 err:
   return 1;
}

/* Removes the items from the list, without unreferencing them.
 * It may be useful if you want to move the items from one list to another. */
int
clip_GTK_LISTREMOVEITEMSNOUNREF(ClipMachine * ClipMachineMemory)
{
   C_widget *clst = _fetch_cw_arg(ClipMachineMemory);

   ClipVar  *cv = _clip_spar(ClipMachineMemory, 2);

   GList    *items = NULL;

   C_widget *citem;

   int       i;

   CHECKCWID(clst, GTK_IS_LIST);
   CHECKARG(2, ARRAY_type_of_ClipVarType);
   for (i = 0; i < cv->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar; i++)
    {
       switch (cv->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar[i].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
	{
	case MAP_type_of_ClipVarType:
	case NUMERIC_type_of_ClipVarType:
	   citem = _fetch_cwidget(ClipMachineMemory, &cv->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar[i]);
	   CHECKCWID(citem, GTK_IS_LIST_ITEM);
	   items = g_list_append(items, citem->widget);
	   break;
	default:
	   break;
	}
    }
   gtk_list_remove_items_no_unref(GTK_LIST(clst->widget), items);
   return 0;
 err:
   return 1;
}

/* Removes the items between index start (included) and end (excluded)
 * from the list. If end is negative, or greater than the number of
 * children of list, it's assumed to be exactly the number of elements.
 * If start is greater than or equal to end, nothing is done. */
int
clip_GTK_LISTCLEARITEMS(ClipMachine * ClipMachineMemory)
{
   C_widget *clst = _fetch_cw_arg(ClipMachineMemory);

   gint      start = INT_OPTION(ClipMachineMemory, 2, 1);

   gint      end = INT_OPTION(ClipMachineMemory, 3, 1);

   CHECKCWID(clst, GTK_IS_LIST);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   gtk_list_clear_items(GTK_LIST(clst->widget), start - 1, end - 1);
   return 0;
 err:
   return 1;
}

/* Selects the child number item of the list. Nothing happens if item
 * is out of bounds. The signal GtkList::select-child will be emitted. */
int
clip_GTK_LISTSELECTITEM(ClipMachine * ClipMachineMemory)
{
   C_widget *clst = _fetch_cw_arg(ClipMachineMemory);

   gint      item = INT_OPTION(ClipMachineMemory, 2, 1);

   CHECKCWID(clst, GTK_IS_LIST);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_list_select_item(GTK_LIST(clst->widget), item - 1);
   return 0;
 err:
   return 1;
}

/* Unselects the child number item of the list. Nothing happens if item
 * is out of bounds. The signal GtkList::unselect-child will be emitted. */
int
clip_GTK_LISTUNSELECTITEM(ClipMachine * ClipMachineMemory)
{
   C_widget *clst = _fetch_cw_arg(ClipMachineMemory);

   gint      item = INT_OPTION(ClipMachineMemory, 2, 1);

   CHECKCWID(clst, GTK_IS_LIST);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_list_unselect_item(GTK_LIST(clst->widget), item - 1);
   return 0;
 err:
   return 1;
}

/* Selects the given child. The signal GtkList::select-child will be emitted. */
int
clip_GTK_LISTSELECTCHILD(ClipMachine * ClipMachineMemory)
{
   C_widget *clst = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cchild = CWIDGET_ARG(ClipMachineMemory, 2);

   CHECKCWID(clst, GTK_IS_LIST);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cchild, GTK_IS_LIST_ITEM);
   gtk_list_select_child(GTK_LIST(clst->widget), cchild->widget);
   return 0;
 err:
   return 1;
}

/* Unselects the given child. The signal GtkList::unselect-child will be emitted. */
int
clip_GTK_LISTUNSELECTCHILD(ClipMachine * ClipMachineMemory)
{
   C_widget *clst = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cchild = CWIDGET_ARG(ClipMachineMemory, 2);

   CHECKCWID(clst, GTK_IS_LIST);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cchild, GTK_IS_LIST_ITEM);
   gtk_list_unselect_child(GTK_LIST(clst->widget), cchild->widget);
   return 0;
 err:
   return 1;
}

/* Searches the children of list for the index of child. */
int
clip_GTK_LISTCHILDPOSITION(ClipMachine * ClipMachineMemory)
{
   C_widget *clst = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cchild = CWIDGET_ARG(ClipMachineMemory, 2);

   CHECKCWID(clst, GTK_IS_LIST);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cchild, GTK_IS_LIST_ITEM);
   _clip_retni(ClipMachineMemory, gtk_list_child_position(GTK_LIST(clst->widget), cchild->widget));
   return 0;
 err:
   return 1;
}

/*
Set the list selection mode. The selection mode can be any value in GtkSelectionMode:

GTK_SELECTION_SINGLE

    Zero or one element may be selected.

GTK_SELECTION_BROWSE

    Exactly one element is always selected (this can be false after you have changed the selection mode).

GTK_SELECTION_MULTIPLE

    Any number of elements may be selected. Clicks toggle the state of an item.

GTK_SELECTION_EXTENDED

    Any number of elements may be selected. Click-drag selects a range of elements; the Ctrl key may be used to enlarge the
    selection, and Shift key to select between the focus and the child pointed to.
*/
int
clip_GTK_LISTSETSELECTIONMODE(ClipMachine * ClipMachineMemory)
{
   C_widget *clst = _fetch_cw_arg(ClipMachineMemory);

   GtkSelectionMode mode = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(clst, GTK_IS_LIST);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_list_set_selection_mode(GTK_LIST(clst->widget), mode);
   return 0;
 err:
   return 1;
}

/* Extends the selection by moving the anchor according
 * to scroll_type. Only in GTK_SELECTION_EXTENDED. */
int
clip_GTK_LISTEXTENDSELECTION(ClipMachine * ClipMachineMemory)
{
   C_widget *clst = _fetch_cw_arg(ClipMachineMemory);

   GtkScrollType scroll_type = _clip_parni(ClipMachineMemory, 2);

   gfloat    position = _clip_parnd(ClipMachineMemory, 3);

   gboolean  auto_start_selection = BOOL_OPTION(ClipMachineMemory, 4, TRUE);

   CHECKCWID(clst, GTK_IS_LIST);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, LOGICAL_type_of_ClipVarType);
   gtk_list_extend_selection(GTK_LIST(clst->widget), scroll_type, position, auto_start_selection);
   return 0;
 err:
   return 1;
}

/* Starts a selection (or part of selection) at the
 * focused child. Only in GTK_SELECTION_EXTENDED mode. */
int
clip_GTK_LISTSTARTSELECTION(ClipMachine * ClipMachineMemory)
{
   C_widget *clst = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(clst, GTK_IS_LIST);
   gtk_list_start_selection(GTK_LIST(clst->widget));
   return 0;
 err:
   return 1;
}

/* Ends the selection. Used with gtk_list_extend_selection and
 * gtk_list_start_selection. Only in GTK_SELECTION_EXTENDED. */
int
clip_GTK_LISTENDSELECTION(ClipMachine * ClipMachineMemory)
{
   C_widget *clst = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(clst, GTK_IS_LIST);
   gtk_list_end_selection(GTK_LIST(clst->widget));
   return 0;
 err:
   return 1;
}

/* Selects all children of list. A signal will
 * be emitted for each newly selected child. */
int
clip_GTK_LISTSELECTALL(ClipMachine * ClipMachineMemory)
{
   C_widget *clst = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(clst, GTK_IS_LIST);
   gtk_list_select_all(GTK_LIST(clst->widget));
   return 0;
 err:
   return 1;
}

/* Unselects all children of list. A signal will
 * be emitted for each newly unselected child. */
int
clip_GTK_LISTUNSELECTALL(ClipMachine * ClipMachineMemory)
{
   C_widget *clst = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(clst, GTK_IS_LIST);
   gtk_list_unselect_all(GTK_LIST(clst->widget));
   return 0;
 err:
   return 1;
}

/* Scrolls list horizontaly. This supposes that the list is packed into a
 * scrolled window or something similar, and adjustments are well set. Step
 * and page increment are those from the horizontal adjustment of list.
 * Backward means to the left, and forward to the right. Out of bounds values
 * are truncated. scroll_type may be any valid GtkScrollType. If scroll_type is
 * GTK_SCROLL_NONE, nothing is done. If it's GTK_SCROLL_JUMP, the list scrolls
 * to the ratio position: 0 is full left, 1 is full right. */
int
clip_GTK_LISTSCROLLHORIZONTAL(ClipMachine * ClipMachineMemory)
{
   C_widget *clst = _fetch_cw_arg(ClipMachineMemory);

   GtkScrollType scroll_type = _clip_parni(ClipMachineMemory, 2);

   gfloat    position = _clip_parnd(ClipMachineMemory, 3);

   CHECKCWID(clst, GTK_IS_LIST);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   gtk_list_scroll_horizontal(GTK_LIST(clst->widget), scroll_type, position);
   return 0;
 err:
   return 1;
}

/* Scrolls list vertically. This supposes that the list is packed into a
 * scrolled window or something similar, and adjustments are well set. Step
 * and page increment are those from the vertical adjustment of list.
 * Backward means up, and forward down. Out of bounds values are truncated.
 * scroll_type may be any valid GtkScrollType. If scroll_type is GTK_SCROLL_NONE,
 * nothing is done. If it's GTK_SCROLL_JUMP, the list scrolls to the ratio
 * position: 0 is top, 1 is bottom. */
int
clip_GTK_LISTSCROLLVERTICAL(ClipMachine * ClipMachineMemory)
{
   C_widget *clst = _fetch_cw_arg(ClipMachineMemory);

   GtkScrollType scroll_type = _clip_parni(ClipMachineMemory, 2);

   gfloat    position = _clip_parnd(ClipMachineMemory, 3);

   CHECKCWID(clst, GTK_IS_LIST);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   gtk_list_scroll_vertical(GTK_LIST(clst->widget), scroll_type, position);
   return 0;
 err:
   return 1;
}

/* Toggles between adding to the selection and beginning a new selection.
 * Only in GTK_SELECTION_EXTENDED. Useful with gtk_list_extend_selection. */
int
clip_GTK_LISTTOGGLEADDMODE(ClipMachine * ClipMachineMemory)
{
   C_widget *clst = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(clst, GTK_IS_LIST);
   gtk_list_toggle_add_mode(GTK_LIST(clst->widget));
   return 0;
 err:
   return 1;
}

/* Toggles the focus row. If the focus row is selected, it's unselected.
 * If the focus row is unselected, it's selected. If the selection mode of
 * list is GTK_SELECTION_BROWSE, this has no effect, as the selection is
 * always at the focus row. */
int
clip_GTK_LISTTOGGLEFOCUSROW(ClipMachine * ClipMachineMemory)
{
   C_widget *clst = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(clst, GTK_IS_LIST);
   gtk_list_toggle_focus_row(GTK_LIST(clst->widget));
   return 0;
 err:
   return 1;
}

/* Toggles the child item of list. If the selection mode of list is
 * GTK_SELECTION_BROWSE, the item is selected, and the others are unselected. */
int
clip_GTK_LISTTOGGLEROW(ClipMachine * ClipMachineMemory)
{
   C_widget *clst = _fetch_cw_arg(ClipMachineMemory);

   C_widget *citem = CWIDGET_ARG(ClipMachineMemory, 2);

   CHECKCWID(clst, GTK_IS_LIST);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(citem, GTK_IS_LIST_ITEM);
   gtk_list_toggle_row(GTK_LIST(clst->widget), citem->widget);
   return 0;
 err:
   return 1;
}

/* Restores the selection in the last state, only if selection mode is
 * GTK_SELECTION_EXTENDED. If this function is called twice, the selection
 * is cleared. This function sometimes gives stranges "last states". */
int
clip_GTK_LISTUNDOSELECTION(ClipMachine * ClipMachineMemory)
{
   C_widget *clst = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(clst, GTK_IS_LIST);
   gtk_list_undo_selection(GTK_LIST(clst->widget));
   return 0;
 err:
   return 1;
}

/* Stops the drag selection mode and ungrabs the pointer.
 * This has no effect if a drag selection is not active. */
int
clip_GTK_LISTENDDRAGSELECTION(ClipMachine * ClipMachineMemory)
{
   C_widget *clst = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(clst, GTK_IS_LIST);
   gtk_list_end_drag_selection(GTK_LIST(clst->widget));
   return 0;
 err:
   return 1;
}
