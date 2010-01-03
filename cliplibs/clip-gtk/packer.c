/*
    Copyright (C) 2002  ITK
    Author  : Alexey M. Tkachenko <alexey@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg.h"

#include <gtk/gtk.h>

#include "ci_clip-gtk.ch"
#include "ci_clip-gtk.h"

/*********************** SIGNALS **************************/

/***             Packer have no signals                 ***/

/**********************************************************/
CLIP_DLLEXPORT GtkType
_gtk_type_packer()
{
   return GTK_TYPE_PACKER;
}

long
_clip_type_packer()
{
   return GTK_WIDGET_PACKER;
}

const char *
_clip_type_name_packer()
{
   return "GTK_TYPE_PACKER";
}

/* Register packer in global table */
int
clip_INIT___PACKER(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_packer, _clip_type_name_packer, _gtk_type_packer, _gtk_type_container, NULL);
   return 0;
}

/**** ------------------ ****/
/**** PACKER constructor ****/
int
clip_GTK_PACKERNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   GtkWidget *wid = NULL;

   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   wid = gtk_packer_new();
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
clip_GTK_PACKERADDDEFAULTS(ClipMachine * ClipMachineMemory)
{
   C_widget *cpck = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   GtkSideType side = INT_OPTION(ClipMachineMemory, 3, GTK_SIDE_TOP);

   GtkAnchorType anchor = INT_OPTION(ClipMachineMemory, 4, GTK_ANCHOR_CENTER);

   GtkPackerOptions options = INT_OPTION(ClipMachineMemory, 5, GTK_PACK_EXPAND);

   CHECKCWID(cpck, GTK_IS_PACKER);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cwid, GTK_IS_WIDGET);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, NUMERIC_type_of_ClipVarType);
   CHECKOPT(5, NUMERIC_type_of_ClipVarType);
   gtk_packer_add_defaults(GTK_PACKER(cpck->widget), cwid->widget, side, anchor, options);
   return 0;
 err:
   return 1;
}

int
clip_GTK_PACKERADD(ClipMachine * ClipMachineMemory)
{
   C_widget *cpck = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   GtkSideType side = INT_OPTION(ClipMachineMemory, 3, GTK_SIDE_TOP);

   GtkAnchorType anchor = INT_OPTION(ClipMachineMemory, 4, GTK_ANCHOR_CENTER);

   GtkPackerOptions options = INT_OPTION(ClipMachineMemory, 5, GTK_PACK_EXPAND);

   guint     border_width = _clip_parni(ClipMachineMemory, 6);

   guint     pad_x = _clip_parni(ClipMachineMemory, 7);

   guint     pad_y = _clip_parni(ClipMachineMemory, 8);

   guint     i_pad_x = _clip_parni(ClipMachineMemory, 9);

   guint     i_pad_y = _clip_parni(ClipMachineMemory, 10);

   CHECKCWID(cpck, GTK_IS_PACKER);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cwid, GTK_IS_WIDGET);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, NUMERIC_type_of_ClipVarType);
   CHECKOPT(5, NUMERIC_type_of_ClipVarType);
   CHECKOPT(6, NUMERIC_type_of_ClipVarType);
   CHECKOPT(7, NUMERIC_type_of_ClipVarType);
   CHECKOPT(8, NUMERIC_type_of_ClipVarType);
   CHECKOPT(9, NUMERIC_type_of_ClipVarType);
   CHECKOPT(10, NUMERIC_type_of_ClipVarType);
   gtk_packer_add(GTK_PACKER(cpck->widget), cwid->widget, side, anchor, options, border_width, pad_x, pad_y, i_pad_x, i_pad_y);
   return 0;
 err:
   return 1;
}

