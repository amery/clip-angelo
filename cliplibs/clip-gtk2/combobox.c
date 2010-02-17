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

static GtkTreeIter _Iter;
static GtkTreeIter *Iter = &_Iter;
/**********************************************************/
static SignalTable combo_box_signals[] = {
   {"changed", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_CHANGED_SIGNAL},
   {"", NULL, NULL, 0}
};

/**********************************************************/

/* Register combo box in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_combo_box()
{
   return GTK_TYPE_COMBO_BOX;
}

long
_clip_type_combo_box()
{
   return GTK_WIDGET_COMBO_BOX;
}

const char *
_clip_type_name_combo_box()
{
   return "GTK_WIDGET_COMBO_BOX";
}

int
clip_INIT___COMBOBOX(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_combo_box, _clip_type_name_combo_box, _gtk_type_combo_box, _gtk_type_bin, combo_box_signals);
   return 0;
}

/* Register combo box entry in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_combo_box_entry()
{
   return GTK_TYPE_COMBO_BOX_ENTRY;
}

long
_clip_type_combo_box_entry()
{
   return GTK_WIDGET_COMBO_BOX_ENTRY;
}

const char *
_clip_type_name_combo_box_entry()
{
   return "GTK_WIDGET_COMBO_BOX_ENTRY";
}

int
clip_INIT___COMBOBOXENTRY(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_combo_box_entry, _clip_type_name_combo_box_entry, _gtk_type_combo_box_entry, _gtk_type_combo_box, NULL);
   return 0;
}

/**** Combo box constructor ****/
int
clip_GTK_COMBOBOXNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   GtkWidget *wid = NULL;
   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);

   wid = gtk_combo_box_new();
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

int
clip_GTK_COMBOBOXNEWWITHMODEL(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   C_object *cmodel = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   GtkWidget *wid = NULL;
   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(cmodel, GTK_IS_TREE_MODEL(cmodel->object));

   wid = gtk_combo_box_new_with_model(GTK_TREE_MODEL(cmodel->object));
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

int
clip_GTK_COMBOBOXNEWTEXT(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   GtkWidget *wid = NULL;
   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);

   wid = gtk_combo_box_new_text();
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

int
clip_GTK_COMBOBOXSETWRAPWIDTH(ClipMachine * ClipMachineMemory)
{
   C_widget *ccmb = _fetch_cw_arg(ClipMachineMemory);
   gint width = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(ccmb, GTK_IS_COMBO_BOX);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);

   gtk_combo_box_set_wrap_width(GTK_COMBO_BOX(ccmb->widget), width);

   return 0;
 err:
   return 1;
}

int
clip_GTK_COMBOBOXSETROWSPANCOLUMN(ClipMachine * ClipMachineMemory)
{
   C_widget *ccmb = _fetch_cw_arg(ClipMachineMemory);
   gint row_span = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(ccmb, GTK_IS_COMBO_BOX);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);

   gtk_combo_box_set_row_span_column(GTK_COMBO_BOX(ccmb->widget), row_span);

   return 0;
 err:
   return 1;
}

int
clip_GTK_COMBOBOXSETCOLUMNSPANCOLUMN(ClipMachine * ClipMachineMemory)
{
   C_widget *ccmb = _fetch_cw_arg(ClipMachineMemory);
   gint column_span = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(ccmb, GTK_IS_COMBO_BOX);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);

   gtk_combo_box_set_column_span_column(GTK_COMBO_BOX(ccmb->widget), column_span);

   return 0;
 err:
   return 1;
}

int
clip_GTK_COMBOBOXGETACTIVE(ClipMachine * ClipMachineMemory)
{
   C_widget *ccmb = _fetch_cw_arg(ClipMachineMemory);
   gint index;

   CHECKCWID(ccmb, GTK_IS_COMBO_BOX);

   index = gtk_combo_box_get_active(GTK_COMBO_BOX(ccmb->widget));

   if (index > -1)
      index++;
   _clip_retni(ClipMachineMemory, index);

   return 0;
 err:
   return 1;
}

