/*
    Copyright (C) 2004  ITK
    Author  : Alexey M. Tkachenko <alexey@itk.ru>
    	      Elena V. Kornilovs <alena@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include <string.h>
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg2.h"

#include <gtk/gtk.h>

#include "ci_clip-gtk2.ch"
#include "ci_clip-gtk2.h"

/**********************************************************/
/* Signal handlers */
static gint
handle_text_pushed_signal(GtkWidget * widget, guint context_id, gchar * text, C_signal * cs)
{
   PREPARECV(cs, cv);
   LOCALE_FROM_UTF(text);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_CONTEXTID, context_id);
   _clip_mputc(cs->cw->cmachine, &cv, HASH_TEXT, text, strlen(text));
   FREE_TEXT(text);
   INVOKESIGHANDLER(widget, cs, cv);
}

static gint
handle_text_popped_signal(GtkWidget * widget, guint context_id, gchar * text, C_signal * cs)
{
   PREPARECV(cs, cv);
   LOCALE_FROM_UTF(text);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_CONTEXTID, context_id);
   _clip_mputc(cs->cw->cmachine, &cv, HASH_TEXT, text, strlen(text));
   FREE_TEXT(text);
   INVOKESIGHANDLER(widget, cs, cv);
}

/* Signals table */
static SignalTable statusbar_signals[] = {
   {"text-pushed", GSF(handle_text_pushed_signal), ESF(object_emit_signal),
    GTK_TEXT_PUSHED_SIGNAL},
   {"text-popped", GSF(handle_text_popped_signal), ESF(object_emit_signal),
    GTK_TEXT_POPPED_SIGNAL},
   {"", NULL, NULL, 0}
};

/**********************************************************/

/* Register status bar in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_status_bar()
{
   return gtk_statusbar_get_type();
}

long
_clip_type_status_bar()
{
   return GTK_WIDGET_STATUSBAR;
}

const char *
_clip_type_name_status_bar()
{
   return "GTK_WIDGET_STATUSBAR";
}

int
clip_INIT___STATUSBAR(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_status_bar, _clip_type_name_status_bar, _gtk_type_status_bar, _gtk_type_hbox, statusbar_signals);
   return 0;
}

/**** Status bar constructor ****/
int
clip_GTK_STATUSBARNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   GtkWidget *wid = NULL;
   C_widget *cwid;
   CHECKOPT(1, MAP_type_of_ClipVarType);

   wid = gtk_statusbar_new();
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

int
clip_GTK_STATUSBARGETCONTEXTID(ClipMachine * ClipMachineMemory)
{
   C_widget *cstb = _fetch_cw_arg(ClipMachineMemory);
   gchar *context_descr = _clip_parc(ClipMachineMemory, 2);
   CHECKCWID(cstb, GTK_IS_STATUSBAR);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);
   LOCALE_TO_UTF(context_descr);
   _clip_retni(ClipMachineMemory, gtk_statusbar_get_context_id(GTK_STATUSBAR(cstb->widget), context_descr));
   FREE_TEXT(context_descr);
   return 0;
 err:
   return 1;
}

/* Returns message_id used for gtk_statusbar_remove */
int
clip_GTK_STATUSBARPUSH(ClipMachine * ClipMachineMemory)
{
   C_widget *cstb = _fetch_cw_arg(ClipMachineMemory);
   guint context_id = _clip_parni(ClipMachineMemory, 2);
   gchar *text = _clip_parc(ClipMachineMemory, 3);
   CHECKCWID(cstb, GTK_IS_STATUSBAR);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, CHARACTER_type_of_ClipVarType);
   LOCALE_TO_UTF(text);
   _clip_retni(ClipMachineMemory, gtk_statusbar_push(GTK_STATUSBAR(cstb->widget), context_id, text));
   FREE_TEXT(text);
   return 0;
 err:
   return 1;
}

