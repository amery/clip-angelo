/*
    Copyright (C) 2003 - 2005  ITK
    Author  : Elena V. Kornilova <alena@itk.ru>
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
/* Signals table */
static int
handle_delete_from_cursor(GtkTextView * text, GtkDeleteType arg1, gint arg2, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_DELETETYPE, arg1);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_ARG2, arg2);
   INVOKESIGHANDLER(GTK_WIDGET(text), cs, cv);
}

static int
handle_insert_at_cursor(GtkTextView * text, gchar * arg1, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputc(cs->cw->cmachine, &cv, HASH_ARG1, arg1, strlen(arg1));
   INVOKESIGHANDLER(GTK_WIDGET(text), cs, cv);
}

static int
handle_move_cursor(GtkTextView * text, GtkMovementStep arg1, gint arg2, gboolean arg3, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_ARG1, arg1);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_ARG2, arg2);
   _clip_mputl(cs->cw->cmachine, &cv, HASH_ARG3, arg3);
   INVOKESIGHANDLER(GTK_WIDGET(text), cs, cv);
}

static int
handle_move_focus(GtkTextView * text, GtkDirectionType arg1, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_ARG1, arg1);
   INVOKESIGHANDLER(GTK_WIDGET(text), cs, cv);
}

static int
handle_page_horizontally(GtkTextView * text, gint arg1, gboolean arg2, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_ARG1, arg1);
   _clip_mputl(cs->cw->cmachine, &cv, HASH_ARG2, arg2);
   INVOKESIGHANDLER(GTK_WIDGET(text), cs, cv);
}

static int
handle_populate_popup(GtkTextView * text, GtkMenu * arg1, C_signal * cs)
{
   C_widget *cwid;

   PREPARECV(cs, cv);
   cwid = _list_get_cwidget(cs->cw->cmachine, GTK_WIDGET(arg1));
   if (!cwid)
      cwid = _register_widget(cs->cw->cmachine, GTK_WIDGET(arg1), NULL);
   if (cwid)
      _clip_madd(cs->cw->cmachine, &cv, HASH_MENU, &cwid->obj);
   INVOKESIGHANDLER(GTK_WIDGET(text), cs, cv);
}

static int
handle_set_scroll_adjustments(GtkTextView * text, GtkAdjustment * arg1, GtkAdjustment * arg2, C_signal * cs)
{
   C_widget *cwid1;

   C_widget *cwid2;

   PREPARECV(cs, cv);

   cwid1 = _list_get_cwidget(cs->cw->cmachine, GTK_WIDGET(arg1));
   if (!cwid1)
      cwid1 = _register_widget(cs->cw->cmachine, GTK_WIDGET(arg1), NULL);
   if (cwid1)
      _clip_madd(cs->cw->cmachine, &cv, HASH_HADJUSTMENT, &cwid1->obj);

   cwid2 = _list_get_cwidget(cs->cw->cmachine, GTK_WIDGET(arg2));
   if (!cwid2)
      cwid2 = _register_widget(cs->cw->cmachine, GTK_WIDGET(arg2), NULL);
   if (cwid2)
      _clip_madd(cs->cw->cmachine, &cv, HASH_VADJUSTMENT, &cwid2->obj);
   INVOKESIGHANDLER(GTK_WIDGET(text), cs, cv);
}

