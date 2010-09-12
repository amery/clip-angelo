/*
    Copyright (C) 2005  ITK
    Author  : Kornilova E.V. <alena@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg2.h"

#include <gtk/gtk.h>

#include "ci_clip-gtk2.ch"
#include "ci_clip-gtk2.h"

/**********************************************************/
CLIP_DLLEXPORT GtkType
_gtk_type_file_chooser_button()
{
   return GTK_TYPE_FILE_CHOOSER_BUTTON;
}

long
_clip_type_file_chooser_button()
{
   return GTK_WIDGET_FILE_CHOOSER_BUTTON;
}

const char *
_clip_type_name_file_chooser_button()
{
   return "GTK_WIDGET_FILE_CHOOSER_BUTTON";
}

int
clip_INIT___FILE_CHOOSER_BUTTON(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_file_chooser_button,
		    _clip_type_name_file_chooser_button, _gtk_type_file_chooser_button, _gtk_type_hbox, NULL);
   return 0;
}

/**********************************************************/

int
clip_GTK_FILECHOOSERBUTTONNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   gchar    *title = _clip_parc(ClipMachineMemory, 2);

   GtkFileChooserAction action = _clip_parni(ClipMachineMemory, 3);

   GtkWidget *wid = NULL;

   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);

   wid = gtk_file_chooser_button_new(title, action);

   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERBUTTONNEWWITHBACKEND(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   gchar    *title = _clip_parc(ClipMachineMemory, 2);

   GtkFileChooserAction action = _clip_parni(ClipMachineMemory, 3);

   gchar    *backend = _clip_parc(ClipMachineMemory, 4);

   GtkWidget *wid = NULL;

   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);

   wid = gtk_file_chooser_button_new_with_backend(title, action, backend);

   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERBUTTONNEWWITHDIALOG(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   C_widget *cdlg = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   GtkWidget *wid = NULL;

   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKCWID(cdlg, GTK_IS_DIALOG);

   wid = gtk_file_chooser_button_new_with_dialog(GTK_WIDGET(cdlg->widget));

   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERBUTTONGETTITLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cbutton = _fetch_cw_arg(ClipMachineMemory);

   gchar    *title;

   CHECKCWID(cbutton, GTK_IS_FILE_CHOOSER_BUTTON);

   title = (gchar *) gtk_file_chooser_button_get_title(GTK_FILE_CHOOSER_BUTTON(cbutton->widget));

   LOCALE_FROM_UTF(title);
   _clip_retc(ClipMachineMemory, title);
   FREE_TEXT(title);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERBUTTONSETTITLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cbutton = _fetch_cw_arg(ClipMachineMemory);

   gchar    *title = _clip_parc(ClipMachineMemory, 2);

   CHECKCWID(cbutton, GTK_IS_FILE_CHOOSER_BUTTON);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(title);
   gtk_file_chooser_button_set_title(GTK_FILE_CHOOSER_BUTTON(cbutton->widget), (const gchar *) title);
   FREE_TEXT(title);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERBUTTONGETWIDTHCHARS(ClipMachine * ClipMachineMemory)
{
   C_widget *cbutton = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cbutton, GTK_IS_FILE_CHOOSER_BUTTON);

   _clip_retni(ClipMachineMemory, gtk_file_chooser_button_get_width_chars(GTK_FILE_CHOOSER_BUTTON(cbutton->widget)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERBUTTONSETWIDTHCHARS(ClipMachine * ClipMachineMemory)
{
   C_widget *cbutton = _fetch_cw_arg(ClipMachineMemory);

   gint      width = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(cbutton, GTK_IS_FILE_CHOOSER_BUTTON);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_file_chooser_button_set_width_chars(GTK_FILE_CHOOSER_BUTTON(cbutton->widget), width);

   return 0;
 err:
   return 1;
}
