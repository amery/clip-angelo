/*
    Copyright (C) 2001-2004  ITK
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

/*********************** SIGNALS ***************************/
/* Signals table */
static SignalTable colorsel_signals[] = {
   {"color-changed", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_COLOR_CHANGED_SIGNAL},
   {"", NULL, NULL, 0}
};

/***********************************************************/

/* Register color selection in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_color_selection()
{
   return GTK_TYPE_COLOR_SELECTION;
}

long
_clip_type_color_selection()
{
   return GTK_WIDGET_COLOR_SELECTION;
}

const char *
_clip_type_name_color_selection()
{
   return "GTK_WIDGET_COLOR_SELECTION";
}

/* Register color selection dialog in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_color_selection_dialog()
{
   return GTK_TYPE_COLOR_SELECTION_DIALOG;
}

long
_clip_type_color_selection_dialog()
{
   return GTK_WIDGET_COLOR_SELECTION_DIALOG;
}

const char *
_clip_type_name_color_selection_dialog()
{
   return "GTK_WIDGET_COLOR_SELECTION_DIALOG";
}

int
clip_INIT___COLORSELECTION(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_color_selection,
		    _clip_type_name_color_selection, _gtk_type_color_selection, _gtk_type_vbox, colorsel_signals);
   _wtype_table_put(_clip_type_color_selection_dialog,
		    _clip_type_name_color_selection_dialog, _gtk_type_color_selection_dialog, _gtk_type_window, NULL);
   return 0;
}

/**********************************************************/

/****  Color selection constructor ****/
int
clip_GTK_COLORSELECTIONNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   GtkWidget *wid = NULL;

   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);

   wid = gtk_color_selection_new();
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);

   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

/****  ------------------ ****/

/* Sets the policy controlling when the color_changed signals are emitted.
   The available policies are:

    GTK_UPDATE_CONTINUOUS - signals are sent continuously as the color selection changes.

    GTK_UPDATE_DISCONTINUOUS - signals are sent only when the mouse button is released.

    GTK_UPDATE_DELAYED - signals are sent when the mouse button is released or when the
	mouse has been motionless for a period of time.
*/
int
clip_GTK_COLORSELECTIONSETUPDATEPOLICY(ClipMachine * ClipMachineMemory)
{
   C_widget *ccsel = _fetch_cw_arg(ClipMachineMemory);

   GtkUpdateType policy = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(ccsel, GTK_IS_COLOR_SELECTION);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_color_selection_set_update_policy(GTK_COLOR_SELECTION(ccsel->widget), policy);
   return 0;
 err:
   return 1;
}

/* Controls whether opacity can be set with the GtkColorSelection. If this
 * functionality is enabled, the necessary additional widgets are added to
 * the GtkColorSelection and the opacity value can be retrieved via the fourth
 * value in the color array returned by the gtk_color_selection_get_color() function. */
int
clip_GTK_COLORSELECTIONSETHASOPACITYCONTROL(ClipMachine * ClipMachineMemory)
{
   C_widget *ccsel = _fetch_cw_arg(ClipMachineMemory);

   gboolean  use_opacity = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(ccsel, GTK_IS_COLOR_SELECTION);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      use_opacity = TRUE;
   gtk_color_selection_set_has_opacity_control(GTK_COLOR_SELECTION(ccsel->widget), use_opacity);
   return 0;
 err:
   return 1;
}

int
clip_GTK_COLORSELECTIONGETHASOPACITYCONTROL(ClipMachine * ClipMachineMemory)
{
   C_widget *ccsel = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(ccsel, GTK_IS_COLOR_SELECTION);
   _clip_retl(ClipMachineMemory, gtk_color_selection_get_has_opacity_control(GTK_COLOR_SELECTION(ccsel->widget)));
   return 0;
 err:
   return 1;
}

int
clip_GTK_COLORSELECTIONSETHASPALETTE(ClipMachine * ClipMachineMemory)
{
   C_widget *ccsel = _fetch_cw_arg(ClipMachineMemory);

   gboolean  use_palette = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(ccsel, GTK_IS_COLOR_SELECTION);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      use_palette = TRUE;
   gtk_color_selection_set_has_palette(GTK_COLOR_SELECTION(ccsel->widget), use_palette);
   return 0;
 err:
   return 1;
}