static SignalTable text_view_signals[] = {
   {"copy-clipboard", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_COPY_CLIPBOARD_SIGNAL},
   {"cut-clipboard", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_CUT_CLIPBOARD_SIGNAL},
   {"delete-from-cursor", GSF(handle_delete_from_cursor),
    ESF(object_emit_signal), GTK_DELETE_FROM_CURSOR_SIGNAL},
   {"insert-at-cursor", GSF(handle_insert_at_cursor),
    ESF(object_emit_signal), GTK_INSERT_AT_CURSOR_SIGNAL},
   {"move-cursor", GSF(handle_move_cursor), ESF(object_emit_signal),
    GTK_MOVE_CURSOR_SIGNAL},
   {"move-focus", GSF(handle_move_focus), ESF(object_emit_signal),
    GTK_MOVE_FOCUS_SIGNAL},
   {"page-horizontally", GSF(handle_page_horizontally),
    ESF(object_emit_signal), GTK_PAGE_HORIZONTALLY_SIGNAL},
   {"paste-clipboard", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_PASTE_CLIPBOARD_SIGNAL},
   {"populate-popup", GSF(handle_populate_popup), ESF(object_emit_signal),
    GTK_POPULATE_POPUP_SIGNAL},
   {"set-anchor", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_SET_ANCHOR_SIGNAL},
   {"set-scroll-adjustments", GSF(handle_set_scroll_adjustments),
    ESF(object_emit_signal), GTK_SET_SCROLL_ADJUSTMENTS_SIGNAL},
   {"toggle-overwrite", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_TOGGLE_OVERWRITE_SIGNAL},
   {"", NULL, NULL, 0}
};

/**********************************************************/
CLIP_DLLEXPORT GtkType
_gtk_type_text_view()
{
   return GTK_TYPE_TEXT_VIEW;
}

CLIP_DLLEXPORT GtkType
_gtk_type_text_child_anchor()
{
   return GTK_TYPE_TEXT_CHILD_ANCHOR;
}

long
_clip_type_text_view()
{
   return GTK_WIDGET_TEXT_VIEW;
}

long
_clip_type_text_child_anchor()
{
   return GTK_OBJECT_TEXT_CHILD_ANCHOR;
}

const char *
_clip_type_name_text_view()
{
   return "GTK_OBJECT_TEXT_VIEW";
}

const char *
_clip_type_name_text_child_anchor()
{
   return "GTK_OBJECT_TEXT_CHILD_ANCHOR";
}

int
clip_INIT___TEXTVIEW(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_text_view, _clip_type_name_text_view,
		    _gtk_type_text_view, _gtk_type_container, text_view_signals);
   _wtype_table_put(_clip_type_text_child_anchor, _clip_type_name_text_child_anchor, _gtk_type_text_child_anchor, NULL, NULL);
   return 0;
}

