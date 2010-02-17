/*
    Copyright (C) 2001-2004  ITK
    Author  : Alexey M. Tkachenko <alexey@itk.ru>
              Elena V. Kornilova <alena@itk.ru>
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
/****           Combo box has no signals               ****/
/**********************************************************/

/* Register combo box in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_combo()
{
   return gtk_combo_get_type();
}

long
_clip_type_combo()
{
   return GTK_WIDGET_COMBO;
}

const char *
_clip_type_name_combo()
{
   return "GTK_WIDGET_COMBO";
}

int
clip_INIT___COMBO(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_combo, _clip_type_name_combo, _gtk_type_combo, _gtk_type_hbox, NULL);
   return 0;
}

/**** Combo box constructor ****/
int
clip_GTK_COMBONEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   GtkWidget *wid = NULL;
   C_widget *cwid;
   C_widget *centry, *clist, *cbutton, *cpopup, *cpopwin;
   CHECKOPT(1, MAP_type_of_ClipVarType);

   wid = gtk_combo_new();
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);

   centry = _register_widget(ClipMachineMemory, GTK_COMBO(wid)->entry, NULL);
   clist = _register_widget(ClipMachineMemory, GTK_COMBO(wid)->list, NULL);
   cbutton = _register_widget(ClipMachineMemory, GTK_COMBO(wid)->button, NULL);
   cpopup = _register_widget(ClipMachineMemory, GTK_COMBO(wid)->popup, NULL);
   cpopwin = _register_widget(ClipMachineMemory, GTK_COMBO(wid)->popwin, NULL);

   if (centry)
      {
	 _clip_madd(ClipMachineMemory, &cwid->obj, HASH_ENTRY, &centry->obj);
	 _clip_mputn(ClipMachineMemory, &centry->obj, HASH_PARENT, cwid->handle);
      }
   if (clist)
      {
	 _clip_madd(ClipMachineMemory, &cwid->obj, HASH_LIST, &clist->obj);
	 _clip_mputn(ClipMachineMemory, &clist->obj, HASH_PARENT, cwid->handle);
      }
   if (cbutton)
      {
	 _clip_madd(ClipMachineMemory, &cwid->obj, HASH_BUTTON, &cbutton->obj);
	 _clip_mputn(ClipMachineMemory, &cbutton->obj, HASH_PARENT, cwid->handle);
      }
   if (cpopup)
      {
	 _clip_madd(ClipMachineMemory, &cwid->obj, HASH_POPUP, &cpopup->obj);
	 _clip_mputn(ClipMachineMemory, &cpopup->obj, HASH_PARENT, cwid->handle);
      }
   if (cpopwin)
      {
	 _clip_madd(ClipMachineMemory, &cwid->obj, HASH_POPWIN, &cpopwin->obj);
	 _clip_mputn(ClipMachineMemory, &cpopwin->obj, HASH_PARENT, cwid->handle);
      }
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

static int
_combo_simple_list_select(GtkList * list, GtkWidget * widget, gpointer data)
{
   GtkWidget *child = NULL;
   GtkCombo *cb = GTK_COMBO(data);
   char *text;

   child = GTK_BIN(&(GTK_ITEM(widget)->bin))->child;
   if (GTK_IS_LABEL(child))
      {
	 gtk_label_get(GTK_LABEL(child), &text);
	 LOCALE_TO_UTF(text);
	 gtk_entry_set_text(GTK_ENTRY(cb->entry), text);
	 FREE_TEXT(text);
      }

   return 0;
}

static int
_combo_simple_find_in_list(GtkCombo * cb, G_CONST_RETURN gchar * ftext)
{
   GList *list = GTK_LIST(cb->list)->children;
   GtkWidget *child = NULL;
   int i;
   char *text;
   gchar *ft;

   ft = (gchar *) ftext;
   LOCALE_TO_UTF(ft);

   for (i = 0; list; i++, list = list->next)
      {
	 child = GTK_BIN(&(GTK_ITEM(list->data)->bin))->child;

	 if (GTK_IS_LABEL(child))
	    {
	       gtk_label_get(GTK_LABEL(child), &text);
	       if (cb->case_sensitive)
		  {
		     if (strcmp(ft, text) == 0)
			return i;
		  }
	       else
		  {
		     if (strcasecmp(ft, text) == 0)
			return i;
		  }
	    }
      }
   FREE_TEXT(ft);

   return -1;
}

