/*
    Copyright (C) 2004  JARA for ITK
    Author  : Sergio Zayas <icaro.maneton@lycos.es>
    	      Elena Kornilova <alena@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/

#include <string.h>
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg2.h"

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include "ci_clip-gtk2.ch"
#include "ci_clip-gtk2.h"

/*********************** SIGNALS **************************/

/****              Pixbuf has no signal                  ****/

/**********************************************************/

GtkType
_gdk_type_pixbuf()
{
   return GDK_TYPE_PIXBUF;
}

long
_clip_type_pixbuf()
{
   return GDK_OBJECT_PIXBUF;
}

const char *
_clip_type_name_pixbuf()
{
   return "GDK_OBJECT_PIXBUF";
}

int
clip_INIT___PIXBUF(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_pixbuf, _clip_type_name_pixbuf, _gdk_type_pixbuf, NULL, NULL);
   return 0;
}

/**********************************************************/

/* Create a blank pixbuf with an optimal rowstride and a new buffer */
int
clip_GDK_PIXBUFNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   gboolean has_alpha = _clip_parl(ClipMachineMemory, 2);
   int bits_per_sample = _clip_parni(ClipMachineMemory, 3);
   int width = _clip_parni(ClipMachineMemory, 4);
   int height = _clip_parni(ClipMachineMemory, 5);
   GdkPixbuf *pixbuf;
   C_object *cpixbuf;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKARG(2, LOGICAL_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);
   CHECKARG(5, NUMERIC_type_of_ClipVarType);

   pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, has_alpha, bits_per_sample, width, height);
   cpixbuf = _list_get_cobject(ClipMachineMemory, pixbuf);
   if (!cpixbuf)
      cpixbuf = _register_object(ClipMachineMemory, pixbuf, GDK_TYPE_PIXBUF, cv, NULL);
   if (cpixbuf)
      _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cpixbuf->obj);

   return 0;
 err:
   return 1;
}

/* Copy a pixbuf */

int
clip_GDK_PIXBUFCOPY(ClipMachine * ClipMachineMemory)
{
   C_object *cpixbufsrc = _fetch_co_arg(ClipMachineMemory);
   GdkPixbuf *pixbuf;
   C_object *cpixbuf = 0;

   CHECKCOBJ(cpixbufsrc, GDK_IS_PIXBUF(cpixbuf->object));

   pixbuf = gdk_pixbuf_copy(GDK_PIXBUF(cpixbufsrc->object));
   cpixbuf = _list_get_cobject(ClipMachineMemory, pixbuf);
   if (!cpixbuf)
      cpixbuf = _register_object(ClipMachineMemory, pixbuf, GDK_TYPE_PIXBUF, NULL, NULL);
   if (cpixbuf)
      _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cpixbuf->obj);
   return 0;
 err:
   return 1;
}

int
clip_GDK_PIXBUFNEWFROMFILE(ClipMachine * ClipMachineMemory)
/* Load image from file
  format permited are : png, jpeg, tiff, gif, xpm, pnm, ras, bmp, xbm and ico
  somes formats depend of how are compiled gdk-pixbuf
*/
{
   char *fname = _clip_parc(ClipMachineMemory, 1);
   GdkPixbuf *pixbuf;
   C_object *cpixbuf = NULL;
   GError *error = NULL;

   if (!fname)
      return 1;

   pixbuf = gdk_pixbuf_new_from_file(fname, &error);

   if (!pixbuf)
      return 1;

   cpixbuf = _list_get_cobject(ClipMachineMemory, pixbuf);
   if (!cpixbuf)
      cpixbuf = _register_object(ClipMachineMemory, pixbuf, GDK_TYPE_PIXBUF, NULL, NULL);
   if (cpixbuf)
      _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cpixbuf->obj);

   return 0;
}				/* clip_GDK_PIXBUFNEWFROMFILE() */

int
clip_GDK_PIXBUFGETWIDTH(ClipMachine * ClipMachineMemory)
/* Return width of image in pixels */
{
   C_object *cpixbufsrc = _fetch_co_arg(ClipMachineMemory);

   CHECKCOBJ(cpixbufsrc, GDK_IS_PIXBUF(cpixbufsrc->object));

   _clip_retni(ClipMachineMemory, gdk_pixbuf_get_width(GDK_PIXBUF(cpixbufsrc->object)));

   return (0);
 err:
   return (1);
}				/* clip_GDK_PIXBUFGETWIDTH() */

