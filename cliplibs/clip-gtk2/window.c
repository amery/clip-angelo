/*
    Copyright (C) 2001 - 2005  ITK
    Author  : Alexey M. Tkachenko <alexey@itk.ru>
              Elena V. Kornilova  <alena@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include <string.h>
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg2.h"

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include "ci_clip-gtk2.ch"
#include "ci_clip-gtk2.h"

/*********************** SIGNALS **************************/
static gint
handle_set_focus_signal(GtkWindow * window, GtkWidget * widget, C_signal * cs)
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

static gint
handle_move_focus_signal(GtkWindow * window, GtkDirectionType arg1, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_DIRECTIONTYPE, arg1);
   INVOKESIGHANDLER(GTK_WIDGET(window), cs, cv);
}

static gint
handle_frame_event_signal(GtkWindow * window, GdkEvent * event, C_signal * cs)
{
   return handle_events(GTK_WIDGET(window), event, cs);

}

static SignalTable window_signals[] = {
   {"activate-default", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_ACTIVATE_DEFAULT_SIGNAL},
   {"activate-focus", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_ACTIVATE_FOCUS_SIGNAL},
   {"kyes-changed", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_KEYS_CHANGED_SIGNAL},
   {"move-focus", GSF(handle_move_focus_signal), ESF(object_emit_signal),
    GTK_MOVE_FOCUS_SIGNAL},
   {"set-focus", GSF(handle_set_focus_signal), ESF(object_emit_signal),
    GTK_SET_FOCUS_SIGNAL},
   {"frame-event", GSF(handle_frame_event_signal), ESF(object_emit_signal),
    GTK_FRAME_EVENT_SIGNAL},
   {"", NULL, NULL, 0}
};

/* Register widget signals table in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_window()
{
   return GTK_TYPE_WINDOW;
}

long
_clip_type_window()
{
   return GTK_WIDGET_WINDOW;
}

const char *
_clip_type_name_window()
{
   return "GTK_WIDGET_WINDOW";
}

int
clip_INIT___WINDOW(ClipMachine * ClipMachineMemory)
{
  //_wtype_table_put(_clip_type_window, _clip_type_name_window, _gtk_type_window, _gtk_type_container, window_signals);
   _wtype_table_put(_clip_type_window, _clip_type_name_window, _gtk_type_window, _gtk_type_bin, window_signals);
   return 0;
}

/**********************************************************/

/*
gint delete_window_handler( GtkWidget *widget, GdkEvent *event, gpointer data )
{
	gtk_widget_destroy(widget);
	gtk_main_quit();
	return(FALSE);
}
*/

/****  WINDOW constructor ****/
int
clip_GTK_WINDOWNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   char *title = _clip_parc(ClipMachineMemory, 2);
   gint typ = INT_OPTION(ClipMachineMemory, 3, 0);
   GtkWidget *wid = NULL;
   C_widget *cwid;
   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);

  //wid = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   wid = gtk_window_new(typ);
   if (!wid)
      goto err;
   if (_clip_parinfo(ClipMachineMemory, 2) == CHARACTER_type_of_ClipVarType)
      {
	 LOCALE_TO_UTF(title);
	 gtk_window_set_title(GTK_WINDOW(wid), title);
	 FREE_TEXT(title);
      }
  //cwid = (C_widget*)calloc( 1, sizeof(C_widget) );
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   cwid->accel_group = gtk_accel_group_new();
   gtk_window_add_accel_group(GTK_WINDOW(wid), cwid->accel_group);
//      gtk_signal_connect( GTK_OBJECT( wid ), "delete-event",
//              GTK_SIGNAL_FUNC( delete_window_handler ), NULL );

   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

/****  ------------------ ****/

// Set ICON PIXMAP property for window
int
clip_GTK_WINDOWSETICONPIXMAP(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cpix = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   GdkPixmap *pix;
   GdkBitmap *bit;
   CHECKCWID(cwin, GTK_IS_WINDOW);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cpix, GTK_IS_PIXMAP);
   gtk_pixmap_get(GTK_PIXMAP(cpix->widget), &pix, &bit);
   gdk_window_set_icon(GDK_WINDOW(cwin->widget->window), GDK_WINDOW(cpix->widget->window), pix, bit);
   return 0;
 err:
   return 1;
}

#if !((GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 6))

// Set ICON NAME property for window

