/*
    Copyright (C) 2004  ITK
    Author  : Kornilova E.V. <alena@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg2.h"

#include <gtk/gtk.h>

#include "ci_clip-gtk2.ch"
#include "ci_clip-gtk2.h"

/**********************************************************/
/* Signals table */
static SignalTable color_button_signals[] = {
   {"color-set", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_COLOR_SET_SIGNAL},
   {"", NULL, NULL, 0}
};

/* Register widget signals table in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_color_button()
{
   return GTK_TYPE_COLOR_BUTTON;
}

long
_clip_type_color_button()
{
   return GTK_WIDGET_COLOR_BUTTON;
}

const char *
_clip_type_name_color_button()
{
   return "GTK_WIDGET_COLOR_BUTTON";
}

int
clip_INIT___COLORBUTTON(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_color_button, _clip_type_name_color_button,
		    _gtk_type_color_button, _gtk_type_button, color_button_signals);
   return 0;
}

/**********************************************************/

int
clip_GTK_COLORBUTTONNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   GtkWidget *wid = NULL;

   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);

   wid = gtk_color_button_new();

   if (!wid)
      goto err;

   cwid = _register_widget(ClipMachineMemory, wid, cv);

   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

int
clip_GTK_COLORBUTTONNEWWITHCOLOR(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   ClipVar  *cclr = _clip_spar(ClipMachineMemory, 2);

   GtkWidget *wid = NULL;

   C_widget *cwid;

   GdkColor  color;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKARG(2, MAP_type_of_ClipVarType);

   _map_colors_to_gdk(ClipMachineMemory, cclr, &color);

   wid = gtk_color_button_new_with_color(&color);

   if (!wid)
      goto err;

   cwid = _register_widget(ClipMachineMemory, wid, cv);

   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

int
clip_GTK_COLORBUTTONSETCOLOR(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   ClipVar  *cclr = _clip_spar(ClipMachineMemory, 2);

   GdkColor  color;

   CHECKCWID(cbtn, GTK_IS_COLOR_BUTTON);
   CHECKARG(2, MAP_type_of_ClipVarType);

   _map_colors_to_gdk(ClipMachineMemory, cclr, &color);

   gtk_color_button_set_color(GTK_COLOR_BUTTON(cbtn->widget), &color);
   return 0;
 err:
   return 1;
}

int
clip_GTK_COLORBUTTONGETCOLOR(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   ClipVar  *cclr = RETPTR(ClipMachineMemory);

   GdkColor  color;

   CHECKCWID(cbtn, GTK_IS_COLOR_BUTTON);

   gtk_color_button_get_color(GTK_COLOR_BUTTON(cbtn->widget), &color);
   _gdk_color_to_map(ClipMachineMemory, color, cclr);

   return 0;
 err:
   return 1;
}

int
clip_GTK_COLORBUTTONSETALPHA(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   guint16   alpha = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(cbtn, GTK_IS_COLOR_BUTTON);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_color_button_set_alpha(GTK_COLOR_BUTTON(cbtn->widget), alpha);

   return 0;
 err:
   return 1;
}

int
clip_GTK_COLORBUTTONGETALPHA(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cbtn, GTK_IS_COLOR_BUTTON);

   _clip_retni(ClipMachineMemory, gtk_color_button_get_alpha(GTK_COLOR_BUTTON(cbtn->widget)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_COLORBUTTONSETUSEALPHA(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   gboolean  alpha = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(cbtn, GTK_IS_COLOR_BUTTON);
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_color_button_set_use_alpha(GTK_COLOR_BUTTON(cbtn->widget), alpha);

   return 0;
 err:
   return 1;
}

int
clip_GTK_COLORBUTTONGETUSEALPHA(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cbtn, GTK_IS_COLOR_BUTTON);

   _clip_retl(ClipMachineMemory, gtk_color_button_get_use_alpha(GTK_COLOR_BUTTON(cbtn->widget)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_COLORBUTTONSETTITLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   gchar    *title = _clip_parc(ClipMachineMemory, 2);

   CHECKCWID(cbtn, GTK_IS_COLOR_BUTTON);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(title);

   gtk_color_button_set_title(GTK_COLOR_BUTTON(cbtn->widget), title);
   FREE_TEXT(title);
   return 0;
 err:
   return 1;
}

int
clip_GTK_COLORBUTTONGETTITLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   gchar    *title;

   CHECKCWID(cbtn, GTK_IS_COLOR_BUTTON);

   title = (gchar *) gtk_color_button_get_title(GTK_COLOR_BUTTON(cbtn->widget));
   LOCALE_FROM_UTF(title);
   _clip_retc(ClipMachineMemory, title);
   FREE_TEXT(title);
   return 0;
 err:
   return 1;
}
