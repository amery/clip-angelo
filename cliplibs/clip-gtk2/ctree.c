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
gint
handle_tree_select_row_signal(GtkWidget * widget, GtkCTreeNode * node, gint column, C_signal * cs)
{
   C_object *cnode = (C_object *) _list_get_cobject(cs->cw->cmachine, node);
   PREPARECV(cs, cv);
   if (!cnode)
      cnode = _register_object(cs->cw->cmachine, node, GTK_TYPE_CTREE_NODE, NULL, NULL);
   if (cnode)
      _clip_madd(cs->cw->cmachine, &cv, HASH_NODE, &cnode->obj);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_COLUMN, column + 1);
   INVOKESIGHANDLER(widget, cs, cv);
}

gint
handle_tree_unselect_row_signal(GtkWidget * widget, GtkCTreeNode * node, gint column, C_signal * cs)
{
   C_object *cnode = (C_object *) _list_get_cobject(cs->cw->cmachine, node);
   PREPARECV(cs, cv);
   if (!cnode)
      cnode = _register_object(cs->cw->cmachine, node, GTK_TYPE_CTREE_NODE, NULL, NULL);
   if (cnode)
      _clip_madd(cs->cw->cmachine, &cv, HASH_NODE, &cnode->obj);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_COLUMN, column + 1);
   INVOKESIGHANDLER(widget, cs, cv);
}

gint
handle_tree_expand_signal(GtkWidget * widget, GtkCTreeNode * node, C_signal * cs)
{
   C_object *cnode = (C_object *) _list_get_cobject(cs->cw->cmachine, node);
   PREPARECV(cs, cv);
   if (!cnode)
      cnode = _register_object(cs->cw->cmachine, node, GTK_TYPE_CTREE_NODE, NULL, NULL);
   if (cnode)
      _clip_madd(cs->cw->cmachine, &cv, HASH_NODE, &cnode->obj);
   INVOKESIGHANDLER(widget, cs, cv);
}

gint
handle_tree_collapse_signal(GtkWidget * widget, GtkCTreeNode * node, C_signal * cs)
{
   C_object *cnode = (C_object *) _list_get_cobject(cs->cw->cmachine, node);
   PREPARECV(cs, cv);
   if (!cnode)
      cnode = _register_object(cs->cw->cmachine, node, GTK_TYPE_CTREE_NODE, NULL, NULL);
   if (cnode)
      _clip_madd(cs->cw->cmachine, &cv, HASH_NODE, &cnode->obj);
   INVOKESIGHANDLER(widget, cs, cv);
}

gint
handle_tree_move_signal(GtkWidget * widget, GtkCTreeNode * node, GtkCTreeNode * new_parent, GtkCTreeNode * new_sibling, C_signal * cs)
{
   C_object *cnode = (C_object *) _list_get_cobject(cs->cw->cmachine, node);
   C_object *cnewparent = (C_object *) _list_get_cobject(cs->cw->cmachine, new_parent);
   C_object *cnewsibling = (C_object *) _list_get_cobject(cs->cw->cmachine, new_sibling);
   PREPARECV(cs, cv);
   if (!cnode)
      cnode = _register_object(cs->cw->cmachine, node, GTK_TYPE_CTREE_NODE, NULL, NULL);
   if (!cnewparent)
      cnode = _register_object(cs->cw->cmachine, new_parent, GTK_TYPE_CTREE_NODE, NULL, NULL);
   if (!cnewsibling)
      cnode = _register_object(cs->cw->cmachine, new_sibling, GTK_TYPE_CTREE_NODE, NULL, NULL);
   if (cnode)
      _clip_madd(cs->cw->cmachine, &cv, HASH_NODE, &cnode->obj);
   if (cnewparent)
      _clip_madd(cs->cw->cmachine, &cv, HASH_NEWPARENT, &cnewparent->obj);
   if (cnewsibling)
      _clip_madd(cs->cw->cmachine, &cv, HASH_NEWSIBLING, &cnewsibling->obj);
   INVOKESIGHANDLER(widget, cs, cv);
}

gint
handle_change_focus_row_expansion_signal(GtkWidget * widget, GtkCTreeExpansionType action, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_ACTION, action);
   INVOKESIGHANDLER(widget, cs, cv);
}

/* Signals table */
static SignalTable ctree_signals[] = {
   {"tree-select-row", GSF(handle_tree_select_row_signal),
    ESF(object_emit_signal), GTK_TREE_SELECT_ROW_SIGNAL},
   {"tree-unselect-row", GSF(handle_tree_unselect_row_signal),
    ESF(object_emit_signal), GTK_TREE_UNSELECT_ROW_SIGNAL},
   {"tree-expand", GSF(handle_tree_expand_signal), ESF(object_emit_signal),
    GTK_TREE_EXPAND_SIGNAL},
   {"tree-collapse", GSF(handle_tree_collapse_signal),
    ESF(object_emit_signal), GTK_TREE_COLLAPSE_SIGNAL},
   {"tree-move", GSF(handle_tree_move_signal), ESF(object_emit_signal),
    GTK_TREE_MOVE_SIGNAL},
   {"change-focus-row-expansion",
    GSF(handle_change_focus_row_expansion_signal), ESF(object_emit_signal),
    GTK_CHANGE_FOCUS_ROW_EXPANSION_SIGNAL},
   {"", NULL, NULL, 0}
};

/**********************************************************/

