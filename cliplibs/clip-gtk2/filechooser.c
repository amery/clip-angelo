/*
    Copyright (C) 2004 - 2005 ITK
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
static gint
handler_signal(GtkFileChooser * filech, C_signal * cs)
{
   OBJECTPREPARECV(cs, cv);
   OBJECTINVOKESIGHANDLER(cs, cv);
}

static SignalTable file_chooser_signals[] = {
   {"current-folder-changed", GSF(handler_signal), ESF(object_emit_signal),
    GTK_CURRENT_FOLDER_CHANGED_SIGNAL},
   {"file-activated", GSF(handler_signal), ESF(object_emit_signal),
    GTK_FILE_ACTIVATED_SIGNAL},
   {"selection-changed", GSF(handler_signal), ESF(object_emit_signal),
    GTK_SELECTION_CHANGED_SIGNAL},
   {"update-preview", GSF(handler_signal), ESF(object_emit_signal),
    GTK_UPDATE_PREVIEW_SIGNAL},
   {"", NULL, NULL, 0}
};

/**********************************************************/
CLIP_DLLEXPORT GtkType
_gtk_type_file_chooser()
{
   return GTK_TYPE_FILE_CHOOSER;
}

long
_clip_type_file_chooser()
{
   return GTK_OBJECT_FILE_CHOOSER;
}

const char *
_clip_type_name_file_chooser()
{
   return "GTK_OBJECT_FILE_CHOOSER";
}

CLIP_DLLEXPORT GtkType
_gtk_type_file_chooser_dialog()
{
   return GTK_TYPE_FILE_CHOOSER_DIALOG;
}

long
_clip_type_file_chooser_dialog()
{
   return GTK_WIDGET_FILE_CHOOSER_DIALOG;
}

const char *
_clip_type_name_file_chooser_dialog()
{
   return "GTK_WIDGET_FILE_CHOOSER_DIALOG";
}

CLIP_DLLEXPORT GtkType
_gtk_type_file_chooser_widget()
{
   return GTK_TYPE_FILE_CHOOSER_WIDGET;
}

long
_clip_type_file_chooser_widget()
{
   return GTK_WIDGET_FILE_CHOOSER_WIDGET;
}

const char *
_clip_type_name_file_chooser_widget()
{
   return "GTK_WIDGET_FILE_CHOOSER_WIDGET";
}

int
clip_INIT___FILECHOOSER(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_file_chooser, _clip_type_name_file_chooser, _gtk_type_file_chooser, NULL, file_chooser_signals);
   _wtype_table_put(_clip_type_file_chooser_dialog, _clip_type_name_file_chooser_dialog, _gtk_type_file_chooser_dialog, _gtk_type_dialog, NULL);
   _wtype_table_put(_clip_type_file_chooser_widget, _clip_type_name_file_chooser_widget, _gtk_type_file_chooser_widget, _gtk_type_vbox, NULL);
   return 0;
}

int
clip_GTK_FILECHOOSERERRORQUARK(ClipMachine * ClipMachineMemory)
{
   GQuark error_quark;
   C_object *cerr;

   error_quark = gtk_file_chooser_error_quark();

   if (error_quark)
      {
	 cerr = _register_object(ClipMachineMemory, &error_quark, GTK_TYPE_OBJECT, NULL, NULL);
	 if (cerr)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cerr->obj);
      }

   return 0;
}

