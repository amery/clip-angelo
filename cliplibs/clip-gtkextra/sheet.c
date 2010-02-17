/*
    Copyright (C) 2002  ITK
    Author  : Alexey M. Tkachenko <alexey@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkextracfg.h"

#include <gtkextra/gtkextra.h>
#include <string.h>

#include "ci_clip-gtk.ch"
#include "ci_clip-gtk.h"

#include "ci_clip-gtkextra.ch"
#include "ci_clip-gtkextra.h"

/************ Forward declarations ************************/
static void _map_to_sheet_range(ClipMachine * ClipMachineMemory, ClipVar * mrange, GtkSheetRange * range);
static void _sheet_range_to_map(ClipMachine * ClipMachineMemory, GtkSheetRange * range, ClipVar * mrange);
/**********************************************************/
/* Signal handlers */
static gint
handle_select_row_signal(GtkWidget * widget, gint row, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_ROW, row + 1);
   INVOKESIGHANDLER(widget, cs, cv);
}

static gint
handle_select_column_signal(GtkWidget * widget, gint column, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_COL, column + 1);
   INVOKESIGHANDLER(widget, cs, cv);
}

static gint
handle_select_range_signal(GtkWidget * widget, GtkSheetRange * range, C_signal * cs)
{
   ClipVar crange;
   PREPARECV(cs, cv);
   memset(&crange, 0, sizeof(ClipVar));
   _clip_map(cs->cw->cmachine, &crange);
   _sheet_range_to_map(cs->cw->cmachine, range, &crange);
   _clip_madd(cs->cw->cmachine, &cv, HASH_RANGE, &crange);
   INVOKESIGHANDLER(widget, cs, cv);
}

static gint
handle_clip_range_signal(GtkWidget * widget, GtkSheetRange * range, C_signal * cs)
{
   ClipVar crange;
   PREPARECV(cs, cv);
   memset(&crange, 0, sizeof(ClipVar));
   _clip_map(cs->cw->cmachine, &crange);
   _sheet_range_to_map(cs->cw->cmachine, range, &crange);
   _clip_madd(cs->cw->cmachine, &cv, HASH_RANGE, &crange);
   INVOKESIGHANDLER(widget, cs, cv);
}

static gint
handle_resize_range_signal(GtkWidget * widget, GtkSheetRange * drag_range, GtkSheetRange * range, C_signal * cs)
{
   ClipVar cvdr, cvr;
   PREPARECV(cs, cv);
   memset(&cvdr, 0, sizeof(ClipVar));
   memset(&cvr, 0, sizeof(ClipVar));
   _clip_map(cs->cw->cmachine, &cvdr);
   _clip_map(cs->cw->cmachine, &cvr);
   _sheet_range_to_map(cs->cw->cmachine, drag_range, &cvdr);
   _sheet_range_to_map(cs->cw->cmachine, range, &cvr);
   _clip_madd(cs->cw->cmachine, &cv, HASH_OLDRANGE, &cvdr);
   _clip_madd(cs->cw->cmachine, &cv, HASH_NEWRANGE, &cvr);
   INVOKESIGHANDLER(widget, cs, cv);
}

static gint
handle_move_range_signal(GtkWidget * widget, GtkSheetRange * drag_range, GtkSheetRange * range, C_signal * cs)
{
   ClipVar cvdr, cvr;
   PREPARECV(cs, cv);
   memset(&cvdr, 0, sizeof(ClipVar));
   memset(&cvr, 0, sizeof(ClipVar));
   _clip_map(cs->cw->cmachine, &cvdr);
   _clip_map(cs->cw->cmachine, &cvr);
   _sheet_range_to_map(cs->cw->cmachine, drag_range, &cvdr);
   _sheet_range_to_map(cs->cw->cmachine, range, &cvr);
   _clip_madd(cs->cw->cmachine, &cv, HASH_OLDRANGE, &cvdr);
   _clip_madd(cs->cw->cmachine, &cv, HASH_NEWRANGE, &cvr);
   INVOKESIGHANDLER(widget, cs, cv);
}

static gint
handle_traverse_signal(GtkWidget * widget, gint row, gint col, gint * new_row, gint * new_col, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_ROW, row + 1);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_COL, col + 1);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_NEWROW, (*new_row) + 1);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_NEWCOL, (*new_col) + 1);
   INVOKESIGHANDLER(widget, cs, cv);
}

static gint
handle_deactivate_signal(GtkWidget * widget, gint row, gint col, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_ROW, row + 1);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_COL, col + 1);
   INVOKESIGHANDLER(widget, cs, cv);
}

static gint
handle_activate_signal(GtkWidget * widget, gint row, gint col, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_ROW, row + 1);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_COL, col + 1);
   INVOKESIGHANDLER(widget, cs, cv);
}

static gint
handle_set_cell_signal(GtkWidget * widget, gint row, gint col, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_ROW, row + 1);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_COL, col + 1);
   INVOKESIGHANDLER(widget, cs, cv);
}

static gint
handle_clear_cell_signal(GtkWidget * widget, gint row, gint col, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_ROW, row + 1);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_COLUMN, col + 1);
   INVOKESIGHANDLER(widget, cs, cv);
}

static gint
handle_changed_signal(GtkWidget * widget, gint row, gint col, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_ROW, row + 1);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_COL, col + 1);
   INVOKESIGHANDLER(widget, cs, cv);
}

static gint
handle_new_col_width_signal(GtkWidget * widget, gint column, guint width, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_COL, column + 1);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_WIDTH, width);
   INVOKESIGHANDLER(widget, cs, cv);
}

static gint
handle_new_row_height_signal(GtkWidget * widget, gint row, guint height, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_ROW, row + 1);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_HEIGHT, height);
   INVOKESIGHANDLER(widget, cs, cv);
}

/* Signals table */
static SignalTable sheet_signals[] = {
   {"select-row", GSF(handle_select_row_signal), ESF(object_emit_signal), GTK_SELECT_ROW_SIGNAL},
   {"select-column", GSF(handle_select_column_signal), ESF(object_emit_signal), GTK_SELECT_COLUMN_SIGNAL},
   {"select-range", GSF(handle_select_range_signal), ESF(object_emit_signal), GTK_SELECT_RANGE_SIGNAL},
   {"clip-range", GSF(handle_clip_range_signal), ESF(object_emit_signal), GTK_CLIP_RANGE_SIGNAL},
   {"resize-range", GSF(handle_resize_range_signal), ESF(object_emit_signal), GTK_RESIZE_RANGE_SIGNAL},
   {"move-range", GSF(handle_move_range_signal), ESF(object_emit_signal), GTK_MOVE_RANGE_SIGNAL},
   {"traverse", GSF(handle_traverse_signal), ESF(object_emit_signal), GTK_TRAVERSE_SIGNAL},
   {"deactivate", GSF(handle_deactivate_signal), ESF(object_emit_signal), GTK_DEACTIVATE_SIGNAL},
   {"activate", GSF(handle_activate_signal), ESF(object_emit_signal), GTK_ACTIVATE_SIGNAL},
   {"set-cell", GSF(handle_set_cell_signal), ESF(object_emit_signal), GTK_SET_CELL_SIGNAL},
   {"clear-cell", GSF(handle_clear_cell_signal), ESF(object_emit_signal), GTK_CLEAR_CELL_SIGNAL},
   {"changed", GSF(handle_changed_signal), ESF(object_emit_signal), GTK_CHANGED_SIGNAL},
   {"new-col-width", GSF(handle_new_col_width_signal), ESF(object_emit_signal), GTK_NEW_COL_WIDTH_SIGNAL},
   {"new-row-height", GSF(handle_new_row_height_signal), ESF(object_emit_signal), GTK_NEW_ROW_HEIGHT_SIGNAL},
   {"", NULL, NULL, 0}
};

