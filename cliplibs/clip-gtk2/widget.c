/*
    Copyright (C) 2001 - 2004  ITK
    Author  : Alexey M. Tkachenko <alexey@itk.ru>
	      Elena V. Kornilova  <alena@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include <string.h>
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg2.h"

#include <gtk/gtk.h>
#include <gtk/gtkprivate.h>

#include "ci_clip-gtk2.ch"
#include "ci_clip-gtk2.h"

/*************************************************************************/

/* Signal handlers */
CLIP_DLLEXPORT gint
widget_signal_handler(GtkWidget * widget, C_signal * cs)
{
   return handle_signals(widget, cs, NULL);
}

static    gint
handle_draw_signal(GtkWidget * widget, GdkRectangle * area, C_signal * cs)
{
   return handle_signals(widget, cs, NULL);
}

static    gint
handle_size_request_signal(GtkWidget * widget, GtkRequisition * requisition, C_signal * cs)
{
   return handle_signals(widget, cs, NULL);
}

static    gint
handle_size_allocate_signal(GtkWidget * widget, GtkAllocation * allocation, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_X, allocation->x);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_Y, allocation->y);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_WIDTH, allocation->width);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_HEIGHT, allocation->height);
   INVOKESIGHANDLER(widget, cs, cv);
}

static    gint
handle_state_changed_signal(GtkWidget * widget, GtkStateType state, C_signal * cs)
{
   return handle_signals(widget, cs, NULL);
}

static    gint
handle_parent_set_signal(GtkWidget * widget, GtkObject * old_parent, C_signal * cs)
{
   return handle_signals(widget, cs, NULL);
}

static    gint
handle_style_set_signal(GtkWidget * widget, GtkStyle * previous_style, C_signal * cs)
{
   return handle_signals(widget, cs, NULL);
}

static    gint
handle_add_accelerator_signal(GtkWidget * widget, guint accel_signal_id,
			      GtkAccelGroup * accel_group, guint accel_key,
			      GdkModifierType accel_mods, GtkAccelFlags accel_flags, C_signal * cs)
{
   return handle_signals(widget, cs, NULL);
}

static    gint
handle_remove_accelerator_signal(GtkWidget * widget,
				 GtkAccelGroup * accel_group, guint accel_key, GdkModifierType accel_mods, C_signal * cs)
{
   return handle_signals(widget, cs, NULL);
}

static    gint
handle_selection_get_signal(GtkWidget * widget, GtkSelectionData * data, guint info, guint time, C_signal * cs)
{
   return handle_signals(widget, cs, NULL);
}

static    gint
handle_selection_received_signal(GtkWidget * widget, GtkSelectionData * data, guint time, C_signal * cs)
{
   return handle_signals(widget, cs, NULL);
}

static    gint
handle_drag_begin_signal(GtkWidget * widget, GdkDragContext * drag_context, C_signal * cs)
{
   return handle_signals(widget, cs, NULL);
}

static    gint
handle_drag_end_signal(GtkWidget * widget, GdkDragContext * drag_context, C_signal * cs)
{
   C_object *cdrag;

   PREPARECV(cs, cv);

   cdrag = _list_get_cobject(cs->cw->cmachine, drag_context);
   if (!cdrag)
      cdrag = _register_object(cs->cw->cmachine, drag_context, GDK_TYPE_DRAG_CONTEXT, NULL, NULL);
   _clip_madd(cs->cw->cmachine, &cv, HASH_DRAGCONTEXT, &cdrag->obj);
   INVOKESIGHANDLER(widget, cs, cv);
}

static    gint
handle_drag_data_delete_signal(GtkWidget * widget, GdkDragContext * drag_context, C_signal * cs)
{
   C_object *cdrag;

   PREPARECV(cs, cv);

   cdrag = _list_get_cobject(cs->cw->cmachine, drag_context);
   if (!cdrag)
      cdrag = _register_object(cs->cw->cmachine, drag_context, GDK_TYPE_DRAG_CONTEXT, NULL, NULL);
   _clip_madd(cs->cw->cmachine, &cv, HASH_DRAGCONTEXT, &cdrag->obj);
   INVOKESIGHANDLER(widget, cs, cv);
}

static    gint
handle_drag_leave_signal(GtkWidget * widget, GdkDragContext * drag_context, guint time, C_signal * cs)
{
   C_object *cdrag;

   PREPARECV(cs, cv);

   cdrag = _list_get_cobject(cs->cw->cmachine, drag_context);
   if (!cdrag)
      cdrag = _register_object(cs->cw->cmachine, drag_context, GDK_TYPE_DRAG_CONTEXT, NULL, NULL);
   _clip_madd(cs->cw->cmachine, &cv, HASH_DRAGCONTEXT, &cdrag->obj);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_TIME, time);
   INVOKESIGHANDLER(widget, cs, cv);
}

