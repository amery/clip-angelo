/*
    Copyright (C) 2001  ITK
    Author  : Alexey M. Tkachenko <alexey@itk.ru>
    Author  : Kornilova E.V. <alena@itk.ru>
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

/*************** SIGNALS for InputDialog  *****************/

/* Signal handlers */
static    gint
handle_input_dialog_signal(GtkInputDialog * dialog, gint deviceid, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_DEVICEID, deviceid);
   INVOKESIGHANDLER(GTK_WIDGET(dialog), cs, cv);
}

static    gint
emit_input_dialog_signal(C_widget * cidial, const gchar * signal_name)
{
   ClipMachine *ClipMachineMemory = cidial->cmachine;

   gint      deviceid = _clip_parni(ClipMachineMemory, 3);

   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   gtk_signal_emit_by_name(GTK_OBJECT(cidial->widget), signal_name, deviceid, cidial);
   return 0;
 err:
   return 1;
}

/* Signals table */
static SignalTable input_dialog_signals[] = {
   {"enable-device", GSF(handle_input_dialog_signal),
    ESF(emit_input_dialog_signal), GTK_ENABLE_DEVICE_SIGNAL},
   {"disable-device", GSF(handle_input_dialog_signal),
    ESF(emit_input_dialog_signal), GTK_DISABLE_DEVICE_SIGNAL},
   {"", NULL, NULL, 0}
};

static    gint
handle_response_signal(GtkDialog * dialog, gint arg1, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_RESPONSE_ID, arg1);
   INVOKESIGHANDLER(GTK_WIDGET(dialog), cs, cv);
}

/* Signals table */
static SignalTable dialog_signals[] = {
   {"close", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_CLOSE_SIGNAL},
   {"response", GSF(handle_response_signal), ESF(object_emit_signal),
    GTK_RESPONSE_SIGNAL},
   {"", NULL, NULL, 0}
};

/**********************************************************/

/* Register dialog in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_dialog()
{
   return GTK_TYPE_DIALOG;
}

long
_clip_type_dialog()
{
   return GTK_WIDGET_DIALOG;
}

const char *
_clip_type_name_dialog()
{
   return "GTK_WIDGET_DIALOG";
}

/* Register Input Dialog in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_input_dialog()
{
   return GTK_TYPE_INPUT_DIALOG;
}

long
_clip_type_input_dialog()
{
   return GTK_WIDGET_INPUT_DIALOG;
}

const char *
_clip_type_name_input_dialog()
{
   return "GTK_WIDGET_INPUT_DIALOG";
}

/* Register Message Dialog in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_message_dialog()
{
   return GTK_TYPE_MESSAGE_DIALOG;
}

long
_clip_type_message_dialog()
{
   return GTK_WIDGET_MESSAGE_DIALOG;
}

const char *
_clip_type_name_message_dialog()
{
   return "GTK_WIDGET_MESSAGE_DIALOG";
}

int
clip_INIT___DIALOG(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_dialog, _clip_type_name_dialog, _gtk_type_dialog, _gtk_type_window, dialog_signals);
   _wtype_table_put(_clip_type_input_dialog, _clip_type_name_input_dialog,
		    _gtk_type_input_dialog, _gtk_type_dialog, input_dialog_signals);
   _wtype_table_put(_clip_type_message_dialog, _clip_type_name_message_dialog,
		    _gtk_type_message_dialog, _gtk_type_dialog, NULL);
   return 0;
}

/**********************************************************/

/****  DIALOG constructor ****/
int
clip_GTK_DIALOGNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   char     *title = _clip_parc(ClipMachineMemory, 2);

   GtkWidget *wid = NULL;

   C_widget *cwid;

   C_widget *cvbox, *carea;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);

   wid = gtk_dialog_new();
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
   cvbox = _register_widget(ClipMachineMemory, GTK_DIALOG(wid)->vbox, NULL);
   carea = _register_widget(ClipMachineMemory, GTK_DIALOG(wid)->action_area, NULL);

   if (cvbox)
      _clip_madd(ClipMachineMemory, &cwid->obj, HASH_VBOX, &cvbox->obj);
   if (carea)
      _clip_madd(ClipMachineMemory, &cwid->obj, HASH_ACTIONAREA, &carea->obj);

   cwid->accel_group = gtk_accel_group_new();
   gtk_window_add_accel_group(GTK_WINDOW(wid), cwid->accel_group);

   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

