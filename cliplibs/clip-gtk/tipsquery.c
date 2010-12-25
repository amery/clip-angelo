/*
    Copyright (C) 2002  ITK
    Author  : Alexey M. Tkachenko <alexey@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg.h"

#include <gtk/gtk.h>
#include <string.h>

#include "ci_clip-gtk.ch"
#include "ci_clip-gtk.h"

/*********************** SIGNALS **************************/

/* Signal handlers */
static    gint
handle_widget_entered_signal(GtkWidget * widget, GtkWidget * wid, gchar * tip_text, gchar * tip_private, C_signal * cs)
{
   C_widget *cwid = _list_get_cwidget(cs->cw->cmachine, wid);

   PREPARECV(cs, cv);
   if (!cwid)
      cwid = _register_widget(cs->cw->cmachine, wid, NULL);
   if (cwid)
      _clip_madd(cs->cw->cmachine, &cv, HASH_WIDGET, &cwid->obj);
   LOCALE_FROM_UTF(tip_text);
   LOCALE_FROM_UTF(tip_private);
   _clip_mputc(cs->cw->cmachine, &cv, HASH_TIPTEXT, tip_text, strlen(tip_text));
   _clip_mputc(cs->cw->cmachine, &cv, HASH_TIPPRIVATE, tip_private, strlen(tip_private));
   FREE_TEXT(tip_text);
   FREE_TEXT(tip_private);
   INVOKESIGHANDLER(widget, cs, cv);
}

static    gint
handle_widget_selected_signal(GtkWidget * widget, GtkWidget * wid,
			      gchar * tip_text, gchar * tip_private, GdkEventButton * event, C_signal * cs)
{
   C_widget *cwid = _list_get_cwidget(cs->cw->cmachine, wid);

   PREPARECV(cs, cv);
   if (!cwid)
      cwid = _register_widget(cs->cw->cmachine, wid, NULL);
   if (cwid)
      _clip_madd(cs->cw->cmachine, &cv, HASH_WIDGET, &cwid->obj);
   LOCALE_FROM_UTF(tip_text);
   LOCALE_FROM_UTF(tip_private);
   _clip_mputc(cs->cw->cmachine, &cv, HASH_TIPTEXT, tip_text, strlen(tip_text));
   _clip_mputc(cs->cw->cmachine, &cv, HASH_TIPPRIVATE, tip_private, strlen(tip_private));
   _clip_mputn(cs->cw->cmachine, &cv, HASH_BUTTON, event->button);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_X, event->x);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_Y, event->y);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_XROOT, event->x_root);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_YROOT, event->y_root);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_PRESSURE, event->pressure);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_XTILT, event->xtilt);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_YTILT, event->ytilt);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_STATE, event->state);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_SOURCE, event->source);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_DEVICIED, event->deviceid);
   FREE_TEXT(tip_text);
   FREE_TEXT(tip_private);
   INVOKESIGHANDLER(widget, cs, cv);
}

/* Signals table */
static SignalTable tips_query_signals[] = {
  /* Emitted when the query is started. */
   {"start-query", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_START_QUERY_SIGNAL},
  /* Emitted when the query is stopped. */
   {"stop-query", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_STOP_QUERY_SIGNAL},
  /* Emitted when a widget is entered by the pointer while the query is in effect. */
   {"widget-entered", GSF(handle_widget_entered_signal),
    ESF(object_emit_signal), GTK_WIDGET_ENTERED_SIGNAL},
  /* Emitted when a widget is selected during a query. */
   {"widget-selected", GSF(handle_widget_selected_signal),
    ESF(object_emit_signal), GTK_WIDGET_SELECTED_SIGNAL},
   {"", NULL, NULL, 0}
};

/**********************************************************/
CLIP_DLLEXPORT GtkType
_gtk_type_tips_query()
{
   return GTK_TYPE_TIPS_QUERY;
}

long
_clip_type_tips_query()
{
   return GTK_WIDGET_TIPS_QUERY;
}

const char *
_clip_type_name_tips_query()
{
   return "GTK_TYPE_TIPS_QUERY";
}

/* Register toolbar in global table */
int
clip_INIT___TIPS_QUERY(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_tips_query, _clip_type_name_tips_query,
		    _gtk_type_tips_query, _gtk_type_label, tips_query_signals);
   return 0;
}

