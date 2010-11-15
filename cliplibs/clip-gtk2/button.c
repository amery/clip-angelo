/*
    Copyright (C) 2001  ITK
    Author  : Alexey M. Tkachenko <alexey@itk.ru>
              Kornilova E.V. <alena@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg2.h"

#include <gtk/gtk.h>

#include "ci_clip-gtk2.ch"
#include "ci_clip-gtk2.h"

/**********************************************************/
/* Signals table */
static SignalTable button_signals[] = {
   {"activate", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_ACTIVATE_SIGNAL},
  /* Emitted when the button is initially pressed */
   {"pressed", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_PRESSED_SIGNAL},
  /* Emitted when a button which is pressed is released,
   * no matter where the mouse cursor is                 */
   {"released", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_RELEASED_SIGNAL},
  /* Emitted when a button clicked on by the mouse and the cursor stays on
   * the button. If the cursor is not on the button when the
   * mouse button is released, the signal is not emitted */
  //{"clicked",   GSF( handle_clicked_signal ),   ESF( object_emit_signal ), GTK_CLICKED_SIGNAL},
   {"clicked", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_CLICKED_SIGNAL},
  /* Emitted when the mouse cursor enters the region of the button */
   {"enter", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_ENTER_SIGNAL},
  /* Emitted when the mouse cursor leaves the region of the button */
   {"leave", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_LEAVE_SIGNAL},
   {"", NULL, NULL, 0}
};

/* Register widget signals table in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_button()
{
   return GTK_TYPE_BUTTON;
}

long
_clip_type_button()
{
   return GTK_WIDGET_BUTTON;
}

const char *
_clip_type_name_button()
{
   return "GTK_WIDGET_BUTTON";
}

int
clip_INIT___BUTTON(ClipMachine * ClipMachineMemory)
{
  //_wtype_table_put(_clip_type_button, _clip_type_name_button, _gtk_type_button,  _gtk_type_container, button_signals);
   _wtype_table_put(_clip_type_button, _clip_type_name_button, _gtk_type_button, _gtk_type_bin, button_signals);
   return 0;
}

/**********************************************************/

/**** BUTTON constructor ****/
int
clip_GTK_BUTTONNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   char     *title = _clip_parc(ClipMachineMemory, 2);

   char     *pchar = _clip_parc(ClipMachineMemory, 3);

   GtkWidget *wid = NULL, *label = NULL;

   C_widget *cwid, *clabel = NULL;

   guint     accel_key = 0;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT(3, CHARACTER_type_of_ClipVarType);

   if (pchar)
    {
       char     *pc;

       for (pc = title; pc && *pc; pc++)
	  if (*pc == *pchar)
	     *pc = '_';
    }
   if (_clip_parinfo(ClipMachineMemory, 2) == CHARACTER_type_of_ClipVarType)
    {
       LOCALE_TO_UTF(title) wid = gtk_button_new_with_label(title);
       label = GTK_BIN(&(GTK_BUTTON(wid)->bin))->child;
       if (pchar)
	  accel_key = gtk_label_parse_uline(GTK_LABEL(label), title);
       FREE_TEXT(title);
    }
   else
    {
       wid = gtk_button_new();
    }
   if (!wid)
      goto err;

   cwid = _register_widget(ClipMachineMemory, wid, cv);

   if (label)
      clabel = _register_widget(ClipMachineMemory, label, NULL);
   if (clabel)
      _clip_madd(ClipMachineMemory, &cwid->obj, HASH_LABEL, &clabel->obj);

   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   _clip_mputn(ClipMachineMemory, &cwid->obj, HASH_ACCELKEY, accel_key);

   return 0;
 err:
   return 1;
}

/* Creates a new GtkButton containing the image and text from a stock item.      */
/* Some stock ids have preprocessor macros like GTK_STOCK_OK and GTK_STOCK_APPLY.*/
int
clip_GTK_BUTTONNEWFROMSTOCK(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   const char *stock_id = _clip_parc(ClipMachineMemory, 2);

   GtkWidget *wid;

   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);

   wid = gtk_button_new_from_stock(stock_id);

   if (!wid)
      goto err;

   cwid = _register_widget(ClipMachineMemory, wid, cv);

   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

