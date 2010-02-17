/*
    Copyright (C) 2001  ITK
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
static gint
container_handle_signal(GtkContainer * container, GtkWidget * widget, C_signal * cs)
{
   C_widget *cwid;
   PREPARECV(cs, cv);
   cwid = _list_get_cwidget(cs->cw->cmachine, widget);
   if (!cwid)
      cwid = _register_widget(cs->cw->cmachine, widget, NULL);
   if (cwid)
      _clip_madd(cs->cw->cmachine, &cv, HASH_WIDGET, &cwid->obj);
   INVOKESIGHANDLER(GTK_WIDGET(container), cs, cv);
}

static SignalTable container_signals[] = {
   {"add", GSF(container_handle_signal), ESF(object_emit_signal),
    GTK_ADD_SIGNAL},
   {"remove", GSF(container_handle_signal), ESF(object_emit_signal),
    GTK_REMOVE_SIGNAL},
   {"check-resize", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_CHECK_RESIZE_SIGNAL},
   {"focus", GSF(container_handle_signal), ESF(object_emit_signal),
    GTK_FOCUS_SIGNAL},
   {"set-focus-child", GSF(container_handle_signal), ESF(object_emit_signal),
    GTK_SET_FOCUS_CHILD_SIGNAL},
   {"", NULL, NULL, 0}
};

/* Register container signals table in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_container()
{
   return GTK_TYPE_CONTAINER;
}

long
_clip_type_container()
{
   return GTK_WIDGET_CONTAINER;
}

const char *
_clip_type_name_container()
{
   return "GTK_WIDGET_CONTAINER";
}

CLIP_DLLEXPORT GtkType
_gtk_type_event_box()
{
   return GTK_TYPE_EVENT_BOX;
}

long
_clip_type_event_box()
{
   return GTK_WIDGET_EVENT_BOX;
}

const char *
_clip_type_name_event_box()
{
   return "GTK_WIDGET_EVENT_BOX";
}

int
clip_INIT___CONTAINER(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_container, _clip_type_name_container, _gtk_type_container, _gtk_type_widget, container_signals);
   _wtype_table_put(_clip_type_event_box, _clip_type_name_event_box, _gtk_type_event_box, _gtk_type_container, NULL);
   return 0;
}

/**********************************************************/

/**** Event Box constructor ****/
int
clip_GTK_EVENTBOXNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   GtkWidget *wid = NULL;
   C_widget *cwid;
   CHECKOPT(1, MAP_type_of_ClipVarType);

   wid = gtk_event_box_new();
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
clip_GTK_CONTAINERADD(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKCWID(ccon, GTK_IS_CONTAINER);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_container_add(GTK_CONTAINER(ccon->widget), cwid->widget);
   return 0;
 err:
   return 1;
}

int
clip_GTK_CONTAINERREMOVE(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKCWID(ccon, GTK_IS_CONTAINER);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_container_remove(GTK_CONTAINER(ccon->widget), cwid->widget);
   return 0;
 err:
   return 1;
}

int
clip_GTK_CONTAINERSETRESIZEMODE(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   int mode = _clip_parni(ClipMachineMemory, 2);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ccon, GTK_IS_CONTAINER);
   gtk_container_set_resize_mode(GTK_CONTAINER(ccon->widget), mode);
   return 0;
 err:
   return 1;
}

int
clip_GTK_CONTAINERCHECKRESIZE(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(ccon, GTK_IS_CONTAINER);
   gtk_container_check_resize(GTK_CONTAINER(ccon->widget));
   return 0;
 err:
   return 1;
}

static void
_container_children_func(GtkWidget * wid, gpointer data)
{
   C_var *c = (C_var *) data;
   C_widget *c_wid = _list_get_cwidget(c->ClipMachineMemory, wid);
   if (!c_wid)
      c_wid = _register_widget(c->ClipMachineMemory, wid, NULL);
   if (c_wid)
      _clip_aadd(c->ClipMachineMemory, c->cv, &c_wid->obj);
}

