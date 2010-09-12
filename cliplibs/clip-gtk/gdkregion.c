/*
    Copyright (C) 2001  ITK
    Author  : Alexey M. Tkachenko <alexey@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
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

GtkType
_gdk_type_region()
{
   return GDK_OBJECT_REGION;
}

long
_clip_type_region()
{
   return GDK_OBJECT_REGION;
}

const char *
_clip_type_name_region()
{
   return "GDK_OBJECT_REGION";
}

int
clip_INIT___GDKREGION(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_region, _clip_type_name_region, _gdk_type_region, NULL, NULL);
   return 0;
}

/**********************************************************/

/*
static int
gdk_object_region_destructor(ClipMachine *ClipMachineMemory, C_object *creg)
{
	if (creg && GDK_IS_REGION(creg))
        	gdk_region_destroy(GDK_REGION(creg->object));
        return 0;
}
*/

/* Get GdkRectangle data from a map */
static int
_map_get_gdk_rectangle(ClipMachine * ClipMachineMemory, ClipVar * map, GdkRectangle * region)
{
   if (map && map->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType && region)
    {
       double    x, y, width, height;

       _clip_mgetn(ClipMachineMemory, map, HASH_X, &x);
       _clip_mgetn(ClipMachineMemory, map, HASH_Y, &y);
       _clip_mgetn(ClipMachineMemory, map, HASH_WIDTH, &width);
       _clip_mgetn(ClipMachineMemory, map, HASH_HEIGHT, &height);
       region->x = x;
       region->y = y;
       region->width = width;
       region->height = height;
       return 0;
    }
   return -1;
}

/* Set GdkRectangle data to a map */
static int
_map_put_gdk_rectangle(ClipMachine * ClipMachineMemory, ClipVar * map, GdkRectangle * region)
{
   if (map && map->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType && region)
    {
       _clip_mputn(ClipMachineMemory, map, HASH_X, region->x);
       _clip_mputn(ClipMachineMemory, map, HASH_Y, region->y);
       _clip_mputn(ClipMachineMemory, map, HASH_WIDTH, region->width);
       _clip_mputn(ClipMachineMemory, map, HASH_HEIGHT, region->height);
       return 0;
    }
   return -1;
}

/* Calculates the intersection of two rectangles. */
int
clip_GDK_RECTANGLEINTERSECT(ClipMachine * ClipMachineMemory)
{
   ClipVar  *creg1 = _clip_spar(ClipMachineMemory, 1);

   ClipVar  *creg2 = _clip_spar(ClipMachineMemory, 2);

  //ClipVar *cdest = _clip_spar(ClipMachineMemory,3);
   GdkRectangle reg1, reg2, dest;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKARG(2, MAP_type_of_ClipVarType);
   CHECKOPT(3, MAP_type_of_ClipVarType);

   _map_get_gdk_rectangle(ClipMachineMemory, creg1, &reg1);
   _map_get_gdk_rectangle(ClipMachineMemory, creg2, &reg2);

   _clip_retl(ClipMachineMemory, gdk_rectangle_intersect(&reg1, &reg2, &dest));

   if (_clip_parinfo(ClipMachineMemory, 3) == MAP_type_of_ClipVarType)
      _map_put_gdk_rectangle(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3), &dest);

   return 0;
 err:
   return 1;
}

/* Calculates the union of two rectangles. The union of rectangles
 * src1 and src2 is the smallest rectangle which includes both
 * src1 and src2 within it. */
int
clip_GDK_RECTANGLEUNION(ClipMachine * ClipMachineMemory)
{
   ClipVar  *creg1 = _clip_spar(ClipMachineMemory, 1);

   ClipVar  *creg2 = _clip_spar(ClipMachineMemory, 2);

   ClipVar  *cdest = RETPTR(ClipMachineMemory);

   GdkRectangle reg1, reg2, dest;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKARG(2, MAP_type_of_ClipVarType);
   CHECKARG(3, MAP_type_of_ClipVarType);

   _map_get_gdk_rectangle(ClipMachineMemory, creg1, &reg1);
   _map_get_gdk_rectangle(ClipMachineMemory, creg2, &reg2);

   gdk_rectangle_union(&reg1, &reg2, &dest);

   memset(cdest, 0, sizeof(*cdest));
   _clip_map(ClipMachineMemory, cdest);
   _map_put_gdk_rectangle(ClipMachineMemory, cdest, &dest);

   return 0;
 err:
   return 1;
}