/**********************************************************/

/* Register Sheet in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_sheet()
{
   return GTK_TYPE_SHEET;
}

long
_clip_type_sheet()
{
   return GTK_WIDGET_SHEET;
}

const char *
_clip_type_name_sheet()
{
   return "GTK_WIDGET_SHEET";
}

int
clip_INIT___SHEET(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_sheet, _clip_type_name_sheet, _gtk_type_sheet, _gtk_type_container, sheet_signals);
   return 0;
}

static void
_map_to_sheet_range(ClipMachine * ClipMachineMemory, ClipVar * mrange, GtkSheetRange * range)
{
   if (mrange && mrange->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType && range)
      {
	 double d_row0, d_col0, d_rowi, d_coli;
	 _clip_mgetn(ClipMachineMemory, mrange, HASH_ROW0, &d_row0);
	 range->row0 = d_row0 - 1;
	 _clip_mgetn(ClipMachineMemory, mrange, HASH_COL0, &d_col0);
	 range->col0 = d_col0 - 1;
	 _clip_mgetn(ClipMachineMemory, mrange, HASH_ROWI, &d_rowi);
	 range->rowi = d_rowi - 1;
	 _clip_mgetn(ClipMachineMemory, mrange, HASH_COLI, &d_coli);
	 range->coli = d_coli - 1;
      }
}

static void
_sheet_range_to_map(ClipMachine * ClipMachineMemory, GtkSheetRange * range, ClipVar * mrange)
{
   if (mrange && mrange->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType && range)
      {
	 _clip_mputn(ClipMachineMemory, mrange, HASH_ROW0, range->row0 + 1);
	 _clip_mputn(ClipMachineMemory, mrange, HASH_COL0, range->col0 + 1);
	 _clip_mputn(ClipMachineMemory, mrange, HASH_ROWI, range->rowi + 1);
	 _clip_mputn(ClipMachineMemory, mrange, HASH_COLI, range->coli + 1);
      }
}

/**** Sheet constructor ****/
int
clip_GTK_SHEETNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   gint ncolumns = INT_OPTION(ClipMachineMemory, 2, 0);
   gint nrows = INT_OPTION(ClipMachineMemory, 3, 0);
   gchar *title = CHAR_OPTION(ClipMachineMemory, 4, "");
   gint entry_type = _clip_parni(ClipMachineMemory, 5);
   GtkWidget *wid = NULL;
   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, CHARACTER_type_of_ClipVarType);
   CHECKOPT(5, NUMERIC_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 5) == UNDEF_type_of_ClipVarType)
      {
	 wid = gtk_sheet_new(ncolumns, nrows, title);
//              wid = gtk_sheet_new_with_custom_entry(ncolumns,nrows,title,GTK_TYPE_ENTRY);
      }
   else
      {
	 WTypeTable *wt_item = _wtype_table_get_by_clip_type(entry_type);
	 GtkType e_type = wt_item ? wt_item->ftype() : GTK_TYPE_INVALID;
	 wid = gtk_sheet_new_with_custom_entry(ncolumns, nrows, title, e_type);
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

/**** Sheet browser constructor. It cells can not be edited ****/
int
clip_GTK_SHEETNEWBROWSER(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   gint ncolumns = INT_OPTION(ClipMachineMemory, 2, 0);
   gint nrows = INT_OPTION(ClipMachineMemory, 3, 0);
   gchar *title = CHAR_OPTION(ClipMachineMemory, 4, "");
   GtkWidget *wid = NULL;
   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, CHARACTER_type_of_ClipVarType);

   wid = gtk_sheet_new_browser(ncolumns, nrows, title);

   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

int
clip_GTK_SHEETFLAGS(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(csht, GTK_IS_SHEET);
   _clip_retnl(ClipMachineMemory, GTK_SHEET_FLAGS(csht->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_SHEETSETFLAGS(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   long flag = _clip_parnl(ClipMachineMemory, 2);

   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   _clip_retnl(ClipMachineMemory, GTK_SHEET_SET_FLAGS(csht->widget, flag));
   return 0;
 err:
   return 1;
}

int
clip_GTK_SHEETUNSETFLAGS(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   long flag = _clip_parnl(ClipMachineMemory, 2);

   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   _clip_retnl(ClipMachineMemory, GTK_SHEET_UNSET_FLAGS(csht->widget, flag));
   return 0;
 err:
   return 1;
}

/* change scroll adjustments */
int
clip_GTK_SHEETSETHADJUSTMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cadj = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cadj, GTK_IS_ADJUSTMENT);
   gtk_sheet_set_hadjustment(GTK_SHEET(csht->widget), GTK_ADJUSTMENT(cadj->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_SHEETSETVADJUSTMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cadj = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cadj, GTK_IS_ADJUSTMENT);
   gtk_sheet_set_vadjustment(GTK_SHEET(csht->widget), GTK_ADJUSTMENT(cadj->widget));
   return 0;
 err:
   return 1;
}

/* Change entry */
int
clip_GTK_SHEETCHANGEENTRY(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   long en_type = _clip_parni(ClipMachineMemory, 2);
   WTypeTable *wt_item = _wtype_table_get_by_clip_type(en_type);
   GtkType entry_type = wt_item ? wt_item->ftype() : GTK_TYPE_ENTRY;

   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_sheet_change_entry(GTK_SHEET(csht->widget), entry_type);
   return 0;
 err:
   return 1;
}

/* Returns sheet's entry widget */
int
clip_GTK_SHEETGETENTRY(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwid = NULL;
   GtkWidget *wid = NULL;

   CHECKCWID(csht, GTK_IS_SHEET);
   wid = gtk_sheet_get_entry(GTK_SHEET(csht->widget));
   if (wid)
      {
	 cwid = _list_get_cwidget(ClipMachineMemory, wid);
	 if (!cwid)
	    cwid = _register_widget(ClipMachineMemory, wid, NULL);
	 if (cwid)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
      }
   return 0;
 err:
   return 1;
}

/* Returns state of sheet */
int
clip_GTK_SHEETGETSTATE(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(csht, GTK_IS_SHEET);
   _clip_retni(ClipMachineMemory, gtk_sheet_get_state(GTK_SHEET(csht->widget)));
   return 0;
 err:
   return 1;
}

/* Returns column's properties */
int
clip_GTK_SHEETGETCOLUMN(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   gint col = INT_OPTION(ClipMachineMemory, 2, 1);
   ClipVar *ret = RETPTR(ClipMachineMemory);

   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   if (col > 0 && col <= GTK_SHEET(csht->widget)->maxcol)
      {
	 GtkSheetColumn *column = &GTK_SHEET(csht->widget)->column[col - 1];
	 memset(ret, 0, sizeof(ClipVar));
	 _clip_map(ClipMachineMemory, ret);
	 if (column->name)
	    _clip_mputc(ClipMachineMemory, ret, HASH_NAME, column->name, strlen(column->name));
	 _clip_mputn(ClipMachineMemory, ret, HASH_WIDTH, column->width);
	 _clip_mputn(ClipMachineMemory, ret, HASH_LEFTXPIXEL, column->left_xpixel);
	/* min left column displaying text on this column */
	 _clip_mputn(ClipMachineMemory, ret, HASH_LEFTTEXTCOLUMN, column->left_text_column);
	/* max right column displaying text on this column */
	 _clip_mputn(ClipMachineMemory, ret, HASH_RIGHTTEXTCOLUMN, column->right_text_column);
	 _clip_mputn(ClipMachineMemory, ret, HASH_JUSTIFICATION, column->justification);
	 _clip_mputl(ClipMachineMemory, ret, HASH_ISSENSITIVE, column->is_sensitive);
	 _clip_mputl(ClipMachineMemory, ret, HASH_ISVISIBLE, column->is_visible);
      }
   return 0;
 err:
   return 1;
}

/* Returns row's properies */
int
clip_GTK_SHEETGETROW(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   gint nrow = INT_OPTION(ClipMachineMemory, 2, 1);
   ClipVar *ret = RETPTR(ClipMachineMemory);

   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   if (nrow > 0 && nrow <= GTK_SHEET(csht->widget)->maxrow)
      {
	 GtkSheetRow *row = &GTK_SHEET(csht->widget)->row[nrow - 1];
	 memset(ret, 0, sizeof(ClipVar));
	 _clip_map(ClipMachineMemory, ret);
	 if (row->name)
	    _clip_mputc(ClipMachineMemory, ret, HASH_NAME, row->name, strlen(row->name));
	 _clip_mputn(ClipMachineMemory, ret, HASH_HEIGHT, row->height);
	 _clip_mputn(ClipMachineMemory, ret, HASH_TOPYPIXEL, row->top_ypixel);
	 _clip_mputl(ClipMachineMemory, ret, HASH_ISSENSITIVE, row->is_sensitive);
	 _clip_mputl(ClipMachineMemory, ret, HASH_ISVISIBLE, row->is_visible);
      }
   return 0;
 err:
   return 1;
}

/* Returns number of columns */
int
clip_GTK_SHEETGETCOLUMNSCOUNT(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(csht, GTK_IS_SHEET);
   _clip_retni(ClipMachineMemory, gtk_sheet_get_columns_count(GTK_SHEET(csht->widget)));
   return 0;
 err:
   return 1;
}

/* Returns number of rows */
int
clip_GTK_SHEETGETROWSCOUNT(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(csht, GTK_IS_SHEET);

   _clip_retni(ClipMachineMemory, gtk_sheet_get_rows_count(GTK_SHEET(csht->widget)));
   return 0;
 err:
   return 1;
}

/* Returns visible range */
int
clip_GTK_SHEETGETVISIBLERANGE(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   GtkSheetRange range;
   ClipVar *cvrange = RETPTR(ClipMachineMemory);
   CHECKCWID(csht, GTK_IS_SHEET);

   gtk_sheet_get_visible_range(GTK_SHEET(csht->widget), &range);
   memset(cvrange, 0, sizeof(&cvrange));
   _clip_map(ClipMachineMemory, cvrange);
   _sheet_range_to_map(ClipMachineMemory, &range, cvrange);
   return 0;
 err:
   return 1;
}

/* Returns current selected range */
int
clip_GTK_SHEETGETRANGE(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   ClipVar *cvrange = RETPTR(ClipMachineMemory);
   CHECKCWID(csht, GTK_IS_SHEET);

   memset(cvrange, 0, sizeof(&cvrange));
   _clip_map(ClipMachineMemory, cvrange);
   _sheet_range_to_map(ClipMachineMemory, &GTK_SHEET(csht->widget)->range, cvrange);
   return 0;
 err:
   return 1;
}

/* Sets selection mode */
int
clip_GTK_SHEETSETSELECTIONMODE(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   gint mode = _clip_parni(ClipMachineMemory, 2);
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_sheet_set_selection_mode(GTK_SHEET(csht->widget), mode);
   return 0;
 err:
   return 1;
}

/* set sheet title */
int
clip_GTK_SHEETSETTITLE(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   gchar *title = CHAR_OPTION(ClipMachineMemory, 2, "");
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);
   gtk_sheet_set_title(GTK_SHEET(csht->widget), title);
   return 0;
 err:
   return 1;
}

/* freeze all visual updates of the sheet.
 * Then thaw the sheet after you have made a number of changes.
 * The updates will occure in a more efficent way than if
 * you made them on a unfrozen sheet */
int
clip_GTK_SHEETFREEZE(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(csht, GTK_IS_SHEET);
   gtk_sheet_freeze(GTK_SHEET(csht->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_SHEETTHAW(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(csht, GTK_IS_SHEET);
   gtk_sheet_thaw(GTK_SHEET(csht->widget));
   return 0;
 err:
   return 1;
}

/* Sets column title */
int
clip_GTK_SHEETSETCOLUMNTITLE(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   gint column = INT_OPTION(ClipMachineMemory, 2, 1);
   gchar *title = CHAR_OPTION(ClipMachineMemory, 3, "");
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, CHARACTER_type_of_ClipVarType);
   gtk_sheet_set_column_title(GTK_SHEET(csht->widget), column - 1, title);
   return 0;
 err:
   return 1;
}

/* Sets row title */
int
clip_GTK_SHEETSETROWTITLE(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   gint row = INT_OPTION(ClipMachineMemory, 2, 1);
   gchar *title = CHAR_OPTION(ClipMachineMemory, 3, "");
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, CHARACTER_type_of_ClipVarType);
   gtk_sheet_set_row_title(GTK_SHEET(csht->widget), row - 1, title);
   return 0;
 err:
   return 1;
}

/* Adds row button label */
int
clip_GTK_SHEETROWBUTTONADDLABEL(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   gint row = INT_OPTION(ClipMachineMemory, 2, 1);
   char *label = CHAR_OPTION(ClipMachineMemory, 3, "");
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, CHARACTER_type_of_ClipVarType);
   gtk_sheet_row_button_add_label(GTK_SHEET(csht->widget), row - 1, label);
   return 0;
 err:
   return 1;
}

/* Adds column button label */
int
clip_GTK_SHEETCOLUMNBUTTONADDLABEL(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   gint column = INT_OPTION(ClipMachineMemory, 2, 1);
   char *label = CHAR_OPTION(ClipMachineMemory, 3, "");
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, CHARACTER_type_of_ClipVarType);
   gtk_sheet_column_button_add_label(GTK_SHEET(csht->widget), column - 1, label);
   return 0;
 err:
   return 1;
}

/* Sets justification of row button */
int
clip_GTK_SHEETROWBUTTONJUSTIFY(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   gint row = INT_OPTION(ClipMachineMemory, 2, 1);
   GtkJustification justify = _clip_parni(ClipMachineMemory, 3);
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   gtk_sheet_row_button_justify(GTK_SHEET(csht->widget), row - 1, justify);
   return 0;
 err:
   return 1;
}

/* Sets justification of column button */
int
clip_GTK_SHEETCOLUMNBUTTONJUSTIFY(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   gint column = INT_OPTION(ClipMachineMemory, 2, 1);
   GtkJustification justify = _clip_parni(ClipMachineMemory, 3);
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   gtk_sheet_column_button_justify(GTK_SHEET(csht->widget), column - 1, justify);
   return 0;
 err:
   return 1;
}

/* scroll the viewing area of the sheet to the given column
* and row; row_align and col_align are between 0-1 representing the
* location the row should appear on the screnn, 0.0 being top or left,
* 1.0 being bottom or right; if row or column is negative then there
* is no change */
int
clip_GTK_SHEETMOVETO(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   gint row = INT_OPTION(ClipMachineMemory, 2, 1);
   gint column = INT_OPTION(ClipMachineMemory, 3, 1);
   gfloat row_align = _clip_parnd(ClipMachineMemory, 4);
   gfloat col_align = _clip_parnd(ClipMachineMemory, 5);
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, NUMERIC_type_of_ClipVarType);
   CHECKOPT(5, NUMERIC_type_of_ClipVarType);
   gtk_sheet_moveto(GTK_SHEET(csht->widget), row - 1, column - 1, row_align, col_align);
   return 0;
 err:
   return 1;
}

