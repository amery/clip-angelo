/*
    Copyright (C) 2005 ITK
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

/**********************************************************/

/* Register about_dialog in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_about_dialog()
{
   return GTK_TYPE_ABOUT_DIALOG;
}

long
_clip_type_about_dialog()
{
   return GTK_WIDGET_ABOUT_DIALOG;
}

const char *
_clip_type_name_about_dialog()
{
   return "GTK_WIDGET_ABOUT_DIALOG";
}

int
clip_INIT___ABOUT_DIALOG(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_about_dialog, _clip_type_name_about_dialog, _gtk_type_about_dialog, _gtk_type_dialog, NULL);
   return 0;
}

/**********************************************************/

/****  DIALOG constructor ****/
int
clip_GTK_ABOUTDIALOGNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   char *title = _clip_parc(ClipMachineMemory, 2);
   GtkWidget *wid = NULL;
   C_widget *cwid;
   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);

   wid = gtk_about_dialog_new();
   if (!wid)
      goto err;
   if (_clip_parinfo(ClipMachineMemory, 2) == CHARACTER_type_of_ClipVarType)
      {
	 LOCALE_TO_UTF(title);
	 gtk_window_set_title(GTK_WINDOW(wid), title);
	 FREE_TEXT(title);
      }
   cwid = _register_widget(ClipMachineMemory, wid, cv);

   cwid->accel_group = gtk_accel_group_new();
   gtk_window_add_accel_group(GTK_WINDOW(wid), cwid->accel_group);

   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