/* Creates a new GtkButton containing a label. If characters in label are  */
/* preceded by an underscore, they are underlined. If you need a literal   */
/* underscore character in a label, use '__' (two underscores).            */
/* The first underlined character represents a keyboard accelerator called */
/* a mnemonic. Pressing Alt and that key activates the button.             */
int
clip_GTK_BUTTONNEWWITHMNEMONIC(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   char     *label = _clip_parc(ClipMachineMemory, 2);

   char     *pchar = _clip_parc(ClipMachineMemory, 3);

   GtkWidget *wid;

   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT(3, CHARACTER_type_of_ClipVarType);

   if (pchar)
    {
       char     *pc;

       for (pc = label; pc && *pc; pc++)
	  if (*pc == *pchar)
	     *pc = '_';
    }

   LOCALE_TO_UTF(label);
   wid = gtk_button_new_with_mnemonic((const char *) label);
   FREE_TEXT(label);

   if (!wid)
      goto err;

   cwid = _register_widget(ClipMachineMemory, wid, cv);

   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

/* Emits a GtkButton::pressed signal to the given GtkButton. */
int
clip_GTK_BUTTONPRESSED(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cbtn, GTK_IS_BUTTON);
   gtk_button_pressed(GTK_BUTTON(cbtn->widget));
   return 0;
 err:
   return 1;
}

/* Emits a GtkButton::released signal to the given GtkButton. */
int
clip_GTK_BUTTONRELEASED(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cbtn, GTK_IS_BUTTON);
   gtk_button_released(GTK_BUTTON(cbtn->widget));
   return 0;
 err:
   return 1;
}

/* Emits a GtkButton::clicked signal to the given GtkButton. */
int
clip_GTK_BUTTONCLICKED(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cbtn, GTK_IS_BUTTON);
   gtk_button_clicked(GTK_BUTTON(cbtn->widget));
   return 0;
 err:
   return 1;
}

/* Emits a GtkButton::enter signal to the given GtkButton. */
int
clip_GTK_BUTTONENTER(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cbtn, GTK_IS_BUTTON);
   gtk_button_enter(GTK_BUTTON(cbtn->widget));
   return 0;
 err:
   return 1;
}

/* Emits a GtkButton::leave signal to the given GtkButton. */
int
clip_GTK_BUTTONLEAVE(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cbtn, GTK_IS_BUTTON);
   gtk_button_leave(GTK_BUTTON(cbtn->widget));
   return 0;
 err:
   return 1;
}

/* Sets the relief style of the edges of the given Button widget.
 * Three styles exist, GTK_RELIEF_NORMAL,GTK_RELIEF_HALF, GTK_RELIEF_NONE.
 * The default style is, as one can guess, GTK_RELIEF_NORMAL. */
int
clip_GTK_BUTTONSETRELIEF(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   int       relief = _clip_parni(ClipMachineMemory, 2);

   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      relief = GTK_RELIEF_NORMAL;
   CHECKCWID(cbtn, GTK_IS_BUTTON);
   gtk_button_set_relief(GTK_BUTTON(cbtn->widget), relief);
   return 0;
 err:
   return 1;
}

/* Returns the current relief style of the given Button. */
int
clip_GTK_BUTTONGETRELIEF(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cbtn, GTK_IS_BUTTON);
   _clip_retni(ClipMachineMemory, gtk_button_get_relief(GTK_BUTTON(cbtn->widget)));
   return 0;
 err:
   return 1;
}

int
clip_GTK_BUTTONSETSTYLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   ClipVar  *mstyle = _clip_spar(ClipMachineMemory, 2);

   GtkStyle *style;

   GtkWidget *wid;

   CHECKCWID(cbtn, GTK_IS_BUTTON);
   CHECKARG(2, MAP_type_of_ClipVarType);
  /*alena - I'm think it is bad idea: wid = GTK_BIN(&(GTK_BUTTON(cbtn->widget)->bin))->child; */
  /* alena - style for button now setting! */
   wid = cbtn->widget;
   style = gtk_style_copy(wid->style);
   _map_to_style(ClipMachineMemory, mstyle, style);
   gtk_widget_set_style(wid, style);

   wid = GTK_BIN(&(GTK_BUTTON(cbtn->widget)->bin))->child;
   gtk_widget_set_style(wid, style);
   return 0;
 err:
   return 1;
}

int
clip_GTK_BUTTONMODIFYFONT(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   gchar    *font = _clip_parc(ClipMachineMemory, 2);

   GtkWidget *wid;

   CHECKCWID(cbtn, GTK_IS_BUTTON);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   wid = GTK_BIN(&(GTK_BUTTON(cbtn->widget)->bin))->child;
  //wid = GTK_WIDGET(cbtn->widget);
   gtk_widget_modify_font(wid, pango_font_description_from_string(font));
   return 0;
 err:
   return 1;
}

