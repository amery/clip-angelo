/*
    Copyright (C) 2002  ITK
    Author  : Alexey M. Tkachenko <alexey@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg.h"

#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "ci_clip-gtk.ch"
#include "ci_clip-gtk.h"
#include "ci_clip-gdk.h"

/**********************************************************/

/* Draws a rectangular outline or filled rectangle, using the foreground
 * color and other attributes of the GdkGC. */

/* Note: A rectangle drawn filled is 1 pixel smaller in both dimensions
 * than a rectangle outlined. Calling gdk_draw_rectangle (window, gc, TRUE, 0, 0, 20, 20)
 * results in a filled rectangle 20 pixels wide and 20 pixels high. Calling
 * gdk_draw_rectangle (window, gc, FALSE, 0, 0, 20, 20) results in an outlined
 * rectangle with corners at (0, 0), (0, 20), (20, 20), and (20, 0), which makes
 * it 21 pixels wide and 21 pixels high. */
int
clip_GDK_DRAWRECTANGLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   C_object *cgc = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gint      filled = BOOL_OPTION(ClipMachineMemory, 3, TRUE);

   gint      x = _clip_parni(ClipMachineMemory, 4);

   gint      y = _clip_parni(ClipMachineMemory, 5);

   gint      width = _clip_parni(ClipMachineMemory, 6);

   gint      height = _clip_parni(ClipMachineMemory, 7);

   GdkDrawable *drw = NULL;

   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKOPT2(2, NUMERIC_type_of_ClipVarType, MAP_type_of_ClipVarType);
   CHECKCOBJ(cgc, GDK_IS_GC(cgc));
   CHECKOPT(3, LOGICAL_type_of_ClipVarType);
   CHECKOPT(4, NUMERIC_type_of_ClipVarType);
   CHECKOPT(5, NUMERIC_type_of_ClipVarType);
   CHECKOPT(6, NUMERIC_type_of_ClipVarType);
   CHECKOPT(7, NUMERIC_type_of_ClipVarType);
   drw = cwid->widget->window;
   if (GTK_IS_PIXMAP(cwid->widget))
      drw = GTK_PIXMAP(cwid->widget)->pixmap;
   gdk_draw_rectangle(drw, GDK_GC(cgc->object), filled, x, y, width, height);
   return 0;
 err:
   return 1;
}

/* Alena */
/* Draws a line, using the foreground color and other attributes of the GdkGC. */
int
clip_GDK_DRAWLINE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   C_object *cgc = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gint      x1 = _clip_parni(ClipMachineMemory, 3);

   gint      y1 = _clip_parni(ClipMachineMemory, 4);

   gint      x2 = _clip_parni(ClipMachineMemory, 5);

   gint      y2 = _clip_parni(ClipMachineMemory, 6);

   GdkDrawable *drw = NULL;

   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKOPT2(2, NUMERIC_type_of_ClipVarType, MAP_type_of_ClipVarType);
   CHECKCOBJ(cgc, GDK_IS_GC(cgc));
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, NUMERIC_type_of_ClipVarType);
   CHECKOPT(5, NUMERIC_type_of_ClipVarType);
   CHECKOPT(6, NUMERIC_type_of_ClipVarType);

   drw = cwid->widget->window;
   if (GTK_IS_PIXMAP(cwid->widget))
      drw = GTK_PIXMAP(cwid->widget)->pixmap;
   gdk_draw_line(drw, GDK_GC(cgc->object), x1, y1, x2, y2);
   return 0;
 err:
   return 1;
}

/* Alena */
/* Draws a number of characters in the given font or fontset. */
int
clip_GDK_DRAWTEXT(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   C_object *cgc = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_object *font = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   gint      x = _clip_parni(ClipMachineMemory, 4);

   gint      y = _clip_parni(ClipMachineMemory, 5);

   gchar    *text = _clip_parc(ClipMachineMemory, 6);

   gint      length = _clip_parni(ClipMachineMemory, 7);

   GdkDrawable *drw = NULL;

   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKOPT2(2, NUMERIC_type_of_ClipVarType, MAP_type_of_ClipVarType);
   CHECKCOBJ(cgc, GDK_IS_GC(cgc));
   CHECKOPT2(3, NUMERIC_type_of_ClipVarType, MAP_type_of_ClipVarType);
   CHECKCOBJ(cgc, GDK_IS_FONT(font));
   CHECKOPT(4, NUMERIC_type_of_ClipVarType);
   CHECKOPT(5, NUMERIC_type_of_ClipVarType);
   CHECKOPT(6, CHARACTER_type_of_ClipVarType);
   CHECKOPT(7, NUMERIC_type_of_ClipVarType);
   drw = cwid->widget->window;
   if (GTK_IS_PIXMAP(cwid->widget))
      drw = GTK_PIXMAP(cwid->widget)->pixmap;
   LOCALE_TO_UTF(text);
   gdk_draw_text(drw, GDK_FONT(font->object), GDK_GC(cgc->object), x, y, text, length);
   FREE_TEXT(text);
   return 0;
 err:
   return 1;
}

/* Alena */
int
clip_GDK_DRAWPIXMAP(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   C_object *cgc = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_widget *cpix = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   gint      xsrc = _clip_parni(ClipMachineMemory, 4);

   gint      ysrc = _clip_parni(ClipMachineMemory, 5);

   gint      xdest = _clip_parni(ClipMachineMemory, 6);

   gint      ydest = _clip_parni(ClipMachineMemory, 7);

   gint      width = _clip_parni(ClipMachineMemory, 8);

   gint      height = _clip_parni(ClipMachineMemory, 9);

   GdkDrawable *drw = NULL;

   CHECKARG2(1, NUMERIC_type_of_ClipVarType, MAP_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKARG2(2, NUMERIC_type_of_ClipVarType, MAP_type_of_ClipVarType);
   CHECKCOBJ(cgc, GDK_IS_GC(cgc));
   CHECKARG2(3, NUMERIC_type_of_ClipVarType, MAP_type_of_ClipVarType);
   CHECKCWID(cpix, GTK_IS_WIDGET);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);
   CHECKARG(5, NUMERIC_type_of_ClipVarType);
   CHECKARG(6, NUMERIC_type_of_ClipVarType);
   CHECKARG(7, NUMERIC_type_of_ClipVarType);
   CHECKARG(8, NUMERIC_type_of_ClipVarType);
   CHECKARG(9, NUMERIC_type_of_ClipVarType);
   drw = cwid->widget->window;
   if (GTK_IS_PIXMAP(cwid->widget))
      drw = GTK_PIXMAP(cwid->widget)->pixmap;
   gdk_draw_pixmap(drw, GDK_GC(cgc->object), GTK_PIXMAP(cpix->widget)->pixmap, xsrc, ysrc, xdest, ydest, width, height);
   return 0;
 err:
   return 1;
}