int
clip_GTK_COLORSELECTIONGETHASPALETTE(ClipMachine * ClipMachineMemory)
{
   C_widget *ccsel = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(ccsel, GTK_IS_COLOR_SELECTION);
   _clip_retl(ClipMachineMemory, gtk_color_selection_get_has_palette(GTK_COLOR_SELECTION(ccsel->widget)));
   return 0;
 err:
   return 1;
}

int
clip_GTK_COLORSELECTIONGETCURRENTALPHA(ClipMachine * ClipMachineMemory)
{
   C_widget *ccsel = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(ccsel, GTK_IS_COLOR_SELECTION);
   _clip_retni(ClipMachineMemory, (int) gtk_color_selection_get_current_alpha(GTK_COLOR_SELECTION(ccsel->widget)));
   return 0;
 err:
   return 1;
}

int
clip_GTK_COLORSELECTIONSETCURRENTALPHA(ClipMachine * ClipMachineMemory)
{
   C_widget *ccsel = _fetch_cw_arg(ClipMachineMemory);

   guint16   alpha = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(ccsel, GTK_IS_COLOR_SELECTION);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_color_selection_set_current_alpha(GTK_COLOR_SELECTION(ccsel->widget), alpha);
   return 0;
 err:
   return 1;
}

int
clip_GTK_COLORSELECTIONGETCURRENTCOLOR(ClipMachine * ClipMachineMemory)
{
   C_widget *ccsel = _fetch_cw_arg(ClipMachineMemory);

   GdkColor  color;

   CHECKCWID(ccsel, GTK_IS_COLOR_SELECTION);

   gtk_color_selection_get_current_color(GTK_COLOR_SELECTION(ccsel->widget), &color);

   memset(RETPTR(ClipMachineMemory), 0, sizeof(ClipVar));
   _clip_map(ClipMachineMemory, RETPTR(ClipMachineMemory));
   _gdk_color_to_map(ClipMachineMemory, color, RETPTR(ClipMachineMemory));

   return 0;
 err:
   return 1;
}

int
clip_GTK_COLORSELECTIONSETCURRENTCOLOR(ClipMachine * ClipMachineMemory)
{
   C_widget *ccsel = _fetch_cw_arg(ClipMachineMemory);

   ClipVar  *cvclr = _clip_spar(ClipMachineMemory, 2);

   GdkColor  color;

   CHECKCWID(ccsel, GTK_IS_COLOR_SELECTION);
   CHECKARG(2, MAP_type_of_ClipVarType);

   _map_to_gdk_color(ClipMachineMemory, &color, cvclr);

   gtk_color_selection_set_current_color(GTK_COLOR_SELECTION(ccsel->widget), &color);

   return 0;
 err:
   return 1;
}

int
clip_GTK_COLORSELECTIONGETPREVIOUSALPHA(ClipMachine * ClipMachineMemory)
{
   C_widget *ccsel = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(ccsel, GTK_IS_COLOR_SELECTION);
   _clip_retni(ClipMachineMemory, (int) gtk_color_selection_get_previous_alpha(GTK_COLOR_SELECTION(ccsel->widget)));
   return 0;
 err:
   return 1;
}

int
clip_GTK_COLORSELECTIONSETPREVIOUSALPHA(ClipMachine * ClipMachineMemory)
{
   C_widget *ccsel = _fetch_cw_arg(ClipMachineMemory);

   guint16   alpha = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(ccsel, GTK_IS_COLOR_SELECTION);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_color_selection_set_previous_alpha(GTK_COLOR_SELECTION(ccsel->widget), alpha);
   return 0;
 err:
   return 1;
}

