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

#include "ci_inkey.ch"
#include "ci_clip-gtk2.ch"
#include "ci_clip-gtk2.h"

/* ctag - array of array with items: 1  target - string - drag type  */
/*                                   2  flags  - numeric - target flags */
/*                                   3  info   - numeric */
int
clip_GTK_DRAGDESTSET(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   GtkDestDefaults flags = _clip_parni(ClipMachineMemory, 2);

   ClipArrVar *ctag = (ClipArrVar *) _clip_vptr(_clip_par(ClipMachineMemory, 3));

   guint     ntags = _clip_parni(ClipMachineMemory, 4);

   GdkDragAction actions = _clip_parni(ClipMachineMemory, 5);

   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, ARRAY_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);
   CHECKARG(5, NUMERIC_type_of_ClipVarType);

   if (ctag)
    {
       GtkTargetEntry *tags;

       gint      i;

       tags = malloc(ntags * sizeof(GtkTargetEntry));
       memset(tags, 0, sizeof(GtkTargetEntry) * ntags);
       for (i = 0; i < ntags; i++)
	{
	   GtkTargetEntry t;

	   _array_to_target_entry(ClipMachineMemory, &ctag->ClipVar_items_of_ClipArrVar[i], &t);
	   tags[i] = t;
	}

       gtk_drag_dest_set(GTK_WIDGET(cwid->widget), flags, tags, ntags, actions);
       free(tags);
    }
   else
      gtk_drag_dest_set(GTK_WIDGET(cwid->widget), flags, NULL, ntags, actions);

   return 0;
 err:
   return 1;
}

int
clip_GTK_DRAGDESTSETPROXY(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   C_object *cwin = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   GdkDragProtocol protocol = _clip_parni(ClipMachineMemory, 3);

   gboolean  use_coordinates = _clip_parl(ClipMachineMemory, 4);

   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKCOBJ(cwin, GDK_IS_WINDOW(cwin->object));
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, LOGICAL_type_of_ClipVarType);

   gtk_drag_dest_set_proxy(GTK_WIDGET(cwid->widget), GDK_WINDOW(cwin->object), protocol, use_coordinates);

   return 0;
 err:
   return 1;
}

int
clip_GTK_DRAGDESTUNSET(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cwid, GTK_IS_WIDGET);

   gtk_drag_dest_unset(GTK_WIDGET(cwid->widget));

   return 0;
 err:
   return 1;
}

int
clip_GTK_DRAGDESTFINDTARGET(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   C_object *ccontext = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_object *ctlist = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   GdkAtom   atom;

   C_object *catom;

   CHECKCWID(cwid, GTK_IS_WIDGET);
   if (!ccontext || ccontext->type != GDK_TYPE_DRAG_CONTEXT)
      goto err;
   if (!ctlist || ctlist->type != GTK_TYPE_TARGET_LIST)
      goto err;

   atom = gtk_drag_dest_find_target(GTK_WIDGET(cwid->widget),
				    (GdkDragContext *) ccontext->object, (GtkTargetList *) ctlist->object);
   if (atom)
    {
       catom = _list_get_cobject(ClipMachineMemory, &atom);
       if (!catom)
	  catom = _register_object(ClipMachineMemory, &atom, GDK_TYPE_ATOM, NULL, NULL);
       if (catom)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &catom->obj);
    }

   return 0;
 err:
   return 1;
}

int
clip_GTK_DRAGDESTGETTARGETLIST(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   GtkTargetList *list;

   C_object *ctlist;

   CHECKCWID(cwid, GTK_IS_WIDGET);

   list = gtk_drag_dest_get_target_list(GTK_WIDGET(cwid->widget));

   if (list)
    {
       ctlist = _list_get_cobject(ClipMachineMemory, list);
       if (!ctlist)
	  ctlist = _register_object(ClipMachineMemory, list, GDK_TYPE_ATOM, NULL, NULL);
       if (ctlist)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &ctlist->obj);
    }

   return 0;
 err:
   return 1;
}

int
clip_GTK_DRAGDESTSETTARGETLIST(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   C_object *ctlist = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(cwid, GTK_IS_WIDGET);
   if (!ctlist || ctlist->type != GTK_TYPE_TARGET_LIST)
      goto err;

   gtk_drag_dest_set_target_list(GTK_WIDGET(cwid->widget), (GtkTargetList *) ctlist);

   return 0;
 err:
   return 1;
}

