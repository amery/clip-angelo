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

/* There is no signals                                    */
/**********************************************************/
CLIP_DLLEXPORT GtkType
_gtk_type_file_filter()
{
   return GTK_TYPE_FILE_FILTER;
}

long
_clip_type_file_filter()
{
   return GTK_OBJECT_FILE_FILTER;
}

const char *
_clip_type_name_file_filter()
{
   return "GTK_OBJECT_FILE_FILTER";
}

int
clip_INIT___FILEFILTER(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_file_filter, _clip_type_name_file_filter, _gtk_type_file_filter, NULL, NULL);
   return 0;
}

int
clip_GTK_FILEFILTERNEW(ClipMachine * ClipMachineMemory)
{
   GtkFileFilter *filter;

   C_object *cfilter;

   filter = gtk_file_filter_new();

   if (filter)
    {
       cfilter = _register_object(ClipMachineMemory, filter, GTK_TYPE_FILE_FILTER, NULL, NULL);
       if (cfilter)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cfilter->obj);
    }

   return 0;
}

int
clip_GTK_FILEFILTERSETNAME(ClipMachine * ClipMachineMemory)
{
   C_object *cfilter = _fetch_co_arg(ClipMachineMemory);

   gchar    *name = _clip_parc(ClipMachineMemory, 2);

   CHECKCOBJ(cfilter, GTK_IS_FILE_FILTER(cfilter->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(name);
   gtk_file_filter_set_name(GTK_FILE_FILTER(cfilter->object), name);
   FREE_TEXT(name);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILEFILTERGETNAME(ClipMachine * ClipMachineMemory)
{
   C_object *cfilter = _fetch_co_arg(ClipMachineMemory);

   gchar    *name;

   CHECKCOBJ(cfilter, GTK_IS_FILE_FILTER(cfilter->object));

   name = (gchar *) gtk_file_filter_get_name(GTK_FILE_FILTER(cfilter->object));
   LOCALE_FROM_UTF(name);
   _clip_retc(ClipMachineMemory, name);
   FREE_TEXT(name);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILEFILTERADDMIMETYPE(ClipMachine * ClipMachineMemory)
{
   C_object *cfilter = _fetch_co_arg(ClipMachineMemory);

   gchar    *type = _clip_parc(ClipMachineMemory, 2);

   CHECKCOBJ(cfilter, GTK_IS_FILE_FILTER(cfilter->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   gtk_file_filter_add_mime_type(GTK_FILE_FILTER(cfilter->object), type);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILEFILTERADDPATTERN(ClipMachine * ClipMachineMemory)
{
   C_object *cfilter = _fetch_co_arg(ClipMachineMemory);

   gchar    *pattern = _clip_parc(ClipMachineMemory, 2);

   CHECKCOBJ(cfilter, GTK_IS_FILE_FILTER(cfilter->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   gtk_file_filter_add_pattern(GTK_FILE_FILTER(cfilter->object), pattern);

   return 0;
 err:
   return 1;
}

static    gboolean
_file_filter_func(GtkFileFilterInfo * info, gpointer data)
{
   C_var    *c = (C_var *) data;

   ClipVar   stack[2];

   ClipVar   cv;

   ClipVar   res;

   gboolean  ret = TRUE;

   memset(&stack, 0, sizeof(stack));
   memset(&res, 0, sizeof(ClipVar));
   memset(&cv, 0, sizeof(ClipVar));
   _clip_map(c->ClipMachineMemory, &cv);
   _file_filter_info_to_map(c->ClipMachineMemory, info, &cv);
   _clip_mclone(c->ClipMachineMemory, &stack[0], &c->co->obj);
   _clip_mclone(c->ClipMachineMemory, &stack[1], &cv);
   if (_clip_eval(c->ClipMachineMemory, &(c->cfunc), 2, stack, &res) == 0)
      ret =
       res.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType ==
       LOGICAL_type_of_ClipVarType ? res.ClipLogVar_l_of_ClipVar.value_of_ClipLogVar : ret;
   _clip_destroy(c->ClipMachineMemory, &res);
   _clip_destroy(c->ClipMachineMemory, &cv);
   _clip_destroy(c->ClipMachineMemory, &stack[0]);
   _clip_destroy(c->ClipMachineMemory, &stack[1]);
   return ret;
}

int
clip_GTK_FILEFILTERADDCUSTOM(ClipMachine * ClipMachineMemory)
{
   C_object *cfilter = _fetch_co_arg(ClipMachineMemory);

   GtkFileFilterFlags flags = _clip_parni(ClipMachineMemory, 2);

   ClipVar  *func = _clip_spar(ClipMachineMemory, 3);

   C_var    *c = 0;

   CHECKCOBJ(cfilter, GTK_IS_FILE_FILTER(cfilter->object));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT2(3, PCODE_type_of_ClipVarType, CCODE_type_of_ClipVarType);

   c->ClipMachineMemory = ClipMachineMemory;
   c->co = cfilter;
   _clip_mclone(ClipMachineMemory, &c->cfunc, func);
   gtk_file_filter_add_custom(GTK_FILE_FILTER(cfilter->object), flags, (GtkFileFilterFunc) _file_filter_func, c, NULL);

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILEFILTERGETNEEDED(ClipMachine * ClipMachineMemory)
{
   C_object *cfilter = _fetch_co_arg(ClipMachineMemory);

   CHECKCOBJ(cfilter, GTK_IS_FILE_FILTER(cfilter->object));

   _clip_retni(ClipMachineMemory, (int) gtk_file_filter_get_needed(GTK_FILE_FILTER(cfilter->object)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_FILEFILTERFILTER(ClipMachine * ClipMachineMemory)
{
   C_object *cfilter = _fetch_co_arg(ClipMachineMemory);

   ClipVar  *cv = _clip_spar(ClipMachineMemory, 2);

   GtkFileFilterInfo info;

   CHECKCOBJ(cfilter, GTK_IS_FILE_FILTER(cfilter->object));
   CHECKARG(2, MAP_type_of_ClipVarType);

   _map_to_file_filter_info(ClipMachineMemory, cv, &info);
   _clip_retl(ClipMachineMemory, gtk_file_filter_filter(GTK_FILE_FILTER(cfilter->object), &info));

   return 0;
 err:
   return 1;
}

#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 6)
int
clip_GTK_FILEFILTERADDPIXBUFFORMATS(ClipMachine * ClipMachineMemory)
{
   C_object *cfilter = _fetch_co_arg(ClipMachineMemory);

   CHECKCOBJ(cfilter, GTK_IS_FILE_FILTER(cfilter->object));

   gtk_file_filter_add_pixbuf_formats(GTK_FILE_FILTER(cfilter->object));

   return 0;
 err:
   return 1;
}
#endif