int
clip_GTK_WINDOWSETICONNAME(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   gchar *name = _clip_parc(ClipMachineMemory, 2);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   CHECKCWID(cwin, GTK_IS_WINDOW);
   if (cwin->widget->window)
      {
	 LOCALE_TO_UTF(name);
	 gdk_window_set_icon_name(cwin->widget->window, name);
	 FREE_TEXT(name);
	 _clip_retl(ClipMachineMemory, TRUE);
      }
   else
      _clip_retl(ClipMachineMemory, FALSE);
   return 0;
 err:
   return 1;
}
#endif

// Set DECORATIONS for window
int
clip_GTK_WINDOWSETDECORATIONS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   GdkWMDecoration Decor = _clip_parni(ClipMachineMemory, 2);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   CHECKCWID(cwin, GTK_IS_WINDOW);
   if (cwin->widget->window)
      {
	 gdk_window_set_decorations(cwin->widget->window, Decor);
	 _clip_retl(ClipMachineMemory, TRUE);
      }
   else
      _clip_retl(ClipMachineMemory, FALSE);
   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWSETDECORATED(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   gboolean setting = _clip_parl(ClipMachineMemory, 2);

   CHECKARG(2, LOGICAL_type_of_ClipVarType);
   CHECKCWID(cwin, GTK_IS_WINDOW);

   gtk_window_set_decorated(GTK_WINDOW(cwin->widget), setting);
   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWGETDECORATED(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   gboolean setting;

   CHECKCWID(cwin, GTK_IS_WINDOW);

   setting = gtk_window_get_decorated(GTK_WINDOW(cwin->widget));

   _clip_parl(ClipMachineMemory, setting);
   return 0;
 err:
   return 1;
}

// Set FUNCTIONS for window
int
clip_GTK_WINDOWSETFUNCTIONS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   GdkWMFunction Func = _clip_parni(ClipMachineMemory, 2);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   CHECKCWID(cwin, GTK_IS_WINDOW);
   if (cwin->widget->window)
      {
	 gdk_window_set_functions(cwin->widget->window, Func);
	 _clip_retl(ClipMachineMemory, TRUE);
      }
   else
      _clip_retl(ClipMachineMemory, FALSE);
   return 0;
 err:
   return 1;
}

/* Sets resize policy */
int
clip_GTK_WINDOWSETPOLICY(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   gboolean AutoShrink = FALSE, AllowShrink = TRUE, AllowGrow = TRUE;
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   CHECKOPT(3, LOGICAL_type_of_ClipVarType);
   CHECKOPT(4, LOGICAL_type_of_ClipVarType);
   CHECKCWID(cwin, GTK_IS_WINDOW);
   if (_clip_parinfo(ClipMachineMemory, 2) == LOGICAL_type_of_ClipVarType)
      AutoShrink = _clip_parl(ClipMachineMemory, 2);
   if (_clip_parinfo(ClipMachineMemory, 3) == LOGICAL_type_of_ClipVarType)
      AllowGrow = _clip_parl(ClipMachineMemory, 3);
   if (_clip_parinfo(ClipMachineMemory, 4) == LOGICAL_type_of_ClipVarType)
      AllowShrink = _clip_parl(ClipMachineMemory, 4);
   gtk_window_set_policy(GTK_WINDOW(cwin->widget), AllowShrink, AllowGrow, AutoShrink);
   return 0;
 err:
   return 1;
}

/* Raise window */
int
clip_GTK_WINDOWRAISE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cwin, GTK_IS_WINDOW);
   if (cwin->widget->window)
      {
	 gdk_window_raise(cwin->widget->window);
	 _clip_retl(ClipMachineMemory, TRUE);
      }
   else
      _clip_retl(ClipMachineMemory, FALSE);
   return 0;
 err:
   return 1;
}

/* Sets title for window */
int
clip_GTK_WINDOWSETTITLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   gchar *title = _clip_parc(ClipMachineMemory, 2);
   CHECKCWID(cwin, GTK_IS_WINDOW);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);
   LOCALE_TO_UTF(title);
   gtk_window_set_title(GTK_WINDOW(cwin->widget), title);
   FREE_TEXT(title);
   return 0;
 err:
   return 1;
}

/* Gets title of window */
int
clip_GTK_WINDOWGETTITLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   gchar *title;

   CHECKCWID(cwin, GTK_IS_WINDOW);

   title = (gchar *) gtk_window_get_title(GTK_WINDOW(cwin->widget));

   LOCALE_FROM_UTF(title);
   _clip_retc(ClipMachineMemory, title);
   FREE_TEXT(title);
   return 0;
 err:
   return 1;
}