int
clip_GTK_FILECHOOSERSETACTION(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);
   GtkFileChooserAction action = _clip_parni(ClipMachineMemory, 2);

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_file_chooser_set_action(GTK_FILE_CHOOSER(cchooser->object), action);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERGETACTION(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));

   _clip_retni(ClipMachineMemory, (int) gtk_file_chooser_get_action(GTK_FILE_CHOOSER(cchooser->object)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERSETLOCALONLY(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);
   gboolean set = _clip_parl(ClipMachineMemory, 2);

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_file_chooser_set_local_only(GTK_FILE_CHOOSER(cchooser->object), set);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERGETLOCALONLY(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));

   _clip_retl(ClipMachineMemory, gtk_file_chooser_get_local_only(GTK_FILE_CHOOSER(cchooser->object)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERSETSELECTMILTIPLE(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);
   gboolean set = _clip_parl(ClipMachineMemory, 2);

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(cchooser->object), set);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERGETSELECTMULTIPLE(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));

   _clip_retl(ClipMachineMemory, gtk_file_chooser_get_select_multiple(GTK_FILE_CHOOSER(cchooser->object)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERSETCURRENTNAME(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);
   gchar *name = _clip_parc(ClipMachineMemory, 2);

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(name);
   gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(cchooser->object), name);
   FREE_TEXT(name);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERSETFILENAME(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);
   gchar *name = _clip_parc(ClipMachineMemory, 2);

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(name);
   _clip_retl(ClipMachineMemory, gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(cchooser->object), name));
   FREE_TEXT(name);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERGETFILENAME(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);
   gchar *name;

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));

   name = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(cchooser->object));
   LOCALE_FROM_UTF(name);
   _clip_retc(ClipMachineMemory, name);
   FREE_TEXT(name);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERSELECTFILENAME(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);
   gchar *name = _clip_parc(ClipMachineMemory, 2);

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(name);
   _clip_retl(ClipMachineMemory, gtk_file_chooser_select_filename(GTK_FILE_CHOOSER(cchooser->object), name));
   FREE_TEXT(name);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERUNSELECTFILENAME(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);
   gchar *name = _clip_parc(ClipMachineMemory, 2);

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(name);
   gtk_file_chooser_unselect_filename(GTK_FILE_CHOOSER(cchooser->object), name);
   FREE_TEXT(name);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERSELECTALL(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));

   gtk_file_chooser_select_all(GTK_FILE_CHOOSER(cchooser->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERUNSELECTALL(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));

   gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(cchooser->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERGETFILENAMES(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);
   GSList *list;
   ClipVar *cv = RETPTR(ClipMachineMemory);
   long l;

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));

   list = gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(cchooser->object));
   l = g_slist_length(list);

   _clip_array(ClipMachineMemory, cv, 1, &l);
   for (l = 0; list; list = g_slist_next(list), l++)
      {
	 ClipVar c;
	 gchar *str;
	 memset(&c, 0, sizeof(c));
	 str = (gchar *) list->data;
	 _clip_var_str(str, strlen(str), &c);
	 _clip_aset(ClipMachineMemory, cv, &c, 1, &l);
	 _clip_destroy(ClipMachineMemory, &c);
	 g_free(str);
      }
   g_slist_free(list);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERSETCURRENTFOLDER(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);
   gchar *name = _clip_parc(ClipMachineMemory, 2);

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(name);
   gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(cchooser->object), name);
   FREE_TEXT(name);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERGETCURRENTFOLDER(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);
   gchar *name;

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));

   name = gtk_file_chooser_get_current_folder(GTK_FILE_CHOOSER(cchooser->object));
   LOCALE_FROM_UTF(name);
   _clip_retc(ClipMachineMemory, name);
   FREE_TEXT(name);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERGETURI(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);
   gchar *uri;

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));

   uri = gtk_file_chooser_get_uri(GTK_FILE_CHOOSER(cchooser->object));
   LOCALE_FROM_UTF(uri);
   _clip_retc(ClipMachineMemory, uri);
   FREE_TEXT(uri);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERSETURI(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);
   gchar *uri = _clip_parc(ClipMachineMemory, 2);

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(uri);
   gtk_file_chooser_set_uri(GTK_FILE_CHOOSER(cchooser->object), uri);
   FREE_TEXT(uri);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERSELECTURI(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);
   gchar *uri = _clip_parc(ClipMachineMemory, 2);

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(uri);
   _clip_retl(ClipMachineMemory, gtk_file_chooser_select_uri(GTK_FILE_CHOOSER(cchooser->object), uri));
   FREE_TEXT(uri);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERUNSELECTURI(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);
   gchar *uri = _clip_parc(ClipMachineMemory, 2);

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(uri);
   gtk_file_chooser_select_uri(GTK_FILE_CHOOSER(cchooser->object), uri);
   FREE_TEXT(uri);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERGETURIS(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);
   GSList *list;
   ClipVar *cv = RETPTR(ClipMachineMemory);
   long l;

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));

   list = gtk_file_chooser_get_uris(GTK_FILE_CHOOSER(cchooser->object));
   l = g_slist_length(list);

   _clip_array(ClipMachineMemory, cv, 1, &l);
   for (l = 0; list; list = g_slist_next(list), l++)
      {
	 ClipVar c;
	 gchar *str;
	 memset(&c, 0, sizeof(c));
	 str = (gchar *) list->data;
	 _clip_var_str(str, strlen(str), &c);
	 _clip_aset(ClipMachineMemory, cv, &c, 1, &l);
	 _clip_destroy(ClipMachineMemory, &c);
      }
   g_slist_free(list);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERSETCURRENTFOLDERURI(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);
   gchar *uri = _clip_parc(ClipMachineMemory, 2);

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(uri);
   _clip_retl(ClipMachineMemory, gtk_file_chooser_set_current_folder_uri(GTK_FILE_CHOOSER(cchooser->object), uri));
   FREE_TEXT(uri);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERGETCURRENTFOLDERURI(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);
   gchar *uri;

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));

   uri = gtk_file_chooser_get_current_folder_uri(GTK_FILE_CHOOSER(cchooser->object));
   LOCALE_FROM_UTF(uri);
   _clip_retc(ClipMachineMemory, uri);
   FREE_TEXT(uri);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERSETPREVIEWWIDGET(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));
   CHECKCWID(cwid, GTK_IS_WIDGET);

   gtk_file_chooser_set_preview_widget(GTK_FILE_CHOOSER(cchooser->object), GTK_WIDGET(cwid->widget));

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERGETPREVIEWWIDGET(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);
   GtkWidget *wid;
   C_widget *cwid;

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));

   wid = gtk_file_chooser_get_preview_widget(GTK_FILE_CHOOSER(cchooser->object));
   if (wid)
      {
	 cwid = _list_get_cwidget(ClipMachineMemory, wid);
	 if (!cwid)
	    cwid = _register_widget(ClipMachineMemory, wid, NULL);
	 if (cwid)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
      }
   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERSETPREVIEWWIDGETACTIVE(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);
   gboolean set = _clip_parl(ClipMachineMemory, 2);

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_file_chooser_set_preview_widget_active(GTK_FILE_CHOOSER(cchooser->object), set);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERGETPREVIEWWIDGETACTIVE(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));

   _clip_retl(ClipMachineMemory, gtk_file_chooser_get_preview_widget_active(GTK_FILE_CHOOSER(cchooser->object)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERGETUSEPREVIEWLABEL(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));

   _clip_retl(ClipMachineMemory, gtk_file_chooser_get_use_preview_label(GTK_FILE_CHOOSER(cchooser->object)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERGETPREVIEWFILENAME(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));

   _clip_retc(ClipMachineMemory, gtk_file_chooser_get_preview_filename(GTK_FILE_CHOOSER(cchooser->object)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERGETPREVIEWURI(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));

   _clip_retc(ClipMachineMemory, gtk_file_chooser_get_preview_uri(GTK_FILE_CHOOSER(cchooser->object)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERSETEXTRAWIDGET(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));
   CHECKCWID(cwid, GTK_IS_WIDGET);

   gtk_file_chooser_set_extra_widget(GTK_FILE_CHOOSER(cchooser->object), GTK_WIDGET(cwid->widget));

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERGETEXTRAWIDGET(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);
   GtkWidget *wid;
   C_widget *cwid;

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));

   wid = gtk_file_chooser_get_extra_widget(GTK_FILE_CHOOSER(cchooser->object));
   if (wid)
      {
	 cwid = _list_get_cwidget(ClipMachineMemory, wid);
	 if (!cwid)
	    cwid = _register_widget(ClipMachineMemory, wid, NULL);
	 if (cwid)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
      }
   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERADDFILTER(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);
   C_object *cfilter = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));
   CHECKCOBJ(cfilter, GTK_IS_FILE_FILTER(cfilter->object));

   gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(cchooser->object), GTK_FILE_FILTER(cfilter->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERREMOVEFILTER(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);
   C_object *cfilter = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));
   CHECKCOBJ(cfilter, GTK_IS_FILE_FILTER(cfilter->object));

   gtk_file_chooser_remove_filter(GTK_FILE_CHOOSER(cchooser->object), GTK_FILE_FILTER(cfilter->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERLISTFILTERS(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);
   GSList *list;
   ClipVar *cv = RETPTR(ClipMachineMemory);
   long l;

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));

   list = gtk_file_chooser_list_filters(GTK_FILE_CHOOSER(cchooser->object));
   l = g_slist_length(list);

   _clip_array(ClipMachineMemory, cv, 1, &l);
   for (l = 0; list; list = g_slist_next(list), l++)
      {
	 GtkFileFilter *filter;
	 C_object *cfilter;

	 filter = GTK_FILE_FILTER(list->data);
	 cfilter = _list_get_cobject(ClipMachineMemory, filter);
	 if (!cfilter)
	    cfilter = _register_object(ClipMachineMemory, filter, GTK_TYPE_FILE_FILTER, NULL, NULL);
	 if (cfilter)
	    _clip_aset(ClipMachineMemory, cv, &cfilter->obj, 1, &l);
      }
   g_slist_free(list);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERSETFILTER(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);
   C_object *cfilter = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));
   CHECKCOBJ(cfilter, GTK_IS_FILE_FILTER(cfilter->object));

   gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(cchooser->object), GTK_FILE_FILTER(cfilter->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERGETFILTER(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);
   C_object *cfilter;
   GtkFileFilter *filter;

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));

   filter = gtk_file_chooser_get_filter(GTK_FILE_CHOOSER(cchooser->object));
   if (filter)
      {
	 cfilter = _list_get_cobject(ClipMachineMemory, filter);
	 if (!cfilter)
	    cfilter = _register_object(ClipMachineMemory, filter, GTK_TYPE_FILE_FILTER, NULL, NULL);
	 if (cfilter)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cfilter->obj);
      }
   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERADDSHORTCUTFOLDER(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);
   gchar *name = _clip_parc(ClipMachineMemory, 2);
   GError *error;

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(name);
   _clip_retl(ClipMachineMemory, gtk_file_chooser_add_shortcut_folder(GTK_FILE_CHOOSER(cchooser->object), name, &error));
   FREE_TEXT(name);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERREMOVESHORTCUTFOLDER(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);
   gchar *name = _clip_parc(ClipMachineMemory, 2);
   GError *error;

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(name);
   _clip_retl(ClipMachineMemory, gtk_file_chooser_remove_shortcut_folder(GTK_FILE_CHOOSER(cchooser->object), name, &error));
   FREE_TEXT(name);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERLISTSHORTCUTFOLDERS(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);
   GSList *list;
   ClipVar *cv = RETPTR(ClipMachineMemory);
   long l;

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));

   list = gtk_file_chooser_list_shortcut_folders(GTK_FILE_CHOOSER(cchooser->object));
   l = g_slist_length(list);

   _clip_array(ClipMachineMemory, cv, 1, &l);
   for (l = 0; list; list = g_slist_next(list), l++)
      {
	 ClipVar c;
	 gchar *str;
	 memset(&c, 0, sizeof(c));
	 str = (gchar *) list->data;
	 _clip_var_str(str, strlen(str), &c);
	 _clip_aset(ClipMachineMemory, cv, &c, 1, &l);
	 _clip_destroy(ClipMachineMemory, &c);
	 g_free(str);
      }
   g_slist_free(list);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERADDSHORTCUTFOLDERURI(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);
   gchar *name = _clip_parc(ClipMachineMemory, 2);
   GError *error;

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(name);
   _clip_retl(ClipMachineMemory, gtk_file_chooser_add_shortcut_folder_uri(GTK_FILE_CHOOSER(cchooser->object), name, &error));
   FREE_TEXT(name);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERREMOVESHORTCUTFOLDERURI(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);
   gchar *name = _clip_parc(ClipMachineMemory, 2);
   GError *error;

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(name);
   _clip_retl(ClipMachineMemory, gtk_file_chooser_remove_shortcut_folder_uri(GTK_FILE_CHOOSER(cchooser->object), name, &error));
   FREE_TEXT(name);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERLISTSHORTCUTFOLDERURIS(ClipMachine * ClipMachineMemory)
{
   C_object *cchooser = _fetch_co_arg(ClipMachineMemory);
   GSList *list;
   ClipVar *cv = RETPTR(ClipMachineMemory);
   long l;

   CHECKCOBJ(cchooser, GTK_IS_FILE_CHOOSER(cchooser->object));

   list = gtk_file_chooser_list_shortcut_folder_uris(GTK_FILE_CHOOSER(cchooser->object));
   l = g_slist_length(list);

   _clip_array(ClipMachineMemory, cv, 1, &l);
   for (l = 0; list; list = g_slist_next(list), l++)
      {
	 ClipVar c;
	 gchar *str;
	 memset(&c, 0, sizeof(c));
	 str = (gchar *) list->data;
	 _clip_var_str(str, strlen(str), &c);
	 _clip_aset(ClipMachineMemory, cv, &c, 1, &l);
	 _clip_destroy(ClipMachineMemory, &c);
	 g_free(str);
      }
   g_slist_free(list);

   return 0;
 err:
   return 1;
}

