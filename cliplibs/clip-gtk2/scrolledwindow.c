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

/*********************** SIGNALS **************************/
static    gint
handler_move_focus_out_signal(GtkScrolledWindow * swin, GtkDirectionType type, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_DIRECTIONTYPE, type);
   INVOKESIGHANDLER(GTK_WIDGET(swin), cs, cv);
}

static    gint
handler_scroll_child_signal(GtkScrolledWindow * swin, GtkScrollType type, gboolean arg2, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_SCROLLTYPE, type);
   _clip_mputl(cs->cw->cmachine, &cv, HASH_ARG2, arg2);
   INVOKESIGHANDLER(GTK_WIDGET(swin), cs, cv);
}

/* Signals table */
static SignalTable scrolled_window_signals[] = {
  /* signals */
   {"move-focus-out", GSF(handler_move_focus_out_signal),
    ESF(object_emit_signal), GTK_MOVE_FOCUS_OUT_SIGNAL},
   {"scroll-child", GSF(handler_scroll_child_signal),
    ESF(object_emit_signal), GTK_SCROLL_CHILD_SIGNAL},
   {"", NULL, NULL, 0}
};

/**********************************************************/

/* Register scrolled window in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_scrolled_window()
{
   return GTK_TYPE_SCROLLED_WINDOW;
}

long
_clip_type_scrolled_window()
{
   return GTK_WIDGET_SCROLLED_WINDOW;
}

const char *
_clip_type_name_scrolled_window()
{
   return "GTK_WIDGET_SCROLLED_WINDOW";
}

int
clip_INIT___SCROLLED_WINDOW(ClipMachine * ClipMachineMemory)
{
//      _wtype_table_put(_clip_type_scrolled_window, _clip_type_name_scrolled_window, _gtk_type_scrolled_window, _gtk_type_container, scrolled_window_signals);
   _wtype_table_put(_clip_type_scrolled_window,
		    _clip_type_name_scrolled_window, _gtk_type_scrolled_window, _gtk_type_bin, scrolled_window_signals);
   return 0;
}

/**** SCROLLED WINDOW constructor ****/
int
clip_GTK_SCROLLEDWINDOWNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   C_widget *chadj = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_widget *cvadj = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   GtkWidget *wid = NULL;

   C_widget *cwid;

   C_widget *chscr, *cvscr;

   GtkAdjustment *hadj, *vadj;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(chadj, GTK_IS_ADJUSTMENT);
   CHECKOPT2(3, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cvadj, GTK_IS_ADJUSTMENT);

   hadj = chadj ? GTK_ADJUSTMENT(chadj->widget) : NULL;
   vadj = cvadj ? GTK_ADJUSTMENT(cvadj->widget) : NULL;
   wid = gtk_scrolled_window_new(hadj, vadj);
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);

   chscr = _register_widget(ClipMachineMemory, GTK_SCROLLED_WINDOW(wid)->hscrollbar, NULL);
   cvscr = _register_widget(ClipMachineMemory, GTK_SCROLLED_WINDOW(wid)->vscrollbar, NULL);

   if (chscr)
      _clip_madd(ClipMachineMemory, &cwid->obj, HASH_HSCROLLBAR, &chscr->obj);
   if (cvscr)
      _clip_madd(ClipMachineMemory, &cwid->obj, HASH_VSCROLLBAR, &cvscr->obj);

   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

/****  ------------------ ****/

/* Returns the horizontal scrollbar's adjustment, used to
 * connect the horizontal scrollbar to the child widget's
 * horizontal scroll functionality. */
int
clip_GTK_SCROLLEDWINDOWGETHADJUSTMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *csw = _fetch_cw_arg(ClipMachineMemory);

   ClipVar  *cv = RETPTR(ClipMachineMemory);

   GtkAdjustment *adj;

   C_widget *cadj;

   CHECKCWID(csw, GTK_IS_SCROLLED_WINDOW);
   adj = gtk_scrolled_window_get_hadjustment(GTK_SCROLLED_WINDOW(csw->widget));
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
clip_GTK_SCROLLEDWINDOWGETVADJUSTMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *csw = _fetch_cw_arg(ClipMachineMemory);

   ClipVar  *cv = RETPTR(ClipMachineMemory);

   GtkAdjustment *adj;

   C_widget *cadj;

   CHECKCWID(csw, GTK_IS_SCROLLED_WINDOW);
   adj = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(csw->widget));
   cadj = _list_get_cwidget(ClipMachineMemory, adj);
   if (!cadj)
      cadj = _register_widget(ClipMachineMemory, (GtkWidget *) adj, NULL);
   if (cadj)
      _clip_mclone(ClipMachineMemory, cv, &cadj->obj);
   return 0;
 err:
   return 1;
}

/* Sets the scrollbar policy for the horizontal and vertical scrollbars.
 * The policy determines when the scrollbar should appear; it is a value
 * from the GtkPolicyType enumeration. If GTK_POLICY_ALWAYS, the scrollbar
 * is always present; if GTK_POLICY_NEVER, the scrollbar is never present;
 * if GTK_POLICY_AUTOMATIC, the scrollbar is present only if needed (that
 * is, if the slider part of the bar would be smaller than the trough - the
 * display is larger than the page size). */