/* resize column titles */
int
clip_GTK_SHEETSETCOLUMNTITLESHEIGHT(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   guint height = _clip_parni(ClipMachineMemory, 2);
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_sheet_set_column_titles_height(GTK_SHEET(csht->widget), height);
   return 0;
 err:
   return 1;
}

/* resize row titles */
int
clip_GTK_SHEETSETROWTITLESWIDTH(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   guint width = _clip_parni(ClipMachineMemory, 2);
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_sheet_set_row_titles_width(GTK_SHEET(csht->widget), width);
   return 0;
 err:
   return 1;
}

/* Shows column titles */
int
clip_GTK_SHEETSHOWCOLUMNTITLES(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(csht, GTK_IS_SHEET);
   gtk_sheet_show_column_titles(GTK_SHEET(csht->widget));
   return 0;
 err:
   return 1;
}

/* Hides column titles */
int
clip_GTK_SHEETHIDECOLUMNTITLES(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(csht, GTK_IS_SHEET);
   gtk_sheet_hide_column_titles(GTK_SHEET(csht->widget));
   return 0;
 err:
   return 1;
}

/* Shows row titles */
int
clip_GTK_SHEETSHOWROWTITLES(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(csht, GTK_IS_SHEET);
   gtk_sheet_show_row_titles(GTK_SHEET(csht->widget));
   return 0;
 err:
   return 1;
}