static    gint
handle_drag_motion_signal(GtkWidget * widget, GdkDragContext * drag_context, gint x, gint y, guint time, C_signal * cs)
{
   C_object *cdrag;

   PREPARECV(cs, cv);

   cdrag = _list_get_cobject(cs->cw->cmachine, drag_context);
   if (!cdrag)
      cdrag = _register_object(cs->cw->cmachine, drag_context, GDK_TYPE_DRAG_CONTEXT, NULL, NULL);
   _clip_madd(cs->cw->cmachine, &cv, HASH_DRAGCONTEXT, &cdrag->obj);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_X, x);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_Y, y);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_TIME, time);
   _clip_mputn(cs->cw->cmachine, &cdrag->obj, HASH_SUGGESTED_ACTION, drag_context->suggested_action);
   _clip_mputn(cs->cw->cmachine, &cdrag->obj, HASH_ACTION, drag_context->action);
   _clip_mputn(cs->cw->cmachine, &cdrag->obj, HASH_ACTIONS, drag_context->actions);
   INVOKESIGHANDLER(widget, cs, cv);
}

static    gint
handle_drag_drop_signal(GtkWidget * widget, GdkDragContext * drag_context, gint x, gint y, guint time, C_signal * cs)
{
   C_object *cdrag;

   C_object *ctargets;

   PREPARECV(cs, cv);

   cdrag = _list_get_cobject(cs->cw->cmachine, drag_context);
   if (!cdrag)
      cdrag = _register_object(cs->cw->cmachine, drag_context, GDK_TYPE_DRAG_CONTEXT, NULL, NULL);
   _clip_madd(cs->cw->cmachine, &cv, HASH_DRAGCONTEXT, &cdrag->obj);
   if (drag_context->targets)
    {
       ctargets = _list_get_cobject(cs->cw->cmachine, drag_context->targets);
       if (!ctargets)
	  ctargets = _register_object(cs->cw->cmachine, drag_context->targets, GDK_TYPE_ATOM, NULL, NULL);
//                      ctargets = _register_object(cs->cw->cmachine, drag_context->targets->data, GDK_TYPE_ATOM, NULL, NULL);
       _clip_madd(cs->cw->cmachine, &cdrag->obj, HASH_TARGETS, &ctargets->obj);
    }
   else
      _clip_madd(cs->cw->cmachine, &cv, HASH_TARGETS, NULL);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_X, x);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_Y, y);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_TIME, time);
   INVOKESIGHANDLER(widget, cs, cv);
}

static    gint
handle_drag_data_get_signal(GtkWidget * widget,
			    GdkDragContext * drag_context, GtkSelectionData * data, guint info, guint time, C_signal * cs)
{
   C_object *cdrag;

   C_object *csel;

   C_object *ctarg;

   C_object *ctype;

   C_object *cdata;

   ClipVar   seld;

   PREPARECV(cs, cv);

   cdrag = _list_get_cobject(cs->cw->cmachine, drag_context);
   if (!cdrag)
      cdrag = _register_object(cs->cw->cmachine, drag_context, GDK_TYPE_DRAG_CONTEXT, NULL, NULL);
   csel = _list_get_cobject(cs->cw->cmachine, data->selection);
   if (!csel)
      csel = _register_object(cs->cw->cmachine, data->selection, GDK_TYPE_ATOM, NULL, NULL);
   ctarg = _list_get_cobject(cs->cw->cmachine, data->target);
   if (!ctarg)
      ctarg = _register_object(cs->cw->cmachine, data->target, GDK_TYPE_ATOM, NULL, NULL);
   ctype = _list_get_cobject(cs->cw->cmachine, data->type);
   if (!ctype)
      ctype = _register_object(cs->cw->cmachine, data->type, GDK_TYPE_ATOM, NULL, NULL);
   memset(&seld, 0, sizeof(ClipVar));
  //_clip_map(cs->cw->cmachine, &seld);
   cdata = _list_get_cobject(cs->cw->cmachine, data);
   if (!cdata)
      cdata = _register_object(cs->cw->cmachine, data, GTK_TYPE_SELECTION_DATA, NULL, NULL);
   _clip_mclone(cs->cw->cmachine, &seld, &cdata->obj);
   _clip_mputn(cs->cw->cmachine, &cdata->obj, HASH_LENGTH, data->length);
   _clip_mputn(cs->cw->cmachine, &cdata->obj, HASH_FORMAT, data->format);
   _clip_mputc(cs->cw->cmachine, &cdata->obj, HASH_DATA, (char *) data->data,
	       data->data ? strlen((const char *) data->data) : 0);
   _clip_madd(cs->cw->cmachine, &cdata->obj, HASH_SELECTION, &csel->obj);
   _clip_madd(cs->cw->cmachine, &cdata->obj, HASH_TARGET, &ctarg->obj);
   _clip_madd(cs->cw->cmachine, &cdata->obj, HASH_TYPE, &ctype->obj);

   _clip_madd(cs->cw->cmachine, &cv, HASH_DRAGCONTEXT, &cdrag->obj);
   _clip_madd(cs->cw->cmachine, &cv, HASH_SELECTIONDATA, &seld);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_INFO, info);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_TIME, time);
   INVOKESIGHANDLER(widget, cs, cv);
   _clip_destroy(cs->cw->cmachine, &seld);
}