int
clip_GTK_DIALOGNEWWITHBUTTONS(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   char     *title = _clip_parc(ClipMachineMemory, 2);

   C_widget *cpwin = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   GtkDialogFlags flags = INT_OPTION(ClipMachineMemory, 4, 0);

   GtkWidget *wid = NULL;

   C_widget *cwid;

   C_widget *cvbox, *carea;

   gint      i, n = _clip_parinfo(ClipMachineMemory, 0) - 4;

   gchar    *stock[n];

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT2(3, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cpwin, GTK_WINDOW);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);

   memset(stock, 0, sizeof(stock));
   for (i = 0; i < n; i++)
    {
       CHECKOPT(i + 4, CHARACTER_type_of_ClipVarType);
       if (_clip_parinfo(ClipMachineMemory, i + 4) == UNDEF_type_of_ClipVarType)
	  break;
       stock[i] = _clip_parc(ClipMachineMemory, i + 4);
    }
   LOCALE_TO_UTF(title);
   wid = gtk_dialog_new_with_buttons(title,
				     (cpwin !=
				      NULL) ? GTK_WINDOW(cpwin->widget) : NULL,
				     flags, stock[0], stock[1], stock[2],
				     stock[3], stock[4], stock[5], stock[6],
				     stock[7], stock[8], stock[9], stock[10],
				     stock[11], stock[12], stock[13],
				     stock[14], stock[15], stock[16], stock[17], stock[18], stock[19]);
   if (!wid)
      goto err;
  //cwid = (C_widget*)calloc( 1, sizeof(C_widget) );
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   cvbox = _register_widget(ClipMachineMemory, GTK_DIALOG(wid)->vbox, NULL);
   carea = _register_widget(ClipMachineMemory, GTK_DIALOG(wid)->action_area, NULL);

   if (cvbox)
      _clip_madd(ClipMachineMemory, &cwid->obj, HASH_VBOX, &cvbox->obj);
   if (carea)
      _clip_madd(ClipMachineMemory, &cwid->obj, HASH_ACTIONAREA, &carea->obj);

   cwid->accel_group = gtk_accel_group_new();
   gtk_window_add_accel_group(GTK_WINDOW(wid), cwid->accel_group);

   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   FREE_TEXT(title);
   return 0;
 err:
   return 1;
}

/****  ------------------ ****/

/****  INPUT DIALOG constructor ****/
int
clip_GTK_INPUTDIALOGNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   char     *title = _clip_parc(ClipMachineMemory, 2);

   GtkWidget *wid = NULL;

   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);

   wid = gtk_input_dialog_new();
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

   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

/****  ------------------ ****/

/* Runs dialog and returns one of response dialog types  */
int
clip_GTK_DIALOGRUN(ClipMachine * ClipMachineMemory)
{
   C_widget *cdialog = _fetch_cw_arg(ClipMachineMemory);

   gint      ret;

   CHECKCWID(cdialog, GTK_IS_DIALOG);

   ret = gtk_dialog_run(GTK_DIALOG(cdialog->widget));
   _clip_retni(ClipMachineMemory, ret);

   return 0;
 err:
   return 1;
}

/* Emit  response signal  */
int
clip_GTK_DIALOGRESPONSE(ClipMachine * ClipMachineMemory)
{
   C_widget *cdialog = _fetch_cw_arg(ClipMachineMemory);

   gint      response_id = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(cdialog, GTK_IS_DIALOG);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_dialog_response(GTK_DIALOG(cdialog->widget), response_id);

   return 0;
 err:
   return 1;
}

