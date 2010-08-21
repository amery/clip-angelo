/*
    Copyright (C) 2001  ITK
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

/* Box has no signals */

/**********************************************************/
CLIP_DLLEXPORT GtkType
_gtk_type_box()
{
   return GTK_TYPE_BOX;
}

CLIP_DLLEXPORT GtkType
_gtk_type_hbox()
{
   return GTK_TYPE_HBOX;
}

CLIP_DLLEXPORT GtkType
_gtk_type_vbox()
{
   return GTK_TYPE_VBOX;
}

long
_clip_type_box()
{
   return GTK_WIDGET_BOX;
}

long
_clip_type_hbox()
{
   return GTK_WIDGET_HBOX;
}

long
_clip_type_vbox()
{
   return GTK_WIDGET_VBOX;
}

const char *
_clip_type_name_box()
{
   return "GTK_TYPE_BOX";
}

const char *
_clip_type_name_hbox()
{
   return "GTK_TYPE_HBOX";
}

const char *
_clip_type_name_vbox()
{
   return "GTK_TYPE_VBOX";
}

/* Register boxes in global table */
int
clip_INIT___BOX(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_box, _clip_type_name_box, _gtk_type_box, _gtk_type_container, NULL);
   _wtype_table_put(_clip_type_hbox, _clip_type_name_hbox, _gtk_type_hbox, _gtk_type_box, NULL);
   _wtype_table_put(_clip_type_vbox, _clip_type_name_vbox, _gtk_type_vbox, _gtk_type_box, NULL);
   return 0;
}

/**** ------------------ ****/
/**** HBOX constructor ****/
int
clip_GTK_HBOXNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   gboolean  homogeneous = _clip_parl(ClipMachineMemory, 2);

   gint      spacing = _clip_parni(ClipMachineMemory, 3);

   GtkWidget *wid = NULL;

   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   wid = gtk_hbox_new(homogeneous, spacing);
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

/**** VBOX constructor ****/
int
clip_GTK_VBOXNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   gboolean  homogeneous = _clip_parl(ClipMachineMemory, 2);

   gint      spacing = _clip_parni(ClipMachineMemory, 3);

   GtkWidget *wid = NULL;

   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   wid = gtk_vbox_new(homogeneous, spacing);
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

/* Adds child to box, packed with reference to the start of box.
 * The child is packed after any other child packed with
 * reference to the start of box. */
int
clip_GTK_BOXPACKSTART(ClipMachine * ClipMachineMemory)
{
   C_widget *cbox = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gboolean  expand = _clip_parl(ClipMachineMemory, 3);

   gboolean  fill = _clip_parl(ClipMachineMemory, 4);

   guint     padd = _clip_parni(ClipMachineMemory, 5);

   CHECKCWID(cbox, GTK_IS_BOX);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKOPT(3, LOGICAL_type_of_ClipVarType);
   CHECKOPT(4, LOGICAL_type_of_ClipVarType);
   CHECKOPT(5, NUMERIC_type_of_ClipVarType);
   gtk_box_pack_start(GTK_BOX(cbox->widget), cwid->widget, expand, fill, padd);
   return 0;
 err:
   return 1;
}

/* Adds child to box, packed with reference to the end of box.
 *The child is packed after (away from end of) any other child
 * packed with reference to the end of box. */
int
clip_GTK_BOXPACKEND(ClipMachine * ClipMachineMemory)
{
   C_widget *cbox = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gboolean  expand = _clip_parl(ClipMachineMemory, 3);

   gboolean  fill = _clip_parl(ClipMachineMemory, 4);

   guint     padd = _clip_parni(ClipMachineMemory, 5);

   CHECKCWID(cbox, GTK_IS_BOX);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKOPT(3, LOGICAL_type_of_ClipVarType);
   CHECKOPT(4, LOGICAL_type_of_ClipVarType);
   CHECKOPT(5, NUMERIC_type_of_ClipVarType);
   gtk_box_pack_end(GTK_BOX(cbox->widget), cwid->widget, expand, fill, padd);
   return 0;
 err:
   return 1;
}