/* Register CTree in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_ctree()
{
   return GTK_TYPE_CTREE;
}

long
_clip_type_ctree()
{
   return GTK_WIDGET_CTREE;
}

const char *
_clip_type_name_ctree()
{
   return "GTK_WIDGET_CTREE";
}

int
clip_INIT___CTREE(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_ctree, _clip_type_name_ctree, _gtk_type_ctree, _gtk_type_clist, ctree_signals);
   return 0;
}

/**** CTree constructor ****/
int
clip_GTK_CTREENEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   gint ncolumns = _clip_parni(ClipMachineMemory, 2);
   gint tree_column = _clip_parni(ClipMachineMemory, 3);
   ClipVar *cvcol = _clip_spar(ClipMachineMemory, 4);
   ClipArrVar *acol;
   int i;
   gchar *empty_string = "\0";
   gchar **columns = NULL;
   GtkWidget *wid = NULL;
   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT2(4, ARRAY_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      ncolumns = 1;
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      tree_column = 1;
   switch (cvcol->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
      {
      case CHARACTER_type_of_ClipVarType:
	 columns = (gchar **) calloc(sizeof(columns), ncolumns);
#ifdef OS_CYGWIN
	 columns[0] = _clip_locale_to_utf8(cvcol->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
#else
	 columns[0] = cvcol->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
#endif
	 for (i = 1; i < ncolumns; i++)
	    columns[i] = empty_string;
	 wid = gtk_ctree_new_with_titles(ncolumns, tree_column - 1, columns);
#ifdef OS_CYGWIN
	 g_free(columns[0]);
#endif
	 if (columns)
	    free(columns);
	 break;
      case ARRAY_type_of_ClipVarType:
	 acol = (ClipArrVar *) _clip_vptr(cvcol);
	 columns = (gchar **) calloc(sizeof(columns), ncolumns);
#ifdef OS_CYGWIN
	 for (i = 0; i < ncolumns; i++)
	    {
	       if (i < acol->count && acol->items[i].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
		  columns[i] = _clip_locale_to_utf8(acol->items[i].ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
	       else
		  columns[i] = _clip_locale_to_utf8(empty_string);
	    }
	 wid = gtk_ctree_new_with_titles(ncolumns, tree_column - 1, columns);
	 for (i = 0; i < ncolumns; i++)
	    {
	       g_free(columns[i]);
	    }
	 if (columns)
	    free(columns);
#else
	 for (i = 0; i < ncolumns; i++)
	    {
	       if (i < acol->count_of_ClipArrVar && acol->ClipVar_items_of_ClipArrVar[i].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
		  columns[i] = acol->ClipVar_items_of_ClipArrVar[i].ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
	       else
		  columns[i] = empty_string;
	    }
	 wid = gtk_ctree_new_with_titles(ncolumns, tree_column - 1, columns);
	 if (columns)
	    free(columns);
#endif
      default:
	 wid = gtk_ctree_new(ncolumns, tree_column - 1);
      }

   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

/* Insert a new node to the tree. The position is specified through the
 * parent-sibling notation, as explained in the introduction above. */
int
clip_GTK_CTREEINSERTNODE(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cparent = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   C_object *csibling = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));
   ClipVar *cvtext = _clip_spar(ClipMachineMemory, 4);
   guint8 spacing = _clip_parni(ClipMachineMemory, 5);
   C_widget *cclosed = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 6));
   C_widget *copened = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 7));
   gboolean is_leaf = _clip_parl(ClipMachineMemory, 8);
   gboolean expanded = _clip_parl(ClipMachineMemory, 9);
   GtkCTreeNode *new_node, *parent = NULL, *sibling = NULL;
   GdkPixmap *pxm_closed = NULL, *pxm_opened = NULL;
   GdkBitmap *mask_closed = NULL, *mask_opened = NULL;
   C_object *cnew_node;
   int i;
   gchar **columns = NULL;
   int ncolumns;

   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cparent, cparent->type == GTK_TYPE_CTREE_NODE);
   CHECKOPT2(3, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(csibling, csibling->type == GTK_TYPE_CTREE_NODE);
   CHECKOPT2(4, ARRAY_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   CHECKOPT(5, NUMERIC_type_of_ClipVarType);
   CHECKOPT2(6, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cclosed, GTK_IS_PIXMAP);
   CHECKOPT2(7, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(copened, GTK_IS_PIXMAP);
   CHECKOPT(8, LOGICAL_type_of_ClipVarType);
   CHECKOPT(9, LOGICAL_type_of_ClipVarType);

   if (cparent)
      parent = GTK_CTREE_NODE(cparent->object);
   if (csibling)
      sibling = GTK_CTREE_NODE(csibling->object);
   if (cclosed)
      {
	 pxm_closed = GTK_PIXMAP(cclosed->widget)->pixmap;
	 mask_closed = GTK_PIXMAP(cclosed->widget)->mask;
      }
   if (copened)
      {
	 pxm_opened = GTK_PIXMAP(copened->widget)->pixmap;
	 mask_opened = GTK_PIXMAP(copened->widget)->mask;
      }
   if (_clip_parinfo(ClipMachineMemory, 8) == UNDEF_type_of_ClipVarType)
      is_leaf = TRUE;
   if (_clip_parinfo(ClipMachineMemory, 9) == UNDEF_type_of_ClipVarType)
      expanded = FALSE;

   ncolumns = GTK_CLIST(cctree->widget)->columns;
   columns = (gchar **) calloc(sizeof(columns), ncolumns);
   for (i = 0; i < ncolumns; i++)
      columns[i] = "";
   if (cvtext->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
      {
	 ClipArrVar *acol = (ClipArrVar *) _clip_vptr(cvtext);
	 for (i = 0; i < acol->count_of_ClipArrVar; i++)
	    if (i < acol->count_of_ClipArrVar && acol->ClipVar_items_of_ClipArrVar[i].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
	       {
		  columns[i] = acol->ClipVar_items_of_ClipArrVar[i].ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
		  LOCALE_TO_UTF(columns[i]);
	       }
      }
   if (cvtext->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
      {
	 columns[0] = _clip_parc(ClipMachineMemory, 2);
	 LOCALE_TO_UTF(columns[0]);
      }

   new_node = gtk_ctree_insert_node(GTK_CTREE(cctree->widget), parent, sibling, columns, spacing, pxm_closed, mask_closed, pxm_opened, mask_opened, is_leaf, expanded);
   if (new_node)
      {
	 cnew_node = _register_object(ClipMachineMemory, new_node, GTK_TYPE_CTREE_NODE, NULL, NULL);
	 if (cnew_node)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cnew_node->obj);
	 gtk_ctree_node_set_row_data_full(GTK_CTREE(cctree->widget), new_node, cnew_node, (GtkDestroyNotify) destroy_c_object);
      }

#ifdef OS_CYGWIN
   if (cvtext->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
      {
	 ClipArrVar *acol = (ClipArrVar *) _clip_vptr(cvtext);
	 for (i = 0; i < acol->count; i++)
	    if (i < acol->count && acol->items[i].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
	       FREE_TEXT(columns[i]);
      }
   if (cvtext->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
      FREE_TEXT(columns[0]);
#endif
   if (columns)
      free(columns);
   return 0;
 err:
   return 1;
}

/* Remove the node and all nodes underneath it from the tree. */
int
clip_GTK_CTREEREMOVENODE(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   gtk_ctree_remove_node(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object));
   return 0;
 err:
   return 1;
}

typedef struct Ctree_var
{
   ClipMachine *ClipMachineMemory;
   ClipVar *cv;
   C_widget *cw;
   ClipVar *cfunc;
}
Ctree_var;

void
_ctree_func(GtkCTree * ctree, GtkCTreeNode * node, Ctree_var * c)
{
   C_object *cnode = _list_get_cobject(c->ClipMachineMemory, node);
   ClipVar stack[2];
   ClipVar res;
   if (!cnode)
      cnode = _register_object(c->ClipMachineMemory, node, GTK_TYPE_CTREE_NODE, NULL, NULL);
   memset(&stack, 0, sizeof(stack));
   memset(&res, 0, sizeof(ClipVar));
   _clip_mclone(c->ClipMachineMemory, &stack[0], &c->cw->obj);
   _clip_mclone(c->ClipMachineMemory, &stack[1], &cnode->obj);
  //stack[0] = c->cw->obj;
  //stack[1] = cnode->obj;
   _clip_eval(c->ClipMachineMemory, c->cfunc, 2, stack, &res);
   _clip_destroy(c->ClipMachineMemory, &res);
   _clip_destroy(c->ClipMachineMemory, &stack[0]);
   _clip_destroy(c->ClipMachineMemory, &stack[1]);
}

/* Recursively apply a function to all nodes of the tree at or below a
 * certain node. The function is called for each node after it has been
 * called for that node's children. */
int
clip_GTK_CTREEPOSTRECURSIVE(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   ClipVar *cfunc = _clip_spar(ClipMachineMemory, 3);
   Ctree_var c;
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   CHECKARG2(3, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);
   c.ClipMachineMemory = ClipMachineMemory;
   c.cv = cfunc;
   c.cw = cctree;
   c.cfunc = cfunc;
   gtk_ctree_post_recursive(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object), (GtkCTreeFunc) _ctree_func, &c);
   return 0;
 err:
   return 1;
}

/* Recursively apply a function to nodes up to a certain depth.
 * The function is called for each node after it has been called
 * for that node's children. */
int
clip_GTK_CTREEPOSTRECURSIVETODEPTH(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   gint depth = _clip_parni(ClipMachineMemory, 3);
   ClipVar *cfunc = _clip_spar(ClipMachineMemory, 4);
   Ctree_var c;
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKARG2(4, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      depth = -1;
   c.ClipMachineMemory = ClipMachineMemory;
   c.cv = cfunc;
   c.cw = cctree;
   c.cfunc = cfunc;
   gtk_ctree_post_recursive_to_depth(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object), depth, (GtkCTreeFunc) _ctree_func, &c);
   return 0;
 err:
   return 1;
}

/* Recursively apply a function to all nodes of the tree at or below a
 * certain node. The function is called for each node after it has
 * been called for its parent. */
int
clip_GTK_CTREEPRERECURSIVE(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   ClipVar *cfunc = _clip_spar(ClipMachineMemory, 3);
   Ctree_var c;
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   CHECKARG2(3, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);
   c.ClipMachineMemory = ClipMachineMemory;
   c.cv = cfunc;
   c.cw = cctree;
   c.cfunc = cfunc;
   gtk_ctree_pre_recursive(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object), (GtkCTreeFunc) _ctree_func, &c);
   return 0;
 err:
   return 1;
}

/* Recursively apply a function to nodes up to a certain depth. The function
 * is called for each node after it has been called for that node's children. */
int
clip_GTK_CTREEPRERECURSIVETODEPTH(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   gint depth = _clip_parni(ClipMachineMemory, 3);
   ClipVar *cfunc = _clip_spar(ClipMachineMemory, 4);
   Ctree_var c;
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKARG2(4, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      depth = -1;
   c.ClipMachineMemory = ClipMachineMemory;
   c.cv = cfunc;
   c.cw = cctree;
   c.cfunc = cfunc;
   gtk_ctree_pre_recursive_to_depth(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object), depth, (GtkCTreeFunc) _ctree_func, &c);
   return 0;
 err:
   return 1;
}

/* This function checks whether the given node is viewable i.e. so that all
 * of its parent nodes are expanded. This is different from being actually
 * visible: the node can be viewable but outside the scrolling area of the
 * window. */
int
clip_GTK_CTREEISVIEWABLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   _clip_retl(ClipMachineMemory, gtk_ctree_is_viewable(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object)));
   return 0;
 err:
   return 1;
}

/* Returns the last child of the last child of the last child... of the given node. */
int
clip_GTK_CTREELAST(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   GtkCTreeNode *last;
   C_object *clast;
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   last = gtk_ctree_last(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object));
   if (last)
      {
	 clast = _list_get_cobject(ClipMachineMemory, last);
	 if (!clast)
	    clast = _register_object(ClipMachineMemory, last, GTK_TYPE_CTREE_NODE, NULL, NULL);
	 if (clast)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &clast->obj);
      }
   return 0;
 err:
   return 1;
}