static    gint
handle_drag_data_received_signal(GtkWidget * widget,
				 GdkDragContext * drag_context, gint x,
				 gint y, GtkSelectionData * data, guint info, guint time, C_signal * cs)
{
   C_object *cdrag;

   C_object *csel;

   C_object *ctarg;

   C_object *ctype;

   C_object *cdata;

   ClipVar   seld;

   PREPARECV(cs, cv);

   cdrag = _list_get_cobject(cs->cw->cmachine, drag_context);
   if (!cdrag)
      cdrag = _register_object(cs->cw->cmachine, drag_context, GDK_TYPE_DRAG_CONTEXT, NULL, NULL);
   csel = _list_get_cobject(cs->cw->cmachine, data->selection);
   if (!csel)
      csel = _register_object(cs->cw->cmachine, data->selection, GDK_TYPE_ATOM, NULL, NULL);
   ctarg = _list_get_cobject(cs->cw->cmachine, data->target);
   if (!ctarg)
      ctarg = _register_object(cs->cw->cmachine, data->target, GDK_TYPE_ATOM, NULL, NULL);
   ctype = _list_get_cobject(cs->cw->cmachine, data->type);
   if (!ctype)
      ctype = _register_object(cs->cw->cmachine, data->type, GDK_TYPE_ATOM, NULL, NULL);
   memset(&seld, 0, sizeof(ClipVar));
//      _clip_map(cs->cw->cmachine, &seld);
   cdata = _list_get_cobject(cs->cw->cmachine, data);
   if (!cdata)
      cdata = _register_object(cs->cw->cmachine, data, GTK_TYPE_SELECTION_DATA, &seld, NULL);
   _clip_mclone(cs->cw->cmachine, &seld, &cdata->obj);
   _clip_mputn(cs->cw->cmachine, &cdata->obj, HASH_LENGTH, data->length);
   _clip_mputn(cs->cw->cmachine, &cdata->obj, HASH_FORMAT, data->format);
   _clip_mputc(cs->cw->cmachine, &cdata->obj, HASH_DATA, (char *) data->data,
	       data->data ? strlen((const char *) data->data) : 0);
   _clip_madd(cs->cw->cmachine, &cdata->obj, HASH_SELECTION, &csel->obj);
   _clip_madd(cs->cw->cmachine, &cdata->obj, HASH_TARGET, &ctarg->obj);
   _clip_madd(cs->cw->cmachine, &cdata->obj, HASH_TYPE, &ctype->obj);

   _clip_madd(cs->cw->cmachine, &cv, HASH_DRAGCONTEXT, &cdrag->obj);
   _clip_madd(cs->cw->cmachine, &cv, HASH_SELECTIONDATA, &seld);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_X, x);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_Y, y);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_INFO, info);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_TIME, time);
   INVOKESIGHANDLER(widget, cs, cv);
   _clip_destroy(cs->cw->cmachine, &seld);
}

static    gint
handle_debug_msg_signal(GtkWidget * widget, gchar * message, C_signal * cs)
{
   return handle_signals(widget, cs, NULL);
}

static    gint
handle_focus_in_out_event(GtkWidget * widget, GdkEventFocus * event, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_TYPE, event->type);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_SENDEVENT, event->send_event);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_IN, event->in);
   INVOKESIGHANDLER(widget, cs, cv);
}

static    gint
widget_focus_signal(GtkWidget * widget, GtkDirectionType arg1, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_DIRECTIONTYPE, arg1);
   INVOKESIGHANDLER(widget, cs, cv);
}

static    gint
widget_grab_notify_signal(GtkWidget * widget, gboolean arg1, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_GRAB, arg1);
   INVOKESIGHANDLER(widget, cs, cv);
}

static    gint
widget_direction_changed(GtkWidget * widget, GtkTextDirection arg1, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_TEXTDIRECTION, arg1);
   INVOKESIGHANDLER(widget, cs, cv);
}

static    gint
widget_hierarchy_changed(GtkWidget * widget1, GtkWidget * widget, C_signal * cs)
{
   C_widget *cwid;

   PREPARECV(cs, cv);
   cwid = _list_get_cwidget(cs->cw->cmachine, widget);
   if (!cwid)
      cwid = _register_widget(cs->cw->cmachine, widget, NULL);
   if (cwid)
      _clip_madd(cs->cw->cmachine, &cv, HASH_WIDGET, &cwid->obj);
   INVOKESIGHANDLER(widget, cs, cv);
}

static    gint
widget_mnemonic_activate(GtkWidget * widget, gboolean arg1, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_MNEMONICACTIVATE, arg1);
   INVOKESIGHANDLER(widget, cs, cv);
}

static    gint
widget_show_help(GtkWidget * widget, GtkWidgetHelpType arg1, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_HELPTYPE, arg1);
   INVOKESIGHANDLER(widget, cs, cv);
}

static    gint
handle_motion_notify_events(GtkWidget * widget, GdkEventMotion * event, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_X, event->x);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_Y, event->y);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_X_ROOT, event->x);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_Y_ROOT, event->y);
   INVOKESIGHANDLER(widget, cs, cv);
}

/*************************************************************************/

