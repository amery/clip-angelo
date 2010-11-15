/*
    Copyright (C) 2001  ITK
    Author  : Alexey M. Tkachenko <alexey@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg.h"

#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "ci_clip-gtk.ch"
#include "ci_clip-gtk.h"
#include "ci_clip-gdk.h"

GtkType
_gdk_type_cursor()
{
   return GDK_OBJECT_CURSOR;
}

long
_clip_type_cursor()
{
   return GDK_OBJECT_CURSOR;
}

const char *
_clip_type_name_cursor()
{
   return "GDK_OBJECT_CURSOR";
}

int
clip_INIT___GDK(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_cursor, _clip_type_name_cursor, _gdk_type_cursor, NULL, NULL);
   return 0;
}

/**********************************************************/

/* Returns the width of the screen in pixels. */
int
clip_GDK_SCREENWIDTH(ClipMachine * ClipMachineMemory)
{
   _clip_retni(ClipMachineMemory, gdk_screen_width());
   return 0;
}

// Returns the height of the screen in pixels.
int
clip_GDK_SCREENHEIGHT(ClipMachine * ClipMachineMemory)
{
   _clip_retni(ClipMachineMemory, gdk_screen_height());
   return 0;
}

// Returns the width of the screen in millimeters.
// Note that on many X servers this value will not be correct.
int
clip_GDK_SCREENWIDTHMM(ClipMachine * ClipMachineMemory)
{
   _clip_retni(ClipMachineMemory, gdk_screen_width_mm());
   return 0;
}

// Returns the height of the screen in millimeters.
// Note that on many X servers this value will not be correct.
int
clip_GDK_SCREENHEIGHTMM(ClipMachine * ClipMachineMemory)
{
   _clip_retni(ClipMachineMemory, gdk_screen_height_mm());
   return 0;
}

/* Emits a short beep. */
int
clip_GDK_BEEP(ClipMachine * ClipMachineMemory)
{
   gdk_beep();
   return 0;
}

static int
gdk_object_cursor_destructor(ClipMachine * ClipMachineMemory, C_object * ccur)
{
   if (ccur && GDK_IS_CURSOR(ccur) && ccur->ref_count >= 0)
      gdk_cursor_destroy(GDK_CURSOR(ccur->object));
   return 0;
}

// Sets shape of mouse cursor
int
clip_GDK_WINDOWSETCURSOR(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   GdkCursorType cursor_type = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(cwin, GTK_IS_WIDGET);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);

   if (cwin && cwin->widget)
      {
	 GdkCursor *cursor = gdk_cursor_new(cursor_type);
	 C_object *ccur;

	 if (cursor)
	    {
	       ccur = _register_object(ClipMachineMemory, cursor, GDK_OBJECT_CURSOR, NULL, (coDestructor) gdk_object_cursor_destructor);
	       ccur->ref_count = 1;
	       _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &ccur->obj);
	    }
	 gdk_window_set_cursor(cwin->widget->window, cursor);
      }
   return 0;
 err:
   return 1;
}

/* Grabs the pointer (usually a mouse) so that all events are passed to this
 * application until the pointer is ungrabbed with gdk_pointer_ungrab(), or
 * the grab window becomes unviewable. This overrides any previous pointer
 * grab by this client.

 * Pointer grabs are used for operations which need complete control over mouse
 * events, even if the mouse leaves the application. For example in GTK+ it is
 * used for Drag and Drop, for dragging the handle in the GtkHPaned and GtkVPaned
 * widgets, and for resizing columns in GtkCList widgets.

 * Note that if the event mask of an X window has selected both button press and
 * button release events, then a button press event will cause an automatic pointer
 * grab until the button is released. X does this automatically since most
 * applications expect to receive button press and release events in pairs.
 * It is equivalent to a pointer grab on the window with owner_events set to TRUE. */
int
clip_GDK_POINTERGRAB(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   GdkWindow *win = NULL;
   gint owner_events = _clip_parl(ClipMachineMemory, 2);
   GdkEventMask event_mask = _clip_parnl(ClipMachineMemory, 3);
   C_widget *cconfine_to = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 4));
   GdkWindow *confine_to = NULL;
   C_object *ccursor = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 5));
   GdkCursor *cursor = NULL;

   CHECKCWID(cwin, GTK_IS_WIDGET);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT2(4, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cwin, GTK_IS_WIDGET);
   CHECKOPT2(5, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(ccursor, GDK_IS_CURSOR(ccursor));

   if (cwin && cwin->widget)
      win = cwin->widget->window;
   if (cconfine_to && cconfine_to->widget)
      confine_to = cconfine_to->widget->window;
   if (ccursor)
      cursor = GDK_CURSOR(ccursor->object);

   _clip_retni(ClipMachineMemory, gdk_pointer_grab(win, owner_events, event_mask, confine_to, cursor, GDK_CURRENT_TIME));

   return 0;
 err:
   return 1;
}