int
clip_GDK_PIXBUFGETHEIGHT(ClipMachine * ClipMachineMemory)
/* Return height of image in pixels. */
{
   C_object *cpixbufsrc = _fetch_co_arg(ClipMachineMemory);

   CHECKCOBJ(cpixbufsrc, GDK_IS_PIXBUF(cpixbufsrc->object));

   _clip_retni(ClipMachineMemory, gdk_pixbuf_get_height(GDK_PIXBUF(cpixbufsrc->object)));
   return (0);
 err:
   return (1);
}				/* clip_GDK_PIXBUFGETHEIGHT() */

int
clip_GDK_PIXBUFGETROWSTRIDE(ClipMachine * ClipMachineMemory)
/* Return image's rowstride. */
{
   C_object *cpixbufsrc = _fetch_co_arg(ClipMachineMemory);
   CHECKCOBJ(cpixbufsrc, GDK_IS_PIXBUF(cpixbufsrc->object));

   _clip_retni(ClipMachineMemory, gdk_pixbuf_get_rowstride(GDK_PIXBUF(cpixbufsrc->object)));
   return (0);
 err:
   return (1);
}				/* clip_GDK_PIXBUFGETROWSTRIDE() */

int
clip_GDK_PIXBUFGETPIXELS(ClipMachine * ClipMachineMemory)
/* Return string with the image's pixels. */
{
   gint rowstride, height;

   C_object *cpxbsrc = _fetch_co_arg(ClipMachineMemory);
   CHECKCOBJ(cpxbsrc, GDK_IS_PIXBUF(cpxbsrc->object));

   rowstride = gdk_pixbuf_get_rowstride(GDK_PIXBUF(cpxbsrc->object));
   height = gdk_pixbuf_get_height(GDK_PIXBUF(cpxbsrc->object));
   _clip_retcn(ClipMachineMemory, (char *) gdk_pixbuf_get_pixels(GDK_PIXBUF(cpxbsrc->object)), rowstride * height);
   return (0);
 err:
   return (1);
}				/* clip_GDK_PIXBUFGETPIXELS() */

int
clip_GDK_PIXBUFCOMPOSITE(ClipMachine * ClipMachineMemory)
{
   C_object *cpxbsrc = _fetch_co_arg(ClipMachineMemory);
   ClipVar *cvdst = _clip_spar(ClipMachineMemory, 2);
   C_object *cpxbdst = _fetch_cobject(ClipMachineMemory, cvdst);
   int dest_x = _clip_parni(ClipMachineMemory, 3);
   int dest_y = _clip_parni(ClipMachineMemory, 4);
   int dest_width = _clip_parni(ClipMachineMemory, 5);
   int dest_height = _clip_parni(ClipMachineMemory, 6);
   double offset_x = _clip_parnd(ClipMachineMemory, 7);
   double offset_y = _clip_parnd(ClipMachineMemory, 8);
   double scale_x = _clip_parnd(ClipMachineMemory, 9);
   double scale_y = _clip_parnd(ClipMachineMemory, 10);
   GdkInterpType interp_type = INT_OPTION(ClipMachineMemory, 11, 0);
   int overall_alpha = _clip_parni(ClipMachineMemory, 12);
   GdkPixbuf *pixbuf;

   CHECKCOBJ(cpxbsrc, GDK_IS_PIXBUF(cpxbsrc->object));
   CHECKCOBJ(cpxbdst, GDK_IS_PIXBUF(cpxbdst->object));
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);
   CHECKARG(5, NUMERIC_type_of_ClipVarType);
   CHECKARG(6, NUMERIC_type_of_ClipVarType);
   CHECKARG(7, NUMERIC_type_of_ClipVarType);
   CHECKARG(8, NUMERIC_type_of_ClipVarType);
   CHECKARG(9, NUMERIC_type_of_ClipVarType);
   CHECKARG(10, NUMERIC_type_of_ClipVarType);
   CHECKARG(11, NUMERIC_type_of_ClipVarType);
   CHECKARG(12, NUMERIC_type_of_ClipVarType);

   pixbuf = GDK_PIXBUF(cpxbdst->object);
   gdk_pixbuf_composite(GDK_PIXBUF(cpxbsrc->object), pixbuf, dest_x, dest_y, dest_width, dest_height, offset_x, offset_y, scale_x, scale_y, interp_type, overall_alpha);

   if (pixbuf)
      {
	 C_object *cpxb;
	 cpxb = _list_get_cobject(ClipMachineMemory, pixbuf);
	 if (!cpxb)
	    cpxb = _register_object(ClipMachineMemory, pixbuf, GDK_TYPE_PIXBUF, cvdst, NULL);
	 if (cpxb)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cpxb->obj);
      }

   return (0);
 err:
   return (1);
}				/* clip_GDK_PIXBUFCOMPOSITE() */

