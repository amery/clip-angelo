/*
    Copyright (C) 2005  ITK
    Author  : Kornilova E.V. <alena@itk.ru>
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
/* Signals table */
static int
handle_item_activated(GtkIconView * view, GtkTreePath * path, C_signal * cs)
{
   C_object *cpath;

   PREPARECV(cs, cv);

   cpath = _list_get_cobject(cs->cw->cmachine, path);
   if (!cpath)
      cpath = _register_object(cs->cw->cmachine, path, GTK_TYPE_TREE_PATH, NULL, NULL);
   if (cpath)
      _clip_madd(cs->cw->cmachine, &cv, HASH_TREEPATH, &cpath->obj);

   INVOKESIGHANDLER(GTK_WIDGET(view), cs, cv);
}

static int
handle_move_cursor(GtkIconView * view, GtkMovementStep arg1, gint arg2, C_signal * cs)
{
   PREPARECV(cs, cv);

   _clip_mputn(cs->cw->cmachine, &cv, HASH_MOVEMENTSTEP, (int) arg1);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_ARG2, arg2);

   INVOKESIGHANDLER(GTK_WIDGET(view), cs, cv);
}

static int
handle_set_scroll_adjustments(GtkIconView * view, GtkAdjustment * arg1, GtkAdjustment * arg2, C_signal * cs)
{
   C_widget *carg1;
   C_widget *carg2;
   PREPARECV(cs, cv);

   carg1 = _list_get_cwidget(cs->cw->cmachine, (GtkWidget *) arg1);
   if (!carg1)
      carg1 = _register_widget(cs->cw->cmachine, (GtkWidget *) arg1, NULL);
   if (carg1)
      _clip_madd(cs->cw->cmachine, &cv, HASH_HADJUSTMENT, &carg1->obj);

   carg2 = _list_get_cwidget(cs->cw->cmachine, (GtkWidget *) arg2);
   if (!carg2)
      carg2 = _register_widget(cs->cw->cmachine, (GtkWidget *) arg2, NULL);
   if (carg2)
      _clip_madd(cs->cw->cmachine, &cv, HASH_VADJUSTMENT, &carg2->obj);

   INVOKESIGHANDLER(GTK_WIDGET(view), cs, cv);
}

static SignalTable icon_view_signals[] = {
   {"activate-cursor-item", GSF(widget_signal_handler),
    ESF(object_emit_signal), GTK_ACTIVATE_CURSOR_ITEM_SIGNAL},
   {"item-activated", GSF(handle_item_activated), ESF(object_emit_signal),
    GTK_ITEM_ACTIVATED_SIGNAL},
   {"move-cursor", GSF(handle_move_cursor), ESF(object_emit_signal),
    GTK_MOVE_CURSOR_SIGNAL},
   {"select-all", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_SELECT_ALL_SIGNAL},
   {"select-cursor-item", GSF(widget_signal_handler),
    ESF(object_emit_signal), GTK_SELECT_CURSOR_ITEM_SIGNAL},
   {"selection-changed", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_SELECTION_CHANGED_SIGNAL},
   {"set-scroll-adjustments", GSF(handle_set_scroll_adjustments),
    ESF(object_emit_signal), GTK_SET_SCROLL_ADJUSTMENTS_SIGNAL},
   {"toggle-cursor-item", GSF(widget_signal_handler),
    ESF(object_emit_signal), GTK_TOGGLE_CURSOR_ITEM_SIGNAL},
   {"unselect-all", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_UNSELECT_ALL_SIGNAL},
   {"", NULL, NULL, 0}
};

CLIP_DLLEXPORT GtkType
_gtk_type_icon_view()
{
   return GTK_TYPE_ICON_VIEW;
}

long
_clip_type_icon_view()
{
   return GTK_WIDGET_ICON_VIEW;
}

const char *
_clip_type_name_icon_view()
{
   return "GTK_WIDGET_ICON_VIEW";
}

int
clip_INIT___ICON_VIEW(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_icon_view, _clip_type_name_icon_view, _gtk_type_icon_view, _gtk_type_container, icon_view_signals);
   return 0;
}