/* Signals table */
static SignalTable widget_signals[] = {
  /* signals */
   {"accel-closures-changed", GSF(widget_signal_handler),
    ESF(object_emit_signal), GTK_ACCEL_CLOSURES_CHANGED_SIGNAL},
   {"direction-changed", GSF(widget_direction_changed),
    ESF(object_emit_signal), GTK_DIRECTION_CHANGED_SIGNAL},
   {"hierarchy-changed", GSF(widget_hierarchy_changed),
    ESF(object_emit_signal), GTK_HIERARCHY_CHANGED_SIGNAL},
   {"mnemonic-activate", GSF(widget_mnemonic_activate),
    ESF(object_emit_signal), GTK_MNEMONIC_ACTIVATE_SIGNAL},
   {"map", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_MAP_SIGNAL},
   {"unmap", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_UNMAP_SIGNAL},
   {"show", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_SHOW_SIGNAL},
   {"show-help", GSF(widget_show_help), ESF(object_emit_signal),
    GTK_SHOW_HELP_SIGNAL},
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
   {"popup-menu", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_POPUP_MENU_SIGNAL},
   {"focus", GSF(widget_focus_signal), ESF(object_emit_signal),
    GTK_FOCUS_SIGNAL},
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
   {"grab-notify", GSF(widget_grab_notify_signal), ESF(object_emit_signal),
    GTK_GRAB_NOTIFY_SIGNAL},
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
   {"event-after", GSF(handle_events), ESF(object_emit_event),
    GTK_EVENT_AFTER},
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
   {"scroll-event", GSF(handle_events), ESF(object_emit_event),
    GTK_SCROLL_EVENT},
   {"expose-event", GSF(handle_events), ESF(object_emit_event), GTK_EXPOSE},
   {"motion-notify-event", GSF(handle_motion_notify_events),
    ESF(object_emit_event), GTK_MOTION_NOTIFY},
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
  //       {"drag-enter-event",    GSF( handle_events ), ESF( object_emit_event ), GTK_DRAG_ENTER },
  //       {"drag-leave-event",    GSF( handle_events ), ESF( object_emit_event ), GTK_DRAG_LEAVE },
  //       {"drag-motion-event",   GSF( handle_events ), ESF( object_emit_event ), GTK_DRAG_MOTION },
  //       {"drag-status-event",   GSF( handle_events ), ESF( object_emit_event ), GTK_DRAG_STATUS },
  //       {"drop-start-event",    GSF( handle_events ), ESF( object_emit_event ), GTK_DROP_START },
  //       {"drop-finished-event", GSF( handle_events ), ESF( object_emit_event ), GTK_DROP_FINISHED },
   {"client-event", GSF(handle_events), ESF(object_emit_event),
    GTK_CLIENT_EVENT},
   {"visibility-notify-event", GSF(handle_events), ESF(object_emit_event),
    GTK_VISIBILITY_NOTIFY},
   {"window-state-event", GSF(handle_events), ESF(object_emit_event),
    GTK_WINDOW_STATE_EVENT},
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

   long      cwtype = GTK_WIDGET_UNKNOWN;

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

/* Sets a size of the widget */
int
clip_GTK_WIDGETSETSIZE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   gint      width = _clip_parni(ClipMachineMemory, 2);

   gint      height = _clip_parni(ClipMachineMemory, 3);

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

/* Checks if an object is a widget */
int
clip_GTK_ISWIDGET(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   _clip_retl(ClipMachineMemory, cwid && cwid->objtype == GTK_OBJ_WIDGET && cwid->widget && GTK_IS_WIDGET(cwid->widget));
   return 0;
}

/* Returns a size of the widget */
int
clip_GTK_WIDGETGETSIZE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cwid, GTK_IS_WIDGET);

   _clip_storni(ClipMachineMemory, GTK_WIDGET(cwid->widget)->allocation.width, 2, 0);
   _clip_storni(ClipMachineMemory, GTK_WIDGET(cwid->widget)->allocation.height, 3, 0);
   return 0;
 err:
   return 1;
}

int
clip_GTK_WIDGETGETSETTINGS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   GtkSettings *settings;

   C_object *csettings;

   CHECKCWID(cwid, GTK_IS_WIDGET);

   settings = gtk_widget_get_settings(GTK_WIDGET(cwid->widget));

   if (settings)
    {
       csettings = _list_get_cobject(ClipMachineMemory, settings);
       if (!csettings)
	  csettings = _register_object(ClipMachineMemory, settings, GTK_TYPE_SETTINGS, NULL, NULL);
       if (csettings)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &csettings->obj);
    }

   return 0;
 err:
   return 1;
}

int
clip_GTK_WIDGETGETSIZEREQUEST(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   gint      width;

   gint      height;

   CHECKCWID(cwid, GTK_IS_WIDGET);

   gtk_widget_get_size_request(GTK_WIDGET(cwid->widget), &width, &height);
   _clip_storni(ClipMachineMemory, width, 2, 0);
   _clip_storni(ClipMachineMemory, height, 3, 0);
   return 0;
 err:
   return 1;
}

