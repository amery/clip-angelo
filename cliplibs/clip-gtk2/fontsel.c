/*
    Copyright (C) 2001-2004  ITK
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

/*********************** SIGNALS ***************************/

/****      Font selection widget has no own signals     ****/

/****  Font selection dialog widget has no own signals  ****/

/***********************************************************/

/* Register font selection in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_font_selection()
{
   return GTK_TYPE_FONT_SELECTION;
}

long
_clip_type_font_selection()
{
   return GTK_WIDGET_FONT_SELECTION;
}

const char *
_clip_type_name_font_selection()
{
   return "GTK_WIDGET_FONT_SELECTION";
}

/* Register font selection dialog in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_font_selection_dialog()
{
   return GTK_TYPE_FONT_SELECTION_DIALOG;
}

long
_clip_type_font_selection_dialog()
{
   return GTK_WIDGET_FONT_SELECTION_DIALOG;
}

const char *
_clip_type_name_font_selection_dialog()
{
   return "GTK_WIDGET_FONT_SELECTION_DIALOG";
}

int
clip_INIT___FONT_SELECTION(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_font_selection, _clip_type_name_font_selection, _gtk_type_font_selection, _gtk_type_vbox, NULL);
   _wtype_table_put(_clip_type_font_selection_dialog,
		    _clip_type_name_font_selection_dialog, _gtk_type_font_selection_dialog, _gtk_type_dialog, NULL);
   return 0;
}

/**********************************************************/

/****  Font selection constructor ****/
int
clip_GTK_FONTSELECTIONNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   GtkWidget *wid = NULL;

   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);

   wid = gtk_font_selection_new();
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);

   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

/****  ------------------ ****/

/* Gets the currently-selected font name. */
int
clip_GTK_FONTSELECTIONGETFONTNAME(ClipMachine * ClipMachineMemory)
{
   C_widget *cfsel = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cfsel, GTK_IS_FONT_SELECTION);
   _clip_retc(ClipMachineMemory, gtk_font_selection_get_font_name(GTK_FONT_SELECTION(cfsel->widget)));
   return 0;
 err:
   return 1;
}

/* Sets the currently-selected font. */
int
clip_GTK_FONTSELECTIONSETFONTNAME(ClipMachine * ClipMachineMemory)
{
   C_widget *cfsel = _fetch_cw_arg(ClipMachineMemory);

   gchar    *font_name = _clip_parc(ClipMachineMemory, 2);

   CHECKCWID(cfsel, GTK_IS_FONT_SELECTION);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      font_name = "\0";
   _clip_retl(ClipMachineMemory, gtk_font_selection_set_font_name(GTK_FONT_SELECTION(cfsel->widget), font_name));
   return 0;
 err:
   return 1;
}

/* Gets the text displayed in the preview area. */
int
clip_GTK_FONTSELECTIONGETPREVIEWTEXT(ClipMachine * ClipMachineMemory)
{
   C_widget *cfsel = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cfsel, GTK_IS_FONT_SELECTION);
   _clip_retc(ClipMachineMemory, (gchar *) gtk_font_selection_get_preview_text(GTK_FONT_SELECTION(cfsel->widget)));
   return 0;
 err:
   return 1;
}

/* Sets the text displayed in the preview area. */
int
clip_GTK_FONTSELECTIONSETPREVIEWTEXT(ClipMachine * ClipMachineMemory)
{
   C_widget *cfsel = _fetch_cw_arg(ClipMachineMemory);

   gchar    *preview_text = _clip_parc(ClipMachineMemory, 2);

   CHECKCWID(cfsel, GTK_IS_FONT_SELECTION);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      preview_text = "\0";
   LOCALE_TO_UTF(preview_text);
   gtk_font_selection_set_preview_text(GTK_FONT_SELECTION(cfsel->widget), preview_text);
   FREE_TEXT(preview_text);
   return 0;
 err:
   return 1;
}

/***************************************************************************/

/************         * Font selection dialog *                 ************/

