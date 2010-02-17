/*
    Copyright (C) 2004  ITK
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

/**********************************************************/
CLIP_DLLEXPORT GtkType
_gtk_type_icon_source()
{
   return GTK_TYPE_ICON_SOURCE;
}

long
_clip_type_icon_source()
{
   return GTK_OBJECT_ICON_SOURCE;
}

const char *
_clip_type_name_icon_source()
{
   return "GTK_OBJECT_ICON_SOURCE";
}

/* Register in global table */
int
clip_INIT___ICONSOURCE(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_icon_source, _clip_type_name_icon_source, _gtk_type_icon_source, NULL, NULL);
   return 0;
}

int
clip_GTK_ICONSOURCENEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   GtkIconSource *isrc;
   C_object *cisrc;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   isrc = gtk_icon_source_new();

   if (isrc)
      {
	 cisrc = _list_get_cobject(ClipMachineMemory, isrc);
	 if (!cisrc)
	    cisrc = _register_object(ClipMachineMemory, isrc, GTK_TYPE_ICON_SOURCE, cv, NULL);
	 if (cisrc)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cisrc->obj);
      }
   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONSOURCECOPY(ClipMachine * ClipMachineMemory)
{
   C_object *cisrc = _fetch_co_arg(ClipMachineMemory);
   GtkIconSource *isrc;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(cisrc, GTK_IS_ICON_SOURCE(cisrc));

   isrc = gtk_icon_source_copy(GTK_ICON_SOURCE(cisrc->object));

   if (isrc)
      {
	 cisrc = _list_get_cobject(ClipMachineMemory, isrc);
	 if (!cisrc)
	    cisrc = _register_object(ClipMachineMemory, isrc, GTK_TYPE_ICON_SOURCE, NULL, NULL);
	 if (cisrc)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cisrc->obj);
      }
   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONSOURCEFREE(ClipMachine * ClipMachineMemory)
{
   C_object *cisrc = _fetch_co_arg(ClipMachineMemory);

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(cisrc, GTK_IS_ICON_SOURCE(cisrc));

   gtk_icon_source_free(GTK_ICON_SOURCE(cisrc->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONSOURCEGETDIRECTION(ClipMachine * ClipMachineMemory)
{
   C_object *cisrc = _fetch_co_arg(ClipMachineMemory);

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(cisrc, GTK_IS_ICON_SOURCE(cisrc));

   _clip_retni(ClipMachineMemory, (int) gtk_icon_source_get_direction(GTK_ICON_SOURCE(cisrc->object)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONSOURCEGETDIRECTIONWILDCARDED(ClipMachine * ClipMachineMemory)
{
   C_object *cisrc = _fetch_co_arg(ClipMachineMemory);

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(cisrc, GTK_IS_ICON_SOURCE(cisrc));

   _clip_retl(ClipMachineMemory, gtk_icon_source_get_direction_wildcarded(GTK_ICON_SOURCE(cisrc->object)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONSOURCEGETFILENAME(ClipMachine * ClipMachineMemory)
{
   C_object *cisrc = _fetch_co_arg(ClipMachineMemory);

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(cisrc, GTK_IS_ICON_SOURCE(cisrc));

   _clip_retc(ClipMachineMemory, (gchar *) gtk_icon_source_get_filename(GTK_ICON_SOURCE(cisrc->object)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONSOURCEGETPIXBUF(ClipMachine * ClipMachineMemory)
{
   C_object *cisrc = _fetch_co_arg(ClipMachineMemory);
   GdkPixbuf *pixbuf;
   C_object *cpixbuf;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(cisrc, GTK_IS_ICON_SOURCE(cisrc));

   pixbuf = gtk_icon_source_get_pixbuf(GTK_ICON_SOURCE(cisrc->object));

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
clip_GTK_ICONSOURCEGETSIZE(ClipMachine * ClipMachineMemory)
{
   C_object *cisrc = _fetch_co_arg(ClipMachineMemory);

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(cisrc, GTK_IS_ICON_SOURCE(cisrc));

   _clip_retni(ClipMachineMemory, (int) gtk_icon_source_get_size(GTK_ICON_SOURCE(cisrc->object)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONSOURCEGETSIZEWILDCARDED(ClipMachine * ClipMachineMemory)
{
   C_object *cisrc = _fetch_co_arg(ClipMachineMemory);

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(cisrc, GTK_IS_ICON_SOURCE(cisrc));

   _clip_retl(ClipMachineMemory, gtk_icon_source_get_size_wildcarded(GTK_ICON_SOURCE(cisrc->object)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONSOURCEGETSTATE(ClipMachine * ClipMachineMemory)
{
   C_object *cisrc = _fetch_co_arg(ClipMachineMemory);

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(cisrc, GTK_IS_ICON_SOURCE(cisrc));

   _clip_retni(ClipMachineMemory, (int) gtk_icon_source_get_state(GTK_ICON_SOURCE(cisrc->object)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONSOURCEGETSTATEWILDCARDED(ClipMachine * ClipMachineMemory)
{
   C_object *cisrc = _fetch_co_arg(ClipMachineMemory);

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(cisrc, GTK_IS_ICON_SOURCE(cisrc));

   _clip_retl(ClipMachineMemory, gtk_icon_source_get_state_wildcarded(GTK_ICON_SOURCE(cisrc->object)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONSOURCESETDIRECTION(ClipMachine * ClipMachineMemory)
{
   C_object *cisrc = _fetch_co_arg(ClipMachineMemory);
   GtkTextDirection direction = _clip_parni(ClipMachineMemory, 2);

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(cisrc, GTK_IS_ICON_SOURCE(cisrc));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_icon_source_set_direction(GTK_ICON_SOURCE(cisrc->object), direction);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONSOURCESETDIRECTIONWILDCARDED(ClipMachine * ClipMachineMemory)
{
   C_object *cisrc = _fetch_co_arg(ClipMachineMemory);
   gboolean setting = _clip_parl(ClipMachineMemory, 2);

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(cisrc, GTK_IS_ICON_SOURCE(cisrc));
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_icon_source_set_direction_wildcarded(GTK_ICON_SOURCE(cisrc->object), setting);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONSOURCESETFILENAME(ClipMachine * ClipMachineMemory)
{
   C_object *cisrc = _fetch_co_arg(ClipMachineMemory);
   gchar *filename = _clip_parc(ClipMachineMemory, 2);

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(cisrc, GTK_IS_ICON_SOURCE(cisrc));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   gtk_icon_source_set_filename(GTK_ICON_SOURCE(cisrc->object), filename);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONSOURCESETPIXBUF(ClipMachine * ClipMachineMemory)
{
   C_object *cisrc = _fetch_co_arg(ClipMachineMemory);
   C_object *cpixbuf = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(cisrc, GTK_IS_ICON_SOURCE(cisrc));
   CHECKARG(2, MAP_type_of_ClipVarType);
   CHECKCOBJ(cpixbuf, GDK_IS_PIXBUF(cpixbuf));

   gtk_icon_source_set_pixbuf(GTK_ICON_SOURCE(cisrc->object), GDK_PIXBUF(cpixbuf->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONSOURCESETSIZE(ClipMachine * ClipMachineMemory)
{
   C_object *cisrc = _fetch_co_arg(ClipMachineMemory);
   GtkIconSize size = _clip_parni(ClipMachineMemory, 2);

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(cisrc, GTK_IS_ICON_SOURCE(cisrc));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_icon_source_set_size(GTK_ICON_SOURCE(cisrc->object), size);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONSOURCESETSIZEWILDCARDED(ClipMachine * ClipMachineMemory)
{
   C_object *cisrc = _fetch_co_arg(ClipMachineMemory);
   gboolean setting = _clip_parl(ClipMachineMemory, 2);

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(cisrc, GTK_IS_ICON_SOURCE(cisrc));
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_icon_source_set_size_wildcarded(GTK_ICON_SOURCE(cisrc->object), setting);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONSOURCESETSTATE(ClipMachine * ClipMachineMemory)
{
   C_object *cisrc = _fetch_co_arg(ClipMachineMemory);
   GtkStateType type = _clip_parni(ClipMachineMemory, 2);

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(cisrc, GTK_IS_ICON_SOURCE(cisrc));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_icon_source_set_state(GTK_ICON_SOURCE(cisrc->object), type);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ICONSOURCESETSTATEWILDCARDED(ClipMachine * ClipMachineMemory)
{
   C_object *cisrc = _fetch_co_arg(ClipMachineMemory);
   gboolean setting = _clip_parl(ClipMachineMemory, 2);

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(cisrc, GTK_IS_ICON_SOURCE(cisrc));
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_icon_source_set_state_wildcarded(GTK_ICON_SOURCE(cisrc->object), setting);

   return 0;
 err:
   return 1;
}