/*  */
int
clip_GTK_WINDOWACTIVATEFOCUS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(cwin, GTK_IS_WINDOW);
   _clip_retni(ClipMachineMemory, gtk_window_activate_focus(GTK_WINDOW(cwin->widget)));
   return 0;
 err:
   return 1;
}

/*  */
int
clip_GTK_WINDOWACTIVATEDEFAULT(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(cwin, GTK_IS_WINDOW);
   _clip_retni(ClipMachineMemory, gtk_window_activate_default(GTK_WINDOW(cwin->widget)));
   return 0;
 err:
   return 1;
}

/*  */
int
clip_GTK_WINDOWSETTRANSIENTFOR(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwin2 = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwin, GTK_IS_WINDOW);
   CHECKCWID(cwin2, GTK_IS_WINDOW);
   gtk_window_set_transient_for(GTK_WINDOW(cwin->widget), GTK_WINDOW(cwin2->widget));
   return 0;
 err:
   return 1;
}

/* If window is set modal, input will be grabbed when show and released when hide */
int
clip_GTK_WINDOWSETMODAL(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   gboolean modal = _clip_parl(ClipMachineMemory, 2);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      modal = TRUE;
   CHECKCWID(cwin, GTK_IS_WINDOW);
   gtk_window_set_modal(GTK_WINDOW(cwin->widget), modal);
   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWGETMODAL(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   gboolean modal;
   CHECKCWID(cwin, GTK_IS_WINDOW);
   modal = gtk_window_get_modal(GTK_WINDOW(cwin->widget));
   _clip_retl(ClipMachineMemory, modal);
   return 0;
 err:
   return 1;
}

/* Sets focus to widget */
int
clip_GTK_WINDOWSETFOCUS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwin, GTK_IS_WINDOW);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_window_set_focus(GTK_WINDOW(cwin->widget), cwid->widget);
   return 0;
 err:
   return 1;
}

/* Gets focus to widget */
int
clip_GTK_WINDOWGETFOCUS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwid;
   CHECKCWID(cwin, GTK_IS_WINDOW);

   cwid = _get_cwidget(ClipMachineMemory, gtk_window_get_focus(GTK_WINDOW(cwin->widget)));

   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

/* Sets default widget in window */
int
clip_GTK_WINDOWSETDEFAULT(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwin, GTK_IS_WINDOW);
   CHECKCWID(cwid, GTK_IS_WIDGET);

   GTK_WIDGET_SET_FLAGS(GTK_WIDGET(cwid->widget), GTK_CAN_DEFAULT);
   gtk_window_set_default(GTK_WINDOW(cwin->widget), GTK_WIDGET(cwid->widget));
   return 0;
 err:
   return 1;
}

/* Sets default size for window */
int
clip_GTK_WINDOWSETDEFAULTSIZE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   gint width = _clip_parni(ClipMachineMemory, 2);
   gint height = _clip_parni(ClipMachineMemory, 3);

   CHECKCWID(cwin, GTK_IS_WINDOW);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      width = -1;
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      height = -1;

   gtk_window_set_default_size(GTK_WINDOW(cwin->widget), width, height);
   return 0;
 err:
   return 1;
}

/* Gets default size for window */
int
clip_GTK_WINDOWGETDEFAULTSIZE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   gint width;
   gint height;

   CHECKCWID(cwin, GTK_IS_WINDOW);
   gtk_window_get_default_size(GTK_WINDOW(cwin->widget), &width, &height);
   _clip_storni(ClipMachineMemory, width, 2, 0);
   _clip_storni(ClipMachineMemory, height, 3, 0);
   return 0;
 err:
   return 1;
}

/* Alena:Set  window positions */
/* GTK_WIN_POS_NONE   	No influence is made on placement.                      */
/* GTK_WIN_POS_CENTER   Windows should be placed in the center of the screen.   */
/* GTK_WIN_POS_MOUSE    Windows should be placed at the current mouse position. */
/* GTK_WIN_POS_CENTER_ALWAYS Keep window centered as it changes size, etc.      */
/* GTK_WIN_POS_CENTER_ON_PARENT Center the window on its transient parent (see  */
/*				 gtk_window_set_transient_for()). 		*/
int
clip_GTK_WINDOWSETPOSITION(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   gint position = _clip_parni(ClipMachineMemory, 2);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwin, GTK_IS_WINDOW);
   gtk_window_set_position(GTK_WINDOW(cwin->widget), position);
   return 0;
 err:
   return 1;
}

