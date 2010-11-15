/*
    Copyright (C) 2003-2005  ITK
    Author  : Elena V. Kornilova <alena@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include <string.h>
#include "hashcode.h"
#include "clip.h"
#include "clip-gtkcfg2.h"

#include <gtk/gtk.h>

#include "clip-gtk2.ch"
#include "clip-gtk2.h"

/*********************** SIGNALS **************************/

/* ListStore has no signals */

/**********************************************************/
CLIP_DLLEXPORT GtkType _gtk_type_list_store() { return GTK_TYPE_LIST_STORE; }

long _clip_type_list_store() { return GTK_OBJECT_LIST_STORE; }

const char * _clip_type_name_list_store()  { return "GTK_OBJECT_LIST_STORE"; }

/* Register boxes in global table */
int
clip_INIT___LISTSTORE(ClipMachine *cm)
{
	_wtype_table_put(_clip_type_list_store,  _clip_type_name_list_store,  _gtk_type_list_store,  NULL, NULL);
	return 0;
}

void
__list_store_destroy(ClipMachine *cm, C_object *colist)
{

	_clip_destroy(cm, _clip_mget(cm, &colist->obj, HASH_UTYPES));
	return;
}

int
static __list_store_set(ClipMachine *cm, GtkTreeIter *iter, gint startDataParam)
{
	C_object *cslist  = _fetch_co_arg(cm);
        ClipArrVar *utypes;
        gint i;

        CHECKCOBJ(cslist, GTK_IS_LIST_STORE(cslist->object));

        utypes = (ClipArrVar *)_clip_vptr(_clip_mget(cm, &cslist->obj, HASH_UTYPES));
	for (i=startDataParam; i<=_clip_parinfo(cm, 0); i+=2)
        {
        	gint column = _clip_parni(cm, i);
        	ClipVar *val;
        	GValue value;
                C_object *cobj;
                gchar *str;
		int j, n;
                double d;

		CHECKARG(i, NUMERIC_t);

                if (column == -1)
                	break;

		column --;
		val = _clip_par(cm, i+1);
		memset(&value, 0, sizeof(value));
		switch ((int)utypes->items[column].n.d)
		{
		case TREE_TYPE_NUMERIC:
			g_value_init(&value, G_TYPE_INT);
			if (val->t.type == NUMERIC_t)
                		g_value_set_int(&value, (int)val->n.d);
                	else
                        {
                		n = _clip_strtod(val->s.str.buf, &j);
                		g_value_set_int(&value, n);
                        }
			break;
		case TREE_TYPE_NUMERIC_FLOAT:
			g_value_init(&value, G_TYPE_FLOAT);
			if (val->t.type == NUMERIC_t)
                		g_value_set_float(&value, val->n.d);
                	else
                        {
                		d = _clip_strtod(val->s.str.buf, &j);
                		g_value_set_float(&value, d);
                        }
			break;
		case TREE_TYPE_STRING:
			str = val->s.str.buf;
			LOCALE_TO_UTF(str);
			g_value_init(&value,  G_TYPE_STRING);
	                g_value_set_string(&value, str);
                        FREE_TEXT(str);
			break;
		case TREE_TYPE_LOGICAL:
			g_value_init(&value,  G_TYPE_BOOLEAN);
	                g_value_set_boolean(&value, val->l.val);
			break;
		case TREE_TYPE_PIXBUF:
			g_value_init(&value,  GDK_TYPE_PIXBUF);
	                cobj = _fetch_cobject(cm, val);
	                g_value_set_object(&value, cobj->object);
			break;
		case TREE_TYPE_DATE:
			g_value_init(&value,  G_TYPE_STRING);
	                if (val->t.type == DATE_t)
	                	str = _clip_date_to_str(val->lv.l, cm->date_format);
                        else
                        	str = _clip_date_to_str(_clip_str_to_date(val->s.str.buf, cm->date_format, cm->epoch), cm->date_format);
	                g_value_set_string(&value, str);
			break;
		}
        	gtk_list_store_set_value(GTK_LIST_STORE(cslist->object), iter, column, &value);
	}
	return 0;
err:
	return 1;
}

