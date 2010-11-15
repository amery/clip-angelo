/*
    Copyright (C) 2001  ITK
    Author  : Alexey M. Tkachenko <alexey@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg.h"

#include <gtk/gtk.h>
#include <gtk/gtkprivate.h>
#include <string.h>

#include "ci_clip-gtk.ch"
#include "ci_clip-gtk.h"

/*************************************************************************/

/* Signal handlers */
CLIP_DLLEXPORT gint
widget_signal_handler(GtkWidget * widget, C_signal * cs)
{
   return handle_signals(widget, cs, NULL);
}

static gint
handle_draw_signal(GtkWidget * widget, GdkRectangle * area, C_signal * cs)
{
   return handle_signals(widget, cs, NULL);
}

static gint
handle_size_request_signal(GtkWidget * widget, GtkRequisition * requisition, C_signal * cs)
{
   return handle_signals(widget, cs, NULL);
}

static gint
handle_size_allocate_signal(GtkWidget * widget, GtkAllocation * allocation, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_X, allocation->x);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_Y, allocation->y);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_WIDTH, allocation->width);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_HEIGHT, allocation->height);
   INVOKESIGHANDLER(widget, cs, cv);
}

static gint
handle_state_changed_signal(GtkWidget * widget, GtkStateType state, C_signal * cs)
{
   return handle_signals(widget, cs, NULL);
}

static gint
handle_parent_set_signal(GtkWidget * widget, GtkObject * old_parent, C_signal * cs)
{
   return handle_signals(widget, cs, NULL);
}

static gint
handle_style_set_signal(GtkWidget * widget, GtkStyle * previous_style, C_signal * cs)
{
   return handle_signals(widget, cs, NULL);
}

static gint
handle_add_accelerator_signal(GtkWidget * widget, guint accel_signal_id, GtkAccelGroup * accel_group, guint accel_key, GdkModifierType accel_mods, GtkAccelFlags accel_flags, C_signal * cs)
{
   return handle_signals(widget, cs, NULL);
}

static gint
handle_remove_accelerator_signal(GtkWidget * widget, GtkAccelGroup * accel_group, guint accel_key, GdkModifierType accel_mods, C_signal * cs)
{
   return handle_signals(widget, cs, NULL);
}

static gint
handle_selection_get_signal(GtkWidget * widget, GtkSelectionData * data, guint info, guint time, C_signal * cs)
{
   return handle_signals(widget, cs, NULL);
}

static gint
handle_selection_received_signal(GtkWidget * widget, GtkSelectionData * data, guint time, C_signal * cs)
{
   return handle_signals(widget, cs, NULL);
}

static gint
handle_drag_begin_signal(GtkWidget * widget, GdkDragContext * drag_context, C_signal * cs)
{
   return handle_signals(widget, cs, NULL);
}

static gint
handle_drag_end_signal(GtkWidget * widget, GdkDragContext * drag_context, C_signal * cs)
{
   return handle_signals(widget, cs, NULL);
}

static gint
handle_drag_data_delete_signal(GtkWidget * widget, GdkDragContext * drag_context, C_signal * cs)
{
   return handle_signals(widget, cs, NULL);
}

static gint
handle_drag_leave_signal(GtkWidget * widget, GdkDragContext * drag_context, guint time, C_signal * cs)
{
   return handle_signals(widget, cs, NULL);
}

static gint
handle_drag_motion_signal(GtkWidget * widget, GdkDragContext * drag_context, gint x, gint y, guint time, C_signal * cs)
{
   return handle_signals(widget, cs, NULL);
}

static gint
handle_drag_drop_signal(GtkWidget * widget, GdkDragContext * drag_context, gint x, gint y, guint time, C_signal * cs)
{
   return handle_signals(widget, cs, NULL);
}

static gint
handle_drag_data_get_signal(GtkWidget * widget, GdkDragContext * drag_context, GtkSelectionData * data, guint info, guint time, C_signal * cs)
{
   return handle_signals(widget, cs, NULL);
}

static gint
handle_drag_data_received_signal(GtkWidget * widget, GdkDragContext * drag_context, gint x, gint y, GtkSelectionData * data, guint info, guint time, C_signal * cs)
{
   return handle_signals(widget, cs, NULL);
}

static gint
handle_debug_msg_signal(GtkWidget * widget, gchar * message, C_signal * cs)
{
   return handle_signals(widget, cs, NULL);
}

static gint
handle_focus_in_out_event(GtkWidget * widget, GdkEventFocus * event, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_TYPE, event->type);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_SENDEVENT, event->send_event);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_IN, event->in);
   INVOKESIGHANDLER(widget, cs, cv);
}

/*************************************************************************/

