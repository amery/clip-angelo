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

typedef struct column_format
{
   guint     column;
   gchar    *format;
} column_format;

/*********************** SIGNALS **************************/

/* Signals table */
static    gint
handle_tree_view_column_clicked(GtkTreeViewColumn * column, C_signal * cs)
{
   OBJECTPREPARECV(cs, cv);
   OBJECTINVOKESIGHANDLER(cs, cv);
}

static SignalTable tree_view_column_signals[] = {
   {"clicked", GSF(handle_tree_view_column_clicked), ESF(object_emit_signal),
    GTK_TREE_VIEW_COLUMN_CLICKED_SIGNAL},
   {"", NULL, NULL, 0}
};

/**********************************************************/
CLIP_DLLEXPORT GtkType
_gtk_type_tree_view_column()
{
   return GTK_TYPE_TREE_VIEW_COLUMN;
}

long
_clip_type_tree_view_column()
{
   return GTK_OBJECT_TREE_VIEW_COLUMN;
}

const char *
_clip_type_name_tree_view_column()
{
   return "GTK_OBJECT_TREE_VIEW_COLUMN";
}

/* Register boxes in global table */
int
clip_INIT___TREEVIEWCOLUMN(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_tree_view_column,
		    _clip_type_name_tree_view_column, _gtk_type_tree_view_column, NULL, tree_view_column_signals);
   return 0;
}