/* Adds a button with the given text
*(or a stock button, if button_text is a stock ID)  */
int
clip_GTK_DIALOGADDBUTTON(ClipMachine * ClipMachineMemory)
{
   C_widget *cdialog = _fetch_cw_arg(ClipMachineMemory);

   gchar    *text = _clip_parc(ClipMachineMemory, 2);

   gint      response_id = _clip_parni(ClipMachineMemory, 3);

   CHECKCWID(cdialog, GTK_IS_DIALOG);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);

   LOCALE_TO_UTF(text);
   gtk_dialog_add_button(GTK_DIALOG(cdialog->widget), text, response_id);
   FREE_TEXT(text);

   return 0;
 err:
   return 1;
}

/* Adds a buttons with the given text
*(or a stock button, if button_text is a stock ID)  */
int
clip_GTK_DIALOGADDBUTTONS(ClipMachine * ClipMachineMemory)
{
   C_widget *cdialog = _fetch_cw_arg(ClipMachineMemory);

   gint      n = _clip_parinfo(ClipMachineMemory, 0) - 1;

   gchar    *text[20];

   gint      response_id[20];

   gint      j, i;

   CHECKCWID(cdialog, GTK_IS_DIALOG);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);

   memset(text, 0, sizeof(text));
   memset(response_id, 0, sizeof(response_id));
   for (i = 0, j = 0; j < n; j += 2, i++)
    {
       CHECKOPT(i, CHARACTER_type_of_ClipVarType);
       if (_clip_parinfo(ClipMachineMemory, i) == UNDEF_type_of_ClipVarType)
	{
	   n--;
	   n = n / 2;
	   break;
	}
       CHECKOPT(i + 1, NUMERIC_type_of_ClipVarType);
       text[i] = _clip_parc(ClipMachineMemory, i);
       response_id[i] = _clip_parni(ClipMachineMemory, i + 1);
       LOCALE_TO_UTF(text[i]);
    }
   gtk_dialog_add_buttons(GTK_DIALOG(cdialog->widget),
			  text[0], response_id[0], text[1], response_id[1],
			  text[2], response_id[2], text[3], response_id[3],
			  text[4], response_id[4], text[5], response_id[5],
			  text[6], response_id[6], text[7], response_id[7],
			  text[8], response_id[8], text[9], response_id[9],
			  text[10], response_id[10], text[11],
			  response_id[11], text[12], response_id[12],
			  text[13], response_id[13], text[14],
			  response_id[14], text[15], response_id[15],
			  text[16], response_id[16], text[17],
			  response_id[17], text[18], response_id[18], text[19], response_id[19], NULL);

   for (i = 0; i < n; i++)
    {
       FREE_TEXT(text[i]);
    }
   return 0;
 err:
   return 1;
}

int
clip_GTK_DIALOGADDACTIONWIDGET(ClipMachine * ClipMachineMemory)
{
   C_widget *cdialog = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gint      response_id = _clip_parni(ClipMachineMemory, 3);

   CHECKCWID(cdialog, GTK_IS_DIALOG);
   CHECKARG(2, MAP_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_WIDGET);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);

   gtk_dialog_add_action_widget(GTK_DIALOG(cdialog->widget), GTK_WIDGET(cwid->widget), response_id);

   return 0;
 err:
   return 1;
}

int
clip_GTK_DIALOGGETHASSEPARATOR(ClipMachine * ClipMachineMemory)
{
   C_widget *cdialog = _fetch_cw_arg(ClipMachineMemory);

   gboolean  ret;

   CHECKCWID(cdialog, GTK_IS_DIALOG);

   ret = gtk_dialog_get_has_separator(GTK_DIALOG(cdialog->widget));

   _clip_retni(ClipMachineMemory, ret);

   return 0;
 err:
   return 1;
}