/***************************************************************************/

/****  Font selection dialog constructor ****/
int
clip_GTK_FONTSELECTIONDIALOGNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   char     *title = _clip_parc(ClipMachineMemory, 2);

   GtkWidget *wid = NULL;

   C_widget *cwid, *cokbtn, *capplybtn, *ccancelbtn;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      title = "\0";
   LOCALE_TO_UTF(title);
   wid = gtk_font_selection_dialog_new(title);
   FREE_TEXT(title);
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   cokbtn = _register_widget(ClipMachineMemory, GTK_FONT_SELECTION_DIALOG(wid)->ok_button, NULL);
   capplybtn = _register_widget(ClipMachineMemory, GTK_FONT_SELECTION_DIALOG(wid)->apply_button, NULL);
   ccancelbtn = _register_widget(ClipMachineMemory, GTK_FONT_SELECTION_DIALOG(wid)->cancel_button, NULL);

   if (cokbtn)
      _clip_madd(ClipMachineMemory, &cwid->obj, HASH_OKBUTTON, &cokbtn->obj);
   if (capplybtn)
      _clip_madd(ClipMachineMemory, &cwid->obj, HASH_APPLYBUTTON, &capplybtn->obj);
   if (ccancelbtn)
      _clip_madd(ClipMachineMemory, &cwid->obj, HASH_CANCELBUTTON, &ccancelbtn->obj);

   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

/****  ------------------ ****/

/* Gets the currently-selected font name. */
int
clip_GTK_FONTSELECTIONDIALOGGETFONTNAME(ClipMachine * ClipMachineMemory)
{
   C_widget *cfsel = _fetch_cw_arg(ClipMachineMemory);

   gchar    *fontname;

   CHECKCWID(cfsel, GTK_IS_FONT_SELECTION_DIALOG);
   fontname = gtk_font_selection_dialog_get_font_name(GTK_FONT_SELECTION_DIALOG(cfsel->widget));
   _clip_retc(ClipMachineMemory, fontname);
   return 0;
 err:
   return 1;
}

/* Sets the currently-selected font. */
int
clip_GTK_FONTSELECTIONDIALOGSETFONTNAME(ClipMachine * ClipMachineMemory)
{
   C_widget *cfsel = _fetch_cw_arg(ClipMachineMemory);

   gchar    *font_name = _clip_parc(ClipMachineMemory, 2);

   CHECKCWID(cfsel, GTK_IS_FONT_SELECTION_DIALOG);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      font_name = "\0";
   _clip_retl(ClipMachineMemory, gtk_font_selection_dialog_set_font_name(GTK_FONT_SELECTION_DIALOG(cfsel->widget), font_name));
   return 0;
 err:
   return 1;
}

/* Gets the text displayed in the preview area. */
int
clip_GTK_FONTSELECTIONDIALOGGETPREVIEWTEXT(ClipMachine * ClipMachineMemory)
{
   C_widget *cfsel = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cfsel, GTK_IS_FONT_SELECTION_DIALOG);
   _clip_retc(ClipMachineMemory,
	      (gchar *) gtk_font_selection_dialog_get_preview_text(GTK_FONT_SELECTION_DIALOG(cfsel->widget)));
   return 0;
 err:
   return 1;
}

/* Sets the text displayed in the preview area. */
int
clip_GTK_FONTSELECTIONDIALOGSETPREVIEWTEXT(ClipMachine * ClipMachineMemory)
{
   C_widget *cfsel = _fetch_cw_arg(ClipMachineMemory);

   gchar    *preview_text = _clip_parc(ClipMachineMemory, 2);

   CHECKCWID(cfsel, GTK_IS_FONT_SELECTION_DIALOG);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      preview_text = "\0";
   LOCALE_TO_UTF(preview_text);
   gtk_font_selection_dialog_set_preview_text(GTK_FONT_SELECTION_DIALOG(cfsel->widget), preview_text);
   FREE_TEXT(preview_text);
   return 0;
 err:
   return 1;
}