/* Ungrabs the pointer, if it is grabbed by this application. */
int
clip_GDK_POINTERUNGRAB(ClipMachine * ClipMachineMemory)
{
   gdk_pointer_ungrab(GDK_CURRENT_TIME);
   return 0;
}

/* Returns TRUE if the pointer is currently grabbed by this application.

 * Note that the return value is not completely reliable since the X server
 * may automatically ungrab the pointer, without informing the application,
 * if the grab window becomes unviewable. It also does not take passive
 * pointer grabs into account. */
int
clip_GDK_POINTERISGRABBED(ClipMachine * ClipMachineMemory)
{
   _clip_retl(ClipMachineMemory, gdk_pointer_is_grabbed());
   return 0;
}

/* Grabs the keyboard so that all events are passed to this application until
 * the keyboard is ungrabbed with gdk_keyboard_ungrab().
 * This overrides any previous keyboard grab by this client. */
int
clip_GDK_KEYBOARDGRAB(ClipMachine * ClipMachineMemory)
{
   C_widget *cwin = _fetch_cw_arg(ClipMachineMemory);
   GdkWindow *win = NULL;
   gint owner_events = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(cwin, GTK_IS_WIDGET);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);

   if (cwin && cwin->widget)
      win = cwin->widget->window;

   gdk_keyboard_grab(win, owner_events, GDK_CURRENT_TIME);

   return 0;
 err:
   return 1;
}

/* Ungrabs the keyboard, if it is grabbed by this application. */
int
clip_GDK_KEYBOARDUNGRAB(ClipMachine * ClipMachineMemory)
{
   gdk_keyboard_ungrab(GDK_CURRENT_TIME);
   return 0;
}

/* Disables the keyboard auto-repeat mode.
 * This should be used with care as it may affect other applications. */
int
clip_GDK_KEYREPEATDISABLE(ClipMachine * ClipMachineMemory)
{
   gdk_key_repeat_disable();
   return 0;
}

/* Restores the keyboard auto-repeat mode to its state when the application was started. */
int
clip_GDK_KEYREPEATRESTORE(ClipMachine * ClipMachineMemory)
{
   gdk_key_repeat_restore();
   return 0;
}

/* Converts a key value into a symbolic name. The names are the same as those
   in the <clip-gdk.ch> header file but without the leading "GDK_". */
int
clip_GDK_KEYVALNAME(ClipMachine * ClipMachineMemory)
{
   guint keyval = INT_OPTION(ClipMachineMemory, 1, 0);
   _clip_retc(ClipMachineMemory, gdk_keyval_name(keyval));
   return 0;
}

/* Converts a key name to a key value. */
int
clip_GDK_KEYVALFROMNAME(ClipMachine * ClipMachineMemory)
{
   gchar *keyval_name = CHAR_OPTION(ClipMachineMemory, 1, "");
   _clip_retni(ClipMachineMemory, gdk_keyval_from_name(keyval_name));
   return 0;
}

/* Returns TRUE if the given key value is in upper case. */
int
clip_GDK_KEYVALISUPPER(ClipMachine * ClipMachineMemory)
{
   guint keyval = INT_OPTION(ClipMachineMemory, 1, 0);
   _clip_retl(ClipMachineMemory, gdk_keyval_is_upper(keyval));
   return 0;
}

/* Returns TRUE if the given key value is in lower case. */
int
clip_GDK_KEYVALISLOWER(ClipMachine * ClipMachineMemory)
{
   guint keyval = INT_OPTION(ClipMachineMemory, 1, 0);
   _clip_retl(ClipMachineMemory, gdk_keyval_is_lower(keyval));
   return 0;
}

/* Converts a key value to upper case, if applicable. */
int
clip_GDK_KEYVALTOUPPER(ClipMachine * ClipMachineMemory)
{
   guint keyval = INT_OPTION(ClipMachineMemory, 1, 0);
   _clip_retl(ClipMachineMemory, gdk_keyval_to_upper(keyval));
   return 0;
}

/* Converts a key value to lower case, if applicable. */
int
clip_GDK_KEYVALTOLOWER(ClipMachine * ClipMachineMemory)
{
   guint keyval = INT_OPTION(ClipMachineMemory, 1, 0);
   _clip_retl(ClipMachineMemory, gdk_keyval_to_lower(keyval));
   return 0;
}