int
clip_GTK_DRAGFINISH(ClipMachine * ClipMachineMemory)
{
   C_object *ccontext = _fetch_co_arg(ClipMachineMemory);

   gboolean  success = _clip_parl(ClipMachineMemory, 2);

   gboolean  del = _clip_parl(ClipMachineMemory, 3);

   guint32   time = _clip_parni(ClipMachineMemory, 4);

   if (!ccontext || ccontext->type != GDK_TYPE_DRAG_CONTEXT)
      goto err;
   CHECKARG(2, LOGICAL_type_of_ClipVarType);
   CHECKARG(3, LOGICAL_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);

   gtk_drag_finish((GdkDragContext *) ccontext->object, success, del, time);

   return 0;
 err:
   return 1;
}

int
clip_GTK_DRAGGETDATA(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   C_object *ccontext = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_object *catom = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   guint32   time = _clip_parni(ClipMachineMemory, 4);

   if (!ccontext || ccontext->type != GDK_TYPE_DRAG_CONTEXT)
      goto err;
   CHECKCWID(cwid, GTK_IS_WIDGET);
   if (!catom || catom->type != GDK_TYPE_ATOM)
      goto err;
   CHECKARG(4, NUMERIC_type_of_ClipVarType);

   gtk_drag_get_data(GTK_WIDGET(cwid->widget), (GdkDragContext *) ccontext->object, (GdkAtom) (catom->object), time);

   return 0;
 err:
   return 1;
}

int
clip_GTK_DRAGGETSOURCEWIDGET(ClipMachine * ClipMachineMemory)
{
   C_object *ccontext = _fetch_co_arg(ClipMachineMemory);

   GtkWidget *wid;

   C_widget *cwid;

   if (!ccontext || ccontext->type != GDK_TYPE_DRAG_CONTEXT)
      goto err;

   wid = gtk_drag_get_source_widget((GdkDragContext *) ccontext->object);

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
clip_GTK_DRAGHIGHLIGHT(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cwid, GTK_IS_WIDGET);

   gtk_drag_highlight(GTK_WIDGET(cwid->widget));

   return 0;
 err:
   return 1;
}

int
clip_GTK_DRAGUNHIGHLIGHT(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cwid, GTK_IS_WIDGET);

   gtk_drag_unhighlight(GTK_WIDGET(cwid->widget));

   return 0;
 err:
   return 1;
}

int
clip_GTK_DRAGBEGIN(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   C_object *ctlist = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   GdkDragAction actions = _clip_parni(ClipMachineMemory, 3);

   gint      button = _clip_parni(ClipMachineMemory, 4);

   C_object *cevent = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 5));

   GdkDragContext *context;

   C_object *ccontext;

   CHECKCWID(cwid, GTK_IS_WIDGET);
   if (!ctlist || ctlist->type != GTK_TYPE_TARGET_LIST)
      goto err;
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);
   if (!cevent || cevent->type != GDK_TYPE_EVENT)
      goto err;

   context = gtk_drag_begin(GTK_WIDGET(cwid->widget),
			    (GtkTargetList *) ctlist->object, actions, button, (GdkEvent *) cevent->object);

   if (context)
    {
       ccontext = _list_get_cobject(ClipMachineMemory, context);
       if (!ccontext)
	  ccontext = _register_object(ClipMachineMemory, context, GDK_TYPE_DRAG_CONTEXT, NULL, NULL);
       if (ccontext)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &ccontext->obj);
    }

   return 0;
 err:
   return 1;
}

int
clip_GTK_DRAGSETICONWIDGET(ClipMachine * ClipMachineMemory)
{
   C_object *ccontext = _fetch_co_arg(ClipMachineMemory);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gint      hot_x = _clip_parni(ClipMachineMemory, 3);

   gint      hot_y = _clip_parni(ClipMachineMemory, 4);

   CHECKCWID(cwid, GTK_IS_WIDGET);
   if (!ccontext || ccontext->type != GDK_TYPE_DRAG_CONTEXT)
      goto err;
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);

   gtk_drag_set_icon_widget((GdkDragContext *) ccontext->object, GTK_WIDGET(cwid->widget), hot_x, hot_y);

   return 0;
 err:
   return 1;
}