int
clip_GTK_COMBOBOXSETACTIVE(ClipMachine * ClipMachineMemory)
{
   C_widget *ccmb = _fetch_cw_arg(ClipMachineMemory);
   gint index = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(ccmb, GTK_IS_COMBO_BOX);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   index--;

   gtk_combo_box_set_active(GTK_COMBO_BOX(ccmb->widget), index);

   return 0;
 err:
   return 1;
}

int
clip_GTK_COMBOBOXSETACTIVEITER(ClipMachine * ClipMachineMemory)
{
   C_widget *ccmb = _fetch_cw_arg(ClipMachineMemory);
   C_object *citer = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(ccmb, GTK_IS_COMBO_BOX);
   CHECKCOBJ(citer, GTK_IS_TREE_ITER(citer->object));

   gtk_combo_box_set_active_iter(GTK_COMBO_BOX(ccmb->widget), GTK_TREE_ITER(citer->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_COMBOBOXGETACTIVEITER(ClipMachine * ClipMachineMemory)
{
   C_widget *ccmb = _fetch_cw_arg(ClipMachineMemory);
   ClipVar *cv = _clip_spar(ClipMachineMemory, 2);
   C_object *citer;

   CHECKCWID(ccmb, GTK_IS_COMBO_BOX);

   _clip_retl(ClipMachineMemory, gtk_combo_box_get_active_iter(GTK_COMBO_BOX(ccmb->widget), Iter));

   if (Iter)
      {
	 citer = _list_get_cobject(ClipMachineMemory, Iter);
	 if (!citer)
	    citer = _register_object(ClipMachineMemory, Iter, GTK_TYPE_TREE_ITER, NULL, NULL);
	 if (citer)
	    _clip_mclone(ClipMachineMemory, cv, &citer->obj);
      }

   return 0;
 err:
   return 1;
}

int
clip_GTK_COMBOBOXSETMODEL(ClipMachine * ClipMachineMemory)
{
   C_widget *ccmb = _fetch_cw_arg(ClipMachineMemory);
   C_object *cmodel = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(ccmb, GTK_IS_COMBO_BOX);
   CHECKCOBJ(cmodel, GTK_IS_TREE_MODEL(cmodel->object));

   gtk_combo_box_set_model(GTK_COMBO_BOX(ccmb->widget), GTK_TREE_MODEL(cmodel->object));

   return 0;
 err:
   return 1;
}

int
clip_GTK_COMBOBOXGETMODEL(ClipMachine * ClipMachineMemory)
{
   C_widget *ccmb = _fetch_cw_arg(ClipMachineMemory);
   GtkTreeModel *model;
   C_object *cmodel;

   CHECKCWID(ccmb, GTK_IS_COMBO_BOX);

   model = gtk_combo_box_get_model(GTK_COMBO_BOX(ccmb->widget));

   if (model)
      {
	 cmodel = _list_get_cobject(ClipMachineMemory, model);
	 if (!cmodel)
	    cmodel = _register_object(ClipMachineMemory, Iter, GTK_TYPE_TREE_MODEL, NULL, NULL);
	 if (cmodel)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cmodel->obj);
      }

   return 0;
 err:
   return 1;
}

int
clip_GTK_COMBOBOXAPPENDTEXT(ClipMachine * ClipMachineMemory)
{
   C_widget *ccmb = _fetch_cw_arg(ClipMachineMemory);
   gchar *text = _clip_parc(ClipMachineMemory, 2);

   CHECKCWID(ccmb, GTK_IS_COMBO_BOX);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(text);
   gtk_combo_box_append_text(GTK_COMBO_BOX(ccmb->widget), text);
   FREE_TEXT(text);

   return 0;
 err:
   return 1;
}

int
clip_GTK_COMBOBOXPREPENDTEXT(ClipMachine * ClipMachineMemory)
{
   C_widget *ccmb = _fetch_cw_arg(ClipMachineMemory);
   gchar *text = _clip_parc(ClipMachineMemory, 2);

   CHECKCWID(ccmb, GTK_IS_COMBO_BOX);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(text);
   gtk_combo_box_prepend_text(GTK_COMBO_BOX(ccmb->widget), text);
   FREE_TEXT(text);

   return 0;
 err:
   return 1;
}

int
clip_GTK_COMBOBOXINSERTTEXT(ClipMachine * ClipMachineMemory)
{
   C_widget *ccmb = _fetch_cw_arg(ClipMachineMemory);
   gint pos = _clip_parni(ClipMachineMemory, 2);
   gchar *text = _clip_parc(ClipMachineMemory, 3);

   CHECKCWID(ccmb, GTK_IS_COMBO_BOX);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, CHARACTER_type_of_ClipVarType);

   pos--;
   LOCALE_TO_UTF(text);
   gtk_combo_box_insert_text(GTK_COMBO_BOX(ccmb->widget), pos, text);
   FREE_TEXT(text);

   return 0;
 err:
   return 1;
}

int
clip_GTK_COMBOBOXREMOVETEXT(ClipMachine * ClipMachineMemory)
{
   C_widget *ccmb = _fetch_cw_arg(ClipMachineMemory);
   gint pos = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(ccmb, GTK_IS_COMBO_BOX);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   pos--;
   gtk_combo_box_remove_text(GTK_COMBO_BOX(ccmb->widget), pos);

   return 0;
 err:
   return 1;
}

int
clip_GTK_COMBOBOXPOPUP(ClipMachine * ClipMachineMemory)
{
   C_widget *ccmb = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(ccmb, GTK_IS_COMBO_BOX);

   gtk_combo_box_popup(GTK_COMBO_BOX(ccmb->widget));

   return 0;
 err:
   return 1;
}

int
clip_GTK_COMBOBOXPOPDOWN(ClipMachine * ClipMachineMemory)
{
   C_widget *ccmb = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(ccmb, GTK_IS_COMBO_BOX);

   gtk_combo_box_popdown(GTK_COMBO_BOX(ccmb->widget));

   return 0;
 err:
   return 1;
}

/******************************************************************************/
/********************** COMBO BOX ENTRY ***************************************/

int
clip_GTK_COMBOBOXENTRYNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   GtkWidget *wid = NULL;
   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);

   wid = gtk_combo_box_entry_new();
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

