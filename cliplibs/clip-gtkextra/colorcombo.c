/*
    Copyright (C) 2002  ITK
    Author  : Alexey M. Tkachenko <alexey@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkextracfg.h"

#include <gtkextra/gtkextra.h>
#include <string.h>

#include "ci_clip-gtk.ch"
#include "ci_clip-gtk.h"

#include "ci_clip-gtkextra.ch"
#include "ci_clip-gtkextra.h"

/**********************************************************/
static gint
handle_changed_signal(GtkWidget * widget, gint selection, gchar * color_name, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_SELECTION, selection);
   _clip_mputc(cs->cw->cmachine, &cv, HASH_COLORNAME, color_name, strlen(color_name));
   INVOKESIGHANDLER(widget, cs, cv);
}

/* Signals table */
static SignalTable color_combo_signals[] = {
   {"changed", GSF(handle_changed_signal), ESF(object_emit_signal), GTK_CHANGED_SIGNAL},
   {"", NULL, NULL, 0}
};

/**********************************************************/

/* Register ColorCombo in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_color_combo()
{
   return gtk_color_combo_get_type();
}

long
_clip_type_color_combo()
{
   return GTK_WIDGET_COLOR_COMBO;
}

const char *
_clip_type_name_color_combo()
{
   return "GTK_WIDGET_COLOR_COMBO";
}

int
clip_INIT___COLOR_COMBO(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_color_combo, _clip_type_name_color_combo, _gtk_type_color_combo, _gtk_type_combo_box, color_combo_signals);
   return 0;
}

/**** COLOR COMBO constructors ****/
int
clip_GTK_COLORCOMBONEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   GtkWidget *wid = NULL;
   C_widget *cwid, *cbutton, *ctable;

   CHECKOPT(1, MAP_type_of_ClipVarType);

   wid = gtk_color_combo_new();

   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   cbutton = _register_widget(ClipMachineMemory, GTK_COMBO_BOX(wid)->button, NULL);
   ctable = _register_widget(ClipMachineMemory, GTK_COLOR_COMBO(wid)->table, NULL);
   _clip_madd(ClipMachineMemory, &cwid->obj, HASH_BUTTON, &cbutton->obj);
   _clip_madd(ClipMachineMemory, &cwid->obj, HASH_TABLE, &ctable->obj);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

int
clip_GTK_COLORCOMBONEWWITHVALUES(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   gint nrows = _clip_parni(ClipMachineMemory, 2);
   gint ncols = _clip_parni(ClipMachineMemory, 3);
   ClipVar *cvalues = _clip_spar(ClipMachineMemory, 4);
   GtkWidget *wid = NULL;
   C_widget *cwid, *cbutton, *ctable;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, ARRAY_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 4) == ARRAY_type_of_ClipVarType)
      {
	 unsigned short i;
	 ClipVar *item;
	 gchar **color_names = NULL;
	 if (cvalues->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar > 0)
	    {
	       color_names = (gchar **) calloc(sizeof(gchar *), cvalues->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar);
	       for (i = 0; i < cvalues->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar; i++)
		  {
		     item = cvalues->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i;
		     if (item->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
			{
			   color_names[i] = (gchar *) calloc(1, item->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf + 1);
			   strcpy(color_names[i], item->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
			}
		  }
	       if (color_names)
		  wid = gtk_color_combo_new_with_values(nrows, ncols, color_names);
	    }
      }

   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   cbutton = _register_widget(ClipMachineMemory, GTK_COMBO_BOX(wid)->button, NULL);
   ctable = _register_widget(ClipMachineMemory, GTK_COLOR_COMBO(wid)->table, NULL);
   _clip_madd(ClipMachineMemory, &cwid->obj, HASH_BUTTON, &cbutton->obj);
   _clip_madd(ClipMachineMemory, &cwid->obj, HASH_TABLE, &ctable->obj);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

int
clip_GTK_COLORCOMBOGETCOLORAT(ClipMachine * ClipMachineMemory)
{
   C_widget *cclc = _fetch_cw_arg(ClipMachineMemory);
   gint row = INT_OPTION(ClipMachineMemory, 2, 1);
   gint col = INT_OPTION(ClipMachineMemory, 3, 1);
   CHECKCWID(cclc, GTK_IS_COLOR_COMBO);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   _clip_retc(ClipMachineMemory, gtk_color_combo_get_color_at(GTK_COLOR_COMBO(cclc->widget), row - 1, col - 1));
   return 0;
 err:
   return 1;
}

int
clip_GTK_COLORCOMBOFINDCOLOR(ClipMachine * ClipMachineMemory)
{
   C_widget *cclc = _fetch_cw_arg(ClipMachineMemory);
   ClipVar *mcolor = _clip_spar(ClipMachineMemory, 2);
   gint row, col;
   GdkColor color;
   CHECKCWID(cclc, GTK_IS_COLOR_COMBO);
   CHECKARG(2, MAP_type_of_ClipVarType);
   _map_colors_to_gdk(ClipMachineMemory, mcolor, &color);
   gtk_color_combo_find_color(GTK_COLOR_COMBO(cclc->widget), &color, &row, &col);
   _clip_storni(ClipMachineMemory, row + 1, 3, 0);
   _clip_storni(ClipMachineMemory, col + 1, 4, 0);
   return 0;
 err:
   return 1;
}