/* Adds widget to box, packed with reference to the start of box.
 * The child is packed after any other child packed with reference
 * to the start of box.
 * Parameters for how to pack the child widget, expand, fill, and
 * padding in GtkBoxChild, are given their default values,
 * TRUE, TRUE, and 0, respectively. */
int
clip_GTK_BOXPACKSTARTDEFAULTS(ClipMachine * ClipMachineMemory)
{
   C_widget *cbox = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(cbox, GTK_IS_BOX);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_box_pack_start_defaults(GTK_BOX(cbox->widget), cwid->widget);
   return 0;
 err:
   return 1;
}

/* Adds widget to box, packed with reference to the end of box.
 * The child is packed after (away from end of) any other child
 * packed with reference to the end of box.
 * Parameters for how to pack the child widget, expand, fill, and
 * padding in GtkBoxChild, are given their default values,
 * TRUE, TRUE, and 0, respectively. */
int
clip_GTK_BOXPACKENDDEFAULTS(ClipMachine * ClipMachineMemory)
{
   C_widget *cbox = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(cbox, GTK_IS_BOX);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_box_pack_end_defaults(GTK_BOX(cbox->widget), cwid->widget);
   return 0;
 err:
   return 1;
}

/* Sets the homogeneous field of GtkBox, controlling whether or not all
 * children of box are given equal space in the box. */
int
clip_GTK_BOXSETHOMOGENEOUS(ClipMachine * ClipMachineMemory)
{
   C_widget *cbox = _fetch_cw_arg(ClipMachineMemory);

   gboolean  homogeneous = _clip_parl(ClipMachineMemory, 2);

   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   CHECKCWID(cbox, GTK_IS_BOX);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      homogeneous = TRUE;
   gtk_box_set_homogeneous(GTK_BOX(cbox->widget), homogeneous);
   return 0;
 err:
   return 1;
}

/* Sets the spacing field of GtkBox, which is the number of pixels
 * to place between children of box. */
int
clip_GTK_BOXSETSPACING(ClipMachine * ClipMachineMemory)
{
   C_widget *cbox = _fetch_cw_arg(ClipMachineMemory);

   gint      spacing = _clip_parni(ClipMachineMemory, 2);

   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cbox, GTK_IS_BOX);
   gtk_box_set_spacing(GTK_BOX(cbox->widget), spacing);
   return 0;
 err:
   return 1;
}

/* Moves child to a new position in the list of box children.
 * The list is the children field of GtkBox, and contains both
 * widgets packed GTK_PACK_START as well as widgets packed
 * GTK_PACK_END, in the order that these widgets were added to box.
 *
 * A widget's position in the box children list determines where the
 * widget is packed into box. A child widget at some position in the
 * list will be packed just after all other widgets of the same packing
 *  type that appear earlier in the list. A negative value of position
 * is interpreted as position 0. */
int
clip_GTK_BOXREORDERCHILD(ClipMachine * ClipMachineMemory)
{
   C_widget *cbox = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gint      position = _clip_parni(ClipMachineMemory, 3);

   CHECKCWID(cbox, GTK_IS_BOX);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   gtk_box_reorder_child(GTK_BOX(cbox->widget), cwid->widget, position);
   return 0;
 err:
   return 1;
}

/* Returns information about how child is packed into box. */
int
clip_GTK_BOXQUERYCHILDPACKING(ClipMachine * ClipMachineMemory)
{
   C_widget *cbox = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gboolean  expand, fill;

   guint     padding;

   GtkPackType pack_type;

   CHECKCWID(cbox, GTK_IS_BOX);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_box_query_child_packing(GTK_BOX(cbox->widget), cwid->widget, &expand, &fill, &padding, &pack_type);
   _clip_storl(ClipMachineMemory, expand, 3, 0);
   _clip_storl(ClipMachineMemory, fill, 4, 0);
   _clip_storni(ClipMachineMemory, padding, 5, 0);
   _clip_storni(ClipMachineMemory, pack_type, 6, 0);
   return 0;
 err:
   return 1;
}