/* Sets unique name for restart application */
/* if TITLE -is unique name - this function can't be use :) */
int
clip_GTK_WINDOWSETROLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   gchar *role = _clip_parc(ClipMachineMemory, 2);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);
   CHECKCWID(cwin, GTK_IS_WINDOW);

   gtk_window_set_role(GTK_WINDOW(cwin->widget), (const gchar *) role);

   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWGETROLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   G_CONST_RETURN gchar *role;

   CHECKCWID(cwin, GTK_IS_WINDOW);

   role = gtk_window_get_role(GTK_WINDOW(cwin->widget));

   _clip_retc(ClipMachineMemory, (gchar *) role);
   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWADDACCELGROUP(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   C_widget *caccg = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwin, GTK_IS_WINDOW);
   CHECKCWID(caccg, GTK_IS_ACCEL_GROUP);

   gtk_window_add_accel_group(GTK_WINDOW(cwin->widget), GTK_ACCEL_GROUP(caccg->widget));

   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWREMOVEACCELGROUP(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   C_widget *caccg = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwin, GTK_IS_WINDOW);
   CHECKCWID(caccg, GTK_IS_ACCEL_GROUP);

   gtk_window_remove_accel_group(GTK_WINDOW(cwin->widget), GTK_ACCEL_GROUP(caccg->widget));

   return 0;
 err:
   return 1;
}

/*
  see clip-gtk2.ch
GDK_WINDOW_TYPE_HINT_NORMAL	1
GDK_WINDOW_TYPE_HINT_DIALOG	2
GDK_WINDOW_TYPE_HINT_MENU	3
GDK_WINDOW_TYPE_HINT_TOOLBAR	4

*/