int
clip_GTK_PACKERREORDERCHILD(ClipMachine * ClipMachineMemory)
{
   C_widget *cpck = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gint      position = INT_OPTION(ClipMachineMemory, 3, 1);

   CHECKCWID(cpck, GTK_IS_PACKER);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cwid, GTK_IS_WIDGET);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   gtk_packer_reorder_child(GTK_PACKER(cpck->widget), cwid->widget, position);
   return 0;
 err:
   return 1;
}

int
clip_GTK_PACKERSETSPACING(ClipMachine * ClipMachineMemory)
{
   C_widget *cpck = _fetch_cw_arg(ClipMachineMemory);

   gint      spacing = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(cpck, GTK_IS_PACKER);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_packer_set_spacing(GTK_PACKER(cpck->widget), spacing);
   return 0;
 err:
   return 1;
}

int
clip_GTK_PACKERSETDEFAULTBORDERWIDTH(ClipMachine * ClipMachineMemory)
{
   C_widget *cpck = _fetch_cw_arg(ClipMachineMemory);

   gint      border = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(cpck, GTK_IS_PACKER);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_packer_set_default_border_width(GTK_PACKER(cpck->widget), border);
   return 0;
 err:
   return 1;
}

int
clip_GTK_PACKERSETDEFAULTPAD(ClipMachine * ClipMachineMemory)
{
   C_widget *cpck = _fetch_cw_arg(ClipMachineMemory);

   gint      pad_x = _clip_parni(ClipMachineMemory, 2);

   gint      pad_y = _clip_parni(ClipMachineMemory, 3);

   CHECKCWID(cpck, GTK_IS_PACKER);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   gtk_packer_set_default_pad(GTK_PACKER(cpck->widget), pad_x, pad_y);
   return 0;
 err:
   return 1;
}

int
clip_GTK_PACKERSETDEFAULTIPAD(ClipMachine * ClipMachineMemory)
{
   C_widget *cpck = _fetch_cw_arg(ClipMachineMemory);

   gint      pad_x = _clip_parni(ClipMachineMemory, 2);

   gint      pad_y = _clip_parni(ClipMachineMemory, 3);

   CHECKCWID(cpck, GTK_IS_PACKER);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   gtk_packer_set_default_ipad(GTK_PACKER(cpck->widget), pad_x, pad_y);
   return 0;
 err:
   return 1;
}

int
clip_GTK_PACKERSETCHILDPACKING(ClipMachine * ClipMachineMemory)
{
   C_widget *cpck = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   GtkSideType side = INT_OPTION(ClipMachineMemory, 3, GTK_SIDE_TOP);

   GtkAnchorType anchor = INT_OPTION(ClipMachineMemory, 4, GTK_ANCHOR_CENTER);

   GtkPackerOptions options = INT_OPTION(ClipMachineMemory, 5, GTK_PACK_EXPAND);

   guint     border_width = _clip_parni(ClipMachineMemory, 6);

   guint     pad_x = _clip_parni(ClipMachineMemory, 7);

   guint     pad_y = _clip_parni(ClipMachineMemory, 8);

   guint     i_pad_x = _clip_parni(ClipMachineMemory, 9);

   guint     i_pad_y = _clip_parni(ClipMachineMemory, 10);

   CHECKCWID(cpck, GTK_IS_PACKER);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cwid, GTK_IS_WIDGET);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, NUMERIC_type_of_ClipVarType);
   CHECKOPT(5, NUMERIC_type_of_ClipVarType);
   CHECKOPT(6, NUMERIC_type_of_ClipVarType);
   CHECKOPT(7, NUMERIC_type_of_ClipVarType);
   CHECKOPT(8, NUMERIC_type_of_ClipVarType);
   CHECKOPT(9, NUMERIC_type_of_ClipVarType);
   CHECKOPT(10, NUMERIC_type_of_ClipVarType);
   gtk_packer_set_child_packing(GTK_PACKER(cpck->widget), cwid->widget,
				side, anchor, options, border_width, pad_x, pad_y, i_pad_x, i_pad_y);
   return 0;
 err:
   return 1;
}
