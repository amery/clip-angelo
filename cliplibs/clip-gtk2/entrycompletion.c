/*
    Copyright (C) 2004-2005  ITK
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
static    gint
handle_action_activated_signal(GtkEntryCompletion * completion, gint index, C_signal * cs)
{
   OBJECTPREPARECV(cs, cv);
   _clip_mputn(cs->co->cmachine, &cv, HASH_INDEX, index);
   OBJECTINVOKESIGHANDLER(cs, cv);
}

static    gint
handle_match_selected_signal(GtkEntryCompletion * completion, GtkTreeModel * model, GtkTreeIter * iter, C_signal * cs)
{
   C_object *cmodell, *citer;

   OBJECTPREPARECV(cs, cv);
   cmodell = _list_get_cobject(cs->co->cmachine, model);
   if (!cmodell)
      cmodell = _register_object(cs->co->cmachine, model, GTK_TYPE_TREE_MODEL, NULL, NULL);
   _clip_madd(cs->co->cmachine, &cv, HASH_TREEMODEL, &cmodell->obj);

   citer = _list_get_cobject(cs->co->cmachine, iter);
   if (!citer)
      citer = _register_object(cs->co->cmachine, iter, GTK_TYPE_TREE_ITER, NULL, NULL);
   _clip_madd(cs->co->cmachine, &cv, HASH_TREEITER, &citer->obj);
   OBJECTINVOKESIGHANDLER(cs, cv);
}

static SignalTable entry_completion_signals[] = {
   {"action-activated", GSF(handle_action_activated_signal),
    ESF(object_emit_signal), GTK_ACTION_ACTIVATED_SIGNAL},
   {"match-selected", GSF(handle_match_selected_signal),
    ESF(object_emit_signal), GTK_MATCH_SELECTED_SIGNAL},
   {"", NULL, NULL, 0}
};

/**********************************************************/
CLIP_DLLEXPORT GtkType
_gtk_type_entry_completion()
{
   return GTK_TYPE_ENTRY_COMPLETION;
}

long
_clip_type_entry_completion()
{
   return GTK_OBJECT_ENTRY_COMPLETION;
}

const char *
_clip_type_name_entry_completion()
{
   return "GTK_OBJECT_ENTRY_COMPLETION";
}

int
clip_INIT___ENTRYCOMPLETION(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_entry_completion,
		    _clip_type_name_entry_completion, _gtk_type_entry_completion, NULL, entry_completion_signals);
   return 0;
}

int
clip_GTK_ENTRYCOMPLETIONNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   GtkEntryCompletion *completion;

   C_object *ccompletion;

   CHECKOPT(1, MAP_type_of_ClipVarType);

   completion = gtk_entry_completion_new();

   if (completion)
    {
       ccompletion = _list_get_cobject(ClipMachineMemory, completion);
       if (!ccompletion)
	  ccompletion = _register_object(ClipMachineMemory, completion, GTK_TYPE_ENTRY_COMPLETION, cv, NULL);
       if (ccompletion)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &ccompletion->obj);
    }

   return 0;
 err:
   return 1;
}