int
clip_GTK_DRAGSETICONPIXMAP(ClipMachine * ClipMachineMemory)
{
   C_object *ccontext = _fetch_co_arg(ClipMachineMemory);

   C_object *ccolormap = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_object *cpixmap = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   C_object *ClipMachineMemoryask = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 4));

   gint      hot_x = _clip_parni(ClipMachineMemory, 5);

   gint      hot_y = _clip_parni(ClipMachineMemory, 6);

   if (!ccontext || ccontext->type != GDK_TYPE_DRAG_CONTEXT)
      goto err;
   CHECKCOBJ(ccolormap, GDK_IS_COLORMAP(ccolormap->object));
   CHECKCOBJ(cpixmap, GDK_IS_PIXMAP(cpixmap->object));
   CHECKCOBJ(ClipMachineMemoryask, GDK_IS_BITMAP(ClipMachineMemoryask));
   CHECKARG(5, NUMERIC_type_of_ClipVarType);
   CHECKARG(6, NUMERIC_type_of_ClipVarType);

   gtk_drag_set_icon_pixmap((GdkDragContext *) ccontext->object,
			    GDK_COLORMAP(ccolormap->object),
			    GDK_PIXMAP(cpixmap->object), GDK_BITMAP(ClipMachineMemoryask->object), hot_x, hot_y);

   return 0;
 err:
   return 1;
}

int
clip_GTK_DRAGSETICONPIXBUF(ClipMachine * ClipMachineMemory)
{
   C_object *ccontext = _fetch_co_arg(ClipMachineMemory);

   C_object *cpixbuf = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gint      hot_x = _clip_parni(ClipMachineMemory, 3);

   gint      hot_y = _clip_parni(ClipMachineMemory, 4);

   if (!ccontext || ccontext->type != GDK_TYPE_DRAG_CONTEXT)
      goto err;
   CHECKCOBJ(cpixbuf, GDK_IS_PIXBUF(cpixbuf->object));
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);

   gtk_drag_set_icon_pixbuf((GdkDragContext *) ccontext->object, GDK_PIXBUF(cpixbuf->object), hot_x, hot_y);

   return 0;
 err:
   return 1;
}

int
clip_GTK_DRAGSETICONSTOCK(ClipMachine * ClipMachineMemory)
{
   C_object *ccontext = _fetch_co_arg(ClipMachineMemory);

   const gchar *stock = _clip_parc(ClipMachineMemory, 2);

   gint      hot_x = _clip_parni(ClipMachineMemory, 3);

   gint      hot_y = _clip_parni(ClipMachineMemory, 4);

   if (!ccontext || ccontext->type != GDK_TYPE_DRAG_CONTEXT)
      goto err;
   CHECKARG(2, CHARACTER_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);

   gtk_drag_set_icon_stock((GdkDragContext *) ccontext->object, stock, hot_x, hot_y);

   return 0;
 err:
   return 1;
}

int
clip_GTK_DRAGSETICONDEFAULT(ClipMachine * ClipMachineMemory)
{
   C_object *ccontext = _fetch_co_arg(ClipMachineMemory);

   if (!ccontext || ccontext->type != GDK_TYPE_DRAG_CONTEXT)
      goto err;

   gtk_drag_set_icon_default((GdkDragContext *) ccontext->object);

   return 0;
 err:
   return 1;
}

int
clip_GTK_DRAGSETDEFAULTICON(ClipMachine * ClipMachineMemory)
{
   C_object *ccolormap = _fetch_co_arg(ClipMachineMemory);

   C_object *cpixmap = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_object *ClipMachineMemoryask = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   gint      hot_x = _clip_parni(ClipMachineMemory, 4);

   gint      hot_y = _clip_parni(ClipMachineMemory, 5);

   CHECKCOBJ(ccolormap, GDK_IS_COLORMAP(ccolormap->object));
   CHECKCOBJ(cpixmap, GDK_IS_PIXMAP(cpixmap->object));
   CHECKCOBJ(ClipMachineMemoryask, GDK_IS_PIXMAP(ClipMachineMemoryask));
   CHECKARG(5, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);

   gtk_drag_set_default_icon(GDK_COLORMAP(ccolormap->object),
			     GDK_PIXMAP(cpixmap->object), GDK_BITMAP(ClipMachineMemoryask->object), hot_x, hot_y);

   return 0;
 err:
   return 1;
}