int
clip_GTK_DIALOGSETDEFAULTRESPONSE(ClipMachine * ClipMachineMemory)
{
   C_widget *cdialog = _fetch_cw_arg(ClipMachineMemory);

   gint      response_id = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(cdialog, GTK_IS_DIALOG);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_dialog_set_default_response(GTK_DIALOG(cdialog->widget), response_id);

   return 0;
 err:
   return 1;
}

int
clip_GTK_DIALOGSETHASSEPARATOR(ClipMachine * ClipMachineMemory)
{
   C_widget *cdialog = _fetch_cw_arg(ClipMachineMemory);

   gboolean  setting = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(cdialog, GTK_IS_DIALOG);
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_dialog_set_default_response(GTK_DIALOG(cdialog->widget), setting);

   return 0;
 err:
   return 1;
}

int
clip_GTK_DIALOGSETRESPONSESENSITIVE(ClipMachine * ClipMachineMemory)
{
   C_widget *cdialog = _fetch_cw_arg(ClipMachineMemory);

   gint      response_id = _clip_parni(ClipMachineMemory, 2);

   gboolean  setting = _clip_parl(ClipMachineMemory, 3);

   CHECKCWID(cdialog, GTK_IS_DIALOG);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, LOGICAL_type_of_ClipVarType);

   gtk_dialog_set_response_sensitive(GTK_DIALOG(cdialog->widget), response_id, setting);

   return 0;
 err:
   return 1;
}

/* Returns the VBox area of the dialog */
int
clip_GTK_DIALOGGETVBOX(ClipMachine * ClipMachineMemory)
{
   C_widget *cdialog = _fetch_cw_arg(ClipMachineMemory);

   GtkWidget *area;

   C_widget *carea = 0;

   CHECKCWID(cdialog, GTK_IS_DIALOG);
   area = GTK_DIALOG(cdialog->widget)->vbox;
   if (area)
      carea = _list_get_cwidget(ClipMachineMemory, area);
   if (!carea)
      carea = _register_widget(ClipMachineMemory, area, NULL);
   if (carea)
      _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &carea->obj);
   return 0;
 err:
   return 1;
}

/* Returns the action area of the dialog */
int
clip_GTK_DIALOGGETACTIONAREA(ClipMachine * ClipMachineMemory)
{
   C_widget *cdialog = _fetch_cw_arg(ClipMachineMemory);

   GtkWidget *area;

   C_widget *carea = 0;

   CHECKCWID(cdialog, GTK_IS_DIALOG);
   area = GTK_DIALOG(cdialog->widget)->action_area;
   if (area)
      carea = _list_get_cwidget(ClipMachineMemory, area);
   if (!carea)
      carea = _register_widget(ClipMachineMemory, area, NULL);
   if (carea)
      _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &carea->obj);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* MESSAGE DIALOG
******************************************************************************/
int
clip_GTK_MESSAGEDIALOGNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   C_widget *cpwin = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   GtkDialogFlags flags = INT_OPTION(ClipMachineMemory, 3, 0);

   GtkMessageType msgs = INT_OPTION(ClipMachineMemory, 4, 0);

   GtkButtonsType btns = INT_OPTION(ClipMachineMemory, 5, 0);

   GtkWidget *wid = NULL;

   C_widget *cwid;

   gint      i, n = _clip_parinfo(ClipMachineMemory, 0) - 5;

   gchar    *mfmt[n];

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cpwin, GTK_WINDOW);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);
   CHECKARG(5, NUMERIC_type_of_ClipVarType);

   memset(mfmt, 0, sizeof(mfmt));
   for (i = 0; i < n; i++)
    {
       CHECKOPT(i + 4, CHARACTER_type_of_ClipVarType);
       if (_clip_parinfo(ClipMachineMemory, i + 4) == UNDEF_type_of_ClipVarType)
	  break;
       mfmt[i] = _clip_parc(ClipMachineMemory, i + 4);
    }
   wid = gtk_message_dialog_new((cpwin !=
				 NULL) ? GTK_WINDOW(cpwin->widget) : NULL,
				flags, msgs, btns, mfmt[0], mfmt[1], mfmt[2],
				mfmt[3], mfmt[4], mfmt[5], mfmt[6], mfmt[7],
				mfmt[8], mfmt[9], mfmt[10], mfmt[11],
				mfmt[12], mfmt[13], mfmt[14], mfmt[15], mfmt[16], mfmt[17], mfmt[18], mfmt[19]);
   if (!wid)
      goto err;
  //cwid = (C_widget*)calloc( 1, sizeof(C_widget) );
   cwid = _register_widget(ClipMachineMemory, wid, cv);

   cwid->accel_group = gtk_accel_group_new();
   gtk_window_add_accel_group(GTK_WINDOW(wid), cwid->accel_group);

   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 4)
