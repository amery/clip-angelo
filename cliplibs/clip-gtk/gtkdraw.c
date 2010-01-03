/*
    Copyright (C) 2002  ITK
    Author  : Elena Kornilova <alena@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg.h"

#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "ci_clip-gtk.ch"
#include "ci_clip-gtk.h"

#define GDK_GC(p)		((GdkGC*)(p))
#define GDK_IS_GC(obj)		(((C_object*)(obj))->type == GDK_OBJECT_GC)
#define GDK_FONT(p)		((GdkFont*)(p))
#define GDK_IS_FONT(obj)	(((C_object*)(obj))->type == GDK_OBJECT_FONT)

int
clip_GTK_DRAWHLINE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   ClipVar  *mstyle = _clip_spar(ClipMachineMemory, 3);

   gint      x1 = _clip_parni(ClipMachineMemory, 4);

   gint      x2 = _clip_parni(ClipMachineMemory, 5);

   gint      y = _clip_parni(ClipMachineMemory, 6);

   GtkStyle *style;

   GdkWindow *win = NULL;

   GdkRectangle *area = 0;

   CHECKCWID(cwin, GTK_IS_WIDGET);
   CHECKARG(2, MAP_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKARG2(3, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, NUMERIC_type_of_ClipVarType);
   CHECKOPT(5, NUMERIC_type_of_ClipVarType);
   CHECKOPT(6, NUMERIC_type_of_ClipVarType);

   win = cwin->widget->window;
   style = gtk_style_new();
   _map_to_style(ClipMachineMemory, mstyle, style);

   gtk_widget_set_style(cwin->widget, style);

   area->x = 0;
   area->y = 0;
   area->width = 300;
   area->height = 300;
   gtk_draw_hline(style, win, GTK_STATE_NORMAL, x1, x2, y);
   return 0;
 err:
   return 1;
}

int
clip_GTK_DRAWBOX(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   ClipVar  *mstyle = _clip_spar(ClipMachineMemory, 2);

   gint      shadowType = INT_OPTION(ClipMachineMemory, 3, 0);

   gint      x1 = _clip_parni(ClipMachineMemory, 4);

   gint      x2 = _clip_parni(ClipMachineMemory, 5);

   gint      width = _clip_parni(ClipMachineMemory, 6);

   gint      height = _clip_parni(ClipMachineMemory, 7);

   GtkStyle *style;

   GdkDrawable *drw = NULL;

   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, NUMERIC_type_of_ClipVarType);
   CHECKOPT(5, NUMERIC_type_of_ClipVarType);
   CHECKOPT(6, NUMERIC_type_of_ClipVarType);
   CHECKOPT(7, NUMERIC_type_of_ClipVarType);

   style = gtk_style_new();
   _map_to_style(ClipMachineMemory, mstyle, style);

   drw = cwid->widget->window;
   if (GTK_IS_PIXMAP(cwid->widget))
      drw = GTK_PIXMAP(cwid->widget)->pixmap;

   gtk_draw_box(style, drw, GTK_STATE_NORMAL, shadowType, x1, x2, width, height);

   return 0;
 err:
   return 1;
}