int
clip_GTK_DRAGCHECKTHRESHOLD(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   gint      start_x = _clip_parni(ClipMachineMemory, 2);

   gint      start_y = _clip_parni(ClipMachineMemory, 3);

   gint      current_x = _clip_parni(ClipMachineMemory, 4);

   gint      current_y = _clip_parni(ClipMachineMemory, 5);

   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(5, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);

   _clip_retl(ClipMachineMemory, gtk_drag_check_threshold(GTK_WIDGET(cwid->widget), start_x, start_y, current_x, current_y));

   return 0;
 err:
   return 1;
}

int
clip_GTK_DRAGSOURCESET(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   GdkModifierType start_button_mask = _clip_parni(ClipMachineMemory, 2);

   ClipArrVar *ctag = (ClipArrVar *) _clip_vptr(_clip_spar(ClipMachineMemory, 3));

   gint      ntargets = _clip_parni(ClipMachineMemory, 4);

   GdkDragAction actions = _clip_parni(ClipMachineMemory, 5);

   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, ARRAY_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);
   CHECKARG(5, NUMERIC_type_of_ClipVarType);

   if (ctag)
    {
       GtkTargetEntry *tags;

       gint      i;

       tags = malloc(ctag->count_of_ClipArrVar * sizeof(GtkTargetEntry));
       memset(tags, 0, sizeof(GtkTargetEntry) * ctag->count_of_ClipArrVar);
       for (i = 0; i < ctag->count_of_ClipArrVar; i++)
	  _array_to_target_entry(ClipMachineMemory, &ctag->ClipVar_items_of_ClipArrVar[i], &tags[i]);
       gtk_drag_source_set(GTK_WIDGET(cwid->widget), start_button_mask, tags, ntargets, actions);
       free(tags);
    }
   else
      gtk_drag_source_set(GTK_WIDGET(cwid->widget), start_button_mask, NULL, ntargets, actions);

   return 0;
 err:
   return 1;
}