/* Hides row titles */
int
clip_GTK_SHEETHIDEROWTITLES(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(csht, GTK_IS_SHEET);
   gtk_sheet_hide_row_titles(GTK_SHEET(csht->widget));
   return 0;
 err:
   return 1;
}

/* Sets columns sensitivity */
int
clip_GTK_SHEETCOLUMNSETSENSITIVITY(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   gint column = INT_OPTION(ClipMachineMemory, 2, 1);
   gboolean sensitive = BOOL_OPTION(ClipMachineMemory, 3, TRUE);
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, LOGICAL_type_of_ClipVarType);
   gtk_sheet_column_set_sensitivity(GTK_SHEET(csht->widget), column - 1, sensitive);
   return 0;
 err:
   return 1;
}

/* Sets sensitivity for all columns */
int
clip_GTK_SHEETCOLUMNSSETSENSITIVITY(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   gboolean sensitive = BOOL_OPTION(ClipMachineMemory, 2, TRUE);
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   gtk_sheet_columns_set_sensitivity(GTK_SHEET(csht->widget), sensitive);
   return 0;
 err:
   return 1;
}

/* Sets rows sensitivity */
int
clip_GTK_SHEETROWSETSENSITIVITY(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   gint row = INT_OPTION(ClipMachineMemory, 2, 1);
   gboolean sensitive = BOOL_OPTION(ClipMachineMemory, 3, TRUE);
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, LOGICAL_type_of_ClipVarType);
   gtk_sheet_row_set_sensitivity(GTK_SHEET(csht->widget), row - 1, sensitive);
   return 0;
 err:
   return 1;
}

/* Sets sensitivity for all rows */
int
clip_GTK_SHEETROWSSETSENSITIVITY(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   gboolean sensitive = BOOL_OPTION(ClipMachineMemory, 2, TRUE);
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   gtk_sheet_rows_set_sensitivity(GTK_SHEET(csht->widget), sensitive);
   return 0;
 err:
   return 1;
}

/* Sets columns visibility */
int
clip_GTK_SHEETCOLUMNSETVISIBILITY(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   gint column = INT_OPTION(ClipMachineMemory, 2, 1);
   gboolean visible = BOOL_OPTION(ClipMachineMemory, 3, TRUE);
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, LOGICAL_type_of_ClipVarType);
   gtk_sheet_column_set_visibility(GTK_SHEET(csht->widget), column - 1, visible);
   return 0;
 err:
   return 1;
}

/* Sets columns label visibility */
int
clip_GTK_SHEETCOLUMNLABELSETVISIBILITY(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   gint column = INT_OPTION(ClipMachineMemory, 2, 1);
   gboolean visible = BOOL_OPTION(ClipMachineMemory, 3, TRUE);
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, LOGICAL_type_of_ClipVarType);
   gtk_sheet_column_label_set_visibility(GTK_SHEET(csht->widget), column - 1, visible);
   return 0;
 err:
   return 1;
}

/* Sets visibility of all columns label */
int
clip_GTK_SHEETCOLUMNSLABELSSETVISIBILITY(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   gboolean visible = BOOL_OPTION(ClipMachineMemory, 2, TRUE);
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   gtk_sheet_columns_labels_set_visibility(GTK_SHEET(csht->widget), visible);
   return 0;
 err:
   return 1;
}

/* Sets rows visibility */
int
clip_GTK_SHEETROWSETVISIBILITY(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   gint row = INT_OPTION(ClipMachineMemory, 2, 1);
   gboolean visible = BOOL_OPTION(ClipMachineMemory, 3, TRUE);
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, LOGICAL_type_of_ClipVarType);
   gtk_sheet_row_set_visibility(GTK_SHEET(csht->widget), row - 1, visible);
   return 0;
 err:
   return 1;
}

/* Sets columns label visibility */
int
clip_GTK_SHEETROWLABELSETVISIBILITY(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   gint row = INT_OPTION(ClipMachineMemory, 2, 1);
   gboolean visible = BOOL_OPTION(ClipMachineMemory, 3, TRUE);
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, LOGICAL_type_of_ClipVarType);
   gtk_sheet_row_label_set_visibility(GTK_SHEET(csht->widget), row - 1, visible);
   return 0;
 err:
   return 1;
}

/* Sets visibility of all columns label */
int
clip_GTK_SHEETROWSLABELSSETVISIBILITY(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   gboolean visible = BOOL_OPTION(ClipMachineMemory, 2, TRUE);
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   gtk_sheet_rows_labels_set_visibility(GTK_SHEET(csht->widget), visible);
   return 0;
 err:
   return 1;
}

