/*
    Copyright (C) 2002-2004  ITK
    Author  : Alexey M. Tkachenko <alexey@itk.ru>
    	      Elena V. Kornilova <alena@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg2.h"

#include <gtk/gtk.h>

#include "ci_clip-gtk2.ch"
#include "ci_clip-gtk2.h"

/*********************** SIGNALS **************************/

/***             Table has no own signals               ***/

/**********************************************************/
CLIP_DLLEXPORT GtkType
_gtk_type_table()
{
   return GTK_TYPE_TABLE;
}

long
_clip_type_table()
{
   return GTK_WIDGET_TABLE;
}

const char *
_clip_type_name_table()
{
   return "GTK_OBJECT_TABLE";
}

/* Register table in global table */
int
clip_INIT___TABLE(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_table, _clip_type_name_table, _gtk_type_table, _gtk_type_container, NULL);
   return 0;
}

/**** ------------------ ****/
/**** TABLE constructor ****/
/* Used to create a new table widget. An initial size must be given by
 * specifying how many rows and columns the table should have, although
 * this can be changed later with gtk_table_resize(). */
int
clip_GTK_TABLENEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   guint     rows = _clip_parni(ClipMachineMemory, 2);

   guint     cols = _clip_parni(ClipMachineMemory, 3);

   gboolean  homogeneous = BOOL_OPTION(ClipMachineMemory, 4, FALSE);

   GtkWidget *wid = NULL;

   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, LOGICAL_type_of_ClipVarType);
   wid = gtk_table_new(rows, cols, homogeneous);
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

/* If you need to change a table's size after it has been created,
 * this function allows you to do so. */
int
clip_GTK_TABLERESIZE(ClipMachine * ClipMachineMemory)
{
   C_widget *ctbl = _fetch_cw_arg(ClipMachineMemory);

   guint     rows = _clip_parni(ClipMachineMemory, 2);

   guint     cols = _clip_parni(ClipMachineMemory, 3);

   CHECKCWID(ctbl, GTK_IS_TABLE);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   gtk_table_resize(GTK_TABLE(ctbl->widget), rows, cols);
   return 0;
 err:
   return 1;
}

/* Adds a widget to a table. The number of 'cells' that a widget
 * will occupy is specified by left_attach, right_attach, top_attach
 * and bottom_attach. These each represent the leftmost, rightmost,
 * uppermost and lowest column and row numbers of the table.
 * (Columns and rows are indexed from 1). */
int
clip_GTK_TABLEATTACH(ClipMachine * ClipMachineMemory)
{
   C_widget *ctbl = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   guint     left_attach = INT_OPTION(ClipMachineMemory, 3, 1);

   guint     right_attach = INT_OPTION(ClipMachineMemory, 4, 1);

   guint     top_attach = INT_OPTION(ClipMachineMemory, 5, 1);

   guint     bottom_attach = INT_OPTION(ClipMachineMemory, 6, 1);

   GtkAttachOptions xoptions = _clip_parni(ClipMachineMemory, 7);

   GtkAttachOptions yoptions = _clip_parni(ClipMachineMemory, 8);

   guint     xpadding = _clip_parni(ClipMachineMemory, 9);

   guint     ypadding = _clip_parni(ClipMachineMemory, 10);

   CHECKCWID(ctbl, GTK_IS_TABLE);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, NUMERIC_type_of_ClipVarType);
   CHECKOPT(5, NUMERIC_type_of_ClipVarType);
   CHECKOPT(6, NUMERIC_type_of_ClipVarType);
   CHECKOPT(7, NUMERIC_type_of_ClipVarType);
   CHECKOPT(8, NUMERIC_type_of_ClipVarType);
   CHECKOPT(9, NUMERIC_type_of_ClipVarType);
   CHECKOPT(10, NUMERIC_type_of_ClipVarType);
   gtk_table_attach(GTK_TABLE(ctbl->widget), cwid->widget, left_attach - 1,
		    right_attach - 1, top_attach - 1, bottom_attach - 1, xoptions, yoptions, xpadding, ypadding);
   return 0;
 err:
   return 1;
}

/* As there are many options associated with gtk_table_attach(), this
 * convenience function provides the programmer with a means to add
 * children to a table with identical padding and expansion options. */
int
clip_GTK_TABLEATTACHDEFAULTS(ClipMachine * ClipMachineMemory)
{
   C_widget *ctbl = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   guint     left_attach = INT_OPTION(ClipMachineMemory, 3, 1);

   guint     right_attach = INT_OPTION(ClipMachineMemory, 4, 1);

   guint     top_attach = INT_OPTION(ClipMachineMemory, 5, 1);

   guint     bottom_attach = INT_OPTION(ClipMachineMemory, 6, 1);

   CHECKCWID(ctbl, GTK_IS_TABLE);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, NUMERIC_type_of_ClipVarType);
   CHECKOPT(5, NUMERIC_type_of_ClipVarType);
   CHECKOPT(6, NUMERIC_type_of_ClipVarType);
   gtk_table_attach_defaults(GTK_TABLE(ctbl->widget), cwid->widget,
			     left_attach - 1, right_attach - 1, top_attach - 1, bottom_attach - 1);
   return 0;
 err:
   return 1;
}