/* Signals table */
static SignalTable widget_signals[] = {
  /* signals */
   {"map", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_MAP_SIGNAL},
   {"unmap", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_UNMAP_SIGNAL},
   {"show", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_SHOW_SIGNAL},
   {"hide", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_HIDE_SIGNAL},
   {"realize", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_REALIZE_SIGNAL},
   {"unrealize", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_UNREALIZE_SIGNAL},
   {"draw", GSF(handle_draw_signal), ESF(object_emit_signal),
    GTK_DRAW_SIGNAL},
   {"draw-focus", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_DRAW_FOCUS_SIGNAL},
   {"draw-default", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_DRAW_DEFAULT_SIGNAL},
   {"size-request", GSF(handle_size_request_signal), ESF(object_emit_signal),
    GTK_SIZE_REQUEST_SIGNAL},
   {"size-allocate", GSF(handle_size_allocate_signal),
    ESF(object_emit_signal), GTK_SIZE_ALLOCATE_SIGNAL},
   {"state-changed", GSF(handle_state_changed_signal),
    ESF(object_emit_signal), GTK_STATE_CHANGED_SIGNAL},
   {"parent-set", GSF(handle_parent_set_signal), ESF(object_emit_signal),
    GTK_PARENT_SET_SIGNAL},
   {"style-set", GSF(handle_style_set_signal), ESF(object_emit_signal),
    GTK_STYLE_SET_SIGNAL},
   {"add-accelerator", GSF(handle_add_accelerator_signal),
    ESF(object_emit_signal), GTK_ADD_ACCELERATOR_SIGNAL},
   {"remove-accelerator", GSF(handle_remove_accelerator_signal),
    ESF(object_emit_signal), GTK_REMOVE_ACCELERATOR_SIGNAL},
   {"grab-focus", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_GRAB_FOCUS_SIGNAL},
   {"focus-in-event", GSF(handle_focus_in_out_event),
    ESF(object_emit_signal), GTK_FOCUS_IN_EVENT},
   {"focus-out-event", GSF(handle_focus_in_out_event),
    ESF(object_emit_signal), GTK_FOCUS_OUT_EVENT},
   {"selection-get", GSF(handle_selection_get_signal),
    ESF(object_emit_signal), GTK_SELECTION_GET_SIGNAL},
   {"selection-received", GSF(handle_selection_received_signal),
    ESF(object_emit_signal), GTK_SELECTION_RECEIVED_SIGNAL},
   {"drag-begin", GSF(handle_drag_begin_signal), ESF(object_emit_signal),
    GTK_DRAG_BEGIN_SIGNAL},
   {"drag-end", GSF(handle_drag_end_signal), ESF(object_emit_signal),
    GTK_DRAG_END_SIGNAL},
   {"drag-data-delete", GSF(handle_drag_data_delete_signal),
    ESF(object_emit_signal), GTK_DRAG_DATA_DELETE_SIGNAL},
   {"drag-leave", GSF(handle_drag_leave_signal), ESF(object_emit_signal),
    GTK_DRAG_LEAVE_SIGNAL},
   {"drag-motion", GSF(handle_drag_motion_signal), ESF(object_emit_signal),
    GTK_DRAG_MOTION_SIGNAL},
   {"drag-drop", GSF(handle_drag_drop_signal), ESF(object_emit_signal),
    GTK_DRAG_DROP_SIGNAL},
   {"drag-data-get", GSF(handle_drag_data_get_signal),
    ESF(object_emit_signal), GTK_DRAG_DATA_GET_SIGNAL},
   {"drag-data-received", GSF(handle_drag_data_received_signal),
    ESF(object_emit_signal), GTK_DRAG_DATA_RECEIVED_SIGNAL},
   {"debug-msg", GSF(handle_debug_msg_signal), ESF(object_emit_signal),
    GTK_DEBUG_MSG_SIGNAL},
  /* events */
   {"event", GSF(handle_events), ESF(object_emit_event), GTK_EVENT},
   {"unknown-event", GSF(handle_events), ESF(object_emit_event),
    GTK_UNKNOWN_EVENT},
   {"enter-notify-event", GSF(handle_events), ESF(object_emit_event),
    GTK_ENTER_NOTIFY},
   {"leave-notify-event", GSF(handle_events), ESF(object_emit_event),
    GTK_LEAVE_NOTIFY},
   {"button-press-event", GSF(handle_events), ESF(object_emit_event),
    GTK_BUTTON_PRESS},
   {"button-release-event", GSF(handle_events), ESF(object_emit_event),
    GTK_BUTTON_RELEASE},
   {"nothing-event", GSF(handle_events), ESF(object_emit_event),
    GTK_NOTHING},
   {"delete-event", GSF(handle_events), ESF(object_emit_event), GTK_DELETE},
   {"destroy-event", GSF(handle_events), ESF(object_emit_event),
    GTK_DESTROY},
   {"expose-event", GSF(handle_events), ESF(object_emit_event), GTK_EXPOSE},
   {"motion-notify-event", GSF(handle_events), ESF(object_emit_event),
    GTK_MOTION_NOTIFY},
   {"2button-press-event", GSF(handle_events), ESF(object_emit_event),
    GTK_2BUTTON_PRESS},
   {"3button-press-event", GSF(handle_events), ESF(object_emit_event),
    GTK_3BUTTON_PRESS},
   {"key-press-event", GSF(handle_events), ESF(object_emit_event),
    GTK_KEY_PRESS},
   {"key-release-event", GSF(handle_events), ESF(object_emit_event),
    GTK_KEY_RELEASE},
   {"focus-change-event", GSF(handle_events), ESF(object_emit_event),
    GTK_FOCUS_CHANGE},
   {"configure-event", GSF(handle_events), ESF(object_emit_event),
    GTK_CONFIGURE},
   {"map-event", GSF(handle_events), ESF(object_emit_event), GTK_MAP},
   {"unmap-event", GSF(handle_events), ESF(object_emit_event), GTK_UNMAP},
   {"property-notify-event", GSF(handle_events), ESF(object_emit_event),
    GTK_PROPERTY_NOTIFY},
   {"selection-clear-event", GSF(handle_events), ESF(object_emit_event),
    GTK_SELECTION_CLEAR},
   {"selection-request-event", GSF(handle_events), ESF(object_emit_event),
    GTK_SELECTION_REQUEST},
   {"selection-notify-event", GSF(handle_events), ESF(object_emit_event),
    GTK_SELECTION_NOTIFY},
   {"proximity-in-event", GSF(handle_events), ESF(object_emit_event),
    GTK_PROXIMITY_IN},
   {"proximity-out-event", GSF(handle_events), ESF(object_emit_event),
    GTK_PROXIMITY_OUT},
   {"drag-enter-event", GSF(handle_events), ESF(object_emit_event),
    GTK_DRAG_ENTER},
   {"drag-leave-event", GSF(handle_events), ESF(object_emit_event),
    GTK_DRAG_LEAVE},
   {"drag-motion-event", GSF(handle_events), ESF(object_emit_event),
    GTK_DRAG_MOTION},
   {"drag-status-event", GSF(handle_events), ESF(object_emit_event),
    GTK_DRAG_STATUS},
   {"drop-start-event", GSF(handle_events), ESF(object_emit_event),
    GTK_DROP_START},
   {"drop-finished-event", GSF(handle_events), ESF(object_emit_event),
    GTK_DROP_FINISHED},
   {"client-event", GSF(handle_events), ESF(object_emit_event),
    GTK_CLIENT_EVENT},
   {"visibility-notify-event", GSF(handle_events), ESF(object_emit_event),
    GTK_VISIBILITY_NOTIFY},
   {"no-expose-event", GSF(handle_events), ESF(object_emit_event),
    GTK_NO_EXPOSE},
   {"", NULL, NULL, 0}
};