// Returns a list of container children
int
clip_GTK_CONTAINERCHILDREN(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   long n = 0;
   ClipVar *a = RETPTR(ClipMachineMemory);
   C_var c;

   CHECKCWID(ccon, GTK_IS_CONTAINER);

   _clip_array(ClipMachineMemory, a, 1, &n);
   c.ClipMachineMemory = ClipMachineMemory;
   c.cv = a;
   gtk_container_foreach(GTK_CONTAINER(ccon->widget), (GtkCallback) _container_children_func, &c);
   return 0;
 err:
   return 1;
}

static void
_container_for_each_func(GtkWidget * wid, gpointer data)
{
   C_var *c = (C_var *) data;
   C_widget *c_wid = _list_get_cwidget(c->ClipMachineMemory, wid);
   ClipVar stack[2];
   ClipVar res;
   if (!c_wid)
      c_wid = _register_widget(c->ClipMachineMemory, wid, NULL);
   if (c_wid)
      {
	 memset(&stack, 0, sizeof(stack));
	 memset(&res, 0, sizeof(ClipVar));
	 _clip_mclone(c->cw->cmachine, &stack[0], &c->cw->obj);
	//stack[0] = c->cw->obj;
	 stack[1] = c_wid->obj;
	 _clip_eval(c->ClipMachineMemory, &(c->cfunc), 2, stack, &res);
	 _clip_destroy(c->ClipMachineMemory, &res);
      }
}

/* Calls the CLIP function for each children */
int
clip_GTK_CONTAINERFOREACH(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   ClipVar *cfunc = _clip_spar(ClipMachineMemory, 2);
   C_var *c = 0;

   CHECKCWID(ccon, GTK_IS_CONTAINER);
   CHECKARG2(2, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);

   c->ClipMachineMemory = ClipMachineMemory;
   c->cw = ccon;
   _clip_mclone(ClipMachineMemory, &c->cfunc, cfunc);
   gtk_container_foreach(GTK_CONTAINER(ccon->widget), (GtkCallback) _container_for_each_func, c);
   return 0;
 err:
   return 1;
}

int
clip_GTK_CONTAINERFOCUS(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   int focus = _clip_parni(ClipMachineMemory, 2);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ccon, GTK_IS_CONTAINER);
   gtk_container_focus(GTK_CONTAINER(ccon->widget), focus);
   return 0;
 err:
   return 1;
}

int
clip_GTK_CONTAINERSETFOCUSCHILD(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKCWID(ccon, GTK_IS_CONTAINER);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_container_set_focus_child(GTK_CONTAINER(ccon->widget), cwid->widget);
   return 0;
 err:
   return 1;
}

int
clip_GTK_CONTAINERSETFOCUSVADJUSTMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKCWID(ccon, GTK_IS_CONTAINER);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_ADJUSTMENT);
   gtk_container_set_focus_vadjustment(GTK_CONTAINER(ccon->widget), GTK_ADJUSTMENT(cwid->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_CONTAINERSETFOCUSHADJUSTMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKCWID(ccon, GTK_IS_CONTAINER);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_ADJUSTMENT);
   gtk_container_set_focus_hadjustment(GTK_CONTAINER(ccon->widget), GTK_ADJUSTMENT(cwid->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_CONTAINERRESIZECHILDREN(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   if (!ccon || !GTK_IS_CONTAINER(ccon->widget))
      goto err;
   gtk_container_resize_children(GTK_CONTAINER(ccon->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_CONTAINERQUEUERESIZE(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(ccon, GTK_IS_CONTAINER);
   gtk_container_queue_resize(GTK_CONTAINER(ccon->widget));
   return 0;
 err:
   return 1;
}

/* Sets a width of container's border */
int
clip_GTK_CONTAINERSETBORDERWIDTH(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   int width = _clip_parni(ClipMachineMemory, 2);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ccon, GTK_IS_CONTAINER);
   gtk_container_set_border_width(GTK_CONTAINER(ccon->widget), width);
   return 0;
 err:
   return 1;
}

int
clip_GTK_CONTAINERCLEARRESIZEWIDGETS(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(ccon, GTK_IS_CONTAINER);

   gtk_container_clear_resize_widgets(GTK_CONTAINER(ccon->widget));
   return 0;
 err:
   return 1;
}
