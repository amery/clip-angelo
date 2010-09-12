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
static    gint
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

static SignalTable window_signals[] = {
   {"set-focus", GSF(handle_set_focus_signal), ESF(object_emit_signal),
    GTK_SET_FOCUS_SIGNAL},
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
   _wtype_table_put(_clip_type_window, _clip_type_name_window, _gtk_type_window, _gtk_type_container, window_signals);
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
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   char     *title = _clip_parc(ClipMachineMemory, 2);

   gint      typ = INT_OPTION(ClipMachineMemory, 3, 0);

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

// Set ICON NAME property for window
int
clip_GTK_WINDOWSETICONNAME(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);

   gchar    *name = _clip_parc(ClipMachineMemory, 2);

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

   gchar    *title = _clip_parc(ClipMachineMemory, 2);

   CHECKCWID(cwin, GTK_IS_WINDOW);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);
   LOCALE_TO_UTF(title);
   gtk_window_set_title(GTK_WINDOW(cwin->widget), title);
   FREE_TEXT(title);
   return 0;
 err:
   return 1;
}

/* Sets resize policy */
int
clip_GTK_WINDOWSETPOLICY(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);

   gboolean  AutoShrink = FALSE, AllowShrink = TRUE, AllowGrow = TRUE;

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

   gboolean  modal = _clip_parl(ClipMachineMemory, 2);

   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      modal = TRUE;
   CHECKCWID(cwin, GTK_IS_WINDOW);
   gtk_window_set_modal(GTK_WINDOW(cwin->widget), modal);
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

/* Sets default widget in window */
int
clip_GTK_WINDOWSETDEFAULT(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwin, GTK_IS_WINDOW);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   GTK_WINDOW(cwin->widget)->default_widget = cwid->widget;
   return 0;
 err:
   return 1;
}

/* Sets default size for window */
int
clip_GTK_WINDOWSETDEFAULTSIZE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);

   gint      width = _clip_parni(ClipMachineMemory, 2);

   gint      height = _clip_parni(ClipMachineMemory, 3);

   GtkArg    arg;

   CHECKCWID(cwin, GTK_IS_WINDOW);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
    {
       arg.type = GTK_TYPE_INT;
       arg.name = "width";
       gtk_widget_get(cwin->widget, &arg);
       if (arg.d.int_data == -1)
	  arg.d.int_data = cwin->widget->requisition.width;
       width = arg.d.int_data;
    }
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
    {
       arg.type = GTK_TYPE_INT;
       arg.name = "height";
       gtk_widget_get(cwin->widget, &arg);
       if (arg.d.int_data == -1)
	  arg.d.int_data = cwin->widget->requisition.height;
       height = arg.d.int_data;
    }
   gtk_window_set_default_size(GTK_WINDOW(cwin->widget), width, height);
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

   gint      position = _clip_parni(ClipMachineMemory, 2);

   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwin, GTK_IS_WINDOW);
   gtk_window_set_position(GTK_WINDOW(cwin->widget), position);
   return 0;
 err:
   return 1;
}

int
clip_GTK_WINDOWADDACCELGROUP(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);

   C_object *caccg = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwin, GTK_IS_WINDOW);
   CHECKCOBJ(caccg, GTK_IS_ACCEL_GROUP(caccg));

   gtk_window_add_accel_group(GTK_WINDOW(cwin->widget), GTK_ACCEL_GROUP(caccg->object));

   return 0;
 err:
   return 1;
}