/* Returns True if child is on some level a child (grandchild...) of the node. */
int
clip_GTK_CTREEFIND(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   C_object *cchild = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   CHECKOPT2(3, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cchild, cchild->type == GTK_TYPE_CTREE_NODE);
   _clip_retl(ClipMachineMemory, gtk_ctree_find(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object), GTK_CTREE_NODE(cchild->object)));
   return 0;
 err:
   return 1;
}

/* Returns True is node is an ancestor of child.  */
int
clip_GTK_CTREEISANCESTOR(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   C_object *cchild = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   CHECKOPT2(3, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cchild, cchild->type == GTK_TYPE_CTREE_NODE);
   _clip_retl(ClipMachineMemory, gtk_ctree_is_ancestor(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object), GTK_CTREE_NODE(cchild->object)));
   return 0;
 err:
   return 1;
}

/* Returns True if the given coordinates lie on an expander button.  */
int
clip_GTK_CTREEISHOTSPOT(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   gint x = _clip_parni(ClipMachineMemory, 2);
   gint y = _clip_parni(ClipMachineMemory, 3);
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   _clip_retl(ClipMachineMemory, gtk_ctree_is_hot_spot(GTK_CTREE(cctree->widget), x, y));
   return 0;
 err:
   return 1;
}

/* Move a node in the tree to another location. */
int
clip_GTK_CTREEMOVE(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   C_object *cnew_parent = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));
   C_object *cnew_sibling = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 4));
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   CHECKOPT2(3, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnew_parent, cnew_parent->type == GTK_TYPE_CTREE_NODE);
   CHECKOPT2(4, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnew_sibling, cnew_sibling->type == GTK_TYPE_CTREE_NODE);
   gtk_ctree_move(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object), GTK_CTREE_NODE(cnew_parent->object), GTK_CTREE_NODE(cnew_sibling->object));
   return 0;
 err:
   return 1;
}

