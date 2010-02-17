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

/*********************** SIGNALS **************************/

static gint
handle_commit_signal(GtkIMContext * imcontext, gchar * arg1, C_signal * cs)
{
   OBJECTPREPARECV(cs, cv);
   _clip_mputc(cs->co->cmachine, &cv, HASH_ARG1, arg1, strlen(arg1));
   OBJECTINVOKESIGHANDLER(cs, cv);
}

static gint
handle_delete_surrounding_signal(GtkIMContext * imcontext, gint arg1, gint arg2, C_signal * cs)
{
   OBJECTPREPARECV(cs, cv);
   _clip_mputn(cs->co->cmachine, &cv, HASH_ARG1, arg1);
   _clip_mputn(cs->co->cmachine, &cv, HASH_ARG2, arg2);
   OBJECTINVOKESIGHANDLER(cs, cv);
}

static SignalTable im_context_signals[] = {
   {"commit", GSF(handle_commit_signal), ESF(object_emit_signal),
    GTK_COMMIT_SIGNAL},
   {"delete-surrounding", GSF(handle_delete_surrounding_signal),
    ESF(object_emit_signal), GTK_DELETE_SURROUNDING_SIGNAL},
   {"preedit-changed", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_PREEDIT_CHANGED_SIGNAL},
   {"preedit-end", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_PREEDIT_END_SIGNAL},
   {"preedit-start", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_PREEDIT_START_SIGNAL},
   {"retrieve-surrounding", GSF(widget_signal_handler),
    ESF(object_emit_signal), GTK_RETRIEVE_SURROUNDING_SIGNAL},
   {"", NULL, NULL, 0}
};

/**********************************************************/
CLIP_DLLEXPORT GtkType
_gtk_type_im_context()
{
   return GTK_TYPE_IM_CONTEXT;
}

long
_clip_type_im_context()
{
   return GTK_OBJECT_IM_CONTEXT;
}

const char *
_clip_type_name_im_context()
{
   return "GTK_OBJECT_IM_CONTEXT";
}

/* Register  in global table */
int
clip_INIT___IMCONTEXT(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_im_context, _clip_type_name_im_context, _gtk_type_im_context, _gtk_type_object, im_context_signals);
   return 0;
}

CLIP_DLLEXPORT GtkType
_gtk_type_im_context_simple()
{
   return GTK_TYPE_IM_CONTEXT_SIMPLE;
}

long
_clip_type_im_context_simple()
{
   return GTK_OBJECT_IM_CONTEXT_SIMPLE;
}

const char *
_clip_type_name_im_context_simple()
{
   return "GTK_OBJECT_IM_CONTEXT_SIMPLE";
}

/* Register  in global table */
int
clip_INIT___IMCONTEXTSIMPLE(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_im_context_simple, _clip_type_name_im_context_simple, _gtk_type_im_context_simple, _gtk_type_im_context, NULL);
   return 0;
}

CLIP_DLLEXPORT GtkType
_gtk_type_im_multicontext()
{
   return GTK_TYPE_IM_MULTICONTEXT;
}

long
_clip_type_im_multicontext()
{
   return GTK_OBJECT_IM_MULTICONTEXT;
}

const char *
_clip_type_name_im_multicontext()
{
   return "GTK_OBJECT_IM_MULTICONTEXT";
}

/* Register  in global table */
int
clip_INIT___IMMULTICONTEXT(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_im_multicontext, _clip_type_name_im_multicontext, _gtk_type_im_multicontext, _gtk_type_im_context, NULL);
   return 0;
}

/******************************************************************************
* gtk_ImContextSetClientWindow(imContextObj, GdkWindowObj) --> NIL
******************************************************************************/
int
clip_GTK_IMCONTEXTSETCLIENTWINDOW(ClipMachine * ClipMachineMemory)
{
   C_object *cimcontext = _fetch_co_arg(ClipMachineMemory);
   C_object *cwin = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cimcontext, GTK_IS_IM_CONTEXT(cimcontext->object));
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJOPT(cwin, GDK_IS_WINDOW(cwin->object));

   gtk_im_context_set_client_window(GTK_IM_CONTEXT(cimcontext->object), GDK_WINDOW(cwin->object));

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ImContextGetPreeditString(imContextObj, @string, @PangoAttrListObj, @cursor_pos) --> NIL
*
******************************************************************************/
int
clip_GTK_IMCONTEXTGETPREEDITSTRING(ClipMachine * ClipMachineMemory)
{
   C_object *cimcontext = _fetch_co_arg(ClipMachineMemory);
   ClipVar *cvlist = _clip_par(ClipMachineMemory, 3);
   gchar *string;
   gint cursor_pos;
   PangoAttrList *list;
   C_object *clist;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cimcontext, GTK_IS_IM_CONTEXT(cimcontext->object));

   gtk_im_context_get_preedit_string(GTK_IM_CONTEXT(cimcontext->object), &string, &list, &cursor_pos);

   if (list)
      {
	 clist = _register_object(ClipMachineMemory, list, GTK_TYPE_PANGO_ATTR_LIST, NULL, NULL);
	 if (clist)
	    _clip_mclone(ClipMachineMemory, cvlist, &clist->obj);
      }

   _clip_storc(ClipMachineMemory, string, 2, 0);
   _clip_storni(ClipMachineMemory, cursor_pos, 4, 0);

   g_free(string);
   pango_attr_list_unref(list);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ImContextFilterKeypress(imContextObj, GdkEventObj) --> TRUE || FALSE
