/*
    Copyright (C) 2003-2005  ITK
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
static int
handle_expand_collapse_cursor_row(GtkTreeView * tree, gboolean arg1, gboolean arg2, gboolean arg3, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputl(cs->cw->cmachine, &cv, HASH_ARG1, arg1);
   _clip_mputl(cs->cw->cmachine, &cv, HASH_ARG2, arg2);
   _clip_mputl(cs->cw->cmachine, &cv, HASH_ARG3, arg3);
   INVOKESIGHANDLER(GTK_WIDGET(tree), cs, cv);
}

static int
handle_move_cursor(GtkTreeView * tree, GtkMovementStep arg1, gint arg2, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_STEP, arg1);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_COUNT, arg2);
   INVOKESIGHANDLER(GTK_WIDGET(tree), cs, cv);
}

#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 4)
static int
handle_row_activated(GtkTreeView * tree, GtkTreePath * arg1, GtkTreeViewColumn * arg2, C_signal * cs)
{
   C_object *cpath;

   C_object *ccolumn;

   PREPARECV(cs, cv);

   if (arg1)
    {
       cpath = _list_get_cobject(cs->cw->cmachine, arg1);
       if (!cpath)
	  cpath = _register_object(cs->cw->cmachine, arg1, GTK_TYPE_TREE_PATH, NULL, NULL);
       if (cpath)
	  _clip_madd(cs->cw->cmachine, &cv, HASH_TREEPATH, &cpath->obj);
    }

   if (arg2)
    {
       ccolumn = _list_get_cobject(cs->cw->cmachine, arg2);
       if (!ccolumn)
	  ccolumn = _register_object(cs->cw->cmachine, arg2, GTK_TYPE_TREE_VIEW_COLUMN, NULL, NULL);
       if (ccolumn)
	  _clip_madd(cs->cw->cmachine, &cv, HASH_VIEWCOLUMN, &ccolumn->obj);
    }
   INVOKESIGHANDLER(GTK_WIDGET(tree), cs, cv);
}

static int
handle_row_collapsed(GtkTreeView * tree, GtkTreeIter * arg1, GtkTreePath * arg2, C_signal * cs)
{
   C_object *cpath;

   C_object *citer;

   PREPARECV(cs, cv);
   if (arg1)
    {
       citer = _list_get_cobject(cs->cw->cmachine, arg1);
       if (!citer)
	  citer = _register_object(cs->cw->cmachine, arg1, GTK_TYPE_TREE_ITER, NULL, NULL);
       if (citer)
	  _clip_madd(cs->cw->cmachine, &cv, HASH_TREEITER, &citer->obj);
    }

   if (arg2)
    {
       cpath = _list_get_cobject(cs->cw->cmachine, arg2);
       if (!cpath)
	  cpath = _register_object(cs->cw->cmachine, arg2, GTK_TYPE_TREE_PATH, NULL, NULL);
       if (cpath)
	  _clip_madd(cs->cw->cmachine, &cv, HASH_TREEPATH, &cpath->obj);
    }

   INVOKESIGHANDLER(GTK_WIDGET(tree), cs, cv);
}

static int
handle_row_expanded(GtkTreeView * tree, GtkTreeIter * arg1, GtkTreePath * arg2, C_signal * cs)
{
   C_object *cpath;

   C_object *citer;

   PREPARECV(cs, cv);
   if (arg1)
    {
       citer = _list_get_cobject(cs->cw->cmachine, arg1);
       if (!citer)
	  citer = _register_object(cs->cw->cmachine, arg1, GTK_TYPE_TREE_ITER, NULL, NULL);
       if (citer)
	  _clip_madd(cs->cw->cmachine, &cv, HASH_TREEITER, &citer->obj);
    }

   if (arg2)
    {
       cpath = _list_get_cobject(cs->cw->cmachine, arg2);
       if (!cpath)
	  cpath = _register_object(cs->cw->cmachine, arg2, GTK_TYPE_TREE_PATH, NULL, NULL);
       if (cpath)
	  _clip_madd(cs->cw->cmachine, &cv, HASH_TREEPATH, &cpath->obj);
    }

   INVOKESIGHANDLER(GTK_WIDGET(tree), cs, cv);
}
#endif

static int
handle_select_cursor_row(GtkTreeView * tree, gboolean arg1, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputl(cs->cw->cmachine, &cv, HASH_ARG1, arg1);
   INVOKESIGHANDLER(GTK_WIDGET(tree), cs, cv);
}

static int
handle_set_scroll_adjustments(GtkTreeView * tree, GtkAdjustment * arg1, GtkAdjustment * arg2, C_signal * cs)
{
   C_widget *carg1;

   C_widget *carg2;

   PREPARECV(cs, cv);

   carg1 = _list_get_cwidget(cs->cw->cmachine, (GtkWidget *) arg1);
   if (!carg1)
      carg1 = _register_widget(cs->cw->cmachine, (GtkWidget *) arg1, NULL);
   if (carg1)
      _clip_madd(cs->cw->cmachine, &cv, HASH_ARG1, &carg1->obj);

   carg2 = _list_get_cwidget(cs->cw->cmachine, (GtkWidget *) arg2);
   if (!carg2)
      carg2 = _register_widget(cs->cw->cmachine, (GtkWidget *) arg2, NULL);
   if (carg2)
      _clip_madd(cs->cw->cmachine, &cv, HASH_ARG2, &carg2->obj);

   INVOKESIGHANDLER(GTK_WIDGET(tree), cs, cv);
}

static int
handle_test_collapse_row(GtkTreeView * tree, C_signal * cs)
{
   PREPARECV(cs, cv);

   INVOKESIGHANDLER(GTK_WIDGET(tree), cs, cv);
}

static int
handle_test_expand_row(GtkTreeView * tree, C_signal * cs)
{
   PREPARECV(cs, cv);

   INVOKESIGHANDLER(GTK_WIDGET(tree), cs, cv);
}

/*
*/
static SignalTable tree_view_signals[] = {
   {"columns-changed", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_COLUMNS_CHANGED_SIGNAL},
   {"cursor-changed", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_CURSOR_CHANGED_SIGNAL},
   {"expand-collapse-cursor-row", GSF(handle_expand_collapse_cursor_row),
    ESF(object_emit_signal), GTK_CURSOR_CHANGED_SIGNAL},
   {"move-cursor", GSF(handle_move_cursor), ESF(object_emit_signal),
    GTK_MOVE_CURSOR_SIGNAL},
#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 4)
   {"row-activated", GSF(handle_row_activated), ESF(object_emit_signal),
    GTK_ROW_ACTIVATED_SIGNAL},
   {"row-collapsed", GSF(handle_row_collapsed), ESF(object_emit_signal),
    GTK_ROW_COLLAPSED_SIGNAL},
   {"row-expanded", GSF(handle_row_expanded), ESF(object_emit_signal),
    GTK_ROW_EXPANDED_SIGNAL},
#endif
   {"select-all", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_SELECT_ALL_SIGNAL},
   {"select-cursor-parent", GSF(widget_signal_handler),
    ESF(object_emit_signal), GTK_SELECT_CURSOR_PARENT_SIGNAL},
   {"select-cursor-row", GSF(handle_select_cursor_row),
    ESF(object_emit_signal), GTK_SELECT_CURSOR_ROW_SIGNAL},
   {"set-scroll-adjustments", GSF(handle_set_scroll_adjustments),
    ESF(object_emit_signal), GTK_SET_SCROLL_ADJUSTMENTS_SIGNAL},
   {"start-interactive-search", GSF(widget_signal_handler),
    ESF(object_emit_signal), GTK_START_INTERACTIVE_SEARCH_SIGNAL},
   {"test-collapse-row", GSF(handle_test_collapse_row),
    ESF(object_emit_signal), GTK_TEST_COLLAPSE_ROW_SIGNAL},
   {"test-expand-row", GSF(handle_test_expand_row), ESF(object_emit_signal),
    GTK_TEST_EXPAND_ROW_SIGNAL},
   {"toggle-cursor-row", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_TOGGLE_CURSOR_ROW_SIGNAL},
   {"unselect-all", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_UNSELECT_ALL_SIGNAL},
   {"", NULL, NULL, 0}
};

