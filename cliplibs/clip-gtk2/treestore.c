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

/*********************** SIGNALS **************************/

/* TreeStore has no signals */

/**********************************************************/
CLIP_DLLEXPORT GtkType
_gtk_type_tree_store()
{
   return GTK_TYPE_TREE_STORE;
}

long
_clip_type_tree_store()
{
   return GTK_OBJECT_TREE_STORE;
}

const char *
_clip_type_name_tree_store()
{
   return "GTK_OBJECT_TREE_STORE";
}

/* Register boxes in global table */
int
clip_INIT___TREESTORE(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_tree_store, _clip_type_name_tree_store, _gtk_type_tree_store, NULL, NULL);
   return 0;
}

void
__tree_store_destroy(ClipMachine * ClipMachineMemory, C_object * cotree)
{
   _clip_destroy(ClipMachineMemory, _clip_mget(ClipMachineMemory, &cotree->obj, HASH_UTYPES));
   return;
}

int static
__tree_store_set(ClipMachine * ClipMachineMemory, GtkTreeIter * iter, gint startDataParam)
{
   C_object *cstree = _fetch_co_arg(ClipMachineMemory);
   ClipArrVar *utypes;
   gint i;

   CHECKCOBJ(cstree, GTK_IS_TREE_STORE(cstree->object));

   utypes = (ClipArrVar *) _clip_vptr(_clip_mget(ClipMachineMemory, &cstree->obj, HASH_UTYPES));
   for (i = startDataParam; i <= _clip_parinfo(ClipMachineMemory, 0); i += 2)
      {
	 gint column = _clip_parni(ClipMachineMemory, i);
	 ClipVar *val;
	 GValue value;
	 C_object *cobj;
	 gchar *str;
	 int j, n;
	 double d;

	 CHECKARG(i, NUMERIC_type_of_ClipVarType);

	 if (column == -1)
	    break;

	 column--;
	 val = _clip_par(ClipMachineMemory, i + 1);
	 memset(&value, 0, sizeof(value));
	 switch ((int) utypes->ClipVar_items_of_ClipArrVar[column].ClipNumVar_n_of_ClipVar.double_of_ClipNumVar)
	    {
	    case TREE_TYPE_NUMERIC:
	       g_value_init(&value, G_TYPE_INT);
	       if (val->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
		  g_value_set_int(&value, (int) val->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
	       else
		  {
		     n = _clip_strtod(val->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, &j);
		     g_value_set_int(&value, n);
		  }
	       break;
	    case TREE_TYPE_NUMERIC_FLOAT:
	       g_value_init(&value, G_TYPE_FLOAT);
	       if (val->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
		  g_value_set_float(&value, val->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
	       else
		  {
		     d = _clip_strtod(val->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, &j);
		     g_value_set_float(&value, d);
		  }
	       break;
	    case TREE_TYPE_STRING:
	       str = val->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
	       LOCALE_TO_UTF(str);
	       g_value_init(&value, G_TYPE_STRING);
	       g_value_set_string(&value, str);
	       FREE_TEXT(str);
	       break;
	    case TREE_TYPE_LOGICAL:
	       g_value_init(&value, G_TYPE_BOOLEAN);
	       g_value_set_boolean(&value, val->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar);
	       break;
	    case TREE_TYPE_DATE:
	       g_value_init(&value, G_TYPE_STRING);
	       if (val->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == DATE_type_of_ClipVarType)
				 str = _clip_date_to_str(val->ClipLongVar_lv_of_ClipVar.long_of_ClipLongVar, ClipMachineMemory->date_format);
	       else
		  str = _clip_date_to_str(_clip_str_to_date(val->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, ClipMachineMemory->date_format, ClipMachineMemory->epoch),
ClipMachineMemory->date_format);
	       g_value_set_string(&value, str);
	       break;
	    case TREE_TYPE_PIXBUF:
	       g_value_init(&value, GDK_TYPE_PIXBUF);
	       cobj = _fetch_cobject(ClipMachineMemory, val);
	       g_value_set_object(&value, cobj->object);
	       break;
	    }
	 gtk_tree_store_set_value(GTK_TREE_STORE(cstree->object), iter, column, &value);
      }
   return 0;
 err:
   return 1;
}

int static
__tree_store_set_types(ClipMachine * ClipMachineMemory, gint ncolumns, GType * types, ClipVar * utypes)
{
   ClipArrVar *cvcol = (ClipArrVar *) _clip_vptr(_clip_spar(ClipMachineMemory, 3));
   gint i;
   long d;
   char is_array;
   int dtype;

   is_array = (_clip_parinfo(ClipMachineMemory, 3) == ARRAY_type_of_ClipVarType ? 1 : 0);

   d = ncolumns;
   _clip_array(ClipMachineMemory, utypes, 1, &d);
   for (i = 0, d = 0; i < ncolumns; i++, d++)
      {
	 ClipVar type;
	 if (is_array)
	    {			//column type as array item
	       memset(&type, 0, sizeof(type));
	       type.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	       type.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = (int) cvcol->ClipVar_items_of_ClipArrVar[i].ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
	       dtype = (int) type.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
	    }
	 else
	    {			// column type as parameter
	       CHECKARG(i + 3, NUMERIC_type_of_ClipVarType);

	       memset(&type, 0, sizeof(type));
	       type.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	       type.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = _clip_parnd(ClipMachineMemory, i + 3);
	       dtype = (int) type.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
	    }

	 switch (dtype)
	    {
	    case TREE_TYPE_STRING:
	       types[i] = G_TYPE_STRING;
	       break;
	    case TREE_TYPE_LOGICAL:
	       types[i] = G_TYPE_BOOLEAN;
	       break;
	    case TREE_TYPE_NUMERIC:
	       types[i] = G_TYPE_INT;
	       break;
	    case TREE_TYPE_NUMERIC_FLOAT:
	       types[i] = G_TYPE_FLOAT;
	       break;
	    case TREE_TYPE_DATE:
	       types[i] = G_TYPE_STRING;
	       break;
	    case TREE_TYPE_DATETIME:
	       types[i] = G_TYPE_STRING;
	       break;
	    case TREE_TYPE_PIXMAP:
	       types[i] = GTK_TYPE_PIXMAP;
	       break;
	    case TREE_TYPE_PIXBUF:
	       types[i] = GDK_TYPE_PIXBUF;
	       break;
	    default:
	       printf("add other type \n");
	       break;
	    }
	 _clip_aset(ClipMachineMemory, utypes, &type, 1, &d);
	 _clip_destroy(ClipMachineMemory, &type);
      }
   return 0;
 err:
   return 1;
}

/* gtk_TreeStoreNew(ncolumns, ...)                                 */
/* ncolumns -  number of columns in the tree store                 */
/* ...      -  all types for the columns, from first to last:      */
/* TREE_TYPE_STRING                                                */
/* TREE_TYPE_NUMERIC                                               */
/* TREE_TYPE_NUMERIC_FLOAT                                         */
/* TREE_TYPE_LOGICAL                                               */
/* TREE_TYPE_DATE                                                  */
/* TREE_TYPE_DATETIME                                              */
/* TREE_TYPE_PIXMAP                                                */
/* TREE_TYPE_PIXBUF                                                */
int
/******************************************************************************
* gtk_TreeStoreNew(map, ncolumns, type1, ...)
******************************************************************************/
clip_GTK_TREESTORENEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   gint ncolumns = _clip_parni(ClipMachineMemory, 2);
   ClipArrVar *cvcol = (ClipArrVar *) _clip_vptr(_clip_spar(ClipMachineMemory, 3));
   ClipVar *utypes;
   GType types[ncolumns];
   int i;
   long d;

   GtkTreeStore *tree;
   C_object *ctree;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   memset(types, 0, sizeof(types));
   utypes = NEW(ClipVar);

   if (_clip_parinfo(ClipMachineMemory, 3) == ARRAY_type_of_ClipVarType && cvcol->count_of_ClipArrVar != ncolumns)
      return 1;
   if (_clip_parinfo(ClipMachineMemory, 3) != UNDEF_type_of_ClipVarType)	// defined column types as parameters or in array
      __tree_store_set_types(ClipMachineMemory, ncolumns, types, utypes);
   else
      {				// if third parameter is empty - use column with G_TYPE_STRING type
	 ClipVar type;
	 d = ncolumns;
	 memset(&type, 0, sizeof(type));
	 type.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	 type.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = 0;
	 _clip_array(ClipMachineMemory, utypes, 1, &d);
	 for (i = 0, d = 0; i < ncolumns; i++, d++)
	    {
	       types[i] = G_TYPE_STRING;
	       _clip_aset(ClipMachineMemory, utypes, &type, 1, &d);
	    }
	 _clip_destroy(ClipMachineMemory, &type);
      }

   tree = gtk_tree_store_newv(ncolumns, types);

   if (tree)
      {
	 ctree = _list_get_cobject(ClipMachineMemory, tree);
	 if (!ctree)
	    ctree = _register_object(ClipMachineMemory, tree, GTK_TYPE_TREE_STORE, cv, __tree_store_destroy);
	 if (ctree)
	    {

	       _clip_madd(ClipMachineMemory, &ctree->obj, HASH_UTYPES, utypes);
	       _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &ctree->obj);
	    }
      }
   free(utypes);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TreeStoreSetColumnTypes(tree, ncolumns, type1, ...)
******************************************************************************/
int
clip_GTK_TREESTORESETCOLUMNTYPES(ClipMachine * ClipMachineMemory)
{
   C_object *cstree = _fetch_co_arg(ClipMachineMemory);
   gint ncolumns = _clip_parni(ClipMachineMemory, 2);
   GType types[ncolumns];
   ClipVar *utypes;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(cstree, GTK_IS_TREE_STORE(cstree->object));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   memset(types, 0, sizeof(types));
   utypes = NEW(ClipVar);

   __tree_store_set_types(ClipMachineMemory, ncolumns, types, utypes);

   gtk_tree_store_set_column_types(GTK_TREE_STORE(cstree->object), ncolumns, types);

   _clip_madd(ClipMachineMemory, &cstree->obj, HASH_UTYPES, utypes);

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TreeStoreSet(tree,path_string,  ncol, val, ...)
******************************************************************************/

int
clip_GTK_TREESTORESET(ClipMachine * ClipMachineMemory)
{
   C_object *cstree = _fetch_co_arg(ClipMachineMemory);
   gchar *path = _clip_parc(ClipMachineMemory, 2);
   GtkTreeIter iter;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cstree, GTK_IS_TREE_STORE(cstree->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   gtk_tree_model_get_iter(GTK_TREE_MODEL(GTK_TREE_STORE(cstree->object)), &iter, gtk_tree_path_new_from_string(path));

   __tree_store_set(ClipMachineMemory, &iter, 3);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TreeStoreSetValue(tree,path_string,  ncol, val)
******************************************************************************/
int
clip_GTK_TREESTORESETVALUE(ClipMachine * ClipMachineMemory)
{
   C_object *cstree = _fetch_co_arg(ClipMachineMemory);
   gchar *path = _clip_parc(ClipMachineMemory, 2);
   gint column = _clip_parni(ClipMachineMemory, 3);
   ClipVar *val = _clip_par(ClipMachineMemory, 4);;
   GtkTreeIter iter;
   ClipArrVar *utypes;
   GValue value;
   C_object *cobj;
   gchar *str;
   int j, n;
   double d;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cstree, GTK_IS_TREE_STORE(cstree->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);

   gtk_tree_model_get_iter(GTK_TREE_MODEL(GTK_TREE_STORE(cstree->object)), &iter, gtk_tree_path_new_from_string(path));

   utypes = (ClipArrVar *) _clip_vptr(_clip_mget(ClipMachineMemory, &cstree->obj, HASH_UTYPES));
   column--;

   memset(&value, 0, sizeof(value));
   switch ((int) utypes->ClipVar_items_of_ClipArrVar[column].ClipNumVar_n_of_ClipVar.double_of_ClipNumVar)
      {
      case TREE_TYPE_NUMERIC:
	 g_value_init(&value, G_TYPE_INT);
	 if (val->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
	    g_value_set_int(&value, (int) val->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
	 else
	    {
	       n = _clip_strtod(val->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, &j);
	       g_value_set_int(&value, n);
	    }
	 break;
      case TREE_TYPE_NUMERIC_FLOAT:
	 g_value_init(&value, G_TYPE_FLOAT);
	 if (val->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
	    g_value_set_float(&value, val->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
	 else
	    {
	       d = _clip_strtod(val->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, &j);
	       g_value_set_float(&value, d);
	    }
	 break;
      case TREE_TYPE_STRING:
	 str = val->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
	 LOCALE_TO_UTF(str);
	 g_value_init(&value, G_TYPE_STRING);
	 g_value_set_string(&value, str);
	 FREE_TEXT(str);
	 break;
      case TREE_TYPE_LOGICAL:
	 g_value_init(&value, G_TYPE_BOOLEAN);
	 g_value_set_boolean(&value, val->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar);
	 break;
      case TREE_TYPE_DATE:
	 g_value_init(&value, G_TYPE_STRING);
	 if (val->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == DATE_type_of_ClipVarType)
		 str = _clip_date_to_str(val->ClipLongVar_lv_of_ClipVar.long_of_ClipLongVar, ClipMachineMemory->date_format);
	 else
	    str = _clip_date_to_str(_clip_str_to_date(val->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, ClipMachineMemory->date_format, ClipMachineMemory->epoch),
ClipMachineMemory->date_format);
	 g_value_set_string(&value, str);
	 break;
      case TREE_TYPE_PIXBUF:
	 g_value_init(&value, GDK_TYPE_PIXBUF);
	 cobj = _fetch_cobject(ClipMachineMemory, val);
	 g_value_set_object(&value, cobj->object);
	 break;
      }
   gtk_tree_store_set_value(GTK_TREE_STORE(cstree->object), &iter, column, &value);

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TreeStoreRemove(tree, path_string)
******************************************************************************/
int
clip_GTK_TREESTOREREMOVE(ClipMachine * ClipMachineMemory)
{

   C_object *cstree = _fetch_co_arg(ClipMachineMemory);
   gchar *path = _clip_parc(ClipMachineMemory, 2);
   GtkTreeIter iter;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cstree, GTK_IS_TREE_STORE(cstree->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   gtk_tree_model_get_iter(GTK_TREE_MODEL(GTK_TREE_STORE(cstree->object)), &iter, gtk_tree_path_new_from_string(path));

   gtk_tree_store_remove(GTK_TREE_STORE(cstree->object), &iter);

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TreeStoreInsert(tree, parent_path_string/NULL, position, ncol, val, ....) -->path_string
******************************************************************************/
int
clip_GTK_TREESTOREINSERT(ClipMachine * ClipMachineMemory)
{

   C_object *cstree = _fetch_co_arg(ClipMachineMemory);
   gchar *parent = _clip_parc(ClipMachineMemory, 2);
   gint position = _clip_parni(ClipMachineMemory, 3);
   GtkTreeIter iter;
   GtkTreeIter parentiter;
   GtkTreePath *path;
   gchar *path_string;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cstree, GTK_IS_TREE_STORE(cstree->object));
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);

   if (parent)
      gtk_tree_model_get_iter(GTK_TREE_MODEL(GTK_TREE_STORE(cstree->object)), &parentiter, gtk_tree_path_new_from_string(parent));

   gtk_tree_store_insert(GTK_TREE_STORE(cstree->object), &iter, (parent ? &parentiter : NULL), position);

   __tree_store_set(ClipMachineMemory, &iter, 4);

   path = gtk_tree_model_get_path(GTK_TREE_MODEL(GTK_TREE_STORE(cstree->object)), &iter);

   path_string = gtk_tree_path_to_string(path);

   _clip_retc(ClipMachineMemory, path_string);

   g_free(path_string);

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TreeStoreInsertBefore(tree, parent_path_string/NULL, sibling_path_string/NULL, ncol, val, ....)-->path_string
******************************************************************************/
int
clip_GTK_TREESTOREINSERTBEFORE(ClipMachine * ClipMachineMemory)
{

   C_object *cstree = _fetch_co_arg(ClipMachineMemory);
   gchar *parent = _clip_parc(ClipMachineMemory, 2);
   gchar *sibling = _clip_parc(ClipMachineMemory, 3);
   GtkTreeIter iter;
   GtkTreeIter siblingiter;
   GtkTreeIter parentiter;
   GtkTreePath *path;
   gchar *path_string;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cstree, GTK_IS_TREE_STORE(cstree->object));
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT(3, CHARACTER_type_of_ClipVarType);

   if (parent)
      gtk_tree_model_get_iter(GTK_TREE_MODEL(GTK_TREE_STORE(cstree->object)), &parentiter, gtk_tree_path_new_from_string(parent));

   if (sibling)
      gtk_tree_model_get_iter(GTK_TREE_MODEL(GTK_TREE_STORE(cstree->object)), &siblingiter, gtk_tree_path_new_from_string(sibling));

   gtk_tree_store_insert_before(GTK_TREE_STORE(cstree->object), &iter, (parent ? &parentiter : NULL), (sibling ? &siblingiter : NULL));

   __tree_store_set(ClipMachineMemory, &iter, 4);

   path = gtk_tree_model_get_path(GTK_TREE_MODEL(GTK_TREE_STORE(cstree->object)), &iter);

   path_string = gtk_tree_path_to_string(path);

   _clip_retc(ClipMachineMemory, path_string);

   g_free(path_string);

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TreeStoreInsertAfter(tree, parent_path_string/NULL, sibling_path_string/NULL, ncol, val, ....)-->path_string
******************************************************************************/
int
clip_GTK_TREESTOREINSERTAFTER(ClipMachine * ClipMachineMemory)
{

   C_object *cstree = _fetch_co_arg(ClipMachineMemory);
   gchar *parent = _clip_parc(ClipMachineMemory, 2);
   gchar *sibling = _clip_parc(ClipMachineMemory, 3);
   GtkTreeIter iter;
   GtkTreeIter parentiter;
   GtkTreeIter siblingiter;
   GtkTreePath *path;
   gchar *path_string;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cstree, GTK_IS_TREE_STORE(cstree->object));
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT(3, CHARACTER_type_of_ClipVarType);

   if (parent)
      gtk_tree_model_get_iter(GTK_TREE_MODEL(GTK_TREE_STORE(cstree->object)), &parentiter, gtk_tree_path_new_from_string(parent));

   if (sibling)
      gtk_tree_model_get_iter(GTK_TREE_MODEL(GTK_TREE_STORE(cstree->object)), &siblingiter, gtk_tree_path_new_from_string(sibling));

   gtk_tree_store_insert_after(GTK_TREE_STORE(cstree->object), &iter, (parent ? &parentiter : NULL), (sibling ? &siblingiter : NULL));

   __tree_store_set(ClipMachineMemory, &iter, 4);

   path = gtk_tree_model_get_path(GTK_TREE_MODEL(GTK_TREE_STORE(cstree->object)), &iter);

   path_string = gtk_tree_path_to_string(path);

   _clip_retc(ClipMachineMemory, path_string);

   g_free(path_string);

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TreeStorePrepend(tree, parent_path_string/NULL,ncol, val, ....)-->path_string
******************************************************************************/

int
clip_GTK_TREESTOREPREPEND(ClipMachine * ClipMachineMemory)
{
   C_object *cstree = _fetch_co_arg(ClipMachineMemory);
   gchar *parent = _clip_parc(ClipMachineMemory, 2);
   GtkTreeIter iter;
   GtkTreeIter parentiter;
   GtkTreePath *path;
   gchar *path_string;

   CHECKCOBJ(cstree, GTK_IS_TREE_STORE(cstree->object));
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);

   if (parent)
      gtk_tree_model_get_iter(GTK_TREE_MODEL(GTK_TREE_STORE(cstree->object)), &parentiter, gtk_tree_path_new_from_string(parent));

   gtk_tree_store_prepend(GTK_TREE_STORE(cstree->object), &iter, (parent ? &parentiter : NULL));

   __tree_store_set(ClipMachineMemory, &iter, 3);

   path = gtk_tree_model_get_path(GTK_TREE_MODEL(GTK_TREE_STORE(cstree->object)), &iter);

   path_string = gtk_tree_path_to_string(path);

   _clip_retc(ClipMachineMemory, path_string);

   g_free(path_string);

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TreeStoreAppend(tree, parent_path_string/NULL, ncol, val, ....)-->path_string
******************************************************************************/

int
clip_GTK_TREESTOREAPPEND(ClipMachine * ClipMachineMemory)
{
   C_object *cstree = _fetch_co_arg(ClipMachineMemory);
   gchar *parent = _clip_parc(ClipMachineMemory, 2);
   GtkTreeIter iter;
   GtkTreeIter parentiter;
   GtkTreePath *path;
   gchar *path_string;

   CHECKCOBJ(cstree, GTK_IS_TREE_STORE(cstree->object));
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);

   memset(&parentiter, 0, sizeof(parentiter));
   if (parent)
      gtk_tree_model_get_iter(GTK_TREE_MODEL(GTK_TREE_STORE(cstree->object)), &parentiter, gtk_tree_path_new_from_string(parent));

   gtk_tree_store_append(GTK_TREE_STORE(cstree->object), &iter, (parent ? &parentiter : NULL));

   __tree_store_set(ClipMachineMemory, &iter, 3);

   path = gtk_tree_model_get_path(GTK_TREE_MODEL(GTK_TREE_STORE(cstree->object)), &iter);

   path_string = gtk_tree_path_to_string(path);

   _clip_retc(ClipMachineMemory, path_string);

   g_free(path_string);

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TreeStoreIsAncestor(tree, ancestor_path_string, descendant_path_string)-->TRUE or FALSE
******************************************************************************/
int
clip_GTK_TREESTOREISANCESTOR(ClipMachine * ClipMachineMemory)
{

   C_object *cstree = _fetch_co_arg(ClipMachineMemory);
   gchar *ancestor = _clip_parc(ClipMachineMemory, 2);
   gchar *descendant = _clip_parc(ClipMachineMemory, 3);
   GtkTreeIter aiter;
   GtkTreeIter diter;
   gboolean ret;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cstree, GTK_IS_TREE_STORE(cstree->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);
   CHECKARG(3, CHARACTER_type_of_ClipVarType);

   gtk_tree_model_get_iter(GTK_TREE_MODEL(GTK_TREE_STORE(cstree->object)), &aiter, gtk_tree_path_new_from_string(ancestor));

   gtk_tree_model_get_iter(GTK_TREE_MODEL(GTK_TREE_STORE(cstree->object)), &diter, gtk_tree_path_new_from_string(descendant));

   ret = gtk_tree_store_is_ancestor(GTK_TREE_STORE(cstree->object), &aiter, &diter);
   _clip_retl(ClipMachineMemory, ret);

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TreeStoreIterDepth(tree, path_string)-->gint depth
******************************************************************************/
int
clip_GTK_TREESTOREITERDEPTH(ClipMachine * ClipMachineMemory)
{

   C_object *cstree = _fetch_co_arg(ClipMachineMemory);
   gchar *path = _clip_parc(ClipMachineMemory, 2);
   GtkTreeIter iter;
   gint ret;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cstree, GTK_IS_TREE_STORE(cstree->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);
   CHECKARG(3, CHARACTER_type_of_ClipVarType);

   gtk_tree_model_get_iter(GTK_TREE_MODEL(GTK_TREE_STORE(cstree->object)), &iter, gtk_tree_path_new_from_string(path));

   ret = gtk_tree_store_iter_depth(GTK_TREE_STORE(cstree->object), &iter);
   _clip_retni(ClipMachineMemory, ret);

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TreeStoreClear(tree)
******************************************************************************/

int
clip_GTK_TREESTORECLEAR(ClipMachine * ClipMachineMemory)
{
   C_object *cstree = _fetch_co_arg(ClipMachineMemory);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cstree, GTK_IS_TREE_STORE(cstree->object));

   gtk_tree_store_clear(GTK_TREE_STORE(cstree->object));

   return 0;
 err:
   return 1;
}

#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 2)
int
clip_GTK_TREESTOREITERISVALID(ClipMachine * ClipMachineMemory)
{
   C_object *cstree = _fetch_co_arg(ClipMachineMemory);
   C_object *citer = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cstree, GTK_IS_TREE_STORE(cstree->object));
   CHECKCOBJ(citer, GTK_IS_TREE_ITER(citer->object));

   gtk_tree_store_iter_is_valid(GTK_TREE_STORE(cstree->object), GTK_TREE_ITER(citer->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREESTOREREORDER(ClipMachine * ClipMachineMemory)
{
   C_object *cstree = _fetch_co_arg(ClipMachineMemory);
   C_object *citer = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   ClipArrVar *arr = (ClipArrVar *) _clip_vptr(_clip_spar(ClipMachineMemory, 3));
   gint n;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cstree, GTK_IS_TREE_STORE(cstree->object));
   CHECKCOBJ(citer, GTK_IS_TREE_ITER(citer->object));
   CHECKARG(3, ARRAY_type_of_ClipVarType);

   n = arr->count_of_ClipArrVar;
   if (arr)
      {
	 gint *order, i;

	 order = malloc(n * sizeof(gint));
	 for (i = 0; i < n; i++)
	    order[i] = arr->ClipVar_items_of_ClipArrVar[i].ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
	 gtk_tree_store_reorder(GTK_TREE_STORE(cstree->object), GTK_TREE_ITER(citer->object), order);

	 free(order);
      }
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREESTORESWAP(ClipMachine * ClipMachineMemory)
{
   C_object *cstree = _fetch_co_arg(ClipMachineMemory);
   C_object *citer1 = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   C_object *citer2 = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cstree, GTK_IS_TREE_STORE(cstree->object));
   CHECKCOBJ(citer1, GTK_IS_TREE_ITER(citer1->object));
   CHECKCOBJ(citer2, GTK_IS_TREE_ITER(citer2->object));

   gtk_tree_store_swap(GTK_TREE_STORE(cstree->object), GTK_TREE_ITER(citer1->object), GTK_TREE_ITER(citer2->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREESTOREMOVEBEFORE(ClipMachine * ClipMachineMemory)
{
   C_object *cstree = _fetch_co_arg(ClipMachineMemory);
   C_object *citer1 = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   C_object *citer2 = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cstree, GTK_IS_TREE_STORE(cstree->object));
   CHECKCOBJ(citer1, GTK_IS_TREE_ITER(citer1->object));
   CHECKCOBJOPT(citer2, GTK_IS_TREE_ITER(citer2->object));

   gtk_tree_store_move_before(GTK_TREE_STORE(cstree->object), GTK_TREE_ITER(citer1->object), (citer2) ? GTK_TREE_ITER(citer2->object) : NULL);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREESTOREMOVEAFTER(ClipMachine * ClipMachineMemory)
{
   C_object *cstree = _fetch_co_arg(ClipMachineMemory);
   C_object *citer1 = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   C_object *citer2 = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cstree, GTK_IS_TREE_STORE(cstree->object));
   CHECKCOBJ(citer1, GTK_IS_TREE_ITER(citer1->object));
   CHECKCOBJOPT(citer2, GTK_IS_TREE_ITER(citer2->object));

   gtk_tree_store_move_after(GTK_TREE_STORE(cstree->object), GTK_TREE_ITER(citer1->object), (citer2) ? GTK_TREE_ITER(citer2->object) : NULL);

   return 0;
 err:
   return 1;
}
#endif