int
clip_GTK_DRAGSOURCESETICON(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   C_object *ccolormap = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_object *cpixmap = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   C_object *ClipMachineMemoryask = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 4));

   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKCOBJ(ccolormap, GDK_IS_COLORMAP(ccolormap->object));
   CHECKCOBJ(cpixmap, GDK_IS_PIXMAP(cpixmap->object));
   CHECKCOBJ(ClipMachineMemoryask, GDK_IS_BITMAP(ClipMachineMemoryask));

   gtk_drag_source_set_icon(GTK_WIDGET(cwid->widget),
			    GDK_COLORMAP(ccolormap->object),
			    GDK_PIXMAP(cpixmap->object), GDK_BITMAP(ClipMachineMemoryask->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_DRAGSOURCESETICONPIXBUF(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   C_object *cpixbuf = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKCOBJ(cpixbuf, GDK_IS_PIXBUF(cpixbuf->object));

   gtk_drag_source_set_icon_pixbuf(GTK_WIDGET(cwid->widget), GDK_PIXBUF(cpixbuf->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_DRAGSOURCESETICONSTOCK(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   const gchar *stock = _clip_parc(ClipMachineMemory, 2);

   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   gtk_drag_source_set_icon_stock(GTK_WIDGET(cwid->widget), stock);

   return 0;
 err:
   return 1;
}

int
clip_GTK_DRAGSOURCEUNSET(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cwid, GTK_IS_WIDGET);

   gtk_drag_source_unset(GTK_WIDGET(cwid->widget));

   return 0;
 err:
   return 1;
}

/******************************************************************************
******* GDK Drag and Drop functions *******************************************
******************************************************************************/

int
clip_GDK_DRAGGETSELECTION(ClipMachine * ClipMachineMemory)
{
   C_object *ccontext = _fetch_co_arg(ClipMachineMemory);

   GdkAtom   atom;

   C_object *catom;

   if (!ccontext || ccontext->type != GDK_TYPE_DRAG_CONTEXT)
      goto err;

   atom = gdk_drag_get_selection((GdkDragContext *) ccontext->object);

   if (atom)
    {
       catom = _list_get_cobject(ClipMachineMemory, atom);
       if (!catom)
	  catom = _register_object(ClipMachineMemory, atom, GDK_TYPE_ATOM, NULL, NULL);
       if (catom)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &catom->obj);
    }
   return 0;
 err:
   return 1;
}

int
clip_GDK_DRAGABORT(ClipMachine * ClipMachineMemory)
{
   C_object *ccontext = _fetch_co_arg(ClipMachineMemory);

   guint32   time = _clip_parni(ClipMachineMemory, 2);

   if (!ccontext || ccontext->type != GDK_TYPE_DRAG_CONTEXT)
      goto err;
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gdk_drag_abort((GdkDragContext *) ccontext->object, time);

   return 0;
 err:
   return 1;
}

int
clip_GDK_DROPREPLY(ClipMachine * ClipMachineMemory)
{
   C_object *ccontext = _fetch_co_arg(ClipMachineMemory);

   gboolean  ok = _clip_parl(ClipMachineMemory, 2);

   guint32   time = _clip_parni(ClipMachineMemory, 3);

   if (!ccontext || ccontext->type != GDK_TYPE_DRAG_CONTEXT)
      goto err;
   CHECKARG(2, LOGICAL_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);

   gdk_drop_reply((GdkDragContext *) ccontext->object, ok, time);

   return 0;
 err:
   return 1;
}

int
clip_GDK_DRAGCONTEXTNEW(ClipMachine * ClipMachineMemory)
{
   GdkDragContext *context;

   C_object *ccontext;

   context = gdk_drag_context_new();

   if (context)
    {
       ccontext = _list_get_cobject(ClipMachineMemory, context);
       if (!ccontext)
	  ccontext = _register_object(ClipMachineMemory, context, GDK_TYPE_DRAG_CONTEXT, NULL, NULL);
       if (ccontext)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &ccontext->obj);
    }

   return 0;
}

int
clip_GDK_DRAGDROP(ClipMachine * ClipMachineMemory)
{
   C_object *ccontext = _fetch_co_arg(ClipMachineMemory);

   guint32   time = _clip_parni(ClipMachineMemory, 2);

   if (!ccontext || ccontext->type != GDK_TYPE_DRAG_CONTEXT)
      goto err;
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gdk_drag_drop((GdkDragContext *) ccontext->object, time);

   return 0;
 err:
   return 1;
}

int
clip_GDK_DRAGFINDWINDOW(ClipMachine * ClipMachineMemory)
{
   C_object *ccontext = _fetch_co_arg(ClipMachineMemory);

   C_object *cgdk_win = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gint      x_root = _clip_parni(ClipMachineMemory, 3);

   gint      y_root = _clip_parni(ClipMachineMemory, 4);

   ClipVar  *cv = _clip_spar(ClipMachineMemory, 5);

   GdkWindow *win;

   C_object *cwin;

   GdkDragProtocol protocol;

   if (!ccontext || ccontext->type != GDK_TYPE_DRAG_CONTEXT)
      goto err;
   CHECKCOBJ(cgdk_win, GDK_IS_WINDOW(cgdk_win->object));
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);

   gdk_drag_find_window((GdkDragContext *) ccontext->object, GDK_WINDOW(cgdk_win->object), x_root, y_root, &win, &protocol);

   if (win)
    {
       cwin = _list_get_cobject(ClipMachineMemory, win);
       if (!cwin)
	  cwin = _register_object(ClipMachineMemory, win, GDK_TYPE_WINDOW, NULL, NULL);
       if (cwin)
	  _clip_mclone(ClipMachineMemory, cv, &cwin->obj);
       _clip_storni(ClipMachineMemory, protocol, 6, 0);
    }
   return 0;
 err:
   return 1;
}

int
clip_GDK_DRAGBEGIN(ClipMachine * ClipMachineMemory)
{
   C_object *cwin = _fetch_co_arg(ClipMachineMemory);

   ClipVar  *cv = _clip_spar(ClipMachineMemory, 2);

   GList    *list = 0;

   gint      i;

   CHECKCOBJ(cwin, GDK_IS_WINDOW(cwin->object));
   CHECKARG(2, ARRAY_type_of_ClipVarType);

   for (i = 0; i < cv->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar; i++)
      g_list_append(list, &cv->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar[i]);

   gdk_drag_begin(GDK_WINDOW(cwin->object), list);

   return 0;
 err:
   return 1;
}

int
clip_GDK_DRAGMOTION(ClipMachine * ClipMachineMemory)
{
   C_object *ccontext = _fetch_co_arg(ClipMachineMemory);

   C_object *cgdk_win = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   GdkDragProtocol protocol = _clip_parni(ClipMachineMemory, 3);

   gint      x_root = _clip_parni(ClipMachineMemory, 4);

   gint      y_root = _clip_parni(ClipMachineMemory, 5);

   GdkDragAction suggested_action = _clip_parni(ClipMachineMemory, 6);

   GdkDragAction possible_actions = _clip_parni(ClipMachineMemory, 7);

   guint32   time = _clip_parni(ClipMachineMemory, 8);

   if (!ccontext || ccontext->type != GDK_TYPE_DRAG_CONTEXT)
      goto err;
   CHECKCOBJ(cgdk_win, GDK_IS_WINDOW(cgdk_win->object));
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);
   CHECKARG(5, NUMERIC_type_of_ClipVarType);
   CHECKARG(6, NUMERIC_type_of_ClipVarType);
   CHECKARG(7, NUMERIC_type_of_ClipVarType);
   CHECKARG(8, NUMERIC_type_of_ClipVarType);

   gdk_drag_motion((GdkDragContext *) ccontext->object,
		   GDK_WINDOW(cgdk_win->object), protocol, x_root, y_root, suggested_action, possible_actions, time);

   return 0;
 err:
   return 1;
}