/* changes the space between a given table row and its surrounding rows. */
int
clip_GTK_TABLESETROWSPACING(ClipMachine * ClipMachineMemory)
{
   C_widget *ctbl = _fetch_cw_arg(ClipMachineMemory);

   guint     row = INT_OPTION(ClipMachineMemory, 2, 1);

   guint     spacing = _clip_parni(ClipMachineMemory, 3);

   CHECKCWID(ctbl, GTK_IS_TABLE);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   gtk_table_set_row_spacing(GTK_TABLE(ctbl->widget), row - 1, spacing);
   return 0;
 err:
   return 1;
}

/* alters the amount of space between a given table column and the adjacent columns. */
int
clip_GTK_TABLESETCOLSPACING(ClipMachine * ClipMachineMemory)
{
   C_widget *ctbl = _fetch_cw_arg(ClipMachineMemory);

   guint     col = INT_OPTION(ClipMachineMemory, 2, 1);

   guint     spacing = _clip_parni(ClipMachineMemory, 3);

   CHECKCWID(ctbl, GTK_IS_TABLE);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   gtk_table_set_col_spacing(GTK_TABLE(ctbl->widget), col - 1, spacing);
   return 0;
 err:
   return 1;
}

/* sets the space between every row in table equal to spacing. */
int
clip_GTK_TABLESETROWSPACINGS(ClipMachine * ClipMachineMemory)
{
   C_widget *ctbl = _fetch_cw_arg(ClipMachineMemory);

   guint     spacing = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(ctbl, GTK_IS_TABLE);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_table_set_row_spacings(GTK_TABLE(ctbl->widget), spacing);
   return 0;
 err:
   return 1;
}

/* sets the space between every column in table equal to spacing. */
int
clip_GTK_TABLESETCOLSPACINGS(ClipMachine * ClipMachineMemory)
{
   C_widget *ctbl = _fetch_cw_arg(ClipMachineMemory);

   guint     spacing = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(ctbl, GTK_IS_TABLE);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_table_set_col_spacings(GTK_TABLE(ctbl->widget), spacing);
   return 0;
 err:
   return 1;
}

/* changes the homogenous property of table cells.
 * Ie. whether all cells are an equal size or not. */
int
clip_GTK_TABLESETHOMOGENEOUS(ClipMachine * ClipMachineMemory)
{
   C_widget *ctbl = _fetch_cw_arg(ClipMachineMemory);

   gboolean  homogeneous = BOOL_OPTION(ClipMachineMemory, 2, FALSE);

   CHECKCWID(ctbl, GTK_IS_TABLE);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   gtk_table_set_homogeneous(GTK_TABLE(ctbl->widget), homogeneous);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TABLEGETDEFAULTROWSPACING(ClipMachine * ClipMachineMemory)
{
   C_widget *ctbl = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(ctbl, GTK_IS_TABLE);
   _clip_retni(ClipMachineMemory, gtk_table_get_default_row_spacing(GTK_TABLE(ctbl->widget)));
   return 0;
 err:
   return 1;
}

int
clip_GTK_TABLEGETHOMOGENEOUS(ClipMachine * ClipMachineMemory)
{
   C_widget *ctbl = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(ctbl, GTK_IS_TABLE);
   _clip_retl(ClipMachineMemory, GTK_TABLE(ctbl->widget)->homogeneous);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TABLEGETROWSPACING(ClipMachine * ClipMachineMemory)
{
   C_widget *ctbl = _fetch_cw_arg(ClipMachineMemory);

   guint     row = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(ctbl, GTK_IS_TABLE);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   _clip_retni(ClipMachineMemory, gtk_table_get_row_spacing(GTK_TABLE(ctbl->widget), row));
   return 0;
 err:
   return 1;
}

int
clip_GTK_TABLEGETCOLSPACING(ClipMachine * ClipMachineMemory)
{
   C_widget *ctbl = _fetch_cw_arg(ClipMachineMemory);

   guint     column = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(ctbl, GTK_IS_TABLE);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   _clip_retni(ClipMachineMemory, gtk_table_get_col_spacing(GTK_TABLE(ctbl->widget), column));
   return 0;
 err:
   return 1;
}

int
clip_GTK_TABLEGETDEFAULTCOLSPACING(ClipMachine * ClipMachineMemory)
{
   C_widget *ctbl = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(ctbl, GTK_IS_TABLE);
   _clip_retni(ClipMachineMemory, gtk_table_get_default_col_spacing(GTK_TABLE(ctbl->widget)));
   return 0;
 err:
   return 1;
}

int
clip_GTK_TABLESETNROWS(ClipMachine * ClipMachineMemory)
{
   C_widget *ctbl = _fetch_cw_arg(ClipMachineMemory);

   guint     nrows = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(ctbl, GTK_IS_TABLE);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_widget_set(ctbl->widget, "n-rows", nrows, NULL);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TABLEGETNROWS(ClipMachine * ClipMachineMemory)
{
   C_widget *ctbl = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(ctbl, GTK_IS_TABLE);
   _clip_retl(ClipMachineMemory, GTK_TABLE(ctbl->widget)->nrows);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TABLESETNCOLUMNS(ClipMachine * ClipMachineMemory)
{
   C_widget *ctbl = _fetch_cw_arg(ClipMachineMemory);

   guint     ncols = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(ctbl, GTK_IS_TABLE);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_widget_set(ctbl->widget, "n-columns", ncols, NULL);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TABLEGETNCOLUMNS(ClipMachine * ClipMachineMemory)
{
   C_widget *ctbl = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(ctbl, GTK_IS_TABLE);
   _clip_retl(ClipMachineMemory, GTK_TABLE(ctbl->widget)->ncols);
   return 0;
 err:
   return 1;
}