int
clip_GDK_PIXBUFCOPYAREA(ClipMachine * ClipMachineMemory)
{
   C_object *cpxbsrc = _fetch_co_arg(ClipMachineMemory);
   int src_x = _clip_parni(ClipMachineMemory, 2);
   int src_y = _clip_parni(ClipMachineMemory, 3);
   int width = _clip_parni(ClipMachineMemory, 4);
   int height = _clip_parni(ClipMachineMemory, 5);
   ClipVar *cdst = _clip_spar(ClipMachineMemory, 6);
   C_object *cpxbdst = _fetch_cobject(ClipMachineMemory, cdst);
   int dest_x = _clip_parnd(ClipMachineMemory, 7);
   int dest_y = _clip_parnd(ClipMachineMemory, 8);
   GdkPixbuf *pixbuf;

   CHECKCOBJ(cpxbsrc, GDK_IS_PIXBUF(cpxbsrc->object));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);
   CHECKARG(5, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cpxbdst, GDK_IS_PIXBUF(cpxbdst->object));
   CHECKARG(7, NUMERIC_type_of_ClipVarType);
   CHECKARG(8, NUMERIC_type_of_ClipVarType);

   pixbuf = GDK_PIXBUF(cpxbdst->object);
   gdk_pixbuf_copy_area(GDK_PIXBUF(cpxbsrc->object), src_x, src_y, width, height, pixbuf, dest_x, dest_y);
   if (pixbuf)
      {
	 C_object *cpxb;
	 cpxb = _list_get_cobject(ClipMachineMemory, pixbuf);
	 if (!cpxb)
	    cpxb = _register_object(ClipMachineMemory, pixbuf, GDK_TYPE_PIXBUF, cdst, NULL);
	 if (cpxb)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cpxb->obj);
      }

   return (0);
 err:
   return (1);
}				/* clip_GDK_PIXBUFCOPYAREA() */

int
clip_GDK_PIXBUFSCALE(ClipMachine * ClipMachineMemory)
{
   C_object *cpxbsrc = _fetch_co_arg(ClipMachineMemory);
   ClipVar *cpxb = _clip_spar(ClipMachineMemory, 2);
   C_object *cpxbdst = _fetch_cobject(ClipMachineMemory, cpxb);
   int dest_x = _clip_parni(ClipMachineMemory, 3);
   int dest_y = _clip_parni(ClipMachineMemory, 4);
   int dest_width = _clip_parni(ClipMachineMemory, 5);
   int dest_height = _clip_parni(ClipMachineMemory, 6);
   double offset_x = _clip_parnd(ClipMachineMemory, 7);
   double offset_y = _clip_parnd(ClipMachineMemory, 8);
   double scale_x = _clip_parnd(ClipMachineMemory, 9);
   double scale_y = _clip_parnd(ClipMachineMemory, 10);
   GdkInterpType interp_type = INT_OPTION(ClipMachineMemory, 11, 0);
   GdkPixbuf *pixbuf;

   CHECKCOBJ(cpxbsrc, GDK_IS_PIXBUF(cpxbsrc->object));
   CHECKCOBJ(cpxbdst, GDK_IS_PIXBUF(cpxbdst->object));
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);
   CHECKARG(5, NUMERIC_type_of_ClipVarType);
   CHECKARG(6, NUMERIC_type_of_ClipVarType);
   CHECKARG(7, NUMERIC_type_of_ClipVarType);
   CHECKARG(8, NUMERIC_type_of_ClipVarType);
   CHECKARG(9, NUMERIC_type_of_ClipVarType);
   CHECKARG(10, NUMERIC_type_of_ClipVarType);
   CHECKOPT(11, NUMERIC_type_of_ClipVarType);

   pixbuf = GDK_PIXBUF(cpxbdst->object);
   gdk_pixbuf_scale(GDK_PIXBUF(cpxbsrc->object), pixbuf, dest_x, dest_y, dest_width, dest_height, offset_x, offset_y, scale_x, scale_y, interp_type);
   if (pixbuf)
      {
	 C_object *cpix;
	 cpix = _list_get_cobject(ClipMachineMemory, pixbuf);
	 if (!cpix)
	    cpix = _register_object(ClipMachineMemory, pixbuf, GDK_TYPE_PIXBUF, cpxb, NULL);
	 _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cpix->obj);
      }
   return (0);
 err:
   return (1);
}				/* clip_GDK_PIXBUFSCALE() */