int
clip_GTK_ENTRYCOMPLETIONGETENTRY(ClipMachine * ClipMachineMemory)
{
   C_object *ccompletion = _fetch_co_arg(ClipMachineMemory);

   C_widget *cwid;

   GtkWidget *wid;

   CHECKCOBJ(ccompletion, GTK_IS_ENTRY_COMPLETION(ccompletion->object));

   wid = gtk_entry_completion_get_entry(GTK_ENTRY_COMPLETION(ccompletion->object));

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
clip_GTK_ENTRYCOMPLETIONSETMODEL(ClipMachine * ClipMachineMemory)
{
   C_object *ccompletion = _fetch_co_arg(ClipMachineMemory);

   C_object *cmodell = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCOBJ(ccompletion, GTK_IS_ENTRY_COMPLETION(ccompletion->object));
   CHECKCOBJ(cmodell, GTK_IS_TREE_MODEL(cmodell->object));

   gtk_entry_completion_set_model(GTK_ENTRY_COMPLETION(ccompletion->object), GTK_TREE_MODEL(cmodell->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ENTRYCOMPLETIONGETMODEL(ClipMachine * ClipMachineMemory)
{
   C_object *ccompletion = _fetch_co_arg(ClipMachineMemory);

   C_object *cmodell;

   GtkTreeModel *model;

   CHECKCOBJ(ccompletion, GTK_IS_ENTRY_COMPLETION(ccompletion->object));

   model = gtk_entry_completion_get_model(GTK_ENTRY_COMPLETION(ccompletion->object));

   if (model)
    {
       cmodell = _list_get_cobject(ClipMachineMemory, model);
       if (!cmodell)
	  cmodell = _register_object(ClipMachineMemory, model, GTK_TYPE_TREE_MODEL, NULL, NULL);
       if (cmodell)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cmodell->obj);
    }
   return 0;
 err:
   return 1;
}

static    gboolean
_match_func(GtkEntryCompletion * completion, const gchar * key, GtkTreeIter * iter, gpointer data)
{
   C_var    *c = (C_var *) data;

   C_object *ccomp = _list_get_cobject(c->ClipMachineMemory, completion);

   C_object *citer = _list_get_cobject(c->ClipMachineMemory, iter);

   ClipVar   stack[4];

   ClipVar   res;

   gboolean  ret = TRUE;

   if (!ccomp)
      ccomp = _register_object(c->ClipMachineMemory, completion, GTK_TYPE_ENTRY_COMPLETION, NULL, NULL);
   if (!citer)
      citer = _register_object(c->ClipMachineMemory, iter, GTK_TYPE_TREE_ITER, NULL, NULL);

   memset(&stack, 0, sizeof(stack));
   memset(&res, 0, sizeof(ClipVar));
   _clip_mclone(c->cw->cmachine, &stack[0], &c->co->obj);
   _clip_mclone(c->cw->cmachine, &stack[1], &ccomp->obj);
   _clip_var_str((gchar *) key, strlen(key), &stack[2]);
   _clip_mclone(c->cw->cmachine, &stack[3], &citer->obj);

   if (_clip_eval(c->ClipMachineMemory, &(c->cfunc), 4, stack, &res) == 0)
      ret =
       res.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType ==
       LOGICAL_type_of_ClipVarType ? res.ClipLogVar_l_of_ClipVar.value_of_ClipLogVar : ret;

   _clip_destroy(c->ClipMachineMemory, &res);
   _clip_destroy(c->ClipMachineMemory, &stack[0]);
   _clip_destroy(c->ClipMachineMemory, &stack[1]);
   _clip_destroy(c->ClipMachineMemory, &stack[2]);
   _clip_destroy(c->ClipMachineMemory, &stack[3]);
   return ret;
}

int
clip_GTK_ENTRYCOMPLETIONSETMATCHFUNC(ClipMachine * ClipMachineMemory)
{
   C_object *ccompletion = _fetch_co_arg(ClipMachineMemory);

   ClipVar  *func = _clip_spar(ClipMachineMemory, 2);

   C_var    *c = 0;

   CHECKCOBJ(ccompletion, GTK_IS_ENTRY_COMPLETION(ccompletion->object));
   CHECKARG2(2, PCODE_type_of_ClipVarType, CCODE_type_of_ClipVarType);

   c->ClipMachineMemory = ClipMachineMemory;
   c->co = ccompletion;
   _clip_mclone(ClipMachineMemory, &c->cfunc, func);
   gtk_entry_completion_set_match_func(GTK_ENTRY_COMPLETION
				       (ccompletion->object), (GtkEntryCompletionMatchFunc) _match_func, c, NULL);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ENTRYCOMPLETIONSETMINIMUMKEYLENGTH(ClipMachine * ClipMachineMemory)
{
   C_object *ccompletion = _fetch_co_arg(ClipMachineMemory);

   gint      length = _clip_parni(ClipMachineMemory, 2);

   CHECKCOBJ(ccompletion, GTK_IS_ENTRY_COMPLETION(ccompletion->object));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_entry_completion_set_minimum_key_length(GTK_ENTRY_COMPLETION(ccompletion->object), length);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ENTRYCOMPLETIONGETMINIMUMKEYLENGTH(ClipMachine * ClipMachineMemory)
{
   C_object *ccompletion = _fetch_co_arg(ClipMachineMemory);

   CHECKCOBJ(ccompletion, GTK_IS_ENTRY_COMPLETION(ccompletion->object));

   _clip_parni(ClipMachineMemory, gtk_entry_completion_get_minimum_key_length(GTK_ENTRY_COMPLETION(ccompletion->object)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ENTRYCOMPLETIONCOMPLETE(ClipMachine * ClipMachineMemory)
{
   C_object *ccompletion = _fetch_co_arg(ClipMachineMemory);

   CHECKCOBJ(ccompletion, GTK_IS_ENTRY_COMPLETION(ccompletion->object));

   gtk_entry_completion_complete(GTK_ENTRY_COMPLETION(ccompletion->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ENTRYCOMPLETIONINSERTACTIONTEXT(ClipMachine * ClipMachineMemory)
{
   C_object *ccompletion = _fetch_co_arg(ClipMachineMemory);

   gint      index = _clip_parni(ClipMachineMemory, 2);

   gchar    *text = _clip_parc(ClipMachineMemory, 3);

   CHECKCOBJ(ccompletion, GTK_IS_ENTRY_COMPLETION(ccompletion->object));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, CHARACTER_type_of_ClipVarType);

   index--;
   LOCALE_TO_UTF(text);
   gtk_entry_completion_insert_action_text(GTK_ENTRY_COMPLETION(ccompletion->object), index, text);
   FREE_TEXT(text);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ENTRYCOMPLETIONINSERTACTIONMARKUP(ClipMachine * ClipMachineMemory)
{
   C_object *ccompletion = _fetch_co_arg(ClipMachineMemory);

   gint      index = _clip_parni(ClipMachineMemory, 2);

   gchar    *text = _clip_parc(ClipMachineMemory, 3);

   CHECKCOBJ(ccompletion, GTK_IS_ENTRY_COMPLETION(ccompletion->object));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, CHARACTER_type_of_ClipVarType);

   index--;
   LOCALE_TO_UTF(text);
   gtk_entry_completion_insert_action_markup(GTK_ENTRY_COMPLETION(ccompletion->object), index, text);
   FREE_TEXT(text);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ENTRYCOMPLETIONDELETEACTION(ClipMachine * ClipMachineMemory)
{
   C_object *ccompletion = _fetch_co_arg(ClipMachineMemory);

   gint      index = _clip_parni(ClipMachineMemory, 2);

   CHECKCOBJ(ccompletion, GTK_IS_ENTRY_COMPLETION(ccompletion->object));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   index--;
   gtk_entry_completion_delete_action(GTK_ENTRY_COMPLETION(ccompletion->object), index);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ENTRYCOMPLETIONSETTEXTCOLUMN(ClipMachine * ClipMachineMemory)
{
   C_object *ccompletion = _fetch_co_arg(ClipMachineMemory);

   gint      index = _clip_parni(ClipMachineMemory, 2);

   CHECKCOBJ(ccompletion, GTK_IS_ENTRY_COMPLETION(ccompletion->object));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   index--;
   gtk_entry_completion_set_text_column(GTK_ENTRY_COMPLETION(ccompletion->object), index);

   return 0;
 err:
   return 1;
}

#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 6)
int
clip_GTK_ENTRYCOMPLETIONGETINLINECOMPLETION(ClipMachine * ClipMachineMemory)
{
   C_object *ccompletion = _fetch_co_arg(ClipMachineMemory);

   CHECKCOBJ(ccompletion, GTK_IS_ENTRY_COMPLETION(ccompletion->object));

   _clip_retl(ClipMachineMemory, gtk_entry_completion_get_inline_completion(GTK_ENTRY_COMPLETION(ccompletion->object)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ENTRYCOMPLETIONGETPOPUPCOMPLETION(ClipMachine * ClipMachineMemory)
{
   C_object *ccompletion = _fetch_co_arg(ClipMachineMemory);

   CHECKCOBJ(ccompletion, GTK_IS_ENTRY_COMPLETION(ccompletion->object));

   _clip_retl(ClipMachineMemory, gtk_entry_completion_get_popup_completion(GTK_ENTRY_COMPLETION(ccompletion->object)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ENTRYCOMPLETIONGETTEXTCOLUMN(ClipMachine * ClipMachineMemory)
{
   C_object *ccompletion = _fetch_co_arg(ClipMachineMemory);

   gint      column;

   CHECKCOBJ(ccompletion, GTK_IS_ENTRY_COMPLETION(ccompletion->object));

   column = gtk_entry_completion_get_text_column(GTK_ENTRY_COMPLETION(ccompletion->object));
   column++;

   _clip_retni(ClipMachineMemory, column);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ENTRYCOMPLETIONINSERTPREFIX(ClipMachine * ClipMachineMemory)
{
   C_object *ccompletion = _fetch_co_arg(ClipMachineMemory);

   CHECKCOBJ(ccompletion, GTK_IS_ENTRY_COMPLETION(ccompletion->object));

   gtk_entry_completion_insert_prefix(GTK_ENTRY_COMPLETION(ccompletion->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_ENTRYCOMPLETIONSETINLINECOMPLETION(ClipMachine * ClipMachineMemory)
{
   C_object *ccompletion = _fetch_co_arg(ClipMachineMemory);

   gboolean  set = _clip_parl(ClipMachineMemory, 2);

   CHECKCOBJ(ccompletion, GTK_IS_ENTRY_COMPLETION(ccompletion->object));
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_entry_completion_set_inline_completion(GTK_ENTRY_COMPLETION(ccompletion->object), set);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ENTRYCOMPLETIONSETPOPUPCOMPLETION(ClipMachine * ClipMachineMemory)
{
   C_object *ccompletion = _fetch_co_arg(ClipMachineMemory);

   gboolean  set = _clip_parl(ClipMachineMemory, 2);

   CHECKCOBJ(ccompletion, GTK_IS_ENTRY_COMPLETION(ccompletion->object));
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_entry_completion_set_popup_completion(GTK_ENTRY_COMPLETION(ccompletion->object), set);

   return 0;
 err:
   return 1;
}
#endif