/**** ------------------ ****/
/**** TIPS QUERY constructor ****/
int
clip_GTK_TIPSQUERYNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   GtkWidget *wid = NULL;

   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   wid = gtk_tips_query_new();
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

/* Starts a query. The GtkTipsQuery widget will take control of the
 * mouse and as the mouse moves it will display the tooltip of the
 * widget beneath the mouse. */
int
clip_GTK_TIPSQUERYSTARTQUERY(ClipMachine * ClipMachineMemory)
{
   C_widget *ctq = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(ctq, GTK_IS_TIPS_QUERY);
   gtk_tips_query_start_query(GTK_TIPS_QUERY(ctq->widget));
   return 0;
 err:
   return 1;
}

/* Stops a query. */
int
clip_GTK_TIPSQUERYSTOPQUERY(ClipMachine * ClipMachineMemory)
{
   C_widget *ctq = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(ctq, GTK_IS_TIPS_QUERY);
   gtk_tips_query_stop_query(GTK_TIPS_QUERY(ctq->widget));
   return 0;
 err:
   return 1;
}

/* Sets the widget which initiates the query, usually a button. If
 * the caller is selected while the query is running, the query is
 * automatically stopped. */
int
clip_GTK_TIPSQUERYSETCALLER(ClipMachine * ClipMachineMemory)
{
   C_widget *ctq = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(ctq, GTK_IS_TIPS_QUERY);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cwid, GTK_IS_WIDGET);
   gtk_tips_query_set_caller(GTK_TIPS_QUERY(ctq->widget), cwid->widget);
   return 0;
 err:
   return 1;
}

/* Sets the text to display when the query is not in effect, and the
 * text to display when the query is in effect but the widget beneath
 * the pointer has no tooltip. */
int
clip_GTK_TIPSQUERYSETLABELS(ClipMachine * ClipMachineMemory)
{
   C_widget *ctq = _fetch_cw_arg(ClipMachineMemory);

   gchar    *label_inactive = _clip_parc(ClipMachineMemory, 2);

   gchar    *label_no_tip = _clip_parc(ClipMachineMemory, 3);

   CHECKCWID(ctq, GTK_IS_TIPS_QUERY);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT(3, CHARACTER_type_of_ClipVarType);
   LOCALE_TO_UTF(label_inactive);
   LOCALE_TO_UTF(label_no_tip);
   gtk_tips_query_set_labels(GTK_TIPS_QUERY(ctq->widget), label_inactive, label_no_tip);
   FREE_TEXT(label_inactive);
   FREE_TEXT(label_no_tip);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TIPSQUERYSETEMITALWAYS(ClipMachine * ClipMachineMemory)
{
   C_widget *ctq = _fetch_cw_arg(ClipMachineMemory);

   gboolean  always = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(ctq, GTK_IS_TIPS_QUERY);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   gtk_widget_set(ctq->widget, "emit-always", always, NULL);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TIPSQUERYGETEMITALWAYS(ClipMachine * ClipMachineMemory)
{
   C_widget *ctq = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(ctq, GTK_IS_TIPS_QUERY);
   _clip_retl(ClipMachineMemory, GTK_TIPS_QUERY(ctq->widget)->emit_always);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TIPSQUERYGETCALLER(ClipMachine * ClipMachineMemory)
{
   C_widget *ctq = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cwid = NULL;

   CHECKCWID(ctq, GTK_IS_TIPS_QUERY);
   if (GTK_TIPS_QUERY(ctq->widget)->caller)
    {
       cwid = _list_get_cwidget(ClipMachineMemory, GTK_TIPS_QUERY(ctq->widget)->caller);
       if (!cwid)
	  cwid = _register_widget(ClipMachineMemory, GTK_TIPS_QUERY(ctq->widget)->caller, NULL);
       if (cwid)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
    }
   return 0;
 err:
   return 1;
}

int
clip_GTK_TIPSQUERYGETLABELINACTIVE(ClipMachine * ClipMachineMemory)
{
   C_widget *ctq = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(ctq, GTK_IS_TIPS_QUERY);
   _clip_retc(ClipMachineMemory, GTK_TIPS_QUERY(ctq->widget)->label_inactive);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TIPSQUERYGETLABELNOTIP(ClipMachine * ClipMachineMemory)
{
   C_widget *ctq = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(ctq, GTK_IS_TIPS_QUERY);
   _clip_retc(ClipMachineMemory, GTK_TIPS_QUERY(ctq->widget)->label_no_tip);
   return 0;
 err:
   return 1;
}