/* Register widget signals table in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_widget()
{
   return GTK_TYPE_WIDGET;
}

long
_clip_type_widget()
{
   return GTK_WIDGET_WIDGET;
}

const char *
_clip_type_name_widget()
{
   return "GTK_WIDGET_WIDGET";
}

int
clip_INIT___WIDGET(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_widget, _clip_type_name_widget, _gtk_type_widget, NULL, widget_signals);
   return 0;
}

/**************************************************************/

/* Returns CLIP type of widget */
int
clip_GTK_WIDGETGETTYPE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   C_object *cobj = _fetch_co_arg(ClipMachineMemory);
   WTypeTable *wt_item = NULL;
   long cwtype = GTK_WIDGET_UNKNOWN;
   if (cwid || cobj)
      {
	 wt_item = _wtype_table_get(cwid->type);
	 if (!wt_item)
	    wt_item = _wtype_table_get(cobj->type);
	 if (wt_item && wt_item->fclip_type)
	    cwtype = wt_item->fclip_type();
      }
   _clip_retni(ClipMachineMemory, cwtype);
   return 0;
}

/* Returns CLIP type name of widget */
int
clip_GTK_WIDGETGETTYPENAME(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   C_object *cobj = _fetch_co_arg(ClipMachineMemory);
   WTypeTable *wt_item = NULL;
   const char *cwtype_name = "GTK_WIDGET_UNKNOWN";
   if (cwid || cobj)
      {
	 wt_item = _wtype_table_get(cwid->type);
	 if (!wt_item)
	    wt_item = _wtype_table_get(cobj->type);
	 if (wt_item && wt_item->ftype_name)
	    cwtype_name = (const char *) wt_item->ftype_name();
      }
   _clip_retc(ClipMachineMemory, (char *) cwtype_name);
   return 0;
}

/* Checks if an object is a widget */
int
clip_GTK_ISWIDGET(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   _clip_retl(ClipMachineMemory, cwid && cwid->objtype == GTK_OBJ_WIDGET && cwid->widget && GTK_IS_WIDGET(cwid->widget));
   return 0;
}

/* Sets a size of the widget */
int
clip_GTK_WIDGETSETSIZE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   gint width = _clip_parni(ClipMachineMemory, 2);
   gint height = _clip_parni(ClipMachineMemory, 3);

   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   if (_clip_parinfo(ClipMachineMemory, 2) == NUMERIC_type_of_ClipVarType)
      gtk_widget_set(cwid->widget, "width", width, NULL);
   if (_clip_parinfo(ClipMachineMemory, 3) == NUMERIC_type_of_ClipVarType)
      gtk_widget_set(cwid->widget, "height", height, NULL);
   return 0;
 err:
   return 1;
}