/**********************************************************/

int
clip_GTK_ICONVIEWNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   GtkWidget *wid = NULL;
   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);

   wid = gtk_icon_view_new();

   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONVIEWNEWWITHMODEL(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   C_object *cmodel = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   GtkWidget *wid = NULL;
   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(cmodel, GTK_IS_TREE_MODEL(cmodel->object));

   wid = gtk_icon_view_new_with_model(GTK_TREE_MODEL(cmodel->object));

   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONVIEWSETMODEL(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);
   C_object *cmodel = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(cview, GTK_IS_ICON_VIEW);
   CHECKCOBJ(cmodel, GTK_IS_TREE_MODEL(cmodel->object));

   gtk_icon_view_set_model(GTK_ICON_VIEW(cview->widget), GTK_TREE_MODEL(cmodel->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONVIEWGETMODEL(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);
   C_object *cmodel;
   GtkTreeModel *model;

   CHECKCWID(cview, GTK_IS_ICON_VIEW);

   model = gtk_icon_view_get_model(GTK_ICON_VIEW(cview->widget));

   if (model)
      {
	 cmodel = _list_get_cobject(ClipMachineMemory, model);
	 if (!cmodel)
	    cmodel = _register_object(ClipMachineMemory, model, GTK_TYPE_TREE_MODEL, NULL, NULL);
	 if (cmodel)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cmodel->obj);
      }

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONVIEWSETTEXTCOLUMN(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);
   gint column = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(cview, GTK_IS_ICON_VIEW);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   column--;

   gtk_icon_view_set_text_column(GTK_ICON_VIEW(cview->widget), column);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONVIEWGETTEXTCOLUMN(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cview, GTK_IS_ICON_VIEW);

   _clip_retni(ClipMachineMemory, gtk_icon_view_get_text_column(GTK_ICON_VIEW(cview->widget)) - 1);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONVIEWSETMARKUPCOLUMN(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);
   gint column = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(cview, GTK_IS_ICON_VIEW);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   column--;

   gtk_icon_view_set_markup_column(GTK_ICON_VIEW(cview->widget), column);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONVIEWGETMARKUPCOLUMN(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cview, GTK_IS_ICON_VIEW);

   _clip_retni(ClipMachineMemory, gtk_icon_view_get_markup_column(GTK_ICON_VIEW(cview->widget)) - 1);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONVIEWSETPIXBUFCOLUMN(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);
   gint column = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(cview, GTK_IS_ICON_VIEW);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   column--;

   gtk_icon_view_set_pixbuf_column(GTK_ICON_VIEW(cview->widget), column);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONVIEWGETPIXBUFCOLUMN(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cview, GTK_IS_ICON_VIEW);

   _clip_retni(ClipMachineMemory, gtk_icon_view_get_pixbuf_column(GTK_ICON_VIEW(cview->widget)) - 1);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONVIEWGETPATHATPOS(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);
   gint x = _clip_parni(ClipMachineMemory, 2);
   gint y = _clip_parni(ClipMachineMemory, 3);
   C_object *cpath;
   GtkTreePath *path;

   CHECKCWID(cview, GTK_IS_ICON_VIEW);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);

   path = gtk_icon_view_get_path_at_pos(GTK_ICON_VIEW(cview->widget), x, y);

   if (path)
      {
	 cpath = _list_get_cobject(ClipMachineMemory, path);
	 if (!cpath)
	    cpath = _register_object(ClipMachineMemory, path, GTK_TYPE_TREE_PATH, NULL, NULL);
	 if (cpath)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cpath->obj);
      }

   return 0;
 err:
   return 1;
}