int
clip_GTK_COMBOBOXENTRYNEWWITHMODEL(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   C_object *cmodel = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   gint text_column = _clip_parni(ClipMachineMemory, 3);
   GtkWidget *wid = NULL;
   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(cmodel, GTK_IS_TREE_MODEL(cmodel->object));
   CHECKARG(3, NUMERIC_type_of_ClipVarType);

   text_column--;
   wid = gtk_combo_box_entry_new_with_model(GTK_TREE_MODEL(cmodel->object), text_column);
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

int
clip_GTK_COMBOBOXENTRYNEWTEXT(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   GtkWidget *wid = NULL;
   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);

   wid = gtk_combo_box_entry_new_text();
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

int
clip_GTK_COMBOBOXENTRYSETTEXTCOLUMN(ClipMachine * ClipMachineMemory)
{
   C_widget *ccmb = _fetch_cw_arg(ClipMachineMemory);
   gint text_column = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(ccmb, GTK_IS_COMBO_BOX);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);

   text_column--;
   gtk_combo_box_entry_set_text_column(GTK_COMBO_BOX_ENTRY(ccmb->widget), text_column);

   return 0;
 err:
   return 1;
}

int
clip_GTK_COMBOBOXENTRYGETTEXTCOLUMN(ClipMachine * ClipMachineMemory)
{
   C_widget *ccmb = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(ccmb, GTK_IS_COMBO_BOX);

   _clip_retni(ClipMachineMemory, gtk_combo_box_entry_get_text_column(GTK_COMBO_BOX_ENTRY(ccmb->widget)) + 1);

   return 0;
 err:
   return 1;
}