/* Returns a position of the widget */
int
clip_GTK_WIDGETGETPOSITION(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cwid, GTK_IS_WIDGET);
   _clip_storni(ClipMachineMemory, GTK_WIDGET(cwid->widget)->allocation.x, 2, 0);
   _clip_storni(ClipMachineMemory, GTK_WIDGET(cwid->widget)->allocation.y, 3, 0);
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

   int       sigid = _clip_parni(ClipMachineMemory, 2);

   char     *signame = _clip_parc(ClipMachineMemory, 2);

   C_widget *cwin = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   guint     accel_key = _clip_parni(ClipMachineMemory, 4);

   guint     accel_mods = _clip_parni(ClipMachineMemory, 5);

   GtkAccelFlags accel_flags = _clip_parni(ClipMachineMemory, 6);

   int       ret = FALSE;

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

   guint     accel_key = _clip_parni(ClipMachineMemory, 3);

   guint     accel_mods = _clip_parni(ClipMachineMemory, 4);

   int       ret = FALSE;

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

int
clip_GTK_WIDGETQUEUEDRAWAREA(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   gint      x = _clip_parni(ClipMachineMemory, 2);

   gint      y = _clip_parni(ClipMachineMemory, 3);

   gint      width = _clip_parni(ClipMachineMemory, 4);

   gint      height = _clip_parni(ClipMachineMemory, 5);

   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);
   CHECKARG(5, NUMERIC_type_of_ClipVarType);
   gtk_widget_queue_draw_area(cwid->widget, x, y, width, height);
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
   _clip_retl(ClipMachineMemory, gtk_widget_set_scroll_adjustments(cwid->widget,
								   GTK_ADJUSTMENT(chadj->widget),
								   GTK_ADJUSTMENT(cvadj->widget)));
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

   gchar    *name = _clip_parc(ClipMachineMemory, 2);

   gchar     emptyname[] = "\0";

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
   _clip_retc(ClipMachineMemory, (gchar *) gtk_widget_get_name(cwid->widget));
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

   gint      state = _clip_parni(ClipMachineMemory, 2);

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

   gboolean  sensitive = _clip_parl(ClipMachineMemory, 2);

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

   gboolean  paintable = _clip_parl(ClipMachineMemory, 2);

   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_widget_set_app_paintable(cwid->widget, paintable);
   return 0;
 err:
   return 1;
}

/* Set PANGO font of a widget */
int
clip_GTK_WIDGETMODIFYFONT(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   gchar    *fontname = _clip_parc(ClipMachineMemory, 2);

   CHECKARG(2, CHARACTER_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);

   gtk_widget_modify_font(cwid->widget, pango_font_description_from_string(fontname));

   return 0;
 err:
   return 1;
}

/* Modify FG for a widget                                                   */
/* Widget states                                                            */
/* GTK_STATE_NORMAL		0 // State during normal operation.         */
/* GTK_STATE_ACTIVE		1 // State of a currently active widget,    */
/*                                   such as a depressed button.            */
/* GTK_STATE_PRELIGHT		2 // State indicating that the mouse        */
/*                                   pointer is over the widget and the     */
/*                                   widget will respond to mouse clicks.   */
/* GTK_STATE_SELECTED		3 // State of a selected item, such the     */
/*                                   selected row in a list.                */
/* GTK_STATE_INSENSITIVE	4 // State indicating that the widget is    */
/*                                   unresponsive to user actions.          */

int
clip_GTK_WIDGETMODIFYFG(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   ClipVar  *mstyle = _clip_par(ClipMachineMemory, 2);

   GtkStateType state = _clip_parni(ClipMachineMemory, 3);

   GdkColor  fgcolor;

   double    colors[4];

   CHECKARG2(2, MAP_type_of_ClipVarType, ARRAY_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);

   _map_get_colors(ClipMachineMemory, mstyle, colors);
   fgcolor.red = colors[0];
   fgcolor.green = colors[1];
   fgcolor.blue = colors[2];
   fgcolor.pixel = colors[3];

   if (_clip_parinfo(ClipMachineMemory, 3) != NUMERIC_type_of_ClipVarType)
      state = GTK_STATE_NORMAL;

   gtk_widget_modify_fg(cwid->widget, state, &fgcolor);
   return 0;
 err:
   return 1;
}

/* Modify BG for a widget */
int
clip_GTK_WIDGETMODIFYBG(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   ClipVar  *mstyle = _clip_par(ClipMachineMemory, 2);

   GtkStateType state = _clip_parni(ClipMachineMemory, 3);

   GdkColor  bgcolor;

   double    colors[4];

   CHECKARG2(2, MAP_type_of_ClipVarType, ARRAY_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);

   _map_get_colors(ClipMachineMemory, mstyle, colors);
   bgcolor.red = colors[0];
   bgcolor.green = colors[1];
   bgcolor.blue = colors[2];
   bgcolor.pixel = colors[3];

   if (_clip_parinfo(ClipMachineMemory, 3) != NUMERIC_type_of_ClipVarType)
      state = GTK_STATE_NORMAL;

   gtk_widget_modify_bg(cwid->widget, state, &bgcolor);
   return 0;
 err:
   return 1;
}

