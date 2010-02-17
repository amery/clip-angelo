/*
    Copyright (C) 2001  ITK
    Author  : Alexey M. Tkachenko <alexey@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg2.h"

#include <gtk/gtk.h>

#include "ci_clip-gtk2.ch"
#include "ci_clip-gtk2.h"

/********************** SIGNALS **************************/
/****          Text have no own signals               ****/
/*********************************************************/

/* Register text in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_text()
{
   return GTK_TYPE_TEXT;
}

long
_clip_type_text()
{
   return GTK_WIDGET_TEXT;
}

const char *
_clip_type_name_text()
{
   return "GTK_WIDGET_TEXT";
}

int
clip_INIT___TEXT(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_text, _clip_type_name_text, _gtk_type_text, _gtk_type_editable, NULL);
   return 0;
}

/**********************************************************/

/**** TEXT constructor ****/
int
clip_GTK_TEXTNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   C_widget *chadj = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   C_widget *cvadj = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));
   GtkAdjustment *hadj, *vadj;
   GtkWidget *wid = NULL;
   C_widget *cwid;
   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(chadj, GTK_IS_ADJUSTMENT);
   CHECKOPT2(3, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cvadj, GTK_IS_ADJUSTMENT);

   hadj = chadj ? GTK_ADJUSTMENT(chadj->widget) : NULL;
   vadj = cvadj ? GTK_ADJUSTMENT(cvadj->widget) : NULL;
   wid = gtk_text_new(hadj, vadj);
   if (!wid)
      goto err;

   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

/* Set whether the GtkText widget can be edited by the user or not.
 * This still allows you the programmer to make changes with the
 * various GtkText functions.  */
int
clip_GTK_TEXTSETEDITABLE(ClipMachine * ClipMachineMemory)
{
   C_widget *ctext = _fetch_cw_arg(ClipMachineMemory);
   gboolean editable = _clip_parl(ClipMachineMemory, 2);
   CHECKCWID(ctext, GTK_IS_TEXT);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      editable = TRUE;
   gtk_text_set_editable(GTK_TEXT(ctext->widget), editable);
   return 0;
 err:
   return 1;
}

/* Sets whether the GtkText widget wraps words down to the next
 * line if it can't be completed on the current line. */
int
clip_GTK_TEXTSETWORDWRAP(ClipMachine * ClipMachineMemory)
{
   C_widget *ctext = _fetch_cw_arg(ClipMachineMemory);
   gboolean wordwrap = _clip_parl(ClipMachineMemory, 2);
   CHECKCWID(ctext, GTK_IS_TEXT);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      wordwrap = TRUE;
   gtk_text_set_word_wrap(GTK_TEXT(ctext->widget), wordwrap);
   return 0;
 err:
   return 1;
}

/* Controls how GtkText handles long lines of continuous text.
 * If line wrap is on, the line is broken when it reaches the
 * extent of the GtkText widget viewing area and the rest is
 * displayed on the next line. If it is not set, the line continues
 * regardless size of current viewing area. Similar to word wrap
 * but it disregards word boundaries. */
int
clip_GTK_TEXTSETLINEWRAP(ClipMachine * ClipMachineMemory)
{
   C_widget *ctext = _fetch_cw_arg(ClipMachineMemory);
   gboolean linewrap = _clip_parl(ClipMachineMemory, 2);
   CHECKCWID(ctext, GTK_IS_TEXT);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      linewrap = TRUE;
   gtk_text_set_line_wrap(GTK_TEXT(ctext->widget), linewrap);
   return 0;
 err:
   return 1;
}

/* Allows you to set GtkAdjustment pointers which in turn allows
 * you to keep track of the viewing position of the GtkText widget. */
int
clip_GTK_TEXTSETADJUSTMENTS(ClipMachine * ClipMachineMemory)
{
   C_widget *ctext = _fetch_cw_arg(ClipMachineMemory);
   C_widget *chadj = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   C_widget *cvadj = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));
   GtkAdjustment *hadj, *vadj;
   CHECKCWID(ctext, GTK_IS_TEXT);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(chadj, GTK_IS_ADJUSTMENT);
   CHECKOPT2(3, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cvadj, GTK_IS_ADJUSTMENT);

   hadj = chadj ? GTK_ADJUSTMENT(chadj->widget) : NULL;
   vadj = cvadj ? GTK_ADJUSTMENT(cvadj->widget) : NULL;
   gtk_text_set_adjustments(GTK_TEXT(ctext->widget), hadj, vadj);
   return 0;
 err:
   return 1;
}

/* Sets the cursor at the given point. In this case a point constitutes the
 * number of characters from the extreme upper left corner of the GtkText widget. */
int
clip_GTK_TEXTSETPOINT(ClipMachine * ClipMachineMemory)
{
   C_widget *ctext = _fetch_cw_arg(ClipMachineMemory);
   guint index = _clip_parni(ClipMachineMemory, 2);
   CHECKCWID(ctext, GTK_IS_TEXT);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_text_set_point(GTK_TEXT(ctext->widget), index);
   return 0;
 err:
   return 1;
}

/* Gets the current position of the cursor as the number of
 * characters from the upper left corner of the GtkText widget.  */
