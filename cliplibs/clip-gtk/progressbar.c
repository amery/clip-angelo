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

/*********************** SIGNALS **************************/

/****         Progress bar has no own signal           ****/

/**********************************************************/

/* Register container signals table in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_progress_bar()
{
   return GTK_TYPE_PROGRESS_BAR;
}

long
_clip_type_progress_bar()
{
   return GTK_WIDGET_PROGRESS_BAR;
}

const char *
_clip_type_name_progress_bar()
{
   return "GTK_WIDGET_PROGRESS_BAR";
}

int
clip_INIT___PROGRESS_BAR(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_progress_bar, _clip_type_name_progress_bar, _gtk_type_progress_bar, _gtk_type_progress, NULL);
   return 0;
}

/**********************************************************/

int
clip_GTK_PROGRESSBARNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   C_widget *cadj = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   GtkWidget *wid = NULL;

   C_widget *cwid;

   GtkAdjustment *adj = 0;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cadj, GTK_IS_ADJUSTMENT);

   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      wid = gtk_progress_bar_new();
   else
      wid = gtk_progress_bar_new_with_adjustment(adj);
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

/* Controls whether progress text is shown. */
int
clip_GTK_PROGRESSBARSETBARSTYLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cprg = _fetch_cw_arg(ClipMachineMemory);

   GtkProgressBarStyle style = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(cprg, GTK_IS_PROGRESS);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_progress_bar_set_bar_style(GTK_PROGRESS_BAR(cprg->widget), style);
   return 0;
 err:
   return 1;
}

/* Sets the number of blocks that the progress bar is divided into when the
 * style is GTK_PROGRESS_DISCRETE. */
int
clip_GTK_PROGRESSBARSETDISCRETEBLOCKS(ClipMachine * ClipMachineMemory)
{
   C_widget *cprg = _fetch_cw_arg(ClipMachineMemory);

   guint     blocks = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(cprg, GTK_IS_PROGRESS);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_progress_bar_set_discrete_blocks(GTK_PROGRESS_BAR(cprg->widget), blocks);
   return 0;
 err:
   return 1;
}

/* Sets the step value used when the progress bar is in activity mode.
 * The step is the amount by which the progress is incremented each iteration. */
int
clip_GTK_PROGRESSBARSETACTIVITYSTEP(ClipMachine * ClipMachineMemory)
{
   C_widget *cprg = _fetch_cw_arg(ClipMachineMemory);

   guint     step = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(cprg, GTK_IS_PROGRESS);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_progress_bar_set_activity_step(GTK_PROGRESS_BAR(cprg->widget), step);
   return 0;
 err:
   return 1;
}

/* Sets the number of blocks used when the progress bar is in activity mode.
 * Larger numbers make the visible block smaller. */
int
clip_GTK_PROGRESSBARSETACTIVITYBLOCKS(ClipMachine * ClipMachineMemory)
{
   C_widget *cprg = _fetch_cw_arg(ClipMachineMemory);

   guint     blocks = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(cprg, GTK_IS_PROGRESS);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_progress_bar_set_activity_blocks(GTK_PROGRESS_BAR(cprg->widget), blocks);
   return 0;
 err:
   return 1;
}

/* Sets the orientation of the progress bar. This controls whether the bar is
 * horizontal or vertical and the direction in which it grows. */
int
clip_GTK_PROGRESSBARSETORIENTATION(ClipMachine * ClipMachineMemory)
{
   C_widget *cprg = _fetch_cw_arg(ClipMachineMemory);

   GtkProgressBarOrientation orientation = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(cprg, GTK_IS_PROGRESS);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_progress_bar_set_orientation(GTK_PROGRESS_BAR(cprg->widget), orientation);
   return 0;
 err:
   return 1;
}