int
clip_GDK_PIXBUFSCALESIMPLE(ClipMachine * ClipMachineMemory)
{
   C_object *cpxb = _fetch_co_arg(ClipMachineMemory);
   int dest_width = _clip_parni(ClipMachineMemory, 2);
   int dest_height = _clip_parni(ClipMachineMemory, 3);
   GdkInterpType interp_type = INT_OPTION(ClipMachineMemory, 4, 0);
   GdkPixbuf *pxb_dst;
   C_object *cpxb_dst;

   CHECKCOBJ(cpxb, GDK_IS_PIXBUF(cpxb->object));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, NUMERIC_type_of_ClipVarType);

   pxb_dst = gdk_pixbuf_scale_simple(GDK_PIXBUF(cpxb->object), dest_width, dest_height, interp_type);
   if (pxb_dst)
      {

	 cpxb_dst = _list_get_cobject(ClipMachineMemory, pxb_dst);
	 if (!cpxb_dst)
	    cpxb_dst = _register_object(ClipMachineMemory, pxb_dst, GDK_TYPE_PIXBUF, NULL, NULL);
	 if (cpxb_dst)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &(cpxb_dst->obj));
      }

   return (0);
 err:
   return (1);
}				/* clip_GDK_PIXBUFSCALESIMPLE() */

/* gdk_PixbufSave(oPixbuf, sFilename, sType[, sValue, sKey ....]) --> TRUE || FALSE */
int
clip_GDK_PIXBUFSAVE(ClipMachine * ClipMachineMemory)
{
   C_object *cpxb = _fetch_co_arg(ClipMachineMemory);
   const gchar *filename = _clip_parc(ClipMachineMemory, 2);
   const gchar *type = _clip_parc(ClipMachineMemory, 3);
   GError *error;
   gchar *list[20];
   gint n, i, j;
   gboolean ret;

   CHECKCOBJ(cpxb, GDK_IS_PIXBUF(cpxb->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);
   CHECKARG(3, CHARACTER_type_of_ClipVarType);

   memset(list, 0, sizeof(list));
   n = _clip_parinfo(ClipMachineMemory, 0);
   for (i = 4, j = 0; i < n; i++, j++)
      list[j] = _clip_parc(ClipMachineMemory, i);

   ret = gdk_pixbuf_save(GDK_PIXBUF(cpxb->object),
			 filename, type, &error,
			 list[0], list[1], list[2], list[3],
			 list[4], list[5], list[6], list[7], list[8], list[9], list[10], list[11], list[12], list[13], list[14], list[15], list[16], list[17], list[18], list[19], NULL);
   _clip_retl(ClipMachineMemory, ret);
   return (0);
 err:
   return (1);
}				/* clip_GDK_PIXBUFSAVE() */

int
clip_GDK_PIXBUFCOMPOSITECOLORSIMPLE(ClipMachine * ClipMachineMemory)
{
   C_object *cpxb = _fetch_co_arg(ClipMachineMemory);
   int dest_width = _clip_parni(ClipMachineMemory, 2);
   int dest_height = _clip_parni(ClipMachineMemory, 3);
   GdkInterpType type = _clip_parni(ClipMachineMemory, 4);
   int overall_alpha = _clip_parni(ClipMachineMemory, 5);
   int check_size = _clip_parni(ClipMachineMemory, 6);
   guint32 color1 = _clip_parni(ClipMachineMemory, 7);
   guint32 color2 = _clip_parni(ClipMachineMemory, 8);
   GdkPixbuf *pixbuf;
   C_object *cpixbuf;

   CHECKCOBJ(cpxb, GDK_IS_PIXBUF(cpxb->object));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);
   CHECKARG(5, NUMERIC_type_of_ClipVarType);
   CHECKARG(6, NUMERIC_type_of_ClipVarType);
   CHECKARG(7, NUMERIC_type_of_ClipVarType);
   CHECKARG(8, NUMERIC_type_of_ClipVarType);

   pixbuf = gdk_pixbuf_composite_color_simple(GDK_PIXBUF(cpxb->object), dest_width, dest_height, type, overall_alpha, check_size, color1, color2);
   if (pixbuf)
      {
	 cpixbuf = _list_get_cobject(ClipMachineMemory, pixbuf);
	 if (!cpixbuf)
	    cpixbuf = _register_object(ClipMachineMemory, pixbuf, GDK_TYPE_PIXBUF, NULL, NULL);
	 if (cpixbuf)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cpixbuf->obj);
      }
   return (0);
 err:
   return (1);
}

