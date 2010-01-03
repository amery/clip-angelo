/*
    Copyright (C) 2001 - 2004  ITK
    Author  : Alexey M. Tkachenko <alexey@itk.ru>
    	      Elena V. Kornilova <alena@itk.ru>
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
static    gint
handle_insert_text_signal(GtkWidget * widget, gchar * new_text, gint new_text_length, gint * position, C_signal * cs)
{
   PREPARECV(cs, cv);
   LOCALE_FROM_UTF(new_text);
   _clip_mputc(cs->cw->cmachine, &cv, HASH_TEXT, new_text, new_text_length);
   FREE_TEXT(new_text);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_TEXTLENGTH, new_text_length);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_POSITION, *position);
   INVOKESIGHANDLER(widget, cs, cv);
}

static    gint
handle_delete_text_signal(GtkWidget * widget, gint start_pos, gint end_pos, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_STARTPOS, start_pos);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_ENDPOS, end_pos);
   INVOKESIGHANDLER(widget, cs, cv);
}

static    gint
handle_set_editable_signal(GtkWidget * widget, gboolean is_editable, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputl(cs->cw->cmachine, &cv, HASH_EDITABLE, is_editable);
   INVOKESIGHANDLER(widget, cs, cv);
}

static    gint
handle_move_cursor_signal(GtkWidget * widget, gint x, gint y, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_X, x);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_Y, y);
   INVOKESIGHANDLER(widget, cs, cv);
}

static    gint
handle_move_word_signal(GtkWidget * widget, gint num_words, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_NUMWORDS, num_words);
   INVOKESIGHANDLER(widget, cs, cv);
}

static    gint
handle_move_page_signal(GtkWidget * widget, gint x, gint y, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_X, x);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_Y, y);
   INVOKESIGHANDLER(widget, cs, cv);
}

static    gint
handle_move_to_row_signal(GtkWidget * widget, gint row, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_ROW, row);
   INVOKESIGHANDLER(widget, cs, cv);
}

static    gint
handle_move_to_column_signal(GtkWidget * widget, gint column, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_COLUMN, column);
   INVOKESIGHANDLER(widget, cs, cv);
}

static    gint
handle_kill_char_signal(GtkWidget * widget, gint direction, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_DIRECTION, direction);
   INVOKESIGHANDLER(widget, cs, cv);
}

static    gint
handle_kill_word_signal(GtkWidget * widget, gint direction, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_DIRECTION, direction);
   INVOKESIGHANDLER(widget, cs, cv);
}

static    gint
handle_kill_line_signal(GtkWidget * widget, gint direction, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_DIRECTION, direction);
   INVOKESIGHANDLER(widget, cs, cv);
}

/* Signals table */
static SignalTable editable_signals[] = {
   {"activate", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_ACTIVATE_SIGNAL},
   {"changed", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_CHANGED_SIGNAL},
   {"insert-text", GSF(handle_insert_text_signal), ESF(object_emit_signal),
    GTK_INSERT_TEXT_SIGNAL},
   {"delete-text", GSF(handle_delete_text_signal), ESF(object_emit_signal),
    GTK_DELETE_TEXT_SIGNAL},
   {"set-editable", GSF(handle_set_editable_signal), ESF(object_emit_signal),
    GTK_SET_EDITABLE_SIGNAL},
   {"move-cursor", GSF(handle_move_cursor_signal), ESF(object_emit_signal),
    GTK_MOVE_CURSOR_SIGNAL},
   {"move-word", GSF(handle_move_word_signal), ESF(object_emit_signal),
    GTK_MOVE_WORD_SIGNAL},
   {"move-page", GSF(handle_move_page_signal), ESF(object_emit_signal),
    GTK_MOVE_PAGE_SIGNAL},
   {"move-to-row", GSF(handle_move_to_row_signal), ESF(object_emit_signal),
    GTK_MOVE_TO_ROW_SIGNAL},
   {"move-to-column", GSF(handle_move_to_column_signal),
    ESF(object_emit_signal), GTK_MOVE_TO_COLUMN_SIGNAL},
   {"kill-char", GSF(handle_kill_char_signal), ESF(object_emit_signal),
    GTK_KILL_CHAR_SIGNAL},
   {"kill-word", GSF(handle_kill_word_signal), ESF(object_emit_signal),
    GTK_KILL_WORD_SIGNAL},
   {"kill-line", GSF(handle_kill_line_signal), ESF(object_emit_signal),
    GTK_KILL_LINE_SIGNAL},
   {"cut-clipboard", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_CUT_CLIPBOARD_SIGNAL},
   {"copy-clipboard", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_COPY_CLIPBOARD_SIGNAL},
   {"paste-clipboard", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_PASTE_CLIPBOARD_SIGNAL},
   {"", NULL, NULL, 0}
};

