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
static SignalTable font_button_signals[] = {
   {"font-set", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_FONT_SET_SIGNAL},
   {"", NULL, NULL, 0}
};

/* Register widget signals table in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_font_button()
{
   return GTK_TYPE_FONT_BUTTON;
}

long
_clip_type_font_button()
{
   return GTK_WIDGET_FONT_BUTTON;
}

const char *
_clip_type_name_font_button()
{
   return "GTK_WIDGET_FONT_BUTTON";
}

int
clip_INIT___FONTBUTTON(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_font_button, _clip_type_name_font_button, _gtk_type_font_button, _gtk_type_button, font_button_signals);
   return 0;
}

/**********************************************************/

int
clip_GTK_FONTBUTTONNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   GtkWidget *wid = NULL;
   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);

   wid = gtk_font_button_new();

   if (!wid)
      goto err;

   cwid = _register_widget(ClipMachineMemory, wid, cv);

   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FONTBUTTONNEWWITHFONT(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   gchar *fontname = _clip_parc(ClipMachineMemory, 2);
   GtkWidget *wid = NULL;
   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   wid = gtk_font_button_new_with_font(fontname);

   if (!wid)
      goto err;

   cwid = _register_widget(ClipMachineMemory, wid, cv);

   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FONTBUTTONSETFONTNAME(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);
   gchar *fontname = _clip_parc(ClipMachineMemory, 2);

   CHECKCWID(cbtn, GTK_IS_FONT_BUTTON);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   gtk_font_button_set_font_name(GTK_FONT_BUTTON(cbtn->widget), fontname);
   return 0;
 err:
   return 1;
}

int
clip_GTK_FONTBUTTONGETFONTNAME(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cbtn, GTK_IS_FONT_BUTTON);

   _clip_retc(ClipMachineMemory, (gchar *) gtk_font_button_get_font_name(GTK_FONT_BUTTON(cbtn->widget)));
   return 0;
 err:
   return 1;
}

int
clip_GTK_FONTBUTTONSETSHOWSTYLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);
   gboolean show_style = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(cbtn, GTK_IS_FONT_BUTTON);
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_font_button_set_show_style(GTK_FONT_BUTTON(cbtn->widget), show_style);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FONTBUTTONGETSHOWSTYLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cbtn, GTK_IS_FONT_BUTTON);

   _clip_retl(ClipMachineMemory, gtk_font_button_get_show_style(GTK_FONT_BUTTON(cbtn->widget)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_FONTBUTTONSETSHOWSIZE(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);
   gboolean show_size = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(cbtn, GTK_IS_FONT_BUTTON);
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_font_button_set_show_size(GTK_FONT_BUTTON(cbtn->widget), show_size);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FONTBUTTONGETSHOWSIZE(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cbtn, GTK_IS_FONT_BUTTON);

   _clip_retl(ClipMachineMemory, gtk_font_button_get_show_size(GTK_FONT_BUTTON(cbtn->widget)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_FONTBUTTONSETUSEFONT(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);
   gboolean use_font = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(cbtn, GTK_IS_FONT_BUTTON);
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_font_button_set_use_font(GTK_FONT_BUTTON(cbtn->widget), use_font);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FONTBUTTONGETUSEFONT(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cbtn, GTK_IS_FONT_BUTTON);

   _clip_retl(ClipMachineMemory, gtk_font_button_get_use_font(GTK_FONT_BUTTON(cbtn->widget)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_FONTBUTTONSETUSESIZE(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);
   gboolean use_size = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(cbtn, GTK_IS_FONT_BUTTON);
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_font_button_set_use_size(GTK_FONT_BUTTON(cbtn->widget), use_size);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FONTBUTTONGETUSESIZE(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cbtn, GTK_IS_FONT_BUTTON);

   _clip_retl(ClipMachineMemory, gtk_font_button_get_use_size(GTK_FONT_BUTTON(cbtn->widget)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_FONTBUTTONSETTITLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);
   gchar *title = _clip_parc(ClipMachineMemory, 2);

   CHECKCWID(cbtn, GTK_IS_FONT_BUTTON);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(title);
   gtk_font_button_set_title(GTK_FONT_BUTTON(cbtn->widget), title);
   FREE_TEXT(title);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FONTBUTTONGETTITLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);
   gchar *title;

   CHECKCWID(cbtn, GTK_IS_FONT_BUTTON);

   title = (gchar *) gtk_font_button_get_title(GTK_FONT_BUTTON(cbtn->widget));
   LOCALE_FROM_UTF(title);
   _clip_retc(ClipMachineMemory, title);
   FREE_TEXT(title);

   return 0;
 err:
   return 1;
}