int
clip_GTK_MESSAGEDIALOGNEWWITHMARKUP(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   C_widget *cpwin = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   GtkDialogFlags flags = INT_OPTION(ClipMachineMemory, 3, 0);

   GtkMessageType msgs = INT_OPTION(ClipMachineMemory, 4, 0);

   GtkButtonsType btns = INT_OPTION(ClipMachineMemory, 5, 0);

   GtkWidget *wid = NULL;

   C_widget *cwid;

   gint      i, n = _clip_parinfo(ClipMachineMemory, 0) - 5;

   gchar    *mfmt[n];

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cpwin, GTK_WINDOW);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);
   CHECKARG(5, NUMERIC_type_of_ClipVarType);

   memset(mfmt, 0, sizeof(mfmt));
   for (i = 0; i < n; i++)
    {
       CHECKOPT(i + 4, CHARACTER_type_of_ClipVarType);
       if (_clip_parinfo(ClipMachineMemory, i + 4) == UNDEF_type_of_ClipVarType)
	  break;
       mfmt[i] = _clip_parc(ClipMachineMemory, i + 4);
    }
   wid = gtk_message_dialog_new_with_markup((cpwin !=
					     NULL) ? GTK_WINDOW(cpwin->widget) :
					    NULL, flags, msgs, btns, mfmt[0],
					    mfmt[1], mfmt[2], mfmt[3],
					    mfmt[4], mfmt[5], mfmt[6],
					    mfmt[7], mfmt[8], mfmt[9],
					    mfmt[10], mfmt[11], mfmt[12],
					    mfmt[13], mfmt[14], mfmt[15], mfmt[16], mfmt[17], mfmt[18], mfmt[19]);
   if (!wid)
      goto err;
  //cwid = (C_widget*)calloc( 1, sizeof(C_widget) );
   cwid = _register_widget(ClipMachineMemory, wid, cv);

   cwid->accel_group = gtk_accel_group_new();
   gtk_window_add_accel_group(GTK_WINDOW(wid), cwid->accel_group);

   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

int
clip_GTK_MESSAGEDIALOGSETMARKUP(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   gchar    *markup = _clip_parc(ClipMachineMemory, 2);

   CHECKCWID(cwid, GTK_IS_MESSAGE_DIALOG);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   gtk_message_dialog_set_markup(GTK_MESSAGE_DIALOG(cwid->widget), markup);
   return 0;
 err:
   return 1;
}
#endif

#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 6)
int
clip_GTK_MESSAGEDIALOGFORMATSECONDARYMARKUP(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   gchar    *markup = _clip_parc(ClipMachineMemory, 2);

   gchar    *margs[10];

   gint      i, j, n;

   CHECKCWID(cwid, GTK_IS_MESSAGE_DIALOG);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);

   n = _clip_parinfo(ClipMachineMemory, 0) - 2;
   for (i = 0, j = 3; i < n; i++)
    {
       gchar    *str = _clip_parc(ClipMachineMemory, j);

       CHECKARG(j, CHARACTER_type_of_ClipVarType);
       LOCALE_TO_UTF(str);
       strcpy(margs[i], str);
       FREE_TEXT(str);
    }
   if (markup)
      LOCALE_TO_UTF(markup);
   gtk_message_dialog_format_secondary_markup(GTK_MESSAGE_DIALOG
					      (cwid->widget), markup,
					      margs[0], margs[1], margs[2],
					      margs[3], margs[4], margs[5], margs[6], margs[7], margs[8], margs[9]);

   if (markup)
      FREE_TEXT(markup);

   return 0;
 err:
   return 1;
}