/* Register editable widget in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_editable()
{
   return GTK_TYPE_EDITABLE;
}

long
_clip_type_editable()
{
   return GTK_WIDGET_EDITABLE;
}

const char *
_clip_type_name_editable()
{
   return "GTK_WIDGET_EDITABLE";
}

int
clip_INIT___EDITABLE(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_editable, _clip_type_name_editable, _gtk_type_editable, _gtk_type_widget, editable_signals);
   return 0;
}

/**********************************************************/

/* Select a region */
int
clip_GTK_EDITABLESELECTREGION(ClipMachine * ClipMachineMemory)
{
   C_widget *cedt = _fetch_cw_arg(ClipMachineMemory);

   gint      start = _clip_parni(ClipMachineMemory, 2) - 1;

   gint      end = _clip_parni(ClipMachineMemory, 3) - 1;

   CHECKCWID(cedt, GTK_IS_EDITABLE);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   gtk_editable_select_region(GTK_EDITABLE(cedt->widget), start, end);
   return 0;
 err:
   return 1;
}

int
clip_GTK_EDITABLEINSERTTEXT(ClipMachine * ClipMachineMemory)
{
   C_widget *cedt = _fetch_cw_arg(ClipMachineMemory);

   gchar    *text = _clip_parc(ClipMachineMemory, 2);

   gint      position = _clip_parni(ClipMachineMemory, 3) - 1;

   gchar     empty_text[] = "\0";

   gint      text_length;

   CHECKCWID(cedt, GTK_IS_EDITABLE);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      text = empty_text;
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      position = gtk_editable_get_position(GTK_EDITABLE(cedt->widget));
   text_length = strlen(text);
   LOCALE_TO_UTF(text);
   gtk_editable_insert_text(GTK_EDITABLE(cedt->widget), text, text_length, &position);
   FREE_TEXT(text);
   return 0;
 err:
   return 1;
}

int
clip_GTK_EDITABLEDELETETEXT(ClipMachine * ClipMachineMemory)
{
   C_widget *cedt = _fetch_cw_arg(ClipMachineMemory);

   gint      start_pos = _clip_parni(ClipMachineMemory, 2) - 1;

   gint      end_pos = _clip_parni(ClipMachineMemory, 3) - 1;

   CHECKCWID(cedt, GTK_IS_EDITABLE);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      start_pos = 0;
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      end_pos = -1;
   gtk_editable_delete_text(GTK_EDITABLE(cedt->widget), start_pos, end_pos);
   return 0;
 err:
   return 1;
}

int
clip_GTK_EDITABLEGETCHARS(ClipMachine * ClipMachineMemory)
{
   C_widget *cedt = _fetch_cw_arg(ClipMachineMemory);

   gint      start_pos = _clip_parni(ClipMachineMemory, 2) - 1;

   gint      end_pos = _clip_parni(ClipMachineMemory, 3) - 1;

   gchar    *text;

   CHECKCWID(cedt, GTK_IS_EDITABLE);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      start_pos = 0;
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      end_pos = -1;
   text = gtk_editable_get_chars(GTK_EDITABLE(cedt->widget), start_pos, end_pos);
   LOCALE_FROM_UTF(text);
   _clip_retc(ClipMachineMemory, text);
   FREE_TEXT(text);
   return 0;
 err:
   return 1;
}

