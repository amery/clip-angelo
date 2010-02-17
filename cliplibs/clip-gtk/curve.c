/*
    Copyright (C) 2001  ITK
    Author  : Alexey M. Tkachenko <alexey@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg.h"

#include <gtk/gtk.h>

#include "ci_clip-gtk.ch"
#include "ci_clip-gtk.h"

/**********************************************************/
/* Signals table */
static SignalTable curve_signals[] = {
   {"curve-type-changed", GSF(widget_signal_handler),
    ESF(object_emit_signal), GTK_CURVE_TYPE_CHANGED_SIGNAL},
   {"", NULL, NULL, 0}
};

/**********************************************************/

/* Register curve in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_curve()
{
   return GTK_TYPE_CURVE;
}

long
_clip_type_curve()
{
   return GTK_WIDGET_CURVE;
}

const char *
_clip_type_name_curve()
{
   return "GTK_WIDGET_CURVE";
}

CLIP_DLLEXPORT GtkType
_gtk_type_gamma_curve()
{
   return gtk_gamma_curve_get_type();
}

long
_clip_type_gamma_curve()
{
   return GTK_WIDGET_GAMMA_CURVE;
}

const char *
_clip_type_name_gamma_curve()
{
   return "GTK_WIDGET_GAMMA_CURVE";
}

int
clip_INIT___CURVE(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_curve, _clip_type_name_curve, _gtk_type_curve, _gtk_type_drawing_area, curve_signals);
   _wtype_table_put(_clip_type_gamma_curve, _clip_type_name_gamma_curve, _gtk_type_gamma_curve, _gtk_type_curve, NULL);
   return 0;
}

/**********************************************************/

/**** Curve constructor ****/
int
clip_GTK_CURVENEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   GtkWidget *wid = NULL;
   C_widget *cwid;
   CHECKOPT(1, MAP_type_of_ClipVarType);

   wid = gtk_curve_new();
   if (!wid)
      goto err;

   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

/**** Gamma Curve constructor ****/
int
clip_GTK_GAMMACURVENEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   GtkWidget *wid = NULL;
   C_widget *cwid;
   CHECKOPT(1, MAP_type_of_ClipVarType);

   wid = gtk_gamma_curve_new();
   if (!wid)
      goto err;

   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

/* Resets the curve to a straight line from the minimum x & y values to the
 * maximum x & y values (i.e. from the bottom-left to the top-right corners).
 * The curve type is not changed. */
int
clip_GTK_CURVERESET(ClipMachine * ClipMachineMemory)
{
   C_widget *ccur = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(ccur, GTK_CURVE);
   gtk_curve_reset(GTK_CURVE(ccur->widget));
   return 0;
 err:
   return 1;
}

/* Recomputes the entire curve using the given gamma value. A gamma value of 1
 * results in a straight line. Values greater than 1 result in a curve above the
 * straight line. Values less than 1 result in a curve below the straight line.
 * The curve type is changed to GTK_CURVE_TYPE_FREE. */
int
clip_GTK_CURVESETGAMMA(ClipMachine * ClipMachineMemory)
{
   C_widget *ccur = _fetch_cw_arg(ClipMachineMemory);
   gfloat gamma = _clip_parnd(ClipMachineMemory, 2);
   CHECKCWID(ccur, GTK_CURVE);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      gamma = 1;
   gtk_curve_set_gamma(GTK_CURVE(ccur->widget), gamma);
   return 0;
 err:
   return 1;
}

/* Sets the minimum and maximum x & y values of the curve.
 * The curve is also reset with a call to gtk_curve_reset(). */
int
clip_GTK_CURVESETRANGE(ClipMachine * ClipMachineMemory)
{
   C_widget *ccur = _fetch_cw_arg(ClipMachineMemory);
   gfloat min_x = _clip_parnd(ClipMachineMemory, 2);
   gfloat max_x = _clip_parnd(ClipMachineMemory, 3);
   gfloat min_y = _clip_parnd(ClipMachineMemory, 4);
   gfloat max_y = _clip_parnd(ClipMachineMemory, 5);
   CHECKCWID(ccur, GTK_CURVE);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, NUMERIC_type_of_ClipVarType);
   CHECKOPT(5, NUMERIC_type_of_ClipVarType);
   gtk_curve_set_range(GTK_CURVE(ccur->widget), min_x, max_x, min_y, max_y);
   return 0;
 err:
   return 1;
}

/* Returns a vector of points representing the curve. */
int
clip_GTK_CURVEGETVECTOR(ClipMachine * ClipMachineMemory)
{
   C_widget *ccur = _fetch_cw_arg(ClipMachineMemory);
   int veclen = _clip_parni(ClipMachineMemory, 2);
   gfloat *vector = calloc(veclen, sizeof(gfloat));
   long n = 0, i;
   ClipVar *a = RETPTR(ClipMachineMemory);
   CHECKCWID(ccur, GTK_CURVE);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   _clip_array(ClipMachineMemory, a, veclen, &n);
   gtk_curve_get_vector(GTK_CURVE(ccur->widget), veclen, vector);
   for (i = 0; i < veclen; i++)
      {
	 a->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar[i].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	a->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar[i].ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = vector[i];
      }
   return 0;
 err:
   return 1;
}

/* Sets the vector of points on the curve. The curve type is set to GTK_CURVE_TYPE_FREE. */
int
clip_GTK_CURVESETVECTOR(ClipMachine * ClipMachineMemory)
{
   C_widget *ccur = _fetch_cw_arg(ClipMachineMemory);
   int veclen = _clip_parni(ClipMachineMemory, 2);
   ClipVar *cvvec = _clip_spar(ClipMachineMemory, 3);
   gfloat *vector = calloc(veclen, sizeof(gfloat));
   long i;
   CHECKCWID(ccur, GTK_CURVE);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, ARRAY_type_of_ClipVarType);
   for (i = 0; i < veclen; i++)
      {
	 if (cvvec->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar[i].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
		 vector[i] = cvvec->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar[i].ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
      }
   gtk_curve_set_vector(GTK_CURVE(ccur->widget), veclen, vector);
   return 0;
 err:
   return 1;
}

/* Sets the type of the curve. The curve will remain unchanged except when changing
 * from a free curve to a linear or spline curve, in which case the curve will be
 * changed as little as possible. */
int
clip_GTK_CURVESETCURVETYPE(ClipMachine * ClipMachineMemory)
{
   C_widget *ccur = _fetch_cw_arg(ClipMachineMemory);
   GtkCurveType type = _clip_parni(ClipMachineMemory, 2);
   CHECKCWID(ccur, GTK_CURVE);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_curve_set_curve_type(GTK_CURVE(ccur->widget), type);
   return 0;
 err:
   return 1;
}