int
clip_GTK_WINDOWSETTYPEHINT(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   gint hint = _clip_parni(ClipMachineMemory, 2);

   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwin, GTK_IS_WINDOW);

   gtk_window_set_type_hint(GTK_WINDOW(cwin->widget), hint);

   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWGETTYPEHINT(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   gint hint;

   CHECKCWID(cwin, GTK_IS_WINDOW);

   hint = gtk_window_get_type_hint(GTK_WINDOW(cwin->widget));

   _clip_retni(ClipMachineMemory, hint);

   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWSETDESTROYWITHPARENT(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   gboolean setting = _clip_parl(ClipMachineMemory, 2);

   CHECKARG(2, LOGICAL_type_of_ClipVarType);
   CHECKCWID(cwin, GTK_IS_WINDOW);

   gtk_window_set_destroy_with_parent(GTK_WINDOW(cwin->widget), setting);

   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWGETDESTROYWITHPARENT(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   gboolean setting;

   CHECKCWID(cwin, GTK_IS_WINDOW);

   setting = gtk_window_get_destroy_with_parent(GTK_WINDOW(cwin->widget));

   _clip_retl(ClipMachineMemory, setting);
   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWSETRESIZEABLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   gboolean resizable = _clip_parl(ClipMachineMemory, 2);

   CHECKARG(2, LOGICAL_type_of_ClipVarType);
   CHECKCWID(cwin, GTK_IS_WINDOW);

   gtk_window_set_resizable(GTK_WINDOW(cwin->widget), resizable);

   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWGETRESIZEABLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   gboolean resizable;

   CHECKCWID(cwin, GTK_IS_WINDOW);

   resizable = gtk_window_get_resizable(GTK_WINDOW(cwin->widget));

   _clip_retl(ClipMachineMemory, resizable);

   return 0;
 err:
   return 1;
}

/*
  see clip-gtk2.ch
GDK_GRAVITY_NORTH_WEST	1
GDK_GRAVITY_NORTH	2
GDK_GRAVITY_NORTH_EAST	3
GDK_GRAVITY_WEST	4
GDK_GRAVITY_CENTER	5
GDK_GRAVITY_EAST	6
GDK_GRAVITY_SOUTH_WEST	7
GDK_GRAVITY_SOUTH	8
GDK_GRAVITY_SOUTH_EAST	9
GDK_GRAVITY_STATIC	10
*/
int
clip_GTK_WINDOWSETGRAVITY(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   gint gravity = _clip_parni(ClipMachineMemory, 2);

   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwin, GTK_IS_WINDOW);

   gtk_window_set_gravity(GTK_WINDOW(cwin->widget), gravity);

   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWGETGRAVITY(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   gint gravity;

   CHECKCWID(cwin, GTK_IS_WINDOW);

   gravity = gtk_window_get_gravity(GTK_WINDOW(cwin->widget));

   _clip_retni(ClipMachineMemory, gravity);

   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWSETHASFRAME(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   gboolean setting = _clip_parl(ClipMachineMemory, 2);

   CHECKARG(2, LOGICAL_type_of_ClipVarType);
   CHECKCWID(cwin, GTK_IS_WINDOW);

   gtk_window_set_has_frame(GTK_WINDOW(cwin->widget), setting);

   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWGETHASFRAME(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   gboolean setting;

   CHECKCWID(cwin, GTK_IS_WINDOW);

   setting = gtk_window_get_has_frame(GTK_WINDOW(cwin->widget));

   _clip_retl(ClipMachineMemory, setting);

   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWSETFRAMEDIMENSIONS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   gint left = _clip_parl(ClipMachineMemory, 2);
   gint top = _clip_parl(ClipMachineMemory, 3);
   gint right = _clip_parl(ClipMachineMemory, 4);
   gint bottom = _clip_parl(ClipMachineMemory, 5);

   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);
   CHECKARG(5, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwin, GTK_IS_WINDOW);

   gtk_window_set_frame_dimensions(GTK_WINDOW(cwin->widget), left, top, right, bottom);

   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWGETFRAMEDIMENSIONS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   gint left;
   gint top;
   gint right;
   gint bottom;

   CHECKCWID(cwin, GTK_IS_WINDOW);

   gtk_window_get_frame_dimensions(GTK_WINDOW(cwin->widget), &left, &top, &right, &bottom);

   _clip_storni(ClipMachineMemory, left, 2, 0);
   _clip_storni(ClipMachineMemory, top, 3, 0);
   _clip_storni(ClipMachineMemory, right, 4, 0);
   _clip_storni(ClipMachineMemory, bottom, 5, 0);
   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWSETICONLIST(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   ClipVar *items = _clip_spar(ClipMachineMemory, 2);

   CHECKCWID(cwin, GTK_IS_WINDOW);
   CHECKOPT(2, ARRAY_type_of_ClipVarType);

   if (items->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
      {
	 GList *list = NULL;
	 unsigned short i;
	 ClipVar *item;
	 C_object *citem;
	 for (i = 0; i < items->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar; i++)
	    {
	       item = &items->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar[i];
	       if (item->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType || item->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
		  {
		     citem = _fetch_cobject(ClipMachineMemory, item);
		     CHECKCOBJ(citem, GDK_IS_PIXBUF(citem->object));
		     list = g_list_append(list, citem->object);
		  }
	    }
	 if (list)
	    {
	       gtk_window_set_icon_list(GTK_WINDOW(cwin->widget), list);
	       g_list_free(list);
	    }
      }

   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWGETICONLIST(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   C_widget *clist;

   CHECKCWID(cwin, GTK_IS_WINDOW);

   clist = _get_cwidget(ClipMachineMemory, GTK_WIDGET(gtk_window_get_icon_list(GTK_WINDOW(cwin->widget))));

   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &clist->obj);
   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWSETICON(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   C_object *cicon = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(cwin, GTK_IS_WINDOW);
   CHECKCOBJ(cicon, GDK_IS_PIXBUF(cicon->object));

   gtk_window_set_icon(GTK_WINDOW(cwin->widget), GDK_PIXBUF(cicon->object));
   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWGETICON(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   GdkPixbuf *pixbuf;
   C_object *cicon;

   CHECKCWID(cwin, GTK_IS_WINDOW);

   pixbuf = gtk_window_get_icon(GTK_WINDOW(cwin->widget));
   cicon = _register_object(ClipMachineMemory, pixbuf, GDK_TYPE_PIXBUF, NULL, NULL);
   if (cicon)
      _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cicon->obj);
   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWSETDEFAULTICONLIST(ClipMachine * ClipMachineMemory)
{
   ClipVar *items = _clip_spar(ClipMachineMemory, 1);

   CHECKOPT(1, ARRAY_type_of_ClipVarType);

   if (items->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
      {
	 GList *list = NULL;
	 unsigned short i;
	 ClipVar *item;
	 C_object *citem;
	 for (i = 0; i < items->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar; i++)
	    {
	       item = &items->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar[i];
	       if (item->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType || item->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
		  {
		     citem = _fetch_cobject(ClipMachineMemory, item);
		     CHECKCOBJ(citem, GDK_IS_PIXBUF(citem->object));
		     list = g_list_append(list, citem->object);
		  }
	    }
	 if (list)
	    {
	       gtk_window_set_default_icon_list(list);
	       g_list_free(list);
	    }
      }

   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWGETDEFAULTICONLIST(ClipMachine * ClipMachineMemory)
{
   C_widget *clist;

   clist = _get_cwidget(ClipMachineMemory, GTK_WIDGET(gtk_window_get_default_icon_list()));

   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &clist->obj);
   return 0;
}

int
clip_GTK_WINDOWLISTTOPLEVELS(ClipMachine * ClipMachineMemory)
{
   GList *list;

   list = gtk_window_list_toplevels();
   if (list)
      {
	 ClipVar *cv = RETPTR(ClipMachineMemory);
	 long l;
	 l = g_list_length(list);
	 _clip_array(ClipMachineMemory, cv, 1, &l);
	 for (l = 0; list; list = g_list_next(list), l++)
	    {
	       GtkWindow *win;
	       C_widget *cwin;
	       win = (GtkWindow *) list->data;
	       cwin = _list_get_cwidget(ClipMachineMemory, win);
	       if (!cwin)
		  cwin = _register_widget(ClipMachineMemory, GTK_WIDGET(win), NULL);
	       if (cwin)
		  _clip_aset(ClipMachineMemory, cv, &cwin->obj, 1, &l);
	    }
      }
   return 0;
}

int
clip_GTK_WINDOWGETACTIVE(ClipMachine * ClipMachineMemory)
{
   GList *list;

   list = gtk_window_list_toplevels();
   if (list)
      {
	 for (; list; list = g_list_next(list))
	    {
	       GtkWidget *win;
	       C_widget *cwin;
	       win = (GtkWidget *) list->data;
	       if (win->state == GTK_STATE_ACTIVE)
		  {
		     cwin = _list_get_cwidget(ClipMachineMemory, win);
		     if (!cwin)
			cwin = _register_widget(ClipMachineMemory, win, NULL);
		     if (cwin)
			_clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwin->obj);
		     break;
		  }
	    }
      }
   return 0;
}

int
clip_GTK_WINDOWADDMNEMONIC(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   guint keyval = _clip_parni(ClipMachineMemory, 2);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwin, GTK_IS_WINDOW);
   CHECKCWID(cwid, GTK_IS_WIDGET);

   gtk_window_add_mnemonic(GTK_WINDOW(cwin->widget), keyval, GTK_WIDGET(cwid->widget));

   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWREMOVEMNEMONIC(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   guint keyval = _clip_parni(ClipMachineMemory, 2);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwin, GTK_IS_WINDOW);
   CHECKCWID(cwid, GTK_IS_WIDGET);

   gtk_window_remove_mnemonic(GTK_WINDOW(cwin->widget), keyval, GTK_WIDGET(cwid->widget));

   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWMNEMONICACTIVATE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   guint keyval = _clip_parni(ClipMachineMemory, 2);
   GdkModifierType modifier = _clip_parni(ClipMachineMemory, 3);
   gboolean ret;

   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwin, GTK_IS_WINDOW);

   ret = gtk_window_mnemonic_activate(GTK_WINDOW(cwin->widget), keyval, modifier);

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWSETMNEMONICMODIFIER(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   GdkModifierType modifier = _clip_parni(ClipMachineMemory, 2);

   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwin, GTK_IS_WINDOW);

   gtk_window_set_mnemonic_modifier(GTK_WINDOW(cwin->widget), modifier);

   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWGETMNEMONICMODIFIER(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   GdkModifierType modifier;

   CHECKCWID(cwin, GTK_IS_WINDOW);

   modifier = gtk_window_get_mnemonic_modifier(GTK_WINDOW(cwin->widget));

   _clip_retni(ClipMachineMemory, modifier);
   return 0;
 err:
   return 1;
}

/* Presents a window to the user. This may mean raising the window in the */
/* stacking order, deiconifying it, moving it to the current desktop,     */
/* and/or giving it the keyboard focus, possibly dependent on the user's  */
/* platform, window manager, and preferences.                             */

int
clip_GTK_WINDOWPRESENT(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cwin, GTK_IS_WINDOW);

   gtk_window_present(GTK_WINDOW(cwin->widget));

   return 0;
 err:
   return 1;
}

/* Asks to iconify (i.e. minimize) the specified window. */
int
clip_GTK_WINDOWICONIFY(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cwin, GTK_IS_WINDOW);

   gtk_window_iconify(GTK_WINDOW(cwin->widget));

   return 0;
 err:
   return 1;
}

/* Asks to deiconify (i.e. unminimize) the specified window. */
int
clip_GTK_WINDOWDEICONIFY(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cwin, GTK_IS_WINDOW);

   gtk_window_deiconify(GTK_WINDOW(cwin->widget));

   return 0;
 err:
   return 1;
}

/* Asks to stick window, which means that it will appear on all user desktops. */
int
clip_GTK_WINDOWSTICK(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cwin, GTK_IS_WINDOW);

   gtk_window_stick(GTK_WINDOW(cwin->widget));

   return 0;
 err:
   return 1;
}

/* Asks to unstick window. */
int
clip_GTK_WINDOWUNSTICK(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cwin, GTK_IS_WINDOW);

   gtk_window_unstick(GTK_WINDOW(cwin->widget));

   return 0;
 err:
   return 1;
}

/* Asks to maximize window, so that it becomes full-screen. */
int
clip_GTK_WINDOWMAXIMIZE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cwin, GTK_IS_WINDOW);

   gtk_window_maximize(GTK_WINDOW(cwin->widget));

   return 0;
 err:
   return 1;
}

/* Asks to unmaximize window. */
int
clip_GTK_WINDOWUNMAXIMIZE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cwin, GTK_IS_WINDOW);

   gtk_window_unmaximize(GTK_WINDOW(cwin->widget));

   return 0;
 err:
   return 1;
}