/* Selects the row */
int
clip_GTK_SHEETSELECTROW(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   gint row = INT_OPTION(ClipMachineMemory, 2, 1);
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_sheet_select_row(GTK_SHEET(csht->widget), row - 1);
   return 0;
 err:
   return 1;
}

/* Selects the column */
int
clip_GTK_SHEETSELECTCOLUMN(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   gint column = INT_OPTION(ClipMachineMemory, 2, 1);
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_sheet_select_column(GTK_SHEET(csht->widget), column - 1);
   return 0;
 err:
   return 1;
}

/* save selected range to "clipboard" */
int
clip_GTK_SHEETCLIPRANGE(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   ClipVar *mrange = _clip_spar(ClipMachineMemory, 2);
   GtkSheetRange range;
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKARG(2, MAP_type_of_ClipVarType);
   _map_to_sheet_range(ClipMachineMemory, mrange, &range);
   gtk_sheet_clip_range(GTK_SHEET(csht->widget), &range);
   return 0;
 err:
   return 1;
}

/* free clipboard */
int
clip_GTK_SHEETUNCLIPRANGE(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(csht, GTK_IS_SHEET);
   gtk_sheet_unclip_range(GTK_SHEET(csht->widget));
   return 0;
 err:
   return 1;
}

/* get scrollbars adjustment */
int
clip_GTK_SHEETGETHADJUSTMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   GtkAdjustment *adj;
   C_widget *cadj;
   CHECKCWID(csht, GTK_IS_SHEET);
   adj = gtk_sheet_get_hadjustment(GTK_SHEET(csht->widget));
   cadj = _list_get_cwidget(ClipMachineMemory, adj);
   if (!cadj)
      cadj = _register_widget(ClipMachineMemory, (GtkWidget *) adj, NULL);
   if (cadj)
      _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cadj->obj);
   return 0;
 err:
   return 1;
}

int
clip_GTK_SHEETGETVADJUSTMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   GtkAdjustment *adj;
   C_widget *cadj;
   CHECKCWID(csht, GTK_IS_SHEET);
   adj = gtk_sheet_get_vadjustment(GTK_SHEET(csht->widget));
   cadj = _list_get_cwidget(ClipMachineMemory, adj);
   if (!cadj)
      cadj = _register_widget(ClipMachineMemory, (GtkWidget *) adj, NULL);
   if (cadj)
      _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cadj->obj);
   return 0;
 err:
   return 1;
}

/* Highlight the selected range */
int
clip_GTK_SHEETSELECTRANGE(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   ClipVar *mrange = _clip_spar(ClipMachineMemory, 2);
   GtkSheetRange range;
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKARG(2, MAP_type_of_ClipVarType);
   _map_to_sheet_range(ClipMachineMemory, mrange, &range);
   gtk_sheet_select_range(GTK_SHEET(csht->widget), &range);
   return 0;
 err:
   return 1;
}

int
clip_GTK_SHEETUNSELECTRANGE(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(csht, GTK_IS_SHEET);
   gtk_sheet_unselect_range(GTK_SHEET(csht->widget));
   return 0;
 err:
   return 1;
}

/* set active cell where the entry will be displayed
 * returns FALSE if current cell can't be deactivated or
 * requested cell can't be activated */
int
clip_GTK_SHEETSETACTIVECELL(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   gint row = INT_OPTION(ClipMachineMemory, 2, 1);
   gint column = INT_OPTION(ClipMachineMemory, 3, 1);
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   _clip_retl(ClipMachineMemory, gtk_sheet_set_active_cell(GTK_SHEET(csht->widget), row - 1, column - 1));
   return 0;
 err:
   return 1;
}

int
clip_GTK_SHEETGETACTIVECELL(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   gint row, column;
   CHECKCWID(csht, GTK_IS_SHEET);
   gtk_sheet_get_active_cell(GTK_SHEET(csht->widget), &row, &column);
   _clip_storni(ClipMachineMemory, row + 1, 2, 0);
   _clip_storni(ClipMachineMemory, column + 1, 3, 0);
   _clip_map(ClipMachineMemory, RETPTR(ClipMachineMemory));
   _clip_mputn(ClipMachineMemory, RETPTR(ClipMachineMemory), HASH_ROW, row + 1);
   _clip_mputn(ClipMachineMemory, RETPTR(ClipMachineMemory), HASH_COL, column + 1);
   return 0;
 err:
   return 1;
}

/* set cell contents and allocate memory if needed */
int
clip_GTK_SHEETSETCELL(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   gint row = INT_OPTION(ClipMachineMemory, 2, 1);
   gint column = INT_OPTION(ClipMachineMemory, 3, 1);
   GtkJustification justification = _clip_parni(ClipMachineMemory, 4);
   const gchar *text = CHAR_OPTION(ClipMachineMemory, 5, "");
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, NUMERIC_type_of_ClipVarType);
   CHECKOPT(5, CHARACTER_type_of_ClipVarType);
   gtk_sheet_set_cell(GTK_SHEET(csht->widget), row - 1, column - 1, justification, text);
   return 0;
 err:
   return 1;
}

/* set cell contents */
int
clip_GTK_SHEETSETCELLTEXT(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   gint row = INT_OPTION(ClipMachineMemory, 2, 1);
   gint column = INT_OPTION(ClipMachineMemory, 3, 1);
   const gchar *text = CHAR_OPTION(ClipMachineMemory, 4, "");
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, CHARACTER_type_of_ClipVarType);
   gtk_sheet_set_cell_text(GTK_SHEET(csht->widget), row - 1, column - 1, text);
   return 0;
 err:
   return 1;
}

/* get cell contents */
int
clip_GTK_SHEETCELLGETTEXT(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   gint row = INT_OPTION(ClipMachineMemory, 2, 1);
   gint column = INT_OPTION(ClipMachineMemory, 3, 1);
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   _clip_retc(ClipMachineMemory, gtk_sheet_cell_get_text(GTK_SHEET(csht->widget), row - 1, column - 1));
   return 0;
 err:
   return 1;
}

/* Clear cell contents */
int
clip_GTK_SHEETCELLCLEAR(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   gint row = INT_OPTION(ClipMachineMemory, 2, 1);
   gint column = INT_OPTION(ClipMachineMemory, 3, 1);
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   gtk_sheet_cell_clear(GTK_SHEET(csht->widget), row - 1, column - 1);
   return 0;
 err:
   return 1;
}

/* Clear cell contents and remove links */
int
clip_GTK_SHEETCELLDELETE(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   gint row = INT_OPTION(ClipMachineMemory, 2, 1);
   gint column = INT_OPTION(ClipMachineMemory, 3, 1);
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   gtk_sheet_cell_delete(GTK_SHEET(csht->widget), row - 1, column - 1);
   return 0;
 err:
   return 1;
}

/* Clear range contents. If range==NIL the whole sheet will be cleared */
int
clip_GTK_SHEETRANGECLEAR(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   ClipVar *mrange = _clip_spar(ClipMachineMemory, 2);
   GtkSheetRange range, *prange = NULL;
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, MAP_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == MAP_type_of_ClipVarType)
      prange = &range;
   _map_to_sheet_range(ClipMachineMemory, mrange, prange);
   gtk_sheet_range_clear(GTK_SHEET(csht->widget), prange);
   return 0;
 err:
   return 1;
}