/* Modify FG for a button                                                   */
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
clip_GTK_BUTTONMODIFYFG(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   ClipVar  *mstyle = _clip_par(ClipMachineMemory, 2);

   GtkStateType state = _clip_parni(ClipMachineMemory, 3);

   GdkColor  fgcolor;

   double    colors[4];

   GtkWidget *wid;

   CHECKARG2(2, MAP_type_of_ClipVarType, ARRAY_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cbtn, GTK_IS_BUTTON);

   _map_get_colors(ClipMachineMemory, mstyle, colors);
   fgcolor.red = colors[0];
   fgcolor.green = colors[1];
   fgcolor.blue = colors[2];
   fgcolor.pixel = colors[3];

   if (_clip_parinfo(ClipMachineMemory, 3) != NUMERIC_type_of_ClipVarType)
      state = GTK_STATE_NORMAL;

   wid = GTK_BIN(&(GTK_BUTTON(cbtn->widget)->bin))->child;
  //wid = GTK_WIDGET(cbtn->widget);
   gtk_widget_modify_fg(wid, state, &fgcolor);
   return 0;
 err:
   return 1;
}

/* Modify BG for a button */
int
clip_GTK_BUTTONMODIFYBG(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   ClipVar  *mstyle = _clip_par(ClipMachineMemory, 2);

   GtkStateType state = _clip_parni(ClipMachineMemory, 3);

   GdkColor  bgcolor;

   double    colors[4];

   GtkWidget *wid;

   CHECKARG2(2, MAP_type_of_ClipVarType, ARRAY_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cbtn, GTK_IS_BUTTON);

   _map_get_colors(ClipMachineMemory, mstyle, colors);
   bgcolor.red = colors[0];
   bgcolor.green = colors[1];
   bgcolor.blue = colors[2];
   bgcolor.pixel = colors[3];

   if (_clip_parinfo(ClipMachineMemory, 3) != NUMERIC_type_of_ClipVarType)
      state = GTK_STATE_NORMAL;

  //wid = GTK_BIN(&(GTK_BUTTON(cbtn->widget)->bin))->child;
   wid = GTK_WIDGET(cbtn->widget);
   gtk_widget_modify_bg(wid, state, &bgcolor);
   return 0;
 err:
   return 1;
}

/* Modify BASE for a button */
int
clip_GTK_BUTTONMODIFYBASE(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   ClipVar  *mstyle = _clip_par(ClipMachineMemory, 2);

   GtkStateType state = _clip_parni(ClipMachineMemory, 3);

   GdkColor  basecolor;

   double    colors[4];

   GtkWidget *wid;

   CHECKARG2(2, MAP_type_of_ClipVarType, ARRAY_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cbtn, GTK_IS_BUTTON);

   _map_get_colors(ClipMachineMemory, mstyle, colors);
   basecolor.red = colors[0];
   basecolor.green = colors[1];
   basecolor.blue = colors[2];
   basecolor.pixel = colors[3];

   if (_clip_parinfo(ClipMachineMemory, 3) != NUMERIC_type_of_ClipVarType)
      state = GTK_STATE_NORMAL;

  //wid = GTK_BIN(&(GTK_BUTTON(cbtn->widget)->bin))->child;
   wid = GTK_WIDGET(cbtn->widget);
   gtk_widget_modify_base(wid, state, &basecolor);
   return 0;
 err:
   return 1;
}

/* Modify TEXT for a button */
int
clip_GTK_BUTTONMODIFYTEXT(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   ClipVar  *mstyle = _clip_par(ClipMachineMemory, 2);

   GtkStateType state = _clip_parni(ClipMachineMemory, 3);

   GdkColor  textcolor;

   double    colors[4];

   GtkWidget *wid;

   CHECKARG2(2, MAP_type_of_ClipVarType, ARRAY_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cbtn, GTK_IS_BUTTON);

   _map_get_colors(ClipMachineMemory, mstyle, colors);
   textcolor.red = colors[0];
   textcolor.green = colors[1];
   textcolor.blue = colors[2];
   textcolor.pixel = colors[3];
   if (_clip_parinfo(ClipMachineMemory, 3) != NUMERIC_type_of_ClipVarType)
      state = GTK_STATE_NORMAL;

  //wid = GTK_BIN(&(GTK_BUTTON(cbtn->widget)->bin))->child;
   wid = GTK_WIDGET(cbtn->widget);
   gtk_widget_modify_text(wid, state, &textcolor);
   return 0;
 err:
   return 1;
}