/* Returns a size of the widget */
int
clip_GTK_WIDGETGETSIZE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   GtkArg argw, argh;

   CHECKCWID(cwid, GTK_IS_WIDGET);
   argw.type = GTK_TYPE_INT;
   argh.type = GTK_TYPE_INT;
   argw.name = "width";
   argh.name = "height";
   gtk_widget_get(cwid->widget, &argw);
   gtk_widget_get(cwid->widget, &argh);
  /*
     printf("argw.d.int_data=%d\n", argw.d.int_data);
     printf("argw.h.int_data=%d\n", argh.d.int_data);
   */
  /*
     if (argw.d.int_data == -1) argw.d.int_data = cwid->widget->requisition.width;
     if (argh.d.int_data == -1) argh.d.int_data = cwid->widget->requisition.height;
   */

   argw.d.int_data = cwid->widget->allocation.width;
   argh.d.int_data = cwid->widget->allocation.height;

  /*
     printf("argw.d.int_data=%d\n", argw.d.int_data);
     printf("argw.h.int_data=%d\n", argh.d.int_data);
   */
   _clip_storni(ClipMachineMemory, argw.d.int_data, 2, 0);
   _clip_storni(ClipMachineMemory, argh.d.int_data, 3, 0);
   return 0;
 err:
   return 1;
}

/* Sets a position of the widget */
int
clip_GTK_WIDGETSETPOSITION(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   gint x = _clip_parni(ClipMachineMemory, 2);
   gint y = _clip_parni(ClipMachineMemory, 3);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   if (_clip_parinfo(ClipMachineMemory, 2) == NUMERIC_type_of_ClipVarType)
      gtk_widget_set(cwid->widget, "x", x, NULL);
   if (_clip_parinfo(ClipMachineMemory, 3) == NUMERIC_type_of_ClipVarType)
      gtk_widget_set(cwid->widget, "y", y, NULL);
   return 0;
 err:
   return 1;
}

/* Alena:Sets a uposition of the widget */
int
clip_GTK_WIDGETSETUPOSITION(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   gint x = _clip_parni(ClipMachineMemory, 2);
   gint y = _clip_parni(ClipMachineMemory, 3);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_widget_set_uposition(cwid->widget, x, y);
   return 0;
 err:
   return 1;
}

/* Returns a position of the widget */
int
clip_GTK_WIDGETGETPOSITION(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   GtkArg argx, argy;

   CHECKCWID(cwid, GTK_IS_WIDGET);
   argx.type = GTK_TYPE_INT;
   argy.type = GTK_TYPE_INT;
   argx.name = "x";
   argy.name = "y";
   gtk_widget_get(cwid->widget, &argx);
   gtk_widget_get(cwid->widget, &argy);
   if (argx.d.int_data == -1)
      argx.d.int_data = cwid->widget->allocation.x;
   if (argy.d.int_data == -1)
      argy.d.int_data = cwid->widget->allocation.y;
   _clip_storni(ClipMachineMemory, argx.d.int_data, 2, 0);
   _clip_storni(ClipMachineMemory, argy.d.int_data, 3, 0);
   return 0;
 err:
   return 1;
}

/* Show a widget */
int
clip_GTK_WIDGETSHOW(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_widget_show(cwid->widget);
   return 0;
 err:
   return 1;
}

/* Show a widget and all it`s children */
int
clip_GTK_WIDGETSHOWALL(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_widget_show_all(cwid->widget);
   return 0;
 err:
   return 1;
}

/* Hide a widget */
int
clip_GTK_WIDGETHIDE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_widget_hide(cwid->widget);
   return 0;
 err:
   return 1;
}

int
clip_GTK_WIDGETHIDEONDELETE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_widget_hide_on_delete(cwid->widget);
   return 0;
 err:
   return 1;
}

/* Set focus to a widget */
int
clip_GTK_WIDGETSETFOCUS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   if (GTK_IS_WINDOW(cwid->widget) && cwid->widget->window)
      {
	 gdk_window_raise(cwid->widget->window);
	 _clip_retl(ClipMachineMemory, TRUE);
      }
   else
      {
	 if (GTK_WIDGET_CAN_FOCUS(cwid->widget))
	    {
	       gtk_window_set_focus(GTK_WINDOW(gtk_widget_get_toplevel(cwid->widget)), cwid->widget);
	       _clip_retl(ClipMachineMemory, TRUE);
	    }
	 else
	    _clip_retl(ClipMachineMemory, FALSE);
      }
   return 0;
 err:
   return 1;
}