static int
_combo_simple_entry_change(GtkEntry * entry, gpointer data)
{
   GtkCombo *cb = GTK_COMBO(data);
   int i;

   i = _combo_simple_find_in_list(cb, gtk_entry_get_text(entry));

   if (i == -1)
      gtk_list_unselect_all(GTK_LIST(cb->list));
   else
      {
	 gtk_signal_handler_block_by_func(GTK_OBJECT(cb->list), GSF(_combo_simple_list_select), data);
	 gtk_list_select_item(GTK_LIST(cb->list), i);
	 gtk_signal_handler_unblock_by_func(GTK_OBJECT(cb->list), GSF(_combo_simple_list_select), data);
      }
   return 0;
}

static int
_combo_simple_entry_focus_out(GtkWidget * entry, GdkEvent * event, gpointer data)
{
   GtkCombo *cb = GTK_COMBO(data);
   G_CONST_RETURN gchar *etext;
   int i;

   if (!cb->value_in_list)
      return 0;

   etext = gtk_entry_get_text(GTK_ENTRY(entry));

   if (cb->ok_if_empty && strlen(etext) == 0)
      return 0;

   i = _combo_simple_find_in_list(cb, etext);

   if (i == -1)
      gtk_window_set_focus(GTK_WINDOW(gtk_widget_get_toplevel(cb->entry)), cb->entry);

   return 0;
}

/**** Combo Simple constructor ****/
int
clip_GTK_COMBOSIMPLENEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   GtkWidget *wid = NULL;
   GtkWidget *list, *popup;
   C_widget *cwid;
   C_widget *centry, *clist, *cpopup;
   CHECKOPT(1, MAP_type_of_ClipVarType);

   wid = gtk_combo_new();
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   if (!cwid)
      goto err;

   cwid->objtype = GTK_WIDGET_COMBO_SIMPLE;

   gtk_widget_destroy(GTK_COMBO(wid)->list);
   gtk_widget_destroy(GTK_COMBO(wid)->button);
   gtk_widget_destroy(GTK_COMBO(wid)->popup);
   gtk_widget_destroy(GTK_COMBO(wid)->popwin);

   list = gtk_list_new();
   popup = gtk_scrolled_window_new(NULL, NULL);
   gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(popup), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
   gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(popup), list);

   GTK_COMBO(wid)->list = list;
   GTK_COMBO(wid)->popup = popup;

   gtk_list_set_selection_mode(GTK_LIST(list), GTK_SELECTION_SINGLE);
   gtk_signal_connect(GTK_OBJECT(list), "select-child", GSF(_combo_simple_list_select), wid);

   gtk_signal_connect(GTK_OBJECT(GTK_COMBO(wid)->entry), "changed", GSF(_combo_simple_entry_change), wid);
   gtk_signal_connect(GTK_OBJECT(GTK_COMBO(wid)->entry), "focus-out-event", GSF(_combo_simple_entry_focus_out), wid);

   centry = _register_widget(ClipMachineMemory, GTK_COMBO(wid)->entry, NULL);
   clist = _register_widget(ClipMachineMemory, GTK_COMBO(wid)->list, NULL);
   cpopup = _register_widget(ClipMachineMemory, GTK_COMBO(wid)->popup, NULL);

   if (centry)
      _clip_madd(ClipMachineMemory, &cwid->obj, HASH_ENTRY, &centry->obj);
   if (clist)
      _clip_madd(ClipMachineMemory, &cwid->obj, HASH_LIST, &clist->obj);
   if (cpopup)
      _clip_madd(ClipMachineMemory, &cwid->obj, HASH_POPUP, &cpopup->obj);

   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

/* the text in the entry must be or not be in the list */
int
clip_GTK_COMBOSETVALUEINLIST(ClipMachine * ClipMachineMemory)
{
   C_widget *ccmb = _fetch_cw_arg(ClipMachineMemory);
   gint val = _clip_parl(ClipMachineMemory, 2);
   gint ok_if_empty = _clip_parl(ClipMachineMemory, 3);
   CHECKCWID(ccmb, GTK_IS_COMBO);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      val = TRUE;
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      ok_if_empty = FALSE;
   gtk_combo_set_value_in_list(GTK_COMBO(ccmb->widget), val, ok_if_empty);
   return 0;
 err:
   return 1;
}