/* Expand one node. */
int
clip_GTK_CTREEEXPAND(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   gtk_ctree_expand(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object));
   return 0;
 err:
   return 1;
}

/* Expand one node and all nodes underneath. */
int
clip_GTK_CTREEEXPANDRECURSIVE(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   gtk_ctree_expand_recursive(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object));
   return 0;
 err:
   return 1;
}

/* Expand a node and its children up to the depth given. */
int
clip_GTK_CTREEEXPANDTODEPTH(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   gint depth = _clip_parni(ClipMachineMemory, 3);
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      depth = -1;
   gtk_ctree_expand_to_depth(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object), depth);
   return 0;
 err:
   return 1;
}

/* Collapse one node. */
int
clip_GTK_CTREECOLLAPSE(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   gtk_ctree_collapse(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object));
   return 0;
 err:
   return 1;
}

/* Collapse one node and all its subnodes. */
int
clip_GTK_CTREECOLLAPSERECURSIVE(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   gtk_ctree_collapse_recursive(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object));
   return 0;
 err:
   return 1;
}

/* Collapse a node and its children up to the depth given. */
int
clip_GTK_CTREECOLLAPSETODEPTH(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   gint depth = _clip_parni(ClipMachineMemory, 3);
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      depth = -1;
   gtk_ctree_collapse_to_depth(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object), depth);
   return 0;
 err:
   return 1;
}

/* Toggle a node, i.e. if it is collapsed, expand it and vice versa. */
int
clip_GTK_CTREETOGGLEEXPANSION(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   gtk_ctree_toggle_expansion(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object));
   return 0;
 err:
   return 1;
}

/* Toggle the expansion of a node and all its children. */
int
clip_GTK_CTREETOGGLEEXPANSIONRECURSIVE(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   gtk_ctree_toggle_expansion_recursive(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object));
   return 0;
 err:
   return 1;
}

/* Cause the given node to be selected and emit the appropriate signal. */
int
clip_GTK_CTREESELECT(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   gtk_ctree_select(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object));
   return 0;
 err:
   return 1;
}