int
clip_GTK_TREEVIEWCOLUMNNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   C_object *ccolumn;

   GtkTreeViewColumn *column;

   CHECKOPT(1, MAP_type_of_ClipVarType);

   column = gtk_tree_view_column_new();

   if (column)
    {
       ccolumn = _list_get_cobject(ClipMachineMemory, column);
       if (!ccolumn)
	  ccolumn = _register_object(ClipMachineMemory, column, GTK_TYPE_TREE_VIEW_COLUMN, cv, NULL);
       if (ccolumn)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &ccolumn->obj);
    }
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNNEWWITHATTRIBUTES(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   gchar    *title = _clip_parc(ClipMachineMemory, 2);

   C_object *ccell = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   C_object *ccolumn;

   GtkTreeViewColumn *column;

   gint      i, j;

   gint      attrn[34];

   gchar    *attrs[34];

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);
   CHECKARG2(3, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccell, GTK_IS_CELL_RENDERER(ccell->object));

   memset(attrn, 0, sizeof(attrn));
   memset(attrs, 0, sizeof(attrs));
   for (i = 4, j = 0; i <= _clip_parinfo(ClipMachineMemory, 0); i += 2, j++)
    {
       CHECKARG(i, CHARACTER_type_of_ClipVarType);
       CHECKARG(i + 1, NUMERIC_type_of_ClipVarType);
       attrs[j] = _clip_parc(ClipMachineMemory, i);
       attrn[j] = _clip_parni(ClipMachineMemory, i + 1) - 1;

    }

   LOCALE_TO_UTF(title);
   column = gtk_tree_view_column_new_with_attributes(title,
						     GTK_CELL_RENDERER
						     (ccell->object),
						     attrs[0], attrn[0],
						     attrs[1], attrn[1],
						     attrs[2], attrn[2],
						     attrs[3], attrn[3],
						     attrs[4], attrn[4],
						     attrs[5], attrn[5],
						     attrs[6], attrn[6],
						     attrs[7], attrn[7],
						     attrs[8], attrn[8],
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
						     attrs[32], attrn[32],
						     attrs[33], attrn[33], attrs[34], attrn[34], NULL, NULL);

   FREE_TEXT(title);

   if (column)
    {
       ccolumn = _list_get_cobject(ClipMachineMemory, column);
       if (!ccolumn)
	  ccolumn = _register_object(ClipMachineMemory, column, GTK_TYPE_TREE_VIEW_COLUMN, cv, NULL);
       if (ccolumn)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &ccolumn->obj);
    }
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNPACKSTART(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   C_object *crenderer = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gboolean  expand = _clip_parl(ClipMachineMemory, 3);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(crenderer, GTK_IS_CELL_RENDERER(crenderer->object));
   CHECKARG(3, LOGICAL_type_of_ClipVarType);

   gtk_tree_view_column_pack_start(GTK_TREE_VIEW_COLUMN(ccolumn->object), GTK_CELL_RENDERER(crenderer->object), expand);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNPACKEND(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   C_object *crenderer = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gboolean  expand = _clip_parl(ClipMachineMemory, 3);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(crenderer, GTK_IS_CELL_RENDERER(crenderer->object));
   CHECKARG(3, LOGICAL_type_of_ClipVarType);

   gtk_tree_view_column_pack_end(GTK_TREE_VIEW_COLUMN(ccolumn->object), GTK_CELL_RENDERER(crenderer->object), expand);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNCLEAR(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));

   gtk_tree_view_column_clear(GTK_TREE_VIEW_COLUMN(ccolumn->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNADDATTRIBUTE(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   C_object *crenderer = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gchar    *attribute = _clip_parc(ClipMachineMemory, 3);

   gint      position = _clip_parni(ClipMachineMemory, 4);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(crenderer, GTK_IS_CELL_RENDERER(crenderer->object));
   CHECKARG(3, CHARACTER_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);

   position--;
   gtk_tree_view_column_add_attribute(GTK_TREE_VIEW_COLUMN(ccolumn->object),
				      GTK_CELL_RENDERER(crenderer->object), attribute, position);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNSETATTRIBUTES(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   C_object *ccell = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gint      i, j;

   gint      attrn[34];

   gchar    *attrs[34];

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccell, GTK_IS_CELL_RENDERER(ccell->object));

   memset(attrn, 0, sizeof(attrn));
   memset(attrs, 0, sizeof(attrs));
   for (i = 3, j = 0; i <= _clip_parinfo(ClipMachineMemory, 0); i += 2, j++)
    {
       CHECKARG(i, CHARACTER_type_of_ClipVarType);
       CHECKARG(i + 1, NUMERIC_type_of_ClipVarType);
       attrs[j] = _clip_parc(ClipMachineMemory, i);
       attrn[j] = _clip_parni(ClipMachineMemory, i + 1) - 1;

    }

   i = 3;
   gtk_tree_view_column_set_attributes(GTK_TREE_VIEW_COLUMN(ccolumn->object),
				       GTK_CELL_RENDERER(ccell->object),
				       attrs[0], attrn[0], attrs[1], attrn[1],
				       attrs[2], attrn[2], attrs[3], attrn[3],
				       attrs[4], attrn[4], attrs[5], attrn[5],
				       attrs[6], attrn[6], attrs[7], attrn[7],
				       attrs[8], attrn[8], attrs[9], attrn[9],
				       attrs[10], attrn[10], attrs[11],
				       attrn[11], attrs[12], attrn[12],
				       attrs[13], attrn[13], attrs[14],
				       attrn[14], attrs[15], attrn[15],
				       attrs[16], attrn[16], attrs[17],
				       attrn[17], attrs[18], attrn[18],
				       attrs[19], attrn[19], attrs[20],
				       attrn[20], attrs[21], attrn[21],
				       attrs[22], attrn[22], attrs[23],
				       attrn[23], attrs[24], attrn[24],
				       attrs[25], attrn[25], attrs[26],
				       attrn[26], attrs[27], attrn[27],
				       attrs[28], attrn[28], attrs[29],
				       attrn[29], attrs[30], attrn[30],
				       attrs[31], attrn[31], attrs[32],
				       attrn[32], attrs[33], attrn[33], attrs[34], attrn[34], NULL, NULL);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNCLEARATRIBUTES(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   C_object *ccell = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccell, GTK_IS_CELL_RENDERER(ccell->object));

   gtk_tree_view_column_clear_attributes(GTK_TREE_VIEW_COLUMN(ccolumn->object), GTK_CELL_RENDERER(ccell->object));

   return 0;
 err:
   return 1;
}

/* Sets the spacing field of tree_column, which is the number of pixels */
/* to place between cell renderers packed into it.                      */
int
clip_GTK_TREEVIEWCOLUMNSETSPACING(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   gint      spacing = _clip_parni(ClipMachineMemory, 2);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_tree_view_column_set_spacing(GTK_TREE_VIEW_COLUMN(ccolumn->object), spacing);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNGETSPACING(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   gint      spacing;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));

   spacing = gtk_tree_view_column_get_spacing(GTK_TREE_VIEW_COLUMN(ccolumn->object));

   _clip_retni(ClipMachineMemory, spacing);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNSETVISIBLE(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   gboolean  visible = _clip_parl(ClipMachineMemory, 2);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_tree_view_column_set_visible(GTK_TREE_VIEW_COLUMN(ccolumn->object), visible);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNGETVISIBLE(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   gboolean  visible;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));

   visible = gtk_tree_view_column_get_visible(GTK_TREE_VIEW_COLUMN(ccolumn->object));

   _clip_retl(ClipMachineMemory, visible);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNSETRESIZABLE(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   gboolean  resizable = _clip_parl(ClipMachineMemory, 2);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(ccolumn->object), resizable);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNGETRESIZABLE(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   gboolean  resizable;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));

   resizable = gtk_tree_view_column_get_resizable(GTK_TREE_VIEW_COLUMN(ccolumn->object));

   _clip_retl(ClipMachineMemory, resizable);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNSETSIZING(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   gint      s = _clip_parni(ClipMachineMemory, 2);

   GtkTreeViewColumnSizing sizing = 0;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   switch (s)
    {
    case CLIP_GTK_TREE_VIEW_COLUMN_GROW_ONLY:
       sizing = GTK_TREE_VIEW_COLUMN_GROW_ONLY;
       break;
    case CLIP_GTK_TREE_VIEW_COLUMN_AUTOSIZE:
       sizing = GTK_TREE_VIEW_COLUMN_AUTOSIZE;
       break;
    case CLIP_GTK_TREE_VIEW_COLUMN_FIXED:
       sizing = GTK_TREE_VIEW_COLUMN_FIXED;
       break;
    }
   gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(ccolumn->object), sizing);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNGETSIZING(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   GtkTreeViewColumnSizing sizing;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));

   sizing = gtk_tree_view_column_get_sizing(GTK_TREE_VIEW_COLUMN(ccolumn->object));
   switch (sizing)
    {
    case GTK_TREE_VIEW_COLUMN_GROW_ONLY:
       _clip_retni(ClipMachineMemory, CLIP_GTK_TREE_VIEW_COLUMN_GROW_ONLY);
       break;
    case GTK_TREE_VIEW_COLUMN_AUTOSIZE:
       _clip_retni(ClipMachineMemory, CLIP_GTK_TREE_VIEW_COLUMN_AUTOSIZE);
       break;
    case GTK_TREE_VIEW_COLUMN_FIXED:
       _clip_retni(ClipMachineMemory, CLIP_GTK_TREE_VIEW_COLUMN_FIXED);
       break;
    }

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNGETWIDTH(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));

   _clip_retni(ClipMachineMemory, gtk_tree_view_column_get_width(GTK_TREE_VIEW_COLUMN(ccolumn->object)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNGETFIXEDWIDTH(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));

   _clip_retni(ClipMachineMemory, gtk_tree_view_column_get_fixed_width(GTK_TREE_VIEW_COLUMN(ccolumn->object)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNSETMINWIDTH(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   gint      min_width = _clip_parni(ClipMachineMemory, 2);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_tree_view_column_set_min_width(GTK_TREE_VIEW_COLUMN(ccolumn->object), min_width);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNGETMINWIDTH(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   gint      min_width;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));

   min_width = gtk_tree_view_column_get_min_width(GTK_TREE_VIEW_COLUMN(ccolumn->object));

   _clip_retni(ClipMachineMemory, min_width);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNSETMAXWIDTH(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   gint      max_width = _clip_parni(ClipMachineMemory, 2);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_tree_view_column_set_max_width(GTK_TREE_VIEW_COLUMN(ccolumn->object), max_width);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNGETMAXWIDTH(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   gint      max_width;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));

   max_width = gtk_tree_view_column_get_max_width(GTK_TREE_VIEW_COLUMN(ccolumn->object));

   _clip_retni(ClipMachineMemory, max_width);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNCLICKED(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));

   gtk_tree_view_column_clicked(GTK_TREE_VIEW_COLUMN(ccolumn->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNSETTITLE(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   gchar    *title = _clip_parc(ClipMachineMemory, 2);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(title);
   gtk_tree_view_column_set_title(GTK_TREE_VIEW_COLUMN(ccolumn->object), title);

   FREE_TEXT(title);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNGETTITLE(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   gchar    *title;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));

   title = (gchar *) gtk_tree_view_column_get_title(GTK_TREE_VIEW_COLUMN(ccolumn->object));

   LOCALE_FROM_UTF(title);
   _clip_retc(ClipMachineMemory, title);
   FREE_TEXT(title);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNSETCLICKABLE(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   gboolean  clickable = _clip_parl(ClipMachineMemory, 2);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_tree_view_column_set_clickable(GTK_TREE_VIEW_COLUMN(ccolumn->object), clickable);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNGETCLICKABLE(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   gboolean  clickable;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));

   clickable = gtk_tree_view_column_get_clickable(GTK_TREE_VIEW_COLUMN(ccolumn->object));

   _clip_retl(ClipMachineMemory, clickable);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNSETWIDGET(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);

   gtk_tree_view_column_set_widget(GTK_TREE_VIEW_COLUMN(ccolumn->object), GTK_WIDGET(cwid->widget));

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNGETWIDGET(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   GtkWidget *wid;

   C_widget *cwid;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));

   wid = gtk_tree_view_column_get_widget(GTK_TREE_VIEW_COLUMN(ccolumn->object));

   cwid = _register_widget(ClipMachineMemory, wid, NULL);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNSETALIGNMENT(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   gfloat    alignment = _clip_parnd(ClipMachineMemory, 2);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_tree_view_column_set_alignment(GTK_TREE_VIEW_COLUMN(ccolumn->object), alignment);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNGETALIGNMENT(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   gfloat    alignment;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));

   alignment = gtk_tree_view_column_get_alignment(GTK_TREE_VIEW_COLUMN(ccolumn->object));

   _clip_retnd(ClipMachineMemory, alignment);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNSETREORDERABLE(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   gboolean  reorderable = _clip_parl(ClipMachineMemory, 2);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_tree_view_column_set_reorderable(GTK_TREE_VIEW_COLUMN(ccolumn->object), reorderable);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNGETREORDERABLE(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   gboolean  reorderable;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));

   reorderable = gtk_tree_view_column_get_reorderable(GTK_TREE_VIEW_COLUMN(ccolumn->object));

   _clip_retl(ClipMachineMemory, reorderable);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNSETSORTCOLUMNID(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   gint      sort_column_id = _clip_parni(ClipMachineMemory, 2);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_tree_view_column_set_sort_column_id(GTK_TREE_VIEW_COLUMN(ccolumn->object), sort_column_id - 1);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNGETSORTCOLUMNID(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   gint      sort_column_id;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));

   sort_column_id = gtk_tree_view_column_get_sort_column_id(GTK_TREE_VIEW_COLUMN(ccolumn->object));

   _clip_retni(ClipMachineMemory, sort_column_id);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNSETSORTINDICATOR(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   gboolean  setting = _clip_parl(ClipMachineMemory, 2);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_tree_view_column_set_sort_indicator(GTK_TREE_VIEW_COLUMN(ccolumn->object), setting);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNGETSORTINDICATOR(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   gboolean  setting;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));

   setting = gtk_tree_view_column_get_sort_indicator(GTK_TREE_VIEW_COLUMN(ccolumn->object));

   _clip_retl(ClipMachineMemory, setting);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNSETSORTORDER(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   gint      s = _clip_parni(ClipMachineMemory, 2);

   GtkSortType type = GTK_SORT_ASCENDING;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   if (s == CLIP_GTK_SORT_ASCENDING)
      type = GTK_SORT_ASCENDING;
   else if (s == CLIP_GTK_SORT_DESCENDING)
      type = GTK_SORT_DESCENDING;

   gtk_tree_view_column_set_sort_order(GTK_TREE_VIEW_COLUMN(ccolumn->object), type);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNGETSORTORDER(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   GtkSortType type;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));

   type = gtk_tree_view_column_get_sizing(GTK_TREE_VIEW_COLUMN(ccolumn->object));
   switch (type)
    {
    case GTK_SORT_ASCENDING:
       _clip_retni(ClipMachineMemory, CLIP_GTK_SORT_ASCENDING);
       break;
    case GTK_SORT_DESCENDING:
       _clip_retni(ClipMachineMemory, CLIP_GTK_SORT_DESCENDING);
       break;
    }

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNCELLGETSIZE(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   GdkRectangle *rect = 0;

   gint      x_offset;

   gint      y_offset;

   gint      width;

   gint      height;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));

   gtk_tree_view_column_cell_get_size(GTK_TREE_VIEW_COLUMN(ccolumn->object), rect, &x_offset, &y_offset, &width, &height);

   _clip_storni(ClipMachineMemory, x_offset, 2, 0);
   _clip_storni(ClipMachineMemory, y_offset, 3, 0);
   _clip_storni(ClipMachineMemory, width, 4, 0);
   _clip_storni(ClipMachineMemory, height, 5, 0);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNCELLISVISIBLE(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   gboolean  visible;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));

   visible = gtk_tree_view_column_cell_is_visible(GTK_TREE_VIEW_COLUMN(ccolumn->object));

   _clip_retl(ClipMachineMemory, visible);
   return 0;
 err:
   return 1;
}

