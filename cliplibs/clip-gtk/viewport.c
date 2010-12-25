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

/* Register Viewport in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_viewport()
{
   return GTK_TYPE_VIEWPORT;
}

long
_clip_type_viewport()
{
   return GTK_WIDGET_VIEWPORT;
}

const char *
_clip_type_name_viewport()
{
   return "GTK_WIDGET_VIEWPORT";
}

int
clip_INIT___VIEWPORT(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_viewport, _clip_type_name_viewport, _gtk_type_viewport, _gtk_type_container, NULL);
   return 0;
}

/**** Viewport constructor ****/
int
clip_GTK_VIEWPORTNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   C_widget *chadj = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_widget *cvadj = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   GtkWidget *wid = NULL;

   C_widget *cwid;

   GtkAdjustment *hadj, *vadj;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(chadj, GTK_IS_ADJUSTMENT);
   CHECKOPT2(3, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cvadj, GTK_IS_ADJUSTMENT);

   hadj = chadj ? GTK_ADJUSTMENT(chadj->widget) : NULL;
   vadj = cvadj ? GTK_ADJUSTMENT(cvadj->widget) : NULL;
   wid = gtk_viewport_new(hadj, vadj);
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

/* Returns the horizontal scrollbar's adjustment, used to
 * connect the horizontal scrollbar to the child widget's
 * horizontal scroll functionality. */
int
clip_GTK_VIEWPORTGETHADJUSTMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *cvp = _fetch_cw_arg(ClipMachineMemory);

   ClipVar  *cv = RETPTR(ClipMachineMemory);

   GtkAdjustment *adj;

   C_widget *cadj;

   CHECKCWID(cvp, GTK_IS_VIEWPORT);
   adj = gtk_viewport_get_hadjustment(GTK_VIEWPORT(cvp->widget));
   cadj = _list_get_cwidget(ClipMachineMemory, adj);
   if (!cadj)
      cadj = _register_widget(ClipMachineMemory, (GtkWidget *) adj, NULL);
   if (cadj)
      _clip_mclone(ClipMachineMemory, cv, &cadj->obj);
   return 0;
 err:
   return 1;
}

/* Returns the vertical scrollbar's adjustment, used to
 * connect the vertical scrollbar to the child widget's
 * vertical scroll functionality. */
int
clip_GTK_VIEWPORTGETVADJUSTMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *cvp = _fetch_cw_arg(ClipMachineMemory);

   ClipVar  *cv = RETPTR(ClipMachineMemory);

   GtkAdjustment *adj;

   C_widget *cadj;

   CHECKCWID(cvp, GTK_IS_VIEWPORT);
   adj = gtk_viewport_get_vadjustment(GTK_VIEWPORT(cvp->widget));
   cadj = _list_get_cwidget(ClipMachineMemory, adj);
   if (!cadj)
      cadj = _register_widget(ClipMachineMemory, (GtkWidget *) adj, NULL);
   if (cadj)
      _clip_mclone(ClipMachineMemory, cv, &cadj->obj);
   return 0;
 err:
   return 1;
}

/* Sets the Adjustment for the horizontal scrollbar. */
int
clip_GTK_VIEWPORTSETHADJUSTMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *cvp = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cadj = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(cvp, GTK_IS_VIEWPORT);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cadj, GTK_IS_ADJUSTMENT);
   gtk_viewport_set_hadjustment(GTK_VIEWPORT(cvp->widget), GTK_ADJUSTMENT(cadj->widget));
   return 0;
 err:
   return 1;
}

/* Sets the Adjustment for the vertical scrollbar. */
int
clip_GTK_VIEWPORTSETVADJUSTMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *cvp = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cadj = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(cvp, GTK_IS_VIEWPORT);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cadj, GTK_IS_ADJUSTMENT);
   gtk_viewport_set_vadjustment(GTK_VIEWPORT(cvp->widget), GTK_ADJUSTMENT(cadj->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_VIEWPORTSETSHADOWTYPE(ClipMachine * ClipMachineMemory)
{
   C_widget *cvp = _fetch_cw_arg(ClipMachineMemory);

   GtkShadowType type = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(cvp, GTK_IS_VIEWPORT);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_viewport_set_shadow_type(GTK_VIEWPORT(cvp->widget), type);
   return 0;
 err:
   return 1;
}

int
clip_GTK_VIEWPORTGETSHADOWTYPE(ClipMachine * ClipMachineMemory)
{
   C_widget *cvp = _fetch_cw_arg(ClipMachineMemory);

   GtkArg    arg;

   CHECKCWID(cvp, GTK_IS_VIEWPORT);
   arg.type = GTK_TYPE_INT;
   arg.name = "shadow-type";
   gtk_widget_get(cvp->widget, &arg);
   _clip_retni(ClipMachineMemory, arg.d.int_data);
   return 0;
 err:
   return 1;
}