int
static __list_store_set_types(ClipMachine * cm, gint ncolumns, GType * types, ClipVar * utypes)
{
	ClipArrVar *cvcol = (ClipArrVar *)_clip_vptr(_clip_spar(cm, 3));
	gint i;
        long d;
	char is_array;
	int dtype;

	is_array = ( _clip_parinfo(cm,3) == ARRAY_t ? 1 : 0 );

	d = ncolumns;
	_clip_array(cm, utypes, 1, &d);
	for (i=0, d = 0; i<ncolumns; i++, d++)
        {
		ClipVar type;
        	if (is_array) { //column type as array item
			memset(&type, 0, sizeof(type));
			type.t.type = NUMERIC_t;
			type.n.d = (int)cvcol->items[i].n.d;
			dtype = (int)type.n.d;
		} else { // column type as parameter
			CHECKARG(i+3, NUMERIC_t);

			memset(&type, 0, sizeof(type));
			type.t.type = NUMERIC_t;
			type.n.d = _clip_parnd(cm, i+3);
			dtype = (int)type.n.d;
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
                _clip_aset(cm, utypes, &type, 1, &d);
                _clip_destroy(cm, &type);
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
clip_GTK_LISTSTORENEW(ClipMachine * cm)
{
	ClipVar * cv   = _clip_spar(cm, 1);
	gint ncolumns  = _clip_parni(cm, 2);
	ClipArrVar *cvcol = (ClipArrVar *)_clip_vptr(_clip_spar(cm, 3));
        ClipVar *utypes;
        GType types[ncolumns];
	int i;
	long d;

        GtkListStore *list;
        C_object *clist;

	CHECKOPT(1,MAP_t);
	CHECKARG(2,NUMERIC_t);
        //CHECKARG2(3, NUMERIC_t, ARRAY_t);

	memset(types, 0, sizeof(types));
        utypes = NEW(ClipVar);

       	if ( _clip_parinfo(cm,3)==ARRAY_t && cvcol->count != ncolumns ) return 1;
	if ( _clip_parinfo(cm,3) != UNDEF_t ) // defined column types as parameters or in array
		__list_store_set_types(cm, ncolumns, types, utypes);
	else  { // if third parameter is empty - use column with G_TYPE_STRING type
		ClipVar type;
		d = ncolumns;
		memset(&type, 0, sizeof(type));
		type.t.type = NUMERIC_t;
		type.n.d = 0;
		_clip_array(cm, utypes, 1, &d);
		for ( i=0, d=0; i<ncolumns; i++, d++ )
                {
			types[i] = G_TYPE_STRING;
                	_clip_aset(cm, utypes, &type, 1, &d);
                }
                _clip_destroy(cm, &type);
	}

	list = gtk_list_store_newv(ncolumns, types);
	if (list)
	{
		clist = _list_get_cobject(cm,list);
		if (!clist) clist = _register_object(cm,list,GTK_TYPE_LIST_STORE,cv,__list_store_destroy);
		if (clist)
		{
			_clip_madd(cm, &clist->obj, HASH_UTYPES, utypes);
			_clip_mclone(cm,RETPTR(cm),&clist->obj);
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
clip_GTK_LISTSTORESETCOLUMNTYPES(ClipMachine * cm)
{
	C_object *cslist = _fetch_co_arg(cm);
	gint    ncolumns = _clip_parni(cm, 2);
        GType types[ncolumns];
        ClipVar *utypes;

	CHECKOPT(1,MAP_t); CHECKCOBJ(cslist, GTK_IS_LIST_STORE(cslist->object));
	CHECKARG(2,NUMERIC_t);

	memset(types, 0, sizeof(types));
        utypes = NEW(ClipVar);

	__list_store_set_types(cm, ncolumns, types, utypes);

        gtk_list_store_set_column_types(GTK_LIST_STORE(cslist->object), ncolumns, types);

	_clip_madd(cm, &cslist->obj, HASH_UTYPES, utypes);

	return 0;
err:
	return 1;
}




/******************************************************************************
* gtk_ListStoreSet(list,path_string,  ncol, val, ...)
******************************************************************************/

int
clip_GTK_LISTSTORESET(ClipMachine * cm)
{
	C_object *cslist = _fetch_co_arg(cm);
        gchar      *path = _clip_parc(cm, 2);
        GtkTreeIter iter;

        CHECKARG2(1, MAP_t, NUMERIC_t);CHECKCOBJ(cslist, GTK_IS_LIST_STORE(cslist->object));
        CHECKARG(2, CHARACTER_t);

        gtk_tree_model_get_iter(GTK_TREE_MODEL(GTK_LIST_STORE(cslist->object)),
        	&iter, gtk_tree_path_new_from_string(path));


	__list_store_set(cm, &iter, 3);

	return 0;
err:
	return 1;
}


/******************************************************************************
* gtk_ListStoreSetValue(list,path_string,  ncol, val)
******************************************************************************/
int
clip_GTK_LISTSTORESETVALUE(ClipMachine * cm)
{
	C_object *cslist = _fetch_co_arg(cm);
        gchar      *path = _clip_parc(cm, 2);
        gint	  column = _clip_parni(cm, 3);
	ClipVar     *val = _clip_par(cm, 4);;
	GtkTreeIter iter;
        ClipArrVar *utypes;
	GValue value;
        C_object *cobj;
	gchar *str;
	int j, n;
	double d;

        CHECKARG2(1, MAP_t, NUMERIC_t);CHECKCOBJ(cslist, GTK_IS_LIST_STORE(cslist->object));
        CHECKARG(2, CHARACTER_t);
        CHECKARG(3, NUMERIC_t);

	gtk_tree_model_get_iter(GTK_TREE_MODEL(GTK_LIST_STORE(cslist->object)),
        	&iter, gtk_tree_path_new_from_string(path));

        utypes = (ClipArrVar *)_clip_vptr(_clip_mget(cm, &cslist->obj, HASH_UTYPES));
	column --;

	memset(&value, 0, sizeof(value));
	switch ((int)utypes->items[column].n.d)
	{
	case TREE_TYPE_NUMERIC:
		g_value_init(&value, G_TYPE_INT);
		if (val->t.type == NUMERIC_t)
			g_value_set_int(&value, (int)val->n.d);
		else
		{
			n = _clip_strtod(val->s.str.buf, &j);
			g_value_set_int(&value, n);
		}
		break;
	case TREE_TYPE_NUMERIC_FLOAT:
		g_value_init(&value, G_TYPE_FLOAT);
		if (val->t.type == NUMERIC_t)
			g_value_set_float(&value, val->n.d);
		else
		{
			d = _clip_strtod(val->s.str.buf, &j);
			g_value_set_float(&value, d);
		}
		break;
	case TREE_TYPE_STRING:
		str = val->s.str.buf;
		LOCALE_TO_UTF(str);
		g_value_init(&value,  G_TYPE_STRING);
		g_value_set_string(&value, str);
		FREE_TEXT(str);
		break;
	case TREE_TYPE_LOGICAL:
		g_value_init(&value,  G_TYPE_BOOLEAN);
		g_value_set_boolean(&value, val->l.val);
		break;
	case TREE_TYPE_PIXBUF:
		g_value_init(&value,  GDK_TYPE_PIXBUF);
	        cobj = _fetch_cobject(cm, val);
	        g_value_set_object(&value, cobj->object);
		break;
	case TREE_TYPE_DATE:
		g_value_init(&value,  G_TYPE_STRING);
		if (val->t.type == DATE_t)
			str = _clip_date_to_str(val->lv.l, cm->date_format);
		else
			str = _clip_date_to_str(_clip_str_to_date(val->s.str.buf, cm->date_format, cm->epoch), cm->date_format);
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
clip_GTK_LISTSTOREREMOVE(ClipMachine * cm)
{

	C_object *cslist = _fetch_co_arg(cm);
        gchar      *path = _clip_parc(cm, 2);
        GtkTreeIter iter;

        CHECKARG2(1, MAP_t, NUMERIC_t); CHECKCOBJ(cslist, GTK_IS_LIST_STORE(cslist->object));
        CHECKARG(2, CHARACTER_t);

        gtk_tree_model_get_iter(GTK_TREE_MODEL(GTK_LIST_STORE(cslist->object)),
        	&iter, gtk_tree_path_new_from_string(path));

	gtk_list_store_remove(GTK_LIST_STORE(cslist->object), &iter);

	return 0;
err:
	return 1;
}

/******************************************************************************
* gtk_ListStoreInsert(list, position, ncol, val, ....) -->path_string
******************************************************************************/
int
clip_GTK_LISTSTOREINSERT(ClipMachine * cm)
{

	C_object *cslist = _fetch_co_arg(cm);
        gint    position = _clip_parni(cm, 2);
        GtkTreeIter iter;
        GtkTreePath *path;
        gchar *path_string;

        CHECKARG2(1, MAP_t, NUMERIC_t); CHECKCOBJ(cslist, GTK_IS_LIST_STORE(cslist->object));
        CHECKARG(2, NUMERIC_t);

	gtk_list_store_insert(GTK_LIST_STORE(cslist->object), &iter, position);

        __list_store_set(cm, &iter, 3);

        path = gtk_tree_model_get_path(GTK_TREE_MODEL(GTK_LIST_STORE(cslist->object)),
        	&iter);

        path_string = gtk_tree_path_to_string(path);

        _clip_retc(cm, path_string);

	g_free(path_string);

	return 0;
err:
	return 1;
}

/******************************************************************************
* gtk_ListStoreInsertBefore(list, sibling_path_string/NULL, ncol, val, ....)-->path_string
******************************************************************************/
int
clip_GTK_LISTSTOREINSERTBEFORE(ClipMachine * cm)
{

	C_object *cslist = _fetch_co_arg(cm);
        gchar   *sibling = _clip_parc(cm, 2);
        GtkTreeIter iter;
        GtkTreeIter siblingiter;
        GtkTreePath *path;
        gchar *path_string;

        CHECKARG2(1, MAP_t, NUMERIC_t); CHECKCOBJ(cslist, GTK_IS_LIST_STORE(cslist->object));
        CHECKOPT(2, CHARACTER_t);

        if (sibling)
        	gtk_tree_model_get_iter(GTK_TREE_MODEL(GTK_LIST_STORE(cslist->object)),
        		&siblingiter, gtk_tree_path_new_from_string(sibling));

	gtk_list_store_insert_before(GTK_LIST_STORE(cslist->object), &iter,
				     (sibling?&siblingiter:NULL));

        __list_store_set(cm, &iter, 3);

        path = gtk_tree_model_get_path(GTK_TREE_MODEL(GTK_LIST_STORE(cslist->object)),
        	&iter);

        path_string = gtk_tree_path_to_string(path);

        _clip_retc(cm, path_string);

	g_free(path_string);

	return 0;
err:
	return 1;
}

/******************************************************************************
* gtk_ListStoreInsertAfter(list, sibling_path_string/NULL, ncol, val, ....)-->path_string
******************************************************************************/
int
clip_GTK_LISTSTOREINSERTAFTER(ClipMachine * cm)
{

	C_object *cslist = _fetch_co_arg(cm);
        gchar   *sibling = _clip_parc(cm, 2);
        GtkTreeIter iter;
        GtkTreeIter siblingiter;
        GtkTreePath *path;
        gchar *path_string;


        CHECKARG2(1, MAP_t, NUMERIC_t); CHECKCOBJ(cslist, GTK_IS_LIST_STORE(cslist->object));
        CHECKOPT(2, CHARACTER_t);

        if (sibling)
        	gtk_tree_model_get_iter(GTK_TREE_MODEL(GTK_LIST_STORE(cslist->object)),
        		&siblingiter, gtk_tree_path_new_from_string(sibling));

	gtk_list_store_insert_after(GTK_LIST_STORE(cslist->object), &iter,
				    (sibling?&siblingiter:NULL));

        __list_store_set(cm, &iter, 3);

        path = gtk_tree_model_get_path(GTK_TREE_MODEL(GTK_LIST_STORE(cslist->object)),
        	&iter);

        path_string = gtk_tree_path_to_string(path);

        _clip_retc(cm, path_string);

	g_free(path_string);

	return 0;
err:
	return 1;
}

/******************************************************************************
* gtk_ListStorePrepend(list, ncol, val, ....)-->path_string
******************************************************************************/

int
clip_GTK_LISTSTOREPREPEND(ClipMachine * cm)
{
	C_object *cslist = _fetch_co_arg(cm);
        GtkTreeIter iter;
        GtkTreePath *path;
        gchar *path_string;

        CHECKCOBJ(cslist, GTK_IS_LIST_STORE(cslist->object));


        gtk_list_store_prepend(GTK_LIST_STORE(cslist->object), &iter);

	__list_store_set(cm, &iter, 2);

        path = gtk_tree_model_get_path(GTK_TREE_MODEL(GTK_LIST_STORE(cslist->object)),
        	&iter);

        path_string = gtk_tree_path_to_string(path);

        _clip_retc(cm, path_string);

	g_free(path_string);

	return 0;
err:
	return 1;
}


/******************************************************************************
* gtk_ListStoreAppend(list, ncol, val, ....)-->path_string
******************************************************************************/

int
clip_GTK_LISTSTOREAPPEND(ClipMachine * cm)
{
	C_object *cslist = _fetch_co_arg(cm);
        GtkTreeIter iter;
        GtkTreePath *path;
        gchar *path_string;

        CHECKCOBJ(cslist, GTK_IS_LIST_STORE(cslist->object));


        gtk_list_store_append(GTK_LIST_STORE(cslist->object), &iter);

	__list_store_set(cm, &iter, 2);

        path = gtk_tree_model_get_path(GTK_TREE_MODEL(GTK_LIST_STORE(cslist->object)),
        	&iter);

        path_string = gtk_tree_path_to_string(path);

        _clip_retc(cm, path_string);


	g_free(path_string);

	return 0;
err:
	return 1;
}


/******************************************************************************
* gtk_ListStoreClear(list)
******************************************************************************/

int
clip_GTK_LISTSTORECLEAR(ClipMachine * cm)
{
	C_object *cslist = _fetch_co_arg(cm);

        CHECKARG2(1, MAP_t, NUMERIC_t); CHECKCOBJ(cslist, GTK_IS_LIST_STORE(cslist->object));


        gtk_list_store_clear(GTK_LIST_STORE(cslist->object));

	return 0;
err:
	return 1;
}

#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 2)

int
clip_GTK_LISTSTOREITERISVALID(ClipMachine * cm)
{
	C_object *cslist = _fetch_co_arg(cm);
        C_object  *citer = _fetch_cobject(cm, _clip_spar(cm, 2));

        CHECKARG2(1, MAP_t, NUMERIC_t); CHECKCOBJ(cslist, GTK_IS_LIST_STORE(cslist->object));
        CHECKCOBJ(citer, GTK_IS_TREE_ITER(citer->object));


        _clip_retl(cm, gtk_list_store_iter_is_valid(GTK_LIST_STORE(cslist->object),
        	GTK_TREE_ITER(citer->object)));

	return 0;
err:
	return 1;
}

int
clip_GTK_LISTSTOREREORDER(ClipMachine * cm)
{
	C_object *cslist = _fetch_co_arg(cm);
        ClipArrVar  *arr = (ClipArrVar *)_clip_vptr(_clip_spar(cm, 2));
        gint           n ;

        CHECKARG2(1, MAP_t, NUMERIC_t); CHECKCOBJ(cslist, GTK_IS_LIST_STORE(cslist->object));
        CHECKARG(2, ARRAY_t);

	n = arr->count;
        if (arr)
        {
        	gint *order, i;

        	order = malloc(n*sizeof(gint));
        	for (i=0; i<n; i++)
                	order[i] = arr->items[i].n.d;
        	gtk_list_store_reorder(GTK_LIST_STORE(cslist->object),
        		order);

		free(order);
        }
	return 0;
err:
	return 1;
}
int
clip_GTK_LISTSTORESWAP(ClipMachine * cm)
{
	C_object *cslist = _fetch_co_arg(cm);
        C_object *citer1 = _fetch_cobject(cm, _clip_spar(cm, 2));
        C_object *citer2 = _fetch_cobject(cm, _clip_spar(cm, 3));

        CHECKARG2(1, MAP_t, NUMERIC_t); CHECKCOBJ(cslist, GTK_IS_LIST_STORE(cslist->object));
        CHECKCOBJ(citer1, GTK_IS_TREE_ITER(citer1->object));
        CHECKCOBJ(citer2, GTK_IS_TREE_ITER(citer2->object));


        gtk_list_store_swap(GTK_LIST_STORE(cslist->object),
        	GTK_TREE_ITER(citer1->object),
        	GTK_TREE_ITER(citer2->object));

	return 0;
err:
	return 1;
}
int
clip_GTK_LISTSTOREMOVEBEFORE(ClipMachine * cm)
{
	C_object *cslist = _fetch_co_arg(cm);
        C_object *citer1 = _fetch_cobject(cm, _clip_spar(cm, 2));
        C_object *citer2 = _fetch_cobject(cm, _clip_spar(cm, 3));

        CHECKARG2(1, MAP_t, NUMERIC_t); CHECKCOBJ(cslist, GTK_IS_LIST_STORE(cslist->object));
        CHECKCOBJ(citer1, GTK_IS_TREE_ITER(citer1->object));
        CHECKCOBJ(citer2, GTK_IS_TREE_ITER(citer2->object));

        gtk_list_store_move_before(GTK_LIST_STORE(cslist->object),
        	GTK_TREE_ITER(citer1->object),
        	GTK_TREE_ITER(citer2->object));

	return 0;
err:
	return 1;
}
int
clip_GTK_LISTSTOREMOVEAFTER(ClipMachine * cm)
{
	C_object *cslist = _fetch_co_arg(cm);
        C_object *citer1 = _fetch_cobject(cm, _clip_spar(cm, 2));
        C_object *citer2 = _fetch_cobject(cm, _clip_spar(cm, 3));

        CHECKARG2(1, MAP_t, NUMERIC_t); CHECKCOBJ(cslist, GTK_IS_LIST_STORE(cslist->object));
        CHECKCOBJ(citer1, GTK_IS_TREE_ITER(citer1->object));
        CHECKCOBJ(citer2, GTK_IS_TREE_ITER(citer2->object));

        gtk_list_store_move_after(GTK_LIST_STORE(cslist->object),
        	GTK_TREE_ITER(citer1->object),
        	GTK_TREE_ITER(citer2->object));

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
clip_GTK_LISTSTOREINSERTWITHVALUES(ClipMachine * cm)
{
	C_object *cslist = _fetch_co_arg(cm);
        ClipVar      *ci = _clip_spar(cm, 2);
        gint    position = _clip_parni(cm, 3);
        gint     nValues ;

	GtkTreeIter iter;
        C_object  *citer;
        C_object   *cobj;
        gint    columns[20];
	GValue values[20];
	gchar *str;
	int i,j;

        CHECKARG2(1, MAP_t, NUMERIC_t);CHECKCOBJ(cslist, GTK_IS_LIST_STORE(cslist->object));
        CHECKARG(3, NUMERIC_t);

	position --;
	nValues = _clip_parinfo(cm, 0)-3;

        memset(columns, 0, sizeof(columns));
        memset(values, 0, sizeof(values));
	for (i=0, j=4; i<nValues; i++, j+=2)
        {
        	gint     c = _clip_parni(cm, j);
                ClipVar *v = _clip_spar(cm, j+1);

        	if (c >= 0)
        		columns[i] = c;
		else
        		columns[i] = -1;
		switch ((int)v->t.type)
		{
		case NUMERIC_t:
			g_value_init(&values[i], G_TYPE_FLOAT);
                	g_value_set_float(&values[i], v->n.d);
			break;
		case CHARACTER_t:
			str = v->s.str.buf;
			LOCALE_TO_UTF(str);
			g_value_init(&values[i],  G_TYPE_STRING);
	                g_value_set_string(&values[i], str);
                        FREE_TEXT(str);
			break;
		case LOGICAL_t:
			g_value_init(&values[i],  G_TYPE_BOOLEAN);
	                g_value_set_boolean(&values[i], v->l.val);
			break;
		case DATE_t:
			g_value_init(&values[i],  G_TYPE_STRING);
	                str = _clip_date_to_str(v->lv.l, cm->date_format);
	                g_value_set_string(&values[i], str);
			break;
		case MAP_t:
			g_value_init(&values[i],  GDK_TYPE_PIXBUF);
	        	cobj = _fetch_cobject(cm, v);
	        	g_value_set_object(&values[i], cobj->object);
			break;
		}
        }

	gtk_list_store_insert_with_values(GTK_LIST_STORE(cslist->object), &iter,
		position,
		columns[0], values[0], columns[1], values[1], columns[2], values[2], columns[3], values[3], columns[4], values[4],
		columns[5], values[5], columns[6], values[6], columns[7], values[7], columns[8], values[8], columns[9], values[9],
		columns[10], values[10], columns[11], values[11], columns[12], values[12], columns[13], values[13], columns[14], values[14],
		columns[15], values[15], columns[16], values[16], columns[17], values[17], columns[18], values[18], columns[19], values[19]
		);

	if (&iter)
        {
        	citer = _list_get_cobject(cm, &iter);
                if (!citer) citer = _register_object(cm, &iter, GTK_TYPE_TREE_ITER, NULL, NULL);
                if (citer) _clip_mclone(cm, ci, &citer->obj);
        }

	return 0;
err:
	return 1;
}

/******************************************************************************
* gtk_ListStoreInsertWithValuesV(list, @iter, position, aColumns, aValues, nValues)
******************************************************************************/
int
clip_GTK_LISTSTOREINSERTWITHVALUESV(ClipMachine * cm)
{
	C_object *cslist = _fetch_co_arg(cm);
        ClipVar      *ci = _clip_spar(cm, 2);
        gint    position = _clip_parni(cm, 3);
        ClipArrVar   *ca = (ClipArrVar *)_clip_vptr(_clip_spar(cm, 4));
        ClipArrVar   *cv = (ClipArrVar *)_clip_vptr(_clip_spar(cm, 5));
        gint     nValues = _clip_parni(cm, 6);

	GtkTreeIter iter;
        C_object  *citer;
        gint    *columns;
	GValue *values;
        C_object *cobj;
	gchar *str;
	int i;

        CHECKARG2(1, MAP_t, NUMERIC_t);CHECKCOBJ(cslist, GTK_IS_LIST_STORE(cslist->object));
        CHECKARG(3, NUMERIC_t);
        CHECKARG(4, ARRAY_t);
        CHECKARG(5, ARRAY_t);
        CHECKARG(6, NUMERIC_t);

	position --;
	columns = malloc(nValues*sizeof(int));
        values   = malloc(nValues*sizeof(GValue));
        memset(columns, 0, nValues*sizeof(int));
        memset(values, 0, nValues*sizeof(GValue));
	for (i=0; i<nValues; i++)
        {
        	ClipVar *c = ca->items+i;
                ClipVar *v = cv->items+i;

        	if (c->t.type == NUMERIC_t)
        		columns[i] = c->n.d - 1;
		switch ((int)v->t.type)
		{
		case NUMERIC_t:
			g_value_init(&values[i], G_TYPE_FLOAT);
                	g_value_set_float(&values[i], v->n.d);
			break;
		case CHARACTER_t:
			str = v->s.str.buf;
			LOCALE_TO_UTF(str);
			g_value_init(&values[i],  G_TYPE_STRING);
	                g_value_set_string(&values[i], str);
                        FREE_TEXT(str);
			break;
		case LOGICAL_t:
			g_value_init(&values[i],  G_TYPE_BOOLEAN);
	                g_value_set_boolean(&values[i], v->l.val);
			break;
		case DATE_t:
			g_value_init(&values[i],  G_TYPE_STRING);
	                str = _clip_date_to_str(v->lv.l, cm->date_format);
	                g_value_set_string(&values[i], str);
			break;
		case MAP_t:
			g_value_init(&values[i],  GDK_TYPE_PIXBUF);
	        	cobj = _fetch_cobject(cm, v);
	        	g_value_set_object(&values[i], cobj->object);
			break;
		}
        }

	gtk_list_store_insert_with_valuesv(GTK_LIST_STORE(cslist->object), &iter,
		position, columns, values, nValues);

	if (&iter)
        {
        	citer = _list_get_cobject(cm, &iter);
                if (!citer) citer = _register_object(cm, &iter, GTK_TYPE_TREE_ITER, NULL, NULL);
                if (citer) _clip_mclone(cm, ci, &citer->obj);
        }

	return 0;
err:
	return 1;
}
#endif