static void
_selected_foreach(GtkIconView * view, GtkTreePath * path, gpointer data)
{
   C_var *c = (C_var *) data;
   C_widget *c_wid = _list_get_cwidget(c->ClipMachineMemory, view);
   C_object *cpath;
   ClipVar stack[3];
   ClipVar res;
   if (!c_wid)
      c_wid = _register_widget(c->ClipMachineMemory, GTK_WIDGET(view), NULL);

   cpath = _list_get_cobject(c->ClipMachineMemory, path);
   if (!cpath)
      cpath = _register_object(c->ClipMachineMemory, path, GTK_TYPE_TREE_PATH, NULL, NULL);

   if (c_wid)
      {
	 memset(&stack, 0, sizeof(stack));
	 memset(&res, 0, sizeof(ClipVar));
	 _clip_mclone(c->cw->cmachine, &stack[0], &c->cw->obj);
	 _clip_mclone(c->cw->cmachine, &stack[1], &cpath->obj);
	 if (c->cv)
	    _clip_mclone(c->cw->cmachine, &stack[2], c->cv);
	 _clip_eval(c->ClipMachineMemory, &(c->cfunc), 3, stack, &res);
	 _clip_destroy(c->ClipMachineMemory, &res);
	 _clip_destroy(c->ClipMachineMemory, &stack[0]);
	 _clip_destroy(c->ClipMachineMemory, &stack[1]);
	 if (c->cv)
	    _clip_destroy(c->ClipMachineMemory, &stack[2]);
      }
}

