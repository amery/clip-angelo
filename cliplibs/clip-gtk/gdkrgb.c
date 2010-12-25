/*
    Copyright (C) 2004  JARA
    Author  : Sergio Zayas <icaro.maneton@lycos.es>
    License : (GPL) http://www.itk.ru/clipper/license.html
    mail : icaro.maneton@lycos.es
*/

#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg.h"

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <string.h>

#include "ci_clip-gtk.ch"
#include "ci_clip-gtk.h"
#include "ci_clip-gdk.h"

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

   gint      x = _clip_parni(ClipMachineMemory, 3);

   gint      y = _clip_parni(ClipMachineMemory, 4);

   gint      width = _clip_parni(ClipMachineMemory, 5);

   gint      height = _clip_parni(ClipMachineMemory, 6);

   GdkRgbDither dith = INT_OPTION(ClipMachineMemory, 7, 0);

   guchar   *rgb = (guchar *) _clip_parc(ClipMachineMemory, 8);

   gint      rowstride = _clip_parni(ClipMachineMemory, 9);

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

   gint      x = _clip_parni(ClipMachineMemory, 3);

   gint      y = _clip_parni(ClipMachineMemory, 4);

   gint      width = _clip_parni(ClipMachineMemory, 5);

   gint      height = _clip_parni(ClipMachineMemory, 6);

   GdkRgbDither dith = INT_OPTION(ClipMachineMemory, 7, 0);

   guchar   *rgb = (guchar *) _clip_parc(ClipMachineMemory, 8);

   gint      rowstride = _clip_parni(ClipMachineMemory, 9);

   gint      xdith = _clip_parni(ClipMachineMemory, 10);

   gint      ydith = _clip_parni(ClipMachineMemory, 11);

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

   gdk_draw_rgb_image_dithalign((GdkDrawable *) cwid->widget->window,
				GDK_GC(cgc->object), x, y, width, height, dith, rgb, rowstride, xdith, ydith);

   return 0;
 err:
   return 1;
}				/* clip_GDK_DRAWRGBIMAGEDITHALIGN() */