/* Starts resizing a window. */
int
clip_GTK_WINDOWBEGINRESIZEDRAG(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   GdkWindowEdge edge = _clip_parni(ClipMachineMemory, 2);
   gint button = _clip_parni(ClipMachineMemory, 3);
   gint root_x = _clip_parni(ClipMachineMemory, 4);
   gint root_y = _clip_parni(ClipMachineMemory, 5);
   guint32 timestamp = _clip_parni(ClipMachineMemory, 6);

   CHECKCWID(cwin, GTK_IS_WINDOW);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);
   CHECKARG(5, NUMERIC_type_of_ClipVarType);
   CHECKARG(6, NUMERIC_type_of_ClipVarType);

   gtk_window_begin_resize_drag(GTK_WINDOW(cwin->widget), edge, button, root_x, root_y, timestamp);

   return 0;
 err:
   return 1;
}

/* Starts moving a window. */
int
clip_GTK_WINDOWBEGINMOVEDRAG(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   gint button = _clip_parni(ClipMachineMemory, 2);
   gint root_x = _clip_parni(ClipMachineMemory, 3);
   gint root_y = _clip_parni(ClipMachineMemory, 4);
   guint32 timestamp = _clip_parni(ClipMachineMemory, 5);

   CHECKCWID(cwin, GTK_IS_WINDOW);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);
   CHECKARG(5, NUMERIC_type_of_ClipVarType);

   gtk_window_begin_move_drag(GTK_WINDOW(cwin->widget), button, root_x, root_y, timestamp);

   return 0;
 err:
   return 1;
}