int
clip_GTK_STATUSBARPOP(ClipMachine * ClipMachineMemory)
{
   C_widget *cstb = _fetch_cw_arg(ClipMachineMemory);
   guint context_id = _clip_parni(ClipMachineMemory, 2);
   CHECKCWID(cstb, GTK_IS_STATUSBAR);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   gtk_statusbar_pop(GTK_STATUSBAR(cstb->widget), context_id);
   return 0;
 err:
   return 1;
}

int
clip_GTK_STATUSBARREMOVE(ClipMachine * ClipMachineMemory)
{
   C_widget *cstb = _fetch_cw_arg(ClipMachineMemory);
   guint context_id = _clip_parni(ClipMachineMemory, 2);
   guint message_id = _clip_parni(ClipMachineMemory, 3);
   CHECKCWID(cstb, GTK_IS_STATUSBAR);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   gtk_statusbar_remove(GTK_STATUSBAR(cstb->widget), context_id, message_id);
   return 0;
 err:
   return 1;
}

int
clip_GTK_STATUSBARSETHASRESIZEGRIP(ClipMachine * ClipMachineMemory)
{
   C_widget *cstb = _fetch_cw_arg(ClipMachineMemory);
   gboolean setting = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(cstb, GTK_IS_STATUSBAR);
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_statusbar_set_has_resize_grip(GTK_STATUSBAR(cstb->widget), setting);
   return 0;
 err:
   return 1;
}

int
clip_GTK_STATUSBARGETHASRESIZEGRIP(ClipMachine * ClipMachineMemory)
{
   C_widget *cstb = _fetch_cw_arg(ClipMachineMemory);
   gboolean setting;

   CHECKCWID(cstb, GTK_IS_STATUSBAR);

   setting = gtk_statusbar_get_has_resize_grip(GTK_STATUSBAR(cstb->widget));

   _clip_retl(ClipMachineMemory, setting);
   return 0;
 err:
   return 1;
}

/* Set text for status bar */
int
clip_GTK_STATUSBARSETTEXT(ClipMachine * ClipMachineMemory)
{
   C_widget *cstb = _fetch_cw_arg(ClipMachineMemory);
   gchar *text = _clip_parc(ClipMachineMemory, 2);
   CHECKCWID(cstb, GTK_IS_STATUSBAR);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);
   LOCALE_TO_UTF(text);
   gtk_label_set_text(GTK_LABEL(GTK_STATUSBAR(cstb->widget)->label), text);
   FREE_TEXT(text);
   return 0;
 err:
   return 1;
}

int
clip_GTK_STATUSBARSETSTYLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cstb = _fetch_cw_arg(ClipMachineMemory);
   ClipVar *mstyle = _clip_spar(ClipMachineMemory, 2);
   GtkStyle *style;
   CHECKCWID(cstb, GTK_IS_STATUSBAR);
   CHECKARG(2, MAP_type_of_ClipVarType);
   style = gtk_style_copy(GTK_STATUSBAR(cstb->widget)->label->style);
  //gtk_style_unref(GTK_STATUSBAR(cstb->widget)->label->style);
   _map_to_style(ClipMachineMemory, mstyle, style);
   gtk_widget_set_style(GTK_STATUSBAR(cstb->widget)->label, style);
   return 0;
 err:
   return 1;
}

int
clip_GTK_STATUSBARSETJUSTIFY(ClipMachine * ClipMachineMemory)
{
   C_widget *cstb = _fetch_cw_arg(ClipMachineMemory);
   GtkJustification jtype = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(cstb, GTK_IS_STATUSBAR);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_label_set_justify(GTK_LABEL(GTK_STATUSBAR(cstb->widget)->label), jtype);

   return 0;
 err:
   return 1;
}

/* Alena */
/* Returns a label from status bar */
int
clip_GTK_STATUSBARGETLABEL(ClipMachine * ClipMachineMemory)
{
   C_widget *cstb = _fetch_cw_arg(ClipMachineMemory);
   C_widget *clbl;

   CHECKCWID(cstb, GTK_IS_STATUSBAR);

   clbl = _get_cwidget(ClipMachineMemory, GTK_STATUSBAR(cstb->widget)->label);
   if (clbl)
      _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &clbl->obj);
   return 0;
 err:
   return 1;
}