/* Cause the given node and its subnodes to be selected and emit the
   appropriate signal(s). */
int
clip_GTK_CTREESELECTRECURSIVE(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   gtk_ctree_select_recursive(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object));
   return 0;
 err:
   return 1;
}

/* Unselect the given node and emit the appropriate signal. */
int
clip_GTK_CTREEUNSELECT(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   gtk_ctree_unselect(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object));
   return 0;
 err:
   return 1;
}

/* Unselect the given node and its subnodes and emit the appropriate
   signal(s). */
int
clip_GTK_CTREEUNSELECTRECURSIVE(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   gtk_ctree_unselect_recursive(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object));
   return 0;
 err:
   return 1;
}

/* Set the text in a node. */
int
clip_GTK_CTREENODESETTEXT(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   gint column = _clip_parni(ClipMachineMemory, 3);
   gchar *text = _clip_parc(ClipMachineMemory, 4);
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, CHARACTER_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      column = 1;
   if (_clip_parinfo(ClipMachineMemory, 4) == UNDEF_type_of_ClipVarType)
      text = "";
   LOCALE_TO_UTF(text);
   gtk_ctree_node_set_text(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object), column - 1, text);
   FREE_TEXT(text);
   return 0;
 err:
   return 1;
}

/* Set the pixmap in a node. */
int
clip_GTK_CTREENODESETPIXMAP(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   gint column = _clip_parni(ClipMachineMemory, 3);
   C_widget *cpixmap = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 4));
   GdkPixmap *pixmap = NULL;
   GdkBitmap *mask = NULL;
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT2(4, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cpixmap, GTK_IS_PIXMAP);
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      column = 1;
   if (cpixmap)
      {
	 pixmap = GTK_PIXMAP(cpixmap->widget)->pixmap;
	 mask = GTK_PIXMAP(cpixmap->widget)->mask;
      }
   gtk_ctree_node_set_pixmap(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object), column - 1, pixmap, mask);
   return 0;
 err:
   return 1;
}

/* Set the pixmap in a node. */
int
clip_GTK_CTREENODESETPIXTEXT(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   gint column = _clip_parni(ClipMachineMemory, 3);
   gchar *text = _clip_parc(ClipMachineMemory, 4);
   gint spacing = _clip_parni(ClipMachineMemory, 5);
   C_widget *cpixmap = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 6));
   GdkPixmap *pixmap = NULL;
   GdkBitmap *mask = NULL;
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, CHARACTER_type_of_ClipVarType);
   CHECKOPT(5, NUMERIC_type_of_ClipVarType);
   CHECKOPT2(6, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cpixmap, GTK_IS_PIXMAP);
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      column = 1;
   if (_clip_parinfo(ClipMachineMemory, 4) == UNDEF_type_of_ClipVarType)
      text = "";
   if (cpixmap)
      {
	 pixmap = GTK_PIXMAP(cpixmap->widget)->pixmap;
	 mask = GTK_PIXMAP(cpixmap->widget)->mask;
      }
   LOCALE_TO_UTF(text);
   gtk_ctree_node_set_pixtext(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object), column - 1, text, spacing, pixmap, mask);
   FREE_TEXT(text);
   return 0;
 err:
   return 1;
}

/* Change the information. Most parameters correspond to the parameters
   of gtk_ctree_insert_node. */
