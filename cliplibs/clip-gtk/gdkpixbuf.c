/*
    Copyright (C) 2003  ITK
    Author  : Kornilova E.V. <alena@itk.ru>
	      Sergio Zayas <icaro.maneton@lycos.es>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/

/* move defines to clip-gdk.h */
#define GDK_OBJECT_PIXBUF            HASH___GDK_OBJECT_PIXBUF
#define GDK_PIXBUF(p)             ((GdkPixbuf*)(p))
#define GDK_IS_PIXBUF(obj)        (obj && ((C_object*)obj)->type == GDK_OBJECT_PIXBUF)
/* end move */

#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg.h"

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <string.h>

#include "ci_clip-gtk.ch"
#include "ci_clip-gtk.h"
#include "ci_clip-gdk.h"

/*********************** SIGNALS **************************/

/****              Pixbuf has no signal                  ****/

/**********************************************************/

GtkType
_gdk_type_pixbuf()
{
   return GDK_OBJECT_PIXBUF;
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
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   gboolean  has_alpha = _clip_parl(ClipMachineMemory, 2);

   int       bits_per_sample = _clip_parni(ClipMachineMemory, 3);

   int       width = _clip_parni(ClipMachineMemory, 4);

   int       height = _clip_parni(ClipMachineMemory, 5);

   GdkPixbuf *pixbuf;

   C_object *cpixbuf;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKARG(2, LOGICAL_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);
   CHECKARG(5, NUMERIC_type_of_ClipVarType);

   pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, has_alpha, bits_per_sample, width, height);
   cpixbuf = _register_object(ClipMachineMemory, pixbuf, GDK_OBJECT_PIXBUF, cv, NULL);
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

   CHECKCOBJ(cpixbufsrc, GDK_IS_PIXBUF(cpixbuf));

   pixbuf = gdk_pixbuf_copy(GDK_PIXBUF(cpixbufsrc->object));
   cpixbuf = _register_object(ClipMachineMemory, pixbuf, GDK_OBJECT_PIXBUF, NULL, NULL);
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
   char     *file = _clip_parc(ClipMachineMemory, 1);

   GdkPixbuf *pixbuf;

   C_object *cpixbuf = NULL;

   if (!file)
      return 1;

   pixbuf = gdk_pixbuf_new_from_file(file);

   if (!pixbuf)
      return 1;

   cpixbuf = _register_object(ClipMachineMemory, pixbuf, GDK_OBJECT_PIXBUF, NULL, NULL);
   if (cpixbuf)
      _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cpixbuf->obj);

   return 0;
}				/* clip_GDK_PIXBUFNEWFROMFILE() */

int
clip_GDK_PIXBUFGETWIDTH(ClipMachine * ClipMachineMemory)
/* Return width of image in pixels */
{
   C_object *cpixbufsrc = _fetch_co_arg(ClipMachineMemory);

   CHECKCOBJ(cpixbufsrc, GDK_IS_PIXBUF(cpixbufsrc));

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

   CHECKCOBJ(cpixbufsrc, GDK_IS_PIXBUF(cpixbufsrc));

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

   CHECKCOBJ(cpixbufsrc, GDK_IS_PIXBUF(cpixbufsrc));

   _clip_retni(ClipMachineMemory, gdk_pixbuf_get_rowstride(GDK_PIXBUF(cpixbufsrc->object)));
   return (0);
 err:
   return (1);
}				/* clip_GDK_PIXBUFGETROWSTRIDE() */

int
clip_GDK_PIXBUFGETPIXELS(ClipMachine * ClipMachineMemory)
/* Return string with the image's pixels. */
{
   gint      rowstride, height;

   C_object *cpxbsrc = _fetch_co_arg(ClipMachineMemory);

   CHECKCOBJ(cpxbsrc, GDK_IS_PIXBUF(cpxbsrc));

   rowstride = gdk_pixbuf_get_rowstride(GDK_PIXBUF(cpxbsrc->object));
   height = gdk_pixbuf_get_height(GDK_PIXBUF(cpxbsrc->object));

   _clip_retcn(ClipMachineMemory, (char *) (gdk_pixbuf_get_pixels(GDK_PIXBUF(cpxbsrc->object))), (int) (rowstride * height));
   return (0);
 err:
   return (1);
}				/* clip_GDK_PIXBUFGETPIXELS() */