/* Add accelerator to a widget */
int
clip_GTK_WIDGETADDACCELERATOR(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   int sigid = _clip_parni(ClipMachineMemory, 2);
   char *signame = _clip_parc(ClipMachineMemory, 2);
   C_widget *cwin = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));
   guint accel_key = _clip_parni(ClipMachineMemory, 4);
   guint accel_mods = _clip_parni(ClipMachineMemory, 5);
   GtkAccelFlags accel_flags = _clip_parni(ClipMachineMemory, 6);
   int ret = FALSE;

   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKARG2(2, NUMERIC_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   CHECKARG2(3, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);
   CHECKOPT(5, NUMERIC_type_of_ClipVarType);
   CHECKOPT(6, NUMERIC_type_of_ClipVarType);

   if (cwin && cwin->accel_group)
      {
	 if (_clip_parinfo(ClipMachineMemory, 2) == NUMERIC_type_of_ClipVarType)
	    signame = _sig_name_by_id(sigid);

	 if (signame != NULL)
	    {
	       gtk_widget_add_accelerator(cwid->widget, signame, cwin->accel_group, accel_key, accel_mods, accel_flags);
	       ret = TRUE;
	    }
      }
   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   _clip_retl(ClipMachineMemory, ret);
   return 1;
}

/* Remove accelerator by shortcut */
int
clip_GTK_WIDGETREMOVEACCELERATOR(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwin = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   guint accel_key = _clip_parni(ClipMachineMemory, 3);
   guint accel_mods = _clip_parni(ClipMachineMemory, 4);
   int ret = FALSE;

   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, NUMERIC_type_of_ClipVarType);

   if (cwin && cwin->accel_group)
      {
	 gtk_widget_remove_accelerator(cwid->widget, cwin->accel_group, accel_key, accel_mods);
	 ret = TRUE;
      }
   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   _clip_retl(ClipMachineMemory, ret);
   return 1;
}

/* Remove accelerators by signal */
int
clip_GTK_WIDGETREMOVEACCELERATORS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   int sigid = _clip_parni(ClipMachineMemory, 2);
   char *signame = _clip_parc(ClipMachineMemory, 2);
   gboolean visible_only = _clip_parl(ClipMachineMemory, 3);
   int ret = FALSE;

   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKARG2(2, NUMERIC_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   CHECKARG(3, LOGICAL_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 2) == NUMERIC_type_of_ClipVarType)
      signame = _sig_name_by_id(sigid);

   if (signame != NULL)
      {
	 gtk_widget_remove_accelerators(cwid->widget, signame, visible_only);
	 ret = TRUE;
      }
   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   _clip_retl(ClipMachineMemory, ret);
   return 1;
}

/* Show a widget now */
int
clip_GTK_WIDGETSHOWNOW(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_widget_show_now(cwid->widget);
   return 0;
 err:
   return 1;
}

/* Hide a widget and it`s children */
int
clip_GTK_WIDGETHIDEALL(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_widget_hide_all(cwid->widget);
   return 0;
 err:
   return 1;
}

/* This function would immediately render the region area of a widget,
 * by invoking the virtual draw method of a widget. */
int
clip_GTK_WIDGETDRAW(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   GdkRectangle area;
   CHECKCWID(cwid, GTK_IS_WIDGET);
   area.x = INT_OPTION(ClipMachineMemory, 2, cwid->widget->allocation.x);
   area.y = INT_OPTION(ClipMachineMemory, 3, cwid->widget->allocation.y);
   area.width = INT_OPTION(ClipMachineMemory, 4, cwid->widget->allocation.width);
   area.height = INT_OPTION(ClipMachineMemory, 5, cwid->widget->allocation.height);
   gtk_widget_draw(cwid->widget, &area);
   return 0;
 err:
   return 1;
}

/* Equivalent to calling gtk_widget_draw() for the entire area of a widget. */
int
clip_GTK_WIDGETQUEUEDRAW(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_widget_queue_draw(cwid->widget);
   return 0;
 err:
   return 1;
}

/* Draw a widget, which have focus */
int
clip_GTK_WIDGETDRAWFOCUS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_widget_draw_focus(cwid->widget);
   return 0;
 err:
   return 1;
}

/* Draw a widget, which is a default */
int
clip_GTK_WIDGETDRAWDEFAULT(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   if (!cwid || !cwid->widget)
      goto err;
   gtk_widget_draw_default(cwid->widget);
   return 0;
 err:
   return 1;
}

/* Locks widget's accelerator */
int
clip_GTK_WIDGETLOCKACCELERATORS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_widget_lock_accelerators(cwid->widget);
   return 0;
 err:
   return 1;
}

/* Unlocks widget's accelerator */
int
clip_GTK_WIDGETUNLOCKACCELERATORS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_widget_unlock_accelerators(cwid->widget);
   return 0;
 err:
   return 1;
}

/* Returns: .T. if accelerators locked, .F. elsewhere */
int
clip_GTK_WIDGETACCELERATORSLOCKED(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   _clip_retl(ClipMachineMemory, gtk_widget_accelerators_locked(cwid->widget));
   return 0;
 err:
   return 1;
}

/* Activates a widget */
int
clip_GTK_WIDGETACTIVATE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   _clip_retl(ClipMachineMemory, gtk_widget_activate(cwid->widget));
   return 0;
 err:
   return 1;
}

