/*
    Copyright (C) 2003  ITK
    Author  : Elena V. Kornilova <alena@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include <string.h>
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg2.h"

#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "ci_clip-gtk2.ch"
#include "ci_clip-gtk2.h"

static GtkTreeIter _Iter;

static GtkTreeIter *Iter = &_Iter;

/*********************** SIGNALS **************************/
/*
static gint
handle_tree_model_row_changed (GtkTreeModel *treemodel, GtkTypeTreePath arg1, GtkTypeTreeIter arg2, C_signal *cs)
{
	OBJECTPREPARECV(cs,cv);
	_clip_mputc(cs->co->cmachine, &cv, HASH_PATHSTRING, arg1);
	_clip_mputn(cs->co->cmachine, &cv, HASH_ARG2, arg2);
	OBJECTINVOKESIGHANDLER(cs,cv);
}
*/
/* Signals table */
static SignalTable tree_model_signals[] = {

  /*
     {"row-changed",      GSF( handle_tree_model_row_changed ), ESF( object_emit_signal ), GTK_TREE_MODEL_ROW_CHANGED_SIGNAL},
     {"row-deleted",      GSF(  ), ESF( object_emit_signal ), GTK_TREE_UNSELECT_ROW_SIGNAL},
     {"row-has-child-toggled", GSF( ), ESF( object_emit_signal ), GTK_TREE_EXPAND_SIGNAL},
     {"row-inserted",     GSF( ), ESF( object_emit_signal ), GTK_TREE_COLLAPSE_SIGNAL},
     {"rows-reordered", GSF(  ), ESF( object_emit_signal ), GTK_TREE_MOVE_SIGNAL},
     {"change-focus-row-expansion",       GSF(  ), ESF( object_emit_signal ), GTK_CHANGE_FOCUS_ROW_EXPANSION_SIGNAL},
   */
   {"", NULL, NULL, 0}
};

/**********************************************************/
CLIP_DLLEXPORT GtkType
_gtk_type_tree_model()
{
   return GTK_TYPE_TREE_MODEL;
}

CLIP_DLLEXPORT GtkType
_gtk_type_tree_path()
{
   return GTK_TYPE_TREE_PATH;
}

CLIP_DLLEXPORT GtkType
_gtk_type_tree_iter()
{
   return GTK_TYPE_TREE_ITER;
}

CLIP_DLLEXPORT GtkType
_gtk_type_tree_model_sort()
{
   return GTK_TYPE_TREE_MODEL_SORT;
}

long
_clip_type_tree_model()
{
   return GTK_OBJECT_TREE_MODEL;
}

long
_clip_type_tree_path()
{
   return GTK_OBJECT_TREE_PATH;
}

long
_clip_type_tree_iter()
{
   return GTK_OBJECT_TREE_ITER;
}

long
_clip_type_tree_model_sort()
{
   return GTK_OBJECT_TREE_MODEL_SORT;
}

const char *
_clip_type_name_tree_model()
{
   return "GTK_OBJECT_TREE_MODEL";
}

const char *
_clip_type_name_tree_path()
{
   return "GTK_OBJECT_TREE_PATH";
}

const char *
_clip_type_name_tree_iter()
{
   return "GTK_OBJECT_TREE_ITER";
}

const char *
_clip_type_name_tree_model_sort()
{
   return "GTK_OBJECT_TREE_MODEL_SORT";
}

#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 4)
CLIP_DLLEXPORT GtkType
_gtk_type_tree_model_filter()
{
   return GTK_TYPE_TREE_MODEL_FILTER;
}

long
_clip_type_tree_model_filter()
{
   return GTK_OBJECT_TREE_MODEL_FILTER;
}

const char *
_clip_type_name_tree_model_filter()
{
   return "GTK_OBJECT_TREE_MODEL_FILTER";
}

#endif
/* Register boxes in global table */
int
clip_INIT___TREEMODEL(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_tree_model, _clip_type_name_tree_model, _gtk_type_tree_model, NULL, tree_model_signals);
   _wtype_table_put(_clip_type_tree_path, _clip_type_name_tree_path, _gtk_type_tree_path, NULL, NULL);
   _wtype_table_put(_clip_type_tree_iter, _clip_type_name_tree_iter, _gtk_type_tree_iter, NULL, NULL);
   _wtype_table_put(_clip_type_tree_model_sort, _clip_type_name_tree_model_sort, _gtk_type_tree_model_sort, NULL, NULL);
#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 4)
   _wtype_table_put(_clip_type_tree_model_filter, _clip_type_name_tree_model_filter, _gtk_type_tree_model_filter, NULL, NULL);
#endif
   return 0;
}

int
clip_GTK_TREEMODEL(ClipMachine * ClipMachineMemory)
{
   C_object *ctree = _fetch_co_arg(ClipMachineMemory);

   C_object *cmodel;

   GtkTreeModel *model = 0;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);

   CHECKCOBJ(ctree, GTK_IS_TREE_MODEL(ctree->object));

   if GTK_IS_LIST_STORE
      (ctree->object) model = GTK_TREE_MODEL(GTK_LIST_STORE(ctree->object));
   else if GTK_IS_TREE_STORE
      (ctree->object) model = GTK_TREE_MODEL(GTK_TREE_STORE(ctree->object));

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
clip_GTK_TREEPATHNEW(ClipMachine * ClipMachineMemory)
{
   C_object *ctreepath;

   GtkTreePath *treepath;

   treepath = gtk_tree_path_new();
   if (treepath)
    {
       ctreepath = _list_get_cobject(ClipMachineMemory, treepath);
       if (!ctreepath)
	  ctreepath = _register_object(ClipMachineMemory, treepath, GTK_TYPE_TREE_PATH, NULL, NULL);
       if (ctreepath)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &ctreepath->obj);
    }
   return 0;
}