int
clip_GDK_PIXBUFCOMPOSITE(ClipMachine * ClipMachineMemory)
{
   C_object *cpxbsrc = _fetch_co_arg(ClipMachineMemory);

   C_object *cpxbdst = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   int       dest_x = _clip_parni(ClipMachineMemory, 3);

   int       dest_y = _clip_parni(ClipMachineMemory, 4);

   int       dest_width = _clip_parni(ClipMachineMemory, 5);

   int       dest_height = _clip_parni(ClipMachineMemory, 6);

   double    offset_x = _clip_parnd(ClipMachineMemory, 7);

   double    offset_y = _clip_parnd(ClipMachineMemory, 8);

   double    scale_x = _clip_parnd(ClipMachineMemory, 9);

   double    scale_y = _clip_parnd(ClipMachineMemory, 10);

   GdkInterpType interp_type = INT_OPTION(ClipMachineMemory, 11, 0);

   int       overall_alpha = _clip_parni(ClipMachineMemory, 12);

   CHECKCOBJ(cpxbsrc, GDK_IS_PIXBUF(cpxbsrc));
   CHECKCOBJ(cpxbdst, GDK_IS_PIXBUF(cpxbdst));
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

   gdk_pixbuf_composite(GDK_PIXBUF(cpxbsrc->object),
			GDK_PIXBUF(cpxbdst->object),
			dest_x,
			dest_y, dest_width, dest_height, offset_x, offset_y, scale_x, scale_y, interp_type, overall_alpha);

   return (0);
 err:
   return (1);
}				/* clip_GDK_PIXBUFCOMPOSITE() */

int
clip_GDK_PIXBUFCOPYAREA(ClipMachine * ClipMachineMemory)
{
   C_object *cpxbsrc = _fetch_co_arg(ClipMachineMemory);

   int       src_x = _clip_parni(ClipMachineMemory, 2);

   int       src_y = _clip_parni(ClipMachineMemory, 3);

   int       width = _clip_parni(ClipMachineMemory, 4);

   int       height = _clip_parni(ClipMachineMemory, 5);

   C_object *cpxbdst = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 6));

   int       dest_x = _clip_parnd(ClipMachineMemory, 7);

   int       dest_y = _clip_parnd(ClipMachineMemory, 8);

   CHECKCOBJ(cpxbsrc, GDK_IS_PIXBUF(cpxbsrc));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);
   CHECKARG(5, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cpxbdst, GDK_IS_PIXBUF(cpxbdst));
   CHECKARG(7, NUMERIC_type_of_ClipVarType);
   CHECKARG(8, NUMERIC_type_of_ClipVarType);

   gdk_pixbuf_copy_area(GDK_PIXBUF(cpxbsrc->object), src_x, src_y, width, height, GDK_PIXBUF(cpxbdst->object), dest_x, dest_y);

   return (0);
 err:
   return (1);
}				/* clip_GDK_PIXBUFCOPYAREA() */