int
clip_GTK_SCROLLEDWINDOWSETPOLICY(ClipMachine * ClipMachineMemory)
{
   C_widget *csw = _fetch_cw_arg(ClipMachineMemory);

   int       hpol = _clip_parni(ClipMachineMemory, 2);

   int       vpol = _clip_parni(ClipMachineMemory, 3);

   CHECKCWID(csw, GTK_IS_SCROLLED_WINDOW);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(csw->widget), hpol, vpol);
   return 0;
 err:
   return 1;
}

/* Used to add children without native scrolling capabilities.
 * This is simply a convenience function; it is equivalent to adding the
 * unscrollable child to a viewport, then adding the viewport to the
 * scrolled window. If a child has native scrolling, use gtk_container_add()
 * instead of this function. */
int
clip_GTK_SCROLLEDWINDOWADDWITHVIEWPORT(ClipMachine * ClipMachineMemory)
{
   C_widget *csw = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(csw, GTK_IS_SCROLLED_WINDOW);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cwid, GTK_IS_WIDGET);
   gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(csw->widget), cwid->widget);
   return 0;
 err:
   return 1;
}

int
clip_GTK_SCROLLEDWINDOWSETSHADOWTYPE(ClipMachine * ClipMachineMemory)
{
   C_widget *csw = _fetch_cw_arg(ClipMachineMemory);

   GtkShadowType type = INT_OPTION(ClipMachineMemory, 2, 0);

   CHECKCWID(csw, GTK_IS_SCROLLED_WINDOW);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(csw->widget), type);
   return 0;
 err:
   return 1;
}

/* Sets the Adjustment for the horizontal scrollbar. */
int
clip_GTK_SCROLLEDWINDOWSETHADJUSTMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *csw = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cadj = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(csw, GTK_IS_SCROLLED_WINDOW);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cadj, GTK_IS_ADJUSTMENT);
   gtk_scrolled_window_set_hadjustment(GTK_SCROLLED_WINDOW(csw->widget), GTK_ADJUSTMENT(cadj->widget));
   return 0;
 err:
   return 1;
}

/* Sets the Adjustment for the vertical scrollbar. */
int
clip_GTK_SCROLLEDWINDOWSETVADJUSTMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *csw = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cadj = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(csw, GTK_IS_SCROLLED_WINDOW);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cadj, GTK_IS_ADJUSTMENT);
   gtk_scrolled_window_set_vadjustment(GTK_SCROLLED_WINDOW(csw->widget), GTK_ADJUSTMENT(cadj->widget));
   return 0;
 err:
   return 1;
}

/* Determines the location of the child widget with respect to the scrollbars.
 * The default is GTK_CORNER_TOP_LEFT, meaning the child is in the top left,
 * with the scrollbars underneath and to the right. Other values in GtkCornerType are
 * GTK_CORNER_TOP_RIGHT, GTK_CORNER_BOTTOM_LEFT, and GTK_CORNER_BOTTOM_RIGHT. */
int
clip_GTK_SCROLLEDWINDOWSETPLACEMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *csw = _fetch_cw_arg(ClipMachineMemory);

   int       placement = _clip_parni(ClipMachineMemory, 2);

   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKCWID(csw, GTK_IS_SCROLLED_WINDOW);
   gtk_scrolled_window_set_placement(GTK_SCROLLED_WINDOW(csw->widget), placement);
   return 0;
 err:
   return 1;
}

int
clip_GTK_SCROLLEDWINDOWGETPLACEMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *csw = _fetch_cw_arg(ClipMachineMemory);

   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   _clip_retni(ClipMachineMemory, (int) gtk_scrolled_window_get_placement(GTK_SCROLLED_WINDOW(csw->widget)));
   return 0;
 err:
   return 1;
}

int
clip_GTK_SCROLLEDWINDOWGETPOLICY(ClipMachine * ClipMachineMemory)
{
   C_widget *csw = _fetch_cw_arg(ClipMachineMemory);

   GtkPolicyType hpolicy, vpolicy;

   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_scrolled_window_get_policy(GTK_SCROLLED_WINDOW(csw->widget), &hpolicy, &vpolicy);

   _clip_storni(ClipMachineMemory, (int) hpolicy, 2, 0);
   _clip_storni(ClipMachineMemory, (int) vpolicy, 3, 0);
   return 0;
 err:
   return 1;
}

int
clip_GTK_SCROLLEDWINDOWGETSHADOWTYPE(ClipMachine * ClipMachineMemory)
{
   C_widget *csw = _fetch_cw_arg(ClipMachineMemory);

   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   _clip_retni(ClipMachineMemory, (int) gtk_scrolled_window_get_shadow_type(GTK_SCROLLED_WINDOW(csw->widget)));
   return 0;
 err:
   return 1;
}
