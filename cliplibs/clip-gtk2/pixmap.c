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
#include "ci_2_bmpxpm.h"

#ifdef OS_CYGWIN
#include "sys\cygwin.h"
#endif

#define MAXCOLORS       256

/*********************** SIGNALS **************************/

/****           Pixmap has no own signal               ****/

/**********************************************************/

/* Register pixmap in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_pixmap()
{
   return GTK_TYPE_PIXMAP;
}

long
_clip_type_pixmap()
{
   return GTK_WIDGET_PIXMAP;
}

const char *
_clip_type_name_pixmap()
{
   return "GTK_WIDGET_PIXMAP";
}

int
clip_INIT___PIXMAP(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_pixmap, _clip_type_name_pixmap, _gtk_type_pixmap, _gtk_type_misc, NULL);
   return 0;
}

/**********************************************************/

/**** PIXMAP constructor ****/
int
clip_GTK_PIXMAPNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   C_object *cpxm = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_object *cmask = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   C_widget *cwid;

   GtkWidget *wid;

   CHECKCOBJ(cpxm, GDK_IS_PIXMAP(cpxm->object));
  //CHECKCOBJ(cmask, GDK_IS_BITMAP(cmask->object));
   wid = gtk_pixmap_new(GDK_PIXMAP(cpxm->object), (GdkBitmap *) (cmask->object));

   if (wid)
    {
       cwid = _list_get_cwidget(ClipMachineMemory, wid);
       if (!cwid)
	  cwid = _register_widget(ClipMachineMemory, wid, cv);
       if (cwid)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
    }

   return 0;
 err:
   return 1;
}