*
******************************************************************************/
int
clip_GTK_IMCONTEXTFILTERKEYPRESS(ClipMachine * ClipMachineMemory)
{
   C_object *cimcontext = _fetch_co_arg(ClipMachineMemory);
   C_object *cevent = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cimcontext, GTK_IS_IM_CONTEXT(cimcontext->object));
   CHECKARG(2, MAP_type_of_ClipVarType);
   CHECKCOBJ(cevent, GDK_IS_EVENT_KEY(cevent));

   _clip_retl(ClipMachineMemory, gtk_im_context_filter_keypress(GTK_IM_CONTEXT(cimcontext->object), GDK_EVENT_KEY(cevent->object)));

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ImContextFocusIn(imContextObj) --> NIL
*
******************************************************************************/
int
clip_GTK_IMCONTEXTFOCUSIN(ClipMachine * ClipMachineMemory)
{
   C_object *cimcontext = _fetch_co_arg(ClipMachineMemory);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cimcontext, GTK_IS_IM_CONTEXT(cimcontext->object));

   gtk_im_context_focus_in(GTK_IM_CONTEXT(cimcontext->object));

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ImContextFocusOut(imContextObj) --> NIL
*
******************************************************************************/
int
clip_GTK_IMCONTEXTFOCUSOUT(ClipMachine * ClipMachineMemory)
{
   C_object *cimcontext = _fetch_co_arg(ClipMachineMemory);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cimcontext, GTK_IS_IM_CONTEXT(cimcontext->object));

   gtk_im_context_focus_out(GTK_IM_CONTEXT(cimcontext->object));

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ImContextReset(imContextObj) --> NIL
*
******************************************************************************/
int
clip_GTK_IMCONTEXTRESET(ClipMachine * ClipMachineMemory)
{
   C_object *cimcontext = _fetch_co_arg(ClipMachineMemory);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cimcontext, GTK_IS_IM_CONTEXT(cimcontext->object));

   gtk_im_context_reset(GTK_IM_CONTEXT(cimcontext->object));

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ImContextSetCursorLocation(imContextObj, aRectangle) --> NIL
******************************************************************************/
int
clip_GTK_IMCONTEXTSETCURSORLOCATION(ClipMachine * ClipMachineMemory)
{
   C_object *cimcontext = _fetch_co_arg(ClipMachineMemory);
   ClipVar *cvarea = _clip_spar(ClipMachineMemory, 2);
   GdkRectangle area;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cimcontext, GTK_IS_IM_CONTEXT(cimcontext->object));
   CHECKARG(2, MAP_type_of_ClipVarType);

   _map_get_gdk_rectangle(ClipMachineMemory, cvarea, &area);
   gtk_im_context_set_cursor_location(GTK_IM_CONTEXT(cimcontext->object), &area);

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ImContextSetUsePreedit(imContextObj, lUse) --> NIL
******************************************************************************/
int
clip_GTK_IMCONTEXTSETUSEPREEDIT(ClipMachine * ClipMachineMemory)
{
   C_object *cimcontext = _fetch_co_arg(ClipMachineMemory);
   gboolean usep = _clip_parl(ClipMachineMemory, 2);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cimcontext, GTK_IS_IM_CONTEXT(cimcontext->object));
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_im_context_set_use_preedit(GTK_IM_CONTEXT(cimcontext->object), usep);

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ImContextSetSurrounding(imContextObj, sText, ncursor_index) --> NIL
******************************************************************************/
int
clip_GTK_IMCONTEXTSETSURROUNDING(ClipMachine * ClipMachineMemory)
{
   C_object *cimcontext = _fetch_co_arg(ClipMachineMemory);
   gchar *text = _clip_parc(ClipMachineMemory, 2);
   gint cursor_ind = _clip_parni(ClipMachineMemory, 3);
   gint len;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cimcontext, GTK_IS_IM_CONTEXT(cimcontext->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);

   LOCALE_TO_UTF(text);
   len = strlen(text);
   gtk_im_context_set_surrounding(GTK_IM_CONTEXT(cimcontext->object), text, len, cursor_ind);
   FREE_TEXT(text);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ImContextGetSurrounding(imContextObj, @sText, @ncursor_index) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_IMCONTEXTGETSURROUNDING(ClipMachine * ClipMachineMemory)
{
   C_object *cimcontext = _fetch_co_arg(ClipMachineMemory);
   gchar *text;
   gint cursor_ind;
   gboolean ret;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cimcontext, GTK_IS_IM_CONTEXT(cimcontext->object));

   ret = gtk_im_context_get_surrounding(GTK_IM_CONTEXT(cimcontext->object), &text, &cursor_ind);
   _clip_retl(ClipMachineMemory, ret);
   if (ret)
      {
	 LOCALE_FROM_UTF(text);
	 _clip_storc(ClipMachineMemory, text, 2, 0);
	 FREE_TEXT(text);
	 _clip_storni(ClipMachineMemory, cursor_ind, 3, 0);
      }
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ImContextDeleteSurrounding(imContextObj, nOffset, nChars) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_IMCONTEXTDELETESURROUNDING(ClipMachine * ClipMachineMemory)
{
   C_object *cimcontext = _fetch_co_arg(ClipMachineMemory);
   gint offset = _clip_parni(ClipMachineMemory, 2);
   gint chars = _clip_parni(ClipMachineMemory, 3);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cimcontext, GTK_IS_IM_CONTEXT(cimcontext->object));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);

   _clip_retl(ClipMachineMemory, gtk_im_context_delete_surrounding(GTK_IM_CONTEXT(cimcontext->object), offset, chars));
   return 0;
 err:
   return 1;
}