int
clip_GTK_COLORSELECTIONGETPREVIOUSCOLOR(ClipMachine * ClipMachineMemory)
{
   C_widget *ccsel = _fetch_cw_arg(ClipMachineMemory);

   GdkColor  color;

   CHECKCWID(ccsel, GTK_IS_COLOR_SELECTION);

   gtk_color_selection_get_previous_color(GTK_COLOR_SELECTION(ccsel->widget), &color);

   memset(RETPTR(ClipMachineMemory), 0, sizeof(ClipVar));
   _clip_map(ClipMachineMemory, RETPTR(ClipMachineMemory));
   _gdk_color_to_map(ClipMachineMemory, color, RETPTR(ClipMachineMemory));

   return 0;
 err:
   return 1;
}

int
clip_GTK_COLORSELECTIONSETPREVIOUSCOLOR(ClipMachine * ClipMachineMemory)
{
   C_widget *ccsel = _fetch_cw_arg(ClipMachineMemory);

   ClipVar  *cvclr = _clip_spar(ClipMachineMemory, 2);

   GdkColor  color;

   CHECKCWID(ccsel, GTK_IS_COLOR_SELECTION);
   CHECKARG(2, MAP_type_of_ClipVarType);

   _map_to_gdk_color(ClipMachineMemory, &color, cvclr);

   gtk_color_selection_set_previous_color(GTK_COLOR_SELECTION(ccsel->widget), &color);

   return 0;
 err:
   return 1;
}

int
clip_GTK_COLORSELECTIONISADJUSTING(ClipMachine * ClipMachineMemory)
{
   C_widget *ccsel = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(ccsel, GTK_IS_COLOR_SELECTION);

   _clip_retl(ClipMachineMemory, gtk_color_selection_is_adjusting(GTK_COLOR_SELECTION(ccsel->widget)));

   return 0;
 err:
   return 1;
}