/*****************************************************************************/
/******************* FILE CHOOSER DIALOG *************************************/
int
clip_GTK_FILECHOOSERDIALOGNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   gchar *title = _clip_parc(ClipMachineMemory, 2);
   C_widget *cparent = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));
   GtkFileChooserAction action = _clip_parni(ClipMachineMemory, 4);
   GtkWidget *wid;
   C_widget *cwid;
   gchar *button[20], *response_id[20];
   gint i, j, n;

   CHECKOPT2(1, NUMERIC_type_of_ClipVarType, MAP_type_of_ClipVarType);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);
   CHECKCWIDOPT(cparent, GTK_IS_WINDOW);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);
   memset(button, 0, sizeof(button));
   memset(response_id, 0, sizeof(response_id));
   n = _clip_parinfo(ClipMachineMemory, 0);
   for (i = 0, j = 5; j < n; j += 2, i++)
      {
	 CHECKOPT(j, CHARACTER_type_of_ClipVarType);
	 if (_clip_parinfo(ClipMachineMemory, j) == UNDEF_type_of_ClipVarType)
	    break;
	 CHECKOPT(j + 1, CHARACTER_type_of_ClipVarType);
	 button[i] = _clip_parc(ClipMachineMemory, j);
	 response_id[i] = _clip_parc(ClipMachineMemory, j + 1);
	 LOCALE_TO_UTF(button[i]);
	 LOCALE_TO_UTF(response_id[i]);
      }

   if (title)
      LOCALE_TO_UTF(title);
   wid = gtk_file_chooser_dialog_new((title) ? title : NULL,
				     GTK_WINDOW(cparent->widget),
				     action,
				     button[0], response_id[0], button[1],
				     response_id[1], button[2],
				     response_id[2], button[3],
				     response_id[3], button[4],
				     response_id[4], button[5],
				     response_id[5], button[6],
				     response_id[6], button[7],
				     response_id[7], button[8],
				     response_id[8], button[9],
				     response_id[9], button[10],
				     response_id[10], button[11],
				     response_id[11], button[12],
				     response_id[12], button[13],
				     response_id[13], button[14],
				     response_id[14], button[15],
				     response_id[15], button[16], response_id[16], button[17], response_id[17], button[18], response_id[18], button[19], response_id[19], NULL);
   for (i = 0; i < n - 5; i++)
      {
	 FREE_TEXT(button[i]);
	 FREE_TEXT(response_id[i]);
      }
   if (title)
      FREE_TEXT(title);
   if (wid)
      {
	 cwid = _list_get_cwidget(ClipMachineMemory, wid);
	 if (!cwid)
	    cwid = _register_widget(ClipMachineMemory, wid, cv);
	 if (cwid)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
      }
   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERDIALOGNEWWITHBACKEND(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   gchar *title = _clip_parc(ClipMachineMemory, 2);
   C_widget *cparent = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));
   GtkFileChooserAction action = _clip_parni(ClipMachineMemory, 4);
   gchar *backend = _clip_parc(ClipMachineMemory, 5);
   GtkWidget *wid;
   C_widget *cwid;
   gchar *button[20], *response_id[20];
   gint i, j, n;

   CHECKOPT2(1, NUMERIC_type_of_ClipVarType, MAP_type_of_ClipVarType);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);
   CHECKCWIDOPT(cparent, GTK_IS_WINDOW);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);
   CHECKOPT(5, CHARACTER_type_of_ClipVarType);

   memset(button, 0, sizeof(button));
   memset(response_id, 0, sizeof(response_id));

   n = _clip_parinfo(ClipMachineMemory, 0);
   for (i = 0, j = 6; j < n; j += 2, i++)
      {
	 CHECKOPT(j, CHARACTER_type_of_ClipVarType);
	 if (_clip_parinfo(ClipMachineMemory, j) == UNDEF_type_of_ClipVarType)
	    break;
	 CHECKOPT(j + 1, CHARACTER_type_of_ClipVarType);
	 button[i] = _clip_parc(ClipMachineMemory, j);
	 response_id[i] = _clip_parc(ClipMachineMemory, j + 1);
	 LOCALE_TO_UTF(button[i]);
	 LOCALE_TO_UTF(response_id[i]);
      }

   if (title)
      LOCALE_TO_UTF(title);
   wid = gtk_file_chooser_dialog_new((title) ? title : NULL,
				     GTK_WINDOW(cparent->widget),
				     action,
				     backend,
				     button[0], response_id[0], button[1],
				     response_id[1], button[2],
				     response_id[2], button[3],
				     response_id[3], button[4],
				     response_id[4], button[5],
				     response_id[5], button[6],
				     response_id[6], button[7],
				     response_id[7], button[8],
				     response_id[8], button[9],
				     response_id[9], button[10],
				     response_id[10], button[11],
				     response_id[11], button[12],
				     response_id[12], button[13],
				     response_id[13], button[14],
				     response_id[14], button[15],
				     response_id[15], button[16], response_id[16], button[17], response_id[17], button[18], response_id[18], button[19], response_id[19], NULL);
   for (i = 0; i < n - 6; i++)
      {
	 FREE_TEXT(button[i]);
	 FREE_TEXT(response_id[i]);
      }
   if (title)
      FREE_TEXT(title);
   if (wid)
      {
	 cwid = _list_get_cwidget(ClipMachineMemory, wid);
	 if (!cwid)
	    cwid = _register_widget(ClipMachineMemory, wid, cv);
	 if (cwid)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
      }
   return 0;
 err:
   return 1;
}