int
clip_GDK_PIXBUFSCALE(ClipMachine * ClipMachineMemory)
{
   C_object *cpxbsrc = _fetch_co_arg(ClipMachineMemory);

   C_object *cpxbdst = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   int       dest_x = _clip_parni(ClipMachineMemory, 3);

   int       dest_y = _clip_parni(ClipMachineMemory, 4);

   int       dest_width = _clip_parni(ClipMachineMemory, 5);

   int       dest_height = _clip_parni(ClipMachineMemory, 6);

   double    offset_x = _clip_parnd(ClipMachineMemory, 7);

   double    offset_y = _clip_parnd(ClipMachineMemory, 8);

   double    scale_x = _clip_parnd(ClipMachineMemory, 9);

   double    scale_y = _clip_parnd(ClipMachineMemory, 10);

   GdkInterpType interp_type = INT_OPTION(ClipMachineMemory, 11, 0);

   CHECKCOBJ(cpxbsrc, GDK_IS_PIXBUF(cpxbsrc));
   CHECKCOBJ(cpxbdst, GDK_IS_PIXBUF(cpxbdst));
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);
   CHECKARG(5, NUMERIC_type_of_ClipVarType);
   CHECKARG(6, NUMERIC_type_of_ClipVarType);
   CHECKARG(7, NUMERIC_type_of_ClipVarType);
   CHECKARG(8, NUMERIC_type_of_ClipVarType);
   CHECKARG(9, NUMERIC_type_of_ClipVarType);
   CHECKARG(10, NUMERIC_type_of_ClipVarType);
   CHECKOPT(11, NUMERIC_type_of_ClipVarType);

   gdk_pixbuf_scale(GDK_PIXBUF(cpxbsrc->object),
		    GDK_PIXBUF(cpxbdst->object),
		    dest_x, dest_y, dest_width, dest_height, offset_x, offset_y, scale_x, scale_y, interp_type);

   return (0);
 err:
   return (1);
}				/* clip_GDK_PIXBUFSCALE() */

int
clip_GDK_PIXBUFSCALESIMPLE(ClipMachine * ClipMachineMemory)
{
   C_object *cpxb = _fetch_co_arg(ClipMachineMemory);

   int       dest_width = _clip_parni(ClipMachineMemory, 2);

   int       dest_height = _clip_parni(ClipMachineMemory, 3);

   GdkInterpType interp_type = INT_OPTION(ClipMachineMemory, 4, 0);

   GdkPixbuf *pxb_dst;

   C_object *cpxb_dst;

   CHECKCOBJ(cpxb, GDK_IS_PIXBUF(cpxb));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, NUMERIC_type_of_ClipVarType);

   pxb_dst = gdk_pixbuf_scale_simple(GDK_PIXBUF(cpxb->object), dest_width, dest_height, interp_type);
   if (pxb_dst)
    {
       cpxb_dst = _register_object(ClipMachineMemory, pxb_dst, GDK_OBJECT_PIXBUF, NULL, NULL);
       if (cpxb_dst)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &(cpxb_dst->obj));
    }

   return (0);
 err:
   return (1);
}				/* clip_GDK_PIXBUFSCALESIMPLE() */

/*********
int clip_GDK_PIXBUFRENDERPIXMAPANDMASK( ClipMachine * ClipMachineMemory )
{
	C_object * cpxb 	= _fetch_co_arg( ClipMachineMemory );
	ClipVar	 * cvpmap	= _clip_par( ClipMachineMemory, 2 );
	ClipVar	 * cvmask	= _clip_par( ClipMachineMemory, 3 );
	int alpha_threshold	= _clip_parni( ClipMachineMemory, 4 );
	GdkPixmap * pmap;
	GdkBitmap * mask;
	C_object * cpmap;
	C_object * cmask;

	CHECKCOBJ(cpxb,GDK_IS_PIXBUF(cpxb));
	CHECKARG2(2,MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType); CHECKARG2(3,MAP_type_of_ClipVarType,NUMERIC_type_of_ClipVarType);
	CHECKARG(4,NUMERIC_type_of_ClipVarType);

	gdk_pixbuf_render_pixmap_and_mask( GDK_PIXBUF(cpxb->object),
		&pmap, &mask, alpha_threshold);
	if ( pmap ) {
		cpmap = _register_object(ClipMachineMemory, pmap, GDK_OBJECT_PIXMAP, cvpmap, NULL);
		if (cpmap)
			_clip_mclone(ClipMachineMemory, cvpmap, &(cpmap->obj));
	}
	if ( mask ) {
		cmask = _register_object(ClipMachineMemory, mask, GDK_OBJECT_BITMAP, cvmask, NULL);
		if (cmask)
			_clip_mclone(ClipMachineMemory, cvmask, &(cmask->obj));
	}

return( 0 );
err:
return( 1 );
}

*************/