int
clip_GTK_CTREENODESETNODEINFO(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   ClipVar *cvtext = _clip_spar(ClipMachineMemory, 3);
   guint8 spacing = _clip_parni(ClipMachineMemory, 4);
   C_widget *cclosed = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 5));
   C_widget *copened = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 6));
   gboolean is_leaf = _clip_parl(ClipMachineMemory, 7);
   gboolean expanded = _clip_parl(ClipMachineMemory, 8);
   GtkCTreeNode *node = 0;
   GdkPixmap *pxm_closed = NULL, *pxm_opened = NULL;
   GdkBitmap *mask_closed = NULL, *mask_opened = NULL;
   int i;
   gchar **columns = NULL;
   int ncolumns;

   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   CHECKOPT2(3, ARRAY_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   CHECKOPT(4, NUMERIC_type_of_ClipVarType);
   CHECKOPT2(5, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cclosed, GTK_IS_PIXMAP);
   CHECKOPT2(6, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(copened, GTK_IS_PIXMAP);
   CHECKOPT(7, LOGICAL_type_of_ClipVarType);
   CHECKOPT(8, LOGICAL_type_of_ClipVarType);

   if (cnode)
      node = GTK_CTREE_NODE(cnode->object);
   if (cclosed)
      {
	 pxm_closed = GTK_PIXMAP(cclosed->widget)->pixmap;
	 mask_closed = GTK_PIXMAP(cclosed->widget)->mask;
      }
   if (copened)
      {
	 pxm_opened = GTK_PIXMAP(copened->widget)->pixmap;
	 mask_opened = GTK_PIXMAP(copened->widget)->mask;
      }
   if (_clip_parinfo(ClipMachineMemory, 7) == UNDEF_type_of_ClipVarType)
      is_leaf = TRUE;
   if (_clip_parinfo(ClipMachineMemory, 8) == UNDEF_type_of_ClipVarType)
      expanded = FALSE;

   ncolumns = GTK_CLIST(cctree->widget)->columns;
   columns = (gchar **) calloc(sizeof(columns), ncolumns);
   for (i = 0; i < ncolumns; i++)
      columns[i] = "";
   if (cvtext->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
      {
	 ClipArrVar *acol = (ClipArrVar *) _clip_vptr(cvtext);
	 for (i = 0; i < acol->count_of_ClipArrVar; i++)
	    if (i < acol->count_of_ClipArrVar && acol->ClipVar_items_of_ClipArrVar[i].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
	       {
		  columns[i] = acol->ClipVar_items_of_ClipArrVar[i].ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
		  LOCALE_TO_UTF(columns[i]);
	       }
      }
   if (cvtext->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
      {
	 columns[0] = _clip_parc(ClipMachineMemory, 2);
	 LOCALE_TO_UTF(columns[0]);
      }
   gtk_ctree_set_node_info(GTK_CTREE(cctree->widget), node, columns[0], spacing, pxm_closed, mask_closed, pxm_opened, mask_opened, is_leaf, expanded);
#ifdef OS_CYGWIN
   if (cvtext->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
      {
	 ClipArrVar *acol = (ClipArrVar *) _clip_vptr(cvtext);
	 for (i = 0; i < acol->count; i++)
	    if (i < acol->count && acol->items[i].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
	       FREE_TEXT(columns[i]);
      }
   if (cvtext->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
      FREE_TEXT(columns[0]);
#endif
   if (columns)
      free(columns);
   return 0;
 err:
   return 1;
}

/* Shift the given cell the given amounts in pixels. */
int
clip_GTK_CTREENODESETSHIFT(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   gint column = _clip_parni(ClipMachineMemory, 3);
   gint vertical = _clip_parni(ClipMachineMemory, 4);
   gint horizontal = _clip_parni(ClipMachineMemory, 5);
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, NUMERIC_type_of_ClipVarType);
   CHECKOPT(5, NUMERIC_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      column = 1;
   gtk_ctree_node_set_shift(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object), column - 1, vertical, horizontal);
   return 0;
 err:
   return 1;
}

/* selectable : Whether this node can be selected by the user. */
int
clip_GTK_CTREENODESETSELECTABLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   gboolean selectable = _clip_parl(ClipMachineMemory, 3);
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   CHECKOPT(3, LOGICAL_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      selectable = TRUE;
   gtk_ctree_node_set_selectable(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object), selectable);
   return 0;
 err:
   return 1;
}

/* Returns : Whether this node can be selected by the user.  */
int
clip_GTK_CTREENODEGETSELECTABLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   _clip_retl(ClipMachineMemory, gtk_ctree_node_get_selectable(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object)));
   return 0;
 err:
   return 1;
}

/* Returns : The type of the given cell. */
int
clip_GTK_CTREENODEGETCELLTYPE(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   gint column = _clip_parni(ClipMachineMemory, 3);
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      column = 1;
   _clip_retni(ClipMachineMemory, gtk_ctree_node_get_cell_type(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object), column - 1));
   return 0;
 err:
   return 1;
}

/*  */
int
clip_GTK_CTREENODEGETTEXT(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   gint column = _clip_parni(ClipMachineMemory, 3);
   gint nColumns;
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   nColumns = GTK_CLIST(cctree->widget)->columns;
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      column = 1;
   if (column == 1)
      {
	 gchar **text = calloc(nColumns, sizeof(gchar *));

	 if (gtk_ctree_get_node_info(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object), text, 0, 0, 0, 0, 0, 0, 0))
	    LOCALE_TO_UTF(*text);
	 _clip_retc(ClipMachineMemory, text[column - 1]);
	 FREE_TEXT(*text);
	 free(text);
      }
   else
      {
	 gchar *text;
	 gtk_ctree_node_get_text(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object), column - 1, &text);
	 LOCALE_FROM_UTF(text);
	 if (text)
	    _clip_retc(ClipMachineMemory, text);
	 FREE_TEXT(text);
      }
   return 0;
 err:
   return 1;
}

int
clip_GTK_CTREENODEGETPIXMAP(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   gint column = _clip_parni(ClipMachineMemory, 3);
   GdkPixmap *pixmap;
   GdkBitmap *mask;
   C_widget *cpixmap;
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      column = 1;
   if (gtk_ctree_node_get_pixmap(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object), column - 1, &pixmap, &mask))
      {
	 if (pixmap)
	    {
	       cpixmap = _list_get_cwidget_by_data(ClipMachineMemory, pixmap);
	       if (!cpixmap)
		  cpixmap = _register_widget(ClipMachineMemory, gtk_pixmap_new(pixmap, mask), NULL);
	       if (cpixmap)
		  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cpixmap->obj);
	    }
      }
   return 0;
 err:
   return 1;
}

/* Get the parameters of a cell containing both a pixmap and text. */
int
clip_GTK_CTREENODEGETPIXTEXT(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   gint column = _clip_parni(ClipMachineMemory, 3);
   gchar *text = NULL;
   guint8 spacing;
   GdkPixmap *pixmap;
   GdkBitmap *mask;
   C_widget *cpixmap;
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      column = 1;
   if (gtk_ctree_node_get_pixtext(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object), column - 1, &text, &spacing, &pixmap, &mask))
      {
	 if (pixmap)
	    {
	       cpixmap = _list_get_cwidget_by_data(ClipMachineMemory, pixmap);
	       if (!cpixmap)
		  cpixmap = _register_widget(ClipMachineMemory, gtk_pixmap_new(pixmap, mask), NULL);
	       if (cpixmap)
		  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cpixmap->obj);
	    }
      }

   if (text)
      {
	 LOCALE_FROM_UTF(text);
	 _clip_storc(ClipMachineMemory, text, 4, 0);
	 FREE_TEXT(text);
      }
   else
      _clip_storc(ClipMachineMemory, "", 4, 0);

   _clip_storni(ClipMachineMemory, spacing, 5, 0);
   return 0;
 err:
   return 1;
}