int
clip_GDK_PIXBUFCOMPOSITECOLOR(ClipMachine * ClipMachineMemory)
{
   C_object *cpxb = _fetch_co_arg(ClipMachineMemory);
   ClipVar *cvdest = _clip_spar(ClipMachineMemory, 2);
   C_object *cpxbdst = _fetch_cobject(ClipMachineMemory, cvdest);
   int dest_x = _clip_parni(ClipMachineMemory, 3);
   int dest_y = _clip_parni(ClipMachineMemory, 4);
   int dest_width = _clip_parni(ClipMachineMemory, 5);
   int dest_height = _clip_parni(ClipMachineMemory, 6);
   double offset_x = _clip_parnd(ClipMachineMemory, 7);
   double offset_y = _clip_parnd(ClipMachineMemory, 8);
   double scale_x = _clip_parnd(ClipMachineMemory, 9);
   double scale_y = _clip_parnd(ClipMachineMemory, 10);
   GdkInterpType type = _clip_parni(ClipMachineMemory, 11);
   int overall_alpha = _clip_parni(ClipMachineMemory, 12);
   int check_x = _clip_parni(ClipMachineMemory, 13);
   int check_y = _clip_parni(ClipMachineMemory, 14);
   int check_size = _clip_parni(ClipMachineMemory, 15);
   guint32 color1 = _clip_parni(ClipMachineMemory, 16);
   guint32 color2 = _clip_parni(ClipMachineMemory, 17);
   GdkPixbuf *pixbuf;
   C_object *cpixbuf;

   CHECKCOBJ(cpxb, GDK_IS_PIXBUF(cpxb->object));
   CHECKCOBJ(cpxbdst, GDK_IS_PIXBUF(cpxbdst->object));
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);
   CHECKARG(5, NUMERIC_type_of_ClipVarType);
   CHECKARG(6, NUMERIC_type_of_ClipVarType);
   CHECKARG(7, NUMERIC_type_of_ClipVarType);
   CHECKARG(8, NUMERIC_type_of_ClipVarType);
   CHECKARG(9, NUMERIC_type_of_ClipVarType);
   CHECKARG(10, NUMERIC_type_of_ClipVarType);
   CHECKARG(11, NUMERIC_type_of_ClipVarType);
   CHECKARG(12, NUMERIC_type_of_ClipVarType);
   CHECKARG(13, NUMERIC_type_of_ClipVarType);
   CHECKARG(14, NUMERIC_type_of_ClipVarType);
   CHECKARG(15, NUMERIC_type_of_ClipVarType);
   CHECKARG(16, NUMERIC_type_of_ClipVarType);
   CHECKARG(17, NUMERIC_type_of_ClipVarType);

   pixbuf = GDK_PIXBUF(cpxbdst->object);
   gdk_pixbuf_composite_color(GDK_PIXBUF(cpxb->object), pixbuf,
			      dest_x, dest_y, dest_width, dest_height, offset_x, offset_y, scale_x, scale_y, type, overall_alpha, check_x, check_y, check_size, color1, color2);
   if (pixbuf)
      {
	 cpixbuf = _list_get_cobject(ClipMachineMemory, pixbuf);
	 if (!cpixbuf)
	    cpixbuf = _register_object(ClipMachineMemory, pixbuf, GDK_TYPE_PIXBUF, cvdest, NULL);
	 if (cpixbuf)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cpixbuf->obj);
      }
   return (0);
 err:
   return (1);
}

int
clip_GDK_PIXBUFUNREF(ClipMachine * ClipMachineMemory)
{
   C_object *cpxb = _fetch_co_arg(ClipMachineMemory);

   CHECKCOBJ(cpxb, GDK_IS_PIXBUF(cpxb->object));

   gdk_pixbuf_unref(GDK_PIXBUF(cpxb->object));
   return (0);
 err:
   return (1);
}