/* Sets vertical and horizontal adjustments */
int
clip_GTK_WIDGETSETSCROLLADJUSTMENTS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   C_widget *chadj = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   C_widget *cvadj = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));
   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKOPT2(2, NUMERIC_type_of_ClipVarType, MAP_type_of_ClipVarType);
   CHECKCWIDOPT(chadj, GTK_IS_ADJUSTMENT);
   CHECKOPT2(3, NUMERIC_type_of_ClipVarType, MAP_type_of_ClipVarType);
   CHECKCWIDOPT(cvadj, GTK_IS_ADJUSTMENT);
   _clip_retl(ClipMachineMemory, gtk_widget_set_scroll_adjustments(cwid->widget, GTK_ADJUSTMENT(chadj->widget), GTK_ADJUSTMENT(cvadj->widget)));
   return 0;
 err:
   return 1;
}

// Disables signals handler call
int
clip_GTK_WIDGETDISABLESIGNALS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   if (!cwid)
      goto err;
   cwid->sigenabled = FALSE;
   return 0;
 err:
   return 1;
}

// Enables signals handler call
int
clip_GTK_WIDGETENABLESIGNALS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   if (!cwid)
      goto err;
   cwid->sigenabled = TRUE;
   return 0;
 err:
   return 1;
}

// Disables users signals handler call
int
clip_GTK_WIDGETDISABLEUSERSSIGNALS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   if (!cwid)
      goto err;
   cwid->usersigenabled = FALSE;
   return 0;
 err:
   return 1;
}

// Enables users signals handler call
int
clip_GTK_WIDGETENABLEUSERSSIGNALS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   if (!cwid)
      goto err;
   cwid->usersigenabled = TRUE;
   return 0;
 err:
   return 1;
}

// Disables events handler call
int
clip_GTK_WIDGETDISABLEEVENTS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   if (!cwid)
      goto err;
   cwid->evntenabled = FALSE;
   return 0;
 err:
   return 1;
}

// Enables events handler call
int
clip_GTK_WIDGETENABLEEVENTS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   if (!cwid)
      goto err;
   cwid->evntenabled = TRUE;
   return 0;
 err:
   return 1;
}

/*  */
int
clip_GTK_WIDGETPOPUP(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   gint x = _clip_parni(ClipMachineMemory, 2);
   gint y = _clip_parni(ClipMachineMemory, 3);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_widget_popup(cwid->widget, x, y);
   return 0;
 err:
   return 1;
}

/***** gtk_widget_intersect ******/

/* Grab a focus */
int
clip_GTK_WIDGETGRABFOCUS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_widget_grab_focus(cwid->widget);
   return 0;
 err:
   return 1;
}

/* Default widget grabs a focus */
int
clip_GTK_WIDGETGRABDEFAULT(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_widget_grab_default(cwid->widget);
   return 0;
 err:
   return 1;
}

/* Sets a name for a widget */
int
clip_GTK_WIDGETSETNAME(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   gchar *name = _clip_parc(ClipMachineMemory, 2);
   gchar emptyname[] = "\0";
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   if (name)
      gtk_widget_set_name(cwid->widget, name);
   else
      gtk_widget_set_name(cwid->widget, emptyname);
   return 0;
 err:
   return 1;
}

/* Gets a name for a widget */
int
clip_GTK_WIDGETGETNAME(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   _clip_retc(ClipMachineMemory, gtk_widget_get_name(cwid->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_WIDGETUNPARENT(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_widget_unparent(cwid->widget);
   return 0;
 err:
   return 1;
}

int
clip_GTK_WIDGETREPARENT(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cpar = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_widget_reparent(cwid->widget, cpar->widget);
   return 0;
 err:
   return 1;
}

int
clip_GTK_WIDGETSETPARENT(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cpar = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_widget_set_parent(cwid->widget, cpar->widget);
   return 0;
 err:
   return 1;
}

/* Sets a state of a widget */
/*
This type indicates the current state of a widget.

 GTK_STATE_NORMAL       The state during normal operation.
 GTK_STATE_ACTIVE       The widget is currently active, such as a
 GTK_STATE_PRELIGHT     The mouse pointer is over the widget.
 GTK_STATE_SELECTED
 GTK_STATE_INSENSITIVE  The state of the widget can not be altered by the user.
			Its appearance will usually indicate this.
*/
int
clip_GTK_WIDGETSETSTATE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   gint state = _clip_parni(ClipMachineMemory, 2);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_widget_set_state(cwid->widget, state);
   return 0;
 err:
   return 1;
}

int
clip_GTK_WIDGETGETSTATE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   _clip_retni(ClipMachineMemory, GTK_WIDGET_STATE(cwid->widget));

   return 0;
 err:
   return 1;
}

int
clip_GTK_WIDGETGETSAVEDSTATE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   _clip_retni(ClipMachineMemory, GTK_WIDGET_SAVED_STATE(cwid->widget));

   return 0;
 err:
   return 1;
}

/* Sets a sensitivity of a widget */
int
clip_GTK_WIDGETSETSENSITIVE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   gboolean sensitive = _clip_parl(ClipMachineMemory, 2);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_widget_set_sensitive(cwid->widget, sensitive);
   return 0;
 err:
   return 1;
}

int
clip_GTK_WIDGETSETAPPPAINTABLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   gboolean paintable = _clip_parl(ClipMachineMemory, 2);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_widget_set_app_paintable(cwid->widget, paintable);
   return 0;
 err:
   return 1;
}