int
clip_GTK_EDITABLECUTCLIPBOARD(ClipMachine * ClipMachineMemory)
{
   C_widget *cedt = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cedt, GTK_IS_EDITABLE);
   gtk_editable_cut_clipboard(GTK_EDITABLE(cedt->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_EDITABLECOPYCLIPBOARD(ClipMachine * ClipMachineMemory)
{
   C_widget *cedt = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cedt, GTK_IS_EDITABLE);
   gtk_editable_copy_clipboard(GTK_EDITABLE(cedt->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_EDITABLEPASTECLIPBOARD(ClipMachine * ClipMachineMemory)
{
   C_widget *cedt = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cedt, GTK_IS_EDITABLE);
   gtk_editable_paste_clipboard(GTK_EDITABLE(cedt->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_EDITABLEDELETESELECTION(ClipMachine * ClipMachineMemory)
{
   C_widget *cedt = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cedt, GTK_IS_EDITABLE);
   gtk_editable_delete_selection(GTK_EDITABLE(cedt->widget));
   return 0;
 err:
   return 1;
}

/*
int
clip_GTK_EDITABLEGETSELECTION(ClipMachine * ClipMachineMemory)
{
	C_widget *cedt = _fetch_cw_arg(ClipMachineMemory);
	CHECKCWID(cedt,GTK_IS_EDITABLE);
	gtk_editable_get_selection(GTK_EDITABLE(cedt->widget));
	_clip_storni(ClipMachineMemory,GTK_EDITABLE(cedt->widget)->selection_start_pos,2,0);
	_clip_storni(ClipMachineMemory,GTK_EDITABLE(cedt->widget)->selection_end_pos,3,0);
	return 0;
err:
	return 1;
}

*/
int
clip_GTK_EDITABLEGETSELECTIONBOUNDS(ClipMachine * ClipMachineMemory)
{
   C_widget *cedt = _fetch_cw_arg(ClipMachineMemory);

   gint      start, end;

   CHECKCWID(cedt, GTK_IS_EDITABLE);
   gtk_editable_get_selection_bounds(GTK_EDITABLE(cedt->widget), &start, &end);
   start++;
   end++;
   _clip_storni(ClipMachineMemory, start, 2, 0);
   _clip_storni(ClipMachineMemory, end, 3, 0);
   return 0;
 err:
   return 1;
}

int
clip_GTK_EDITABLEHASSELECTION(ClipMachine * ClipMachineMemory)
{
   C_widget *cedt = _fetch_cw_arg(ClipMachineMemory);

   gint      start, end;

   CHECKCWID(cedt, GTK_IS_EDITABLE);
   gtk_editable_get_selection_bounds(GTK_EDITABLE(cedt->widget), &start, &end);
   start += end;
   _clip_retl(ClipMachineMemory, start);
   return 0;
 err:
   return 1;
}

int
clip_GTK_EDITABLESETPOSITION(ClipMachine * ClipMachineMemory)
{
   C_widget *cedt = _fetch_cw_arg(ClipMachineMemory);

   gint      position = _clip_parni(ClipMachineMemory, 2) - 1;

   CHECKCWID(cedt, GTK_IS_EDITABLE);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_editable_set_position(GTK_EDITABLE(cedt->widget), position);
   return 0;
 err:
   return 1;
}

int
clip_GTK_EDITABLEGETPOSITION(ClipMachine * ClipMachineMemory)
{
   C_widget *cedt = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cedt, GTK_IS_EDITABLE);
   _clip_retni(ClipMachineMemory, gtk_editable_get_position(GTK_EDITABLE(cedt->widget)) + 1);
   return 0;
 err:
   return 1;
}

int
clip_GTK_EDITABLESETEDITABLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cedt = _fetch_cw_arg(ClipMachineMemory);

   gboolean  editable = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(cedt, GTK_IS_EDITABLE);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      editable = TRUE;
   gtk_editable_set_editable(GTK_EDITABLE(cedt->widget), editable);
   return 0;
 err:
   return 1;
}

int
clip_GTK_EDITABLEGETEDITABLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cedt = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cedt, GTK_IS_EDITABLE);
   _clip_retl(ClipMachineMemory, gtk_editable_get_editable((GTK_EDITABLE(cedt->widget))));
   return 0;
 err:
   return 1;
}