/* Clear range contents and remove links. */
int
clip_GTK_SHEETRANGEDELETE(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   ClipVar *mrange = _clip_spar(ClipMachineMemory, 2);
   GtkSheetRange range, *prange = NULL;
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, MAP_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == MAP_type_of_ClipVarType)
      prange = &range;
   _map_to_sheet_range(ClipMachineMemory, mrange, prange);
   gtk_sheet_range_delete(GTK_SHEET(csht->widget), prange);
   return 0;
 err:
   return 1;
}

/* Returns state of cell - X_STATE_NORMAL or X_STATE_SELECTED */
int
clip_GTK_SHEETCELLGETSTATE(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   gint row = INT_OPTION(ClipMachineMemory, 2, 1);
   gint column = INT_OPTION(ClipMachineMemory, 3, 1);
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   _clip_retni(ClipMachineMemory, gtk_sheet_cell_get_state(GTK_SHEET(csht->widget), row - 1, column - 1));
   return 0;
 err:
   return 1;
}

/* get row and column correspondig to the given position in the screen */
int
clip_GTK_SHEETGETPIXELINFO(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   gint x = _clip_parni(ClipMachineMemory, 2);
   gint y = _clip_parni(ClipMachineMemory, 3);
   gint row = -1, column = -1;
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   _clip_retni(ClipMachineMemory, gtk_sheet_get_pixel_info(GTK_SHEET(csht->widget), x, y, &row, &column));
   _clip_storni(ClipMachineMemory, row + 1, 4, 0);
   _clip_storni(ClipMachineMemory, column + 1, 5, 0);
   return 0;
 err:
   return 1;
}

/* get area of a given cell */
int
clip_GTK_SHEETGETCELLAREA(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   gint row = INT_OPTION(ClipMachineMemory, 2, 1);
   gint column = INT_OPTION(ClipMachineMemory, 3, 1);
   ClipVar *result = RETPTR(ClipMachineMemory);
   GdkRectangle area;
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   memset(result, 0, sizeof(ClipVar));
   if (gtk_sheet_get_cell_area(GTK_SHEET(csht->widget), row - 1, column - 1, &area))
      {
	 _clip_map(ClipMachineMemory, result);
	 _clip_mputn(ClipMachineMemory, result, HASH_X, area.x);
	 _clip_mputn(ClipMachineMemory, result, HASH_Y, area.y);
	 _clip_mputn(ClipMachineMemory, result, HASH_WIDTH, area.width);
	 _clip_mputn(ClipMachineMemory, result, HASH_HEIGHT, area.height);
      }
   return 0;
 err:
   return 1;
}

/* set column width */
int
clip_GTK_SHEETSETCOLUMNWIDTH(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   gint column = INT_OPTION(ClipMachineMemory, 2, 1);
   gint width = _clip_parni(ClipMachineMemory, 3);
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   gtk_sheet_set_column_width(GTK_SHEET(csht->widget), column - 1, width);
   return 0;
 err:
   return 1;
}

/* set row height */
int
clip_GTK_SHEETSETROWHEIGHT(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   gint row = INT_OPTION(ClipMachineMemory, 2, 1);
   gint height = _clip_parni(ClipMachineMemory, 3);
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   gtk_sheet_set_row_height(GTK_SHEET(csht->widget), row - 1, height);
   return 0;
 err:
   return 1;
}

/* append columns to the end of the sheet */
int
clip_GTK_SHEETADDCOLUMN(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   guint ncols = _clip_parni(ClipMachineMemory, 2);
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_sheet_add_column(GTK_SHEET(csht->widget), ncols);
   return 0;
 err:
   return 1;
}

/* append rows to the end of the sheet */
int
clip_GTK_SHEETADDROW(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   guint nrows = _clip_parni(ClipMachineMemory, 2);
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_sheet_add_row(GTK_SHEET(csht->widget), nrows);
   return 0;
 err:
   return 1;
}

/* Insert row before the given row and pull right */
int
clip_GTK_SHEETINSERTROWS(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   guint row = INT_OPTION(ClipMachineMemory, 2, 1);
   guint nrows = _clip_parni(ClipMachineMemory, 3);
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   gtk_sheet_insert_rows(GTK_SHEET(csht->widget), row - 1, nrows);
   return 0;
 err:
   return 1;
}

/* Insert column before the given column and pull down */
int
clip_GTK_SHEETINSERTCOLUMNS(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   guint column = INT_OPTION(ClipMachineMemory, 2, 1);
   guint ncols = _clip_parni(ClipMachineMemory, 3);
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   gtk_sheet_insert_columns(GTK_SHEET(csht->widget), column - 1, ncols);
   return 0;
 err:
   return 1;
}

/* Delete nrows rows starting in row */
int
clip_GTK_SHEETDELETEROWS(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   guint row = INT_OPTION(ClipMachineMemory, 2, 1);
   guint nrows = _clip_parni(ClipMachineMemory, 3);
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   gtk_sheet_delete_rows(GTK_SHEET(csht->widget), row - 1, nrows);
   return 0;
 err:
   return 1;
}

/* Delete ncols columnss starting in column */
int
clip_GTK_SHEETDELETECOLUMNS(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   guint column = INT_OPTION(ClipMachineMemory, 2, 1);
   guint ncols = _clip_parni(ClipMachineMemory, 3);
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   gtk_sheet_delete_columns(GTK_SHEET(csht->widget), column - 1, ncols);
   return 0;
 err:
   return 1;
}

/* Set background color of the given range */
int
clip_GTK_SHEETRANGESETBACKGROUND(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   ClipVar *mrange = _clip_spar(ClipMachineMemory, 2);
   ClipVar *mcolor = _clip_spar(ClipMachineMemory, 3);
   GtkSheetRange range, *prange = NULL;
   GdkColor color;
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, MAP_type_of_ClipVarType);
   CHECKARG(3, MAP_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == MAP_type_of_ClipVarType)
      prange = &range;
   _map_to_sheet_range(ClipMachineMemory, mrange, prange);
   _map_colors_to_gdk(ClipMachineMemory, mcolor, &color);
   gdk_color_alloc(gtk_widget_get_colormap(csht->widget), &color);
   gtk_sheet_range_set_background(GTK_SHEET(csht->widget), prange, &color);
   return 0;
 err:
   return 1;
}

/* Set foreground color of the given range */
int
clip_GTK_SHEETRANGESETFOREGROUND(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   ClipVar *mrange = _clip_spar(ClipMachineMemory, 2);
   ClipVar *mcolor = _clip_spar(ClipMachineMemory, 3);
   GtkSheetRange range, *prange = NULL;
   GdkColor color;
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, MAP_type_of_ClipVarType);
   CHECKARG(3, MAP_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == MAP_type_of_ClipVarType)
      prange = &range;
   _map_to_sheet_range(ClipMachineMemory, mrange, prange);
   _map_colors_to_gdk(ClipMachineMemory, mcolor, &color);
   gdk_color_alloc(gtk_widget_get_colormap(csht->widget), &color);
   gtk_sheet_range_set_foreground(GTK_SHEET(csht->widget), prange, &color);
   return 0;
 err:
   return 1;
}