/* Creates a new empty GdkRegion. */
int
clip_GDK_REGIONNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   GdkRegion *region;

   C_object *cregion;

   CHECKOPT(1, MAP_type_of_ClipVarType);

   region = gdk_region_new();

   if (region)
    {
       cregion = _register_object(ClipMachineMemory, region, GDK_OBJECT_REGION, cv, NULL);
       if (cregion)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cregion->obj);
       else
	  gdk_region_destroy(region);
    }
   return 0;
 err:
   return 1;
}

/* Creates a new GdkRegion using the polygon defined by a number of points. */
/*
parameters - ([mObj], <aPoints>, [nPoints], [nFillRule])
	[mObj] - object, will returned;
	<aPoints> - array of points. Every point is an array {x,y};
        [nPoints] - number of points. If no, gets the length of aPoints;
        [nFillRule] - specifies which pixels are included in the region
        		when the polygon overlaps itself.
*/
int
clip_GDK_REGIONPOLYGON(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   ClipVar  *cap = _clip_spar(ClipMachineMemory, 2);

   int       npoints = _clip_parni(ClipMachineMemory, 3);

   int       fillrule = _clip_parni(ClipMachineMemory, 4);

   GdkRegion *region;

   C_object *cregion;

   unsigned short i;

   ClipVar  *cpoint;

   GdkPoint *points;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKARG(2, ARRAY_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, NUMERIC_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      npoints = cap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
   npoints = MIN(npoints, cap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar);

   points = (GdkPoint *) calloc(npoints, sizeof(GdkPoint));
   for (i = 0; i < npoints; i++)
    {
       cpoint = &cap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar[i];
       if (cpoint->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
	{
	   if (cpoint->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar > 0
	       && cpoint->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar[0].ClipType_t_of_ClipVar.
	       ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
	      points[i].x =
	       cpoint->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar[0].ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
	   if (cpoint->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar > 1
	       && cpoint->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar[1].ClipType_t_of_ClipVar.
	       ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
	      points[i].y =
	       cpoint->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar[1].ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
	}
    }

   region = gdk_region_polygon(points, npoints, fillrule);

   free(points);

   if (region)
    {
       cregion = _register_object(ClipMachineMemory, region, GDK_OBJECT_REGION, cv, NULL);
       if (cregion)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cregion->obj);
       else
	  gdk_region_destroy(region);
    }
   return 0;
 err:
   return 1;
}

/* Destroys a GdkRegion. */
int
clip_GDK_REGIONDESTROY(ClipMachine * ClipMachineMemory)
{
   C_object *creg = _fetch_co_arg(ClipMachineMemory);

   CHECKCOBJ(creg, GDK_IS_REGION(creg->object));

   gdk_region_destroy(GDK_REGION(creg));

   return 0;
 err:
   return 1;
}

/* Returns the intersection of two regions. */
int
clip_GDK_REGIONSINTERSECT(ClipMachine * ClipMachineMemory)
{
   C_object *creg = _fetch_co_arg(ClipMachineMemory);

   C_object *creg2 = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_object *cdest;

   GdkRegion *dest;

   CHECKCOBJ(creg, GDK_IS_REGION(creg->object));
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(creg2, GDK_IS_REGION(creg->object));

   dest = gdk_regions_intersect(GDK_REGION(creg), GDK_REGION(creg2));

   if (dest)
    {
       cdest = _register_object(ClipMachineMemory, dest, GDK_OBJECT_REGION, NULL, NULL);
       if (cdest)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cdest->obj);
       else
	  gdk_region_destroy(dest);
    }

   return 0;
 err:
   return 1;
}

/* Returns the union of two regions. This is all pixels
 * in either of source1 or source2. */
int
clip_GDK_REGIONUNION(ClipMachine * ClipMachineMemory)
{
   C_object *creg = _fetch_co_arg(ClipMachineMemory);

   C_object *creg2 = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_object *cdest;

   GdkRegion *dest;

   CHECKCOBJ(creg, GDK_IS_REGION(creg->object));
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(creg2, GDK_IS_REGION(creg->object));

   dest = gdk_regions_union(GDK_REGION(creg), GDK_REGION(creg2));

   if (dest)
    {
       cdest = _register_object(ClipMachineMemory, dest, GDK_OBJECT_REGION, NULL, NULL);
       if (cdest)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cdest->obj);
       else
	  gdk_region_destroy(dest);
    }

   return 0;
 err:
   return 1;
}

/* Subtracts one region from another. The result is a region containing
 * all the pixels which are in source1, but which are not in source2. */
int
clip_GDK_REGIONSUBTRACT(ClipMachine * ClipMachineMemory)
{
   C_object *creg = _fetch_co_arg(ClipMachineMemory);

   C_object *creg2 = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_object *cdest;

   GdkRegion *dest;

   CHECKCOBJ(creg, GDK_IS_REGION(creg->object));
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(creg2, GDK_IS_REGION(creg->object));

   dest = gdk_regions_subtract(GDK_REGION(creg), GDK_REGION(creg2));

   if (dest)
    {
       cdest = _register_object(ClipMachineMemory, dest, GDK_OBJECT_REGION, NULL, NULL);
       if (cdest)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cdest->obj);
       else
	  gdk_region_destroy(dest);
    }

   return 0;
 err:
   return 1;
}

