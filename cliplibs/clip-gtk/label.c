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

/*********************** SIGNALS **************************/

/****           Label has no own signal                ****/

/**********************************************************/

/* Register label in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_label()
{
   return GTK_TYPE_LABEL;
}

long
_clip_type_label()
{
   return GTK_WIDGET_LABEL;
}

const char *
_clip_type_name_label()
{
   return "GTK_WIDGET_LABEL";
}

/* Register accel label in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_accel_label()
{
   return GTK_TYPE_ACCEL_LABEL;
}

long
_clip_type_accel_label()
{
   return GTK_WIDGET_ACCEL_LABEL;
}

const char *
_clip_type_name_accel_label()
{
   return "GTK_WIDGET_ACCEL_LABEL";
}

int
clip_INIT___LABEL(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_label, _clip_type_name_label, _gtk_type_label, _gtk_type_misc, NULL);
   _wtype_table_put(_clip_type_accel_label, _clip_type_name_accel_label, _gtk_type_accel_label, _gtk_type_label, NULL);
   return 0;
}

/**********************************************************/

/**** LABEL constructor ****/
int
clip_GTK_LABELNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   char     *text = _clip_parc(ClipMachineMemory, 2);

   char     *pchar = _clip_parc(ClipMachineMemory, 3);

   guint     accel_key = 0;

   GtkWidget *wid = NULL;

   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT(3, CHARACTER_type_of_ClipVarType);

   if (pchar)
    {
       char     *pc;

       for (pc = text; pc && *pc; pc++)
	  if (*pc == *pchar)
	     *pc = '_';
    }
   LOCALE_TO_UTF(text);
   wid = gtk_label_new(text);
   if (!wid)
      goto err;
   if (pchar)
      accel_key = gtk_label_parse_uline(GTK_LABEL(wid), text);
   FREE_TEXT(text);

   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   _clip_mputn(ClipMachineMemory, &cwid->obj, HASH_ACCELKEY, accel_key);

   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

/* Sets the text within the GtkLabel widget. It overwrites any text that was
 * there before. Note that underlines that were there before do not get
 * overwritten. If you want to erase underlines just send NULL to
 * gtk_label_set_pattern().  */
int
clip_GTK_LABELSETTEXT(ClipMachine * ClipMachineMemory)
{
   C_widget *clbl = _fetch_cw_arg(ClipMachineMemory);

   char     *text = _clip_parc(ClipMachineMemory, 2);

   char     *pchar = _clip_parc(ClipMachineMemory, 3);

   guint     accel_key = 0;

   CHECKCWID(clbl, GTK_IS_LABEL);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT(3, CHARACTER_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      text = "\0";
   LOCALE_TO_UTF(text);
   gtk_label_set_text(GTK_LABEL(clbl->widget), text);
   if (pchar)
    {
       char     *pc;

       for (pc = text; pc && *pc; pc++)
	  if (*pc == *pchar)
	     *pc = '_';
       accel_key = gtk_label_parse_uline(GTK_LABEL(clbl->widget), text);
    }
   FREE_TEXT(text);
   _clip_mputn(ClipMachineMemory, &clbl->obj, HASH_ACCELKEY, accel_key);
   return 0;
 err:
   return 1;
}

/* The pattern of underlines you want under the existing text within the GtkLabel
 * widget. For example if the current text of the label says "FooBarBaz" passing
 * a pattern of "___ ___" will underline "Foo" and "Baz" but not "Bar".  */
int
clip_GTK_LABELSETPATTERN(ClipMachine * ClipMachineMemory)
{
   C_widget *clbl = _fetch_cw_arg(ClipMachineMemory);

   char     *pattern = _clip_parc(ClipMachineMemory, 2);

   CHECKCWID(clbl, GTK_IS_LABEL);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);
   gtk_label_set_pattern(GTK_LABEL(clbl->widget), pattern);
   return 0;
 err:
   return 1;
}

/* Set where the text within the GtkLabel will align to. This can be one of four
 * values: GTK_JUSTIFY_LEFT, GTK_JUSTIFY_RIGHT, GTK_JUSTIFY_CENTER, and
 * GTK_JUSTIFY_FILL. GTK_JUSTIFY_CENTER is the default value when the widget is
 * first created with gtk_label_new(). */
int
clip_GTK_LABELSETJUSTIFY(ClipMachine * ClipMachineMemory)
{
   C_widget *clbl = _fetch_cw_arg(ClipMachineMemory);

   GtkJustification jtype = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(clbl, GTK_IS_LABEL);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_label_set_justify(GTK_LABEL(clbl->widget), jtype);
   return 0;
 err:
   return 1;
}