/* Resizes the window as if the user had done so, obeying geometry constraints. */
int
clip_GTK_WINDOWRESIZE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   gint width = _clip_parni(ClipMachineMemory, 2);
   gint height = _clip_parni(ClipMachineMemory, 3);

   CHECKCWID(cwin, GTK_IS_WINDOW);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);

   gtk_window_resize(GTK_WINDOW(cwin->widget), width, height);

   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWGETSIZE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   gint width;
   gint height;

   CHECKCWID(cwin, GTK_IS_WINDOW);

   gtk_window_get_size(GTK_WINDOW(cwin->widget), &width, &height);

   _clip_storni(ClipMachineMemory, width, 2, 0);
   _clip_storni(ClipMachineMemory, height, 3, 0);
   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWMOVE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   gint x = _clip_parni(ClipMachineMemory, 2);
   gint y = _clip_parni(ClipMachineMemory, 3);

   CHECKCWID(cwin, GTK_IS_WINDOW);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);

   gtk_window_move(GTK_WINDOW(cwin->widget), x, y);

   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWGETPOSITION(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   gint root_x;
   gint root_y;

   CHECKCWID(cwin, GTK_IS_WINDOW);

   gtk_window_get_position(GTK_WINDOW(cwin->widget), &root_x, &root_y);

   _clip_storni(ClipMachineMemory, root_x, 2, 0);
   _clip_storni(ClipMachineMemory, root_y, 3, 0);
   return 0;
 err:
   return 1;
}

/* Parses a standard X Window System geometry string - see the manual page    */
/* for X (type 'man X') for details on this. gtk_window_parse_geometry() does */
/* work on all GTK+ ports including Win32 but is primarily intended for       */
/* an X environment.                                                          */
/*                                                                            */
/* If either a size or a position can be extracted from the geometry string,  */
/* gtk_window_parse_geometry() returns TRUE and calls                         */
/* gtk_window_set_default_size() and/or gtk_window_move() to resize/move the  */
/* window.                                                                    */

