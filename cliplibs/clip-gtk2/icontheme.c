/*
    Copyright (C) 2004 - 2005  ITK
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
handle_changed_signal(GtkIconTheme * icon, C_signal * cs)
{
   OBJECTPREPARECV(cs, cv);
   OBJECTINVOKESIGHANDLER(cs, cv);
}

static SignalTable icon_theme_signals[] = {
   {"changed", GSF(handle_changed_signal), ESF(object_emit_signal),
    GTK_CHANGED_SIGNAL},
   {"", NULL, NULL, 0}
};

/**********************************************************/
CLIP_DLLEXPORT GtkType
_gtk_type_icon_theme()
{
   return GTK_TYPE_ICON_THEME;
}

long
_clip_type_icon_theme()
{
   return GTK_OBJECT_ICON_THEME;
}

const char *
_clip_type_name_icon_theme()
{
   return "GTK_OBJECT_ICON_THEME";
}

int
clip_INIT___ICONTHEME(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_icon_theme, _clip_type_name_icon_theme, _gtk_type_icon_theme, NULL, icon_theme_signals);
   return 0;
}

int
clip_GTK_ICONTHEMENEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   C_object *cicon;
   GtkIconTheme *icon;

   CHECKOPT(1, MAP_type_of_ClipVarType);

   icon = gtk_icon_theme_new();

   if (icon)
      {
	 cicon = _list_get_cobject(ClipMachineMemory, icon);
	 if (!cicon)
	    cicon = _register_object(ClipMachineMemory, icon, GTK_TYPE_ICON_THEME, cv, NULL);
	 if (cicon)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cicon->obj);
      }

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONTHEMEGETDEFAULT(ClipMachine * ClipMachineMemory)
{
   C_object *cicon;
   GtkIconTheme *icon;

   icon = gtk_icon_theme_get_default();

   if (icon)
      {
	 cicon = _list_get_cobject(ClipMachineMemory, icon);
	 if (!cicon)
	    cicon = _register_object(ClipMachineMemory, icon, GTK_TYPE_ICON_THEME, NULL, NULL);
	 if (cicon)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cicon->obj);
      }

   return 0;
}