/* Returns the difference between the union and the intersection of two
 * regions. This is a region containing the pixels that are in one of the
 * source regions, but which are not in both. */
int
clip_GDK_REGIONXOR(ClipMachine * ClipMachineMemory)
{
   C_object *creg = _fetch_co_arg(ClipMachineMemory);

   C_object *creg2 = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_object *cdest;

   GdkRegion *dest;

   CHECKCOBJ(creg, GDK_IS_REGION(creg->object));
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(creg2, GDK_IS_REGION(creg->object));

   dest = gdk_regions_xor(GDK_REGION(creg), GDK_REGION(creg2));

   if (dest)
    {
       cdest = _register_object(ClipMachineMemory, dest, GDK_OBJECT_REGION, NULL, NULL);
       if (cdest)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cdest->obj);
       else
	  gdk_region_destroy(dest);
    }

   return 0;
 err:
   return 1;
}

/* Returns the union of a region and a rectangle. */
int
clip_GDK_REGIONUNIONWITHRECT(ClipMachine * ClipMachineMemory)
{
   C_object *creg = _fetch_co_arg(ClipMachineMemory);

   ClipVar  *crect = _clip_spar(ClipMachineMemory, 2);

   C_object *cdest;

   GdkRegion *dest;

   GdkRectangle rect;

   CHECKCOBJ(creg, GDK_IS_REGION(creg->object));
   CHECKARG(2, MAP_type_of_ClipVarType);

   _map_get_gdk_rectangle(ClipMachineMemory, crect, &rect);
   dest = gdk_region_union_with_rect(GDK_REGION(creg), &rect);

   if (dest)
    {
       cdest = _register_object(ClipMachineMemory, dest, GDK_OBJECT_REGION, NULL, NULL);
       if (cdest)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cdest->obj);
       else
	  gdk_region_destroy(dest);
    }

   return 0;
 err:
   return 1;
}

