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

/* ListStore has no signals */

/**********************************************************/
CLIP_DLLEXPORT GtkType
_gtk_type_list_store()
{
   return GTK_TYPE_LIST_STORE;
}

long
_clip_type_list_store()
{
   return GTK_OBJECT_LIST_STORE;
}

const char *
_clip_type_name_list_store()
{
   return "GTK_OBJECT_LIST_STORE";
}

/* Register boxes in global table */
int
clip_INIT___LISTSTORE(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_list_store, _clip_type_name_list_store, _gtk_type_list_store, NULL, NULL);
   return 0;
}

void
__list_store_destroy(ClipMachine * ClipMachineMemory, C_object * colist)
{

   _clip_destroy(ClipMachineMemory, _clip_mget(ClipMachineMemory, &colist->obj, HASH_UTYPES));
   return;
}

int static
__list_store_set(ClipMachine * ClipMachineMemory, GtkTreeIter * iter, gint startDataParam)
{
   C_object *cslist = _fetch_co_arg(ClipMachineMemory);

   ClipArrVar *utypes;

   gint      i;

   CHECKCOBJ(cslist, GTK_IS_LIST_STORE(cslist->object));

   utypes = (ClipArrVar *) _clip_vptr(_clip_mget(ClipMachineMemory, &cslist->obj, HASH_UTYPES));
   for (i = startDataParam; i <= _clip_parinfo(ClipMachineMemory, 0); i += 2)
    {
       gint      column = _clip_parni(ClipMachineMemory, i);

       ClipVar  *val;

       GValue    value;

       C_object *cobj;

       gchar    *str;

       int       j, n;

       double    d;

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
	case TREE_TYPE_PIXBUF:
	   g_value_init(&value, GDK_TYPE_PIXBUF);
	   cobj = _fetch_cobject(ClipMachineMemory, val);
	   g_value_set_object(&value, cobj->object);
	   break;
	case TREE_TYPE_DATE:
	   g_value_init(&value, G_TYPE_STRING);
	   if (val->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == DATE_type_of_ClipVarType)
	      str = _clip_date_to_str(val->ClipLongVar_lv_of_ClipVar.len_of_ClipLongVar, ClipMachineMemory->date_format);
	   else
	      str =
	       _clip_date_to_str(_clip_str_to_date
				 (val->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf,
				  ClipMachineMemory->date_format, ClipMachineMemory->epoch), ClipMachineMemory->date_format);
	   g_value_set_string(&value, str);
	   break;
	}
       gtk_list_store_set_value(GTK_LIST_STORE(cslist->object), iter, column, &value);
    }
   return 0;
 err:
   return 1;
}

