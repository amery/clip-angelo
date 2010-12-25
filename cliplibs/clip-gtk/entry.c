/*
    Copyright (C) 2001  ITK
    Author  : Alexey M. Tkachenko <alexey@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg.h"

#include <gtk/gtk.h>

#include "ci_clip-gtk.ch"
#include "ci_clip-gtk.h"

/********************** SIGNALS **************************/
/****          Entry have no own signals              ****/
/*********************************************************/

/* Register entry in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_entry()
{
   return GTK_TYPE_ENTRY;
}

long
_clip_type_entry()
{
   return GTK_WIDGET_ENTRY;
}

const char *
_clip_type_name_entry()
{
   return "GTK_WIDGET_ENTRY";
}

int
clip_INIT___ENTRY(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_entry, _clip_type_name_entry, _gtk_type_entry, _gtk_type_editable, NULL);
   return 0;
}

/**********************************************************/

/**** ENTRY constructor ****/
int
clip_GTK_ENTRYNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   int       max_lentgh = _clip_parni(ClipMachineMemory, 2);

   GtkWidget *wid = NULL;

   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 2) == NUMERIC_type_of_ClipVarType)
      wid = gtk_entry_new_with_max_length(max_lentgh);
   else
      wid = gtk_entry_new();
   if (!wid)
      goto err;

   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

/* Sets text in entry */
int
clip_GTK_ENTRYSETTEXT(ClipMachine * ClipMachineMemory)
{
   C_widget *centry = _fetch_cw_arg(ClipMachineMemory);

   gchar    *text = _clip_parc(ClipMachineMemory, 2);

   gchar     empty_text[] = "\0";

   CHECKCWID(centry, GTK_IS_ENTRY);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      text = empty_text;
   LOCALE_TO_UTF(text);
   gtk_entry_set_text(GTK_ENTRY(centry->widget), text);
   FREE_TEXT(text);
   return 0;
 err:
   return 1;
}

/* Append text to entry */
int
clip_GTK_ENTRYAPPENDTEXT(ClipMachine * ClipMachineMemory)
{
   C_widget *centry = _fetch_cw_arg(ClipMachineMemory);

   gchar    *text = _clip_parc(ClipMachineMemory, 2);

   gchar     empty_text[] = "\0";

   CHECKCWID(centry, GTK_IS_ENTRY);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      text = empty_text;
   LOCALE_TO_UTF(text);
   gtk_entry_append_text(GTK_ENTRY(centry->widget), text);
   FREE_TEXT(text);
   return 0;
 err:
   return 1;
}

/* Insert text to start of entry */
int
clip_GTK_ENTRYPREPENDTEXT(ClipMachine * ClipMachineMemory)
{
   C_widget *centry = _fetch_cw_arg(ClipMachineMemory);

   gchar    *text = _clip_parc(ClipMachineMemory, 2);

   gchar     empty_text[] = "\0";

   CHECKCWID(centry, GTK_IS_ENTRY);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      text = empty_text;
   LOCALE_TO_UTF(text);
   gtk_entry_prepend_text(GTK_ENTRY(centry->widget), text);
   FREE_TEXT(text);
   return 0;
 err:
   return 1;
}

/* Sets a cursor position in entry */
int
clip_GTK_ENTRYSETPOSITION(ClipMachine * ClipMachineMemory)
{
   C_widget *centry = _fetch_cw_arg(ClipMachineMemory);

   gint      position = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(centry, GTK_IS_ENTRY);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_entry_set_position(GTK_ENTRY(centry->widget), position);
   return 0;
 err:
   return 1;
}

/* Returns a text in entry */
int
clip_GTK_ENTRYGETTEXT(ClipMachine * ClipMachineMemory)
{
   C_widget *centry = _fetch_cw_arg(ClipMachineMemory);

   gchar    *text;

   CHECKCWID(centry, GTK_IS_ENTRY);
   text = gtk_entry_get_text(GTK_ENTRY(centry->widget));
   LOCALE_FROM_UTF(text);
   _clip_retc(ClipMachineMemory, text);
   FREE_TEXT(text);
   return 0;
 err:
   return 1;
}

/* Selects a text in entry */
int
clip_GTK_ENTRYSELECTREGION(ClipMachine * ClipMachineMemory)
{
   C_widget *centry = _fetch_cw_arg(ClipMachineMemory);

   gint      start_pos = _clip_parni(ClipMachineMemory, 2);

   gint      end_pos = _clip_parni(ClipMachineMemory, 3);

   CHECKCWID(centry, GTK_IS_ENTRY);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      end_pos = -1;
   gtk_entry_select_region(GTK_ENTRY(centry->widget), start_pos, end_pos);
   return 0;
 err:
   return 1;
}

/* Sets a visibility of characters in entry */
int
clip_GTK_ENTRYSETVISIBILITY(ClipMachine * ClipMachineMemory)
{
   C_widget *centry = _fetch_cw_arg(ClipMachineMemory);

   gboolean  visibility = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(centry, GTK_IS_ENTRY);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      visibility = TRUE;
   gtk_entry_set_visibility(GTK_ENTRY(centry->widget), visibility);
   return 0;
 err:
   return 1;
}

/* Sets a editable of entry */
int
clip_GTK_ENTRYSETEDITABLE(ClipMachine * ClipMachineMemory)
{
   C_widget *centry = _fetch_cw_arg(ClipMachineMemory);

   gboolean  editable = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(centry, GTK_IS_ENTRY);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      editable = TRUE;
   gtk_entry_set_editable(GTK_ENTRY(centry->widget), editable);
   return 0;
 err:
   return 1;
}

/* Sets a max length of text in entry */
int
clip_GTK_ENTRYSETMAXLENGTH(ClipMachine * ClipMachineMemory)
{
   C_widget *centry = _fetch_cw_arg(ClipMachineMemory);

   guint16   max_length = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(centry, GTK_IS_ENTRY);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   gtk_entry_set_max_length(GTK_ENTRY(centry->widget), max_length);
   return 0;
 err:
   return 1;
}

/* Returns a max length of text in entry */
int
clip_GTK_ENTRYGETMAXLENGTH(ClipMachine * ClipMachineMemory)
{
   C_widget *centry = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(centry, GTK_IS_ENTRY);
   _clip_retni(ClipMachineMemory, GTK_ENTRY(centry->widget)->text_max_length);
   return 0;
 err:
   return 1;
}

int
clip_GTK_ENTRYSETSTYLE(ClipMachine * ClipMachineMemory)
{
   C_widget *centry = _fetch_cw_arg(ClipMachineMemory);

   ClipVar  *mstyle = _clip_spar(ClipMachineMemory, 2);

   GtkStyle *style;

   CHECKCWID(centry, GTK_IS_ENTRY);
   CHECKARG(2, MAP_type_of_ClipVarType);
   style = gtk_style_copy(centry->widget->style);
  //gtk_style_unref(centry->widget->style);
   _map_to_style(ClipMachineMemory, mstyle, style);
   gtk_widget_set_style(centry->widget, style);
   return 0;
 err:
   return 1;
}