int
clip_GTK_PIXMAPSET(ClipMachine * ClipMachineMemory)
{
   C_widget *cpxm = _fetch_cw_arg(ClipMachineMemory);

   C_object *cval = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_object *cmask = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   CHECKCWID(cpxm, GTK_IS_PIXMAP);
   CHECKCOBJ(cval, GDK_IS_PIXMAP(cval->object));
  //CHECKCOBJ(cmask, GDK_IS_BITMAP(cmask->object));
   gtk_pixmap_set(GTK_PIXMAP(cpxm->widget), GDK_PIXMAP(cval->object), (GdkBitmap *) (cmask->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_PIXMAPGET(ClipMachine * ClipMachineMemory)
{
   C_widget *cpxm = _fetch_cw_arg(ClipMachineMemory);

   ClipVar  *c1 = _clip_spar(ClipMachineMemory, 2);

   ClipVar  *c2 = _clip_spar(ClipMachineMemory, 3);

   C_object *cval;

   C_object *cmask;

   GdkPixmap *val;

   GdkBitmap *mask;

   CHECKCWID(cpxm, GDK_IS_PIXMAP);
   gtk_pixmap_get(GTK_PIXMAP(cpxm->widget), &val, &mask);

   if (val)
    {
       cval = _list_get_cobject(ClipMachineMemory, val);
       if (!cval)
	  cval = _register_object(ClipMachineMemory, val, GDK_TYPE_PIXMAP, NULL, NULL);
       if (cval)
	  _clip_mclone(ClipMachineMemory, c1, &cval->obj);
    }

   if (mask)
    {
       cmask = _list_get_cobject(ClipMachineMemory, mask);
       if (!cmask)
	  cmask = _register_object(ClipMachineMemory, mask, GDK_TYPE_BITMAP, NULL, NULL);
       if (cmask)
	  _clip_mclone(ClipMachineMemory, c2, &cmask->obj);
    }

   return 0;
 err:
   return 1;
}

int
clip_GTK_PIXMAPSETBUILDINSENSITIVE(ClipMachine * ClipMachineMemory)
{
   C_widget *cpxm = _fetch_cw_arg(ClipMachineMemory);

   gboolean  build = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(cpxm, GTK_IS_PIXMAP);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      build = TRUE;
   gtk_pixmap_set_build_insensitive(GTK_PIXMAP(cpxm->widget), build);
   return 0;
 err:
   return 1;
}

int
clip_GDK_PIXMAPNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   C_widget *cwin = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gint      width = _clip_parni(ClipMachineMemory, 3);

   gint      height = _clip_parni(ClipMachineMemory, 4);

   gint      depth = INT_OPTION(ClipMachineMemory, 5, -1);

   GtkWidget *wid = NULL;

   C_widget *cwid;

//      GdkBitmap *mask;
   GdkPixmap *pixmap;

   GdkWindow *win = NULL;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cwin, GTK_IS_WIDGET);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, NUMERIC_type_of_ClipVarType);
   CHECKOPT(5, NUMERIC_type_of_ClipVarType);

   if (cwin && cwin->widget && cwin->widget->window)
    {
       win = cwin->widget->window;
    }
   pixmap = gdk_pixmap_new(win, width, height, depth);

//        if (pixmap) wid = gtk_pixmap_new(pixmap,mask);
   if (pixmap)
      wid = gtk_pixmap_new(pixmap, NULL);
   if (!wid)
      goto err;

   cwid = _list_get_cwidget(ClipMachineMemory, wid);
   if (!cwid)
      cwid = _register_widget(ClipMachineMemory, wid, cv);
   cwid->data = pixmap;
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

/**** PIXMAP constructor ****/
int
clip_GDK_PIXMAPFROMXPMNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   char     *filename = _clip_parc(ClipMachineMemory, 2);

   C_widget *cwin = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   ClipVar  *cmask = _clip_spar(ClipMachineMemory, 4);

   C_object *cbitmap;

   char      buf[PATH_MAX];

   GtkWidget *wid = NULL;

   C_widget *cwid;

   GdkBitmap *mask;

   GdkPixmap *pixmap;

   GdkWindow *win = NULL;

   GdkColormap *colormap = NULL;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT2(3, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cwin, GTK_IS_WIDGET);

   LOCALE_TO_UTF(filename);
#ifdef OS_CYGWIN
   cygwin_conv_to_win32_path(filename, buf);
#else
   _clip_path(ClipMachineMemory, filename, buf, sizeof(buf), 0);
#endif
   FREE_TEXT(filename);
   if (cwin && cwin->widget && cwin->widget->window)
    {
       win = cwin->widget->window;
       colormap = gtk_widget_get_colormap(cwin->widget);
    }
   else
      colormap = gdk_colormap_get_system();
//        pixmap = gdk_pixmap_create_from_xpm(win, &mask, NULL, buf);
   pixmap = gdk_pixmap_colormap_create_from_xpm(win, colormap, &mask, NULL, buf);
   if (pixmap)
      wid = gtk_pixmap_new(pixmap, mask);
   if (!wid)
      goto err;

   cwid = _list_get_cwidget(ClipMachineMemory, wid);
   if (!cwid)
      cwid = _register_widget(ClipMachineMemory, wid, cv);
   cwid->data = pixmap;
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   if (mask && cmask != NULL)
    {
       cbitmap = _list_get_cobject(ClipMachineMemory, mask);
       if (!cbitmap)
	  cbitmap = _register_object(ClipMachineMemory, mask, GDK_TYPE_BITMAP, cmask, NULL);
       if (cbitmap)
	  _clip_mclone(ClipMachineMemory, cmask, &cbitmap->obj);
    }

   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

/**** PIXMAP from data constructor ****/
int
clip_GDK_PIXMAPCREATEFROMXPMD(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   ClipVar  *cxpmd = _clip_par(ClipMachineMemory, 2);

   C_widget *cwin = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   ClipVar  *cmask = _clip_spar(ClipMachineMemory, 4);

   GtkWidget *wid = NULL;

   C_widget *cwid;

   C_object *cbitmap;

   GdkBitmap *mask;

   GdkPixmap *pixmap;

   GdkWindow *win = NULL;

   GdkColormap *colormap = NULL;

   char    **xpm_data = NULL;

   unsigned short i;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT(2, ARRAY_type_of_ClipVarType);
   CHECKOPT2(3, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cwin, GTK_IS_WIDGET);

   if (cwin && cwin->widget && cwin->widget->window)
    {
       win = cwin->widget->window;
       colormap = gtk_widget_get_colormap(cwin->widget);
    }
   else
      colormap = gdk_colormap_get_system();
  //pixmap = gdk_pixmap_create_from_xpm(win, &mask, NULL, buf);

   if (cxpmd->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar > 0)
    {
       ClipVar  *item;

       xpm_data = (char **) calloc(sizeof(char *), cxpmd->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar);
       for (i = 0; i < cxpmd->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar; i++)
	{
	   item = cxpmd->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i;
	   if (item->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
	    {
	       xpm_data[i] = (char *) calloc(1, item->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf + 1);
	       strcpy(xpm_data[i], item->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
	    }
	}
    }

   pixmap = gdk_pixmap_colormap_create_from_xpm_d(win, colormap, &mask, NULL, xpm_data);
   if (pixmap)
      wid = gtk_pixmap_new(pixmap, mask);
   if (xpm_data)
    {
       for (i = 0; i < cxpmd->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar; i++)
	{
	   if (xpm_data[i])
	      free(xpm_data[i]);
	}
       free(xpm_data);
    }

   if (!wid)
      goto err;

   cwid = _list_get_cwidget(ClipMachineMemory, wid);
   if (!cwid)
      cwid = _register_widget(ClipMachineMemory, wid, cv);
   cwid->data = pixmap;
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   if (mask && cmask != NULL)
    {
       cbitmap = _list_get_cobject(ClipMachineMemory, mask);
       if (!cbitmap)
	  cbitmap = _register_object(ClipMachineMemory, mask, GDK_TYPE_BITMAP, cmask, NULL);
      //if (cbitmap) _clip_mclone(ClipMachineMemory, cmask, &cbitmap->obj);
      //cmask = NEW(ClipVar);
      //_clip_map(ClipMachineMemory, cmask);
       if (cbitmap)
	  _clip_mclone(ClipMachineMemory, cmask, &cbitmap->obj);
    }

   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

/* forwards */
static gchar **_load_xpm_from_bmp(ClipMachine * ClipMachineMemory, char *filename,
				  int *ncolors, unsigned long *rows, unsigned long *cols);
static void _free_xpm_data(char **xpm_data, unsigned long ncolors, unsigned long rows);

/**** PIXMAP from BMP constructor ****/
int
clip_GDK_PIXMAPFROMBMPNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   char     *filename = _clip_parc(ClipMachineMemory, 2);

   C_widget *cwin = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   char      buf[PATH_MAX];

   GtkWidget *wid = NULL;

   C_widget *cwid;

   GdkBitmap *mask;

   GdkPixmap *pixmap = 0;

   GdkWindow *win = NULL;

   GdkColormap *colormap = NULL;

   gchar   **xpmdata;

   int       ncolors = 0;

   unsigned long rows = 0, cols = 0;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT2(3, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cwin, GTK_IS_WIDGET);

   LOCALE_TO_UTF(filename);
   _clip_path(ClipMachineMemory, filename, buf, sizeof(buf), 0);
   FREE_TEXT(filename);
   if (cwin && cwin->widget && cwin->widget->window)
    {
       win = cwin->widget->window;
       colormap = gtk_widget_get_colormap(cwin->widget);
    }
   else
      colormap = gdk_colormap_get_system();
   xpmdata = _load_xpm_from_bmp(ClipMachineMemory, buf, &ncolors, &rows, &cols);
   if (xpmdata)
      pixmap = gdk_pixmap_colormap_create_from_xpm_d(win, colormap, &mask, NULL, xpmdata);
   _free_xpm_data(xpmdata, ncolors, rows);
   wid = gtk_pixmap_new(pixmap, mask);
   if (!wid)
      goto err;

   cwid = _list_get_cwidget(ClipMachineMemory, wid);
   if (!cwid)
      cwid = _register_widget(ClipMachineMemory, wid, cv);
   cwid->data = pixmap;
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

/* build : set to TRUE if an extra pixmap should be automatically
 * created to use when the pixmap is insensitive.  */

static gchar **
_load_xpm_from_bmp(ClipMachine * ClipMachineMemory, char *filename, int *colors, unsigned long *nrows, unsigned long *ncols)
{
   FILE     *ifp;

   int       rc;

   unsigned long pos = 0;

   unsigned long rows, cols, pixstrlen;

   unsigned long offBits;

   unsigned short cBitCount;

   int       class;

   pixval    R[MAXCOLORS];	/* reds */

   pixval    G[MAXCOLORS];	/* greens */

   pixval    B[MAXCOLORS];	/* blues */

   lpixel  **lpixels;

   char    **xpm_data, **pxpm, *ppixstr;

   char      buf[300];

   register lpixel *pP;

   int       ncolors, row, col, i;

   pixval    maxval;		/* pixval == unsigned char or
				 * unsigned short */
   colorhash_table cht;

   colorhist_vector chv;

   char      err[100];

  /* Used for rgb value -> character-pixel string mapping */
   cixel_map *cmap;		/* cixel_map cmap[MAXCOLORS]; */

   int       charspp;		/* chars per pixel */

   if (!(ifp = fopen(filename, "rb")))
    {
       sprintf(err, "can't read file %s\n", filename);
       _clip_trap_err(ClipMachineMemory, EG_READ, FALSE, FALSE, "CLIP_GTK_SYSTEM", EG_READ, err);
       return NULL;
    }

   BMPreadfileheader(ifp, &pos, &offBits);
   BMPreadinfoheader(ifp, &pos, &cols, &rows, &cBitCount, &class);
   *nrows = rows;
   *ncols = cols;

   if (offBits != BMPoffbits(class, cBitCount))
    {
       sprintf(err, "warning: offBits is %ld, expected %ld", pos, BMPoffbits(class, cBitCount));
       _clip_trap_err(ClipMachineMemory, EG_READ, FALSE, FALSE, "CLIP_GTK_SYSTEM", EG_READ, err);
       return NULL;
    }

   rc = BMPreadrgbtable(ifp, &pos, cBitCount, class, R, G, B);

   if (rc != BMPlenrgbtable(class, cBitCount))
    {
       sprintf(err, "warning: %d-byte RGB table, expected %ld bytes", rc, BMPlenrgbtable(class, cBitCount));
       _clip_trap_err(ClipMachineMemory, EG_READ, FALSE, FALSE, "CLIP_GTK_SYSTEM", EG_READ, err);
       return NULL;
    }

   lpixels = BMPreadbits(ifp, &pos, offBits, cols, rows, cBitCount, class, R, G, B);

   if (pos != BMPlenfile(class, cBitCount, cols, rows))
    {
       sprintf(err, "warning: read %ld bytes, expected to read %ld bytes", pos, BMPlenfile(class, cBitCount, cols, rows));
       _clip_trap_err(ClipMachineMemory, EG_READ, FALSE, FALSE, "CLIP_GTK_SYSTEM", EG_READ, err);
       return NULL;
    }

   fclose(ifp);

	/**********************************************/
  /*
   * "maxval" is the largest value that can be be found in the ppm file.
   * All pixel components are relative to this value.
   */
   maxval = (1 << (cBitCount)) - 1;
	/**********************************************/
  /* Figure out the colormap. */

   chv = ppm_computecolorhist(lpixels, cols, rows, MAXCOLORS, &ncolors);
   *colors = ncolors;
   if (chv == (colorhist_vector) 0)
    {
       sprintf(err, "too many colors - try running the pixmap through 'ppmquant 256'");
       _clip_trap_err(ClipMachineMemory, EG_READ, FALSE, FALSE, "CLIP_GTK_SYSTEM", EG_READ, err);
       return NULL;
    }

  /* Make a hash table for fast color lookup. */
   cht = ppm_colorhisttocolorhash(chv, ncolors);

   cmap = (cixel_map *) malloc(ncolors * sizeof(cixel_map));
   if (cmap == (cixel_map *) NULL)
    {
       _clip_trap_err(ClipMachineMemory, EG_MEM, FALSE, FALSE, "CLIP_GTK_SYSTEM", EG_MEM, "out of memory");
       return NULL;
    }
  /* Now generate the character-pixel colormap table. */
   gen_cmap(chv, ncolors, maxval, 0, NULL, 0, cmap, &charspp);

  /* Write out the XPM table. */
   xpm_data = (char **) calloc(ncolors + rows + 1, sizeof(char *));
   pxpm = xpm_data;

  /* XPM */
  /* static char *xpm_data[] = { */
  /* width height ncolors chars_per_pixel */
   sprintf(buf, "%ld %ld %d %d", cols, rows, ncolors, charspp);
   *pxpm = strdup(buf);
   pxpm++;
  /* colors */
   for (i = 0; i < ncolors; i++)
    {
       sprintf(buf, "%s c %s", cmap[i].cixel, cmap[i].rgbname);
       *pxpm = strdup(buf);
       pxpm++;
    }
  /* pixels */

   pixstrlen = charspp * cols + 1;
   for (row = 0; row < rows; row++)
    {
       *pxpm = (char *) calloc(1, pixstrlen);
       ppixstr = *pxpm;
       for (col = 0, pP = lpixels[row]; col < cols; col++, pP++)
	{
	   memcpy(ppixstr, cmap[ppm_lookupcolor(cht, pP)].cixel, charspp);
	   ppixstr += charspp;
	}
       pxpm++;
    }
   ppm_freearray(lpixels, rows);
   ppm_freecolorhash(cht);
   free(cmap);

   return xpm_data;
}

static void
_free_xpm_data(char **xpm_data, unsigned long ncolors, unsigned long rows)
{
   unsigned long i;

   if (!xpm_data)
      return;
   for (i = 0; i < ncolors + rows + 1; i++)
      if (xpm_data[i])
	 free(xpm_data[i]);
   free(xpm_data);
}

/* gtk_PixmapColorMapFromXpmNew(obj, gdkWin/NIL, gdkColormap/NIL,
	@bitmask, colorArray/NIL, filename) --> pixmapObj*/
int
clip_GDK_PIXMAPCOLORMAPCREATEFROMXPM(ClipMachine * ClipMachineMemory)
{
   C_object *cwin = _fetch_co_arg(ClipMachineMemory);

   C_object *cclrmap = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   ClipVar  *cmask = _clip_spar(ClipMachineMemory, 3);

   ClipVar  *ctrclr = _clip_spar(ClipMachineMemory, 4);

   char     *filename = _clip_parc(ClipMachineMemory, 5);

   C_object *cbitmap;

   char      buf[PATH_MAX];

   C_object *cobj;

   GdkBitmap *mask;

   GdkPixmap *pixmap;

   GdkWindow *win = NULL;

   GdkColor *color = NULL;

   GdkColormap *colormap = NULL;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cwin, GDK_IS_WINDOW(cwin->object));
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cclrmap, GDK_IS_COLORMAP(cclrmap->object));
   CHECKOPT(4, ARRAY_type_of_ClipVarType);
   CHECKOPT(5, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(filename);
#ifdef OS_CYGWIN
   cygwin_conv_to_win32_path(filename, buf);
#else
   _clip_path(ClipMachineMemory, filename, buf, sizeof(buf), 0);
#endif
   FREE_TEXT(filename);

   if (cwin)
      win = GDK_WINDOW(cwin->object);
   if (cclrmap)
      colormap = GDK_COLORMAP(cclrmap->object);
   if (ctrclr)
      _map_colors_to_gdk(ClipMachineMemory, ctrclr, color);

   pixmap = gdk_pixmap_colormap_create_from_xpm(win, colormap, &mask, color, buf);

   cobj = _list_get_cobject(ClipMachineMemory, pixmap);
   if (!cobj)
      cobj = _register_object(ClipMachineMemory, pixmap, GDK_TYPE_PIXMAP, NULL, NULL);
  //cobj->data = pixmap;
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cobj->obj);
   if (mask && cmask != NULL)
    {
       cbitmap = _list_get_cobject(ClipMachineMemory, mask);
       if (!cbitmap)
	  cbitmap = _register_object(ClipMachineMemory, mask, GDK_TYPE_BITMAP, cmask, NULL);
       if (cbitmap)
	  _clip_mclone(ClipMachineMemory, cmask, &cbitmap->obj);
    }

   return 0;
 err:
   return 1;
}