int
clip_GTK_ICONTHEMEGETFORSCREEN(ClipMachine * ClipMachineMemory)
{
   C_object *cscreen = _fetch_co_arg(ClipMachineMemory);
   C_object *cicon;
   GtkIconTheme *icon;

   CHECKCOBJ(cscreen, GDK_IS_SCREEN(cscreen->object));

   icon = gtk_icon_theme_get_for_screen(GDK_SCREEN(cscreen->object));

   if (icon)
      {
	 cicon = _list_get_cobject(ClipMachineMemory, icon);
	 if (!cicon)
	    cicon = _register_object(ClipMachineMemory, icon, GTK_TYPE_ICON_THEME, NULL, NULL);
	 if (cicon)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cicon->obj);
      }

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONTHEMESETSCREEN(ClipMachine * ClipMachineMemory)
{
   C_object *cicon = _fetch_co_arg(ClipMachineMemory);
   C_object *cscreen = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCOBJ(cicon, GTK_IS_ICON_THEME(cicon->object));
   CHECKCOBJ(cscreen, GDK_IS_SCREEN(cscreen->object));

   gtk_icon_theme_set_screen(GTK_ICON_THEME(cicon->object), GDK_SCREEN(cscreen->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONTHEMESETSEARCHPATH(ClipMachine * ClipMachineMemory)
{
   C_object *cicon = _fetch_co_arg(ClipMachineMemory);
   ClipArrVar *cv = (ClipArrVar *) _clip_vptr(_clip_spar(ClipMachineMemory, 2));
   gint n_elements = _clip_parni(ClipMachineMemory, 3);
   gint i;

   CHECKCOBJ(cicon, GTK_IS_ICON_THEME(cicon->object));
   CHECKARG(2, ARRAY_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);

   i = cv->count_of_ClipArrVar;
   n_elements = (i < n_elements) ? i : n_elements;
   if (cv)
      {
	 const gchar *path[n_elements];
	 ClipVar *s;
	 for (i = 0; i < n_elements; i++)
	    {
	       s = &cv->ClipVar_items_of_ClipArrVar[i];
	       path[i] = s->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
	    }
	 gtk_icon_theme_set_search_path(GTK_ICON_THEME(cicon->object), path, n_elements);
      }
   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONTHEMEGETSEARCHPATH(ClipMachine * ClipMachineMemory)
{
   C_object *cicon = _fetch_co_arg(ClipMachineMemory);
   ClipVar *cv = RETPTR(ClipMachineMemory);
   gchar **path;
   gint n_elements;
   long i;

   CHECKCOBJ(cicon, GTK_IS_ICON_THEME(cicon->object));

   gtk_icon_theme_get_search_path(GTK_ICON_THEME(cicon->object), &path, &n_elements);
   i = n_elements;
   _clip_array(ClipMachineMemory, cv, 1, &i);
   for (i = 0; i < n_elements; i++)
      {
	 ClipVar c;
	 _clip_var_str(path[i], strlen(path[i]), &c);
	 _clip_aset(ClipMachineMemory, cv, &c, 1, &i);
	 _clip_destroy(ClipMachineMemory, &c);
      }
   g_strfreev(path);
   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONTHEMEAPPENDSEARCHPATH(ClipMachine * ClipMachineMemory)
{
   C_object *cicon = _fetch_co_arg(ClipMachineMemory);
   gchar *path = _clip_parc(ClipMachineMemory, 2);

   CHECKCOBJ(cicon, GTK_IS_ICON_THEME(cicon->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   gtk_icon_theme_append_search_path(GTK_ICON_THEME(cicon->object), path);
   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONTHEMEPREPENDSEARCHPATH(ClipMachine * ClipMachineMemory)
{
   C_object *cicon = _fetch_co_arg(ClipMachineMemory);
   gchar *path = _clip_parc(ClipMachineMemory, 2);

   CHECKCOBJ(cicon, GTK_IS_ICON_THEME(cicon->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   gtk_icon_theme_prepend_search_path(GTK_ICON_THEME(cicon->object), path);
   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONTHEMESETCUSTOMTHEME(ClipMachine * ClipMachineMemory)
{
   C_object *cicon = _fetch_co_arg(ClipMachineMemory);
   gchar *name = _clip_parc(ClipMachineMemory, 2);

   CHECKCOBJ(cicon, GTK_IS_ICON_THEME(cicon->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   gtk_icon_theme_set_custom_theme(GTK_ICON_THEME(cicon->object), name);
   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONTHEMEHASICON(ClipMachine * ClipMachineMemory)
{
   C_object *cicon = _fetch_co_arg(ClipMachineMemory);
   gchar *name = _clip_parc(ClipMachineMemory, 2);

   CHECKCOBJ(cicon, GTK_IS_ICON_THEME(cicon->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   _clip_retl(ClipMachineMemory, gtk_icon_theme_has_icon(GTK_ICON_THEME(cicon->object), name));
   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONTHEMELOOKUPICON(ClipMachine * ClipMachineMemory)
{
   C_object *cicon = _fetch_co_arg(ClipMachineMemory);
   gchar *name = _clip_parc(ClipMachineMemory, 2);
   gint size = _clip_parni(ClipMachineMemory, 3);
   GtkIconLookupFlags flags = _clip_parni(ClipMachineMemory, 4);

   CHECKCOBJ(cicon, GTK_IS_ICON_THEME(cicon->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);

   gtk_icon_theme_lookup_icon(GTK_ICON_THEME(cicon->object), name, size, flags);
   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONTHEMELOADICON(ClipMachine * ClipMachineMemory)
{
   C_object *cicon = _fetch_co_arg(ClipMachineMemory);
   gchar *name = _clip_parc(ClipMachineMemory, 2);
   gint size = _clip_parni(ClipMachineMemory, 3);
   GtkIconLookupFlags flags = _clip_parni(ClipMachineMemory, 4);
   GError *error;
   C_object *cpixbuf;
   GdkPixbuf *pixbuf;

   CHECKCOBJ(cicon, GTK_IS_ICON_THEME(cicon->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);

   pixbuf = gtk_icon_theme_load_icon(GTK_ICON_THEME(cicon->object), name, size, flags, &error);

   if (pixbuf)
      {
	 cpixbuf = _list_get_cobject(ClipMachineMemory, pixbuf);
	 if (!cpixbuf)
	    cpixbuf = _register_object(ClipMachineMemory, pixbuf, GDK_TYPE_PIXBUF, NULL, NULL);
	 if (cpixbuf)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cpixbuf->obj);
      }

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONTHEMELISTICONS(ClipMachine * ClipMachineMemory)
{
   C_object *cicon = _fetch_co_arg(ClipMachineMemory);
   gchar *name = _clip_parc(ClipMachineMemory, 2);
   ClipVar *cv = RETPTR(ClipMachineMemory);
   GList *list;
   long n;

   CHECKCOBJ(cicon, GTK_IS_ICON_THEME(cicon->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   list = gtk_icon_theme_list_icons(GTK_ICON_THEME(cicon->object), name);
   n = g_list_length(list);
   _clip_array(ClipMachineMemory, cv, 1, &n);
   for (n = 0; list; list = g_list_next(list), n++)
      {
	 ClipVar c;
	 gchar *str;

	 str = (gchar *) (list->data);
	 _clip_var_str(str, strlen(str), &c);
	 _clip_aset(ClipMachineMemory, cv, &c, 1, &n);
	 _clip_destroy(ClipMachineMemory, &c);
	 g_free(str);
      }
   g_list_free(list);
   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONTHEMEGETEXAMPLEICONNAME(ClipMachine * ClipMachineMemory)
{
   C_object *cicon = _fetch_co_arg(ClipMachineMemory);
   gchar *name;

   CHECKCOBJ(cicon, GTK_IS_ICON_THEME(cicon->object));

   name = gtk_icon_theme_get_example_icon_name(GTK_ICON_THEME(cicon->object));

   _clip_retc(ClipMachineMemory, name);

   g_free(name);
   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONTHEMERESCANIFNEEDED(ClipMachine * ClipMachineMemory)
{
   C_object *cicon = _fetch_co_arg(ClipMachineMemory);

   CHECKCOBJ(cicon, GTK_IS_ICON_THEME(cicon->object));

   _clip_retl(ClipMachineMemory, gtk_icon_theme_rescan_if_needed(GTK_ICON_THEME(cicon->object)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONTHEMEADDBUILTINAICON(ClipMachine * ClipMachineMemory)
{
   gchar *name = _clip_parc(ClipMachineMemory, 1);
   gint size = _clip_parni(ClipMachineMemory, 2);
   C_object *cpixbuf = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   CHECKARG(1, CHARACTER_type_of_ClipVarType);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cpixbuf, GDK_IS_PIXBUF(cpixbuf->object));

   gtk_icon_theme_add_builtin_icon(name, size, GDK_PIXBUF(cpixbuf->object));

   return 0;
 err:
   return 1;
}

#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 6)
int
clip_GTK_ICONTHEMEGETICONSIZES(ClipMachine * ClipMachineMemory)
{
   C_object *cicon = _fetch_co_arg(ClipMachineMemory);
   gchar *icon_name = _clip_parc(ClipMachineMemory, 2);
   ClipVar *cv = RETPTR(ClipMachineMemory);
   gint *a;

   CHECKCOBJ(cicon, GTK_IS_ICON_THEME(cicon->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(icon_name);
   a = gtk_icon_theme_get_icon_sizes(GTK_ICON_THEME(cicon->object), icon_name);
   FREE_TEXT(icon_name);

   _clip_array(ClipMachineMemory, cv, 0, 0);
   while (a)
      {
	 ClipVar *item;
	 item = NEW(ClipVar);
	 item->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	 item->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = 0;
	 item->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = a[0];
	 _clip_aadd(ClipMachineMemory, cv, item);
	 _clip_delete(ClipMachineMemory, item);
	 a++;
      }

   free(a);

   return 0;
 err:
   return 1;
}
#endif