/* set/unset arrows working for changing the value (can be annoying */
int
clip_GTK_COMBOSETUSEARROWS(ClipMachine * ClipMachineMemory)
{
   C_widget *ccmb = _fetch_cw_arg(ClipMachineMemory);
   gint use_arrows = _clip_parl(ClipMachineMemory, 2);
   CHECKCWID(ccmb, GTK_IS_COMBO);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      use_arrows = TRUE;
   gtk_combo_set_use_arrows(GTK_COMBO(ccmb->widget), use_arrows);
   return 0;
 err:
   return 1;
}

/* up/down arrows change value if current value not in list */
int
clip_GTK_COMBOSETUSEARROWSALWAYS(ClipMachine * ClipMachineMemory)
{
   C_widget *ccmb = _fetch_cw_arg(ClipMachineMemory);
   gint use_arrows = _clip_parl(ClipMachineMemory, 2);
   CHECKCWID(ccmb, GTK_IS_COMBO);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      use_arrows = TRUE;
   gtk_combo_set_use_arrows_always(GTK_COMBO(ccmb->widget), use_arrows);
   return 0;
 err:
   return 1;
}

/* perform case-sensitive compares */
int
clip_GTK_COMBOSETCASESENSITIVE(ClipMachine * ClipMachineMemory)
{
   C_widget *ccmb = _fetch_cw_arg(ClipMachineMemory);
   gint case_sens = _clip_parl(ClipMachineMemory, 2);
   CHECKCWID(ccmb, GTK_IS_COMBO);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      case_sens = TRUE;
   gtk_combo_set_case_sensitive(GTK_COMBO(ccmb->widget), case_sens);
   return 0;
 err:
   return 1;
}

/* call this function on an item if it isn't a label or you
   want it to have a different value to be displayed in the entry */
