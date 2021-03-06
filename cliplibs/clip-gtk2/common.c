/*
    Copyright (C) 2001-2004  ITK
    Author  : Alexey M. Tkachenko <alexey@itk.ru>
	      Elena V. Kornilova  <alena@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include <string.h>
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg2.h"

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <pango/pango.h>

#include "ci_clip-gtk2.ch"
#include "ci_clip-gtk2.h"
#include "ci_clip-gdk2.h"

/* This list associates pointer to a widget with pointer to it`s C_widget structure */
static ClipVar _widget_list;

static ClipVar *widget_list = &_widget_list;

static ClipVar _action_list;

static ClipVar *action_list = &_action_list;

#ifdef OS_CYGWIN
#include <w32api/windows.h>
static unsigned char *WinCharset;

static unsigned char *ClipHostCharset;
#endif

/****************************************************************/
#if 1
CLIP_DLLEXPORT char *
_clip_locale_to_utf8(char *text)
{

#ifdef OS_CYGWIN
   char     *buf;

   char     *utf_text;

   gsize     br, bw;

   GError   *ge;

   int       len;

   if (!WinCharset)
    {
       WinCharset = malloc(10);
       snprintf(WinCharset, 10, "cp%d", GetACP());
    }
   if (!ClipHostCharset)
      ClipHostCharset = _clip_host_charset();

   if (!WinCharset || !ClipHostCharset || !text)
      return text;

   len = strlen(text);
   buf = (char *) malloc(len + 1);
   buf[len] = 0;
   _clip_translate_charset(ClipHostCharset, WinCharset, text, buf, len);
   utf_text = (char *) g_locale_to_utf8(buf, strlen(buf), &br, &bw, &ge);
   free(buf);

   return utf_text;
#else

   gsize     br, bw;

   GError   *ge;

   gchar    *t_utf;

   t_utf = g_locale_to_utf8(text, strlen(text), &br, &bw, &ge);

   return t_utf;
#endif
}

CLIP_DLLEXPORT char *
_clip_locale_from_utf8(char *text)
{
#ifdef OS_CYGWIN
   char     *buf;

   char     *locale_text;

   gsize     br, bw;

   GError   *ge;

   int       len;

   if (!WinCharset)
    {
       WinCharset = malloc(10);
       snprintf(WinCharset, 10, "cp%d", GetACP());
    }
   if (!ClipHostCharset)
      ClipHostCharset = _clip_host_charset();

   if (!WinCharset || !ClipHostCharset || !text)
      return text;

   locale_text = g_locale_from_utf8(text, strlen(text), &br, &bw, &ge);
   len = strlen(locale_text);

   buf = (char *) malloc(len + 1);
   buf[len] = 0;
   _clip_translate_charset(WinCharset, ClipHostCharset, locale_text, buf, len);
   g_free(locale_text);
   return buf;
#else
   gsize     br, bw;

   GError   *ge;

   gchar    *t_utf;

   t_utf = g_convert_with_fallback(text, strlen(text), _clip_host_charset(), "utf-8", "?", &br, &bw, &ge);
   return t_utf;
#endif
}
#else
CLIP_DLLEXPORT unsigned char *
_clip_locale_to_utf8(unsigned char *text)
{

#ifdef OS_CYGWIN
   unsigned char *buf;

   unsigned char *utf_text;

   gsize     br, bw;

   GError   *ge;

   int       len;

   if (!WinCharset)
    {
       WinCharset = malloc(10);
       snprintf(WinCharset, 10, "cp%d", GetACP());
    }
   if (!ClipHostCharset)
      ClipHostCharset = _clip_host_charset();

   if (!WinCharset || !ClipHostCharset || !text)
      return text;

   len = strlen(text);
   buf = (unsigned char *) malloc(len + 1);
   buf[len] = 0;
   _clip_translate_charset(ClipHostCharset, WinCharset, text, buf, len);
   utf_text = g_locale_to_utf8(buf, strlen(buf), &br, &bw, &ge);
   free(buf);

   return utf_text;
#else

   gsize     br, bw;

   GError   *ge;

   gchar    *t_utf;

   t_utf = g_locale_to_utf8(text, strlen(text), &br, &bw, &ge);

   return (unsigned char *) t_utf;
#endif
}

CLIP_DLLEXPORT unsigned char *
_clip_locale_from_utf8(unsigned char *text)
{
#ifdef OS_CYGWIN
   unsigned char *buf;

   unsigned char *locale_text;

   gsize     br, bw;

   GError   *ge;

   int       len;

   if (!WinCharset)
    {
       WinCharset = malloc(10);
       snprintf(WinCharset, 10, "cp%d", GetACP());
    }
   if (!ClipHostCharset)
      ClipHostCharset = _clip_host_charset();

   if (!WinCharset || !ClipHostCharset || !text)
      return text;

   locale_text = g_locale_from_utf8(text, strlen(text), &br, &bw, &ge);
   len = strlen(locale_text);
   buf = (unsigned char *) malloc(len + 1);
   buf[len] = 0;
   _clip_translate_charset(WinCharset, ClipHostCharset, locale_text, buf, len);
   g_free(locale_text);
   return buf;
#else
   gsize     br, bw;

   GError   *ge;

   gchar    *t_utf;

   t_utf = g_locale_from_utf8(text, strlen(text), &br, &bw, &ge);
   return t_utf;
#endif
}
#endif