#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 2)
int
clip_GTK_TREEVIEWCOLUMNFOCUSCELL(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   C_object *ccell = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCOBJ(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));
   CHECKCOBJ(ccell, GTK_IS_CELL_RENDERER(ccell->object));

   gtk_tree_view_column_focus_cell(GTK_TREE_VIEW_COLUMN(ccolumn->object), GTK_CELL_RENDERER(ccell->object));

   return 0;
 err:
   return 1;
}
#endif

#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 4)
int
clip_GTK_TREEVIEWCOLUMNSETEXPAND(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   gboolean  set = _clip_parl(ClipMachineMemory, 2);

   CHECKCOBJ(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));

   gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(ccolumn->object), set);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TREEVIEWCOLUMNGETEXPAND(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   CHECKCOBJ(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));

   _clip_retl(ClipMachineMemory, gtk_tree_view_column_get_expand(GTK_TREE_VIEW_COLUMN(ccolumn->object)));

   return 0;
 err:
   return 1;
}

void
format_data_func(GtkTreeViewColumn * col, GtkCellRenderer * renderer, GtkTreeModel * model, GtkTreeIter * iter, gpointer u_data)
{
   gfloat    n;

   gchar     buf[20];

   column_format *data = (column_format *) u_data;

   gtk_tree_model_get(model, iter, data->column, &n, -1);
   g_snprintf(buf, sizeof(buf), data->format, n);
   g_object_set(renderer, "text", buf, NULL);
}

int
clip_GTK_TREEVIEWCOLUMNSETFORMAT(ClipMachine * ClipMachineMemory)
{
   C_object *ccolumn = _fetch_co_arg(ClipMachineMemory);

   C_object *ccell = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gint      num = _clip_parni(ClipMachineMemory, 3);

   gchar    *format = _clip_parc(ClipMachineMemory, 4);

   column_format *user_data;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccolumn, GTK_IS_TREE_VIEW_COLUMN(ccolumn->object));
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccell, GTK_IS_CELL_RENDERER(ccell->object));
   CHECKARG(3, NUMERIC_type_of_ClipVarType) CHECKARG(4, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(format);
   user_data = (column_format *) malloc(sizeof(column_format));
   user_data->column = num - 1;
   user_data->format = g_strdup(format);

   gtk_tree_view_column_set_cell_data_func(GTK_TREE_VIEW_COLUMN
					   (ccolumn->object),
					   GTK_CELL_RENDERER(ccell->object), format_data_func, (gpointer) user_data, NULL);

   return 0;
 err:
   return 1;

}

#endif
