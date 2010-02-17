/*
    Copyright (C) 2001-2005  ITK
    Author  : Alexey M. Tkachenko <alexey@itk.ru>
    	      Elena V. Kornilova <alena@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg2.h"

#include <gtk/gtk.h>

#include "ci_clip-gtk2.ch"
#include "ci_clip-gtk2.h"

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
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   GtkWidget *wid = NULL;
   C_widget *cwid;
   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      wid = gtk_progress_bar_new();
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

/* Indicates that some progress is made, but you don't know how much.      */
/* Causes the progress bar to enter "activity mode," where a block bounces */
/* back and forth. Each call to gtk_progress_bar_pulse() causes the block  */
/* to move by a little bit (the amount of movement per pulse is determined */
/* by gtk_progress_bar_set_pulse_step()).                                  */

int
clip_GTK_PROGRESSBARPULSE(ClipMachine * ClipMachineMemory)
{
   C_widget *cpbar = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cpbar, GTK_IS_PROGRESS_BAR);
   gtk_progress_bar_pulse(GTK_PROGRESS_BAR(cpbar->widget));
   return 0;
 err:
   return 1;
}

/* Causes the given text to appear superimposed on the progress bar. */

int
clip_GTK_PROGRESSBARSETTEXT(ClipMachine * ClipMachineMemory)
{
   C_widget *cpbar = _fetch_cw_arg(ClipMachineMemory);
   gchar *text = _clip_parc(ClipMachineMemory, 2);

   CHECKCWID(cpbar, GTK_IS_PROGRESS_BAR);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(text);
   gtk_progress_bar_set_text(GTK_PROGRESS_BAR(cpbar->widget), text);
   FREE_TEXT(text);
   return 0;
 err:
   return 1;
}

/* Causes the progress bar to "fill in" the given fraction of the bar. */
/* The fraction should be between 0.0 and 1.0, inclusive.              */
int
clip_GTK_PROGRESSBARSETFRACTION(ClipMachine * ClipMachineMemory)
{
   C_widget *cpbar = _fetch_cw_arg(ClipMachineMemory);
   gdouble fraction = _clip_parnd(ClipMachineMemory, 2);

   CHECKCWID(cpbar, GTK_IS_PROGRESS_BAR);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);

   gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(cpbar->widget), fraction);
   return 0;
 err:
   return 1;
}

/* Sets the fraction of total progress bar length to move the bouncing block */
/* for each call to gtk_progress_bar_pulse().                                */

int
clip_GTK_PROGRESSBARSETPULSESTEP(ClipMachine * ClipMachineMemory)
{
   C_widget *cpbar = _fetch_cw_arg(ClipMachineMemory);
   gdouble step = _clip_parnd(ClipMachineMemory, 2);

   CHECKCWID(cpbar, GTK_IS_PROGRESS_BAR);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);

   gtk_progress_bar_set_pulse_step(GTK_PROGRESS_BAR(cpbar->widget), step);
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

int
clip_GTK_PROGRESSBARGETTEXT(ClipMachine * ClipMachineMemory)
{
   C_widget *cpbar = _fetch_cw_arg(ClipMachineMemory);
   gchar *text;

   CHECKCWID(cpbar, GTK_IS_PROGRESS_BAR);

   text = (gchar *) gtk_progress_bar_get_text(GTK_PROGRESS_BAR(cpbar->widget));
   LOCALE_FROM_UTF(text);
   _clip_retc(ClipMachineMemory, text);
   FREE_TEXT(text);
   return 0;
 err:
   return 1;
}

int
clip_GTK_PROGRESSBARGETFRACTION(ClipMachine * ClipMachineMemory)
{
   C_widget *cpbar = _fetch_cw_arg(ClipMachineMemory);
   gdouble fraction;

   CHECKCWID(cpbar, GTK_IS_PROGRESS_BAR);

   fraction = gtk_progress_bar_get_fraction(GTK_PROGRESS_BAR(cpbar->widget));
   _clip_retnd(ClipMachineMemory, fraction);
   return 0;
 err:
   return 1;
}

int
clip_GTK_PROGRESSBARGETPULSESTEP(ClipMachine * ClipMachineMemory)
{
   C_widget *cpbar = _fetch_cw_arg(ClipMachineMemory);
   gdouble step;

   CHECKCWID(cpbar, GTK_IS_PROGRESS_BAR);

   step = gtk_progress_bar_get_pulse_step(GTK_PROGRESS_BAR(cpbar->widget));
   _clip_retnd(ClipMachineMemory, step);
   return 0;
 err:
   return 1;
}

int
clip_GTK_PROGRESSBARGETORIENTATION(ClipMachine * ClipMachineMemory)
{
   C_widget *cprg = _fetch_cw_arg(ClipMachineMemory);
   GtkProgressBarOrientation orientation;

   CHECKCWID(cprg, GTK_IS_PROGRESS);
   orientation = gtk_progress_bar_get_orientation(GTK_PROGRESS_BAR(cprg->widget));
   _clip_retni(ClipMachineMemory, orientation);
   return 0;
 err:
   return 1;
}

#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 6)
int
clip_GTK_PROGRESSBARSETELLIPSIZE(ClipMachine * ClipMachineMemory)
{
   C_widget *cpbar = _fetch_cw_arg(ClipMachineMemory);
   PangoEllipsizeMode mode = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(cpbar, GTK_IS_PROGRESS_BAR);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_progress_bar_set_ellipsize(GTK_PROGRESS_BAR(cpbar->widget), mode);
   return 0;
 err:
   return 1;
}
#endif
