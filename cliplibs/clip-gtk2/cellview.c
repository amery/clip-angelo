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
_gtk_type_cell_view()
{
   return GTK_TYPE_CELL_VIEW;
}

long
_clip_type_cell_view()
{
   return GTK_WIDGET_CELL_VIEW;
}

const char *
_clip_type_name_cell_view()
{
   return "GTK_WIDGET_CELL_VIEW";
}

int
clip_INIT___CELL_VIEW(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_cell_view, _clip_type_name_cell_view, _gtk_type_cell_view, _gtk_type_widget, NULL);
   return 0;
}

/**********************************************************/

int
clip_GTK_CELLVIEWNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   GtkWidget *wid = NULL;
   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);

   wid = gtk_cell_view_new();

   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

int
clip_GTK_CELLVIEWNEWWITHTEXT(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   gchar *text = _clip_parc(ClipMachineMemory, 2);
   GtkWidget *wid = NULL;
   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(text);
   wid = gtk_cell_view_new_with_text((const gchar *) text);
   FREE_TEXT(text);

   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

int
clip_GTK_CELLVIEWNEWWITHMARKUP(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   gchar *markup = _clip_parc(ClipMachineMemory, 2);
   GtkWidget *wid = NULL;
   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(markup);
   wid = gtk_cell_view_new_with_markup((const gchar *) markup);
   FREE_TEXT(markup);

   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

int
clip_GTK_CELLVIEWNEWWITHPIXBUF(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   C_object *cpix = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   GtkWidget *wid = NULL;
   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(cpix, GDK_IS_PIXBUF(cpix->object));

   wid = gtk_cell_view_new_with_pixbuf(GDK_PIXBUF(cpix->object));

   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

int
clip_GTK_CELLVIEWSETMODEL(ClipMachine * ClipMachineMemory)
{
   C_widget *ccell = _fetch_cw_arg(ClipMachineMemory);
   C_object *ClipMachineMemoryodel = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(ccell, GTK_IS_CELL_VIEW);
   CHECKCOBJ(ClipMachineMemoryodel, GTK_IS_TREE_MODEL(ClipMachineMemoryodel->object));

   gtk_cell_view_set_model(GTK_CELL_VIEW(ccell->widget), GTK_TREE_MODEL(ClipMachineMemoryodel->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_CELLVIEWSETDISPLAYEDROW(ClipMachine * ClipMachineMemory)
{
   C_widget *ccell = _fetch_cw_arg(ClipMachineMemory);
   C_object *cpath = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(ccell, GTK_IS_CELL_VIEW);
   CHECKCOBJ(cpath, GTK_IS_TREE_PATH(cpath->object));

   gtk_cell_view_set_displayed_row(GTK_CELL_VIEW(ccell->widget), GTK_TREE_PATH(cpath->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_CELLVIEWGETDISPLAYEDROW(ClipMachine * ClipMachineMemory)
{
   C_widget *ccell = _fetch_cw_arg(ClipMachineMemory);
   GtkTreePath *path;
   C_object *cpath;

   CHECKCWID(ccell, GTK_IS_CELL_VIEW);

   path = gtk_cell_view_get_displayed_row(GTK_CELL_VIEW(ccell->widget));

   if (path)
      {
	 cpath = _list_get_cobject(ClipMachineMemory, path);
	 if (!cpath)
	    cpath = _register_object(ClipMachineMemory, path, GTK_TYPE_TREE_PATH, NULL, NULL);
	 if (cpath)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cpath->obj);
      }

   return 0;
 err:
   return 1;
}

int
clip_GTK_CELLVIEWGETSIZEOFROW(ClipMachine * ClipMachineMemory)
{
   C_widget *ccell = _fetch_cw_arg(ClipMachineMemory);
   C_object *cpath = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   ClipVar *cv = _clip_spar(ClipMachineMemory, 3);
   GtkRequisition requisition;

   CHECKCWID(ccell, GTK_IS_CELL_VIEW);
   CHECKCOBJ(cpath, GTK_IS_TREE_PATH(cpath->object));

   _clip_retl(ClipMachineMemory, gtk_cell_view_get_size_of_row(GTK_CELL_VIEW(ccell->widget), GTK_TREE_PATH(cpath->object), &requisition));

   _clip_map(ClipMachineMemory, cv);
   _clip_mputn(ClipMachineMemory, cv, HASH_WIDTH, requisition.width);
   _clip_mputn(ClipMachineMemory, cv, HASH_HEIGHT, requisition.height);

   return 0;
 err:
   return 1;
}

int
clip_GTK_CELLVIEWSETBACKGROUNDCOLOR(ClipMachine * ClipMachineMemory)
{
   C_widget *ccell = _fetch_cw_arg(ClipMachineMemory);
   ClipVar *ccolor = _clip_spar(ClipMachineMemory, 2);
   GdkColor *color = 0;

   CHECKCWID(ccell, GTK_IS_CELL_VIEW);
   CHECKARG(2, MAP_type_of_ClipVarType);

   _map_colors_to_gdk(ClipMachineMemory, ccolor, color);

   gtk_cell_view_set_background_color(GTK_CELL_VIEW(ccell->widget), color);

   return 0;
 err:
   return 1;
}

int
clip_GTK_CELLVIEWGETCELLRENDERERS(ClipMachine * ClipMachineMemory)
{
   C_widget *ccell = _fetch_cw_arg(ClipMachineMemory);
   ClipVar *cv = RETPTR(ClipMachineMemory);
   GList *list;
   long l;

   CHECKCWID(ccell, GTK_IS_CELL_VIEW);

   list = gtk_cell_view_get_cell_renderers(GTK_CELL_VIEW(ccell->widget));
   l = g_list_length(list);
   _clip_array(ClipMachineMemory, cv, 1, &l);
   for (l = 0; list; list = g_list_next(list), l++)
      {
	 C_object *cren;
	 GtkCellRenderer *ren;

	 ren = GTK_CELL_RENDERER(list->data);
	 if (ren)
	    {
	       cren = _list_get_cobject(ClipMachineMemory, ren);
	       if (!cren)
		  cren = _register_object(ClipMachineMemory, ren, GTK_TYPE_CELL_RENDERER, NULL, NULL);
	       if (cren)
		  _clip_aset(ClipMachineMemory, cv, &cren->obj, 1, &l);
	    }
      }

   g_list_free(list);

   return 0;
 err:
   return 1;
}