int
clip_GTK_ABOUTDIALOGGETNAME(ClipMachine * ClipMachineMemory)
{
   C_widget *cdialog = _fetch_cw_arg(ClipMachineMemory);
   char *str;

   CHECKCWID(cdialog, GTK_IS_ABOUT_DIALOG);

   str = (char *) gtk_about_dialog_get_name(GTK_ABOUT_DIALOG(cdialog->widget));
   LOCALE_FROM_UTF(str);
   _clip_retc(ClipMachineMemory, str);
   FREE_TEXT(str);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ABOUTDIALOGSETNAME(ClipMachine * ClipMachineMemory)
{
   C_widget *cdialog = _fetch_cw_arg(ClipMachineMemory);
   gchar *str = _clip_parc(ClipMachineMemory, 2);

   CHECKCWID(cdialog, GTK_IS_ABOUT_DIALOG);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(str);
   gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(cdialog->widget), (const gchar *) str);
   FREE_TEXT(str);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ABOUTDIALOGGETVERSION(ClipMachine * ClipMachineMemory)
{
   C_widget *cdialog = _fetch_cw_arg(ClipMachineMemory);
   gchar *str;

   CHECKCWID(cdialog, GTK_IS_ABOUT_DIALOG);

   str = (gchar *) gtk_about_dialog_get_version(GTK_ABOUT_DIALOG(cdialog->widget));
   LOCALE_FROM_UTF(str);
   _clip_retc(ClipMachineMemory, str);
   FREE_TEXT(str);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ABOUTDIALOGSETVERSION(ClipMachine * ClipMachineMemory)
{
   C_widget *cdialog = _fetch_cw_arg(ClipMachineMemory);
   gchar *str = _clip_parc(ClipMachineMemory, 2);

   CHECKCWID(cdialog, GTK_IS_ABOUT_DIALOG);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(str);
   gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(cdialog->widget), (const gchar *) str);
   FREE_TEXT(str);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ABOUTDIALOGGETCOPYRIGHT(ClipMachine * ClipMachineMemory)
{
   C_widget *cdialog = _fetch_cw_arg(ClipMachineMemory);
   gchar *str;

   CHECKCWID(cdialog, GTK_IS_ABOUT_DIALOG);

   str = (gchar *) gtk_about_dialog_get_copyright(GTK_ABOUT_DIALOG(cdialog->widget));
   LOCALE_FROM_UTF(str);
   _clip_retc(ClipMachineMemory, str);
   FREE_TEXT(str);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ABOUTDIALOGSETCOPYRIGHT(ClipMachine * ClipMachineMemory)
{
   C_widget *cdialog = _fetch_cw_arg(ClipMachineMemory);
   gchar *str = _clip_parc(ClipMachineMemory, 2);

   CHECKCWID(cdialog, GTK_IS_ABOUT_DIALOG);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(str);
   gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(cdialog->widget), (const gchar *) str);
   FREE_TEXT(str);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ABOUTDIALOGGETCOMMENTS(ClipMachine * ClipMachineMemory)
{
   C_widget *cdialog = _fetch_cw_arg(ClipMachineMemory);
   gchar *str;

   CHECKCWID(cdialog, GTK_IS_ABOUT_DIALOG);

   str = (gchar *) gtk_about_dialog_get_comments(GTK_ABOUT_DIALOG(cdialog->widget));
   LOCALE_FROM_UTF(str);
   _clip_retc(ClipMachineMemory, str);
   FREE_TEXT(str);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ABOUTDIALOGSETCOMMENTS(ClipMachine * ClipMachineMemory)
{
   C_widget *cdialog = _fetch_cw_arg(ClipMachineMemory);
   gchar *str = _clip_parc(ClipMachineMemory, 2);

   CHECKCWID(cdialog, GTK_IS_ABOUT_DIALOG);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(str);
   gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(cdialog->widget), (const gchar *) str);
   FREE_TEXT(str);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ABOUTDIALOGGETLICENSE(ClipMachine * ClipMachineMemory)
{
   C_widget *cdialog = _fetch_cw_arg(ClipMachineMemory);
   gchar *str;

   CHECKCWID(cdialog, GTK_IS_ABOUT_DIALOG);

   str = (gchar *) gtk_about_dialog_get_license(GTK_ABOUT_DIALOG(cdialog->widget));
   LOCALE_FROM_UTF(str);
   _clip_retc(ClipMachineMemory, str);
   FREE_TEXT(str);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ABOUTDIALOGSETLICENSE(ClipMachine * ClipMachineMemory)
{
   C_widget *cdialog = _fetch_cw_arg(ClipMachineMemory);
   gchar *str = _clip_parc(ClipMachineMemory, 2);

   CHECKCWID(cdialog, GTK_IS_ABOUT_DIALOG);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(str);
   gtk_about_dialog_set_license(GTK_ABOUT_DIALOG(cdialog->widget), (const gchar *) str);
   FREE_TEXT(str);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ABOUTDIALOGGETWEBSITE(ClipMachine * ClipMachineMemory)
{
   C_widget *cdialog = _fetch_cw_arg(ClipMachineMemory);
   gchar *str;

   CHECKCWID(cdialog, GTK_IS_ABOUT_DIALOG);

   str = (gchar *) gtk_about_dialog_get_website(GTK_ABOUT_DIALOG(cdialog->widget));
   LOCALE_FROM_UTF(str);
   _clip_retc(ClipMachineMemory, str);
   FREE_TEXT(str);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ABOUTDIALOGSETWEBSITE(ClipMachine * ClipMachineMemory)
{
   C_widget *cdialog = _fetch_cw_arg(ClipMachineMemory);
   gchar *str = _clip_parc(ClipMachineMemory, 2);

   CHECKCWID(cdialog, GTK_IS_ABOUT_DIALOG);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(str);
   gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(cdialog->widget), (const gchar *) str);
   FREE_TEXT(str);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ABOUTDIALOGGETWEBSITELABEL(ClipMachine * ClipMachineMemory)
{
   C_widget *cdialog = _fetch_cw_arg(ClipMachineMemory);
   gchar *str;

   CHECKCWID(cdialog, GTK_IS_ABOUT_DIALOG);

   str = (gchar *) gtk_about_dialog_get_website_label(GTK_ABOUT_DIALOG(cdialog->widget));
   LOCALE_FROM_UTF(str);
   _clip_retc(ClipMachineMemory, str);
   FREE_TEXT(str);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ABOUTDIALOGSETWEBSITELABEL(ClipMachine * ClipMachineMemory)
{
   C_widget *cdialog = _fetch_cw_arg(ClipMachineMemory);
   gchar *str = _clip_parc(ClipMachineMemory, 2);

   CHECKCWID(cdialog, GTK_IS_ABOUT_DIALOG);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(str);
   gtk_about_dialog_set_website_label(GTK_ABOUT_DIALOG(cdialog->widget), (const gchar *) str);
   FREE_TEXT(str);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ABOUTDIALOGSETAUTHORS(ClipMachine * ClipMachineMemory)
{
   C_widget *cdialog = _fetch_cw_arg(ClipMachineMemory);
   ClipArrVar *ca = (ClipArrVar *) _clip_vptr(_clip_spar(ClipMachineMemory, 2));
   long l;
   int i;
   gchar **str;

   CHECKCWID(cdialog, GTK_IS_ABOUT_DIALOG);
   CHECKARG(2, ARRAY_type_of_ClipVarType);

   l = ca->count_of_ClipArrVar;
   for (i = 0; i < l; i++)
      if (ca->ClipVar_items_of_ClipArrVar[i].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType)
	 goto err;

   str = (gchar **) calloc(sizeof(gchar *), l);

   for (i = 0; i < l; i++)
      {
	 ClipVar *c;

	 c = ca->ClipVar_items_of_ClipArrVar + i;

	 str[i] = (gchar *) calloc(sizeof(gchar *), c->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf + 1);
	 strcpy(str[i], c->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
	 LOCALE_TO_UTF(str[i]);
      }

   gtk_about_dialog_set_authors(GTK_ABOUT_DIALOG(cdialog->widget), (const gchar **) str);

   for (i = 0; i < l; i++)
      {
	 FREE_TEXT(str[i]);
	 free(str[i]);
      }
   free(str);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ABOUTDIALOGSETARTISTS(ClipMachine * ClipMachineMemory)
{
   C_widget *cdialog = _fetch_cw_arg(ClipMachineMemory);
   ClipArrVar *ca = (ClipArrVar *) _clip_vptr(_clip_spar(ClipMachineMemory, 2));
   long l;
   int i;
   gchar **str;

   CHECKCWID(cdialog, GTK_IS_ABOUT_DIALOG);
   CHECKARG(2, ARRAY_type_of_ClipVarType);

   l = ca->count_of_ClipArrVar;
   for (i = 0; i < l; i++)
      if (ca->ClipVar_items_of_ClipArrVar[i].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType)
	 goto err;

   str = (gchar **) calloc(sizeof(gchar *), l);

   for (i = 0; i < l; i++)
      {
	 ClipVar *c;

	 c = ca->ClipVar_items_of_ClipArrVar + i;

	 str[i] = (gchar *) calloc(sizeof(gchar *), c->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf + 1);
	 strcpy(str[i], c->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
	 LOCALE_TO_UTF(str[i]);
      }

   gtk_about_dialog_set_artists(GTK_ABOUT_DIALOG(cdialog->widget), (const gchar **) str);

   for (i = 0; i < l; i++)
      {
	 FREE_TEXT(str[i]);
	 free(str[i]);
      }
   free(str);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ABOUTDIALOGSETDOCUMENTERS(ClipMachine * ClipMachineMemory)
{
   C_widget *cdialog = _fetch_cw_arg(ClipMachineMemory);
   ClipArrVar *ca = (ClipArrVar *) _clip_vptr(_clip_spar(ClipMachineMemory, 2));
   long l;
   int i;
   gchar **str;

   CHECKCWID(cdialog, GTK_IS_ABOUT_DIALOG);
   CHECKARG(2, ARRAY_type_of_ClipVarType);

   l = ca->count_of_ClipArrVar;
   for (i = 0; i < l; i++)
      if (ca->ClipVar_items_of_ClipArrVar[i].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType)
	 goto err;

   str = (gchar **) calloc(sizeof(gchar *), l);

   for (i = 0; i < l; i++)
      {
	 ClipVar *c;

	 c = ca->ClipVar_items_of_ClipArrVar + i;

	 str[i] = (gchar *) calloc(sizeof(gchar *), c->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf + 1);
	 strcpy(str[i], c->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
	 LOCALE_TO_UTF(str[i]);
      }

   gtk_about_dialog_set_documenters(GTK_ABOUT_DIALOG(cdialog->widget), (const gchar **) str);

   for (i = 0; i < l; i++)
      {
	 FREE_TEXT(str[i]);
	 free(str[i]);
      }
   free(str);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ABOUTDIALOGGETTRANSLATORCREDITS(ClipMachine * ClipMachineMemory)
{
   C_widget *cdialog = _fetch_cw_arg(ClipMachineMemory);
   gchar *str;

   CHECKCWID(cdialog, GTK_IS_ABOUT_DIALOG);

   str = (gchar *) gtk_about_dialog_get_translator_credits(GTK_ABOUT_DIALOG(cdialog->widget));
   LOCALE_FROM_UTF(str);
   _clip_retc(ClipMachineMemory, str);
   FREE_TEXT(str);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ABOUTDIALOGSETTRANSLATORCREDITS(ClipMachine * ClipMachineMemory)
{
   C_widget *cdialog = _fetch_cw_arg(ClipMachineMemory);
   gchar *str = _clip_parc(ClipMachineMemory, 2);

   CHECKCWID(cdialog, GTK_IS_ABOUT_DIALOG);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(str);
   gtk_about_dialog_set_translator_credits(GTK_ABOUT_DIALOG(cdialog->widget), (const gchar *) str);
   FREE_TEXT(str);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ABOUTDIALOGGETLOGO(ClipMachine * ClipMachineMemory)
{
   C_widget *cdialog = _fetch_cw_arg(ClipMachineMemory);
   GdkPixbuf *pix;
   C_object *cpix;

   CHECKCWID(cdialog, GTK_IS_ABOUT_DIALOG);

   pix = gtk_about_dialog_get_logo(GTK_ABOUT_DIALOG(cdialog->widget));

   if (pix)
      {
	 cpix = _list_get_cobject(ClipMachineMemory, pix);
	 if (!cpix)
	    cpix = _register_object(ClipMachineMemory, pix, GDK_TYPE_PIXBUF, NULL, NULL);
	 if (cpix)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cpix->obj);
      }

   return 0;
 err:
   return 1;
}

int
clip_GTK_ABOUTDIALOGSETLOGO(ClipMachine * ClipMachineMemory)
{
   C_widget *cdialog = _fetch_cw_arg(ClipMachineMemory);
   C_object *cpix = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(cdialog, GTK_IS_ABOUT_DIALOG);
   CHECKCOBJ(cpix, GDK_IS_PIXBUF(cpix->object));

   gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(cdialog->widget), GDK_PIXBUF(cpix->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ABOUTDIALOGGETLOGOICONNAME(ClipMachine * ClipMachineMemory)
{
   C_widget *cdialog = _fetch_cw_arg(ClipMachineMemory);
   gchar *str;

   CHECKCWID(cdialog, GTK_IS_ABOUT_DIALOG);

   str = (gchar *) gtk_about_dialog_get_logo_icon_name(GTK_ABOUT_DIALOG(cdialog->widget));
   LOCALE_FROM_UTF(str);
   _clip_retc(ClipMachineMemory, str);
   FREE_TEXT(str);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ABOUTDIALOGSETLOGOICONNAME(ClipMachine * ClipMachineMemory)
{
   C_widget *cdialog = _fetch_cw_arg(ClipMachineMemory);
   gchar *str = _clip_parc(ClipMachineMemory, 2);

   CHECKCWID(cdialog, GTK_IS_ABOUT_DIALOG);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(str);
   gtk_about_dialog_set_logo_icon_name(GTK_ABOUT_DIALOG(cdialog->widget), (const gchar *) str);
   FREE_TEXT(str);

   return 0;
 err:
   return 1;
}

static void
_about_dialog_func(GtkAboutDialog * dialog, const gchar * link, gpointer data)
{
   C_var *c = (C_var *) data;
   C_widget *c_wid = _list_get_cwidget(c->ClipMachineMemory, dialog);
   ClipVar stack[2];
   ClipVar res;
   if (!c_wid)
      c_wid = _register_widget(c->ClipMachineMemory, GTK_WIDGET(dialog), NULL);
   if (c_wid)
      {
	 memset(&stack, 0, sizeof(stack));
	 memset(&res, 0, sizeof(ClipVar));
	//_clip_mclone(c->cw->cmachine, &stack[0], &c->cw->obj);
	 _clip_mclone(c->cw->cmachine, &stack[0], &c_wid->obj);
	 _clip_var_str(link, strlen(link), &stack[1]);
	 _clip_eval(c->ClipMachineMemory, &(c->cfunc), 2, stack, &res);
	 _clip_destroy(c->ClipMachineMemory, &res);
	 _clip_destroy(c->ClipMachineMemory, &stack[0]);
	//_clip_destroy(c->ClipMachineMemory, &stack[1]);
      }
}

int
clip_GTK_ABOUTDIALOGSETEMAILHOOK(ClipMachine * ClipMachineMemory)
{
   ClipVar *cfunc = _clip_spar(ClipMachineMemory, 1);
   C_var *c = 0;

   CHECKARG2(1, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);

   c->ClipMachineMemory = ClipMachineMemory;
   _clip_mclone(ClipMachineMemory, &c->cfunc, cfunc);
   gtk_about_dialog_set_email_hook((GtkAboutDialogActivateLinkFunc) _about_dialog_func, c, NULL);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ABOUTDIALOGSETURLHOOK(ClipMachine * ClipMachineMemory)
{
   ClipVar *cfunc = _clip_spar(ClipMachineMemory, 1);
   C_var *c = 0;

   CHECKARG2(1, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);

   c->ClipMachineMemory = ClipMachineMemory;
   _clip_mclone(ClipMachineMemory, &c->cfunc, cfunc);
   gtk_about_dialog_set_url_hook((GtkAboutDialogActivateLinkFunc) _about_dialog_func, c, NULL);

   return 0;
 err:
   return 1;
}