/* gtk_ColorSelectionPaletteFromString(sPalette, @aColor, @nLenColor)--> TRUE||FALSE */
int
clip_GTK_COLORSELECTIONPALETTEFROMSTRING(ClipMachine * ClipMachineMemory)
{
   gchar    *palette = _clip_parc(ClipMachineMemory, 1);

   ClipVar  *cvcolor = _clip_spar(ClipMachineMemory, 2);

   GdkColor *color;

   gint      len;

   gboolean  ret;

   CHECKARG(1, CHARACTER_type_of_ClipVarType);

   ret = gtk_color_selection_palette_from_string(palette, &color, &len);

   if (ret)
    {
       long      i, d = len;

       _clip_array(ClipMachineMemory, cvcolor, 1, &d);
       for (i = 0; i < len; i++)
	{
	   ClipVar  *cv = 0;

	   memset(cv, 0, sizeof(ClipVar));
	   _clip_map(ClipMachineMemory, cv);
	   _gdk_color_to_map(ClipMachineMemory, color[i], cv);
	   _clip_aset(ClipMachineMemory, cvcolor, cv, 1, &i);
	}
       _clip_storni(ClipMachineMemory, len, 3, 0);
    }

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

int
clip_GTK_COLORSELECTIONPALETTETOSTRING(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cvcolor = _clip_spar(ClipMachineMemory, 1);

   gint      len = INT_OPTION(ClipMachineMemory, 2, 0);

   gchar    *palette = 0;

   CHECKARG(1, ARRAY_type_of_ClipVarType);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   if (len > 0)
    {
       GdkColor  color[len];

       gint      i;

       for (i = 0; i < len; i++)
	{
	   _map_to_gdk_color(ClipMachineMemory, &color[i], &cvcolor->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar[i]);
	}
       palette = gtk_color_selection_palette_to_string(color, len);
    }

   _clip_retc(ClipMachineMemory, palette);
   return 0;
 err:
   return 1;
}

/* Sets the color in the GtkColorSelection. The widgets are updated to
 * reflect the new color. */
int
clip_GTK_COLORSELECTIONSETCOLOR(ClipMachine * ClipMachineMemory)
{
   C_widget *ccsel = _fetch_cw_arg(ClipMachineMemory);

   ClipVar  *mcolor = _clip_spar(ClipMachineMemory, 2);

   gdouble   color[4];

   gdouble   k, max;

   CHECKCWID(ccsel, GTK_IS_COLOR_SELECTION);
   CHECKOPT(2, MAP_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      gtk_color_selection_get_color(GTK_COLOR_SELECTION(ccsel->widget), color);
   else
      _map_get_colors(ClipMachineMemory, mcolor, color);
   color[0] = color[0] > 0 ? color[0] : 1;
   color[1] = color[1] > 0 ? color[1] : 1;
   color[2] = color[2] > 0 ? color[2] : 1;
   max = color[0] > color[1] ? color[0] : color[1];
   max = max > color[2] ? max : color[2];
   max = max > 0 ? max : 1;
   k = 1 / max;
   color[0] *= k;
   color[1] *= k;
   color[2] *= k;
   gtk_color_selection_set_color(GTK_COLOR_SELECTION(ccsel->widget), color);
   return 0;
 err:
   return 1;
}

/* Retrieve the currently selected color value. */
int
clip_GTK_COLORSELECTIONGETCOLOR(ClipMachine * ClipMachineMemory)
{
   C_widget *ccsel = _fetch_cw_arg(ClipMachineMemory);

   GtkColorSelection *colorsel;

   ClipVar   mcolor;

   gdouble   color[4];

   CHECKCWID(ccsel, GTK_IS_COLOR_SELECTION);
   colorsel = GTK_COLOR_SELECTION(ccsel->widget);
   gtk_color_selection_get_color(GTK_COLOR_SELECTION(ccsel->widget), color);
   color[0] *= CLIP_GTK_MAX_COLOR;
   color[1] *= CLIP_GTK_MAX_COLOR;
   color[2] *= CLIP_GTK_MAX_COLOR;
   memset(&mcolor, 0, sizeof(mcolor));
   _clip_map(ClipMachineMemory, &mcolor);
   _map_put_colors(ClipMachineMemory, &mcolor, color);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &mcolor);
   return 0;
 err:
   return 1;
}

/***************************************************************************/

/************        * Color selection dialog *                 ************/

/***************************************************************************/

/****  Color selection dialog constructor ****/
int
clip_GTK_COLORSELECTIONDIALOGNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   char     *title = _clip_parc(ClipMachineMemory, 2);

   GtkWidget *wid = NULL;

   C_widget *cwid, *ccolorsel, *cokbtn, *ccancelbtn, *chlpbtn;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      title = "\0";
   LOCALE_TO_UTF(title);
   wid = gtk_color_selection_dialog_new(title);
   FREE_TEXT(title);
   cwid = _register_widget(ClipMachineMemory, wid, cv);

   ccolorsel = _register_widget(ClipMachineMemory, GTK_COLOR_SELECTION_DIALOG(wid)->colorsel, NULL);
   cokbtn = _register_widget(ClipMachineMemory, GTK_COLOR_SELECTION_DIALOG(wid)->ok_button, NULL);
   ccancelbtn = _register_widget(ClipMachineMemory, GTK_COLOR_SELECTION_DIALOG(wid)->cancel_button, NULL);
   chlpbtn = _register_widget(ClipMachineMemory, GTK_COLOR_SELECTION_DIALOG(wid)->help_button, NULL);

   if (ccolorsel)
      _clip_madd(ClipMachineMemory, &cwid->obj, HASH_COLORSEL, &ccolorsel->obj);
   if (cokbtn)
      _clip_madd(ClipMachineMemory, &cwid->obj, HASH_OKBUTTON, &cokbtn->obj);
   if (ccancelbtn)
      _clip_madd(ClipMachineMemory, &cwid->obj, HASH_CANCELBUTTON, &ccancelbtn->obj);
   if (chlpbtn)
      _clip_madd(ClipMachineMemory, &cwid->obj, HASH_HELPBUTTON, &chlpbtn->obj);

   if (ccolorsel)
      _clip_destroy(ClipMachineMemory, &ccolorsel->obj);
   if (cokbtn)
      _clip_destroy(ClipMachineMemory, &cokbtn->obj);
   if (ccancelbtn)
      _clip_destroy(ClipMachineMemory, &ccancelbtn->obj);
   if (chlpbtn)
      _clip_destroy(ClipMachineMemory, &chlpbtn->obj);

   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

/****  ------------------ ****/
