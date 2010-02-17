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
   _wtype_table_put(_clip_type_font_selection, _clip_type_name_font_selection, _gtk_type_font_selection, _gtk_type_notebook, NULL);
   _wtype_table_put(_clip_type_font_selection_dialog, _clip_type_name_font_selection_dialog, _gtk_type_font_selection_dialog, _gtk_type_window, NULL);
   return 0;
}

/**********************************************************/

/****  Font selection constructor ****/
int
clip_GTK_FONTSELECTIONNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
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

/* Gets the currently-selected font. */
int
clip_GTK_FONTSELECTIONGETFONT(ClipMachine * ClipMachineMemory)
{
   C_widget *cfsel = _fetch_cw_arg(ClipMachineMemory);
   C_object *cfont;
   GdkFont *font;
   CHECKCWID(cfsel, GTK_IS_FONT_SELECTION);
   font = gtk_font_selection_get_font(GTK_FONT_SELECTION(cfsel->widget));
   if (!font)
      return 0;
   cfont = _list_get_cobject(ClipMachineMemory, font);
   if (!cfont)
      cfont = _register_object(ClipMachineMemory, font, GDK_OBJECT_FONT, NULL, (coDestructor) gdk_object_font_destructor);
   if (cfont)
      _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cfont->obj);
   return 0;
 err:
   return 1;
}

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
   gchar *font_name = _clip_parc(ClipMachineMemory, 2);
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
   _clip_retc(ClipMachineMemory, gtk_font_selection_get_preview_text(GTK_FONT_SELECTION(cfsel->widget)));
   return 0;
 err:
   return 1;
}