int
clip_GTK_BUTTONSETLABEL(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   char     *text = _clip_parc(ClipMachineMemory, 2);

   char     *pchar = _clip_parc(ClipMachineMemory, 3);

   GtkWidget *label;

   guint     accel_key = 0;

   CHECKCWID(cbtn, GTK_IS_BUTTON);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT(3, CHARACTER_type_of_ClipVarType);

   if (pchar)
    {
       char     *pc;

       for (pc = text; pc && *pc; pc++)
	  if (*pc == *pchar)
	     *pc = '_';
    }
   if (_clip_parinfo(ClipMachineMemory, 2) == CHARACTER_type_of_ClipVarType)
    {
       LOCALE_TO_UTF(text);
       gtk_button_set_label(GTK_BUTTON(cbtn->widget), text);
       label = GTK_BIN(&(GTK_BUTTON(cbtn->widget)->bin))->child;
       if (pchar)
	  accel_key = gtk_label_parse_uline(GTK_LABEL(label), text);
      //gtk_label_set_text(GTK_LABEL(GTK_BIN(&(GTK_BUTTON(cbtn->widget)->bin))->child), text);
       FREE_TEXT(text);
       _clip_mputn(ClipMachineMemory, &cbtn->obj, HASH_ACCELKEY, accel_key);
    }

   return 0;
 err:
   return 1;
}

int
clip_GTK_BUTTONGETLABEL(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   gchar    *text;

   CHECKCWID(cbtn, GTK_IS_BUTTON);

   text = (gchar *) (gtk_button_get_label(GTK_BUTTON(cbtn->widget)));

   LOCALE_FROM_UTF(text) _clip_retc(ClipMachineMemory, text);
   FREE_TEXT(text);

   return 0;
 err:
   return 1;
}

/* If true, an underline in the text of the button label indicates the */
/* next character should be used for the mnemonic accelerator key.     */

int
clip_GTK_BUTTONSETUSEUNDERLINE(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   gboolean  use_underline = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(cbtn, GTK_IS_BUTTON);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);

   gtk_button_set_use_underline(GTK_BUTTON(cbtn->widget), use_underline);

   return 0;
 err:
   return 1;
}

int
clip_GTK_BUTTONGETUSEUNDERLINE(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   gboolean  use_underline;

   CHECKCWID(cbtn, GTK_IS_BUTTON);

   use_underline = gtk_button_get_use_underline(GTK_BUTTON(cbtn->widget));

   _clip_retl(ClipMachineMemory, use_underline);
   return 0;
 err:
   return 1;
}

/* If true, the label set on the button is used as a stock id to select */
/* the stock item for the button.                                       */
int
clip_GTK_BUTTONSETUSESTOCK(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   gboolean  use_stock = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(cbtn, GTK_IS_BUTTON);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);

   gtk_button_set_use_stock(GTK_BUTTON(cbtn->widget), use_stock);

   return 0;
 err:
   return 1;
}

int
clip_GTK_BUTTONGETUSESTOCK(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   gboolean  use_stock;

   CHECKCWID(cbtn, GTK_IS_BUTTON);

   use_stock = gtk_button_get_use_stock(GTK_BUTTON(cbtn->widget));

   _clip_retl(ClipMachineMemory, use_stock);
   return 0;
 err:
   return 1;
}

#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 4)

int
clip_GTK_BUTTONSETFOCUSONCLICK(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   gboolean  focus_on_click = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(cbtn, GTK_IS_BUTTON);
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_button_set_focus_on_click(GTK_BUTTON(cbtn->widget), focus_on_click);

   return 0;
 err:
   return 1;
}

int
clip_GTK_BUTTONGETFOCUSONCLICK(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cbtn, GTK_IS_BUTTON);

   _clip_retl(ClipMachineMemory, gtk_button_get_focus_on_click(GTK_BUTTON(cbtn->widget)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_BUTTONGETALIGNMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);

   gfloat    xalign, yalign;

   CHECKCWID(cbtn, GTK_IS_BUTTON);

   gtk_button_get_alignment(GTK_BUTTON(cbtn->widget), &xalign, &yalign);

   _clip_stornd(ClipMachineMemory, xalign, 2, 0);
   _clip_stornd(ClipMachineMemory, yalign, 3, 0);
   return 0;
 err:
   return 1;
}
#endif
