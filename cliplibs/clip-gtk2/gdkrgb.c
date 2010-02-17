/*
    Copyright (C) 2004  JARA
    Author  : Sergio Zayas <icaro.maneton@lycos.es>
    License : (GPL) http://www.itk.ru/clipper/license.html
    mail : icaro.maneton@lycos.es
*/

#include <string.h>
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg2.h"

#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "ci_clip-gtk2.ch"
#include "ci_clip-gtk2.h"

int
clip_GDK_RGBINIT(ClipMachine * ClipMachineMemory)
{
   gdk_rgb_init();
   return 0;
}

int
clip_GDK_DRAWRGBIMAGE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   C_object *cgc = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   gint x = _clip_parni(ClipMachineMemory, 3);
   gint y = _clip_parni(ClipMachineMemory, 4);
   gint width = _clip_parni(ClipMachineMemory, 5);
   gint height = _clip_parni(ClipMachineMemory, 6);
   GdkRgbDither dith = INT_OPTION(ClipMachineMemory, 7, 0);
   guchar *rgb = (guchar *) _clip_parc(ClipMachineMemory, 8);
   gint rowstride = _clip_parni(ClipMachineMemory, 9);

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKARG(2, MAP_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);
   CHECKARG(5, NUMERIC_type_of_ClipVarType);
   CHECKARG(6, NUMERIC_type_of_ClipVarType);
   CHECKARG(7, NUMERIC_type_of_ClipVarType);
   CHECKARG(8, CHARACTER_type_of_ClipVarType);
   CHECKARG(9, NUMERIC_type_of_ClipVarType);

   rgb = rgb + rowstride * y + x * 3;

   gdk_draw_rgb_image((GdkDrawable *) cwid->widget->window, GDK_GC(cgc->object), x, y, width, height, dith, rgb, rowstride);

   return 0;
 err:
   return 1;
}				/* clip_GDK_DRAWRGBIMAGE() */

int
clip_GDK_DRAWRGBIMAGEDITHALIGN(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   C_object *cgc = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   gint x = _clip_parni(ClipMachineMemory, 3);
   gint y = _clip_parni(ClipMachineMemory, 4);
   gint width = _clip_parni(ClipMachineMemory, 5);
   gint height = _clip_parni(ClipMachineMemory, 6);
   GdkRgbDither dith = INT_OPTION(ClipMachineMemory, 7, 0);
   guchar *rgb = (guchar *) _clip_parc(ClipMachineMemory, 8);
   gint rowstride = _clip_parni(ClipMachineMemory, 9);
   gint xdith = _clip_parni(ClipMachineMemory, 10);
   gint ydith = _clip_parni(ClipMachineMemory, 11);

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKARG(2, MAP_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);
   CHECKARG(5, NUMERIC_type_of_ClipVarType);
   CHECKARG(6, NUMERIC_type_of_ClipVarType);
   CHECKARG(7, NUMERIC_type_of_ClipVarType);
   CHECKARG(8, CHARACTER_type_of_ClipVarType);
   CHECKARG(9, NUMERIC_type_of_ClipVarType);
   CHECKARG(10, NUMERIC_type_of_ClipVarType);
   CHECKARG(11, NUMERIC_type_of_ClipVarType);

   rgb = rgb + rowstride * y + x * 3;

   gdk_draw_rgb_image_dithalign((GdkDrawable *) cwid->widget->window, GDK_GC(cgc->object), x, y, width, height, dith, rgb, rowstride, xdith, ydith);

   return 0;
 err:
   return 1;
}				/* clip_GDK_DRAWRGBIMAGEDITHALIGN() */

int
clip_GDK_PIXBUFRENDERTODRAWABLE(ClipMachine * ClipMachineMemory)
{
   C_object *cpxb = _fetch_co_arg(ClipMachineMemory);
   C_widget *cdrawable = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   C_object *cgc = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));
   int src_x = _clip_parni(ClipMachineMemory, 4);
   int src_y = _clip_parni(ClipMachineMemory, 5);
   int dest_x = _clip_parni(ClipMachineMemory, 6);
   int dest_y = _clip_parni(ClipMachineMemory, 7);
   int width = _clip_parni(ClipMachineMemory, 8);
   int height = _clip_parni(ClipMachineMemory, 9);
   GdkRgbDither dither = _clip_parni(ClipMachineMemory, 10);
   int x_dither = _clip_parnd(ClipMachineMemory, 11);
   int y_dither = _clip_parnd(ClipMachineMemory, 12);

   CHECKCOBJ(cpxb, GDK_IS_PIXBUF(cpxb->object));
   CHECKCWID(cdrawable, GTK_IS_DRAWING_AREA);
   CHECKCOBJ(cgc, GDK_IS_GC(cgc->object));
   CHECKARG(4, NUMERIC_type_of_ClipVarType);
   CHECKARG(5, NUMERIC_type_of_ClipVarType);
   CHECKARG(6, NUMERIC_type_of_ClipVarType);
   CHECKARG(7, NUMERIC_type_of_ClipVarType);
   CHECKARG(8, NUMERIC_type_of_ClipVarType);
   CHECKARG(9, NUMERIC_type_of_ClipVarType);
   CHECKARG(10, NUMERIC_type_of_ClipVarType);
   CHECKARG(11, NUMERIC_type_of_ClipVarType);
   CHECKARG(12, NUMERIC_type_of_ClipVarType);

   gdk_pixbuf_render_to_drawable(GDK_PIXBUF(cpxb->object), GDK_DRAWABLE(cdrawable->widget->window), GDK_GC(cgc->object), src_x, src_y, dest_x, dest_y, width, height, dither, x_dither, y_dither);

   return 0;
 err:
   return 1;
}