int
clip_GDK_DROPFINISH(ClipMachine * ClipMachineMemory)
{
   C_object *ccontext = _fetch_co_arg(ClipMachineMemory);

   gboolean  success = _clip_parl(ClipMachineMemory, 2);

   guint32   time = _clip_parni(ClipMachineMemory, 3);

   if (!ccontext || ccontext->type != GDK_TYPE_DRAG_CONTEXT)
      goto err;
   CHECKARG(2, LOGICAL_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);

   gdk_drop_finish((GdkDragContext *) ccontext->object, success, time);

   return 0;
 err:
   return 1;
}

int
clip_GDK_DRAGGETPROTOCOL(ClipMachine * ClipMachineMemory)
{
   guint32   xid = _clip_parni(ClipMachineMemory, 1);

   GdkDragProtocol protocol;

   CHECKARG(1, NUMERIC_type_of_ClipVarType);

   _clip_retni(ClipMachineMemory, gdk_drag_get_protocol(xid, &protocol));
   _clip_storni(ClipMachineMemory, protocol, 2, 0);
   return 0;
 err:
   return 1;
}

int
clip_GDK_DRAGSTATUS(ClipMachine * ClipMachineMemory)
{
   C_object *ccontext = _fetch_co_arg(ClipMachineMemory);

   GdkDragAction action = _clip_parni(ClipMachineMemory, 2);

   guint32   time = _clip_parni(ClipMachineMemory, 3);

   if (!ccontext || ccontext->type != GDK_TYPE_DRAG_CONTEXT)
      goto err;
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);

   gdk_drag_status((GdkDragContext *) ccontext->object, action, time);
   return 0;
 err:
   return 1;
}

#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 6)
int
clip_GTK_DRAGDESTADDIMAGETARGETS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cwid, GTK_IS_WIDGET);

   gtk_drag_dest_add_image_targets(GTK_WIDGET(cwid->widget));

   return 0;
 err:
   return 1;
}

int
clip_GTK_DRAGDESTADDTEXTTARGETS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cwid, GTK_IS_WIDGET);

   gtk_drag_dest_add_text_targets(GTK_WIDGET(cwid->widget));

   return 0;
 err:
   return 1;
}

int
clip_GTK_DRAGDESTADDURITARGETS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cwid, GTK_IS_WIDGET);

   gtk_drag_dest_add_uri_targets(GTK_WIDGET(cwid->widget));

   return 0;
 err:
   return 1;
}

int
clip_GTK_DRAGSOURCEADDIMAGETARGETS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cwid, GTK_IS_WIDGET);

   gtk_drag_source_add_image_targets(GTK_WIDGET(cwid->widget));

   return 0;
 err:
   return 1;
}

int
clip_GTK_DRAGSOURCEADDTEXTTARGETS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cwid, GTK_IS_WIDGET);

   gtk_drag_source_add_text_targets(GTK_WIDGET(cwid->widget));

   return 0;
 err:
   return 1;
}

int
clip_GTK_DRAGSOURCEADDURITARGETS(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cwid, GTK_IS_WIDGET);

   gtk_drag_source_add_uri_targets(GTK_WIDGET(cwid->widget));

   return 0;
 err:
   return 1;
}
#endif