/* Set style of a widget */
int
clip_GTK_WIDGETSETSTYLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   ClipVar *mstyle = _clip_par(ClipMachineMemory, 2);
   GtkStyle *style;
   int i;

   CHECKARG(2, MAP_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);

   style = gtk_style_copy(cwid->widget->style);
  //style = cwid->widget->style;

  /*alena */

   style->white_gc = cwid->widget->style->white_gc;
   style->black_gc = cwid->widget->style->black_gc;
   for (i = 0; i < 5; i++)
      {
	 style->fg_gc[i] = cwid->widget->style->fg_gc[i];
	 style->bg_gc[i] = cwid->widget->style->bg_gc[i];
	 style->light_gc[i] = cwid->widget->style->light_gc[i];
	 style->dark_gc[i] = cwid->widget->style->dark_gc[i];
	 style->mid_gc[i] = cwid->widget->style->mid_gc[i];
	 style->text_gc[i] = cwid->widget->style->text_gc[i];
	 style->base_gc[i] = cwid->widget->style->base_gc[i];
      }

	/*******/
  //style = cwid->widget->style;
  //gtk_style_unref(cwid->widget->style);
  //gtk_style_ref(style);

  ////style = gtk_widget_get_style(cwid->widget);
   _map_to_style(ClipMachineMemory, mstyle, style);
  // Apply new style to a widget
   gtk_widget_set_style(cwid->widget, style);
   return 0;
 err:
   return 1;
}

/* Get style of a widget */
int
clip_GTK_WIDGETGETSTYLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   ClipVar *mstyle = RETPTR(ClipMachineMemory);
   GtkStyle *style;
   int i;

   CHECKCWID(cwid, GTK_IS_WIDGET);

  //style = gtk_style_copy (cwid->widget->style);
   style = cwid->widget->style;

  /////////alena*

   style->white_gc = cwid->widget->style->white_gc;
   style->black_gc = cwid->widget->style->black_gc;
   for (i = 0; i < 5; i++)
      {
	 style->fg_gc[i] = cwid->widget->style->fg_gc[i];
	 style->bg_gc[i] = cwid->widget->style->bg_gc[i];
	 style->light_gc[i] = cwid->widget->style->light_gc[i];
	 style->dark_gc[i] = cwid->widget->style->dark_gc[i];
	 style->mid_gc[i] = cwid->widget->style->mid_gc[i];
	 style->text_gc[i] = cwid->widget->style->text_gc[i];
	 style->base_gc[i] = cwid->widget->style->base_gc[i];
      }

  ////////////////\*******

  //style = gtk_style_copy(gtk_widget_get_style(cwid->widget));
   if (style)
      {
	//gtk_style_unref (cwid->widget->style);
	//gtk_style_ref (style);
	 memset(mstyle, 0, sizeof(*mstyle));
	 _clip_map(ClipMachineMemory, mstyle);
	 _style_to_map(ClipMachineMemory, style, mstyle);
	//gtk_style_unref(style);
      }
   return 0;
 err:
   return 1;
}

/* Restores default style for a widget */
int
clip_GTK_WIDGETRESTOREDEFAULTSTYLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_widget_restore_default_style(cwid->widget);
   return 0;
 err:
   return 1;
}

/* Set default style of a widget */
int
clip_GTK_WIDGETSETDEFAULTSTYLE(ClipMachine * ClipMachineMemory)
{
   ClipVar *mstyle = _clip_spar(ClipMachineMemory, 1);
   GtkStyle *style;
   CHECKARG(1, MAP_type_of_ClipVarType);

  //style = gtk_style_new();
   style = gtk_widget_get_default_style();

   _map_to_style(ClipMachineMemory, mstyle, style);

  // Apply new style to a widget
   gtk_widget_set_default_style(style);
   return 0;
 err:
   return 1;
}

/* Get default style of a widget */
int
clip_GTK_WIDGETGETDEFAULTSTYLE(ClipMachine * ClipMachineMemory)
{
   ClipVar *mstyle = RETPTR(ClipMachineMemory);
   GtkStyle *style;

   style = gtk_widget_get_default_style();

   memset(mstyle, 0, sizeof(*mstyle));
   _clip_map(ClipMachineMemory, mstyle);
   _style_to_map(ClipMachineMemory, style, mstyle);
   return 0;
}

/* Destroy widget and it`s shadow object in CLIP machine */
int
clip_GTK_WIDGETDESTROY(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   if (cwid)
      {
	 if (cwid->widget)
	    gtk_widget_destroy(cwid->widget);
	 else
	    destroy_c_widget(cwid);
      }
/*
	if (cwid)
	{
		if (cwid->destroy)
			cwid->destroy(ClipMachineMemory, cwid);
		if (cwid->widget)
			gtk_widget_destroy(cwid->widget);
		destroy_c_widget(cwid);
		free(cwid);
	}
*/

   return 0;
}