/* Modify BASE for a widget */
int
clip_GTK_WIDGETMODIFYBASE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   ClipVar  *mstyle = _clip_par(ClipMachineMemory, 2);

   GtkStateType state = _clip_parni(ClipMachineMemory, 3);

   GdkColor  basecolor;

   double    colors[4];

   CHECKARG2(2, MAP_type_of_ClipVarType, ARRAY_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);

   _map_get_colors(ClipMachineMemory, mstyle, colors);
   basecolor.red = colors[0];
   basecolor.green = colors[1];
   basecolor.blue = colors[2];
   basecolor.pixel = colors[3];

   if (_clip_parinfo(ClipMachineMemory, 3) != NUMERIC_type_of_ClipVarType)
      state = GTK_STATE_NORMAL;

   gtk_widget_modify_base(cwid->widget, state, &basecolor);
   return 0;
 err:
   return 1;
}

/* Modify TEXT for a widget */
int
clip_GTK_WIDGETMODIFYTEXT(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   ClipVar  *mstyle = _clip_par(ClipMachineMemory, 2);

   GtkStateType state = _clip_parni(ClipMachineMemory, 3);

   GdkColor  textcolor;

   double    colors[4];

   CHECKARG2(2, MAP_type_of_ClipVarType, ARRAY_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);

   _map_get_colors(ClipMachineMemory, mstyle, colors);
   textcolor.red = colors[0];
   textcolor.green = colors[1];
   textcolor.blue = colors[2];
   textcolor.pixel = colors[3];
   if (_clip_parinfo(ClipMachineMemory, 3) != NUMERIC_type_of_ClipVarType)
      state = GTK_STATE_NORMAL;

   gtk_widget_modify_text(cwid->widget, state, &textcolor);
   return 0;
 err:
   return 1;
}

/* Set style of a widget */
int
clip_GTK_WIDGETSETSTYLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   ClipVar  *mstyle = _clip_par(ClipMachineMemory, 2);

   GtkStyle *style;

   int       i;

   CHECKARG(2, MAP_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);

   style = gtk_style_copy(cwid->widget->style);

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

   _map_to_style(ClipMachineMemory, mstyle, style);

  // Apply new style to a widget
   gtk_widget_set_style(cwid->widget, style);

   return 0;
 err:
   return 1;
}

int
clip_GTK_WIDGETMODIFYSTYLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   ClipVar  *mstyle = _clip_par(ClipMachineMemory, 2);

   GtkRcStyle *style;

   CHECKARG(2, MAP_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);

   style = gtk_widget_get_modifier_style(cwid->widget);

   _map_to_rc_style(ClipMachineMemory, mstyle, style, GTK_STATE_NORMAL);

  // Apply new style to a widget

   gtk_widget_modify_style(cwid->widget, style);

   return 0;
 err:
   return 1;
}

int
clip_GTK_WIDGETGETMODIFIERSTYLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   ClipVar  *mstyle = RETPTR(ClipMachineMemory);

   GtkRcStyle *style;

   CHECKCWID(cwid, GTK_IS_WIDGET);

   style = gtk_widget_get_modifier_style(cwid->widget);

   if (style)
    {
       memset(mstyle, 0, sizeof(*mstyle));
       _clip_map(ClipMachineMemory, mstyle);
       _rc_style_to_map(ClipMachineMemory, style, mstyle);
    }

   return 0;
 err:
   return 1;
}