int
clip_GTK_MESSAGEDIALOGFORMATSECONDARYTEXT(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   gchar    *text = _clip_parc(ClipMachineMemory, 2);

   gchar    *margs[10];

   gint      i, j, n;

   CHECKCWID(cwid, GTK_IS_MESSAGE_DIALOG);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);

   n = _clip_parinfo(ClipMachineMemory, 0) - 2;
   for (i = 0, j = 3; i < n; i++)
    {
       gchar    *str = _clip_parc(ClipMachineMemory, j);

       CHECKARG(j, CHARACTER_type_of_ClipVarType);
       LOCALE_TO_UTF(str);
       strcpy(margs[i], str);
       FREE_TEXT(str);
    }
   if (text)
      LOCALE_TO_UTF(text);
   gtk_message_dialog_format_secondary_text
    (GTK_MESSAGE_DIALOG(cwid->widget),
     text, margs[0], margs[1], margs[2], margs[3], margs[4], margs[5], margs[6], margs[7], margs[8], margs[9]);

   if (text)
      FREE_TEXT(text);

   return 0;
 err:
   return 1;
}
#endif
#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 6)
int
clip_GTK_ALTERNATIVEDIALOGBUTTONORDER(ClipMachine * ClipMachineMemory)
{
   C_object *cscr = _fetch_co_arg(ClipMachineMemory);

  //CHECKCOBJOPT(cscr, GDK_IS_OBJECT(cscr->object));

   _clip_retl(ClipMachineMemory, gtk_alternative_dialog_button_order((cscr) ? (GdkScreen *) (cscr->object) : NULL));
   return 0;
/*
err:
	return 1;
*/
}

/* gtk_DialogSetAlternativeButtonOrder(comboBox, intFirst, ...intNext, -1)*/
int
clip_GTK_DIALOGSETALTERNATIVEBUTTONORDER(ClipMachine * ClipMachineMemory)
{
   C_widget *cdlg = _fetch_cw_arg(ClipMachineMemory);

   gint      i, n;

   gint      a[10];

   CHECKCWID(cdlg, GTK_IS_DIALOG);

   n = _clip_parinfo(ClipMachineMemory, 0) - 1;
   for (i = 0; i < 10; i++)
    {
       if (i <= n)
	{
	   CHECKARG(i + 2, NUMERIC_type_of_ClipVarType);
	   a[i] = _clip_parni(ClipMachineMemory, i + 2);
	}
       else
	  a[i] = -1;
    }

   gtk_dialog_set_alternative_button_order(GTK_DIALOG(cdlg->widget),
					   a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]);

   return 0;
 err:
   return 1;
}

int
clip_GTK_DIALOGSETALTERNATIVEBUTTONORDERFROMARRAY(ClipMachine * ClipMachineMemory)
{
   C_widget *cdlg = _fetch_cw_arg(ClipMachineMemory);

   ClipArrVar *ca = (ClipArrVar *) _clip_vptr(_clip_spar(ClipMachineMemory, 2));

   gint      i, n;

   gint     *a;

   CHECKCWID(cdlg, GTK_IS_DIALOG);
   CHECKARG(2, ARRAY_type_of_ClipVarType);

   n = (int) ca->count_of_ClipArrVar;
   a = calloc(n, sizeof(int));
   for (i = 0; i < n; i++)
    {
       ClipVar  *c;

       c = ca->ClipVar_items_of_ClipArrVar + i;
       if (c->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
	  a[i] = c->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
    }

   gtk_dialog_set_alternative_button_order(GTK_DIALOG(cdlg->widget), n, a);

   free(a);
   return 0;
 err:
   return 1;
}

#endif