/*  */
int
clip_GTK_WIDGETGETTOPLEVEL(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   GtkWidget *toplevel;
   C_widget *ctoplevel;
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   toplevel = gtk_widget_get_toplevel(cwid->widget);
   if (toplevel)
      {
	 ctoplevel = _list_get_cwidget(ClipMachineMemory, toplevel);
	 if (!ctoplevel)
	    ctoplevel = _register_widget(ClipMachineMemory, toplevel, NULL);
	 if (ctoplevel)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &ctoplevel->obj);
      }
   return 0;
 err:
   return 1;
}

/* Returns .T. if widget has a focus */
int
clip_GTK_WIDGETHASFOCUS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   _clip_retl(ClipMachineMemory, GTK_WIDGET_HAS_FOCUS(cwid->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_WIDGETFLAGS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   _clip_retnl(ClipMachineMemory, GTK_WIDGET_FLAGS(cwid->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_WIDGETPRIVATEFLAGS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   _clip_retnl(ClipMachineMemory, GTK_PRIVATE_FLAGS(cwid->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_WIDGETSETFLAGS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   long flags = _clip_parnl(ClipMachineMemory, 2);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   GTK_WIDGET_SET_FLAGS(cwid->widget, flags);
   return 0;
 err:
   return 1;
}

int
clip_GTK_WIDGETUNSETFLAGS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   long flags = _clip_parnl(ClipMachineMemory, 2);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   GTK_WIDGET_SET_FLAGS(cwid->widget, flags);
   return 0;
 err:
   return 1;
}

int
clip_GTK_WIDGETMAP(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_widget_map(cwid->widget);
   return 0;
 err:
   return 1;
}

int
clip_GTK_WIDGETUNMAP(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_widget_unmap(cwid->widget);
   return 0;
 err:
   return 1;
}

int
clip_GTK_WIDGETREALIZE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_widget_realize(cwid->widget);
   return 0;
 err:
   return 1;
}

int
clip_GTK_WIDGETUNREALIZE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_widget_unrealize(cwid->widget);
   return 0;
 err:
   return 1;
}

/* Sets the event mask (see GdkEventMask) for a widget. The event mask
   determines which events a widget will receive. Keep in mind that different
   widgets have different default event masks, and by changing the event mask
   you may disrupt a widget's functionality, so be careful. This function must
   be called while a widget is unrealized. Consider gtk_widget_add_events()
   for widgets that are already realized, or if you want to preserve the existing
   event mask. This function can't be used with GTK_NO_WINDOW widgets; to get
   events on those widgets, place them inside a GtkEventBox and receive events
   on the event box. */
int
clip_GTK_WIDGETSETEVENTS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   gint mask = _clip_parni(ClipMachineMemory, 2);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_widget_set_events(cwid->widget, mask);
   return 0;
 err:
   return 1;
}

/* Adds the events in the bitfield events to the event mask for widget.
   See gtk_widget_set_events() for details. */
int
clip_GTK_WIDGETADDEVENTS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   gint mask = _clip_parni(ClipMachineMemory, 2);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_widget_add_events(cwid->widget, mask);
   return 0;
 err:
   return 1;
}

int
clip_GTK_WIDGETGETCOLORMAP(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   GdkColormap *colormap;
   C_object *ccmap;
   CHECKCWID(cwid, GTK_IS_WIDGET);
   colormap = gtk_widget_get_colormap(cwid->widget);

   if (colormap)
      {
	 ccmap = _register_object(ClipMachineMemory, colormap, GDK_TYPE_COLORMAP, NULL, (coDestructor) gdk_object_colormap_destructor);
	 if (ccmap)
	    {
	       ccmap->ref_count = 1;
	      //ccmap->ref_count ++;
	      //gdk_colormap_ref(colormap);
	       _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &ccmap->obj);
	    }
	 else
	    gdk_colormap_unref(colormap);
      }

   return 0;
 err:
   return 1;
}

/* Alena: */
int
clip_GTK_WIDGETGETPARENTWINDOW(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   GdkWindow *win;
   C_object *cwin;
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   win = gtk_widget_get_parent_window(cwid->widget);
   if (win)
      {
	 cwin = _get_cobject(ClipMachineMemory, win, GDK_TYPE_WINDOW, (coDestructor) gdk_object_window_destructor);
	 _clip_retni(ClipMachineMemory, cwin->handle);
      }
   return 0;
 err:
   return 1;
}

int
clip_GTK_WIDGETGETPARENT(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cparent;
   GtkWidget *parent;

   CHECKCWID(cwid, GTK_IS_WIDGET);

   parent = GTK_WIDGET(GTK_WIDGET(cwid->widget)->parent);
   if (parent)
      {
	 cparent = _list_get_cwidget(ClipMachineMemory, parent);
	 if (!cparent)
	    cparent = _register_widget(ClipMachineMemory, parent, NULL);
	 if (cparent)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cparent->obj);
      }
   return 0;
 err:
   return 1;
}

int
clip_GTK_WIDGETQUEUERESIZE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cwid, GTK_IS_WIDGET);

   gtk_widget_queue_resize(GTK_WIDGET(cwid->widget));
   return 0;
 err:
   return 1;
}