/*****************************************************************************/
/******************* FILE CHOOSER WIDGET *************************************/

int
clip_GTK_FILECHOOSERWIDGETNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   GtkFileChooserAction action = _clip_parni(ClipMachineMemory, 2);
   GtkWidget *wid;
   C_widget *cwid;

   CHECKOPT2(1, NUMERIC_type_of_ClipVarType, MAP_type_of_ClipVarType);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   wid = gtk_file_chooser_widget_new(action);
   if (wid)
      {
	 cwid = _list_get_cwidget(ClipMachineMemory, wid);
	 if (!cwid)
	    cwid = _register_widget(ClipMachineMemory, wid, cv);
	 if (cwid)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
      }
   return 0;
 err:
   return 1;
}

int
clip_GTK_FILECHOOSERWIDGETNEWWITHBACKEND(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   GtkFileChooserAction action = _clip_parni(ClipMachineMemory, 2);
   gchar *backend = _clip_parc(ClipMachineMemory, 2);
   GtkWidget *wid;
   C_widget *cwid;

   CHECKOPT2(1, NUMERIC_type_of_ClipVarType, MAP_type_of_ClipVarType);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, CHARACTER_type_of_ClipVarType);

   wid = gtk_file_chooser_widget_new_with_backend(action, backend);
   if (wid)
      {
	 cwid = _list_get_cwidget(ClipMachineMemory, wid);
	 if (!cwid)
	    cwid = _register_widget(ClipMachineMemory, wid, cv);
	 if (cwid)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
      }
   return 0;
 err:
   return 1;
}