/* Get information corresponding to a node. Any of the return parameters can be null. */
int
clip_GTK_CTREENODEGETNODEINFO(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   ClipVar *minfo = RETPTR(ClipMachineMemory);
   guint8 spacing;
   C_widget *cclosed;
   C_widget *copened;
   gboolean is_leaf;
   gboolean expanded;
   GdkPixmap *pxm_closed = NULL, *pxm_opened = NULL;
   GdkBitmap *mask_closed = NULL, *mask_opened = NULL;
   long i;
   gchar **text;
   long ncolumns;

   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);

   ncolumns = GTK_CLIST(cctree->widget)->columns;
   text = calloc(ncolumns, sizeof(gchar *));
   if (gtk_ctree_get_node_info(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object), text, &spacing, &pxm_closed, &mask_closed, &pxm_opened, &mask_opened, &is_leaf, &expanded))
      {
	 ClipVar *a = NEW(ClipVar);
	 ClipVar cv;
	 cv.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;

	 _clip_array(ClipMachineMemory, a, 1, &ncolumns);

	 for (i = 0; i < ncolumns; i++)
	    {
	       gtk_ctree_node_get_text(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object), i, text);
#ifdef OS_CYGWIN
	       cv.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = _clip_locale_from_utf8(text[0]);
	       cv.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = strlen(cv.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
#else
	       cv.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = strlen(text[0]);
	       cv.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = (gchar *) calloc(1, cv.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf + 1);
	       strcpy(cv.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, text[0]);
#endif
	       _clip_aset(ClipMachineMemory, a, &cv, 1, &i);
	       _clip_destroy(ClipMachineMemory, &cv);
	    }

	 memset(minfo, 0, sizeof(*minfo));
	 _clip_map(ClipMachineMemory, minfo);

	 if (pxm_closed)
	    {
	       cclosed = _list_get_cwidget_by_data(ClipMachineMemory, pxm_closed);
	       if (!cclosed)
		  cclosed = _register_widget(ClipMachineMemory, gtk_pixmap_new(pxm_closed, mask_closed), NULL);
	       if (cclosed)
		  _clip_madd(ClipMachineMemory, minfo, HASH_PIXMAPCLOSED, &cclosed->obj);
	    }

	 if (pxm_opened)
	    {
	       copened = _list_get_cwidget_by_data(ClipMachineMemory, pxm_opened);
	       if (!copened)
		  copened = _register_widget(ClipMachineMemory, gtk_pixmap_new(pxm_opened, mask_opened), NULL);
	       if (copened)
		  _clip_madd(ClipMachineMemory, minfo, HASH_PIXMAPOPENED, &copened->obj);
	    }

	 _clip_madd(ClipMachineMemory, minfo, HASH_TEXT, a);

	 _clip_mputn(ClipMachineMemory, minfo, HASH_SPACING, spacing);
	 _clip_mputl(ClipMachineMemory, minfo, HASH_ISLEAF, is_leaf);
	 _clip_mputl(ClipMachineMemory, minfo, HASH_EXPANDED, expanded);
	 free(a);
      }
   if (text)
      free(text);

   return 0;
 err:
   return 1;
}

/* Set the style of a row. */
int
clip_GTK_CTREENODESETROWSTYLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   ClipVar *mstyle = _clip_spar(ClipMachineMemory, 3);
   GtkStyle *style;
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   CHECKARG(3, MAP_type_of_ClipVarType);
   style = gtk_ctree_node_get_row_style(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object));
   _map_to_style(ClipMachineMemory, mstyle, style);
   gtk_ctree_node_set_row_style(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object), style);
   return 0;
 err:
   return 1;
}

/* Get the style of a row. */
int
clip_GTK_CTREENODEGETROWSTYLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   ClipVar *mstyle = RETPTR(ClipMachineMemory);
   GtkStyle *style;
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   style = gtk_ctree_node_get_row_style(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object));
   memset(mstyle, 0, sizeof(*mstyle));
   _clip_map(ClipMachineMemory, mstyle);
   _style_to_map(ClipMachineMemory, style, mstyle);
   return 0;
 err:
   return 1;
}

/* Set the style of an individual cell. */
int
clip_GTK_CTREENODESETCELLSTYLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   gint column = _clip_parni(ClipMachineMemory, 3);
   ClipVar *mstyle = _clip_spar(ClipMachineMemory, 4);
   GtkStyle *style;
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, MAP_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      column = 1;
   style = gtk_ctree_node_get_cell_style(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object), column - 1);
   _map_to_style(ClipMachineMemory, mstyle, style);
   gtk_ctree_node_set_cell_style(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object), column - 1, style);
   return 0;
 err:
   return 1;
}

/* Get the style of an individual cell. */
int
clip_GTK_CTREENODEGETCELLSTYLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   gint column = _clip_parni(ClipMachineMemory, 3);
   ClipVar *mstyle = RETPTR(ClipMachineMemory);
   GtkStyle *style;
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      column = 1;
   style = gtk_ctree_node_get_cell_style(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object), column - 1);
   memset(mstyle, 0, sizeof(*mstyle));
   _clip_map(ClipMachineMemory, mstyle);
   _style_to_map(ClipMachineMemory, style, mstyle);
   return 0;
 err:
   return 1;
}