int
clip_GTK_ICONVIEWSELECTEDFOREACH(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);
   ClipVar *cfunc = _clip_spar(ClipMachineMemory, 2);
   ClipVar *cdata = _clip_spar(ClipMachineMemory, 3);
   C_var *c = 0;

   CHECKCWID(cview, GTK_IS_ICON_VIEW);
   CHECKARG2(2, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);

   c->ClipMachineMemory = ClipMachineMemory;
   c->cw = cview;
   _clip_mclone(ClipMachineMemory, &c->cfunc, cfunc);
   if (cdata)
      _clip_mclone(ClipMachineMemory, c->cv, cdata);

   gtk_icon_view_selected_foreach(GTK_ICON_VIEW(cview->widget), (GtkIconViewForeachFunc) _selected_foreach, c);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONVIEWSETSELECTIONMODE(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);
   GtkSelectionMode mode = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(cview, GTK_IS_ICON_VIEW);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_icon_view_set_selection_mode(GTK_ICON_VIEW(cview->widget), mode);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONVIEWGETSELECTIONMODE(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cview, GTK_IS_ICON_VIEW);

   _clip_retni(ClipMachineMemory, (int) gtk_icon_view_get_selection_mode(GTK_ICON_VIEW(cview->widget)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONVIEWSETORIENTATION(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);
   GtkOrientation mode = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(cview, GTK_IS_ICON_VIEW);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_icon_view_set_orientation(GTK_ICON_VIEW(cview->widget), mode);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONVIEWGETORIENTATION(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cview, GTK_IS_ICON_VIEW);

   _clip_retni(ClipMachineMemory, (int) gtk_icon_view_get_orientation(GTK_ICON_VIEW(cview->widget)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONVIEWSETCOLUMNS(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);
   gint columns = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(cview, GTK_IS_ICON_VIEW);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_icon_view_set_columns(GTK_ICON_VIEW(cview->widget), columns);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONVIEWGETCOLUMNS(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cview, GTK_IS_ICON_VIEW);

   _clip_retni(ClipMachineMemory, gtk_icon_view_get_columns(GTK_ICON_VIEW(cview->widget)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONVIEWSETITEMWIDTH(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);
   gint width = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(cview, GTK_IS_ICON_VIEW);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_icon_view_set_item_width(GTK_ICON_VIEW(cview->widget), width);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONVIEWGETITEMWIDTH(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cview, GTK_IS_ICON_VIEW);

   _clip_retni(ClipMachineMemory, gtk_icon_view_get_item_width(GTK_ICON_VIEW(cview->widget)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONVIEWSETSPACING(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);
   gint spacing = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(cview, GTK_IS_ICON_VIEW);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_icon_view_set_spacing(GTK_ICON_VIEW(cview->widget), spacing);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONVIEWGETSPACING(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cview, GTK_IS_ICON_VIEW);

   _clip_retni(ClipMachineMemory, gtk_icon_view_get_spacing(GTK_ICON_VIEW(cview->widget)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONVIEWSETROWSPACING(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);
   gint spacing = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(cview, GTK_IS_ICON_VIEW);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_icon_view_set_row_spacing(GTK_ICON_VIEW(cview->widget), spacing);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONVIEWGETROWSPACING(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cview, GTK_IS_ICON_VIEW);

   _clip_retni(ClipMachineMemory, gtk_icon_view_get_row_spacing(GTK_ICON_VIEW(cview->widget)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONVIEWSETCOLUMNSPACING(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);
   gint spacing = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(cview, GTK_IS_ICON_VIEW);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_icon_view_set_column_spacing(GTK_ICON_VIEW(cview->widget), spacing);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONVIEWGETCOLUMNSPACING(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cview, GTK_IS_ICON_VIEW);

   _clip_retni(ClipMachineMemory, gtk_icon_view_get_column_spacing(GTK_ICON_VIEW(cview->widget)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONVIEWSETMARGIN(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);
   gint margin = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(cview, GTK_IS_ICON_VIEW);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_icon_view_set_margin(GTK_ICON_VIEW(cview->widget), margin);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONVIEWGETMARGIN(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cview, GTK_IS_ICON_VIEW);

   _clip_retni(ClipMachineMemory, gtk_icon_view_get_margin(GTK_ICON_VIEW(cview->widget)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONVIEWSELECTPATH(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);
   C_object *cpath = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(cview, GTK_IS_ICON_VIEW);
   CHECKCOBJ(cpath, GTK_IS_TREE_PATH(cpath->object));

   gtk_icon_view_select_path(GTK_ICON_VIEW(cview->widget), GTK_TREE_PATH(cpath->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONVIEWUNSELECTPATH(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);
   C_object *cpath = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(cview, GTK_IS_ICON_VIEW);
   CHECKCOBJ(cpath, GTK_IS_TREE_PATH(cpath->object));

   gtk_icon_view_unselect_path(GTK_ICON_VIEW(cview->widget), GTK_TREE_PATH(cpath->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONVIEWPATHISSELECTED(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);
   C_object *cpath = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(cview, GTK_IS_ICON_VIEW);
   CHECKCOBJ(cpath, GTK_IS_TREE_PATH(cpath->object));

   _clip_retl(ClipMachineMemory, gtk_icon_view_path_is_selected(GTK_ICON_VIEW(cview->widget), GTK_TREE_PATH(cpath->object)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONVIEWGETSELECTEDITEMS(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);
   GList *list;
   ClipVar *cv = RETPTR(ClipMachineMemory);
   long l;

   CHECKCWID(cview, GTK_IS_ICON_VIEW);

   list = gtk_icon_view_get_selected_items(GTK_ICON_VIEW(cview->widget));
   l = g_list_length(list);
   _clip_array(ClipMachineMemory, cv, 1, &l);
   for (l = 0; list; list = g_list_next(list), l++)
      {
	 C_object *cpath;
	 GtkTreePath *path;

	 path = GTK_TREE_PATH(list->data);
	 if (path)
	    {
	       cpath = _list_get_cobject(ClipMachineMemory, path);
	       if (!cpath)
		  cpath = _register_object(ClipMachineMemory, path, GTK_TYPE_TREE_PATH, NULL, NULL);
	       if (cpath)
		  _clip_aset(ClipMachineMemory, cv, &cpath->obj, 1, &l);
	    }
      }

   g_list_free(list);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONVIEWSELECTALL(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cview, GTK_IS_ICON_VIEW);

   gtk_icon_view_select_all(GTK_ICON_VIEW(cview->widget));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONVIEWUNSELECTALL(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cview, GTK_IS_ICON_VIEW);

   gtk_icon_view_unselect_all(GTK_ICON_VIEW(cview->widget));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONVIEWITEMACTIVATED(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);
   C_object *cpath = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(cview, GTK_IS_ICON_VIEW);
   CHECKCOBJ(cpath, GTK_IS_TREE_PATH(cpath->object));

   gtk_icon_view_item_activated(GTK_ICON_VIEW(cview->widget), GTK_TREE_PATH(cpath->object));

   return 0;
 err:
   return 1;
}