/******************************************************************************
*
*******************************************************************************/

/******************************************************************************
* gtk_ImContextSimpleNew(clipObj) --> imContext new object
******************************************************************************/
int
clip_GTK_IMCONTEXTSIMPLENEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   GtkIMContext *imcontext;
   C_object *cimcontext;

   CHECKOPT(1, MAP_type_of_ClipVarType);

   imcontext = gtk_im_context_simple_new();

   if (imcontext)
      {
	 cimcontext = _register_object(ClipMachineMemory, imcontext, GTK_TYPE_IM_CONTEXT, cv, NULL);
	 if (cimcontext)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cimcontext->obj);
      }
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ImContextSimpleAddTable(imContextSimpleObj, arrData, nMax_seq_name, nSeqs) --> NIL
******************************************************************************/
int
clip_GTK_IMCONTEXTSIMPLEADDTABLE(ClipMachine * ClipMachineMemory)
{
   C_object *cimcontext = _fetch_co_arg(ClipMachineMemory);
   ClipVar *arr = _clip_par(ClipMachineMemory, 2);
   gint max_seq_no = _clip_parni(ClipMachineMemory, 3);
   gint n_seq = _clip_parni(ClipMachineMemory, 4);

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cimcontext, GTK_IS_IM_CONTEXT_SIMPLE(cimcontext->object));
   CHECKARG(2, ARRAY_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);

   if (arr)
      {
	 guint16 data[arr->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar];
	 gint i;

	 for (i = 0; i < arr->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar; i++)
		 data[i] = arr->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar[i].ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
	 gtk_im_context_simple_add_table(GTK_IM_CONTEXT_SIMPLE(cimcontext->object), data, max_seq_no, n_seq);
      }
   return 0;
 err:
   return 1;
}

/******************************************************************************
*
*******************************************************************************/

/******************************************************************************
* gtk_ImMultiContextNew(clipObj) --> imContext new object
******************************************************************************/
int
clip_GTK_IMMULTICONTEXTNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   GtkIMContext *imcontext;
   C_object *cimcontext;

   CHECKOPT(1, MAP_type_of_ClipVarType);

   imcontext = gtk_im_multicontext_new();

   if (imcontext)
      {
	 cimcontext = _register_object(ClipMachineMemory, imcontext, GTK_TYPE_IM_CONTEXT, cv, NULL);
	 if (cimcontext)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cimcontext->obj);
      }
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ImMultiContextAppendMenuitems(imMultiContextObj, menuItemsObj) --> NIL
******************************************************************************/
int
clip_GTK_IMMULTICONTEXTAPPENDMENUITEMS(ClipMachine * ClipMachineMemory)
{
   C_object *cimcontext = _fetch_co_arg(ClipMachineMemory);
   C_widget *cmenu = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCOBJ(cimcontext, GTK_IS_IM_MULTICONTEXT(cimcontext->object));
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cmenu, GTK_IS_MENU_SHELL);

   gtk_im_multicontext_append_menuitems(GTK_IM_MULTICONTEXT(cimcontext->object), GTK_MENU_SHELL(cmenu->widget));

   return 0;
 err:
   return 1;
}