int
clip_GTK_TREEPATHNEWFROMSTRING(ClipMachine * ClipMachineMemory)
{
   gchar    *str = _clip_parc(ClipMachineMemory, 1);

   C_object *ctreepath;

   GtkTreePath *treepath;

   CHECKARG(1, CHARACTER_type_of_ClipVarType);

   treepath = gtk_tree_path_new_from_string(str);
   if (treepath)
    {
       ctreepath = _list_get_cobject(ClipMachineMemory, treepath);
       if (!ctreepath)
	  ctreepath = _register_object(ClipMachineMemory, treepath, GTK_TYPE_TREE_PATH, NULL, NULL);
       if (ctreepath)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &ctreepath->obj);
    }
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEPATHTOSTRING(ClipMachine * ClipMachineMemory)
{
   C_object *ctreepath = _fetch_co_arg(ClipMachineMemory);

   gchar    *str;

   CHECKARG(1, MAP_type_of_ClipVarType);

   str = gtk_tree_path_to_string(GTK_TREE_PATH(ctreepath->object));

   _clip_retc(ClipMachineMemory, str);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEPATHNEWFIRST(ClipMachine * ClipMachineMemory)
{
   C_object *ctreepath;

   GtkTreePath *treepath;

   treepath = gtk_tree_path_new_first();
   if (treepath)
    {
       ctreepath = _list_get_cobject(ClipMachineMemory, treepath);
       if (!ctreepath)
	  ctreepath = _register_object(ClipMachineMemory, treepath, GTK_TYPE_TREE_PATH, NULL, NULL);
       if (ctreepath)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &ctreepath->obj);
    }
   return 0;
}