#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 6)
int
clip_GTK_COMBOBOXGETACTIVETEXT(ClipMachine * ClipMachineMemory)
{
   C_widget *ccmb = _fetch_cw_arg(ClipMachineMemory);
   gchar *text;

   CHECKCWID(ccmb, GTK_IS_COMBO_BOX);

   text = gtk_combo_box_get_active_text(GTK_COMBO_BOX(ccmb->widget));
   LOCALE_FROM_UTF(text);
   _clip_retc(ClipMachineMemory, text);
   FREE_TEXT(text);

   return 0;
 err:
   return 1;
}

int
clip_GTK_COMBOBOXGETCOLUMNSPANCOLUMN(ClipMachine * ClipMachineMemory)
{
   C_widget *ccmb = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(ccmb, GTK_IS_COMBO_BOX);

   _clip_retni(ClipMachineMemory, gtk_combo_box_get_column_span_column(GTK_COMBO_BOX(ccmb->widget)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_COMBOBOXGETFOCUSONCLICK(ClipMachine * ClipMachineMemory)
{
   C_widget *ccmb = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(ccmb, GTK_IS_COMBO_BOX);

   _clip_retl(ClipMachineMemory, gtk_combo_box_get_focus_on_click(GTK_COMBO_BOX(ccmb->widget)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_COMBOBOXGETPOPUPACCESSIBLE(ClipMachine * ClipMachineMemory)
{
   C_widget *ccmb = _fetch_cw_arg(ClipMachineMemory);
   AtkObject *atk;
   C_object *catk;

   CHECKCWID(ccmb, GTK_IS_COMBO_BOX);

   atk = gtk_combo_box_get_popup_accessible(GTK_COMBO_BOX(ccmb->widget));

   if (atk)
      {
	 catk = _list_get_cobject(ClipMachineMemory, atk);
	 if (!catk)
	    catk = _register_object(ClipMachineMemory, atk, GTK_TYPE_OBJECT, NULL, NULL);
	 if (catk)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &catk->obj);
      }

   return 0;
 err:
   return 1;
}

int
clip_GTK_COMBOBOXGETROWSPANCOLUMN(ClipMachine * ClipMachineMemory)
{
   C_widget *ccmb = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(ccmb, GTK_IS_COMBO_BOX);

   _clip_retni(ClipMachineMemory, gtk_combo_box_get_row_span_column(GTK_COMBO_BOX(ccmb->widget)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_COMBOBOXGETWRAPWIDTH(ClipMachine * ClipMachineMemory)
{
   C_widget *ccmb = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(ccmb, GTK_IS_COMBO_BOX);

   _clip_retni(ClipMachineMemory, gtk_combo_box_get_wrap_width(GTK_COMBO_BOX(ccmb->widget)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_COMBOBOXSETADDTEAROFFS(ClipMachine * ClipMachineMemory)
{
   C_widget *ccmb = _fetch_cw_arg(ClipMachineMemory);
   gboolean set = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(ccmb, GTK_IS_COMBO_BOX);
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_combo_box_set_add_tearoffs(GTK_COMBO_BOX(ccmb->widget), set);

   return 0;
 err:
   return 1;
}

int
clip_GTK_COMBOBOXSETFOCUSONCLICK(ClipMachine * ClipMachineMemory)
{
   C_widget *ccmb = _fetch_cw_arg(ClipMachineMemory);
   gboolean set = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(ccmb, GTK_IS_COMBO_BOX);
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_combo_box_set_focus_on_click(GTK_COMBO_BOX(ccmb->widget), set);

   return 0;
 err:
   return 1;
}
#endif