int
clip_GTK_COMBOSETITEMSTRING(ClipMachine * ClipMachineMemory)
{
   C_widget *ccmb = _fetch_cw_arg(ClipMachineMemory);
   C_widget *citem = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   gchar *string = _clip_parc(ClipMachineMemory, 3);
   gchar empty_string[] = "\0";
   CHECKCWID(ccmb, GTK_IS_COMBO);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(citem, GTK_IS_WIDGET);
   CHECKOPT(3, CHARACTER_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      string = empty_string;
   LOCALE_TO_UTF(string);
   gtk_combo_set_item_string(GTK_COMBO(ccmb->widget), GTK_ITEM(citem->widget), string);
   FREE_TEXT(string);
   return 0;
 err:
   return 1;
}

/* simple interface */
int
clip_GTK_COMBOSETPOPDOWNSTRINGS(ClipMachine * ClipMachineMemory)
{
   C_widget *ccmb = _fetch_cw_arg(ClipMachineMemory);
   ClipArrVar *astr = (ClipArrVar *) _clip_vptr(_clip_spar(ClipMachineMemory, 2));
   gint num = INT_OPTION(ClipMachineMemory, 3, 1);
   GList *str_list = NULL;
   gchar *text_utf;
   ClipStrVar *s;
   int i;
   CHECKCWID(ccmb, GTK_IS_COMBO);
   CHECKOPT(2, ARRAY_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType || astr->count_of_ClipArrVar == 0)
      return 0;
   for (i = 0; i < astr->count_of_ClipArrVar; i++)
      {
			if (astr->ClipVar_items_of_ClipArrVar[i].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType)
	    continue;
	 s = (ClipStrVar *) _clip_vptr(&astr->ClipVar_items_of_ClipArrVar[i]);
	text_utf = s->ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;

	 if (ccmb->objtype == GTK_WIDGET_COMBO_SIMPLE)
	    {
	       LOCALE_TO_UTF(text_utf);
	       str_list = g_list_append(str_list, gtk_list_item_new_with_label(text_utf));
	       FREE_TEXT(text_utf);
	    }
	 else
	    {
	       text_utf = g_locale_to_utf8(text_utf, strlen(text_utf), NULL, NULL, NULL);
	       str_list = g_list_append(str_list, text_utf);
	      //g_free(text_utf);
	    }

      }
   if (ccmb->objtype == GTK_WIDGET_COMBO_SIMPLE)
      {
	 gtk_list_clear_items(GTK_LIST(GTK_COMBO(ccmb->widget)->list), 0, -1);
	 gtk_list_append_items(GTK_LIST(GTK_COMBO(ccmb->widget)->list), str_list);
      }
   else
      gtk_combo_set_popdown_strings(GTK_COMBO(ccmb->widget), str_list);

  /* set item */
   num--;
   if (num > -1)
      {
	 if (num > astr->count_of_ClipArrVar)
	    num = 0;
	 s = (ClipStrVar *) _clip_vptr(&astr->ClipVar_items_of_ClipArrVar[num]);
	text_utf = s->ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
	 LOCALE_TO_UTF(text_utf);
	 gtk_entry_set_text(GTK_ENTRY(GTK_COMBO(ccmb->widget)->entry), text_utf);
	 FREE_TEXT(text_utf);
      }
   else
      gtk_entry_set_text(GTK_ENTRY(GTK_COMBO(ccmb->widget)->entry), "");

   return 0;
 err:
   return 1;
}

int
clip_GTK_COMBODISABLEACTIVATE(ClipMachine * ClipMachineMemory)
{
   C_widget *ccmb = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(ccmb, GTK_IS_COMBO);
   gtk_combo_disable_activate(GTK_COMBO(ccmb->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_COMBOGETSELECTIONINDEX(ClipMachine * ClipMachineMemory)
{
   C_widget *ccmb = _fetch_cw_arg(ClipMachineMemory);
   GtkList *list;
   GList *selection;
   int index;

   CHECKCWID(ccmb, GTK_IS_COMBO);
   list = GTK_LIST(GTK_COMBO(ccmb->widget)->list);
   selection = list->selection;
   if (selection)
      {
	 index = g_list_index(list->children, selection->data);
	 _clip_retni(ClipMachineMemory, index + 1);
      }
   else
      _clip_retni(ClipMachineMemory, -1);

   return 0;
 err:
   return 1;
}

int
clip_GTK_COMBOGETSELECTION(ClipMachine * ClipMachineMemory)
{
   C_widget *ccmb = _fetch_cw_arg(ClipMachineMemory);
   GtkList *list;
   GList *selection;
   gchar *data;
   guint i;

   CHECKCWID(ccmb, GTK_IS_COMBO);
   list = GTK_LIST(GTK_COMBO(ccmb->widget)->list);
   selection = list->selection;
   if (selection)
      {
	 i = g_list_index(list->children, selection->data);

	 data = (gchar *) gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(ccmb->widget)->entry));
	 LOCALE_FROM_UTF(data);
	 _clip_retc(ClipMachineMemory, data);
	 FREE_TEXT(data);
      }

   return 0;
 err:
   return 1;
}

int
clip_GTK_COMBOGETPOPDOWNSTRINGSLENGTH(ClipMachine * ClipMachineMemory)
{
   C_widget *ccmb = _fetch_cw_arg(ClipMachineMemory);
   GList *list;
   guint length;

   CHECKCWID(ccmb, GTK_IS_COMBO);
   list = gtk_container_get_children(GTK_CONTAINER(GTK_COMBO(ccmb->widget)->list));
   length = g_list_length(list);
   _clip_retni(ClipMachineMemory, length);

   return 0;
 err:
   return 1;
}

int
clip_GTK_COMBOGETPOPDOWNSTRINGS(ClipMachine * ClipMachineMemory)
{
   C_widget *ccmb = _fetch_cw_arg(ClipMachineMemory);
   ClipVar *cvarr = RETPTR(ClipMachineMemory);
   GList *list;
   long l;

   CHECKCWID(ccmb, GTK_IS_COMBO);
   list = gtk_container_get_children(GTK_CONTAINER(GTK_COMBO(ccmb->widget)->list));
   list = g_list_first(list);

   l = g_list_length(list);

   _clip_array(ClipMachineMemory, cvarr, 1, &l);
   for (l = 0; list; list = g_list_next(list), l++)
      {
	 ClipVar cv;
	 gchar *str;
	 gint n;
	 GtkWidget *label;

	 memset(&cv, 0, sizeof(cv));
	 str = (gchar *) gtk_object_get_data(GTK_OBJECT(list->data), "gtk-combo-string-value");
	 if (!str)
	    {
	       label = GTK_BIN(list->data)->child;
	       if (label && GTK_IS_LABEL(label))
		  gtk_label_get(GTK_LABEL(label), &str);
	    }

	 LOCALE_FROM_UTF(str);
	 n = strlen(str);
	 _clip_var_str(str, n, &cv);
	 FREE_TEXT(str);
	 _clip_aset(ClipMachineMemory, cvarr, &cv, 1, &l);
	 _clip_destroy(ClipMachineMemory, &cv);
      }

   return 0;
 err:
   return 1;
}