int
clip_GTK_TREEPATHAPPENDINDEX(ClipMachine * ClipMachineMemory)
{
   C_object *ctreepath = _fetch_co_arg(ClipMachineMemory);

   gint      index = _clip_parni(ClipMachineMemory, 2);

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_tree_path_append_index(GTK_TREE_PATH(ctreepath->object), index);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEPATHPREPENDINDEX(ClipMachine * ClipMachineMemory)
{
   C_object *ctreepath = _fetch_co_arg(ClipMachineMemory);

   gint      index = _clip_parni(ClipMachineMemory, 2);

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_tree_path_prepend_index(GTK_TREE_PATH(ctreepath->object), index);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEPATHGETDEPTH(ClipMachine * ClipMachineMemory)
{
   C_object *ctreepath = _fetch_co_arg(ClipMachineMemory);

   gint      depth;

   CHECKARG(1, MAP_type_of_ClipVarType);

   depth = gtk_tree_path_get_depth(GTK_TREE_PATH(ctreepath->object));

   _clip_retni(ClipMachineMemory, depth);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEPATHGETINDICES(ClipMachine * ClipMachineMemory)
{
   C_object *ctreepath = _fetch_co_arg(ClipMachineMemory);

   GtkTreePath *path;

   ClipVar  *arr = 0;

   gint     *indices;

   gint      i;

   CHECKARG(1, MAP_type_of_ClipVarType);

   path = GTK_TREE_PATH(ctreepath->object);
   indices = gtk_tree_path_get_indices(path);

   _clip_array(ClipMachineMemory, arr, 0, 0);
   for (i = 0; i < gtk_tree_path_get_depth(path); i++)
    {
       ClipVar  *item;

       item = NEW(ClipVar);
       item->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
       item->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = 0;
       item->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = indices[i];
       _clip_aadd(ClipMachineMemory, arr, item);
       _clip_delete(ClipMachineMemory, item);
    }
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), arr);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEPATHFREE(ClipMachine * ClipMachineMemory)
{
   C_object *ctreepath = _fetch_co_arg(ClipMachineMemory);

   CHECKARG(1, MAP_type_of_ClipVarType);

   gtk_tree_path_free(GTK_TREE_PATH(ctreepath->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEPATHCOPY(ClipMachine * ClipMachineMemory)
{
   C_object *ctreepath = _fetch_co_arg(ClipMachineMemory);

   C_object *ctreepath1;

   GtkTreePath *path;

   CHECKARG(1, MAP_type_of_ClipVarType);

   path = gtk_tree_path_copy(GTK_TREE_PATH(ctreepath->object));
   ctreepath1 = _list_get_cobject(ClipMachineMemory, path);
   if (!ctreepath1)
      ctreepath1 = _register_object(ClipMachineMemory, path, GTK_TYPE_TREE_PATH, NULL, NULL);
   if (ctreepath1)
      _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &ctreepath1->obj);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEPATHCOMPARE(ClipMachine * ClipMachineMemory)
{
   C_object *ctreepath1 = _fetch_co_arg(ClipMachineMemory);

   C_object *ctreepath2 = _fetch_cobject(ClipMachineMemory, _clip_par(ClipMachineMemory, 2));

   gint      ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKARG(2, MAP_type_of_ClipVarType);

   ret = gtk_tree_path_compare(GTK_TREE_PATH(ctreepath1->object), GTK_TREE_PATH(ctreepath2->object));

   _clip_retni(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEPATHNEXT(ClipMachine * ClipMachineMemory)
{
   C_object *ctreepath = _fetch_co_arg(ClipMachineMemory);

   CHECKARG(1, MAP_type_of_ClipVarType);

   gtk_tree_path_next(GTK_TREE_PATH(ctreepath->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEPATHPREV(ClipMachine * ClipMachineMemory)
{
   C_object *ctreepath = _fetch_co_arg(ClipMachineMemory);

   gboolean  ret;

   CHECKARG(1, MAP_type_of_ClipVarType);

   ret = gtk_tree_path_prev(GTK_TREE_PATH(ctreepath->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEPATHUP(ClipMachine * ClipMachineMemory)
{
   C_object *ctreepath = _fetch_co_arg(ClipMachineMemory);

   gboolean  ret;

   CHECKARG(1, MAP_type_of_ClipVarType);

   ret = gtk_tree_path_up(GTK_TREE_PATH(ctreepath->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEPATHDOWN(ClipMachine * ClipMachineMemory)
{
   C_object *ctreepath = _fetch_co_arg(ClipMachineMemory);

   CHECKARG(1, MAP_type_of_ClipVarType);

   gtk_tree_path_down(GTK_TREE_PATH(ctreepath->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEPATHISDESCENDANT(ClipMachine * ClipMachineMemory)
{
   C_object *ctreepath1 = _fetch_co_arg(ClipMachineMemory);

   C_object *ctreepath2 = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gboolean  ret;

   CHECKARG(1, MAP_type_of_ClipVarType);

   ret = gtk_tree_path_is_descendant(GTK_TREE_PATH(ctreepath1->object), GTK_TREE_PATH(ctreepath2->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEITERCOPY(ClipMachine * ClipMachineMemory)
{
   C_object *ctreeiter = _fetch_co_arg(ClipMachineMemory);

   C_object *ctreeiter1;

   CHECKARG(1, MAP_type_of_ClipVarType);

   memset(Iter, 0, sizeof(GtkTreeIter));
   Iter = gtk_tree_iter_copy(ctreeiter->object);

   ctreeiter1 = _list_get_cobject(ClipMachineMemory, Iter);
   if (!ctreeiter1)
      ctreeiter1 = _register_object(ClipMachineMemory, Iter, GTK_TYPE_TREE_ITER, NULL, NULL);
   if (ctreeiter1)
      _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &ctreeiter1->obj);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEITERFREE(ClipMachine * ClipMachineMemory)
{
   C_object *ctreeiter = _fetch_co_arg(ClipMachineMemory);

   CHECKARG(1, MAP_type_of_ClipVarType);

   gtk_tree_iter_free(ctreeiter->object);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEMODELGETFLAGS(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   GtkTreeModelFlags flags;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL(ctreemodel->object));

   flags = gtk_tree_model_get_flags(GTK_TREE_MODEL(ctreemodel->object));

   _clip_retni(ClipMachineMemory, flags);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEMODELGETNCOLUMNS(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   gint      ncolumns;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL(ctreemodel->object));

   ncolumns = gtk_tree_model_get_n_columns(GTK_TREE_MODEL(ctreemodel->object));

   _clip_retni(ClipMachineMemory, ncolumns);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEMODELGETCOLUMNTYPE(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   gint      index = _clip_parni(ClipMachineMemory, 2);

   GType     type;

   gint      t = 0;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL(ctreemodel->object));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   type = gtk_tree_model_get_column_type(GTK_TREE_MODEL(ctreemodel->object), index - 1);

   switch (type)
    {
    case G_TYPE_STRING:
       t = CLIP_GTK_TYPE_STRING;
       break;
    case G_TYPE_BOOLEAN:
       t = CLIP_GTK_TYPE_BOOL;
       break;
    case G_TYPE_FLOAT:
       t = CLIP_GTK_TYPE_FLOAT;
       break;
    case G_TYPE_DOUBLE:
       t = CLIP_GTK_TYPE_DOUBLE;
       break;
    case G_TYPE_INT:
       t = CLIP_GTK_TYPE_INT;
       break;
    case G_TYPE_LONG:
       t = CLIP_GTK_TYPE_LONG;
       break;
    }
   _clip_retni(ClipMachineMemory, t);
   return 0;
 err:
   return 1;
}

/*
int
clip_GTK_TREEMODELGETITER(ClipMachine * ClipMachineMemory)
{
	C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);
	ClipVar *treeiter    =  _clip_spar(ClipMachineMemory, 2);
	//C_object *ctreeiter    = _fetch_cobject(ClipMachineMemory,  _clip_spar(ClipMachineMemory, 2));
	C_object *ctreepath  = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));
	C_object *ctreeiter;
	gboolean ret;

	CHECKARG(1,MAP_type_of_ClipVarType); CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL(ctreemodel->object));
	//CHECKOPT(2, MAP_type_of_ClipVarType);
	CHECKARG(3, MAP_type_of_ClipVarType);

	memset(Iter, 0, sizeof(GtkTreeIter));
	ret = gtk_tree_model_get_iter(GTK_TREE_MODEL(ctreemodel->object),
				      Iter,
				      GTK_TREE_PATH(ctreepath->object));

	ctreeiter = _list_get_cobject(ClipMachineMemory, Iter);
	if (!ctreeiter) ctreeiter = _register_object(ClipMachineMemory, Iter,GTK_TYPE_TREE_ITER,NULL,NULL);
	if (ctreeiter)
	{
	_clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &ctreeiter->obj);
	}
	//if (ctreeiter) _clip_mclone(ClipMachineMemory, treeiter, &ctreeiter->obj);
	//if (ctreeiter) _clip_mclone(ClipMachineMemory, treeiter, ctreeiter->object);
	//_clip_retl(ClipMachineMemory, ret);
	return 0;
err:
	return 1;
}
*/

int
clip_GTK_TREEMODELGETITERFROMSTRING(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   ClipVar  *treeiter = _clip_spar(ClipMachineMemory, 2);

   const gchar *path_string = _clip_parc(ClipMachineMemory, 3);

   C_object *ctreeiter;

   gboolean  ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL(ctreemodel->object));
   CHECKARG(2, MAP_type_of_ClipVarType);
   CHECKARG(3, CHARACTER_type_of_ClipVarType);

   memset(Iter, 0, sizeof(GtkTreeIter));
   ret = gtk_tree_model_get_iter_from_string(GTK_TREE_MODEL(ctreemodel->object), Iter, path_string);

   ctreeiter = _list_get_cobject(ClipMachineMemory, Iter);
   if (!ctreeiter)
      ctreeiter = _register_object(ClipMachineMemory, Iter, GTK_TYPE_TREE_ITER, NULL, NULL);
   if (ctreeiter)
      _clip_mclone(ClipMachineMemory, treeiter, &ctreeiter->obj);

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEMODELGETITERFIRST(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   ClipVar  *treeiter = _clip_spar(ClipMachineMemory, 2);

   C_object *ctreeiter;

   gboolean  ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL(ctreemodel->object));
   CHECKARG(2, MAP_type_of_ClipVarType);

   memset(Iter, 0, sizeof(GtkTreeIter));
   ret = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(ctreemodel->object), Iter);

   ctreeiter = _list_get_cobject(ClipMachineMemory, Iter);
   if (!ctreeiter)
      ctreeiter = _register_object(ClipMachineMemory, Iter, GTK_TYPE_TREE_ITER, NULL, NULL);
   if (ctreeiter)
      _clip_mclone(ClipMachineMemory, treeiter, &ctreeiter->obj);

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEMODELGETITERROOT(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   ClipVar  *treeiter = _clip_spar(ClipMachineMemory, 2);

   C_object *ctreeiter;

   gboolean  ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL(ctreemodel->object));
   CHECKARG(2, MAP_type_of_ClipVarType);
   CHECKARG(3, CHARACTER_type_of_ClipVarType);

   memset(Iter, 0, sizeof(GtkTreeIter));
   ret = gtk_tree_model_get_iter_root(GTK_TREE_MODEL(ctreemodel->object), Iter);

   ctreeiter = _list_get_cobject(ClipMachineMemory, Iter);
   if (!ctreeiter)
      ctreeiter = _register_object(ClipMachineMemory, Iter, GTK_TYPE_TREE_ITER, NULL, NULL);
   if (ctreeiter)
      _clip_mclone(ClipMachineMemory, treeiter, &ctreeiter->obj);

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEMODELGETPATH(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   C_object *ctreeiter = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_object *ctreepath;

   GtkTreePath *path;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL(ctreemodel->object));
   CHECKARG(2, MAP_type_of_ClipVarType);

   path = gtk_tree_model_get_path(GTK_TREE_MODEL(ctreemodel->object), ctreeiter->object);

   ctreepath = _list_get_cobject(ClipMachineMemory, path);
   if (!ctreepath)
      ctreepath = _register_object(ClipMachineMemory, path, GTK_TYPE_TREE_PATH, NULL, NULL);
   if (ctreepath)
      _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &ctreepath->obj);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEMODELGETVALUE(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   C_object *ctreeiter = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gint      column = _clip_parni(ClipMachineMemory, 3);

   GValue    value;

   gchar    *str;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL(ctreemodel->object));
   CHECKARG(2, MAP_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);

   gtk_tree_model_get_value(GTK_TREE_MODEL(ctreemodel->object), ctreeiter->object, column, &value);

   switch (G_VALUE_TYPE(&value))
    {
    case G_TYPE_LONG:
       _clip_retnl(ClipMachineMemory, g_value_get_long(&value));
       break;
    case G_TYPE_FLOAT:
       _clip_retnd(ClipMachineMemory, g_value_get_float(&value));
       break;
    case G_TYPE_STRING:
       str = (gchar *) g_value_get_string(&value);
       LOCALE_FROM_UTF(str);
       _clip_retc(ClipMachineMemory, str);
       FREE_TEXT(str);
       break;
    case G_TYPE_BOOLEAN:
       _clip_retl(ClipMachineMemory, g_value_get_boolean(&value));
       break;
    }

   g_value_unset(&value);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEMODELITERNEXT(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   ClipVar  *treeiter = _clip_spar(ClipMachineMemory, 2);

   C_object *ctreeiter;

   gboolean  ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL(ctreemodel->object));
   CHECKOPT(2, MAP_type_of_ClipVarType);

   memset(Iter, 0, sizeof(GtkTreeIter));
   ret = gtk_tree_model_iter_next(GTK_TREE_MODEL(ctreemodel->object), Iter);

   ctreeiter = _list_get_cobject(ClipMachineMemory, Iter);
   if (!ctreeiter)
      ctreeiter = _register_object(ClipMachineMemory, Iter, GTK_TYPE_TREE_ITER, treeiter, NULL);
   if (ctreeiter)
      _clip_mclone(ClipMachineMemory, treeiter, &ctreeiter->obj);

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEMODELITERCHILDREN(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   ClipVar  *treeiter = _clip_spar(ClipMachineMemory, 2);

   C_object *ctreeparent = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   C_object *ctreeiter;

   gboolean  ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL(ctreemodel->object));
   CHECKOPT(2, MAP_type_of_ClipVarType);
   CHECKOPT(3, MAP_type_of_ClipVarType);

   memset(Iter, 0, sizeof(GtkTreeIter));
   ret = gtk_tree_model_iter_children(GTK_TREE_MODEL(ctreemodel->object), Iter, ctreeparent->object);

   ctreeiter = _list_get_cobject(ClipMachineMemory, Iter);
   if (!ctreeiter)
      ctreeiter = _register_object(ClipMachineMemory, Iter, GTK_TYPE_TREE_ITER, treeiter, NULL);
   if (ctreeiter)
      _clip_mclone(ClipMachineMemory, treeiter, &ctreeiter->obj);

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEMODELITERHASCHILD(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   C_object *ctreeiter = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gboolean  ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL(ctreemodel->object));
   CHECKARG(2, MAP_type_of_ClipVarType);

   ret = gtk_tree_model_iter_has_child(GTK_TREE_MODEL(ctreemodel->object), ctreeiter->object);

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEMODELITERNCHILDREN(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   C_object *ctreeiter = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gint      ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL(ctreemodel->object));
   if (ctreeiter == NULL)
    {
       ret = gtk_tree_model_iter_n_children(GTK_TREE_MODEL(ctreemodel->object), NULL);
    }
   else
    {
       CHECKOPT(2, MAP_type_of_ClipVarType);
       ret = gtk_tree_model_iter_n_children(GTK_TREE_MODEL(ctreemodel->object), ctreeiter->object);
    }
   _clip_retni(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEMODELITERNTHCHILD(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   ClipVar  *treeiter = _clip_spar(ClipMachineMemory, 2);

   C_object *ctreeparent = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   gint      n = _clip_parni(ClipMachineMemory, 4);

   C_object *ctreeiter;

   gboolean  ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL(ctreemodel->object));
   CHECKOPT(2, MAP_type_of_ClipVarType);
   CHECKOPT(3, MAP_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);

   memset(Iter, 0, sizeof(GtkTreeIter));
   ret = gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(ctreemodel->object), Iter, ctreeparent->object, n);

   ctreeiter = _list_get_cobject(ClipMachineMemory, Iter);
   if (!ctreeiter)
      ctreeiter = _register_object(ClipMachineMemory, Iter, GTK_TYPE_TREE_ITER, treeiter, NULL);
   if (ctreeiter)
      _clip_mclone(ClipMachineMemory, treeiter, &ctreeiter->obj);

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEMODELITERPARENT(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   ClipVar  *treeiter = _clip_spar(ClipMachineMemory, 2);

   C_object *ctreechild = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   C_object *ctreeiter;

   gboolean  ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL(ctreemodel->object));
   CHECKOPT(2, MAP_type_of_ClipVarType);
   CHECKOPT(3, MAP_type_of_ClipVarType);

   memset(Iter, 0, sizeof(GtkTreeIter));
   ret = gtk_tree_model_iter_parent(GTK_TREE_MODEL(ctreemodel->object), Iter, ctreechild->object);

   ctreeiter = _list_get_cobject(ClipMachineMemory, Iter);
   if (!ctreeiter)
      ctreeiter = _register_object(ClipMachineMemory, Iter, GTK_TYPE_TREE_ITER, treeiter, NULL);
   if (ctreeiter)
      _clip_mclone(ClipMachineMemory, treeiter, &ctreeiter->obj);

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEMODELREFNODE(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   C_object *ctreeiter = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL(ctreemodel->object));
   CHECKOPT(2, MAP_type_of_ClipVarType);

   gtk_tree_model_ref_node(GTK_TREE_MODEL(ctreemodel->object), ctreeiter->object);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEMODELUNREFNODE(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   C_object *ctreeiter = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL(ctreemodel->object));
   CHECKOPT(2, MAP_type_of_ClipVarType);

   gtk_tree_model_unref_node(GTK_TREE_MODEL(ctreemodel->object), ctreeiter->object);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEMODELGET(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   C_object *ctreeiter = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   GtkTreeIter *iter;

   GType    *columns = 0;

   gint      n = _clip_parinfo(ClipMachineMemory, 0);

   gint      i, x;

   gchar    *str = 0;

   glong     l;

   gfloat    f;

   gboolean  b;

  /* not used
     GdkPixbuf *pix;
     C_object  *cpix;
     ClipVar *vv ;
   */

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL(ctreemodel->object));
   CHECKARG(2, MAP_type_of_ClipVarType);

   if GTK_IS_LIST_STORE
      (ctreemodel->object) columns = GTK_LIST_STORE(ctreemodel->object)->column_headers;
   else if GTK_IS_TREE_STORE
      (ctreemodel->object) columns = GTK_TREE_STORE(ctreemodel->object)->column_headers;

   iter = GTK_TREE_ITER(ctreeiter->object);

   for (i = 3; i <= n; i += 2)
    {
       x = _clip_parni(ClipMachineMemory, i);
       if (x == -1)
	  break;
       x--;
       switch (columns[x])
	{
	case G_TYPE_STRING:
	   gtk_tree_model_get(GTK_TREE_MODEL(ctreemodel->object), iter, x, str, -1);
	   _clip_storc(ClipMachineMemory, str, i + 1, 0);
	   break;
	case G_TYPE_BOOLEAN:
	   gtk_tree_model_get(GTK_TREE_MODEL(ctreemodel->object), iter, x, &b, -1);
	   _clip_storl(ClipMachineMemory, b, i + 1, 0);
	   break;
	case G_TYPE_LONG:
	   gtk_tree_model_get(GTK_TREE_MODEL(ctreemodel->object), iter, x, &l, -1);
	   _clip_stornl(ClipMachineMemory, l, i + 1, 0);
	   break;
	case G_TYPE_FLOAT:
	   gtk_tree_model_get(GTK_TREE_MODEL(ctreemodel->object), iter, x, &f, -1);
	   _clip_stornd(ClipMachineMemory, f, i + 1, 0);
	   break;
	  /*case GDK_TYPE_PIXBUF:
	     vv = _clip_spar(ClipMachineMemory, i+1);
	     gtk_tree_model_get(GTK_TREE_MODEL(ctreemodel->object),
	     iter, x, pix, -1);
	     cpix = _list_get_cobject(ClipMachineMemory, pix);
	     if (!cpix)  cpix = _register_object(ClipMachineMemory, pix, GDK_TYPE_PIXBUF, NULL, NULL);
	     _clip_mclone(ClipMachineMemory, vv, &cpix->obj);
	     break; */
	}
    }

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEMODELGETFROMPATHSTRING(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

  //C_object  *ctreeiter = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   const gchar *path = _clip_parc(ClipMachineMemory, 2);

   GtkTreeIter iter;

   GType    *columns = 0;

   gint      n = _clip_parinfo(ClipMachineMemory, 0);

   gint      i, x;

   gchar    *str;

   glong     l;

   gfloat    f;

   gboolean  b;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL(ctreemodel->object));
  //CHECKARG(2, MAP_type_of_ClipVarType);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   if GTK_IS_LIST_STORE
      (ctreemodel->object) columns = GTK_LIST_STORE(ctreemodel->object)->column_headers;
   else if GTK_IS_TREE_STORE
      (ctreemodel->object) columns = GTK_TREE_STORE(ctreemodel->object)->column_headers;

   gtk_tree_model_get_iter_from_string(GTK_TREE_MODEL(ctreemodel->object), &iter, path);
  //iter = (GtkTreeIter *)(ctreeiter->object);

   for (i = 3; i <= n; i += 2)
    {
       x = _clip_parni(ClipMachineMemory, i);
       if (x == -1)
	  break;
       x--;

       switch (columns[x])
	{
	case G_TYPE_STRING:
	  // printf("try string \n");
	   gtk_tree_model_get(GTK_TREE_MODEL(ctreemodel->object), &iter, x, &str, -1);
	   LOCALE_FROM_UTF(str);
	   if (str != NULL)
	      _clip_storc(ClipMachineMemory, str, i + 1, 0);
	   FREE_TEXT(str);
	   break;
	case G_TYPE_BOOLEAN:
	   gtk_tree_model_get(GTK_TREE_MODEL(ctreemodel->object), &iter, x, &b, -1);
	   _clip_storl(ClipMachineMemory, b, i + 1, 0);
	   break;
	case G_TYPE_LONG:
	   gtk_tree_model_get(GTK_TREE_MODEL(ctreemodel->object), &iter, x, &l, -1);
	   _clip_stornl(ClipMachineMemory, l, i + 1, 0);
	   break;
	case G_TYPE_FLOAT:
	   gtk_tree_model_get(GTK_TREE_MODEL(ctreemodel->object), &iter, x, &f, -1);
	   _clip_stornd(ClipMachineMemory, f, i + 1, 0);
	   break;
	}
    }

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEMODELROWCHANGED(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   C_object *ctreepath = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_object *ctreeiter = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL(ctreemodel->object));
   CHECKARG(2, MAP_type_of_ClipVarType);
   CHECKARG(3, MAP_type_of_ClipVarType);

   gtk_tree_model_row_changed(GTK_TREE_MODEL(ctreemodel->object), ctreepath->object, ctreeiter->object);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEMODELROWINSERTED(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   C_object *ctreepath = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_object *ctreeiter = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL(ctreemodel->object));
   CHECKARG(2, MAP_type_of_ClipVarType);
   CHECKARG(3, MAP_type_of_ClipVarType);

   gtk_tree_model_row_inserted(GTK_TREE_MODEL(ctreemodel->object), ctreepath->object, ctreeiter->object);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEMODELROWHASCHILDTOGGLED(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   C_object *ctreepath = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_object *ctreeiter = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL(ctreemodel->object));
   CHECKARG(2, MAP_type_of_ClipVarType);
   CHECKARG(3, MAP_type_of_ClipVarType);

   gtk_tree_model_row_has_child_toggled(GTK_TREE_MODEL(ctreemodel->object), ctreepath->object, ctreeiter->object);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEMODELROWDELETED(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   C_object *ctreepath = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL(ctreemodel->object));
   CHECKARG(2, MAP_type_of_ClipVarType);

   gtk_tree_model_row_deleted(GTK_TREE_MODEL(ctreemodel->object), ctreepath->object);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEMODELROWSREORDERED(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   C_object *ctreepath = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_object *ctreeiter = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   ClipVar  *cv = RETPTR(ClipMachineMemory);

   gint     *new_order = 0;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL(ctreemodel->object));
   CHECKARG(2, MAP_type_of_ClipVarType);

   gtk_tree_model_rows_reordered(GTK_TREE_MODEL(ctreemodel->object), ctreepath->object, ctreeiter->object, new_order);

   memset(cv, 0, sizeof(*cv));
//        _clip_array(ClipMachineMemory, cv);

   return 0;
 err:
   return 1;
}

#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 4)
int
clip_GTK_TREEMODELFILTERNEW(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   C_object *ctreepath = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_object *cnewmodel;

   GtkTreeModel *newmod;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL(ctreemodel->object));
   CHECKCOBJ(ctreepath, GTK_IS_TREE_PATH(ctreepath->object));

   newmod = gtk_tree_model_filter_new(GTK_TREE_MODEL(ctreemodel->object), GTK_TREE_PATH(ctreepath->object));

   if (newmod)
    {
       cnewmodel = _list_get_cobject(ClipMachineMemory, newmod);
       if (!cnewmodel)
	  cnewmodel = _register_object(ClipMachineMemory, newmod, GTK_TYPE_TREE_MODEL, NULL, NULL);
       if (cnewmodel)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cnewmodel->obj);
    }
   return 0;
 err:
   return 1;
}

static    gboolean
_visible_func(GtkTreeModel * model, GtkTreeIter * iter, gpointer data)
{
   C_var    *c = (C_var *) data;

   C_object *cmodl = _list_get_cobject(c->ClipMachineMemory, model);

   C_object *citer = _list_get_cobject(c->ClipMachineMemory, iter);

   ClipVar   stack[3];

   ClipVar   res;

   gboolean  ret = TRUE;

   if (!cmodl)
      cmodl = _register_object(c->ClipMachineMemory, model, GTK_TYPE_TREE_MODEL, NULL, NULL);
   if (!citer)
      citer = _register_object(c->ClipMachineMemory, iter, GTK_TYPE_TREE_ITER, NULL, NULL);

   memset(&stack, 0, sizeof(stack));
   memset(&res, 0, sizeof(ClipVar));
   _clip_mclone(c->cw->cmachine, &stack[0], &c->co->obj);
   _clip_mclone(c->cw->cmachine, &stack[1], &cmodl->obj);
   _clip_mclone(c->cw->cmachine, &stack[2], &citer->obj);

   if (_clip_eval(c->ClipMachineMemory, &(c->cfunc), 3, stack, &res) == 0)
      ret =
       res.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType ==
       LOGICAL_type_of_ClipVarType ? res.ClipLogVar_l_of_ClipVar.value_of_ClipLogVar : ret;

   _clip_destroy(c->ClipMachineMemory, &res);
   _clip_destroy(c->ClipMachineMemory, &stack[0]);
   _clip_destroy(c->ClipMachineMemory, &stack[1]);
   _clip_destroy(c->ClipMachineMemory, &stack[2]);
   return ret;
}

int
clip_GTK_TREEMODELFILTERSETVISIBLEFUNC(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   ClipVar  *func = _clip_spar(ClipMachineMemory, 2);

   C_var    *c = NEW(C_var);

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL_FILTER(ctreemodel->object));
   CHECKARG2(2, PCODE_type_of_ClipVarType, CCODE_type_of_ClipVarType);

   c->ClipMachineMemory = ClipMachineMemory;
   c->co = ctreemodel;
   _clip_mclone(ClipMachineMemory, &c->cfunc, func);
   gtk_tree_model_filter_set_visible_func(GTK_TREE_MODEL_FILTER
					  (ctreemodel->object), (GtkTreeModelFilterVisibleFunc) _visible_func, c, NULL);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEMODELFILTERSETVISIBLECOLUMN(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   gint      column = _clip_parni(ClipMachineMemory, 2);

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL_FILTER(ctreemodel->object));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   column--;
   gtk_tree_model_filter_set_visible_column(GTK_TREE_MODEL_FILTER(ctreemodel->object), column);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEMODELFILTERGETMODEL(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   C_object *cnewmodel;

   GtkTreeModel *newmod;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL_FILTER(ctreemodel->object));

   newmod = gtk_tree_model_filter_get_model(GTK_TREE_MODEL_FILTER(ctreemodel->object));

   if (newmod)
    {
       cnewmodel = _list_get_cobject(ClipMachineMemory, newmod);
       if (!cnewmodel)
	  cnewmodel = _register_object(ClipMachineMemory, newmod, GTK_TYPE_TREE_MODEL, NULL, NULL);
       if (cnewmodel)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cnewmodel->obj);
    }
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEMODELFILTERCONVERTCHILDITERTOITER(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   C_object *citer;

   C_object *cchild = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL_FILTER(ctreemodel->object));
   CHECKCOBJ(cchild, GTK_IS_TREE_ITER(cchild->object));

   gtk_tree_model_filter_convert_child_iter_to_iter(GTK_TREE_MODEL_FILTER
						    (ctreemodel->object), Iter, GTK_TREE_ITER(cchild->object));

   if (Iter)
    {
       citer = _list_get_cobject(ClipMachineMemory, Iter);
       if (!citer)
	  citer = _register_object(ClipMachineMemory, Iter, GTK_TYPE_TREE_ITER, NULL, NULL);
       if (citer)
	  _clip_mclone(ClipMachineMemory, ARGPTR(ClipMachineMemory, 2), &citer->obj);
    }
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEMODELFILTERCONVERTITERTOCHILDITER(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   C_object *citer;

   C_object *cchild = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL_FILTER(ctreemodel->object));
   CHECKCOBJ(cchild, GTK_IS_TREE_ITER(cchild->object));

   gtk_tree_model_filter_convert_iter_to_child_iter(GTK_TREE_MODEL_FILTER
						    (ctreemodel->object), Iter, GTK_TREE_ITER(cchild->object));

   if (Iter)
    {
       citer = _list_get_cobject(ClipMachineMemory, Iter);
       if (!citer)
	  citer = _register_object(ClipMachineMemory, Iter, GTK_TYPE_TREE_ITER, NULL, NULL);
       if (citer)
	  _clip_mclone(ClipMachineMemory, ARGPTR(ClipMachineMemory, 2), &citer->obj);
    }
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEMODELFILTERCONVERTCHILDPATHTOPATH(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   C_object *cpath = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_object *cnpath;

   GtkTreePath *npath;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL_FILTER(ctreemodel->object));
   CHECKCOBJ(cpath, GTK_IS_TREE_PATH(cpath->object));

   npath =
    gtk_tree_model_filter_convert_child_path_to_path(GTK_TREE_MODEL_FILTER(ctreemodel->object), GTK_TREE_PATH(cpath->object));

   if (npath)
    {
       cnpath = _list_get_cobject(ClipMachineMemory, npath);
       if (!cnpath)
	  cnpath = _register_object(ClipMachineMemory, npath, GTK_TYPE_TREE_PATH, NULL, NULL);
       if (cnpath)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cnpath->obj);
    }
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEMODELFILTERCONVERTPATHTOCHILDPATH(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   C_object *cpath = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_object *cnpath;

   GtkTreePath *npath;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL_FILTER(ctreemodel->object));
   CHECKCOBJ(cpath, GTK_IS_TREE_PATH(cpath->object));

   npath =
    gtk_tree_model_filter_convert_path_to_child_path(GTK_TREE_MODEL_FILTER(ctreemodel->object), GTK_TREE_PATH(cpath->object));

   if (npath)
    {
       cnpath = _list_get_cobject(ClipMachineMemory, npath);
       if (!cnpath)
	  cnpath = _register_object(ClipMachineMemory, npath, GTK_TYPE_TREE_PATH, NULL, NULL);
       if (cnpath)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cnpath->obj);
    }
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEMODELFILTERREFILTER(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL_FILTER(ctreemodel->object));

   gtk_tree_model_filter_refilter(GTK_TREE_MODEL_FILTER(ctreemodel->object));
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEMODELFILTERCLEARCACHE(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL_FILTER(ctreemodel->object));

   gtk_tree_model_filter_clear_cache(GTK_TREE_MODEL_FILTER(ctreemodel->object));
   return 0;
 err:
   return 1;
}
#endif
/******************************************************************************/
/***************************** Gtk tree model SORT ****************************/
/******************************************************************************/

int
clip_GTK_TREEMODELSORNEWWITHMODEL(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   C_object *cnewmodel;

   GtkTreeModel *newmod;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL(ctreemodel->object));

   newmod = gtk_tree_model_sort_new_with_model(GTK_TREE_MODEL(ctreemodel->object));

   if (newmod)
    {
       cnewmodel = _list_get_cobject(ClipMachineMemory, newmod);
       if (!cnewmodel)
	  cnewmodel = _register_object(ClipMachineMemory, newmod, GTK_TYPE_TREE_MODEL, NULL, NULL);
       if (cnewmodel)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cnewmodel->obj);
    }
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEMODELSORTGETMODEL(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   C_object *cnewmodel;

   GtkTreeModel *newmod;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL_SORT(ctreemodel->object));

   newmod = gtk_tree_model_sort_get_model(GTK_TREE_MODEL_SORT(ctreemodel->object));

   if (newmod)
    {
       cnewmodel = _list_get_cobject(ClipMachineMemory, newmod);
       if (!cnewmodel)
	  cnewmodel = _register_object(ClipMachineMemory, newmod, GTK_TYPE_TREE_MODEL, NULL, NULL);
       if (cnewmodel)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cnewmodel->obj);
    }
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEMODELSORTCONVERTCHILDPATHTOPATH(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   C_object *cpath = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_object *cnpath;

   GtkTreePath *npath;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL_SORT(ctreemodel->object));
   CHECKCOBJ(cpath, GTK_IS_TREE_PATH(cpath->object));

   npath =
    gtk_tree_model_sort_convert_child_path_to_path(GTK_TREE_MODEL_SORT(ctreemodel->object), GTK_TREE_PATH(cpath->object));

   if (npath)
    {
       cnpath = _list_get_cobject(ClipMachineMemory, npath);
       if (!cnpath)
	  cnpath = _register_object(ClipMachineMemory, npath, GTK_TYPE_TREE_PATH, NULL, NULL);
       if (cnpath)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cnpath->obj);
    }
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEMODELSORTCONVERTPATHTOCHILDPATH(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   C_object *cpath = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_object *cnpath;

   GtkTreePath *npath;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL_SORT(ctreemodel->object));
   CHECKCOBJ(cpath, GTK_IS_TREE_PATH(cpath->object));

   npath =
    gtk_tree_model_sort_convert_path_to_child_path(GTK_TREE_MODEL_SORT(ctreemodel->object), GTK_TREE_PATH(cpath->object));

   if (npath)
    {
       cnpath = _list_get_cobject(ClipMachineMemory, npath);
       if (!cnpath)
	  cnpath = _register_object(ClipMachineMemory, npath, GTK_TYPE_TREE_PATH, NULL, NULL);
       if (cnpath)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cnpath->obj);
    }
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEMODELSORTCONVERTCHILDITERTOITER(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   C_object *citer;

   C_object *cchild = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL_SORT(ctreemodel->object));
   CHECKCOBJ(cchild, GTK_IS_TREE_ITER(cchild->object));

   gtk_tree_model_sort_convert_child_iter_to_iter(GTK_TREE_MODEL_SORT(ctreemodel->object), Iter, GTK_TREE_ITER(cchild->object));

   if (Iter)
    {
       citer = _list_get_cobject(ClipMachineMemory, Iter);
       if (!citer)
	  citer = _register_object(ClipMachineMemory, Iter, GTK_TYPE_TREE_ITER, NULL, NULL);
       if (citer)
	  _clip_mclone(ClipMachineMemory, ARGPTR(ClipMachineMemory, 2), &citer->obj);
    }
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEMODELSORTCONVERTITERTOCHILDITER(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   C_object *citer;

   C_object *cchild = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL_SORT(ctreemodel->object));
   CHECKCOBJ(cchild, GTK_IS_TREE_ITER(cchild->object));

   gtk_tree_model_sort_convert_iter_to_child_iter(GTK_TREE_MODEL_SORT(ctreemodel->object), Iter, GTK_TREE_ITER(cchild->object));

   if (Iter)
    {
       citer = _list_get_cobject(ClipMachineMemory, Iter);
       if (!citer)
	  citer = _register_object(ClipMachineMemory, Iter, GTK_TYPE_TREE_ITER, NULL, NULL);
       if (citer)
	  _clip_mclone(ClipMachineMemory, ARGPTR(ClipMachineMemory, 2), &citer->obj);
    }
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEMODELSORTRESETDEFAULTSORTFUNC(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL_SORT(ctreemodel->object));

   gtk_tree_model_sort_reset_default_sort_func(GTK_TREE_MODEL_SORT(ctreemodel->object));
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEMODELSORTCLEARCACHE(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL_SORT(ctreemodel->object));

   gtk_tree_model_sort_clear_cache(GTK_TREE_MODEL_SORT(ctreemodel->object));
   return 0;
 err:
   return 1;
}

#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 2)
int
clip_GTK_TREEMODELSORTITERISVALID(ClipMachine * ClipMachineMemory)
{
   C_object *ctreemodel = _fetch_co_arg(ClipMachineMemory);

   C_object *citer = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctreemodel, GTK_IS_TREE_MODEL_SORT(ctreemodel->object));
   CHECKCOBJ(citer, GTK_IS_TREE_ITER(citer->object));

   gtk_tree_model_sort_iter_is_valid(GTK_TREE_MODEL_SORT(ctreemodel->object), GTK_TREE_ITER(citer->object));

   return 0;
 err:
   return 1;
}
#endif

static    gboolean
_foreach_func(GtkTreeModel * model, GtkTreePath * path, GtkTreeIter * iter, gpointer data)
{
   C_var    *c = (C_var *) data;

   C_object *cmodl = _list_get_cobject(c->ClipMachineMemory, model);

   C_object *cpath = _list_get_cobject(c->ClipMachineMemory, path);

   C_object *citer = _list_get_cobject(c->ClipMachineMemory, iter);

   ClipVar   stack[3];

   ClipVar   res;

   gboolean  ret = TRUE;

   if (!cmodl)
      cmodl = _register_object(c->ClipMachineMemory, model, GTK_TYPE_TREE_MODEL, NULL, NULL);
   if (!cpath)
      cpath = _register_object(c->ClipMachineMemory, path, GTK_TYPE_TREE_PATH, NULL, NULL);
   if (!citer)
      citer = _register_object(c->ClipMachineMemory, iter, GTK_TYPE_TREE_ITER, NULL, NULL);

   memset(&stack, 0, sizeof(stack));
   memset(&res, 0, sizeof(ClipVar));
   _clip_mclone(c->cw->cmachine, &stack[0], &cmodl->obj);
   _clip_mclone(c->cw->cmachine, &stack[1], &cpath->obj);
   _clip_mclone(c->cw->cmachine, &stack[2], &citer->obj);

   if (_clip_eval(c->ClipMachineMemory, &(c->cfunc), 3, stack, &res) == 0)
      ret =
       res.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType ==
       LOGICAL_type_of_ClipVarType ? res.ClipLogVar_l_of_ClipVar.value_of_ClipLogVar : ret;

   _clip_destroy(c->ClipMachineMemory, &res);
   _clip_destroy(c->ClipMachineMemory, &stack[0]);
   _clip_destroy(c->ClipMachineMemory, &stack[1]);
   _clip_destroy(c->ClipMachineMemory, &stack[2]);
   return ret;
}

int
clip_GTK_TREEMODELFOREACH(ClipMachine * ClipMachineMemory)
{
   C_widget *ctreemodel = _fetch_cw_arg(ClipMachineMemory);

   ClipVar  *func = _clip_spar(ClipMachineMemory, 2);

   C_var    *c = NEW(C_var);

   CHECKCWID(ctreemodel, GTK_IS_TREE_MODEL);
   CHECKARG2(2, PCODE_type_of_ClipVarType, CCODE_type_of_ClipVarType);

   c->ClipMachineMemory = ClipMachineMemory;
   c->cw = ctreemodel;
   _clip_mclone(ClipMachineMemory, &c->cfunc, func);

   gtk_tree_model_foreach(GTK_TREE_MODEL(ctreemodel->widget), (GtkTreeModelForeachFunc) _foreach_func, c);

   return 0;
 err:
   return 1;
}
