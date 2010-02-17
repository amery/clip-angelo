/*
    Copyright (C) 2001-2004  ITK
    Author  : Elena V. Kornilova <alena@itk.ru>
              Alexey M. Tkachenko <alexey@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg2.h"

#include <gtk/gtk.h>

#include "ci_clip-gtk2.ch"
#include "ci_clip-gtk2.h"

/*********************** SIGNALS **************************/

/* Layout has no signals */

/**********************************************************/

/* Register layout signals table in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_layout()
{
   return GTK_TYPE_LAYOUT;
}

long
_clip_type_layout()
{
   return GTK_WIDGET_LAYOUT;
}

const char *
_clip_type_name_layout()
{
   return "GTK_WIDGET_LAYOUT";
}

int
clip_INIT___LAYOUT(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_layout, _clip_type_name_layout, _gtk_type_layout, _gtk_type_container, NULL);
   return 0;
}

int
clip_GTK_LAYOUTNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   C_widget *chadj = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   C_widget *cvadj = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));
   GtkWidget *wid = NULL;
   C_widget *cwid;
   GtkAdjustment *hadj, *vadj;
   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(chadj, GTK_IS_ADJUSTMENT);
   CHECKOPT2(3, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cvadj, GTK_IS_ADJUSTMENT);

   hadj = chadj ? GTK_ADJUSTMENT(chadj->widget) : NULL;
   vadj = cvadj ? GTK_ADJUSTMENT(cvadj->widget) : NULL;
   wid = gtk_layout_new(hadj, vadj);
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

int
clip_GTK_LAYOUTPUT(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   gint x = _clip_parni(ClipMachineMemory, 3);
   gint y = _clip_parni(ClipMachineMemory, 4);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ccon, GTK_IS_LAYOUT);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      x = cwid->widget->allocation.x;
   if (_clip_parinfo(ClipMachineMemory, 4) == UNDEF_type_of_ClipVarType)
      y = cwid->widget->allocation.y;
   gtk_layout_put(GTK_LAYOUT(ccon->widget), cwid->widget, x, y);

   return 0;
 err:
   return 1;
}

int
clip_GTK_LAYOUTMOVE(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   gint x = _clip_parni(ClipMachineMemory, 3);
   gint y = _clip_parni(ClipMachineMemory, 4);

   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ccon, GTK_IS_LAYOUT);
   CHECKCWID(cwid, GTK_IS_WIDGET);

   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      x = cwid->widget->allocation.x;
   if (_clip_parinfo(ClipMachineMemory, 4) == UNDEF_type_of_ClipVarType)
      y = cwid->widget->allocation.y;
   gtk_layout_move(GTK_LAYOUT(ccon->widget), cwid->widget, x, y);

   return 0;
 err:
   return 1;
}

/* These disable and enable moving and repainting the scrolling window
 * of the GtkLayout, respectively.  If you want to update the layout's
 * offsets but do not want it to repaint itself, you should use these
 * functions. */
int
clip_GTK_LAYOUTFREEZE(ClipMachine * ClipMachineMemory)
{
   C_widget *clay = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(clay, GTK_IS_LAYOUT);
   gtk_layout_freeze(GTK_LAYOUT(clay->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_LAYOUTTHAW(ClipMachine * ClipMachineMemory)
{
   C_widget *clay = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(clay, GTK_IS_LAYOUT);
   gtk_layout_thaw(GTK_LAYOUT(clay->widget));
   return 0;
 err:
   return 1;
}

/* Alena: set vertical adjustment for layout */
int
clip_GTK_LAYOUTSETVADJUSTMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *clay = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cadj = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   GtkAdjustment *adj;
   CHECKCWID(clay, GTK_IS_LAYOUT);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cadj, GTK_IS_ADJUSTMENT);

   adj = cadj ? GTK_ADJUSTMENT(cadj->widget) : NULL;
   gtk_layout_set_vadjustment(GTK_LAYOUT(clay->widget), adj);

   return 0;
 err:
   return 1;
}

int
clip_GTK_LAYOUTGETVADJUSTMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *clay = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cadj;
   GtkAdjustment *adj;
   CHECKCWID(clay, GTK_IS_LAYOUT);

   adj = gtk_layout_get_vadjustment(GTK_LAYOUT(clay->widget));
   if (adj)
      {
	 cadj = _list_get_cwidget(ClipMachineMemory, adj);
	 if (!cadj)
	    cadj = _register_widget(ClipMachineMemory, GTK_WIDGET(adj), NULL);
	 if (cadj)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cadj->obj);
      }

   return 0;
 err:
   return 1;
}

/* Alena: set horizontal adjustment for layout */
int
clip_GTK_LAYOUTSETHADJUSTMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *clay = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cadj = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   GtkAdjustment *adj;
   CHECKCWID(clay, GTK_IS_LAYOUT);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cadj, GTK_IS_ADJUSTMENT);

   adj = cadj ? GTK_ADJUSTMENT(cadj->widget) : NULL;
   gtk_layout_set_hadjustment(GTK_LAYOUT(clay->widget), adj);

   return 0;
 err:
   return 1;
}

int
clip_GTK_LAYOUTGETHADJUSTMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *clay = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cadj;
   GtkAdjustment *adj;
   CHECKCWID(clay, GTK_IS_LAYOUT);

   adj = gtk_layout_get_hadjustment(GTK_LAYOUT(clay->widget));
   if (adj)
      {
	 cadj = _list_get_cwidget(ClipMachineMemory, adj);
	 if (!cadj)
	    cadj = _register_widget(ClipMachineMemory, GTK_WIDGET(adj), NULL);
	 if (cadj)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cadj->obj);
      }

   return 0;
 err:
   return 1;
}

int
clip_GTK_LAYOUTSETSIZE(ClipMachine * ClipMachineMemory)
{
   C_widget *clay = _fetch_cw_arg(ClipMachineMemory);
   guint width = _clip_parni(ClipMachineMemory, 2);
   guint height = _clip_parni(ClipMachineMemory, 3);
   CHECKCWID(clay, GTK_IS_LAYOUT);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);

   gtk_layout_set_size(GTK_LAYOUT(clay->widget), width, height);

   return 0;
 err:
   return 1;
}

int
clip_GTK_LAYOUTGETSIZE(ClipMachine * ClipMachineMemory)
{
   C_widget *clay = _fetch_cw_arg(ClipMachineMemory);
   guint width;
   guint height;
   CHECKCWID(clay, GTK_IS_LAYOUT);

   gtk_layout_get_size(GTK_LAYOUT(clay->widget), &width, &height);
   _clip_storni(ClipMachineMemory, width, 2, 0);
   _clip_storni(ClipMachineMemory, height, 3, 0);
   return 0;
 err:
   return 1;
}