int
clip_GTK_TEXTGETPOINT(ClipMachine * ClipMachineMemory)
{
   C_widget *ctext = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(ctext, GTK_IS_TEXT);
   _clip_retni(ClipMachineMemory, gtk_text_get_point(GTK_TEXT(ctext->widget)));
   return 0;
 err:
   return 1;
}

/* Returns the length of the all the text contained within the
 * GtkText widget; disregards current point position.  */
int
clip_GTK_TEXTGETLENGTH(ClipMachine * ClipMachineMemory)
{
   C_widget *ctext = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(ctext, GTK_IS_TEXT);
   _clip_retni(ClipMachineMemory, gtk_text_get_length(GTK_TEXT(ctext->widget)));
   return 0;
 err:
   return 1;
}

/* Freezes the GtkText widget which disallows redrawing of the widget
 * until it is thawed. This is useful if a large number of changes
 * are going to made to the text within the widget, reducing the amount
 * of flicker seen by the user.  */
int
clip_GTK_TEXTFREEZE(ClipMachine * ClipMachineMemory)
{
   C_widget *ctext = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(ctext, GTK_IS_TEXT);
   gtk_text_freeze(GTK_TEXT(ctext->widget));
   return 0;
 err:
   return 1;
}

/* Allows the GtkText widget to be redrawn again by GTK. */
int
clip_GTK_TEXTTHAW(ClipMachine * ClipMachineMemory)
{
   C_widget *ctext = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(ctext, GTK_IS_TEXT);
   gtk_text_thaw(GTK_TEXT(ctext->widget));
   return 0;
 err:
   return 1;
}

/* Inserts given text into the GtkText widget with the given
 * properties as outlined below. */
int
clip_GTK_TEXTINSERT(ClipMachine * ClipMachineMemory)
{
   C_widget *ctext = _fetch_cw_arg(ClipMachineMemory);
   gchar *text = _clip_parc(ClipMachineMemory, 2);
   gchar *fontdescr = _clip_parc(ClipMachineMemory, 3);
   ClipVar *mforecolor = _clip_spar(ClipMachineMemory, 4);
   ClipVar *mbackcolor = _clip_spar(ClipMachineMemory, 5);
   gint length = _clip_parni(ClipMachineMemory, 6);
   GdkFont *font = NULL;
   GdkColor forecolor, backcolor;
   CHECKCWID(ctext, GTK_IS_TEXT);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT(3, CHARACTER_type_of_ClipVarType);
   CHECKOPT(4, MAP_type_of_ClipVarType);
   CHECKOPT(5, MAP_type_of_ClipVarType);
   CHECKOPT(6, NUMERIC_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 3) == CHARACTER_type_of_ClipVarType)	/* font */
      {
	 font = gdk_font_load(fontdescr);
	 if (font)
	    gdk_font_ref(font);
      }
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType || !font)
      font = ctext->widget->style->font;

   if (_clip_parinfo(ClipMachineMemory, 4) == MAP_type_of_ClipVarType)	/* foreground color */
      _map_colors_to_gdk(ClipMachineMemory, mforecolor, &forecolor);
   else
      forecolor = ctext->widget->style->text[0];

   if (_clip_parinfo(ClipMachineMemory, 5) == MAP_type_of_ClipVarType)	/* background color */
      _map_colors_to_gdk(ClipMachineMemory, mbackcolor, &backcolor);
   else
      backcolor = ctext->widget->style->base[0];

   if (_clip_parinfo(ClipMachineMemory, 6) == UNDEF_type_of_ClipVarType)	/* length. If -1, inserts all the text */
      length = -1;

   LOCALE_TO_UTF(text);
   gtk_text_insert(GTK_TEXT(ctext->widget), font, &forecolor, &backcolor, text, length);
   FREE_TEXT(text);
   return 0;
 err:
   return 1;
}

/* Deletes from the current point position backward
 * the given number of characters. */
int
clip_GTK_TEXTBACKWARDDELETE(ClipMachine * ClipMachineMemory)
{
   C_widget *ctext = _fetch_cw_arg(ClipMachineMemory);
   guint nchars = _clip_parni(ClipMachineMemory, 2);
   CHECKCWID(ctext, GTK_IS_TEXT);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_text_backward_delete(GTK_TEXT(ctext->widget), nchars);
   return 0;
 err:
   return 1;
}

/* Deletes from the current point position forward
 * the given number of characters. */
int
clip_GTK_TEXTFORWARDDELETE(ClipMachine * ClipMachineMemory)
{
   C_widget *ctext = _fetch_cw_arg(ClipMachineMemory);
   guint nchars = _clip_parni(ClipMachineMemory, 2);
   CHECKCWID(ctext, GTK_IS_TEXT);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_text_forward_delete(GTK_TEXT(ctext->widget), nchars);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTGETEDITABLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cedt = _fetch_cw_arg(ClipMachineMemory);
   GtkArg arg;
   CHECKCWID(cedt, GTK_IS_EDITABLE);
   arg.type = GTK_TYPE_BOOL;
   arg.name = "editable";
   gtk_widget_get(cedt->widget, &arg);
   _clip_retl(ClipMachineMemory, arg.d.bool_data);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTGETWORDWRAP(ClipMachine * ClipMachineMemory)
{
   C_widget *ctext = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(ctext, GTK_IS_TEXT);
   _clip_retl(ClipMachineMemory, GTK_TEXT(ctext->widget)->word_wrap);
   return 0;
 err:
   return 1;
}