/* Moves a region the specified distance. */
int
clip_GDK_REGIONOFFSET(ClipMachine * ClipMachineMemory)
{
   C_object *creg = _fetch_co_arg(ClipMachineMemory);

   gint      dx = _clip_parni(ClipMachineMemory, 2);

   gint      dy = _clip_parni(ClipMachineMemory, 3);

   CHECKCOBJ(creg, GDK_IS_REGION(creg->object));
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);

   gdk_region_offset(GDK_REGION(creg), dx, dy);

   return 0;
 err:
   return 1;
}

/* Resizes a region by the specified amount.
 * Positive values shrink the region. Negative values expand it. */
int
clip_GDK_REGIONSHRINK(ClipMachine * ClipMachineMemory)
{
   C_object *creg = _fetch_co_arg(ClipMachineMemory);

   gint      dx = _clip_parni(ClipMachineMemory, 2);

   gint      dy = _clip_parni(ClipMachineMemory, 3);

   CHECKCOBJ(creg, GDK_IS_REGION(creg->object));
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);

   gdk_region_shrink(GDK_REGION(creg), dx, dy);

   return 0;
 err:
   return 1;
}

/* Returns TRUE if the GdkRegion is empty. */
int
clip_GDK_REGIONEMPTY(ClipMachine * ClipMachineMemory)
{
   C_object *creg = _fetch_co_arg(ClipMachineMemory);

   CHECKCOBJ(creg, GDK_IS_REGION(creg->object));

   _clip_retl(ClipMachineMemory, gdk_region_empty(GDK_REGION(creg)));

   return 0;
 err:
   return 1;
}

/* Returns TRUE if the two regions are the same. */
int
clip_GDK_REGIONEQUAL(ClipMachine * ClipMachineMemory)
{
   C_object *creg = _fetch_co_arg(ClipMachineMemory);

   C_object *creg2 = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCOBJ(creg, GDK_IS_REGION(creg->object));
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(creg2, GDK_IS_REGION(creg2->object));

   _clip_retl(ClipMachineMemory, gdk_region_equal(GDK_REGION(creg), GDK_REGION(creg2)));

   return 0;
 err:
   return 1;
}

/* Returns TRUE if a point is in a region. */
int
clip_GDK_REGIONPOINTIN(ClipMachine * ClipMachineMemory)
{
   C_object *creg = _fetch_co_arg(ClipMachineMemory);

   gint      x = _clip_parni(ClipMachineMemory, 2);

   gint      y = _clip_parni(ClipMachineMemory, 3);

   CHECKCOBJ(creg, GDK_IS_REGION(creg->object));
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);

   _clip_retl(ClipMachineMemory, gdk_region_point_in(GDK_REGION(creg), x, y));

   return 0;
 err:
   return 1;
}

/* Tests whether a rectangle is within a region. */
int
clip_GDK_REGIONRECTIN(ClipMachine * ClipMachineMemory)
{
   C_object *creg = _fetch_co_arg(ClipMachineMemory);

   ClipVar  *crect = _clip_spar(ClipMachineMemory, 2);

   GdkRectangle rect;

   CHECKCOBJ(creg, GDK_IS_REGION(creg->object));
   CHECKARG(2, MAP_type_of_ClipVarType);

   _map_get_gdk_rectangle(ClipMachineMemory, crect, &rect);
   _clip_retni(ClipMachineMemory, gdk_region_rect_in(GDK_REGION(creg), &rect));

   return 0;
 err:
   return 1;
}

/* Returns the smallest rectangle which includes the entire GdkRegion. */
int
clip_GDK_REGIONGETCLIPBOX(ClipMachine * ClipMachineMemory)
{
   C_object *creg = _fetch_co_arg(ClipMachineMemory);

   ClipVar  *crect = RETPTR(ClipMachineMemory);

   GdkRectangle rect;

   CHECKCOBJ(creg, GDK_IS_REGION(creg->object));

   gdk_region_get_clipbox(GDK_REGION(creg), &rect);

   memset(crect, 0, sizeof(*crect));
   _clip_map(ClipMachineMemory, crect);
   _map_put_gdk_rectangle(ClipMachineMemory, crect, &rect);

   return 0;
 err:
   return 1;
}