/* Sets the text displayed in the preview area. */
int
clip_GTK_FONTSELECTIONSETPREVIEWTEXT(ClipMachine * ClipMachineMemory)
{
   C_widget *cfsel = _fetch_cw_arg(ClipMachineMemory);
   gchar *preview_text = _clip_parc(ClipMachineMemory, 2);
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

/* Sets one of the two font filters, to limit the fonts shown. */
/*
* fontsel	: a GtkFontSelection.
* filter_type	: which of the two font filters to set, either GTK_FONT_FILTER_BASE or
*		  GTK_FONT_FILTER_USER. The user filter can be changed by the user, but
*		  the base filter is permanent.
* font_type 	: the types of font to be shown. This is a bitwise combination of
*		  GTK_FONT_BITMAP, GTK_FONT_SCALABLE and GTK_FONT_SCALABLE_BITMAP, or
*		  GTK_FONT_ALL to show all three font types.
* foundries 	: a NULL-terminated array of strings containing foundry names which
*		  will be shown, or NULL to show all foundries.
* weights 	: a NULL-terminated array of strings containing weight names which
*		  will be shown, or NULL to show all weights.
* slants 		: a NULL-terminated array of strings containing slant names which will
*		  be shown, or NULL to show all slants.
* setwidths 	: a NULL-terminated array of strings containing setwidth names which
*		  will be shown, or NULL to show all setwidths.
* spacings 	: a NULL-terminated array of strings containing spacings which will be
*		  shown, or NULL to show all spacings.
* charsets 	: a NULL-terminated array of strings containing charset names which will
*		  be shown, or NULL to show all charsets.
*/
int
clip_GTK_FONTSELECTIONSETFILTER(ClipMachine * ClipMachineMemory)
{
   C_widget *cfsel = _fetch_cw_arg(ClipMachineMemory);
   int filter_type = _clip_parni(ClipMachineMemory, 2);
   int font_type = _clip_parni(ClipMachineMemory, 3);
   ClipVar *cv_foundries = _clip_spar(ClipMachineMemory, 4);
   ClipVar *cv_weights = _clip_spar(ClipMachineMemory, 5);
   ClipVar *cv_slants = _clip_spar(ClipMachineMemory, 6);
   ClipVar *cv_setwidths = _clip_spar(ClipMachineMemory, 7);
   ClipVar *cv_spacings = _clip_spar(ClipMachineMemory, 8);
   ClipVar *cv_charsets = _clip_spar(ClipMachineMemory, 9);
   gchar **foundries, **weights, **slants, **setwidths, **spacings, **charsets;
   int i;
   ClipArrVar *a;
   CHECKCWID(cfsel, GTK_IS_FONT_SELECTION);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      filter_type = GTK_FONT_FILTER_BASE;
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      font_type = GTK_FONT_ALL;
   switch (cv_foundries->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
      {
      case CHARACTER_type_of_ClipVarType:
	 foundries = (gchar **) calloc(sizeof(*foundries), 2);
	 foundries[0] = cv_foundries->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
	 break;
      case ARRAY_type_of_ClipVarType:
	 a = (ClipArrVar *) _clip_vptr(cv_foundries);
	 foundries = (gchar **) calloc(sizeof(*foundries), a->count_of_ClipArrVar + 1);
	 for (i = 0; i < a->count_of_ClipArrVar; i++)
	    foundries[i] = a->ClipVar_items_of_ClipArrVar[i].ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
      default:
	 foundries = NULL;
      }
   switch (cv_weights->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
      {
      case CHARACTER_type_of_ClipVarType:
	 weights = (gchar **) calloc(sizeof(*weights), 2);
	 weights[0] = cv_weights->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
	 break;
      case ARRAY_type_of_ClipVarType:
	 a = (ClipArrVar *) _clip_vptr(cv_weights);
	 weights = (gchar **) calloc(sizeof(*weights), a->count_of_ClipArrVar + 1);
	 for (i = 0; i < a->count_of_ClipArrVar; i++)
	    weights[i] = a->ClipVar_items_of_ClipArrVar[i].ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
      default:
	 weights = NULL;
      }
   switch (cv_slants->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
      {
      case CHARACTER_type_of_ClipVarType:
	 slants = (gchar **) calloc(sizeof(*slants), 2);
	 slants[0] = cv_slants->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
	 break;
      case ARRAY_type_of_ClipVarType:
	 a = (ClipArrVar *) _clip_vptr(cv_slants);
	 slants = (gchar **) calloc(sizeof(*slants), a->count_of_ClipArrVar + 1);
	 for (i = 0; i < a->count_of_ClipArrVar; i++)
	    slants[i] = a->ClipVar_items_of_ClipArrVar[i].ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
      default:
	 slants = NULL;
      }
   switch (cv_setwidths->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
      {
      case CHARACTER_type_of_ClipVarType:
	 setwidths = (gchar **) calloc(sizeof(*setwidths), 2);
	 setwidths[0] = cv_setwidths->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
	 break;
      case ARRAY_type_of_ClipVarType:
	 a = (ClipArrVar *) _clip_vptr(cv_setwidths);
	 setwidths = (gchar **) calloc(sizeof(*setwidths), a->count_of_ClipArrVar + 1);
	 for (i = 0; i < a->count_of_ClipArrVar; i++)
	    setwidths[i] = a->ClipVar_items_of_ClipArrVar[i].ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
      default:
	 setwidths = NULL;
      }
   switch (cv_spacings->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
      {
      case CHARACTER_type_of_ClipVarType:
	 spacings = (gchar **) calloc(sizeof(*spacings), 2);
	 spacings[0] = cv_spacings->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
	 break;
      case ARRAY_type_of_ClipVarType:
	 a = (ClipArrVar *) _clip_vptr(cv_spacings);
	 spacings = (gchar **) calloc(sizeof(*spacings), a->count_of_ClipArrVar + 1);
	 for (i = 0; i < a->count_of_ClipArrVar; i++)
	    spacings[i] = a->ClipVar_items_of_ClipArrVar[i].ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
      default:
	 spacings = NULL;
      }
   switch (cv_charsets->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
      {
      case CHARACTER_type_of_ClipVarType:
	 charsets = (gchar **) calloc(sizeof(*charsets), 2);
	 charsets[0] = cv_charsets->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
	 break;
      case ARRAY_type_of_ClipVarType:
	 a = (ClipArrVar *) _clip_vptr(cv_charsets);
	 charsets = (gchar **) calloc(sizeof(*charsets), a->count_of_ClipArrVar + 1);
	 for (i = 0; i < a->count_of_ClipArrVar; i++)
	    charsets[i] = a->ClipVar_items_of_ClipArrVar[i].ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
      default:
	 charsets = NULL;
      }
   CHECKOPT2(4, ARRAY_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   CHECKOPT2(5, ARRAY_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   CHECKOPT2(6, ARRAY_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   CHECKOPT2(7, ARRAY_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   CHECKOPT2(8, ARRAY_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   CHECKOPT2(9, ARRAY_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   gtk_font_selection_set_filter(GTK_FONT_SELECTION(cfsel->widget), filter_type, font_type, foundries, weights, slants, setwidths, spacings, charsets);
   if (foundries)
      free(foundries);
   if (weights)
      free(weights);
   if (slants)
      free(slants);
   if (setwidths)
      free(setwidths);
   if (spacings)
      free(spacings);
   if (charsets)
      free(charsets);
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
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   char *title = _clip_parc(ClipMachineMemory, 2);
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

/* Gets the currently-selected font. */
int
clip_GTK_FONTSELECTIONDIALOGGETFONT(ClipMachine * ClipMachineMemory)
{
   C_widget *cfsel = _fetch_cw_arg(ClipMachineMemory);
   C_object *cfont;
   GdkFont *font;
   CHECKCWID(cfsel, GTK_IS_FONT_SELECTION_DIALOG);
   font = gtk_font_selection_dialog_get_font(GTK_FONT_SELECTION_DIALOG(cfsel->widget));
   if (!font)
      return 0;
   cfont = _list_get_cobject(ClipMachineMemory, font);
   if (!cfont)
      cfont = _register_object(ClipMachineMemory, font, GDK_OBJECT_FONT, NULL, (coDestructor) gdk_object_font_destructor);
   if (cfont)
      _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cfont->obj);
   return 0;
 err:
   return 1;
}

/* Gets the currently-selected font name. */
int
clip_GTK_FONTSELECTIONDIALOGGETFONTNAME(ClipMachine * ClipMachineMemory)
{
   C_widget *cfsel = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(cfsel, GTK_IS_FONT_SELECTION_DIALOG);
   _clip_retc(ClipMachineMemory, gtk_font_selection_dialog_get_font_name(GTK_FONT_SELECTION_DIALOG(cfsel->widget)));
   return 0;
 err:
   return 1;
}

/* Sets the currently-selected font. */
int
clip_GTK_FONTSELECTIONDIALOGSETFONTNAME(ClipMachine * ClipMachineMemory)
{
   C_widget *cfsel = _fetch_cw_arg(ClipMachineMemory);
   gchar *font_name = _clip_parc(ClipMachineMemory, 2);
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
   _clip_retc(ClipMachineMemory, gtk_font_selection_dialog_get_preview_text(GTK_FONT_SELECTION_DIALOG(cfsel->widget)));
   return 0;
 err:
   return 1;
}

/* Sets the text displayed in the preview area. */
int
clip_GTK_FONTSELECTIONDIALOGSETPREVIEWTEXT(ClipMachine * ClipMachineMemory)
{
   C_widget *cfsel = _fetch_cw_arg(ClipMachineMemory);
   gchar *preview_text = _clip_parc(ClipMachineMemory, 2);
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

/* Sets one of the two font filters, to limit the fonts shown. */
/*
* fontsel	: a GtkFontSelectionDialog.
* filter_type	: which of the two font filters to set, either GTK_FONT_FILTER_BASE or
*		  GTK_FONT_FILTER_USER. The user filter can be changed by the user, but
*		  the base filter is permanent.
* font_type 	: the types of font to be shown. This is a bitwise combination of
*		  GTK_FONT_BITMAP, GTK_FONT_SCALABLE and GTK_FONT_SCALABLE_BITMAP, or
*		  GTK_FONT_ALL to show all three font types.
* foundries 	: a NULL-terminated array of strings containing foundry names which
*		  will be shown, or NULL to show all foundries.
* weights 	: a NULL-terminated array of strings containing weight names which
*		  will be shown, or NULL to show all weights.
* slants 		: a NULL-terminated array of strings containing slant names which will
*		  be shown, or NULL to show all slants.
* setwidths 	: a NULL-terminated array of strings containing setwidth names which
*		  will be shown, or NULL to show all setwidths.
* spacings 	: a NULL-terminated array of strings containing spacings which will be
*		  shown, or NULL to show all spacings.
* charsets 	: a NULL-terminated array of strings containing charset names which will
*		  be shown, or NULL to show all charsets.
*/
int
clip_GTK_FONTSELECTIONDIALOGSETFILTER(ClipMachine * ClipMachineMemory)
{
   C_widget *cfsel = _fetch_cw_arg(ClipMachineMemory);
   int filter_type = _clip_parni(ClipMachineMemory, 2);
   int font_type = _clip_parni(ClipMachineMemory, 3);
   ClipVar *cv_foundries = _clip_spar(ClipMachineMemory, 4);
   ClipVar *cv_weights = _clip_spar(ClipMachineMemory, 5);
   ClipVar *cv_slants = _clip_spar(ClipMachineMemory, 6);
   ClipVar *cv_setwidths = _clip_spar(ClipMachineMemory, 7);
   ClipVar *cv_spacings = _clip_spar(ClipMachineMemory, 8);
   ClipVar *cv_charsets = _clip_spar(ClipMachineMemory, 9);
   gchar **foundries, **weights, **slants, **setwidths, **spacings, **charsets;
   int i;
   ClipArrVar *a;
   CHECKCWID(cfsel, GTK_IS_FONT_SELECTION_DIALOG);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      filter_type = GTK_FONT_FILTER_BASE;
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      font_type = GTK_FONT_ALL;
   switch (cv_foundries->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
      {
      case CHARACTER_type_of_ClipVarType:
	 foundries = (gchar **) calloc(sizeof(*foundries), 2);
	 foundries[0] = cv_foundries->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
	 break;
      case ARRAY_type_of_ClipVarType:
	 a = (ClipArrVar *) _clip_vptr(cv_foundries);
	 foundries = (gchar **) calloc(sizeof(*foundries), a->count_of_ClipArrVar + 1);
	 for (i = 0; i < a->count_of_ClipArrVar; i++)
	    foundries[i] = a->ClipVar_items_of_ClipArrVar[i].ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
      default:
	 foundries = NULL;
      }
   switch (cv_weights->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
      {
      case CHARACTER_type_of_ClipVarType:
	 weights = (gchar **) calloc(sizeof(*weights), 2);
	 weights[0] = cv_weights->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
	 break;
      case ARRAY_type_of_ClipVarType:
	 a = (ClipArrVar *) _clip_vptr(cv_weights);
	 weights = (gchar **) calloc(sizeof(*weights), a->count_of_ClipArrVar + 1);
	 for (i = 0; i < a->count_of_ClipArrVar; i++)
	    weights[i] = a->ClipVar_items_of_ClipArrVar[i].ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
      default:
	 weights = NULL;
      }
   switch (cv_slants->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
      {
      case CHARACTER_type_of_ClipVarType:
	 slants = (gchar **) calloc(sizeof(*slants), 2);
	 slants[0] = cv_slants->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
	 break;
      case ARRAY_type_of_ClipVarType:
	 a = (ClipArrVar *) _clip_vptr(cv_slants);
	 slants = (gchar **) calloc(sizeof(*slants), a->count_of_ClipArrVar + 1);
	 for (i = 0; i < a->count_of_ClipArrVar; i++)
	    slants[i] = a->ClipVar_items_of_ClipArrVar[i].ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
      default:
	 slants = NULL;
      }
   switch (cv_setwidths->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
      {
      case CHARACTER_type_of_ClipVarType:
	 setwidths = (gchar **) calloc(sizeof(*setwidths), 2);
	 setwidths[0] = cv_setwidths->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
	 break;
      case ARRAY_type_of_ClipVarType:
	 a = (ClipArrVar *) _clip_vptr(cv_setwidths);
	 setwidths = (gchar **) calloc(sizeof(*setwidths), a->count_of_ClipArrVar + 1);
	 for (i = 0; i < a->count_of_ClipArrVar; i++)
	    setwidths[i] = a->ClipVar_items_of_ClipArrVar[i].ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
      default:
	 setwidths = NULL;
      }
   switch (cv_spacings->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
      {
      case CHARACTER_type_of_ClipVarType:
	 spacings = (gchar **) calloc(sizeof(*spacings), 2);
	 spacings[0] = cv_spacings->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
	 break;
      case ARRAY_type_of_ClipVarType:
	 a = (ClipArrVar *) _clip_vptr(cv_spacings);
	 spacings = (gchar **) calloc(sizeof(*spacings), a->count_of_ClipArrVar + 1);
	 for (i = 0; i < a->count_of_ClipArrVar; i++)
	    spacings[i] = a->ClipVar_items_of_ClipArrVar[i].ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
      default:
	 spacings = NULL;
      }
   switch (cv_charsets->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
      {
      case CHARACTER_type_of_ClipVarType:
	 charsets = (gchar **) calloc(sizeof(*charsets), 2);
	 charsets[0] = cv_charsets->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
	 break;
      case ARRAY_type_of_ClipVarType:
	 a = (ClipArrVar *) _clip_vptr(cv_charsets);
	 charsets = (gchar **) calloc(sizeof(*charsets), a->count_of_ClipArrVar + 1);
	 for (i = 0; i < a->count_of_ClipArrVar; i++)
	    charsets[i] = a->ClipVar_items_of_ClipArrVar[i].ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
      default:
	 charsets = NULL;
      }
   CHECKOPT2(4, ARRAY_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   CHECKOPT2(5, ARRAY_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   CHECKOPT2(6, ARRAY_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   CHECKOPT2(7, ARRAY_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   CHECKOPT2(8, ARRAY_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   CHECKOPT2(9, ARRAY_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   gtk_font_selection_dialog_set_filter(GTK_FONT_SELECTION_DIALOG(cfsel->widget), filter_type, font_type, foundries, weights, slants, setwidths, spacings, charsets);
   if (foundries)
      free(foundries);
   if (weights)
      free(weights);
   if (slants)
      free(slants);
   if (setwidths)
      free(setwidths);
   if (spacings)
      free(spacings);
   if (charsets)
      free(charsets);
   return 0;
 err:
   return 1;
}
