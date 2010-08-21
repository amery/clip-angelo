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

#include "ci_inkey.ch"
#include "ci_clip-gtk2.ch"
#include "ci_clip-gtk2.h"

/*****************************************************************************/
/************************* Selections -- Functions for ***********************/
/************** handling inter-process communication via selections. *********/
/*****************************************************************************/

int
clip_GTK_TARGETLISTNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *ctargets = _clip_par(ClipMachineMemory, 1);

   CHECKARG(1, ARRAY_type_of_ClipVarType);

   if (ctargets)
    {
       guint     ntags = ctargets->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;

       GtkTargetEntry tags[ntags];

       GtkTargetList *list;

       C_object *clist;

       gint      i;

       for (i = 0; i < ntags; i++)
	  _array_to_target_entry(ClipMachineMemory, &ctargets->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar[i],
				 &tags[i]);
       list = gtk_target_list_new(tags, ntags);
       if (list)
	{
	   clist = _register_object(ClipMachineMemory, list, GTK_TYPE_TARGET_LIST, NULL, NULL);
	   if (clist)
	      _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &clist->obj);
	}
    }

   return 0;
 err:
   return 1;
}

int
clip_GTK_TARGETLISTREF(ClipMachine * ClipMachineMemory)
{
   C_object *ctlist = _fetch_co_arg(ClipMachineMemory);

   if (!ctlist || ctlist->type != GTK_TYPE_TARGET_LIST)
      goto err;

   gtk_target_list_ref((GtkTargetList *) (ctlist->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_TARGETLISTUNREF(ClipMachine * ClipMachineMemory)
{
   C_object *ctlist = _fetch_co_arg(ClipMachineMemory);

   if (!ctlist || ctlist->type != GTK_TYPE_TARGET_LIST)
      goto err;

   gtk_target_list_unref((GtkTargetList *) (ctlist->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_TARGETLISTADD(ClipMachine * ClipMachineMemory)
{
   C_object *ctlist = _fetch_co_arg(ClipMachineMemory);

   C_object *catom = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   guint     flags = _clip_parni(ClipMachineMemory, 3);

   guint     info = _clip_parni(ClipMachineMemory, 4);

   if (!ctlist || ctlist->type != GTK_TYPE_TARGET_LIST)
      goto err;
   if (!catom || catom->type != GDK_TYPE_ATOM)
      goto err;

   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);

   gtk_target_list_add((GtkTargetList *) (ctlist->object), (GdkAtom) (catom->object), flags, info);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TARGETLISTADDTABLE(ClipMachine * ClipMachineMemory)
{
   C_object *ctlist = _fetch_co_arg(ClipMachineMemory);

   ClipVar  *ctargets = _clip_par(ClipMachineMemory, 2);

   if (!ctlist || ctlist->type != GTK_TYPE_TARGET_LIST)
      goto err;

   CHECKARG(2, ARRAY_type_of_ClipVarType);

   if (ctargets)
    {
       guint     ntags = ctargets->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;

       GtkTargetEntry tags[ntags];

       gint      i;

       for (i = 0; i < ntags; i++)
	  _array_to_target_entry(ClipMachineMemory, &ctargets->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar[i],
				 &tags[i]);
       gtk_target_list_add_table((GtkTargetList *) (ctlist->object), tags, ntags);
    }

   return 0;
 err:
   return 1;
}

int
clip_GTK_TARGETLISTREMOVE(ClipMachine * ClipMachineMemory)
{
   C_object *ctlist = _fetch_co_arg(ClipMachineMemory);

   C_object *catom = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   if (!ctlist || ctlist->type != GTK_TYPE_TARGET_LIST)
      goto err;
   if (!catom || catom->type != GDK_TYPE_ATOM)
      goto err;

   gtk_target_list_remove((GtkTargetList *) (ctlist->object), (GdkAtom) (catom->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_TARGETLISTFIND(ClipMachine * ClipMachineMemory)
{
   C_object *ctlist = _fetch_co_arg(ClipMachineMemory);

   C_object *catom = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   guint     info;

   if (!ctlist || ctlist->type != GTK_TYPE_TARGET_LIST)
      goto err;
   if (!catom || catom->type != GDK_TYPE_ATOM)
      goto err;

   _clip_retl(ClipMachineMemory, gtk_target_list_find((GtkTargetList *) (ctlist->object), (GdkAtom) (catom->object), &info));

   _clip_storni(ClipMachineMemory, info, 3, 0);

   return 0;
 err:
   return 1;
}

int
clip_GTK_SELECTIONOWNERSET(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   C_object *cselection = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   guint32   time = _clip_parni(ClipMachineMemory, 3);

   CHECKCWID(cwid, GTK_IS_WIDGET);
   if (!cselection || cselection->type != GDK_TYPE_ATOM)
      goto err;
   CHECKARG(3, NUMERIC_type_of_ClipVarType);

   _clip_retl(ClipMachineMemory, gtk_selection_owner_set(GTK_WIDGET(cwid->widget), (GdkAtom) (cselection->object), time));

   return 0;
 err:
   return 1;
}

int
clip_GTK_SELECTIONADDTARGET(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   C_object *cselection = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_object *ctarget = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   guint     info = _clip_parni(ClipMachineMemory, 4);

   CHECKCWID(cwid, GTK_IS_WIDGET);
   if (!cselection || cselection->type != GDK_TYPE_ATOM)
      goto err;
   if (!ctarget || ctarget->type != GDK_TYPE_ATOM)
      goto err;
   CHECKARG(4, NUMERIC_type_of_ClipVarType);

   gtk_selection_add_target(GTK_WIDGET(cwid->widget), (GdkAtom) (cselection->object), (GdkAtom) (ctarget->object), info);

   return 0;
 err:
   return 1;
}

int
clip_GTK_SELECTIONADDTARGETS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   C_object *cselection = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   ClipVar  *ctargets = _clip_par(ClipMachineMemory, 3);

   CHECKCWID(cwid, GTK_IS_WIDGET);
   if (!cselection || cselection->type != GDK_TYPE_ATOM)
      goto err;
   CHECKARG(3, ARRAY_type_of_ClipVarType);

   if (ctargets)
    {
       guint     ntags = ctargets->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;

       GtkTargetEntry tags[ntags];

       gint      i;

       for (i = 0; i < ntags; i++)
	  _array_to_target_entry(ClipMachineMemory, &ctargets->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar[i],
				 &tags[i]);
       gtk_selection_add_targets(GTK_WIDGET(cwid->widget), (GdkAtom) (cselection->object), tags, ntags);
    }

   return 0;
 err:
   return 1;
}

int
clip_GTK_SELECTIONCLEARTARGETS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   C_object *cselection = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(cwid, GTK_IS_WIDGET);
   if (!cselection || cselection->type != GDK_TYPE_ATOM)
      goto err;

   gtk_selection_clear_targets(GTK_WIDGET(cwid->widget), (GdkAtom) (cselection->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_SELECTIONCONVERT(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   C_object *cselection = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_object *ctarget = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   guint32   time = _clip_parni(ClipMachineMemory, 4);

   CHECKCWID(cwid, GTK_IS_WIDGET);
   if (!cselection || cselection->type != GDK_TYPE_ATOM)
      goto err;
   if (!ctarget || ctarget->type != GDK_TYPE_ATOM)
      goto err;
   CHECKARG(4, NUMERIC_type_of_ClipVarType);

   gtk_selection_convert(GTK_WIDGET(cwid->widget), (GdkAtom) (cselection->object), (GdkAtom) (ctarget->object), time);

   return 0;
 err:
   return 1;
}

int
clip_GTK_SELECTIONDATASET(ClipMachine * ClipMachineMemory)
{
   C_object *cselection = _fetch_co_arg(ClipMachineMemory);

   C_object *ctype = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gint      format = _clip_parni(ClipMachineMemory, 3);

   const guchar *data = (const guchar *) _clip_parc(ClipMachineMemory, 4);

   gint      length = _clip_parni(ClipMachineMemory, 5);

   if (!cselection || cselection->type != GTK_TYPE_SELECTION_DATA)
      goto err;
   if (!ctype || ctype->type != GDK_TYPE_ATOM)
      goto err;
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, CHARACTER_type_of_ClipVarType);
   CHECKARG(5, NUMERIC_type_of_ClipVarType);

   gtk_selection_data_set((GtkSelectionData *) (cselection->object), (GdkAtom) (ctype->object), format, data, length);

   return 0;
 err:
   return 1;
}

int
clip_GTK_SELECTIONDATASETTEXT(ClipMachine * ClipMachineMemory)
{
   C_object *cselection = _fetch_co_arg(ClipMachineMemory);

   gchar    *str = _clip_parc(ClipMachineMemory, 2);

   gint      length = _clip_parni(ClipMachineMemory, 3);

   if (!cselection || cselection->type != GTK_TYPE_SELECTION_DATA)
      goto err;

   CHECKARG(2, CHARACTER_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);

   LOCALE_TO_UTF(str);
   _clip_retl(ClipMachineMemory, gtk_selection_data_set_text((GtkSelectionData *) (cselection->object), str, length));
   FREE_TEXT(str);

   return 0;
 err:
   return 1;
}

int
clip_GTK_SELECTIONDATAGETTEXT(ClipMachine * ClipMachineMemory)
{
   C_object *cselection = _fetch_co_arg(ClipMachineMemory);

   gchar    *str;

   if (!cselection || cselection->type != GTK_TYPE_SELECTION_DATA)
      goto err;

   str = (gchar *) gtk_selection_data_get_text((GtkSelectionData *) (cselection->object));
   LOCALE_FROM_UTF(str);
   _clip_retc(ClipMachineMemory, str);
   FREE_TEXT(str);

   return 0;
 err:
   return 1;
}

int
clip_GTK_SELECTIONDATAGETTARGETS(ClipMachine * ClipMachineMemory)
{
   C_object *cselection = _fetch_co_arg(ClipMachineMemory);

   ClipVar  *ctargets = _clip_par(ClipMachineMemory, 2);

   GdkAtom  *targets;

   gint      natoms;

   long      l;

   if (!cselection || cselection->type != GTK_TYPE_SELECTION_DATA)
      goto err;

   _clip_retl(ClipMachineMemory, gtk_selection_data_get_targets((GtkSelectionData *) (cselection->object), &targets, &natoms));

   l = natoms;
   _clip_array(ClipMachineMemory, ctargets, 1, &l);
   for (l = 0; l < natoms; l++)
    {
       C_object *ca = _register_object(ClipMachineMemory, targets[l], GDK_TYPE_ATOM, NULL, NULL);

       _clip_aset(ClipMachineMemory, ctargets, &ca->obj, 1, &l);
    }
   _clip_storni(ClipMachineMemory, natoms, 3, 0);

   g_free(targets);
   return 0;
 err:
   return 1;
}

int
clip_GTK_SELECTIONDATATARGETSINCLUDETEXT(ClipMachine * ClipMachineMemory)
{
   C_object *cselection = _fetch_co_arg(ClipMachineMemory);

   if (!cselection || cselection->type != GTK_TYPE_SELECTION_DATA)
      goto err;

   _clip_retl(ClipMachineMemory, gtk_selection_data_targets_include_text((GtkSelectionData *) (cselection->object)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_SELECTIONREMOVEALL(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cwid, GTK_IS_WIDGET);

   gtk_selection_remove_all(GTK_WIDGET(cwid->widget));

   return 0;
 err:
   return 1;
}

int
clip_GTK_SELECTIONDATACOPY(ClipMachine * ClipMachineMemory)
{
   C_object *cselection = _fetch_co_arg(ClipMachineMemory);

   GtkSelectionData *snew;

   C_object *cnew;

   if (!cselection || cselection->type != GTK_TYPE_SELECTION_DATA)
      goto err;

   snew = gtk_selection_data_copy((GtkSelectionData *) (cselection->object));

   if (snew)
    {
       cnew = _register_object(ClipMachineMemory, snew, GTK_TYPE_SELECTION_DATA, NULL, NULL);
       if (cnew)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cnew->obj);
    }

   return 0;
 err:
   return 1;
}

int
clip_GTK_SELECTIONDATAFREE(ClipMachine * ClipMachineMemory)
{
   C_object *cselection = _fetch_co_arg(ClipMachineMemory);

   if (!cselection || cselection->type != GTK_TYPE_SELECTION_DATA)
      goto err;

   gtk_selection_data_free((GtkSelectionData *) (cselection->object));

   return 0;
 err:
   return 1;
}

#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 6)
int
clip_GTK_SELECTIONDATAGETPIXBUF(ClipMachine * ClipMachineMemory)
{
   C_object *cselection = _fetch_co_arg(ClipMachineMemory);

   GdkPixbuf *pix;

   C_object *cpix;

   if (!cselection || cselection->type != GTK_TYPE_SELECTION_DATA)
      goto err;

   pix = gtk_selection_data_get_pixbuf((GtkSelectionData *) (cselection->object));

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
clip_GTK_SELECTIONDATAGETURIS(ClipMachine * ClipMachineMemory)
{
   C_object *cselection = _fetch_co_arg(ClipMachineMemory);

   gchar   **uris;

   ClipVar  *cv = RETPTR(ClipMachineMemory);

   if (!cselection || cselection->type != GTK_TYPE_SELECTION_DATA)
      goto err;

   uris = gtk_selection_data_get_uris((GtkSelectionData *) (cselection->object));

   if (uris)
    {
       long      l;

       gchar   **u = uris;

       _clip_array(ClipMachineMemory, cv, 1, 0);

       for (l = 0; u; l++)
	{
	   ClipVar  *s = 0;

	   _clip_var_str(u[0], strlen(u[0]), s);
	   _clip_aadd(ClipMachineMemory, cv, s);
	}

       g_strfreev(uris);
    }

   return 0;
 err:
   return 1;
}

int
clip_GTK_SELECTIONDATASETPIXBUF(ClipMachine * ClipMachineMemory)
{
   C_object *cselection = _fetch_co_arg(ClipMachineMemory);

   C_object *cpix = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   if (!cselection || cselection->type != GTK_TYPE_SELECTION_DATA)
      goto err;
   CHECKCOBJ(cpix, GDK_IS_PIXBUF(cpix->object));

   gtk_selection_data_set_pixbuf((GtkSelectionData *) (cselection->object), GDK_PIXBUF(cpix->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_SELECTIONDATASETURIS(ClipMachine * ClipMachineMemory)
{
   C_object *cselection = _fetch_co_arg(ClipMachineMemory);

   ClipArrVar *ca = (ClipArrVar *) _clip_vptr(_clip_spar(ClipMachineMemory, 2));

   gchar   **uris = 0;

   long      l;

   int       i;

   if (!cselection || cselection->type != GTK_TYPE_SELECTION_DATA)
      goto err;
   CHECKARG(2, ARRAY_type_of_ClipVarType);

   l = ca->count_of_ClipArrVar;
   *uris = calloc(l + 1, sizeof(gchar));
   *uris[l + 1] = 0;
   for (i = 0; i < l; i++)
    {
       ClipVar  *s = ca->ClipVar_items_of_ClipArrVar + i;

       if (s->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType)
	  goto err;

       strcpy(uris[i], s->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
    }
   gtk_selection_data_set_uris((GtkSelectionData *) (cselection->object), uris);
   g_strfreev(uris);

   return 0;
 err:
   return 1;
}

int
clip_GTK_SELECTIONDATATARGETSINCLUDEIMAGE(ClipMachine * ClipMachineMemory)
{
   C_object *cselection = _fetch_co_arg(ClipMachineMemory);

   gboolean  writable = _clip_parl(ClipMachineMemory, 2);

   if (!cselection || cselection->type != GTK_TYPE_SELECTION_DATA)
      goto err;
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   _clip_retl(ClipMachineMemory, gtk_selection_data_targets_include_image((GtkSelectionData *) (cselection->object), writable));

   return 0;
 err:
   return 1;
}
#endif
#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 6)
int
clip_GTK_TARGETLISTADDIMAGETARGETS(ClipMachine * ClipMachineMemory)
{
   C_object *ctlist = _fetch_co_arg(ClipMachineMemory);

   guint     info = _clip_parni(ClipMachineMemory, 2);

   gboolean  writable = _clip_parl(ClipMachineMemory, 3);

   if (!ctlist || ctlist->type != GTK_TYPE_TARGET_LIST)
      goto err;

   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, LOGICAL_type_of_ClipVarType);

   gtk_target_list_add_image_targets((GtkTargetList *) (ctlist->object), info, writable);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TARGETLISTADDTEXTTARGETS(ClipMachine * ClipMachineMemory)
{
   C_object *ctlist = _fetch_co_arg(ClipMachineMemory);

   guint     info = _clip_parni(ClipMachineMemory, 2);

   if (!ctlist || ctlist->type != GTK_TYPE_TARGET_LIST)
      goto err;

   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_target_list_add_text_targets((GtkTargetList *) (ctlist->object), info);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TARGETLISTADDURITARGETS(ClipMachine * ClipMachineMemory)
{
   C_object *ctlist = _fetch_co_arg(ClipMachineMemory);

   guint     info = _clip_parni(ClipMachineMemory, 2);

   if (!ctlist || ctlist->type != GTK_TYPE_TARGET_LIST)
      goto err;

   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_target_list_add_uri_targets((GtkTargetList *) (ctlist->object), info);

   return 0;
 err:
   return 1;
}
#endif