/* set text justification (GTK_JUSTIFY_LEFT, RIGHT, CENTER) of the given range.
 * The default value is GTK_JUSTIFY_LEFT. If autoformat is on, the
 * default justification for numbers is GTK_JUSTIFY_RIGHT */
int
clip_GTK_SHEETRANGESETJUSTIFICATION(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   ClipVar *mrange = _clip_spar(ClipMachineMemory, 2);
   GtkJustification justification = INT_OPTION(ClipMachineMemory, 3, GTK_JUSTIFY_LEFT);
   GtkSheetRange range, *prange = NULL;
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, MAP_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == MAP_type_of_ClipVarType)
      prange = &range;
   _map_to_sheet_range(ClipMachineMemory, mrange, prange);
   gtk_sheet_range_set_justification(GTK_SHEET(csht->widget), prange, justification);
   return 0;
 err:
   return 1;
}

/* Set justification for column */
int
clip_GTK_SHEETCOLUMNSETJUSTIFICATION(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   gint column = INT_OPTION(ClipMachineMemory, 2, 1);
   GtkJustification justification = INT_OPTION(ClipMachineMemory, 3, GTK_JUSTIFY_LEFT);
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   gtk_sheet_column_set_justification(GTK_SHEET(csht->widget), column - 1, justification);
   return 0;
 err:
   return 1;
}

/* set if cell contents can be edited or not in the given range:
 * accepted values are TRUE or FALSE. */
int
clip_GTK_SHEETRANGESETEDITABLE(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   ClipVar *mrange = _clip_spar(ClipMachineMemory, 2);
   gboolean editable = BOOL_OPTION(ClipMachineMemory, 3, TRUE);
   GtkSheetRange range, *prange = NULL;
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, MAP_type_of_ClipVarType);
   CHECKOPT(3, LOGICAL_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == MAP_type_of_ClipVarType)
      prange = &range;
   _map_to_sheet_range(ClipMachineMemory, mrange, prange);
   gtk_sheet_range_set_editable(GTK_SHEET(csht->widget), prange, editable);
   return 0;
 err:
   return 1;
}

/* set if cell contents are visible or not in the given range:
 * accepted values are TRUE or FALSE.*/
int
clip_GTK_SHEETRANGESETVISIBLE(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   ClipVar *mrange = _clip_spar(ClipMachineMemory, 2);
   gboolean visible = _clip_parl(ClipMachineMemory, 3);
   GtkSheetRange range, *prange = NULL;
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, MAP_type_of_ClipVarType);
   CHECKOPT(3, LOGICAL_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == MAP_type_of_ClipVarType)
      prange = &range;
   _map_to_sheet_range(ClipMachineMemory, mrange, prange);
   gtk_sheet_range_set_visible(GTK_SHEET(csht->widget), prange, visible);
   return 0;
 err:
   return 1;
}

int
clip_GTK_SHEETGETMAXCOL(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(csht, GTK_IS_SHEET);
   _clip_retni(ClipMachineMemory, GTK_SHEET(csht->widget)->maxcol);
   return 0;
 err:
   return 1;
}

int
clip_GTK_SHEETGETMAXROW(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(csht, GTK_IS_SHEET);
   _clip_retni(ClipMachineMemory, GTK_SHEET(csht->widget)->maxrow);
   return 0;
 err:
   return 1;
}

/* set cell border style in the given range.
 * mask values are CELL_LEFT_BORDER, CELL_RIGHT_BORDER, CELL_TOP_BORDER,
 * CELL_BOTTOM_BORDER
 * width is the width of the border line in pixels
 * line_style is the line_style for the border line */
int
clip_GTK_SHEETRANGESETBORDER(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   ClipVar *mrange = _clip_spar(ClipMachineMemory, 2);
   gint mask = _clip_parni(ClipMachineMemory, 3);
   gint width = _clip_parni(ClipMachineMemory, 4);
   gint line_style = _clip_parni(ClipMachineMemory, 5);
   GtkSheetRange range, *prange = NULL;
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, MAP_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, NUMERIC_type_of_ClipVarType);
   CHECKOPT(5, NUMERIC_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == MAP_type_of_ClipVarType)
      prange = &range;
   _map_to_sheet_range(ClipMachineMemory, mrange, prange);
   gtk_sheet_range_set_border(GTK_SHEET(csht->widget), prange, mask, width, line_style);
   return 0;
 err:
   return 1;
}

/* Set border color of the given range */
int
clip_GTK_SHEETRANGESETBORDERCOLOR(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   ClipVar *mrange = _clip_spar(ClipMachineMemory, 2);
   ClipVar *mcolor = _clip_spar(ClipMachineMemory, 3);
   GtkSheetRange range, *prange = NULL;
   GdkColor color;
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, MAP_type_of_ClipVarType);
   CHECKARG(3, MAP_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == MAP_type_of_ClipVarType)
      prange = &range;
   _map_to_sheet_range(ClipMachineMemory, mrange, prange);
   _map_colors_to_gdk(ClipMachineMemory, mcolor, &color);
   gdk_color_alloc(gtk_widget_get_colormap(csht->widget), &color);
   gtk_sheet_range_set_border_color(GTK_SHEET(csht->widget), prange, &color);
   return 0;
 err:
   return 1;
}

/* Set font for the given range */
int
clip_GTK_SHEETRANGESETFONT(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   ClipVar *mrange = _clip_spar(ClipMachineMemory, 2);
   char *font_descr = _clip_parc(ClipMachineMemory, 3);
   GdkFont *font = NULL;
   C_object *cfont = NULL;
   GtkSheetRange range, *prange = NULL;
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, MAP_type_of_ClipVarType);
   CHECKOPT3(3, NUMERIC_type_of_ClipVarType, MAP_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 2) == MAP_type_of_ClipVarType)
      prange = &range;

   if (_clip_parinfo(ClipMachineMemory, 3) == NUMERIC_type_of_ClipVarType || _clip_parinfo(ClipMachineMemory, 3) == MAP_type_of_ClipVarType)
      {
	 cfont = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));
	 if (cfont)
	    font = (GdkFont *) (cfont->object);
      }
   else
      {
	 font = gdk_font_load(font_descr);
	 cfont = _register_object(ClipMachineMemory, font, GDK_OBJECT_FONT, NULL, (coDestructor) gdk_object_font_destructor);
      }

   if (font)
      {
	 _map_to_sheet_range(ClipMachineMemory, mrange, prange);
	 gtk_sheet_range_set_font(GTK_SHEET(csht->widget), prange, font);
      }
   return 0;
 err:
   return 1;
}