/* Gets the current string of text within the GtkLabel */
int
clip_GTK_LABELGET(ClipMachine * ClipMachineMemory)
{
   C_widget *clbl = _fetch_cw_arg(ClipMachineMemory);

   gchar    *text;

   CHECKCWID(clbl, GTK_IS_LABEL);
   gtk_label_get(GTK_LABEL(clbl->widget), &text);
   LOCALE_FROM_UTF(text);
   _clip_retc(ClipMachineMemory, text);
   FREE_TEXT(text);
   return 0;
 err:
   return 1;
}

/* Parses the given string for underscores and converts the next character
 * to an underlined character. The last character that was underlined will
 * have its lower-cased accelerator keyval returned (i.e. "_File" would
 * return the keyval for "f". This is probably only used within the Gtk+
 * library itself for menu items and such.  */
int
clip_GTK_LABELPARSEULINE(ClipMachine * ClipMachineMemory)
{
   C_widget *clbl = _fetch_cw_arg(ClipMachineMemory);

   char     *string = CHAR_OPTION(ClipMachineMemory, 2, "");

   CHECKCWID(clbl, GTK_IS_LABEL);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);
   LOCALE_TO_UTF(string);
   _clip_retni(ClipMachineMemory, gtk_label_parse_uline(GTK_LABEL(clbl->widget), string));
   FREE_TEXT(string);
   return 0;
 err:
   return 1;
}

/* Toggles line wrapping within the GtkLabel widget. TRUE makes it break lines
 * if text exceeds the widget's size. FALSE lets the text get cut off by the
 * edge of the widget if it exceeds the widget size. */
int
clip_GTK_LABELSETLINEWRAP(ClipMachine * ClipMachineMemory)
{
   C_widget *clbl = _fetch_cw_arg(ClipMachineMemory);

   gboolean  wrap = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(clbl, GTK_IS_LABEL);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   gtk_label_set_line_wrap(GTK_LABEL(clbl->widget), wrap);
   return 0;
 err:
   return 1;
}

/****************************************************************/
/****************************************************************/
/****************************************************************/
/****************************************************************/

/**** ACCEL LABEL constructor ****/
int
clip_GTK_ACCELLABELNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   char     *text = _clip_parc(ClipMachineMemory, 2);

   GtkWidget *wid = NULL;

   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(text);
   wid = gtk_accel_label_new(text);
   FREE_TEXT(text);
   if (!wid)
      goto err;

   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

/* Sets the widget whose accelerators are to be shown. */
int
clip_GTK_ACCELLABELSETACCELWIDGET(ClipMachine * ClipMachineMemory)
{
   C_widget *clbl = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(clbl, GTK_IS_ACCEL_LABEL);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   gtk_accel_label_set_accel_widget(GTK_ACCEL_LABEL(clbl->widget), cwid->widget);
   return 0;
 err:
   return 1;
}

/* Returns the width needed to display the accelerator key(s). This is
   used by menus to align all of the GtkMenuItem widgets, and shouldn't
   be needed by applications. */
int
clip_GTK_ACCELLABELGETACCELWIDTH(ClipMachine * ClipMachineMemory)
{
   C_widget *clbl = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(clbl, GTK_IS_ACCEL_LABEL);
   _clip_retni(ClipMachineMemory, gtk_accel_label_get_accel_width(GTK_ACCEL_LABEL(clbl->widget)));
   return 0;
 err:
   return 1;
}

/* Recreates the string representing the accelerator keys. This should
   not be needed since the string is automatically updated whenever
   accelerators are added or removed from the associated widget. */
int
clip_GTK_ACCELLABELREFETCH(ClipMachine * ClipMachineMemory)
{
   C_widget *clbl = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(clbl, GTK_IS_ACCEL_LABEL);
   _clip_retl(ClipMachineMemory, gtk_accel_label_refetch(GTK_ACCEL_LABEL(clbl->widget)));
   return 0;
 err:
   return 1;
}

/* Parses the given string for underscores and converts the next character
 * to an underlined character. The last character that was underlined will
 * have its lower-cased accelerator keyval returned (i.e. "_File" would
 * return the keyval for "f". This is probably only used within the Gtk+
 * library itself for menu items and such. */
int
_label_parse_uline(GtkWidget * wid, char text[])
{
   int       i, j;

   int       ret;

   if (!text)
      return 0;

   ret = gtk_label_parse_uline(GTK_LABEL(wid), text);
   for (i = 0, j = 0; text[i]; i++)
    {
       if (text[i] != '_')
	  text[j++] = text[i];
    }
   text[j] = '\0';
   gtk_label_set_text(GTK_LABEL(wid), text);
   return ret;
}