int
clip_GTK_TEXTVIEWNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   GtkWidget *wid;

   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);

   wid = gtk_text_view_new();

   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWNEWWITHBUFFER(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   C_object *cbuffer = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   GtkWidget *wid;

   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cbuffer, GTK_IS_TEXT_BUFFER(cbuffer->object));

   wid = gtk_text_view_new_with_buffer(GTK_TEXT_BUFFER(cbuffer->object));

   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWSETBUFFER(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   C_object *cbuffer = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cbuffer, GTK_IS_TEXT_BUFFER(cbuffer->object));

   gtk_text_view_set_buffer(GTK_TEXT_VIEW(cview->widget), GTK_TEXT_BUFFER(cbuffer->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWGETBUFFER(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   GtkTextBuffer *buffer;

   C_object *cbuffer;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);

   buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(cview->widget));

   if (buffer)
    {
       cbuffer = _list_get_cobject(ClipMachineMemory, buffer);
       if (!cbuffer)
	  cbuffer = _register_object(ClipMachineMemory, buffer, GTK_TYPE_TEXT_BUFFER, NULL, NULL);
       if (cbuffer)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cbuffer->obj);
    }
   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWSCROLLTOMARK(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   C_object *cmark = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gdouble   within_margin = _clip_parnd(ClipMachineMemory, 3);

   gboolean  use_align = _clip_parl(ClipMachineMemory, 4);

   gdouble   xallign = _clip_parnd(ClipMachineMemory, 5);

   gdouble   yallign = _clip_parnd(ClipMachineMemory, 6);

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cmark, GTK_IS_TEXT_MARK(cmark->object));
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, LOGICAL_type_of_ClipVarType);
   CHECKARG(5, NUMERIC_type_of_ClipVarType);
   CHECKARG(6, NUMERIC_type_of_ClipVarType);

   gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(cview->widget),
				GTK_TEXT_MARK(cmark->object), within_margin, use_align, xallign, yallign);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWSCROLLTOITER(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   C_object *citer = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gdouble   within_margin = _clip_parnd(ClipMachineMemory, 3);

   gboolean  use_align = _clip_parl(ClipMachineMemory, 4);

   gdouble   xallign = _clip_parnd(ClipMachineMemory, 5);

   gdouble   yallign = _clip_parnd(ClipMachineMemory, 6);

   gboolean  ret;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, LOGICAL_type_of_ClipVarType);
   CHECKARG(5, NUMERIC_type_of_ClipVarType);
   CHECKARG(6, NUMERIC_type_of_ClipVarType);

   ret = gtk_text_view_scroll_to_iter(GTK_TEXT_VIEW(cview->widget),
				      (GtkTextIter *) (citer->object), within_margin, use_align, xallign, yallign);

   _clip_retl(ClipMachineMemory, ret);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWSCROLLMARKONSCREEN(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   C_object *cmark = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cmark, GTK_IS_TEXT_MARK(cmark->object));

   gtk_text_view_scroll_mark_onscreen(GTK_TEXT_VIEW(cview->widget), GTK_TEXT_MARK(cmark->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWMOVEMARKONSCREEN(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   C_object *cmark = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cmark, GTK_IS_TEXT_MARK(cmark->object));

   gtk_text_view_move_mark_onscreen(GTK_TEXT_VIEW(cview->widget), GTK_TEXT_MARK(cmark->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWPLACECURSORONSCREEN(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   gboolean  ret;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);

   ret = gtk_text_view_place_cursor_onscreen(GTK_TEXT_VIEW(cview->widget));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWGETVISIBLERECT(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   GdkRectangle rect;

   ClipVar  *mrect = RETPTR(ClipMachineMemory);

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);

   gtk_text_view_get_visible_rect(GTK_TEXT_VIEW(cview->widget), &rect);

   memset(mrect, 0, sizeof(mrect));
   _clip_map(ClipMachineMemory, mrect);
   _map_put_gdk_rectangle(ClipMachineMemory, mrect, &rect);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWGETITERLOCATION(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   C_object *citer = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   GdkRectangle rect;

   ClipVar  *mrect = RETPTR(ClipMachineMemory);

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);

   gtk_text_view_get_iter_location(GTK_TEXT_VIEW(cview->widget), (const GtkTextIter *) (citer->object), &rect);

   memset(mrect, 0, sizeof(mrect));
   _clip_map(ClipMachineMemory, mrect);
   _map_put_gdk_rectangle(ClipMachineMemory, mrect, &rect);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWGETLINEATY(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   C_object *citer = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gint      y = _clip_parni(ClipMachineMemory, 3);

   gint      line_top;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);

   gtk_text_view_get_line_at_y(GTK_TEXT_VIEW(cview->widget), (GtkTextIter *) (citer->object), y, &line_top);

   _clip_retni(ClipMachineMemory, line_top);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWGETLINEYRANGE(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   C_object *citer = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gint      y;

   gint      height;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);

   gtk_text_view_get_line_yrange(GTK_TEXT_VIEW(cview->widget), (GtkTextIter *) (citer->object), &y, &height);

   _clip_storni(ClipMachineMemory, y, 3, 0);
   _clip_storni(ClipMachineMemory, height, 4, 0);
   return 0;
 err:
   return 1;
}

/* gtk_TextViewGetIterAtLocation(textView, x, y) --> iterObj */
int
clip_GTK_TEXTVIEWGETITERATLOCATION(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   gint      x = _clip_parni(ClipMachineMemory, 2);

   gint      y = _clip_parni(ClipMachineMemory, 3);

   GtkTextIter *iter = 0;

   C_object *citer;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);

   gtk_text_view_get_iter_at_location(GTK_TEXT_VIEW(cview->widget), iter, x, y);

   if (iter)
    {
       citer = _list_get_cobject(ClipMachineMemory, iter);
       if (!citer)
	  citer = _register_object(ClipMachineMemory, iter, GTK_TYPE_TEXT_ITER, NULL, NULL);
       if (citer)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &citer->obj);
    }
   return 0;
 err:
   return 1;
}

/* gtk_TextViewBufferToWindowCoords(textView, winType, buffer_x,
   				buffer_y, @window_x, @window_y) */
int
clip_GTK_TEXTVIEWBUFFERTOWINDOWCOORDS(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   GtkTextWindowType win = INT_OPTION(ClipMachineMemory, 2, 0);

   gint      buffer_x = _clip_parni(ClipMachineMemory, 3);

   gint      buffer_y = _clip_parni(ClipMachineMemory, 4);

   gint      window_x;

   gint      window_y;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);

   gtk_text_view_buffer_to_window_coords(GTK_TEXT_VIEW(cview->widget), win, buffer_x, buffer_y, &window_x, &window_y);

   _clip_storni(ClipMachineMemory, window_x, 5, 0);
   _clip_storni(ClipMachineMemory, window_y, 6, 0);
   return 0;
 err:
   return 1;
}