int static
__list_store_set_types(ClipMachine * ClipMachineMemory, gint ncolumns, GType * types, ClipVar * utypes)
{
   ClipArrVar *cvcol = (ClipArrVar *) _clip_vptr(_clip_spar(ClipMachineMemory, 3));

   gint      i;

   long      d;

   char      is_array;

   int       dtype;

   is_array = (_clip_parinfo(ClipMachineMemory, 3) == ARRAY_type_of_ClipVarType ? 1 : 0);

   d = ncolumns;
   _clip_array(ClipMachineMemory, utypes, 1, &d);
   for (i = 0, d = 0; i < ncolumns; i++, d++)
    {
       ClipVar   type;

       if (is_array)
	{			//column type as array item
	   memset(&type, 0, sizeof(type));
	   type.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	   type.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar =
	    (int) cvcol->ClipVar_items_of_ClipArrVar[i].ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
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

/* gtk_ListStoreNew(ncolumns, ...)                                 */
/* ncolumns -  number of columns in the list store                 */
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
* gtk_ListStoreNew(map, ncolumns, type1, ...)
******************************************************************************/
clip_GTK_LISTSTORENEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   gint      ncolumns = _clip_parni(ClipMachineMemory, 2);

   ClipArrVar *cvcol = (ClipArrVar *) _clip_vptr(_clip_spar(ClipMachineMemory, 3));

   ClipVar  *utypes;

   GType     types[ncolumns];

   int       i;

   long      d;

   GtkListStore *list;

   C_object *clist;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
  //CHECKARG2(3, NUMERIC_type_of_ClipVarType, ARRAY_type_of_ClipVarType);

   memset(types, 0, sizeof(types));
   utypes = NEW(ClipVar);

   if (_clip_parinfo(ClipMachineMemory, 3) == ARRAY_type_of_ClipVarType && cvcol->count_of_ClipArrVar != ncolumns)
      return 1;
   if (_clip_parinfo(ClipMachineMemory, 3) != UNDEF_type_of_ClipVarType)	// defined column types as parameters or in array
      __list_store_set_types(ClipMachineMemory, ncolumns, types, utypes);
   else
    {				// if third parameter is empty - use column with G_TYPE_STRING type
       ClipVar   type;

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

   list = gtk_list_store_newv(ncolumns, types);
   if (list)
    {
       clist = _list_get_cobject(ClipMachineMemory, list);
       if (!clist)
	  clist = _register_object(ClipMachineMemory, list, GTK_TYPE_LIST_STORE, cv, __list_store_destroy);
       if (clist)
	{
	   _clip_madd(ClipMachineMemory, &clist->obj, HASH_UTYPES, utypes);
	   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &clist->obj);
	}
    }

   free(utypes);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ListStoreSetColumnTypes(list, ncolumns, type1, ...)
******************************************************************************/
int
clip_GTK_LISTSTORESETCOLUMNTYPES(ClipMachine * ClipMachineMemory)
{
   C_object *cslist = _fetch_co_arg(ClipMachineMemory);

   gint      ncolumns = _clip_parni(ClipMachineMemory, 2);

   GType     types[ncolumns];

   ClipVar  *utypes;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(cslist, GTK_IS_LIST_STORE(cslist->object));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   memset(types, 0, sizeof(types));
   utypes = NEW(ClipVar);

   __list_store_set_types(ClipMachineMemory, ncolumns, types, utypes);

   gtk_list_store_set_column_types(GTK_LIST_STORE(cslist->object), ncolumns, types);

   _clip_madd(ClipMachineMemory, &cslist->obj, HASH_UTYPES, utypes);

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ListStoreSet(list,path_string,  ncol, val, ...)
******************************************************************************/

int
clip_GTK_LISTSTORESET(ClipMachine * ClipMachineMemory)
{
   C_object *cslist = _fetch_co_arg(ClipMachineMemory);

   gchar    *path = _clip_parc(ClipMachineMemory, 2);

   GtkTreeIter iter;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cslist, GTK_IS_LIST_STORE(cslist->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   gtk_tree_model_get_iter(GTK_TREE_MODEL(GTK_LIST_STORE(cslist->object)), &iter, gtk_tree_path_new_from_string(path));

   __list_store_set(ClipMachineMemory, &iter, 3);

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ListStoreSetValue(list,path_string,  ncol, val)
******************************************************************************/
int
clip_GTK_LISTSTORESETVALUE(ClipMachine * ClipMachineMemory)
{
   C_object *cslist = _fetch_co_arg(ClipMachineMemory);

   gchar    *path = _clip_parc(ClipMachineMemory, 2);

   gint      column = _clip_parni(ClipMachineMemory, 3);

   ClipVar  *val = _clip_par(ClipMachineMemory, 4);;
   GtkTreeIter iter;

   ClipArrVar *utypes;

   GValue    value;

   C_object *cobj;

   gchar    *str;

   int       j, n;

   double    d;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cslist, GTK_IS_LIST_STORE(cslist->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);

   gtk_tree_model_get_iter(GTK_TREE_MODEL(GTK_LIST_STORE(cslist->object)), &iter, gtk_tree_path_new_from_string(path));

   utypes = (ClipArrVar *) _clip_vptr(_clip_mget(ClipMachineMemory, &cslist->obj, HASH_UTYPES));
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
    case TREE_TYPE_PIXBUF:
       g_value_init(&value, GDK_TYPE_PIXBUF);
       cobj = _fetch_cobject(ClipMachineMemory, val);
       g_value_set_object(&value, cobj->object);
       break;
    case TREE_TYPE_DATE:
       g_value_init(&value, G_TYPE_STRING);
       if (val->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == DATE_type_of_ClipVarType)
	  str = _clip_date_to_str(val->ClipLongVar_lv_of_ClipVar.len_of_ClipLongVar, ClipMachineMemory->date_format);
       else
	  str =
	   _clip_date_to_str(_clip_str_to_date
			     (val->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf,
			      ClipMachineMemory->date_format, ClipMachineMemory->epoch), ClipMachineMemory->date_format);
       g_value_set_string(&value, str);
       break;
    }
   gtk_list_store_set_value(GTK_LIST_STORE(cslist->object), &iter, column, &value);

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ListStoreRemove(list, path_string)
******************************************************************************/
int
clip_GTK_LISTSTOREREMOVE(ClipMachine * ClipMachineMemory)
{

   C_object *cslist = _fetch_co_arg(ClipMachineMemory);

   gchar    *path = _clip_parc(ClipMachineMemory, 2);

   GtkTreeIter iter;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cslist, GTK_IS_LIST_STORE(cslist->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   gtk_tree_model_get_iter(GTK_TREE_MODEL(GTK_LIST_STORE(cslist->object)), &iter, gtk_tree_path_new_from_string(path));

   gtk_list_store_remove(GTK_LIST_STORE(cslist->object), &iter);

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ListStoreInsert(list, position, ncol, val, ....) -->path_string
******************************************************************************/
int
clip_GTK_LISTSTOREINSERT(ClipMachine * ClipMachineMemory)
{

   C_object *cslist = _fetch_co_arg(ClipMachineMemory);

   gint      position = _clip_parni(ClipMachineMemory, 2);

   GtkTreeIter iter;

   GtkTreePath *path;

   gchar    *path_string;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cslist, GTK_IS_LIST_STORE(cslist->object));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_list_store_insert(GTK_LIST_STORE(cslist->object), &iter, position);

   __list_store_set(ClipMachineMemory, &iter, 3);

   path = gtk_tree_model_get_path(GTK_TREE_MODEL(GTK_LIST_STORE(cslist->object)), &iter);

   path_string = gtk_tree_path_to_string(path);

   _clip_retc(ClipMachineMemory, path_string);

   g_free(path_string);

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ListStoreInsertBefore(list, sibling_path_string/NULL, ncol, val, ....)-->path_string
******************************************************************************/
int
clip_GTK_LISTSTOREINSERTBEFORE(ClipMachine * ClipMachineMemory)
{

   C_object *cslist = _fetch_co_arg(ClipMachineMemory);

   gchar    *sibling = _clip_parc(ClipMachineMemory, 2);

   GtkTreeIter iter;

   GtkTreeIter siblingiter;

   GtkTreePath *path;

   gchar    *path_string;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cslist, GTK_IS_LIST_STORE(cslist->object));
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);

   if (sibling)
      gtk_tree_model_get_iter(GTK_TREE_MODEL(GTK_LIST_STORE(cslist->object)),
			      &siblingiter, gtk_tree_path_new_from_string(sibling));

   gtk_list_store_insert_before(GTK_LIST_STORE(cslist->object), &iter, (sibling ? &siblingiter : NULL));

   __list_store_set(ClipMachineMemory, &iter, 3);

   path = gtk_tree_model_get_path(GTK_TREE_MODEL(GTK_LIST_STORE(cslist->object)), &iter);

   path_string = gtk_tree_path_to_string(path);

   _clip_retc(ClipMachineMemory, path_string);

   g_free(path_string);

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ListStoreInsertAfter(list, sibling_path_string/NULL, ncol, val, ....)-->path_string
******************************************************************************/
int
clip_GTK_LISTSTOREINSERTAFTER(ClipMachine * ClipMachineMemory)
{

   C_object *cslist = _fetch_co_arg(ClipMachineMemory);

   gchar    *sibling = _clip_parc(ClipMachineMemory, 2);

   GtkTreeIter iter;

   GtkTreeIter siblingiter;

   GtkTreePath *path;

   gchar    *path_string;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cslist, GTK_IS_LIST_STORE(cslist->object));
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);

   if (sibling)
      gtk_tree_model_get_iter(GTK_TREE_MODEL(GTK_LIST_STORE(cslist->object)),
			      &siblingiter, gtk_tree_path_new_from_string(sibling));

   gtk_list_store_insert_after(GTK_LIST_STORE(cslist->object), &iter, (sibling ? &siblingiter : NULL));

   __list_store_set(ClipMachineMemory, &iter, 3);

   path = gtk_tree_model_get_path(GTK_TREE_MODEL(GTK_LIST_STORE(cslist->object)), &iter);

   path_string = gtk_tree_path_to_string(path);

   _clip_retc(ClipMachineMemory, path_string);

   g_free(path_string);

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ListStorePrepend(list, ncol, val, ....)-->path_string
******************************************************************************/

int
clip_GTK_LISTSTOREPREPEND(ClipMachine * ClipMachineMemory)
{
   C_object *cslist = _fetch_co_arg(ClipMachineMemory);

   GtkTreeIter iter;

   GtkTreePath *path;

   gchar    *path_string;

   CHECKCOBJ(cslist, GTK_IS_LIST_STORE(cslist->object));

   gtk_list_store_prepend(GTK_LIST_STORE(cslist->object), &iter);

   __list_store_set(ClipMachineMemory, &iter, 2);

   path = gtk_tree_model_get_path(GTK_TREE_MODEL(GTK_LIST_STORE(cslist->object)), &iter);

   path_string = gtk_tree_path_to_string(path);

   _clip_retc(ClipMachineMemory, path_string);

   g_free(path_string);

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ListStoreAppend(list, ncol, val, ....)-->path_string
******************************************************************************/

int
clip_GTK_LISTSTOREAPPEND(ClipMachine * ClipMachineMemory)
{
   C_object *cslist = _fetch_co_arg(ClipMachineMemory);

   GtkTreeIter iter;

   GtkTreePath *path;

   gchar    *path_string;

   CHECKCOBJ(cslist, GTK_IS_LIST_STORE(cslist->object));

   gtk_list_store_append(GTK_LIST_STORE(cslist->object), &iter);

   __list_store_set(ClipMachineMemory, &iter, 2);

   path = gtk_tree_model_get_path(GTK_TREE_MODEL(GTK_LIST_STORE(cslist->object)), &iter);

   path_string = gtk_tree_path_to_string(path);

   _clip_retc(ClipMachineMemory, path_string);

   g_free(path_string);

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ListStoreClear(list)
******************************************************************************/

int
clip_GTK_LISTSTORECLEAR(ClipMachine * ClipMachineMemory)
{
   C_object *cslist = _fetch_co_arg(ClipMachineMemory);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cslist, GTK_IS_LIST_STORE(cslist->object));

   gtk_list_store_clear(GTK_LIST_STORE(cslist->object));

   return 0;
 err:
   return 1;
}

#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 2)

int
clip_GTK_LISTSTOREITERISVALID(ClipMachine * ClipMachineMemory)
{
   C_object *cslist = _fetch_co_arg(ClipMachineMemory);

   C_object *citer = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cslist, GTK_IS_LIST_STORE(cslist->object));
   CHECKCOBJ(citer, GTK_IS_TREE_ITER(citer->object));

   _clip_retl(ClipMachineMemory, gtk_list_store_iter_is_valid(GTK_LIST_STORE(cslist->object), GTK_TREE_ITER(citer->object)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_LISTSTOREREORDER(ClipMachine * ClipMachineMemory)
{
   C_object *cslist = _fetch_co_arg(ClipMachineMemory);

   ClipArrVar *arr = (ClipArrVar *) _clip_vptr(_clip_spar(ClipMachineMemory, 2));

   gint      n;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cslist, GTK_IS_LIST_STORE(cslist->object));
   CHECKARG(2, ARRAY_type_of_ClipVarType);

   n = arr->count_of_ClipArrVar;
   if (arr)
    {
       gint     *order, i;

       order = malloc(n * sizeof(gint));
       for (i = 0; i < n; i++)
	  order[i] = arr->ClipVar_items_of_ClipArrVar[i].ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
       gtk_list_store_reorder(GTK_LIST_STORE(cslist->object), order);

       free(order);
    }
   return 0;
 err:
   return 1;
}

int
clip_GTK_LISTSTORESWAP(ClipMachine * ClipMachineMemory)
{
   C_object *cslist = _fetch_co_arg(ClipMachineMemory);

   C_object *citer1 = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_object *citer2 = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cslist, GTK_IS_LIST_STORE(cslist->object));
   CHECKCOBJ(citer1, GTK_IS_TREE_ITER(citer1->object));
   CHECKCOBJ(citer2, GTK_IS_TREE_ITER(citer2->object));

   gtk_list_store_swap(GTK_LIST_STORE(cslist->object), GTK_TREE_ITER(citer1->object), GTK_TREE_ITER(citer2->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_LISTSTOREMOVEBEFORE(ClipMachine * ClipMachineMemory)
{
   C_object *cslist = _fetch_co_arg(ClipMachineMemory);

   C_object *citer1 = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_object *citer2 = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cslist, GTK_IS_LIST_STORE(cslist->object));
   CHECKCOBJ(citer1, GTK_IS_TREE_ITER(citer1->object));
   CHECKCOBJ(citer2, GTK_IS_TREE_ITER(citer2->object));

   gtk_list_store_move_before(GTK_LIST_STORE(cslist->object), GTK_TREE_ITER(citer1->object), GTK_TREE_ITER(citer2->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_LISTSTOREMOVEAFTER(ClipMachine * ClipMachineMemory)
{
   C_object *cslist = _fetch_co_arg(ClipMachineMemory);

   C_object *citer1 = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_object *citer2 = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cslist, GTK_IS_LIST_STORE(cslist->object));
   CHECKCOBJ(citer1, GTK_IS_TREE_ITER(citer1->object));
   CHECKCOBJ(citer2, GTK_IS_TREE_ITER(citer2->object));

   gtk_list_store_move_after(GTK_LIST_STORE(cslist->object), GTK_TREE_ITER(citer1->object), GTK_TREE_ITER(citer2->object));

   return 0;
 err:
   return 1;
}
#endif
#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 6)
/******************************************************************************
* gtk_ListStoreInsertWithValues(list, @iter, position, ncol, val, ....) -->path_string
******************************************************************************/
int
clip_GTK_LISTSTOREINSERTWITHVALUES(ClipMachine * ClipMachineMemory)
{
   C_object *cslist = _fetch_co_arg(ClipMachineMemory);

   ClipVar  *ci = _clip_spar(ClipMachineMemory, 2);

   gint      position = _clip_parni(ClipMachineMemory, 3);

   gint      nValues;

   GtkTreeIter iter;

   C_object *citer;

   C_object *cobj;

   gint      columns[20];

   GValue    values[20];

   gchar    *str;

   int       i, j;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cslist, GTK_IS_LIST_STORE(cslist->object));
   CHECKARG(3, NUMERIC_type_of_ClipVarType);

   position--;
   nValues = _clip_parinfo(ClipMachineMemory, 0) - 3;

   memset(columns, 0, sizeof(columns));
   memset(values, 0, sizeof(values));
   for (i = 0, j = 4; i < nValues; i++, j += 2)
    {
       gint      c = _clip_parni(ClipMachineMemory, j);

       ClipVar  *v = _clip_spar(ClipMachineMemory, j + 1);

       if (c >= 0)
	  columns[i] = c;
       else
	  columns[i] = -1;
       switch ((int) v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
	{
	case NUMERIC_type_of_ClipVarType:
	   g_value_init(&values[i], G_TYPE_FLOAT);
	   g_value_set_float(&values[i], v->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
	   break;
	case CHARACTER_type_of_ClipVarType:
	   str = v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
	   LOCALE_TO_UTF(str);
	   g_value_init(&values[i], G_TYPE_STRING);
	   g_value_set_string(&values[i], str);
	   FREE_TEXT(str);
	   break;
	case LOGICAL_type_of_ClipVarType:
	   g_value_init(&values[i], G_TYPE_BOOLEAN);
	   g_value_set_boolean(&values[i], v->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar);
	   break;
	case DATE_type_of_ClipVarType:
	   g_value_init(&values[i], G_TYPE_STRING);
	   str = _clip_date_to_str(v->ClipLongVar_lv_of_ClipVar.len_of_ClipLongVar, ClipMachineMemory->date_format);
	   g_value_set_string(&values[i], str);
	   break;
	case MAP_type_of_ClipVarType:
	   g_value_init(&values[i], GDK_TYPE_PIXBUF);
	   cobj = _fetch_cobject(ClipMachineMemory, v);
	   g_value_set_object(&values[i], cobj->object);
	   break;
	}
    }

   gtk_list_store_insert_with_values(GTK_LIST_STORE(cslist->object), &iter,
				     position,
				     columns[0], values[0], columns[1],
				     values[1], columns[2], values[2],
				     columns[3], values[3], columns[4],
				     values[4], columns[5], values[5],
				     columns[6], values[6], columns[7],
				     values[7], columns[8], values[8],
				     columns[9], values[9], columns[10],
				     values[10], columns[11], values[11],
				     columns[12], values[12], columns[13],
				     values[13], columns[14], values[14],
				     columns[15], values[15], columns[16],
				     values[16], columns[17], values[17], columns[18], values[18], columns[19], values[19]);

   if (&iter)
    {
       citer = _list_get_cobject(ClipMachineMemory, &iter);
       if (!citer)
	  citer = _register_object(ClipMachineMemory, &iter, GTK_TYPE_TREE_ITER, NULL, NULL);
       if (citer)
	  _clip_mclone(ClipMachineMemory, ci, &citer->obj);
    }

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ListStoreInsertWithValuesV(list, @iter, position, aColumns, aValues, nValues)
******************************************************************************/
int
clip_GTK_LISTSTOREINSERTWITHVALUESV(ClipMachine * ClipMachineMemory)
{
   C_object *cslist = _fetch_co_arg(ClipMachineMemory);

   ClipVar  *ci = _clip_spar(ClipMachineMemory, 2);

   gint      position = _clip_parni(ClipMachineMemory, 3);

   ClipArrVar *ca = (ClipArrVar *) _clip_vptr(_clip_spar(ClipMachineMemory, 4));

   ClipArrVar *cv = (ClipArrVar *) _clip_vptr(_clip_spar(ClipMachineMemory, 5));

   gint      nValues = _clip_parni(ClipMachineMemory, 6);

   GtkTreeIter iter;

   C_object *citer;

   gint     *columns;

   GValue   *values;

   C_object *cobj;

   gchar    *str;

   int       i;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cslist, GTK_IS_LIST_STORE(cslist->object));
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, ARRAY_type_of_ClipVarType);
   CHECKARG(5, ARRAY_type_of_ClipVarType);
   CHECKARG(6, NUMERIC_type_of_ClipVarType);

   position--;
   columns = malloc(nValues * sizeof(int));
   values = malloc(nValues * sizeof(GValue));
   memset(columns, 0, nValues * sizeof(int));
   memset(values, 0, nValues * sizeof(GValue));
   for (i = 0; i < nValues; i++)
    {
       ClipVar  *c = ca->ClipVar_items_of_ClipArrVar + i;

       ClipVar  *v = cv->ClipVar_items_of_ClipArrVar + i;

       if (c->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
	  columns[i] = c->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar - 1;
       switch ((int) v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
	{
	case NUMERIC_type_of_ClipVarType:
	   g_value_init(&values[i], G_TYPE_FLOAT);
	   g_value_set_float(&values[i], v->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
	   break;
	case CHARACTER_type_of_ClipVarType:
	   str = v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
	   LOCALE_TO_UTF(str);
	   g_value_init(&values[i], G_TYPE_STRING);
	   g_value_set_string(&values[i], str);
	   FREE_TEXT(str);
	   break;
	case LOGICAL_type_of_ClipVarType:
	   g_value_init(&values[i], G_TYPE_BOOLEAN);
	   g_value_set_boolean(&values[i], v->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar);
	   break;
	case DATE_type_of_ClipVarType:
	   g_value_init(&values[i], G_TYPE_STRING);
	   str = _clip_date_to_str(v->ClipLongVar_lv_of_ClipVar.len_of_ClipLongVar, ClipMachineMemory->date_format);
	   g_value_set_string(&values[i], str);
	   break;
	case MAP_type_of_ClipVarType:
	   g_value_init(&values[i], GDK_TYPE_PIXBUF);
	   cobj = _fetch_cobject(ClipMachineMemory, v);
	   g_value_set_object(&values[i], cobj->object);
	   break;
	}
    }

   gtk_list_store_insert_with_valuesv(GTK_LIST_STORE(cslist->object), &iter, position, columns, values, nValues);

   if (&iter)
    {
       citer = _list_get_cobject(ClipMachineMemory, &iter);
       if (!citer)
	  citer = _register_object(ClipMachineMemory, &iter, GTK_TYPE_TREE_ITER, NULL, NULL);
       if (citer)
	  _clip_mclone(ClipMachineMemory, ci, &citer->obj);
    }

   return 0;
 err:
   return 1;
}
#endif