/* Get style of a widget */
int
clip_GTK_WIDGETGETSTYLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   ClipVar  *mstyle = RETPTR(ClipMachineMemory);

   GtkStyle *style;

   int       i;

   CHECKCWID(cwid, GTK_IS_WIDGET);

  //style = gtk_style_copy (cwid->widget->style);
   style = cwid->widget->style;

  /////////alena*

   style->white_gc = cwid->widget->style->white_gc;
   g_object_ref(style->white_gc);
   style->black_gc = cwid->widget->style->black_gc;
   g_object_ref(style->black_gc);
   for (i = 0; i < 5; i++)
    {
       style->fg_gc[i] = cwid->widget->style->fg_gc[i];
       g_object_ref(style->fg_gc[i]);
       style->bg_gc[i] = cwid->widget->style->bg_gc[i];
       g_object_ref(style->bg_gc[i]);
       style->light_gc[i] = cwid->widget->style->light_gc[i];
       g_object_ref(style->light_gc[i]);
       style->dark_gc[i] = cwid->widget->style->dark_gc[i];
       g_object_ref(style->dark_gc[i]);
       style->mid_gc[i] = cwid->widget->style->mid_gc[i];
       g_object_ref(style->mid_gc[i]);
       style->text_gc[i] = cwid->widget->style->text_gc[i];
       g_object_ref(style->text_gc[i]);
       style->base_gc[i] = cwid->widget->style->base_gc[i];
       g_object_ref(style->base_gc[i]);
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

/* Get default style of a widget */
int
clip_GTK_WIDGETGETDEFAULTSTYLE(ClipMachine * ClipMachineMemory)
{
   ClipVar  *mstyle = RETPTR(ClipMachineMemory);

   GtkStyle *style;

   style = gtk_widget_get_default_style();

   memset(mstyle, 0, sizeof(*mstyle));
   _clip_map(ClipMachineMemory, mstyle);
   _style_to_map(ClipMachineMemory, style, mstyle);
   return 0;
}

void static
_remove_widget(GtkWidget * wid, gpointer data)
{
   ClipMachine *ClipMachineMemory = (ClipMachine *) data;

   if (GTK_IS_CONTAINER(wid))
      gtk_container_foreach(GTK_CONTAINER(wid), (GtkCallback) _remove_widget, ClipMachineMemory);
  //printf("22 widget type %d %d \n", GTK_WIDGET_GET_CLASS(wid), GTK_TYPE_ENTRY);
   _list_remove_cwidget(ClipMachineMemory, wid);
}

/* Destroy widget and it`s shadow object in CLIP machine */
int
clip_GTK_WIDGETDESTROY(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   if (cwid)
    {
      /*
         WTypeTable *wt_item = NULL;
         const char * cwtype_name = "GTK_WIDGET_UNKNOWN";
         wt_item = _wtype_table_get(cwid->type);
         if (wt_item && wt_item->ftype_name) cwtype_name = (const char *)wt_item->ftype_name();
         printf("start destroy widget %s \n", (char *)cwtype_name);
       */
       gtk_widget_destroy(cwid->widget);
      //        printf("widget destroyed \n");
    }
/*
	if (cwid)
	{
		if (cwid->widget)
		{
			if (GTK_IS_CONTAINER(cwid->widget))
				gtk_container_foreach(GTK_CONTAINER(cwid->widget), (GtkCallback)_remove_widget, ClipMachineMemory);
			else
			{
				//printf("widget type %d %d \n", cwid->type, GTK_TYPE_ENTRY);
				_list_remove_cwidget(ClipMachineMemory, cwid->widget);
			}
			gtk_widget_destroy(cwid->widget);
		}
		else
			destroy_c_widget(cwid);
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

int
clip_GTK_WIDGETGETANCESTOR(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   gint      type = _clip_parni(ClipMachineMemory, 2);

   GtkWidget *ancestor;

   C_widget *cancestor;

   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   switch (type)
    {
    case HASH_GTK_TYPE_WINDOW:
       type = GTK_TYPE_WINDOW;
       break;
    case HASH_GTK_TYPE_BOX:
       type = GTK_TYPE_BOX;
       break;
    case HASH_GTK_TYPE_CONTAINER:
       type = GTK_TYPE_CONTAINER;
       break;
    case HASH_GTK_TYPE_COMBO:
       type = GTK_TYPE_COMBO;
       break;
    case HASH_GTK_TYPE_TOOLBAR:
       type = GTK_TYPE_TOOLBAR;
       break;
    case HASH_GTK_TYPE_MENU:
       type = GTK_TYPE_MENU;
       break;
    default:
       type = GTK_TYPE_WIDGET;
       break;
    }
   ancestor = gtk_widget_get_ancestor(cwid->widget, type);
   if (ancestor)
    {
       cancestor = _list_get_cwidget(ClipMachineMemory, ancestor);
       if (!cancestor)
	  cancestor = _register_widget(ClipMachineMemory, ancestor, NULL);
       if (cancestor)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cancestor->obj);
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

   long      flags = _clip_parnl(ClipMachineMemory, 2);

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

   long      flags = _clip_parnl(ClipMachineMemory, 2);

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

   gint      mask = _clip_parni(ClipMachineMemory, 2);

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

   gint      mask = _clip_parni(ClipMachineMemory, 2);

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
       ccmap = _register_object(ClipMachineMemory, colormap, GDK_TYPE_COLORMAP, NULL,
				(coDestructor) gdk_object_colormap_destructor);
       if (ccmap)
	{
	  //ccmap->ref_count = 1;
	  //ccmap->ref_count ++;
	  //gdk_colormap_ref(colormap);
	  //g_object_ref(ccmap);
	   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &ccmap->obj);
	}
      //else
      //      gdk_colormap_unref(colormap);
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

   CHECKCWID(cwid, GTK_IS_WIDGET);
   win = gtk_widget_get_parent_window(cwid->widget);
   if (win)
    {
       cwin = _get_cobject(ClipMachineMemory, win, GDK_OBJECT_WINDOW, (coDestructor) gdk_object_window_destructor);
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

   GtkWidget *parent;

   C_widget *cparent;

   CHECKCWID(cwid, GTK_IS_WIDGET);
   parent = gtk_widget_get_parent(cwid->widget);
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
clip_GTK_WIDGETSETSIZEREQUEST(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   gint      width = _clip_parni(ClipMachineMemory, 2);

   gint      height = _clip_parni(ClipMachineMemory, 3);

   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   if (_clip_parinfo(ClipMachineMemory, 2) != NUMERIC_type_of_ClipVarType)
      width = -1;
   if (_clip_parinfo(ClipMachineMemory, 3) != NUMERIC_type_of_ClipVarType)
      height = -1;

   gtk_widget_set_size_request(cwid->widget, width, height);

   return 0;
 err:
   return 1;
}

int
clip_GTK_WIDGETSIZEREQUEST(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   gint      width = _clip_parni(ClipMachineMemory, 2);

   gint      height = _clip_parni(ClipMachineMemory, 3);

   GtkRequisition sreq;

   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);

   sreq.width = width;
   sreq.height = height;
   gtk_widget_size_request(cwid->widget, &sreq);

   return 0;
 err:
   return 1;
}

int
clip_GTK_WIDGETSHAPECOMBINEMASK(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   C_object *ccmask = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gint      offset_x = _clip_parni(ClipMachineMemory, 3);

   gint      offset_y = _clip_parni(ClipMachineMemory, 4);

   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);

   gtk_widget_shape_combine_mask(cwid->widget, (GdkPixmap *) (ccmask->object), offset_x, offset_y);

   return 0;
 err:
   return 1;
}

int
clip_GTK_WIDGETRESETSHAPES(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cwid, GTK_IS_WIDGET);

   gtk_widget_reset_shapes(cwid->widget);

   return 0;
 err:
   return 1;
}

int
clip_GTK_WIDGETCREATEPANGOCONTEXT(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   PangoContext *context;

   C_object *ccontext;

   CHECKCWID(cwid, GTK_IS_WIDGET);

   context = gtk_widget_create_pango_context(cwid->widget);

   if (context)
    {
       ccontext = _list_get_cobject(ClipMachineMemory, context);
       if (!ccontext)
	  ccontext = _register_object(ClipMachineMemory, context, GTK_TYPE_PANGO_CONTEXT, NULL, NULL);
       if (ccontext)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &ccontext->obj);
    }

   return 0;
 err:
   return 1;
}

int
clip_GTK_WIDGETGETPANGOCONTEXT(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   PangoContext *context;

   C_object *ccontext;

   CHECKCWID(cwid, GTK_IS_WIDGET);

   context = gtk_widget_get_pango_context(cwid->widget);

   if (context)
    {
       ccontext = _list_get_cobject(ClipMachineMemory, context);
       if (!ccontext)
	  ccontext = _register_object(ClipMachineMemory, context, GTK_TYPE_PANGO_CONTEXT, NULL, NULL);
       if (ccontext)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &ccontext->obj);
    }

   return 0;
 err:
   return 1;
}

int
clip_GTK_WIDGETCREATEPANGOLAYOUT(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   gchar    *text = _clip_parc(ClipMachineMemory, 2);

   PangoLayout *layout;

   C_object *clayout;

   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);

   if (text)
      LOCALE_TO_UTF(text);
   layout = gtk_widget_create_pango_layout(cwid->widget, (text) ? (const gchar *) text : NULL);

   if (layout)
    {
       clayout = _list_get_cobject(ClipMachineMemory, layout);
       if (!clayout)
	  clayout = _register_object(ClipMachineMemory, layout, GTK_TYPE_PANGO_LAYOUT, NULL, NULL);
       if (clayout)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &clayout->obj);
    }

   if (text)
      FREE_TEXT(text);
   return 0;
 err:
   return 1;
}

#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 4)

int
clip_GTK_WIDGETQUEURESIZENOREDRAW(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cwid, GTK_IS_WIDGET);

   gtk_widget_queue_resize_no_redraw(cwid->widget);

   return 0;
 err:
   return 1;
}

int
clip_GTK_WIDGETCANACTIVATEACCEL(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   guint     signal_id = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_widget_can_activate_accel(cwid->widget, signal_id);

   return 0;
 err:
   return 1;
}

int
clip_GTK_WIDGETSETNOSHOWALL(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   gboolean  show = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_widget_set_no_show_all(cwid->widget, show);

   return 0;
 err:
   return 1;
}

int
clip_GTK_WIDGETGETNOSHOWALL(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cwid, GTK_IS_WIDGET);

   _clip_retl(ClipMachineMemory, gtk_widget_get_no_show_all(cwid->widget));

   return 0;
 err:
   return 1;
}

int
clip_GTK_WIDGETLISTMNEMONICLABELS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   ClipVar  *cv = RETPTR(ClipMachineMemory);

   GList    *list;

   long      l;

   CHECKCWID(cwid, GTK_IS_WIDGET);

   list = gtk_widget_list_mnemonic_labels(cwid->widget);
   l = g_list_length(list);
   _clip_array(ClipMachineMemory, cv, 1, &l);
   for (l = 0; list; list = g_list_next(list), l++)
    {
       C_widget *cwid;

       GtkWidget *wid;

       wid = GTK_WIDGET(list->data);
       if (wid)
	{
	   cwid = _list_get_cwidget(ClipMachineMemory, wid);
	   if (!cwid)
	      cwid = _register_widget(ClipMachineMemory, wid, NULL);
	   if (cwid)
	      _clip_aset(ClipMachineMemory, cv, &cwid->obj, 1, &l);
	}
    }

   g_list_free(list);
   return 0;
 err:
   return 1;
}

int
clip_GTK_WIDGETADDMNEMONICLABEL(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   C_widget *clabel = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKCWID(clabel, GTK_IS_WIDGET);

   gtk_widget_add_mnemonic_label(cwid->widget, clabel->widget);
   return 0;
 err:
   return 1;
}

int
clip_GTK_WIDGETREMOVEMNEMONICLABEL(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   C_widget *clabel = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKCWID(clabel, GTK_IS_WIDGET);

   gtk_widget_remove_mnemonic_label(cwid->widget, clabel->widget);
   return 0;
 err:
   return 1;
}
#endif
