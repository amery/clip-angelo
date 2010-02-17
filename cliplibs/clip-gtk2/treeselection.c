/*
    Copyright (C) 2003-2004  ITK
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

static GtkTreeIter _Iter;
static GtkTreeIter *Iter = &_Iter;

/*********************** SIGNALS **************************/
/* Signals table */
static SignalTable tree_selection_signals[] = {
   {"changed", GSF(object_signal_handler), ESF(object_emit_signal),
    GTK_CHANGED_SIGNAL},
   {"", NULL, NULL, 0}
};

/**********************************************************/
CLIP_DLLEXPORT GtkType
_gtk_type_tree_selection()
{
   return GTK_TYPE_TREE_SELECTION;
}

long
_clip_type_tree_selection()
{
   return GTK_OBJECT_TREE_SELECTION;
}

const char *
_clip_type_name_tree_selection()
{
   return "GTK_OBJECT_TREE_SELECTION";
}

/* Register boxes in global table */
int
clip_INIT___TREESELECTION(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_tree_selection, _clip_type_name_tree_selection, _gtk_type_tree_selection, NULL, tree_selection_signals);
   return 0;
}

int
clip_GTK_TREESELECTIONSETMODE(ClipMachine * ClipMachineMemory)
{
   C_object *ctreesel = _fetch_co_arg(ClipMachineMemory);
   GtkSelectionMode mode = _clip_parni(ClipMachineMemory, 2);

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(ctreesel, GTK_IS_TREE_SELECTION(ctreesel->object));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_tree_selection_set_mode(GTK_TREE_SELECTION(ctreesel->object), mode);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREESELECTIONGETMODE(ClipMachine * ClipMachineMemory)
{
   C_object *ctreesel = _fetch_co_arg(ClipMachineMemory);
   GtkSelectionMode mode;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(ctreesel, GTK_IS_TREE_SELECTION(ctreesel->object));

   mode = gtk_tree_selection_get_mode(GTK_TREE_SELECTION(ctreesel->object));

   _clip_retni(ClipMachineMemory, mode);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREESELECTIONGETTREEVIEW(ClipMachine * ClipMachineMemory)
{
   C_object *ctreesel = _fetch_co_arg(ClipMachineMemory);
   GtkTreeView *view;
   C_widget *cwid;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(ctreesel, GTK_IS_TREE_SELECTION(ctreesel->object));

   view = gtk_tree_selection_get_tree_view(GTK_TREE_SELECTION(ctreesel->object));

   if (!view)
      goto err;
   cwid = _register_widget(ClipMachineMemory, GTK_WIDGET(view), NULL);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREESELECTIONGETSELECTED(ClipMachine * ClipMachineMemory)
{
   C_object *ctreesel = _fetch_co_arg(ClipMachineMemory);
   C_object *cmodel = _fetch_cobject(ClipMachineMemory, _clip_par(ClipMachineMemory, 2));
   ClipVar *cviter = _clip_par(ClipMachineMemory, 3);
   C_object *citer;
   GtkTreeModel *model;
   gboolean ret;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(ctreesel, GTK_IS_TREE_SELECTION(ctreesel->object));

   memset(Iter, 0, sizeof(Iter));
   if (cmodel)
      {
	 model = GTK_TREE_MODEL(cmodel->object);
	 ret = gtk_tree_selection_get_selected(GTK_TREE_SELECTION(ctreesel->object), &model, Iter);
      }
   else
      ret = gtk_tree_selection_get_selected(GTK_TREE_SELECTION(ctreesel->object), NULL, Iter);

   if (ret && Iter)
      {
	 citer = _list_get_cobject(ClipMachineMemory, Iter);
	 if (!citer)
	    citer = _register_object(ClipMachineMemory, Iter, GTK_TYPE_TREE_ITER, cviter, NULL);
	 if (citer)
	    _clip_mclone(ClipMachineMemory, cviter, &citer->obj);
      }

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 2)

int
clip_GTK_TREESELECTIONGETSELECTEDROWS(ClipMachine * ClipMachineMemory)
{
   C_object *ctreesel = _fetch_co_arg(ClipMachineMemory);
   ClipVar *mod = ARGPTR(ClipMachineMemory, 2);
   GtkTreeModel *model;
   C_object *cmodel;
   GList *list;
   long l;
   ClipVar *ret = RETPTR(ClipMachineMemory);

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(ctreesel, GTK_IS_TREE_SELECTION(ctreesel->object));

   list = gtk_tree_selection_get_selected_rows(GTK_TREE_SELECTION(ctreesel->object), &model);

   l = g_list_length(list);
   _clip_array(ClipMachineMemory, ret, 1, &l);
   for (l = 0; list; list = g_list_next(list), l++)
      {
	 C_object *cpath;
	 GtkTreePath *path;

	 path = GTK_TREE_PATH(list->data);
	 cpath = _list_get_cobject(ClipMachineMemory, path);
	 if (!cpath)
	    cpath = _register_object(ClipMachineMemory, path, GTK_TYPE_TREE_PATH, NULL, NULL);
	 if (cpath)
	    _clip_aset(ClipMachineMemory, ret, &cpath->obj, 1, &l);
      }

   if (model)
      {
	 cmodel = _list_get_cobject(ClipMachineMemory, model);
	 if (!cmodel)
	    cmodel = _register_object(ClipMachineMemory, model, GTK_TYPE_TREE_MODEL, NULL, NULL);
	 if (cmodel)
	    _clip_mclone(ClipMachineMemory, mod, &cmodel->obj);
      }
   g_list_foreach(list, (GFunc) (gtk_tree_path_free), NULL);
   g_list_free(list);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREESELECTIONCOUNTSELECTEDROWS(ClipMachine * ClipMachineMemory)
{
   C_object *ctreesel = _fetch_co_arg(ClipMachineMemory);

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(ctreesel, GTK_IS_TREE_SELECTION(ctreesel->object));

   _clip_retni(ClipMachineMemory, gtk_tree_selection_count_selected_rows(GTK_TREE_SELECTION(ctreesel->object)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREESELECTIONUNSELECTRANGE(ClipMachine * ClipMachineMemory)
{
   C_object *ctreesel = _fetch_co_arg(ClipMachineMemory);
   C_object *cpath1 = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   C_object *cpath2 = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(ctreesel, GTK_IS_TREE_SELECTION(ctreesel->object));
   CHECKCOBJ(cpath1, GTK_IS_TREE_PATH(cpath1->object));
   CHECKCOBJ(cpath2, GTK_IS_TREE_PATH(cpath2->object));

   gtk_tree_selection_unselect_range(GTK_TREE_SELECTION(ctreesel->object), GTK_TREE_PATH(cpath1->object), GTK_TREE_PATH(cpath2->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREESELECTIONSELECTRANGE(ClipMachine * ClipMachineMemory)
{
   C_object *ctreesel = _fetch_co_arg(ClipMachineMemory);
   C_object *cpath1 = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   C_object *cpath2 = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(ctreesel, GTK_IS_TREE_SELECTION(ctreesel->object));
   CHECKCOBJ(cpath1, GTK_IS_TREE_PATH(cpath1->object));
   CHECKCOBJ(cpath2, GTK_IS_TREE_PATH(cpath2->object));

   gtk_tree_selection_select_range(GTK_TREE_SELECTION(ctreesel->object), GTK_TREE_PATH(cpath1->object), GTK_TREE_PATH(cpath2->object));

   return 0;
 err:
   return 1;
}
#endif