/* gtk_TextViewWindowToBufferCoords(textView, winType, window_x,
   				window_y, @buffer_x, @buffer_y) */
int
clip_GTK_TEXTVIEWWINDOWTOBUFFERCOORDS(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   GtkTextWindowType win = INT_OPTION(ClipMachineMemory, 2, 0);

   gint      window_x = _clip_parni(ClipMachineMemory, 3);

   gint      window_y = _clip_parni(ClipMachineMemory, 4);

   gint      buffer_x;

   gint      buffer_y;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);

   gtk_text_view_buffer_to_window_coords(GTK_TEXT_VIEW(cview->widget), win, window_x, window_y, &buffer_x, &buffer_y);

   _clip_storni(ClipMachineMemory, buffer_x, 5, 0);
   _clip_storni(ClipMachineMemory, buffer_y, 6, 0);
   return 0;
 err:
   return 1;
}

/* gtk_TextViewGetWindow(textView, textWindowType) --> GDK window object */
int
clip_GTK_TEXTVIEWGETWINDOW(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   GtkTextWindowType win = INT_OPTION(ClipMachineMemory, 2, 0);

   GdkWindow *window;

   C_object *cwindow;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   window = gtk_text_view_get_window(GTK_TEXT_VIEW(cview->widget), win);

   if (window)
    {
       cwindow = _list_get_cobject(ClipMachineMemory, window);
       if (!cwindow)
	  cwindow = _register_object(ClipMachineMemory, window, GDK_TYPE_WINDOW, NULL, NULL);
       if (cwindow)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwindow->obj);
    }
   return 0;
 err:
   return 1;
}