/* get cell attributes of the given cell */
/* TRUE means that the cell is currently allocated */
int
clip_GTK_SHEETGETATTRIBUTES(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   gint row = INT_OPTION(ClipMachineMemory, 2, 1);
   gint column = INT_OPTION(ClipMachineMemory, 3, 1);
   GtkSheetCellAttr attributes;
   ClipVar *result = RETPTR(ClipMachineMemory);
   CHECKCWID(csht, GTK_IS_SHEET);
   memset(result, 0, sizeof(ClipVar));
   gtk_sheet_get_attributes(GTK_SHEET(csht->widget), row - 1, column - 1, &attributes);
   {
      ClipVar mforecolor, mbackcolor, mborder, mbordercolor;
      memset(&mforecolor, 0, sizeof(ClipVar));
      memset(&mbackcolor, 0, sizeof(ClipVar));
      memset(&mborder, 0, sizeof(ClipVar));
      memset(&mbordercolor, 0, sizeof(ClipVar));
      _clip_map(ClipMachineMemory, result);
      _clip_map(ClipMachineMemory, &mforecolor);
      _clip_map(ClipMachineMemory, &mbackcolor);
      _clip_map(ClipMachineMemory, &mborder);
      _clip_map(ClipMachineMemory, &mbordercolor);

      _gdk_color_to_map(ClipMachineMemory, attributes.foreground, &mforecolor);
      _clip_madd(ClipMachineMemory, result, HASH_FOREGROUND, &mforecolor);
      _gdk_color_to_map(ClipMachineMemory, attributes.background, &mbackcolor);
      _clip_madd(ClipMachineMemory, result, HASH_BACKGROUND, &mbackcolor);

      _clip_mputn(ClipMachineMemory, &mborder, HASH_MASK, attributes.border.mask);
      _clip_mputn(ClipMachineMemory, &mborder, HASH_WIDTH, attributes.border.width);
      _clip_mputn(ClipMachineMemory, &mborder, HASH_LINESTYLE, attributes.border.line_style);
      _clip_mputn(ClipMachineMemory, &mborder, HASH_CAPSTYLE, attributes.border.cap_style);
      _clip_mputn(ClipMachineMemory, &mborder, HASH_JOINSTYLE, attributes.border.join_style);
      _gdk_color_to_map(ClipMachineMemory, attributes.border.color, &mbordercolor);
      _clip_madd(ClipMachineMemory, &mborder, HASH_COLOR, &mbordercolor);
      _clip_madd(ClipMachineMemory, result, HASH_BORDER, &mborder);

      _clip_mputn(ClipMachineMemory, result, HASH_JUSTIFICATION, attributes.justification);
      _clip_mputl(ClipMachineMemory, result, HASH_ISEDITABLE, attributes.is_editable);
      _clip_mputl(ClipMachineMemory, result, HASH_ISVISIBLE, attributes.is_visible);
   }
   return 0;
 err:
   return 1;
}

int
clip_GTK_SHEETPUT(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   gint x = _clip_parni(ClipMachineMemory, 3);
   gint y = _clip_parni(ClipMachineMemory, 4);
   GtkSheetChild *child;
   ClipVar *result = RETPTR(ClipMachineMemory);
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   memset(result, 0, sizeof(ClipVar));
   child = gtk_sheet_put(GTK_SHEET(csht->widget), cwid->widget, x, y);
   if (child)
      {
	 _clip_map(ClipMachineMemory, result);
	 _clip_madd(ClipMachineMemory, result, HASH_WIDGET, &cwid->obj);
	 _clip_mputn(ClipMachineMemory, result, HASH_X, child->x);
	 _clip_mputn(ClipMachineMemory, result, HASH_Y, child->y);
	 _clip_mputl(ClipMachineMemory, result, HASH_ATTACHEDTOCELL, child->attached_to_cell);
	 _clip_mputn(ClipMachineMemory, result, HASH_ROW, child->row);
	 _clip_mputn(ClipMachineMemory, result, HASH_COL, child->col);
	 _clip_mputn(ClipMachineMemory, result, HASH_XALIGN, child->x_align);
	 _clip_mputn(ClipMachineMemory, result, HASH_YALIGN, child->y_align);
      }
   return 0;
 err:
   return 1;
}

int
clip_GTK_SHEETATTACH(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   gint row = INT_OPTION(ClipMachineMemory, 3, 1);
   gint col = INT_OPTION(ClipMachineMemory, 4, 1);
   gfloat x_align = DBL_OPTION(ClipMachineMemory, 5, 0);
   gfloat y_align = DBL_OPTION(ClipMachineMemory, 6, 0);
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, NUMERIC_type_of_ClipVarType);
   CHECKOPT(5, NUMERIC_type_of_ClipVarType);
   CHECKOPT(6, NUMERIC_type_of_ClipVarType);
   gtk_sheet_attach(GTK_SHEET(csht->widget), cwid->widget, row - 1, col - 1, x_align, y_align);
   return 0;
 err:
   return 1;
}

int
clip_GTK_SHEETMOVECHILD(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   gint x = _clip_parni(ClipMachineMemory, 3);
   gint y = _clip_parni(ClipMachineMemory, 4);
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, NUMERIC_type_of_ClipVarType);
   gtk_sheet_move_child(GTK_SHEET(csht->widget), cwid->widget, x, y);
   return 0;
 err:
   return 1;
}

int
clip_GTK_SHEETGETCHILDAT(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   gint row = INT_OPTION(ClipMachineMemory, 2, 1);
   gint col = INT_OPTION(ClipMachineMemory, 3, 1);
   GtkSheetChild *child;
   ClipVar *result = RETPTR(ClipMachineMemory);
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   memset(result, 0, sizeof(ClipVar));
   child = gtk_sheet_get_child_at(GTK_SHEET(csht->widget), row - 1, col - 1);
   if (child)
      {
	 C_widget *cchild = _list_get_cwidget(ClipMachineMemory, child->widget);
	 _clip_map(ClipMachineMemory, result);
	 if (!cchild)
	    cchild = _register_widget(ClipMachineMemory, child->widget, NULL);
	 if (cchild)
	    _clip_madd(ClipMachineMemory, result, HASH_WIDGET, &cchild->obj);
	 _clip_mputn(ClipMachineMemory, result, HASH_X, child->x);
	 _clip_mputn(ClipMachineMemory, result, HASH_Y, child->y);
	 _clip_mputl(ClipMachineMemory, result, HASH_ATTACHEDTOCELL, child->attached_to_cell);
	 _clip_mputn(ClipMachineMemory, result, HASH_ROW, child->row + 1);
	 _clip_mputn(ClipMachineMemory, result, HASH_COL, child->col + 1);
	 _clip_mputn(ClipMachineMemory, result, HASH_XALIGN, child->x_align);
	 _clip_mputn(ClipMachineMemory, result, HASH_YALIGN, child->y_align);
      }
   return 0;
 err:
   return 1;
}

int
clip_GTK_SHEETBUTTONATTACH(ClipMachine * ClipMachineMemory)
{
   C_widget *csht = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   gint row = INT_OPTION(ClipMachineMemory, 3, 1);
   gint col = INT_OPTION(ClipMachineMemory, 4, 1);
   gfloat x_align = _clip_parnd(ClipMachineMemory, 5);
   gfloat y_align = _clip_parnd(ClipMachineMemory, 6);
   CHECKCWID(csht, GTK_IS_SHEET);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, NUMERIC_type_of_ClipVarType);
   CHECKOPT(5, NUMERIC_type_of_ClipVarType);
   CHECKOPT(6, NUMERIC_type_of_ClipVarType);
   gtk_sheet_button_attach(GTK_SHEET(csht->widget), cwid->widget, row - 1, col - 1, x_align, y_align);
   return 0;
 err:
   return 1;
}