/****************************************************************/
CLIP_DLLEXPORT long
_list_length_cwidget()
{
   if (widget_list->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
      return (long) widget_list->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar;
   else
      return 0;
}

CLIP_DLLEXPORT void
_list_put_cwidget(ClipMachine * ClipMachineMemory, void *pointer, C_widget * cwid)
{
   if (widget_list->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != MAP_type_of_ClipVarType)
      _clip_map(ClipMachineMemory, widget_list);
   if (pointer)
      _clip_mputn(ClipMachineMemory, widget_list, (long) pointer, (long) cwid);
}

CLIP_DLLEXPORT void
_list_put_cobject(ClipMachine * ClipMachineMemory, void *pointer, C_object * cobj)
{
   if (widget_list->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != MAP_type_of_ClipVarType)
      _clip_map(ClipMachineMemory, widget_list);
   if (pointer)
      _clip_mputn(ClipMachineMemory, widget_list, (long) pointer, (long) cobj);
}

CLIP_DLLEXPORT void
_list_remove_cobject(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv;

   if (widget_list->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != MAP_type_of_ClipVarType)
      return;
   cv = (ClipVar *) & (widget_list->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[0]);
   while (cv)
    {

       if (((ClipVar *) & (widget_list->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[0]))->ClipMapVar_m_of_ClipVar.
	   count_of_ClipMapVar)
	  cv = (ClipVar *) & (widget_list->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[0]);
       else
	  break;
       C_object *cobj =
	(C_object
	 *) ((long) (((ClipVar *) & (cv->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[0]))->ClipNumVar_n_of_ClipVar.
		     double_of_ClipNumVar));
       C_widget *cwid =
	(C_widget
	 *) ((long) (((ClipVar *) & (cv->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[0]))->ClipNumVar_n_of_ClipVar.
		     double_of_ClipNumVar));

       if (cobj->objtype)
	  destroy_c_object(cobj);
       else
	  destroy_c_widget(cwid);
    }
}

CLIP_DLLEXPORT C_widget *
_list_get_cwidget(ClipMachine * ClipMachineMemory, void *pointer)
{
   double    d;

   if (pointer && widget_list->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
      if (_clip_mgetn(ClipMachineMemory, widget_list, (long) pointer, &d) == 0)
	 return (C_widget *) ((long) d);
   return NULL;
}

CLIP_DLLEXPORT C_widget *
_list_get_cwidget_by_data(ClipMachine * ClipMachineMemory, void *data)
{
   if (data && widget_list->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
    {
       int       i;

       ClipVar  *cv;

       for (i = 0; i < widget_list->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar; i++)
	{
	   cv = &(widget_list->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar + i)->ClipVar_var_of_ClipVarEl;
	   return (C_widget *) ((long) cv->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
	}
    }
   return NULL;
}

CLIP_DLLEXPORT C_object *
_list_get_cobject(ClipMachine * ClipMachineMemory, void *pointer)
{
   double    d;

   if (pointer && widget_list->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
      if (_clip_mgetn(ClipMachineMemory, widget_list, (long) pointer, &d) == 0)
	 return (C_object *) ((long) d);
   return NULL;
}

CLIP_DLLEXPORT void
_list_remove_cwidget(ClipMachine * ClipMachineMemory, void *pointer)
{
   if (pointer && widget_list->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
      _clip_mdel(ClipMachineMemory, widget_list, (long) pointer);
}

CLIP_DLLEXPORT void
_list_put_action(ClipMachine * ClipMachineMemory, void *pointer, ClipVar * cv)
{
   if (action_list->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != MAP_type_of_ClipVarType)
      _clip_map(ClipMachineMemory, action_list);
   if (pointer)
      _clip_mputn(ClipMachineMemory, action_list, (long) pointer, (long) cv);
}

CLIP_DLLEXPORT ClipVar *
_list_get_action(ClipMachine * ClipMachineMemory, void *pointer)
{
   double    d;

   if (pointer && action_list->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
      if (_clip_mgetn(ClipMachineMemory, action_list, (long) pointer, &d) == 0)
	 return (ClipVar *) ((long) d);
   return NULL;
}

CLIP_DLLEXPORT void
_list_remove_action(ClipMachine * ClipMachineMemory, void *pointer)
{
   if (pointer && action_list->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
      _clip_mdel(ClipMachineMemory, action_list, (long) pointer);
}

/*****************************************************************/

/* This list associates widget type with pointer to table of signals */
static WTypeTable *wtype_table = NULL;

CLIP_DLLEXPORT void
_wtype_table_put(ClipTypeFunc ClipType, TypeNameFunc TypeName, TypeFunc WType, TypeFunc SuperType, SignalTable * s_table)
{
   WTypeTable *wt_item = NULL;

   static WTypeTable *last_wt_item = NULL;

   if (!wtype_table)
    {
       wtype_table = NEW(WTypeTable);
       last_wt_item = wt_item = wtype_table;
    }
   else
    {
       wt_item = last_wt_item;
       wt_item->next = NEW(WTypeTable);
       last_wt_item = wt_item = wt_item->next;
    }
   wt_item->ftype = WType;
   wt_item->ftype_name = TypeName;
   wt_item->fsuper_type = SuperType;
   wt_item->fclip_type = ClipType;
   wt_item->signal_table = s_table;
}

CLIP_DLLEXPORT WTypeTable *
_wtype_table_get(GtkType WType)
{
   WTypeTable *wt_item = NULL;

   if (!wtype_table)
      return NULL;
   for (wt_item = wtype_table; wt_item; wt_item = wt_item->next)
    {
       if (wt_item->ftype && wt_item->ftype() == WType)
	  break;
    }
   return wt_item;
}

CLIP_DLLEXPORT WTypeTable *
_wtype_table_get_by_clip_type(long WClipType)
{
   WTypeTable *wt_item = NULL;

   if (!wtype_table)
      return NULL;
   for (wt_item = wtype_table; wt_item; wt_item = wt_item->next)
    {
       if (wt_item->fclip_type && wt_item->fclip_type() == WClipType)
	  break;
    }
   return wt_item;
}

CLIP_DLLEXPORT WTypeTable *
_wtype_table_get_first()
{
   return wtype_table;
}

CLIP_DLLEXPORT void
_wtype_table_destroy(WTypeTable * wt_item)
{
   if (wt_item->next)
      _wtype_table_destroy(wt_item->next);
   free(wt_item);
}

/****************************************************************/
CLIP_DLLEXPORT void
destroy_c_widget(void *item)
{
   C_widget *cw = (C_widget *) item;

   C_signal *cs, *csnext;

//printf("destroy widget %s\n", cw->type_name);
   if (!cw)
      return;
   if (cw->destroy)
      cw->destroy(cw->cmachine, cw);
   if (cw->widget)
      _list_remove_cwidget(cw->cmachine, cw->widget);
   _clip_destroy(cw->cmachine, &cw->obj);
   for (cs = cw->siglist; cs;)
    {
       csnext = cs->next;
       _clip_destroy(cw->cmachine, &cs->cfunc);
       if (cs)
	  free(cs);
       cs = csnext;
    }
   if (cs)
      free(cs);

   _clip_destroy_c_item(cw->cmachine, cw->handle, _C_ITEM_TYPE_WIDGET);
   free(cw);
//printf("destroy ok \n");
}

CLIP_DLLEXPORT void
destroy_c_object(void *item)
{
   C_object *co = (C_object *) item;

  /* alena add for some object */
   C_signal *cs, *csnext;

//printf("destroy object %s\n", co->type_name);
   if (!co)
      return;
   if (co->destroy)
      co->destroy(co->cmachine, co);
   if (co->object)
      _list_remove_cwidget(co->cmachine, co->object);
   _clip_destroy(co->cmachine, &co->obj);

   for (cs = co->siglist; cs;)
    {
       csnext = cs->next;
      //printf("destroy signal %s \n", cs->signame);
       _clip_destroy(co->cmachine, &cs->cfunc);
       if (cs)
	  free(cs);
       cs = csnext;
    }

   _clip_destroy_c_item(co->cmachine, co->handle, _C_ITEM_TYPE_WIDGET);
   free(co);
}

int
clip_GTK_OBJECTDESTROY(ClipMachine * ClipMachineMemory)
{
   C_object *co = _fetch_co_arg(ClipMachineMemory);

   destroy_c_object(co);

   return 0;
}

/********************************************************/
GtkWidget *
map_get_widget(ClipMachine * ClipMachineMemory, ClipVar * map)
{
   double    h;

   C_widget *cwid;

   if (!map || map->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != MAP_type_of_ClipVarType)
      goto err;
   if (_clip_mgetn(ClipMachineMemory, map, HASH_HANDLE, &h) != 0)
      goto err;
   cwid = (C_widget *) _clip_fetch_c_item(ClipMachineMemory, (int) h, _C_ITEM_TYPE_WIDGET);
   if (cwid == NULL || cwid->widget == NULL)
      goto err;

   return cwid->widget;
 err:
   return NULL;
}

C_widget *
map_get_cwidget(ClipMachine * ClipMachineMemory, ClipVar * map)
{
   double    h;

   C_widget *cwid;

   if (!map || map->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != MAP_type_of_ClipVarType)
      goto err;
   if (_clip_mgetn(ClipMachineMemory, map, HASH_HANDLE, &h) != 0)
      goto err;
   cwid = (C_widget *) _clip_fetch_c_item(ClipMachineMemory, (int) h, _C_ITEM_TYPE_WIDGET);
   if (cwid == NULL)
      goto err;

   return cwid;
 err:
   return NULL;
}

void     *
map_get_data(ClipMachine * ClipMachineMemory, ClipVar * map)
{
   double    h;

   C_widget *cwid;

   if (!map || map->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != MAP_type_of_ClipVarType)
      goto err;
   if (_clip_mgetn(ClipMachineMemory, map, HASH_HANDLE, &h) != 0)
      goto err;
   cwid = (C_widget *) _clip_fetch_c_item(ClipMachineMemory, (int) h, _C_ITEM_TYPE_WIDGET);
   if (cwid == NULL || cwid->data == NULL)
      goto err;

   return cwid->data;
 err:
   return NULL;
}

/********************************************************/
CLIP_DLLEXPORT C_widget *
_fetch_cw_arg(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid;

   if (_clip_parinfo(ClipMachineMemory, 1) == NUMERIC_type_of_ClipVarType)
    {
       cwid = (C_widget *) _clip_fetch_c_item(ClipMachineMemory, _clip_parni(ClipMachineMemory, 1), _C_ITEM_TYPE_WIDGET);
    }
   else
    {
       if (_clip_parinfo(ClipMachineMemory, 1) == MAP_type_of_ClipVarType)
	{
	   double    h;

	   _clip_mgetn(ClipMachineMemory, _clip_spar(ClipMachineMemory, 1), HASH_HANDLE, &h);
	   cwid = (C_widget *) _clip_fetch_c_item(ClipMachineMemory, (int) h, _C_ITEM_TYPE_WIDGET);
	}
       else
	{
	   _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, "CLIP_GTK_SYSTEM", EG_ARG, "Bad widget descriptor");
	   return NULL;
	}
    }
   if (!cwid)
    {
       _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, "CLIP_GTK_SYSTEM", EG_ARG, "Bad widget descriptor");
       return NULL;
    }
   return cwid;
}

CLIP_DLLEXPORT C_widget *
_fetch_cw_opt(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid;

   if (_clip_parinfo(ClipMachineMemory, 1) == UNDEF_type_of_ClipVarType)
      return NULL;
   if (_clip_parinfo(ClipMachineMemory, 1) == NUMERIC_type_of_ClipVarType)
    {
       cwid = (C_widget *) _clip_fetch_c_item(ClipMachineMemory, _clip_parni(ClipMachineMemory, 1), _C_ITEM_TYPE_WIDGET);
    }
   else
    {
       if (_clip_parinfo(ClipMachineMemory, 1) == MAP_type_of_ClipVarType)
	{
	   double    h;

	   _clip_mgetn(ClipMachineMemory, _clip_spar(ClipMachineMemory, 1), HASH_HANDLE, &h);
	   cwid = (C_widget *) _clip_fetch_c_item(ClipMachineMemory, (int) h, _C_ITEM_TYPE_WIDGET);
	}
       else
	{
	   _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, "CLIP_GTK_SYSTEM", EG_ARG, "Bad widget descriptor");
	   return NULL;
	}
    }
   if (!cwid)
    {
       _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, "CLIP_GTK_SYSTEM", EG_ARG, "Bad widget descriptor");
       return NULL;
    }
   return cwid;
}

CLIP_DLLEXPORT C_object *
_fetch_co_arg(ClipMachine * ClipMachineMemory)
{
   C_object *cobj;

   if (_clip_parinfo(ClipMachineMemory, 1) == NUMERIC_type_of_ClipVarType)
    {
       cobj = (C_object *) _clip_fetch_c_item(ClipMachineMemory, _clip_parni(ClipMachineMemory, 1), _C_ITEM_TYPE_WIDGET);
    }
   else
    {
       if (_clip_parinfo(ClipMachineMemory, 1) == MAP_type_of_ClipVarType)
	{
	   double    h;

	   _clip_mgetn(ClipMachineMemory, _clip_spar(ClipMachineMemory, 1), HASH_HANDLE, &h);
	   cobj = (C_object *) _clip_fetch_c_item(ClipMachineMemory, (int) h, _C_ITEM_TYPE_WIDGET);
	}
       else
	{
	   _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, "CLIP_GTK_SYSTEM", EG_ARG, "Bad object descriptor");
	   return NULL;
	}
    }
   if (!cobj)
    {
       _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, "CLIP_GTK_SYSTEM", EG_ARG, "Bad object descriptor");
       return NULL;
    }
   return cobj;
}

CLIP_DLLEXPORT C_object *
_fetch_co_opt(ClipMachine * ClipMachineMemory)
{
   if (_clip_parinfo(ClipMachineMemory, 1) == UNDEF_type_of_ClipVarType)
      return NULL;
   else
      return _fetch_co_arg(ClipMachineMemory);
}

CLIP_DLLEXPORT C_widget *
_fetch_cwidget(ClipMachine * ClipMachineMemory, ClipVar * cv)
{
   C_widget *cwid;

   if (!cv)
      return NULL;

   if (cv->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
    {
       cwid = (C_widget *) _clip_fetch_c_item(ClipMachineMemory, cv->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar,
					      _C_ITEM_TYPE_WIDGET);
    }
   else
    {
       if (cv->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
	{
	   double    h;

	   _clip_mgetn(ClipMachineMemory, cv, HASH_HANDLE, &h);
	   cwid = (C_widget *) _clip_fetch_c_item(ClipMachineMemory, (int) h, _C_ITEM_TYPE_WIDGET);
	}
       else
	{
/*			_clip_trap_err(ClipMachineMemory,EG_ARG,0,0,"CLIP_GTK_SYSTEM",
				EG_ARG,"Bad widget descriptor"); */
	   return NULL;
	}
    }
   if (!cwid)
    {
/*		_clip_trap_err(ClipMachineMemory,EG_ARG,0,0,"CLIP_GTK_SYSTEM",
			EG_ARG,"Bad widget descriptor"); */
       return NULL;
    }
   return cwid;
}

CLIP_DLLEXPORT C_widget *
_fetch_cwidgetn(ClipMachine * ClipMachineMemory, int h)
{
   C_widget *cwid;

   cwid = (C_widget *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_WIDGET);

   if (!cwid)
    {
/*		_clip_trap_err(ClipMachineMemory,EG_ARG,0,0,"CLIP_GTK_SYSTEM",
			EG_ARG,"Bad widget descriptor"); */
       return NULL;
    }
   return cwid;
}

CLIP_DLLEXPORT C_object *
_fetch_cobject(ClipMachine * ClipMachineMemory, ClipVar * cv)
{
   C_object *cobj;

   if (!cv)
      return NULL;

   if (cv->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
    {
       cobj = (C_object *) _clip_fetch_c_item(ClipMachineMemory, cv->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar,
					      _C_ITEM_TYPE_WIDGET);
    }
   else
    {
       if (cv->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
	{
	   double    h;

	   _clip_mgetn(ClipMachineMemory, cv, HASH_HANDLE, &h);
	   cobj = (C_object *) _clip_fetch_c_item(ClipMachineMemory, (int) h, _C_ITEM_TYPE_WIDGET);
	}
       else
	{
/*			_clip_trap_err(ClipMachineMemory,EG_ARG,0,0,"CLIP_GTK_SYSTEM",
				EG_ARG,"Bad widget descriptor"); */
	   return NULL;
	}
    }
   if (!cobj)
    {
/*		_clip_trap_err(ClipMachineMemory,EG_ARG,0,0,"CLIP_GTK_SYSTEM",
			EG_ARG,"Bad widget descriptor"); */
       return NULL;
    }
   return cobj;
}

CLIP_DLLEXPORT C_object *
_fetch_cobjectn(ClipMachine * ClipMachineMemory, int h)
{
   C_object *cobj;

   cobj = (C_object *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_WIDGET);

   if (!cobj)
    {
/*		_clip_trap_err(ClipMachineMemory,EG_ARG,0,0,"CLIP_GTK_SYSTEM",
			EG_ARG,"Bad widget descriptor"); */
       return NULL;
    }
   return cobj;
}

void
widget_destructor(C_widget * cw)
{
   if (cw)
    {
       double    d;

       if (_clip_mgetn(cw->cmachine, widget_list, (long) cw->widget, &d) == 0)
	  destroy_c_widget(cw);
    }
}

void
object_destructor(C_object * co)
{
   if (co)
      destroy_c_object(co);
}

CLIP_DLLEXPORT C_widget *
_register_widget(ClipMachine * ClipMachineMemory, GtkWidget * wid, ClipVar * cv)
{
   int       handle = -1;

   C_widget *cwid = (C_widget *) calloc(1, sizeof(C_widget));

   WTypeTable *wt_item;

   long      clip_wtype = GTK_WIDGET_UNKNOWN;

   cwid->widget = wid;
   cwid->objtype = GTK_OBJ_WIDGET;
   cwid->cmachine = ClipMachineMemory;

   cwid->type = wid ? GTK_WIDGET_TYPE(wid) : GTK_TYPE_INVALID;
   wt_item = _wtype_table_get(cwid->type);
   if (wt_item && wt_item->fclip_type)
      clip_wtype = wt_item->fclip_type();
   if (wt_item && wt_item->ftype_name)
      cwid->type_name = wt_item->ftype_name();

   cwid->sigenabled = TRUE;
   cwid->usersigenabled = TRUE;
   cwid->evntenabled = TRUE;
   cwid->destroy = NULL;

   if (cv && cv->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
     //_clip_mclone(ClipMachineMemory, &cwid->obj, cv);
      cwid->obj = *cv;
   else
      _clip_map(ClipMachineMemory, &cwid->obj);
  /* Saving widget info into CLIP state machine
   * and it`s handle to a map HANDLE property */
   handle = _clip_store_c_item(ClipMachineMemory, cwid, _C_ITEM_TYPE_WIDGET, NULL);
   cwid->handle = handle;
   _clip_mputn(ClipMachineMemory, &cwid->obj, HASH_HANDLE, handle);
   _clip_mputn(ClipMachineMemory, &cwid->obj, HASH_TYPE, clip_wtype);
  /* Store C_widget pointer in list of widgets */
   _list_put_cwidget(ClipMachineMemory, wid, cwid);
  //if (wid && GTK_IS_OBJECT(wid))
   if (wid && GTK_IS_WIDGET(wid))
      g_object_set_data_full(G_OBJECT(wid), "destructor", cwid, (GDestroyNotify) widget_destructor);
   return cwid;
}

CLIP_DLLEXPORT C_object *
_register_object(ClipMachine * ClipMachineMemory, void *data, long clip_type, ClipVar * cv, coDestructor fDestroy)
{
   int       handle = -1;

   C_object *cobj = (C_object *) calloc(1, sizeof(C_object));

   WTypeTable *wt_item;

   long      clip_wtype = GTK_WIDGET_UNKNOWN;

   cobj->object = data;
   cobj->objtype = GTK_OBJ_OBJECT;
   cobj->cmachine = ClipMachineMemory;
   cobj->type = clip_type;
   cobj->destroy = fDestroy;

  /* alena add signals to object */
   cobj->sigenabled = TRUE;
   cobj->usersigenabled = TRUE;
   cobj->evntenabled = TRUE;

   wt_item = _wtype_table_get(cobj->type);
   if (wt_item && wt_item->fclip_type)
      clip_wtype = wt_item->fclip_type();
   if (wt_item && wt_item->ftype_name)
      cobj->type_name = wt_item->ftype_name();
   if (cv && cv->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
     //_clip_mclone(ClipMachineMemory, &cobj->obj, cv);
      cobj->obj = *cv;
   else
      _clip_map(ClipMachineMemory, &cobj->obj);

  /* Saving widget info into CLIP state machine
   * and it`s handle to a map HANDLE property */
   handle = _clip_store_c_item(ClipMachineMemory, cobj, _C_ITEM_TYPE_WIDGET, NULL);
   cobj->handle = handle;
   _clip_mputn(ClipMachineMemory, &cobj->obj, HASH_HANDLE, handle);
   _clip_mputn(ClipMachineMemory, &cobj->obj, HASH_TYPE, clip_wtype);
  /* Store C_object pointer in list of widgets */
   _list_put_cobject(ClipMachineMemory, data, cobj);
   if (data && clip_type != GDK_TYPE_ATOM && clip_type != GTK_TYPE_TREE_ITER
       && clip_type != GTK_TYPE_TREE_PATH && clip_type != GTK_TYPE_SELECTION_DATA)

      if (G_IS_OBJECT(data))
	 g_object_set_data_full(G_OBJECT(data), "destructor", cobj, (GDestroyNotify) object_destructor);
   return cobj;
}

/* Try to find registered widget. If fail, register it. */
CLIP_DLLEXPORT C_widget *
_get_cwidget(ClipMachine * ClipMachineMemory, GtkWidget * wid)
{
   C_widget *cwid = _list_get_cwidget(ClipMachineMemory, wid);

   if (wid && !cwid)
      cwid = _register_widget(ClipMachineMemory, wid, NULL);
   return cwid;
}

/* Try to find registered object. If fail, register it. */
CLIP_DLLEXPORT C_object *
_get_cobject(ClipMachine * ClipMachineMemory, void *obj, long clip_type, coDestructor fDestroy)
{
   C_object *cobj = _list_get_cobject(ClipMachineMemory, obj);

   if (obj && !cobj)
      cobj = _register_object(ClipMachineMemory, obj, clip_type, NULL, fDestroy);
   return cobj;
}

// Get colors for GTK+ from a map
CLIP_DLLEXPORT void
_map_get_colors(ClipMachine * ClipMachineMemory, ClipVar * map, double colors[])
{
   if (map && map->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
    {
       colors[0] = colors[1] = colors[2] = 0;
       colors[3] = 65355;
       _clip_mgetn(ClipMachineMemory, map, HASH_RED, &colors[0]);
       _clip_mgetn(ClipMachineMemory, map, HASH_GREEN, &colors[1]);
       _clip_mgetn(ClipMachineMemory, map, HASH_BLUE, &colors[2]);
       _clip_mgetn(ClipMachineMemory, map, HASH_OPACITY, &colors[3]);
    }
}

// Put colors from GTK+ to a map
CLIP_DLLEXPORT void
_map_put_colors(ClipMachine * ClipMachineMemory, ClipVar * map, double colors[])
{
   if (map && map->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
    {
       _clip_mputn(ClipMachineMemory, map, HASH_RED, colors[0]);
       _clip_mputn(ClipMachineMemory, map, HASH_GREEN, colors[1]);
       _clip_mputn(ClipMachineMemory, map, HASH_BLUE, colors[2]);
       _clip_mputn(ClipMachineMemory, map, HASH_OPACITY, colors[3]);
    }
}

// Get colors for color selection from a map
CLIP_DLLEXPORT void
_map_get_sel_colors(ClipMachine * ClipMachineMemory, ClipVar * map, double colors[])
{
   _map_get_colors(ClipMachineMemory, map, colors);

   colors[0] /= 65535;
   colors[1] /= 65535;
   colors[2] /= 65535;
   colors[3] /= 65535;
}

CLIP_DLLEXPORT void
_map_colors_to_gdk(ClipMachine * ClipMachineMemory, ClipVar * map, GdkColor * gdk_color)
{
   double    red, green, blue, pixel;

   if (map->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
    {
       _clip_mgetn(ClipMachineMemory, map, HASH_RED, &red);
       gdk_color->red = red;
       _clip_mgetn(ClipMachineMemory, map, HASH_GREEN, &green);
       gdk_color->green = green;
       _clip_mgetn(ClipMachineMemory, map, HASH_BLUE, &blue);
       gdk_color->blue = blue;
       _clip_mgetn(ClipMachineMemory, map, HASH_PIXEL, &pixel);
       gdk_color->pixel = pixel;
    }
}

CLIP_DLLEXPORT void
_map_colors_to_gdk_array(ClipMachine * ClipMachineMemory, ClipVar * map, GdkColor gdk_color[])
{
   double    colors[4];

   if (map->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
    {
       _map_get_colors(ClipMachineMemory, map, colors);
       gdk_color[0].red = colors[0];
       gdk_color[0].green = colors[1];
       gdk_color[0].blue = colors[2];

    }
   if (map->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
    {
       ClipArrVar *a = (ClipArrVar *) _clip_vptr(map);

       int       i;

       for (i = 0; i < a->count_of_ClipArrVar && i < 5; i++)
	{
	   if (a->ClipVar_items_of_ClipArrVar[i].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != MAP_type_of_ClipVarType)
	      continue;
	   _map_get_colors(ClipMachineMemory, (ClipVar *) & a->ClipVar_items_of_ClipArrVar[i], colors);
	   gdk_color[i].red = colors[0];
	   gdk_color[i].green = colors[1];
	   gdk_color[i].blue = colors[2];
	}
    }
}

CLIP_DLLEXPORT void
_map_gc_to_gdk_array(ClipMachine * ClipMachineMemory, ClipVar * map, GdkGC * gdk_gc[])
{
   if (map->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType
       || map->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
    {
       C_object *cgc = _fetch_cobject(ClipMachineMemory, map);

       gdk_gc[0] = cgc ? (GdkGC *) (cgc->object) : NULL;
    }
   if (map->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
    {
       ClipArrVar *a = (ClipArrVar *) _clip_vptr(map);

       int       i;

       C_object *cgc;

       for (i = 0; i < a->count_of_ClipArrVar && i < 5; i++)
	{
	   if (a->ClipVar_items_of_ClipArrVar[i].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != MAP_type_of_ClipVarType
	       && a->ClipVar_items_of_ClipArrVar[i].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType !=
	       NUMERIC_type_of_ClipVarType)
	      continue;
	   cgc = _fetch_cobject(ClipMachineMemory, &a->ClipVar_items_of_ClipArrVar[i]);
	   gdk_gc[i] = cgc ? (GdkGC *) (cgc->object) : NULL;
	}
    }
}

CLIP_DLLEXPORT void
_map_gc_to_gdk(ClipMachine * ClipMachineMemory, ClipVar * map, GdkGC * *gdk_gc)
{
   if (map->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType
       || map->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
    {
       C_object *cgc = _fetch_cobject(ClipMachineMemory, map);

       *gdk_gc = cgc ? (GdkGC *) (cgc->object) : NULL;
    }
}

CLIP_DLLEXPORT void
_gdk_gc_to_map(ClipMachine * ClipMachineMemory, GdkGC * *gdk_gc, ClipVar * map)
{
   if (map->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType
       || map->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
    {
       C_object *cgc = _get_cobject(ClipMachineMemory, map, GDK_OBJECT_GC,
				    (coDestructor) gdk_object_gc_destructor);

       *gdk_gc = cgc ? (GdkGC *) (cgc->object) : NULL;
    }
}

/* Get color and store it to map */
CLIP_DLLEXPORT void
_gdk_color_to_map(ClipMachine * ClipMachineMemory, GdkColor gdk_color, ClipVar * col)
{
   _clip_mputn(ClipMachineMemory, col, HASH_RED, gdk_color.red);
   _clip_mputn(ClipMachineMemory, col, HASH_GREEN, gdk_color.green);
   _clip_mputn(ClipMachineMemory, col, HASH_BLUE, gdk_color.blue);
   _clip_mputn(ClipMachineMemory, col, HASH_PIXEL, gdk_color.pixel);
}

/* Get color map and store it to GdkColor */
CLIP_DLLEXPORT void
_map_to_gdk_color(ClipMachine * ClipMachineMemory, GdkColor * gdk_color, ClipVar * col)
{
   double    r, g, b, p;

   _clip_mgetn(ClipMachineMemory, col, HASH_RED, &r);
   _clip_mgetn(ClipMachineMemory, col, HASH_GREEN, &g);
   _clip_mgetn(ClipMachineMemory, col, HASH_BLUE, &b);
   _clip_mgetn(ClipMachineMemory, col, HASH_PIXEL, &p);

   gdk_color->red = r;
   gdk_color->green = g;
   gdk_color->blue = b;
   gdk_color->pixel = p;
}

/* Get array of GCs and store it to array of maps */
CLIP_DLLEXPORT void
_gdk_array_to_map_gcs(ClipMachine * ClipMachineMemory, GdkGC * gdk_gcs[], ClipVar * a)
{
   long      i = 5;

   C_object *cgc = NULL;

   _clip_array(ClipMachineMemory, a, 1, &i);
   for (i = 0; i < 5; i++)
    {
       cgc = _get_cobject(ClipMachineMemory, gdk_gcs[i], GDK_OBJECT_GC, (coDestructor) gdk_object_gc_destructor);
       if (cgc)
	  _clip_aset(ClipMachineMemory, a, &cgc->obj, 1, &i);
    }
}

/* Get array of colors and store it to array of maps */
CLIP_DLLEXPORT void
_gdk_array_to_map_colors(ClipMachine * ClipMachineMemory, GdkColor gdk_colors[], ClipVar * a)
{
   long      i = 5;

   ClipVar   cv;

   memset(&cv, 0, sizeof(cv));
   _clip_array(ClipMachineMemory, a, 1, &i);
   for (i = 0; i < 5; i++)
    {
       _clip_map(ClipMachineMemory, &cv);
       _gdk_color_to_map(ClipMachineMemory, gdk_colors[i], &cv);
       _clip_aset(ClipMachineMemory, a, &cv, 1, &i);
    }
   _clip_destroy(ClipMachineMemory, &cv);
}

CLIP_DLLEXPORT void
_style_to_map(ClipMachine * ClipMachineMemory, GtkStyle * style, ClipVar * m_style)
{
   ClipVar  *c = NEW(ClipVar);

   ClipVar  *a = NEW(ClipVar);

   C_object *cgc;

   ClipVar   cfont;

   gchar    *font;

   if (!style || !m_style || m_style->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != MAP_type_of_ClipVarType)
      return;
  /* Get colors of widget */

   _gdk_array_to_map_colors(ClipMachineMemory, style->fg, a);
   _clip_madd(ClipMachineMemory, m_style, HASH_FG_COLOR, a);
   _clip_destroy(ClipMachineMemory, a);

   _gdk_array_to_map_colors(ClipMachineMemory, style->bg, a);
   _clip_madd(ClipMachineMemory, m_style, HASH_BG_COLOR, a);
   _clip_destroy(ClipMachineMemory, a);

   _gdk_array_to_map_colors(ClipMachineMemory, style->light, a);
   _clip_madd(ClipMachineMemory, m_style, HASH_LIGHT_COLOR, a);
   _clip_destroy(ClipMachineMemory, a);

   _gdk_array_to_map_colors(ClipMachineMemory, style->dark, a);
   _clip_madd(ClipMachineMemory, m_style, HASH_DARK_COLOR, a);
   _clip_destroy(ClipMachineMemory, a);

   _gdk_array_to_map_colors(ClipMachineMemory, style->mid, a);
   _clip_madd(ClipMachineMemory, m_style, HASH_MID_COLOR, a);
   _clip_destroy(ClipMachineMemory, a);

   _gdk_array_to_map_colors(ClipMachineMemory, style->text, a);
   _clip_madd(ClipMachineMemory, m_style, HASH_TEXT_COLOR, a);
   _clip_destroy(ClipMachineMemory, a);

   _gdk_array_to_map_colors(ClipMachineMemory, style->base, a);
   _clip_madd(ClipMachineMemory, m_style, HASH_BASE_COLOR, a);
   _clip_destroy(ClipMachineMemory, a);

   _gdk_array_to_map_gcs(ClipMachineMemory, style->fg_gc, a);
   _clip_madd(ClipMachineMemory, m_style, HASH_FG_GC, a);
   _clip_destroy(ClipMachineMemory, a);

   _gdk_array_to_map_gcs(ClipMachineMemory, style->bg_gc, a);
   _clip_madd(ClipMachineMemory, m_style, HASH_BG_GC, a);
   _clip_destroy(ClipMachineMemory, a);

   _gdk_array_to_map_gcs(ClipMachineMemory, style->light_gc, a);
   _clip_madd(ClipMachineMemory, m_style, HASH_LIGHT_GC, a);
   _clip_destroy(ClipMachineMemory, a);

   _clip_map(ClipMachineMemory, c);
   _gdk_color_to_map(ClipMachineMemory, style->black, c);
   _clip_madd(ClipMachineMemory, m_style, HASH_BLACK_COLOR, c);
   _clip_map(ClipMachineMemory, c);
   _gdk_color_to_map(ClipMachineMemory, style->white, c);
   _clip_madd(ClipMachineMemory, m_style, HASH_WHITE_COLOR, c);

   _gdk_array_to_map_gcs(ClipMachineMemory, style->dark_gc, a);
   _clip_madd(ClipMachineMemory, m_style, HASH_DARK_GC, a);
   _clip_destroy(ClipMachineMemory, a);

   _gdk_array_to_map_gcs(ClipMachineMemory, style->mid_gc, a);
   _clip_madd(ClipMachineMemory, m_style, HASH_MID_GC, a);
   _clip_destroy(ClipMachineMemory, a);

   _gdk_array_to_map_gcs(ClipMachineMemory, style->text_gc, a);
   _clip_madd(ClipMachineMemory, m_style, HASH_TEXT_GC, a);
   _clip_destroy(ClipMachineMemory, a);

   _gdk_array_to_map_gcs(ClipMachineMemory, style->base_gc, a);
   _clip_madd(ClipMachineMemory, m_style, HASH_BASE_GC, a);
   _clip_destroy(ClipMachineMemory, a);

   cgc = _get_cobject(ClipMachineMemory, style->black_gc, GDK_OBJECT_GC, (coDestructor) gdk_object_gc_destructor);
   if (cgc)
      _clip_madd(ClipMachineMemory, m_style, HASH_BLACK_GC, &cgc->obj);
   cgc = _get_cobject(ClipMachineMemory, style->white_gc, GDK_OBJECT_GC, (coDestructor) gdk_object_gc_destructor);
   if (cgc)
      _clip_madd(ClipMachineMemory, m_style, HASH_WHITE_GC, &cgc->obj);

   if (style->font_desc)
    {
       font = pango_font_description_to_string(style->font_desc);
       if (font)
	{
	   memset(&cfont, 0, sizeof(cfont));
	   _clip_var_str(font, strlen(font), &cfont);

	   _clip_madd(ClipMachineMemory, m_style, HASH_FONT, &cfont);
	   _clip_destroy(ClipMachineMemory, &cfont);
	}
    }
   if (style->colormap)
    {
       C_object *ccmap = _list_get_cobject(ClipMachineMemory, style->colormap);

       if (!ccmap)
	  ccmap = _register_object(ClipMachineMemory, style->colormap, GDK_OBJECT_COLORMAP,
				   NULL, (coDestructor) gdk_object_colormap_destructor);
       if (ccmap)
	{
	   _clip_madd(ClipMachineMemory, m_style, HASH_COLORMAP, &ccmap->obj);
	}
    }

   _clip_destroy(ClipMachineMemory, c);
   free(c);
   free(a);

}

CLIP_DLLEXPORT void
_rc_style_to_map(ClipMachine * ClipMachineMemory, GtkRcStyle * style, ClipVar * m_style)
{
   ClipVar  *a = NEW(ClipVar);

   ClipVar   cfont;

   gchar    *font;

   if (!style || !m_style || m_style->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != MAP_type_of_ClipVarType)
      return;
  /* Get colors of widget */

   _gdk_array_to_map_colors(ClipMachineMemory, style->fg, a);
   _clip_madd(ClipMachineMemory, m_style, HASH_FG_COLOR, a);
   _clip_destroy(ClipMachineMemory, a);

   _gdk_array_to_map_colors(ClipMachineMemory, style->bg, a);
   _clip_madd(ClipMachineMemory, m_style, HASH_BG_COLOR, a);
   _clip_destroy(ClipMachineMemory, a);

   _gdk_array_to_map_colors(ClipMachineMemory, style->text, a);
   _clip_madd(ClipMachineMemory, m_style, HASH_TEXT_COLOR, a);
   _clip_destroy(ClipMachineMemory, a);

   _gdk_array_to_map_colors(ClipMachineMemory, style->base, a);
   _clip_madd(ClipMachineMemory, m_style, HASH_BASE_COLOR, a);
   _clip_destroy(ClipMachineMemory, a);

   if (style->font_desc)
    {
       font = pango_font_description_to_string(style->font_desc);
       if (font)
	{
	   memset(&cfont, 0, sizeof(cfont));
	   _clip_var_str(font, strlen(font), &cfont);

	   _clip_madd(ClipMachineMemory, m_style, HASH_FONT, &cfont);
	   _clip_destroy(ClipMachineMemory, &cfont);
	}
    }

   free(a);

}

CLIP_DLLEXPORT void
_map_to_style(ClipMachine * ClipMachineMemory, ClipVar * m_style, GtkStyle * style)
{
   ClipVar  *c;

   PangoFontDescription *font = NULL;

   C_object *ccmap;

   if ((c = _clip_mget(ClipMachineMemory, m_style, HASH_BG_GC)) != NULL)
      _map_gc_to_gdk_array(ClipMachineMemory, c, style->bg_gc);
  /* Colors */
   if ((c = _clip_mget(ClipMachineMemory, m_style, HASH_FG_COLOR)) != NULL)
      _map_colors_to_gdk_array(ClipMachineMemory, c, style->fg);
   if ((c = _clip_mget(ClipMachineMemory, m_style, HASH_BG_COLOR)) != NULL)
      _map_colors_to_gdk_array(ClipMachineMemory, c, style->bg);
   if ((c = _clip_mget(ClipMachineMemory, m_style, HASH_LIGHT_COLOR)) != NULL)
      _map_colors_to_gdk_array(ClipMachineMemory, c, style->light);
   if ((c = _clip_mget(ClipMachineMemory, m_style, HASH_DARK_COLOR)) != NULL)
      _map_colors_to_gdk_array(ClipMachineMemory, c, style->dark);
   if ((c = _clip_mget(ClipMachineMemory, m_style, HASH_MID_COLOR)) != NULL)
      _map_colors_to_gdk_array(ClipMachineMemory, c, style->mid);
   if ((c = _clip_mget(ClipMachineMemory, m_style, HASH_TEXT_COLOR)) != NULL)
      _map_colors_to_gdk_array(ClipMachineMemory, c, style->text);
   if ((c = _clip_mget(ClipMachineMemory, m_style, HASH_BASE_COLOR)) != NULL)
      _map_colors_to_gdk_array(ClipMachineMemory, c, style->base);
   if ((c = _clip_mget(ClipMachineMemory, m_style, HASH_BLACK_COLOR)) != NULL)
      _map_colors_to_gdk(ClipMachineMemory, c, &style->black);
   if ((c = _clip_mget(ClipMachineMemory, m_style, HASH_WHITE_COLOR)) != NULL)
      _map_colors_to_gdk(ClipMachineMemory, c, &style->white);

  /* Graphics contexts */
   if ((c = _clip_mget(ClipMachineMemory, m_style, HASH_FG_GC)) != NULL)
      _map_gc_to_gdk_array(ClipMachineMemory, c, style->fg_gc);
   if ((c = _clip_mget(ClipMachineMemory, m_style, HASH_BG_GC)) != NULL)
      _map_gc_to_gdk_array(ClipMachineMemory, c, style->bg_gc);
   if ((c = _clip_mget(ClipMachineMemory, m_style, HASH_LIGHT_GC)) != NULL)
      _map_gc_to_gdk_array(ClipMachineMemory, c, style->light_gc);
   if ((c = _clip_mget(ClipMachineMemory, m_style, HASH_DARK_GC)) != NULL)
      _map_gc_to_gdk_array(ClipMachineMemory, c, style->dark_gc);
   if ((c = _clip_mget(ClipMachineMemory, m_style, HASH_MID_GC)) != NULL)
      _map_gc_to_gdk_array(ClipMachineMemory, c, style->mid_gc);
   if ((c = _clip_mget(ClipMachineMemory, m_style, HASH_TEXT_GC)) != NULL)
      _map_gc_to_gdk_array(ClipMachineMemory, c, style->text_gc);
   if ((c = _clip_mget(ClipMachineMemory, m_style, HASH_BASE_GC)) != NULL)
      _map_gc_to_gdk_array(ClipMachineMemory, c, style->base_gc);
   if ((c = _clip_mget(ClipMachineMemory, m_style, HASH_BLACK_GC)) != NULL)
      _map_gc_to_gdk(ClipMachineMemory, c, &style->black_gc);
   if ((c = _clip_mget(ClipMachineMemory, m_style, HASH_WHITE_GC)) != NULL)
      _map_gc_to_gdk(ClipMachineMemory, c, &style->white_gc);

/* Set font of widget */
   c = _clip_mget(ClipMachineMemory, m_style, HASH_FONT);
   if (c != NULL)
    {
       font = pango_font_description_from_string(c->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);

       if (font)
	{
	   style->font_desc = font;
	}
    }
/* Set colormap of widget */
   c = _clip_mget(ClipMachineMemory, m_style, HASH_COLORMAP);
   if (c
       && (c->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType
	   || c->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType))
    {
       ccmap = _fetch_cobject(ClipMachineMemory, _clip_mget(ClipMachineMemory, m_style, HASH_COLORMAP));
       if (ccmap)
	{
	   style->colormap = GDK_COLORMAP(ccmap->object);
	}
    }
}

CLIP_DLLEXPORT void
_map_to_rc_style(ClipMachine * ClipMachineMemory, ClipVar * m_style, GtkRcStyle * style, GtkStateType state)
{
   ClipVar  *c;

   ClipArrVar *cv;

   PangoFontDescription *font = NULL;

   char     *str;

   int       i;

  /* Colors */
   if ((c = _clip_mget(ClipMachineMemory, m_style, HASH_FG_COLOR)) != NULL)
    {
       _map_colors_to_gdk_array(ClipMachineMemory, c, style->fg);
       style->color_flags[state] |= GTK_RC_FG;
    }
   if ((c = _clip_mget(ClipMachineMemory, m_style, HASH_BG_COLOR)) != NULL)
    {
       _map_colors_to_gdk_array(ClipMachineMemory, c, style->bg);
       style->color_flags[state] |= GTK_RC_BG;
    }
   if ((c = _clip_mget(ClipMachineMemory, m_style, HASH_TEXT_COLOR)) != NULL)
    {
       _map_colors_to_gdk_array(ClipMachineMemory, c, style->text);
       style->color_flags[state] |= GTK_RC_TEXT;
    }
   if ((c = _clip_mget(ClipMachineMemory, m_style, HASH_BASE_COLOR)) != NULL)
    {
       _map_colors_to_gdk_array(ClipMachineMemory, c, style->base);
       style->color_flags[state] |= GTK_RC_BASE;
    }

/* Set font of widget */
   c = _clip_mget(ClipMachineMemory, m_style, HASH_FONT);
   if (c != NULL)
    {
       font = pango_font_description_from_string(c->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);

       if (font)
	{
	   style->font_desc = font;
	}
    }

   c = _clip_mget(ClipMachineMemory, m_style, HASH_BG_PIXMAP_NAME);
   if (c
       && (c->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType
	   || c->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType))
    {
       if (c->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
	{
	   _clip_mgetc(ClipMachineMemory, m_style, HASH_BG_PIXMAP_NAME, &str, &i);
	   style->bg_pixmap_name[0] = str;
	}
       else
	{
	   cv = (ClipArrVar *) _clip_vptr(_clip_mget(ClipMachineMemory, m_style, HASH_BG_PIXMAP_NAME));
	   for (i = 0; i < 5 && i < cv->count_of_ClipArrVar; i++)
	    {
	       str = cv->ClipVar_items_of_ClipArrVar[i].ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
	       style->bg_pixmap_name[i] = str;
	    }
	}
    }

}

CLIP_DLLEXPORT gint
_map_to_gdk_geometry(ClipMachine * ClipMachineMemory, ClipVar * m_geom, GdkGeometry * geom)
{
   ClipVar  *c;

   gint      mask = 0;

   if ((c = _clip_mget(ClipMachineMemory, m_geom, HASH_MIN_WIDTH)) != NULL)
    {
       geom->min_width = c->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
       mask |= GDK_HINT_MIN_SIZE;
    }
   if ((c = _clip_mget(ClipMachineMemory, m_geom, HASH_MIN_HEIGHT)) != NULL)
    {
       geom->min_height = c->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
       mask |= GDK_HINT_MIN_SIZE;
    }
   if ((c = _clip_mget(ClipMachineMemory, m_geom, HASH_MAX_WIDTH)) != NULL)
    {
       geom->max_width = c->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
       mask |= GDK_HINT_MAX_SIZE;
    }
   if ((c = _clip_mget(ClipMachineMemory, m_geom, HASH_MAX_HEIGHT)) != NULL)
    {
       geom->max_height = c->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
       mask |= GDK_HINT_MAX_SIZE;
    }
   if ((c = _clip_mget(ClipMachineMemory, m_geom, HASH_BASE_WIDTH)) != NULL)
    {
       geom->base_width = c->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
       mask |= GDK_HINT_BASE_SIZE;
    }
   if ((c = _clip_mget(ClipMachineMemory, m_geom, HASH_BASE_HEIGHT)) != NULL)
    {
       geom->base_height = c->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
       mask |= GDK_HINT_BASE_SIZE;
    }
   if ((c = _clip_mget(ClipMachineMemory, m_geom, HASH_WIDTH_INC)) != NULL)
    {
       geom->width_inc = c->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
       mask |= GDK_HINT_RESIZE_INC;
    }
   if ((c = _clip_mget(ClipMachineMemory, m_geom, HASH_HEIGHT_INC)) != NULL)
    {
       geom->height_inc = c->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
       mask |= GDK_HINT_RESIZE_INC;
    }
   if ((c = _clip_mget(ClipMachineMemory, m_geom, HASH_MIN_ASPECT)) != NULL)
    {
       geom->min_aspect = c->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
       mask |= GDK_HINT_ASPECT;
    }
   if ((c = _clip_mget(ClipMachineMemory, m_geom, HASH_MAX_ASPECT)) != NULL)
    {
       geom->max_aspect = c->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
       mask |= GDK_HINT_ASPECT;
    }
   return mask;
}

CLIP_DLLEXPORT gint
_arr_to_valist(ClipMachine * ClipMachineMemory, ClipVar * marg, va_list valist)
{
  /*
     unsigned long n = marg->ClipArrVar_a_of_ClipVar.count;
     GValue value;
     gint i;
     for (i=0; i<n; i++)
     {
     ClipVar c;
     ClipVar *elem;

     memset(&value, 0, sizeof(value));
     elem = marg->ClipArrVar_a_of_ClipVar.items+i;
     switch (_clip_type(elem))
     {
     case NUMERIC_type_of_ClipVarType:
     g_value_init(&value, G_TYPE_LONG);
     g_value_set_long(&value, elem->lv.l);
     break;
     case CHARACTER_type_of_ClipVarType:
     g_value_init(&value, G_TYPE_STRING);
     g_value_set_string(&value, elem->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf);
     break;
     case LOGICAL_type_of_ClipVarType:
     g_value_init(&value, G_TYPE_BOOLEAN);
     g_value_set_boolean(&value, elem->ClipLogVar_l_of_ClipVar.val);
     break;

     case DATE_type_of_ClipVarType:
     value[0] = G_TYPE_LONG;
     value[1] = elemv.ClipDateVar_d_of_ClipVar.julian;
     break;
     case DATETIME_type_of_ClipVarType:
     rc = "T";
     break;
     case OBJECT_type_of_ClipVarType:
     value[0] = G_TYPE_LONG;
     value[1] = elem.ClipDateVar_d_of_ClipVar.julian;
     break;
     case PCODE_type_of_ClipVarType:
     case CCODE_type_of_ClipVarType:
     rc = "B";
     break;
     case ARRAY_type_of_ClipVarType:
     rc = "A";
     break;
     case MAP_type_of_ClipVarType:
     rc = "O";
     break;
     }
     _clip_aget(ClipMachineMemory, marg, &c, 2, value);
     }
   */
   return 0;
}

CLIP_DLLEXPORT void
_map_to_gtk_accel_key(ClipMachine * ClipMachineMemory, ClipVar * cv, GtkAccelKey * key)
{
   double    accel_key, accel_mods, accel_flags;

   _clip_mgetn(ClipMachineMemory, cv, HASH_ACCEL_LEY, &accel_key);
   _clip_mgetn(ClipMachineMemory, cv, HASH_ACCEL_MODS, &accel_mods);
   _clip_mgetn(ClipMachineMemory, cv, HASH_ACCEL_FLAGS, &accel_flags);

   key->accel_key = (guint) accel_key;
   key->accel_mods = (GdkModifierType) accel_mods;
   key->accel_flags = (guint) accel_flags;
}

CLIP_DLLEXPORT void
_array_to_target_entry(ClipMachine * ClipMachineMemory, ClipVar * cv, GtkTargetEntry * target)
{
   target->target =
    cv->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar[0].ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
   target->flags =
    (guint) cv->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar[1].ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
   target->info =
    (guint) cv->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar[2].ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
}

CLIP_DLLEXPORT void
_map_to_stock_item(ClipMachine * ClipMachineMemory, ClipVar * cv, GtkStockItem * item)
{
   ClipVar  *c;

   c = _clip_mget(ClipMachineMemory, cv, HASH_STOCK_ID);
   item->stock_id = c->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;

   c = _clip_mget(ClipMachineMemory, cv, HASH_LABEL);
   item->label = c->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;

   c = _clip_mget(ClipMachineMemory, cv, HASH_MODIFIER);
   item->modifier = (GdkModifierType) c->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;

   c = _clip_mget(ClipMachineMemory, cv, HASH_KEYVAL);
   item->keyval = (guint) c->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;

   c = _clip_mget(ClipMachineMemory, cv, HASH_TRANSL_DOMAIN);
   item->translation_domain = c->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;

}

CLIP_DLLEXPORT void
_stock_item_to_map(ClipMachine * ClipMachineMemory, ClipVar * cv, GtkStockItem * item)
{

   _clip_map(ClipMachineMemory, cv);

   _clip_mputc(ClipMachineMemory, cv, HASH_STOCK_ID, item->stock_id, strlen(item->stock_id));
   _clip_mputc(ClipMachineMemory, cv, HASH_LABEL, item->label, strlen(item->label));
   _clip_mputn(ClipMachineMemory, cv, HASH_MODIFIER, item->modifier);
   _clip_mputn(ClipMachineMemory, cv, HASH_KEYVAL, item->keyval);
   _clip_mputc(ClipMachineMemory, cv, HASH_TRANSL_DOMAIN, item->translation_domain, strlen(item->translation_domain));

}

#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 4)
static void
callback_action(GtkAction * action, gpointer data)
{

   C_var    *c = (C_var *) data;

   ClipVar  *cfunc = _list_get_action(c->ClipMachineMemory, action);

   ClipVar   stack[1];

   ClipVar   res;

   if (cfunc)
    {
       memset(&stack, 0, sizeof(stack));
       memset(&res, 0, sizeof(ClipVar));
       _clip_mclone(c->ClipMachineMemory, &stack[0], &c->co->obj);
       _clip_eval(c->ClipMachineMemory, cfunc, 1, stack, &res);

       _clip_destroy(c->ClipMachineMemory, &res);

       _clip_destroy(c->ClipMachineMemory, &stack[0]);
    }

}

CLIP_DLLEXPORT void
_map_to_action_entry(ClipMachine * ClipMachineMemory, ClipVar * cv, GtkActionEntry * act)
{
   ClipVar  *c;

   c = _clip_mget(ClipMachineMemory, cv, HASH_NAME);
   act->name = c->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;

   c = _clip_mget(ClipMachineMemory, cv, HASH_STOCK_ID);
   act->stock_id = c->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;

   c = _clip_mget(ClipMachineMemory, cv, HASH_LABEL);
   act->label = c->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;

   c = _clip_mget(ClipMachineMemory, cv, HASH_ACCELERATOR);
   act->accelerator = c->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;

   c = _clip_mget(ClipMachineMemory, cv, HASH_TOOLTIP);
   act->tooltip = c->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;

   c = _clip_mget(ClipMachineMemory, cv, HASH_CALLBACK);
   act->callback = (GCallback) callback_action;
   _list_put_action(ClipMachineMemory, act, c);

}

CLIP_DLLEXPORT void
_map_to_toggle_action_entry(ClipMachine * ClipMachineMemory, ClipVar * cv, GtkToggleActionEntry * act)
{
   ClipVar  *c;

   c = _clip_mget(ClipMachineMemory, cv, HASH_NAME);
   act->name = c->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;

   c = _clip_mget(ClipMachineMemory, cv, HASH_STOCK_ID);
   act->stock_id = c->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;

   c = _clip_mget(ClipMachineMemory, cv, HASH_LABEL);
   act->label = c->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;

   c = _clip_mget(ClipMachineMemory, cv, HASH_ACCELERATOR);
   act->accelerator = c->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;

   c = _clip_mget(ClipMachineMemory, cv, HASH_TOOLTIP);
   act->tooltip = c->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;

   c = _clip_mget(ClipMachineMemory, cv, HASH_CALLBACK);
   act->callback = (GCallback) callback_action;
   _list_put_action(ClipMachineMemory, act, c);

   c = _clip_mget(ClipMachineMemory, cv, HASH_IS_ACTIVE);
   act->is_active = c->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;

}

CLIP_DLLEXPORT void
_map_to_radio_action_entry(ClipMachine * ClipMachineMemory, ClipVar * cv, GtkRadioActionEntry * act)
{
   ClipVar  *c;

   c = _clip_mget(ClipMachineMemory, cv, HASH_NAME);
   act->name = c->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;

   c = _clip_mget(ClipMachineMemory, cv, HASH_STOCK_ID);
   act->stock_id = c->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;

   c = _clip_mget(ClipMachineMemory, cv, HASH_LABEL);
   act->label = c->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;

   c = _clip_mget(ClipMachineMemory, cv, HASH_ACCELERATOR);
   act->accelerator = c->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;

   c = _clip_mget(ClipMachineMemory, cv, HASH_TOOLTIP);
   act->tooltip = c->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;

   c = _clip_mget(ClipMachineMemory, cv, HASH_VALUE);
   act->value = c->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;

}

CLIP_DLLEXPORT void
_file_filter_info_to_map(ClipMachine * ClipMachineMemory, GtkFileFilterInfo * info, ClipVar * cv)
{

   _clip_mputn(ClipMachineMemory, cv, HASH_CONTAINS, info->contains);
   _clip_mputc(ClipMachineMemory, cv, HASH_FILENAME, (gchar *) info->filename, strlen(info->filename));
   _clip_mputc(ClipMachineMemory, cv, HASH_URI, (gchar *) info->uri, strlen(info->uri));
   _clip_mputc(ClipMachineMemory, cv, HASH_DISPLAY_NAME, (gchar *) info->display_name, strlen(info->display_name));
   _clip_mputc(ClipMachineMemory, cv, HASH_MIME_TYPE, (gchar *) info->mime_type, strlen(info->mime_type));
}

CLIP_DLLEXPORT void
_map_to_file_filter_info(ClipMachine * ClipMachineMemory, ClipVar * cv, GtkFileFilterInfo * info)
{

   ClipVar  *c;

   c = _clip_mget(ClipMachineMemory, cv, HASH_CONTAINS);
   info->contains = c->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;

   c = _clip_mget(ClipMachineMemory, cv, HASH_FILENAME);
   info->filename = c->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;

   c = _clip_mget(ClipMachineMemory, cv, HASH_URI);
   info->uri = c->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;

   c = _clip_mget(ClipMachineMemory, cv, HASH_DISPLAY_NAME);
   info->display_name = c->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;

   c = _clip_mget(ClipMachineMemory, cv, HASH_MIME_TYPE);
   info->mime_type = c->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
}

#endif