/* gtk_TextViewGetWindowType(textView, gdkWindow) --> GTKwindowType */
int
clip_GTK_TEXTVIEWGETWINDOWTYPE(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   C_object *cwindow = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   GtkTextWindowType win;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);

   win = gtk_text_view_get_window_type(GTK_TEXT_VIEW(cview->widget), (GdkWindow *) (cwindow->object));

   _clip_retni(ClipMachineMemory, win);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWSETBORDERWINDOWSIZE(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   GtkTextWindowType win = INT_OPTION(ClipMachineMemory, 2, 0);

   gint      size = _clip_parni(ClipMachineMemory, 3);

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);

   gtk_text_view_set_border_window_size(GTK_TEXT_VIEW(cview->widget), win, size);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWGETBORDERWINDOWSIZE(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   GtkTextWindowType win = INT_OPTION(ClipMachineMemory, 2, 0);

   gint      size;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   size = gtk_text_view_get_border_window_size(GTK_TEXT_VIEW(cview->widget), win);

   _clip_retni(ClipMachineMemory, size);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWFORWARDDISPLAYLINE(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   C_object *citer = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gboolean  ret;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);

   ret = gtk_text_view_forward_display_line(GTK_TEXT_VIEW(cview->widget), (GtkTextIter *) (citer->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWBACKWARDDISPLAYLINE(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   C_object *citer = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gboolean  ret;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);

   ret = gtk_text_view_backward_display_line(GTK_TEXT_VIEW(cview->widget), (GtkTextIter *) (citer->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWFORWARDDISPLAYLINEEND(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   C_object *citer = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gboolean  ret;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);

   ret = gtk_text_view_forward_display_line_end(GTK_TEXT_VIEW(cview->widget), (GtkTextIter *) (citer->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWBACKWARDDISPLAYLINESTART(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   C_object *citer = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gboolean  ret;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);

   ret = gtk_text_view_backward_display_line_start(GTK_TEXT_VIEW(cview->widget), (GtkTextIter *) (citer->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWSTARTSDISPLAYLINE(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   C_object *citer = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gboolean  ret;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);

   ret = gtk_text_view_starts_display_line(GTK_TEXT_VIEW(cview->widget), (GtkTextIter *) (citer->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWMOVEVISUALLY(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   C_object *citer = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gint      count = _clip_parni(ClipMachineMemory, 3);

   gboolean  ret;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);

   ret = gtk_text_view_move_visually(GTK_TEXT_VIEW(cview->widget), (GtkTextIter *) (citer->object), count);

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/* gtk_TextViewAddChildAtAnchor(textView, widget) --> GTK child anchor object */
int
clip_GTK_TEXTVIEWADDCHILDATANCHOR(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cchild = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   GtkTextChildAnchor *anchor = 0;

   C_object *canchor;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);

   gtk_text_view_add_child_at_anchor(GTK_TEXT_VIEW(cview->widget), GTK_WIDGET(cchild->widget), anchor);

   if (anchor)
    {
       canchor = _list_get_cobject(ClipMachineMemory, anchor);
       if (!canchor)
	  canchor = _register_object(ClipMachineMemory, anchor, GTK_TYPE_TEXT_CHILD_ANCHOR, NULL, NULL);
       if (canchor)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &canchor->obj);
    }
   return 0;
 err:
   return 1;
}

/************** GTK TEXT CHILD ANCHOR **********************/

int
clip_GTK_TEXTCHILDANCHORNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   GtkTextChildAnchor *anchor;

   C_object *canchor;

   CHECKOPT(1, MAP_type_of_ClipVarType);

   anchor = gtk_text_child_anchor_new();

   if (anchor)
    {
       canchor = _list_get_cobject(ClipMachineMemory, anchor);
       if (!canchor)
	  canchor = _register_object(ClipMachineMemory, anchor, GTK_TYPE_TEXT_CHILD_ANCHOR, cv, NULL);
       if (canchor)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &canchor->obj);
    }

   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTCHILDANCHORGETWIDGETS(ClipMachine * ClipMachineMemory)
{
   C_object *chanchor = _fetch_co_arg(ClipMachineMemory);

   GList    *glist;

   ClipVar  *list = RETPTR(ClipMachineMemory);

   long      n;

   CHECKARG(1, MAP_type_of_ClipVarType);

   glist = gtk_text_child_anchor_get_widgets((GtkTextChildAnchor *) (chanchor->object));
   n = g_list_length(glist);
   memset(list, 0, sizeof(*list));
   _clip_array(ClipMachineMemory, list, 1, &n);
   if (n > 0)
    {
       long      i = 0;

       while (glist)
	{
	   ClipVar   cv;

	   C_widget *cwid;

	   memset(&cv, 0, sizeof(cv));
	   cv.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = MAP_type_of_ClipVarType;

	   if (!glist->data)
	      goto err;
	   cwid = _register_widget(ClipMachineMemory, GTK_WIDGET(glist->data), NULL);
	   _clip_mclone(ClipMachineMemory, &cv, &cwid->obj);
	   _clip_aset(ClipMachineMemory, list, &cv, 1, &i);
	   glist = g_list_next(glist);
	   i++;
	   _clip_destroy(ClipMachineMemory, &cv);
	}
    }

   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTCHILDANCHORGETDELETED(ClipMachine * ClipMachineMemory)
{
   C_object *chanchor = _fetch_co_arg(ClipMachineMemory);

   gboolean  ret;

   CHECKARG(1, MAP_type_of_ClipVarType);

   ret = gtk_text_child_anchor_get_deleted((GtkTextChildAnchor *) (chanchor->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWADDCHILDINWINDOW(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cchild = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   GtkTextWindowType type = INT_OPTION(ClipMachineMemory, 3, 0);

   gint      xpos = _clip_parni(ClipMachineMemory, 4);

   gint      ypos = _clip_parni(ClipMachineMemory, 5);

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);
   CHECKARG(5, NUMERIC_type_of_ClipVarType);

   gtk_text_view_add_child_in_window(GTK_TEXT_VIEW(cview->widget), GTK_WIDGET(cchild->widget), type, xpos, ypos);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWMOVECHILD(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cchild = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gint      xpos = _clip_parni(ClipMachineMemory, 3);

   gint      ypos = _clip_parni(ClipMachineMemory, 4);

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);

   gtk_text_view_move_child(GTK_TEXT_VIEW(cview->widget), GTK_WIDGET(cchild->widget), xpos, ypos);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWSETWRAPMODE(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   GtkWrapMode mode = INT_OPTION(ClipMachineMemory, 2, 0);

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(cview->widget), mode);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWGETWRAPMODE(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   GtkWrapMode mode;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);

   mode = gtk_text_view_get_wrap_mode(GTK_TEXT_VIEW(cview->widget));

   _clip_retni(ClipMachineMemory, mode);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWSETEDITABLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   gboolean  editable = BOOL_OPTION(ClipMachineMemory, 2, 1);

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_text_view_set_editable(GTK_TEXT_VIEW(cview->widget), editable);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWGETEDITABLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   gboolean  editable;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);

   editable = gtk_text_view_get_editable(GTK_TEXT_VIEW(cview->widget));

   _clip_retl(ClipMachineMemory, editable);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWSETCURSORVISIBLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   gboolean  setting = BOOL_OPTION(ClipMachineMemory, 2, 1);

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(cview->widget), setting);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWGETCURSORVISIBLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   gboolean  setting;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);

   setting = gtk_text_view_get_cursor_visible(GTK_TEXT_VIEW(cview->widget));

   _clip_retl(ClipMachineMemory, setting);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWSETPIXELSABOVELINES(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   gint      pixels = _clip_parni(ClipMachineMemory, 2);

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_text_view_set_pixels_above_lines(GTK_TEXT_VIEW(cview->widget), pixels);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWGETPIXELSABOVELINES(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   gint      pixels;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);

   pixels = gtk_text_view_get_pixels_above_lines(GTK_TEXT_VIEW(cview->widget));

   _clip_retni(ClipMachineMemory, pixels);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWSETPIXELSBELOWLINES(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   gint      pixels = _clip_parni(ClipMachineMemory, 2);

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_text_view_set_pixels_below_lines(GTK_TEXT_VIEW(cview->widget), pixels);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWGETPIXELSBELOWLINES(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   gint      pixels;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);

   pixels = gtk_text_view_get_pixels_below_lines(GTK_TEXT_VIEW(cview->widget));

   _clip_retni(ClipMachineMemory, pixels);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWSETPIXELSINSIDEWRAP(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   gint      pixels = _clip_parni(ClipMachineMemory, 2);

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_text_view_set_pixels_inside_wrap(GTK_TEXT_VIEW(cview->widget), pixels);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWGETPIXELSINSIDEWRAP(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   gint      pixels;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);

   pixels = gtk_text_view_get_pixels_inside_wrap(GTK_TEXT_VIEW(cview->widget));

   _clip_retni(ClipMachineMemory, pixels);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWSETJUSTIFICATION(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   GtkJustification justification = INT_OPTION(ClipMachineMemory, 2, 0);

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_text_view_set_justification(GTK_TEXT_VIEW(cview->widget), justification);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWGETJUSTIOFICATION(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   GtkJustification justification;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);

   justification = gtk_text_view_get_justification(GTK_TEXT_VIEW(cview->widget));

   _clip_retni(ClipMachineMemory, justification);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWSETLEFTMARGIN(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   gint      margin = _clip_parni(ClipMachineMemory, 2);

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_text_view_set_left_margin(GTK_TEXT_VIEW(cview->widget), margin);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWGETLEFTMARGIN(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   gint      margin;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);

   margin = gtk_text_view_get_left_margin(GTK_TEXT_VIEW(cview->widget));

   _clip_retni(ClipMachineMemory, margin);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWSETRIGHTMARGIN(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   gint      margin = _clip_parni(ClipMachineMemory, 2);

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_text_view_set_right_margin(GTK_TEXT_VIEW(cview->widget), margin);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWGETRIGHTMARGIN(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   gint      margin;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);

   margin = gtk_text_view_get_right_margin(GTK_TEXT_VIEW(cview->widget));

   _clip_retni(ClipMachineMemory, margin);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWSETINDENT(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   gint      indent = _clip_parni(ClipMachineMemory, 2);

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_text_view_set_indent(GTK_TEXT_VIEW(cview->widget), indent);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWGETINDENT(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   gint      indent;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);

   indent = gtk_text_view_get_indent(GTK_TEXT_VIEW(cview->widget));

   _clip_retni(ClipMachineMemory, indent);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWGETDEFAULTATTRIBUTES(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   GtkTextAttributes *attrs;

   C_object *cattrs;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);

   attrs = gtk_text_view_get_default_attributes(GTK_TEXT_VIEW(cview->widget));

   if (attrs)
    {
       cattrs = _list_get_cobject(ClipMachineMemory, attrs);
       if (!cattrs)
	  cattrs = _register_object(ClipMachineMemory, attrs, GTK_TYPE_TEXT_ATTRIBUTES, NULL, NULL);
       if (cattrs)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cattrs->obj);
    }
   return 0;
 err:
   return 1;
}

#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 4)
int
clip_GTK_TEXTVIEWSETOVERWRITE(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   gboolean  set = _clip_parl(ClipMachineMemory, 2);

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_text_view_set_overwrite(GTK_TEXT_VIEW(cview->widget), set);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWGETOVERWRITE(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);

   _clip_retl(ClipMachineMemory, gtk_text_view_get_overwrite(GTK_TEXT_VIEW(cview->widget)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWSETACCEPTSTAB(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   gboolean  set = _clip_parl(ClipMachineMemory, 2);

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_text_view_set_accepts_tab(GTK_TEXT_VIEW(cview->widget), set);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TEXTVIEWGETACCEPTSTAB(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);

   _clip_retl(ClipMachineMemory, gtk_text_view_get_accepts_tab(GTK_TEXT_VIEW(cview->widget)));

   return 0;
 err:
   return 1;
}
#endif
#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 6)
/* gtk_TextViewGetIterAtPosition(textView, @trailing, x, y) --> gtkTextIter) */
int
clip_GTK_TEXTVIEWGETITERATPOSITION(ClipMachine * ClipMachineMemory)
{
   C_widget *cview = _fetch_cw_arg(ClipMachineMemory);

   gint      x = _clip_parni(ClipMachineMemory, 3);

   gint      y = _clip_parni(ClipMachineMemory, 4);

   gint      trailing;

   GtkTextIter *iter = 0;

   C_object *citer;

   CHECKOPT2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cview, GTK_IS_TEXT_VIEW);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);

   gtk_text_view_get_iter_at_position(GTK_TEXT_VIEW(cview->widget), iter, &trailing, x, y);

   if (iter)
    {
       citer = _list_get_cobject(ClipMachineMemory, iter);
       if (!citer)
	  citer = _register_object(ClipMachineMemory, iter, GTK_TYPE_TEXT_ITER, NULL, NULL);
       if (citer)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &citer->obj);
    }

   _clip_storni(ClipMachineMemory, trailing, 2, 0);

   return 0;
 err:
   return 1;
}
#endif