/* Sets the foreground color of a node */
int
clip_GTK_CTREENODESETFOREGROUND(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   ClipVar *mcolor = _clip_spar(ClipMachineMemory, 3);
   GdkColor color;
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   CHECKOPT(3, MAP_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 3) == MAP_type_of_ClipVarType)
      _map_colors_to_gdk(ClipMachineMemory, mcolor, &color);
   else
      color = cctree->widget->style->text[0];
   gtk_ctree_node_set_foreground(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object), &color);
   return 0;
 err:
   return 1;
}

/* Sets the background color of a node */
int
clip_GTK_CTREENODESETBACKGROUND(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   ClipVar *mcolor = _clip_spar(ClipMachineMemory, 3);
   GdkColor color;
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   CHECKOPT(3, MAP_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 3) == MAP_type_of_ClipVarType)
      _map_colors_to_gdk(ClipMachineMemory, mcolor, &color);
   else
      color = cctree->widget->style->bg[0];
   gtk_ctree_node_set_background(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object), &color);
   return 0;
 err:
   return 1;
}

/* This function makes the given column of the given node visible by scrolling. */
int
clip_GTK_CTREENODEMOVETO(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   gint column = _clip_parni(ClipMachineMemory, 3);
   gfloat row_align = _clip_parnd(ClipMachineMemory, 4);
   gfloat col_align = _clip_parnd(ClipMachineMemory, 5);
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, NUMERIC_type_of_ClipVarType);
   CHECKOPT(5, NUMERIC_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      column = 1;
   gtk_ctree_node_moveto(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object), column - 1, row_align, col_align);
   return 0;
 err:
   return 1;
}

/* Returns : True if the node is currently inside the bounds of the window.
 * Note that this function can return true even if the node is not viewable,
 * if the node's ancestor is visible.  */
int
clip_GTK_CTREENODEISVISIBLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   _clip_retni(ClipMachineMemory, gtk_ctree_node_is_visible(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object)));
   return 0;
 err:
   return 1;
}

/* indent : The number of pixels to shift the levels of the tree. */
int
clip_GTK_CTREESETINDENT(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   gint indent = _clip_parni(ClipMachineMemory, 2);
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_ctree_set_indent(GTK_CTREE(cctree->widget), indent);
   return 0;
 err:
   return 1;
}

/* The spacing between the tree graphic and the actual node content. */
int
clip_GTK_CTREESETSPACING(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   gint spacing = _clip_parni(ClipMachineMemory, 2);
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_ctree_set_spacing(GTK_CTREE(cctree->widget), spacing);
   return 0;
 err:
   return 1;
}

int
clip_GTK_CTREESETREORDERABLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   gint spacing = _clip_parni(ClipMachineMemory, 2);
   CHECKCWID(cctree, GTK_IS_CLIST);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_ctree_set_reorderable(GTK_CLIST(cctree->widget), spacing);
   return 0;
 err:
   return 1;
}

int
clip_GTK_CTREESETLINESTYLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   gint line_style = _clip_parni(ClipMachineMemory, 2);
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_ctree_set_line_style(GTK_CTREE(cctree->widget), line_style);
   return 0;
 err:
   return 1;
}

int
clip_GTK_CTREESETEXPANDERSTYLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   gint expander_style = _clip_parni(ClipMachineMemory, 2);
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_ctree_set_expander_style(GTK_CTREE(cctree->widget), expander_style);
   return 0;
 err:
   return 1;
}

/* Sort the children of a node. See GtkCList for how to set the sorting criteria etc. */
int
clip_GTK_CTREESORTNODE(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   gtk_ctree_sort_node(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object));
   return 0;
 err:
   return 1;
}

/* Sort the descendants of a node. See GtkCList for how to set the sorting criteria etc. */
int
clip_GTK_CTREESORTRECURSIVE(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   C_object *cnode = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cnode, cnode->type == GTK_TYPE_CTREE_NODE);
   gtk_ctree_sort_recursive(GTK_CTREE(cctree->widget), GTK_CTREE_NODE(cnode->object));
   return 0;
 err:
   return 1;
}

/* Returns : The node corresponding to the row th row. */
int
clip_GTK_CTREENODENTH(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   guint row = _clip_parni(ClipMachineMemory, 2);
   C_object *cnode;
   GtkCTreeNode *node;
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      row = 1;
   node = gtk_ctree_node_nth(GTK_CTREE(cctree->widget), row - 1);
   if (node)
      {
	 cnode = _list_get_cobject(ClipMachineMemory, node);
	 if (!cnode)
	    cnode = _register_object(ClipMachineMemory, node, GTK_TYPE_CTREE_NODE, NULL, NULL);
	 if (cnode)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cnode->obj);
      }
   return 0;
 err:
   return 1;
}

int
clip_GTK_CTREESETSHOWSTUB(ClipMachine * ClipMachineMemory)
{
   C_widget *cctree = _fetch_cw_arg(ClipMachineMemory);
   gboolean show_stub = _clip_parl(ClipMachineMemory, 2);
   CHECKCWID(cctree, GTK_IS_CTREE);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      show_stub = 1;
   gtk_ctree_set_show_stub(GTK_CTREE(cctree->widget), show_stub);
   return 0;
 err:
   return 1;
}