/**********************************************************/
CLIP_DLLEXPORT GtkType
_gtk_type_tree_view()
{
   return GTK_TYPE_TREE_VIEW;
}

long
_clip_type_tree_view()
{
   return GTK_WIDGET_TREE_VIEW;
}

const char *
_clip_type_name_tree_view()
{
   return "GTK_OBJECT_TREE_VIEW";
}

/* Register boxes in global table */
int
clip_INIT___TREEVIEW(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_tree_view, _clip_type_name_tree_view,
		    _gtk_type_tree_view, _gtk_type_container, tree_view_signals);
   return 0;
}

int
clip_GTK_TREEVIEWNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   GtkWidget *wid;

   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);

   wid = gtk_tree_view_new();

   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWNEWFROMLISTSTORE(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   C_object *cmodel = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   GtkWidget *wid;

   C_widget *cwid;

   GtkTreeModel *model;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cmodel, GTK_IS_LIST_STORE(cmodel->object));

   model = GTK_TREE_MODEL(GTK_LIST_STORE(cmodel->object));
   wid = gtk_tree_view_new_with_model(model);

   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWNEWFROMTREESTORE(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   C_object *cmodel = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   GtkWidget *wid;

   C_widget *cwid;

   GtkTreeModel *model;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cmodel, GTK_IS_TREE_STORE(cmodel->object));

   model = GTK_TREE_MODEL(GTK_TREE_STORE(cmodel->object));
   wid = gtk_tree_view_new_with_model(model);

   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWNEWWITHMODEL(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   C_object *cmodel = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   GtkWidget *wid;

   C_widget *cwid;

   GtkTreeModel *model;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cmodel, GTK_IS_TREE_MODEL(cmodel->object));

   model = GTK_TREE_MODEL(cmodel->object);
   wid = gtk_tree_view_new_with_model(model);

   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWGETMODEL(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   C_object *cmodel;

   GtkTreeModel *model;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);

   model = gtk_tree_view_get_model(GTK_TREE_VIEW(ctree->widget));

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
clip_GTK_TREEVIEWSETMODEL(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   C_object *cmodel = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cmodel, GTK_IS_TREE_MODEL(cmodel->object));

   gtk_tree_view_set_model(GTK_TREE_VIEW(ctree->widget), GTK_TREE_MODEL(cmodel->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWGETSELECTION(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   GtkTreeSelection *selection;

   C_object *cselection;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);

   selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(ctree->widget));

   if (selection)
    {
       cselection = _list_get_cobject(ClipMachineMemory, selection);
       if (!cselection)
	  cselection = _register_object(ClipMachineMemory, selection, GTK_TYPE_TREE_SELECTION, NULL, NULL);
       if (cselection)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cselection->obj);
    }
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWGETHADJUSTMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   GtkAdjustment *adjust;

   C_object *cadjust;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);

   adjust = gtk_tree_view_get_hadjustment(GTK_TREE_VIEW(ctree->widget));

   if (adjust)
    {
       cadjust = _list_get_cobject(ClipMachineMemory, adjust);
       if (!cadjust)
	  cadjust = _register_object(ClipMachineMemory, adjust, GTK_WIDGET_ADJUSTMENT, NULL, NULL);
       if (cadjust)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cadjust->obj);
    }
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWSETHADJUSTMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   C_object *cadjust = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cadjust, GTK_IS_ADJUSTMENT(cadjust->object));

   gtk_tree_view_set_hadjustment(GTK_TREE_VIEW(ctree->widget), GTK_ADJUSTMENT(cadjust->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWGETVADJUSTMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   GtkAdjustment *adjust;

   C_object *cadjust;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);

   adjust = gtk_tree_view_get_vadjustment(GTK_TREE_VIEW(ctree->widget));

   if (adjust)
    {
       cadjust = _list_get_cobject(ClipMachineMemory, adjust);
       if (!cadjust)
	  cadjust = _register_object(ClipMachineMemory, adjust, GTK_WIDGET_ADJUSTMENT, NULL, NULL);
       if (cadjust)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cadjust->obj);
    }
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWSETVADJUSTMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   C_object *cadjust = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cadjust, GTK_IS_ADJUSTMENT(cadjust->object));

   gtk_tree_view_set_vadjustment(GTK_TREE_VIEW(ctree->widget), GTK_ADJUSTMENT(cadjust->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWGETHEADERSVISIBLE(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   gboolean  visible;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);

   visible = gtk_tree_view_get_headers_visible(GTK_TREE_VIEW(ctree->widget));
   _clip_retl(ClipMachineMemory, visible);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWSETHEADERSVISIBLE(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   gboolean  visible = _clip_parl(ClipMachineMemory, 2);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);

   gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(ctree->widget), visible);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNSAUTOSIZE(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);

   gtk_tree_view_columns_autosize(GTK_TREE_VIEW(ctree->widget));

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWSETHEADERSCLICKABLE(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   gboolean  clickable = _clip_parl(ClipMachineMemory, 2);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_tree_view_set_headers_clickable(GTK_TREE_VIEW(ctree->widget), clickable);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWSETRULESHINT(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   gboolean  hint = _clip_parl(ClipMachineMemory, 2);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);

   gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(ctree->widget), hint);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWGETRULESHINT(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   gboolean  hint;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);

   hint = gtk_tree_view_get_rules_hint(GTK_TREE_VIEW(ctree->widget));

   _clip_retl(ClipMachineMemory, hint);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWAPPENDCOLUMN(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   C_object *ccolumn = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gint      col;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));

   col = gtk_tree_view_append_column(GTK_TREE_VIEW(ctree->widget), GTK_TREE_VIEW_COLUMN(ccolumn->object));
   _clip_retni(ClipMachineMemory, col);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWREMOVECOLUMN(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   C_object *ccolumn = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gint      col;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));

   col = gtk_tree_view_remove_column(GTK_TREE_VIEW(ctree->widget), GTK_TREE_VIEW_COLUMN(ccolumn->object));
   _clip_retni(ClipMachineMemory, col);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWINSERTCOLUMN(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   C_object *ccolumn = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gint      position = _clip_parni(ClipMachineMemory, 3);

   gint      col;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));
   CHECKARG(3, NUMERIC_type_of_ClipVarType);

   col = gtk_tree_view_insert_column(GTK_TREE_VIEW(ctree->widget), GTK_TREE_VIEW_COLUMN(ccolumn->object), position);

   _clip_retni(ClipMachineMemory, col);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWINSERTCOLUMNWITHATTRIBUTES(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   gint      position = _clip_parni(ClipMachineMemory, 2);

   gchar    *title = _clip_parc(ClipMachineMemory, 3);

   C_object *ccell = _fetch_cobject(ClipMachineMemory, _clip_par(ClipMachineMemory, 4));

   gint      i, col, np, j;

   gint      attrn[34];

   gchar    *attrs[34];

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, CHARACTER_type_of_ClipVarType);
   CHECKARG2(4, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccell, GTK_IS_CELL_RENDERER(ccell->object));

   memset(attrn, 0, sizeof(attrn));
   memset(attrs, 0, sizeof(attrs));
   np = _clip_parinfo(ClipMachineMemory, 0);
   for (i = 5, j = 0; i <= np; i += 2, j++)
    {
       CHECKOPT(i, CHARACTER_type_of_ClipVarType);
       CHECKOPT(i + 1, NUMERIC_type_of_ClipVarType);
       attrs[j] = _clip_parc(ClipMachineMemory, i);
       attrn[j] = _clip_parni(ClipMachineMemory, i + 1) - 1;

    }

   LOCALE_TO_UTF(title);
   col =
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW
						(ctree->widget),
						(position <
						 0 ? position : position -
						 1), title,
						GTK_CELL_RENDERER(ccell->object),
						attrs[0], attrn[0], attrs[1],
						attrn[1], attrs[2], attrn[2],
						attrs[3], attrn[3], attrs[4],
						attrn[4], attrs[5], attrn[5],
						attrs[6], attrn[6], attrs[7],
						attrn[7], attrs[8], attrn[8],
						attrs[9], attrn[9],
						attrs[10], attrn[10],
						attrs[11], attrn[11],
						attrs[12], attrn[12],
						attrs[13], attrn[13],
						attrs[14], attrn[14],
						attrs[15], attrn[15],
						attrs[16], attrn[16],
						attrs[17], attrn[17],
						attrs[18], attrn[18],
						attrs[19], attrn[19],
						attrs[20], attrn[20],
						attrs[21], attrn[21],
						attrs[22], attrn[22],
						attrs[23], attrn[23],
						attrs[24], attrn[24],
						attrs[25], attrn[25],
						attrs[26], attrn[26],
						attrs[27], attrn[27],
						attrs[28], attrn[28],
						attrs[29], attrn[29],
						attrs[30], attrn[30],
						attrs[31], attrn[31],
						attrs[32], attrn[32], attrs[33], attrn[33], attrs[34], attrn[34], NULL, NULL);
   FREE_TEXT(title);

   _clip_retni(ClipMachineMemory, col);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWGETCOLUMN(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   gint      position = _clip_parni(ClipMachineMemory, 2);

   C_object *ccolumn;

   GtkTreeViewColumn *column;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   column = gtk_tree_view_get_column(GTK_TREE_VIEW(ctree->widget), (position < 0 ? position : position - 1));

   ccolumn = _list_get_cobject(ClipMachineMemory, column);
   if (!ccolumn)
      ccolumn = _register_object(ClipMachineMemory, column, GTK_TYPE_TREE_VIEW_COLUMN, NULL, NULL);
   if (ccolumn)
      _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &ccolumn->obj);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWMOVECOLUMNAFTER(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   C_object *ccolumn = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_object *cbase_column = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));
   CHECKARG2(3, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(cbase_column->object));

   gtk_tree_view_move_column_after(GTK_TREE_VIEW(ctree->widget),
				   GTK_TREE_VIEW_COLUMN(ccolumn->object), GTK_TREE_VIEW_COLUMN(cbase_column->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWSETEXPANDERCOLUMN(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   C_object *ccolumn = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));

   gtk_tree_view_set_expander_column(GTK_TREE_VIEW(ctree->widget), GTK_TREE_VIEW_COLUMN(ccolumn->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWGETEXPANDERCOLUMN(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   C_object *ccolumn;

   GtkTreeViewColumn *column;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);

   column = gtk_tree_view_get_expander_column(GTK_TREE_VIEW(ctree->widget));

   if (column)
    {
       ccolumn = _list_get_cobject(ClipMachineMemory, column);
       if (!ccolumn)
	  ccolumn = _register_object(ClipMachineMemory, column, GTK_TYPE_TREE_VIEW_COLUMN, NULL, NULL);
       if (ccolumn)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &ccolumn->obj);
    }
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWSETCOLUMNDRAGFUNCTION(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

  //C_object *ccolumn;
  //GtkTreeViewColumn *column;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
  /* not realiazed */
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWSCROLLTOPOINT(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   gint      tree_x = _clip_parni(ClipMachineMemory, 2);

   gint      tree_y = _clip_parni(ClipMachineMemory, 3);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);

   gtk_tree_view_scroll_to_point(GTK_TREE_VIEW(ctree->widget), tree_x, tree_y);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWSCROLLTOCELL(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   C_object *cpath = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_object *ccolumn = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   gboolean  use_align = _clip_parl(ClipMachineMemory, 4);

   gfloat    row_align = _clip_parnd(ClipMachineMemory, 5);

   gfloat    col_align = _clip_parnd(ClipMachineMemory, 6);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT2(3, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));
   CHECKARG(4, LOGICAL_type_of_ClipVarType);
   CHECKOPT(5, NUMERIC_type_of_ClipVarType);
   CHECKOPT(6, NUMERIC_type_of_ClipVarType);

   gtk_tree_view_scroll_to_cell(GTK_TREE_VIEW(ctree->widget),
				GTK_TREE_PATH(cpath->object),
				(ccolumn) ? GTK_TREE_VIEW_COLUMN(ccolumn->object) : NULL, use_align, row_align, col_align);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWSETCURSOR(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   C_object *cpath = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_object *ccolumn = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   gboolean  start_editing = _clip_parl(ClipMachineMemory, 4);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT2(3, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));
   CHECKARG(4, LOGICAL_type_of_ClipVarType);

   gtk_tree_view_set_cursor(GTK_TREE_VIEW(ctree->widget),
			    GTK_TREE_PATH(cpath->object),
			    (ccolumn) ? GTK_TREE_VIEW_COLUMN(ccolumn->object) : NULL, start_editing);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWGETCURSOR(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   C_object *cpath = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_object *ccolumn = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   GtkTreePath *path;

   GtkTreeViewColumn *column;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);

   gtk_tree_view_get_cursor(GTK_TREE_VIEW(ctree->widget), &path, &column);

   if (column)
    {
       ccolumn = _list_get_cobject(ClipMachineMemory, column);
       if (!ccolumn)
	  ccolumn = _register_object(ClipMachineMemory, column, GTK_TYPE_TREE_VIEW_COLUMN, NULL, NULL);
      //if (ccolumn) _clip_mclone(ClipMachineMemory,RETPTR(ClipMachineMemory),&ccolumn->obj);
    }
   if (path)
    {
       cpath = _list_get_cobject(ClipMachineMemory, path);
       if (!cpath)
	  cpath = _register_object(ClipMachineMemory, path, GTK_TYPE_TREE_PATH, NULL, NULL);
      //if (cpath) _clip_mclone(ClipMachineMemory,RETPTR(ClipMachineMemory),&cpath->obj);
    }
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWROWACTIVATED(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   C_object *cpath = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_object *ccolumn = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT2(3, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));

   gtk_tree_view_row_activated(GTK_TREE_VIEW(ctree->widget),
			       GTK_TREE_PATH(cpath->object), GTK_TREE_VIEW_COLUMN(ccolumn->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWEXPANDALL(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);

   gtk_tree_view_expand_all(GTK_TREE_VIEW(ctree->widget));

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLLAPSEALL(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);

   gtk_tree_view_collapse_all(GTK_TREE_VIEW(ctree->widget));

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWEXPANDROW(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   C_object *cpath = _fetch_cobject(ClipMachineMemory, _clip_par(ClipMachineMemory, 2));

   gboolean  all = _clip_parl(ClipMachineMemory, 3);

   gboolean  ret;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
   CHECKARG(2, MAP_type_of_ClipVarType);
   CHECKCOBJ(cpath, GTK_IS_TREE_PATH(cpath));
   CHECKARG(3, LOGICAL_type_of_ClipVarType);

   ret = gtk_tree_view_expand_row(GTK_TREE_VIEW(ctree->widget), GTK_TREE_PATH(cpath->object), all);

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLLAPSEROW(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   C_object *cpath = _fetch_cobject(ClipMachineMemory, _clip_par(ClipMachineMemory, 2));

   gboolean  ret;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
   CHECKARG(2, MAP_type_of_ClipVarType);
   CHECKCOBJ(cpath, GTK_IS_TREE_PATH(cpath));

   ret = gtk_tree_view_collapse_row(GTK_TREE_VIEW(ctree->widget), GTK_TREE_PATH(cpath->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWMAPEXPANDEDROWS(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
  /* not realized */
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWROWEXPANDED(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   C_object *cpath = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gboolean  ret;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);

   ret = gtk_tree_view_row_expanded(GTK_TREE_VIEW(ctree->widget), GTK_TREE_PATH(cpath->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWSETREORDERABLE(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   gboolean  reorderable = _clip_parl(ClipMachineMemory, 2);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_tree_view_set_reorderable(GTK_TREE_VIEW(ctree->widget), reorderable);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWGETREORDERABLE(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   gboolean  reorderable;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);

   reorderable = gtk_tree_view_get_reorderable(GTK_TREE_VIEW(ctree->widget));

   _clip_retl(ClipMachineMemory, reorderable);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWGETPATHATPOS(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   gint      x = _clip_parni(ClipMachineMemory, 2);

   gint      y = _clip_parni(ClipMachineMemory, 3);

   C_object *cpath = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 4));

   C_object *ccolumn = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 5));

   GtkTreePath *path;

   GtkTreeViewColumn *column;

   gint      cell_x, cell_y;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT2(4, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT2(5, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));

   gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(ctree->widget), x, y, &path, &column, &cell_x, &cell_y);

   if (column)
    {
       ccolumn = _list_get_cobject(ClipMachineMemory, column);
       if (!ccolumn)
	  ccolumn = _register_object(ClipMachineMemory, column, GTK_TYPE_TREE_VIEW_COLUMN, NULL, NULL);
      //if (ccolumn) _clip_mclone(ClipMachineMemory,RETPTR(ClipMachineMemory),&ccolumn->obj);
    }
   if (path)
    {
       cpath = _list_get_cobject(ClipMachineMemory, path);
       if (!cpath)
	  cpath = _register_object(ClipMachineMemory, path, GTK_TYPE_TREE_PATH, NULL, NULL);
      //if (cpath) _clip_mclone(ClipMachineMemory,RETPTR(ClipMachineMemory),&cpath->obj);
    }

   _clip_storni(ClipMachineMemory, cell_x, 6, 0);
   _clip_storni(ClipMachineMemory, cell_y, 7, 0);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWGETCELLAREA(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   C_object *cpath = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_object *ccolumn = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   GdkRectangle rect;

   ClipVar  *mrect = RETPTR(ClipMachineMemory);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT2(3, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));

   gtk_tree_view_get_cell_area(GTK_TREE_VIEW(ctree->widget),
			       GTK_TREE_PATH(cpath->object), GTK_TREE_VIEW_COLUMN(ccolumn->object), &rect);

   memset(mrect, 0, sizeof(mrect));
   _clip_map(ClipMachineMemory, mrect);
   _map_put_gdk_rectangle(ClipMachineMemory, mrect, &rect);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWGETBACKGROUNDAREA(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   C_object *cpath = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_object *ccolumn = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   GdkRectangle rect;

   ClipVar  *mrect = RETPTR(ClipMachineMemory);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT2(3, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));

   gtk_tree_view_get_background_area(GTK_TREE_VIEW(ctree->widget),
				     GTK_TREE_PATH(cpath->object), GTK_TREE_VIEW_COLUMN(ccolumn->object), &rect);

   memset(mrect, 0, sizeof(mrect));
   _clip_map(ClipMachineMemory, mrect);
   _map_put_gdk_rectangle(ClipMachineMemory, mrect, &rect);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWGETVISIBLERECT(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   GdkRectangle rect;

   ClipVar  *mrect = RETPTR(ClipMachineMemory);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);

   gtk_tree_view_get_visible_rect(GTK_TREE_VIEW(ctree->widget), &rect);

   memset(mrect, 0, sizeof(mrect));
   _clip_map(ClipMachineMemory, mrect);
   _map_put_gdk_rectangle(ClipMachineMemory, mrect, &rect);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWGETBINWINDOW(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   GdkWindow *win;

   C_object *cwin;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);

   win = gtk_tree_view_get_bin_window(GTK_TREE_VIEW(ctree->widget));

   if (win)
    {
       cwin = _list_get_cobject(ClipMachineMemory, win);
       if (!cwin)
	  cwin = _register_object(ClipMachineMemory, win, GDK_TYPE_WINDOW, NULL, NULL);
       if (cwin)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwin->obj);
    }
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWWIDGETTOTREECOORDS(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   gint      wx = _clip_parni(ClipMachineMemory, 2);

   gint      wy = _clip_parni(ClipMachineMemory, 3);

   gint      tx;

   gint      ty;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);

   gtk_tree_view_widget_to_tree_coords(GTK_TREE_VIEW(ctree->widget), wx, wy, &tx, &ty);

   _clip_storni(ClipMachineMemory, tx, 4, 0);
   _clip_storni(ClipMachineMemory, ty, 5, 0);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWTREETOWIDGETCOORDS(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   gint      tx = _clip_parni(ClipMachineMemory, 2);

   gint      ty = _clip_parni(ClipMachineMemory, 3);

   gint      wx;

   gint      wy;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);

   gtk_tree_view_tree_to_widget_coords(GTK_TREE_VIEW(ctree->widget), tx, ty, &wx, &wy);

   _clip_storni(ClipMachineMemory, wx, 4, 0);
   _clip_storni(ClipMachineMemory, wy, 5, 0);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWENABLEMODELDRAGDEST(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   ClipArrVar *ctag = (ClipArrVar *) _clip_vptr(_clip_spar(ClipMachineMemory, 2));

   gint      ntargets = _clip_parni(ClipMachineMemory, 3);

   GdkDragAction actions = _clip_parni(ClipMachineMemory, 4);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
   CHECKARG(2, ARRAY_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);

   if (ctag)
    {
       GtkTargetEntry *tags;

       gint      i;

       tags = malloc(ctag->count_of_ClipArrVar * sizeof(GtkTargetEntry));
       memset(tags, 0, sizeof(GtkTargetEntry) * ctag->count_of_ClipArrVar);
       for (i = 0; i < ctag->count_of_ClipArrVar; i++)
	  _array_to_target_entry(ClipMachineMemory, &ctag->ClipVar_items_of_ClipArrVar[i], &tags[i]);
       gtk_tree_view_enable_model_drag_dest(GTK_TREE_VIEW(ctree->widget), tags, ntargets, actions);
       free(tags);
    }
   else
      gtk_tree_view_enable_model_drag_dest(GTK_TREE_VIEW(ctree->widget), NULL, ntargets, actions);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWENABLEMODELDRAGSOURCE(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   GdkModifierType start_button_mask = _clip_parni(ClipMachineMemory, 2);

   ClipArrVar *ctag = (ClipArrVar *) _clip_vptr(_clip_spar(ClipMachineMemory, 3));

   gint      ntargets = _clip_parni(ClipMachineMemory, 4);

   GdkDragAction actions = _clip_parni(ClipMachineMemory, 5);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, ARRAY_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);
   CHECKARG(5, NUMERIC_type_of_ClipVarType);

   if (ctag)
    {
       GtkTargetEntry *tags;

       gint      i;

       tags = malloc(ctag->count_of_ClipArrVar * sizeof(GtkTargetEntry));
       memset(tags, 0, sizeof(GtkTargetEntry) * ctag->count_of_ClipArrVar);
       for (i = 0; i < ctag->count_of_ClipArrVar; i++)
	{
	   GtkTargetEntry t;

	   _array_to_target_entry(ClipMachineMemory, &ctag->ClipVar_items_of_ClipArrVar[i], &t);
	   tags[i] = t;
	}
       gtk_tree_view_enable_model_drag_source(GTK_TREE_VIEW(ctree->widget), start_button_mask, tags, ntargets, actions);
       free(tags);
    }
   else
      gtk_tree_view_enable_model_drag_source(GTK_TREE_VIEW(ctree->widget), start_button_mask, NULL, ntargets, actions);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWUNSETROWSDRAGSOURCE(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);

   gtk_tree_view_unset_rows_drag_source(GTK_TREE_VIEW(ctree->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWUNSETROWSDRAGDEST(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);

   gtk_tree_view_unset_rows_drag_dest(GTK_TREE_VIEW(ctree->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWSETDRAGDESTROW(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   C_object *cpath = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gint      cpos = _clip_parni(ClipMachineMemory, 3);

   GtkTreeViewDropPosition pos = GTK_TREE_VIEW_DROP_BEFORE;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);

   switch (cpos)
    {
    case CLIP_GTK_TREE_VIEW_DROP_BEFORE:
       pos = GTK_TREE_VIEW_DROP_BEFORE;
       break;
    case CLIP_GTK_TREE_VIEW_DROP_AFTER:
       pos = GTK_TREE_VIEW_DROP_AFTER;
       break;
    case CLIP_GTK_TREE_VIEW_DROP_INTO_OR_BEFORE:
       pos = GTK_TREE_VIEW_DROP_INTO_OR_BEFORE;
       break;
    case CLIP_GTK_TREE_VIEW_DROP_INTO_OR_AFTER:
       pos = GTK_TREE_VIEW_DROP_INTO_OR_AFTER;
       break;
    }
   gtk_tree_view_set_drag_dest_row(GTK_TREE_VIEW(ctree->widget), GTK_TREE_PATH(cpath->object), pos);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWGETDRAGDESTROW(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   C_object *cpath = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gint      cpos = CLIP_GTK_TREE_VIEW_DROP_BEFORE;

   GtkTreePath *path;

   GtkTreeViewDropPosition pos;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);

   path = GTK_TREE_PATH(cpath->object);
   gtk_tree_view_get_drag_dest_row(GTK_TREE_VIEW(ctree->widget), &path, &pos);
   switch (pos)
    {
    case GTK_TREE_VIEW_DROP_BEFORE:
       cpos = CLIP_GTK_TREE_VIEW_DROP_BEFORE;
       break;
    case GTK_TREE_VIEW_DROP_AFTER:
       cpos = CLIP_GTK_TREE_VIEW_DROP_AFTER;
       break;
    case GTK_TREE_VIEW_DROP_INTO_OR_BEFORE:
       cpos = CLIP_GTK_TREE_VIEW_DROP_INTO_OR_BEFORE;
       break;
    case GTK_TREE_VIEW_DROP_INTO_OR_AFTER:
       cpos = CLIP_GTK_TREE_VIEW_DROP_INTO_OR_AFTER;
       break;
    }

   _clip_retni(ClipMachineMemory, cpos);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWGETDESTROWATPOS(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   gint      drag_x = _clip_parni(ClipMachineMemory, 2);

   gint      drag_y = _clip_parni(ClipMachineMemory, 3);

   GtkTreePath *path;

   GtkTreeViewDropPosition pos;

   gboolean  ret;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);

   ret = gtk_tree_view_get_dest_row_at_pos(GTK_TREE_VIEW(ctree->widget), drag_x, drag_y, &path, &pos);

   _clip_retl(ClipMachineMemory, ret);

   return 0;
 err:
   return 1;
}

/* Creates a GdkPixmap representation of the row at path. This image is */
/* used for a drag icon.                                                */
int
clip_GTK_TREEVIEWCREATEROWDRAGICON(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   C_object *cpath = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   GdkPixmap *pixmap;

   C_object *cpixmap;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);

   pixmap = gtk_tree_view_create_row_drag_icon(GTK_TREE_VIEW(ctree->widget), GTK_TREE_PATH(cpath->object));

   if (pixmap)
    {
       cpixmap = _list_get_cobject(ClipMachineMemory, pixmap);
       if (!cpixmap)
	  cpixmap = _register_object(ClipMachineMemory, pixmap, GDK_TYPE_PIXMAP, NULL, NULL);
       if (cpixmap)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cpixmap->obj);
    }

   return 0;
 err:
   return 1;
}

/* If enable_search is set, then the user can type in text to search through */
/* the tree interactively.                                                   */
int
clip_GTK_TREEVIEWSETENABLESEARCH(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   gboolean  enable_search = _clip_parl(ClipMachineMemory, 2);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);

   gtk_tree_view_set_enable_search(GTK_TREE_VIEW(ctree->widget), enable_search);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWGETENABLESEARCH(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   gboolean  enable_search;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);

   enable_search = gtk_tree_view_get_enable_search(GTK_TREE_VIEW(ctree->widget));

   _clip_retl(ClipMachineMemory, enable_search);
   return 0;
 err:
   return 1;
}

/* Gets the column searched on by the interactive search code. */
int
clip_GTK_TREEVIEWGETSEARCHCOLUMN(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   gint      column;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);

   column = gtk_tree_view_get_search_column(GTK_TREE_VIEW(ctree->widget));

   _clip_retni(ClipMachineMemory, column);
   return 0;
 err:
   return 1;
}

/* Sets column as the column where the interactive search code should */
/* search in. Additionally, turns on interactive searching.           */
int
clip_GTK_TREEVIEWSETSEARCHCOLUMN(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   gint      column = _clip_parni(ClipMachineMemory, 2);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_tree_view_set_search_column(GTK_TREE_VIEW(ctree->widget), column - 1);

   return 0;
 err:
   return 1;
}

#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 2)
int
clip_GTK_TREEVIEWSETCURSORONCELL(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   C_object *cpath = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_object *ccolumn = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   C_object *crender = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 4));

   gboolean  start_edit = _clip_parni(ClipMachineMemory, 5);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
   CHECKCOBJ(cpath, GTK_IS_TREE_PATH(cpath));
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));
   CHECKCOBJOPT(crender, GTK_IS_CELL_RENDERER(crender->object));
   CHECKARG(5, LOGICAL_type_of_ClipVarType);

   gtk_tree_view_set_cursor_on_cell(GTK_TREE_VIEW(ctree->widget),
				    GTK_TREE_PATH(cpath->object),
				    (ccolumn) ?
				    GTK_TREE_VIEW_COLUMN(ccolumn->object) : NULL,
				    (crender) ? GTK_CELL_RENDERER(crender->object) : NULL, start_edit);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWEXPANDTOPATH(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   C_object *cpath = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
   CHECKCOBJ(cpath, GTK_IS_TREE_PATH(cpath));

   gtk_tree_view_expand_to_path(GTK_TREE_VIEW(ctree->widget), GTK_TREE_PATH(cpath->object));

   return 0;
 err:
   return 1;
}
#endif
#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 6)
int
clip_GTK_TREEVIEWGETFIXEDHEIGHTMODE(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);

   _clip_retl(ClipMachineMemory, gtk_tree_view_get_fixed_height_mode(GTK_TREE_VIEW(ctree->widget)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWSETFIXEDHEIGHTMODE(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   gboolean  enable = _clip_parl(ClipMachineMemory, 2);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_tree_view_set_fixed_height_mode(GTK_TREE_VIEW(ctree->widget), enable);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWGETHOVERSELECTION(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);

   _clip_retl(ClipMachineMemory, gtk_tree_view_get_hover_selection(GTK_TREE_VIEW(ctree->widget)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWSETHOVERSELECTION(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   gboolean  enable = _clip_parl(ClipMachineMemory, 2);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_tree_view_set_hover_selection(GTK_TREE_VIEW(ctree->widget), enable);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWGETHOVEREXPAND(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);

   _clip_retl(ClipMachineMemory, gtk_tree_view_get_hover_expand(GTK_TREE_VIEW(ctree->widget)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWSETHOVEREXPAND(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   gboolean  enable = _clip_parl(ClipMachineMemory, 2);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_tree_view_set_hover_expand(GTK_TREE_VIEW(ctree->widget), enable);

   return 0;
 err:
   return 1;
}

/* Get index of selected column in tree view widget */
int
clip_GTK_TREEVIEWGETSELECTEDCOLUMN(ClipMachine * ClipMachineMemory)
{
   C_widget *ctree = _fetch_cw_arg(ClipMachineMemory);

   GList    *cols;

   gint      cIndex;

   GtkTreePath *path;

   GtkTreeViewColumn *column;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ctree, GTK_IS_TREE_VIEW);

  /* get current cursor position */
   gtk_tree_view_get_cursor(GTK_TREE_VIEW(ctree->widget), &path, &column);

  /* return column index */
   cols = gtk_tree_view_get_columns(GTK_TREE_VIEW(ctree->widget));
   cIndex = g_list_index(cols, (gpointer) column);
   g_list_free(cols);

   _clip_retni(ClipMachineMemory, cIndex + 1);
   return 0;
 err:
   return 1;
}
#endif