int
clip_GTK_WINDOWPARSEGEOMETRY(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   const char *geometry = _clip_parc(ClipMachineMemory, 2);
   gboolean ret;

   CHECKCWID(cwin, GTK_IS_WINDOW);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   ret = gtk_window_parse_geometry(GTK_WINDOW(cwin->widget), geometry);

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWSETGEOMETRYHINTS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   ClipVar *mgeom = _clip_par(ClipMachineMemory, 3);
   GdkGeometry *geometry = 0;
   gint mask = 0;

   CHECKCWID(cwin, GTK_IS_WINDOW);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKOPT(2, MAP_type_of_ClipVarType);

   mask = _map_to_gdk_geometry(ClipMachineMemory, mgeom, geometry);
   gtk_window_set_geometry_hints(GTK_WINDOW(cwin->widget), GTK_WIDGET(cwid->widget), geometry, mask);

   return 0;
 err:
   return 1;
}

#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 4)
int
clip_GTK_WINDOWISACTIVE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cwin, GTK_IS_WINDOW);

   _clip_retl(ClipMachineMemory, gtk_window_is_active(GTK_WINDOW(cwin->widget)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWHASTOPLEVELFOCUS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cwin, GTK_IS_WINDOW);

   _clip_retl(ClipMachineMemory, gtk_window_has_toplevel_focus(GTK_WINDOW(cwin->widget)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWFULLSCREEN(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cwin, GTK_IS_WINDOW);

   gtk_window_fullscreen(GTK_WINDOW(cwin->widget));

   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWUNFULLSCREEN(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cwin, GTK_IS_WINDOW);

   gtk_window_unfullscreen(GTK_WINDOW(cwin->widget));

   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWSETKEEPABOVE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   gboolean set = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(cwin, GTK_IS_WINDOW);
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_window_set_keep_above(GTK_WINDOW(cwin->widget), set);

   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWSETKEEPBELOW(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   gboolean set = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(cwin, GTK_IS_WINDOW);
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_window_set_keep_below(GTK_WINDOW(cwin->widget), set);

   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWSETACCEPTFOCUS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   gboolean set = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(cwin, GTK_IS_WINDOW);
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_window_set_accept_focus(GTK_WINDOW(cwin->widget), set);

   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWGETACCEPTFOCUS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cwin, GTK_IS_WINDOW);

   _clip_retl(ClipMachineMemory, gtk_window_get_accept_focus(GTK_WINDOW(cwin->widget)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWSETDEFAULTICON(ClipMachine * ClipMachineMemory)
{
   C_object *cobj = _fetch_co_arg(ClipMachineMemory);

   CHECKCOBJ(cobj, GDK_IS_PIXBUF(cobj->object));

   gtk_window_set_default_icon(GDK_PIXBUF(cobj->object));

   return 0;
 err:
   return 1;
}
#endif
#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 6)
int
clip_GTK_WINDOWGETFOCUSONMAP(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cwid, GTK_IS_WINDOW);

   _clip_retl(ClipMachineMemory, gtk_window_get_focus_on_map(GTK_WINDOW(cwid->widget)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWSETFOCUSONMAP(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   gboolean enable = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(cwid, GTK_IS_WINDOW);
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_window_set_focus_on_map(GTK_WINDOW(cwid->widget), enable);

   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWGETICONNAME(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   gchar *name;

   CHECKCWID(cwid, GTK_IS_WINDOW);

   name = (gchar *) gtk_window_get_icon_name(GTK_WINDOW(cwid->widget));

   if (name)
      {
	 LOCALE_FROM_UTF(name);
	 _clip_retc(ClipMachineMemory, name);
	 FREE_TEXT(name);
      }

   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWSETDEFAULTICONNAME(ClipMachine * ClipMachineMemory)
{
   gchar *name = _clip_parc(ClipMachineMemory, 1);

   CHECKARG(1, CHARACTER_type_of_ClipVarType);

   gtk_window_set_default_icon_name((const gchar *) name);

   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWSETICONNAME(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   gchar *name = _clip_parc(ClipMachineMemory, 2);

   CHECKCWID(cwid, GTK_IS_WINDOW);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   gtk_window_set_icon_name(GTK_WINDOW(cwid->widget), (const gchar *) name);

   return 0;
 err:
   return 1;
}
#endif
