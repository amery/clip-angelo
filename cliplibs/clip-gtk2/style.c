/*
    Copyright (C) 2004  ITK
    Author  : Elena V. Kornilova <alena@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include <string.h>
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg2.h"

#include <gtk/gtk.h>

#include "ci_clip-gtk2.ch"
#include "ci_clip-gtk2.h"

int
clip_GTK_STYLENEW(ClipMachine * ClipMachineMemory)
{
   GtkStyle *style;

   ClipVar  *cstyle = RETPTR(ClipMachineMemory);

   _clip_map(ClipMachineMemory, cstyle);
   style = gtk_style_new();

   _style_to_map(ClipMachineMemory, style, cstyle);
   return 0;
}

int
clip_GTK_STYLECOPY(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cs = _clip_spar(ClipMachineMemory, 1);

   ClipVar  *cstyle = RETPTR(ClipMachineMemory);

   GtkStyle *style = 0, *newstyle;

   CHECKARG(1, MAP_type_of_ClipVarType);
   _map_to_style(ClipMachineMemory, cs, style);
   newstyle = gtk_style_copy(style);

   _style_to_map(ClipMachineMemory, newstyle, cstyle);
   return 0;
 err:
   return 1;
}

int
clip_GTK_STYLEATTACH(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cs = _clip_spar(ClipMachineMemory, 1);

   C_widget *cwin = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   ClipVar  *cstyle = RETPTR(ClipMachineMemory);

   GtkStyle *style = 0, *newstyle;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCWID(cwin, GTK_IS_WINDOW);
   _map_to_style(ClipMachineMemory, cs, style);
   newstyle = gtk_style_attach(style, GDK_WINDOW(cwin->widget->window));

   _style_to_map(ClipMachineMemory, newstyle, cstyle);
   return 0;
 err:
   return 1;
}

int
clip_GTK_STYLEDETACH(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cs = _clip_spar(ClipMachineMemory, 1);

   GtkStyle *style = 0;

   CHECKARG(1, MAP_type_of_ClipVarType);
   _map_to_style(ClipMachineMemory, cs, style);
   gtk_style_detach(style);

   return 0;
 err:
   return 1;
}

int
clip_GTK_STYLESETBACKGROUND(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cs = _clip_spar(ClipMachineMemory, 1);

   C_widget *cwin = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   GtkStateType t = _clip_parni(ClipMachineMemory, 3);

   GtkStyle *style = 0;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCWID(cwin, GTK_IS_WINDOW);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);

   _map_to_style(ClipMachineMemory, cs, style);
   gtk_style_set_background(style, GDK_WINDOW(cwin->widget->window), t);

   return 0;
 err:
   return 1;
}

int
clip_GTK_STYLEAPPLYDEFAULTBACKGROUND(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cs = _clip_spar(ClipMachineMemory, 1);

   C_widget *cwin = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gboolean  set_bg = _clip_parl(ClipMachineMemory, 3);

   GtkStateType t = _clip_parni(ClipMachineMemory, 4);

   ClipVar  *cv = _clip_spar(ClipMachineMemory, 5);

   gint      x = _clip_parni(ClipMachineMemory, 6);

   gint      y = _clip_parni(ClipMachineMemory, 7);

   gint      width = _clip_parni(ClipMachineMemory, 8);

   gint      height = _clip_parni(ClipMachineMemory, 9);

   GtkStyle *style = 0;

   GdkRectangle rect;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCWID(cwin, GTK_IS_WINDOW);
   CHECKARG(3, LOGICAL_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);
   CHECKARG(5, MAP_type_of_ClipVarType);
   CHECKARG(6, NUMERIC_type_of_ClipVarType);
   CHECKARG(7, NUMERIC_type_of_ClipVarType);
   CHECKARG(8, NUMERIC_type_of_ClipVarType);
   CHECKARG(9, NUMERIC_type_of_ClipVarType);

   _map_to_style(ClipMachineMemory, cs, style);
   _map_put_gdk_rectangle(ClipMachineMemory, cv, &rect);
   gtk_style_apply_default_background(style, GDK_WINDOW(cwin->widget->window), set_bg, t, &rect, x, y, width, height);

   return 0;
 err:
   return 1;
}
