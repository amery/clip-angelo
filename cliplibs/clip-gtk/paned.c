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

/****           Paned has no own signals               ****/

/**********************************************************/

/* Register Paned in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_paned()
{
   return GTK_TYPE_PANED;
}

CLIP_DLLEXPORT GtkType
_gtk_type_hpaned()
{
   return gtk_hpaned_get_type();
}

CLIP_DLLEXPORT GtkType
_gtk_type_vpaned()
{
   return gtk_vpaned_get_type();
}

long
_clip_type_paned()
{
   return GTK_WIDGET_PANED;
}

long
_clip_type_hpaned()
{
   return GTK_WIDGET_HPANED;
}

long
_clip_type_vpaned()
{
   return GTK_WIDGET_VPANED;
}

const char *
_clip_type_name_paned()
{
   return "GTK_WIDGET_PANED";
}

const char *
_clip_type_name_hpaned()
{
   return "GTK_WIDGET_HPANED";
}

const char *
_clip_type_name_vpaned()
{
   return "GTK_WIDGET_VPANED";
}

int
clip_INIT___PANED(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_paned, _clip_type_name_paned, _gtk_type_paned, _gtk_type_container, NULL);
   _wtype_table_put(_clip_type_hpaned, _clip_type_name_hpaned, _gtk_type_hpaned, _gtk_type_paned, NULL);
   _wtype_table_put(_clip_type_vpaned, _clip_type_name_vpaned, _gtk_type_vpaned, _gtk_type_paned, NULL);
   return 0;
}

int
clip_GTK_HPANEDNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   GtkWidget *wid = NULL;

   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);

   wid = gtk_hpaned_new();
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

int
clip_GTK_VPANEDNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   GtkWidget *wid = NULL;

   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);

   wid = gtk_vpaned_new();
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

/* Add a child to the top or left pane with default parameters.
 * This is equivalent to gtk_paned_pack1(paned, FALSE, TRUE); */
int
clip_GTK_PANEDADD1(ClipMachine * ClipMachineMemory)
{
   C_widget *cpan = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(cpan, GTK_IS_PANED);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_paned_add1(GTK_PANED(cpan->widget), cwid->widget);
   return 0;
 err:
   return 1;
}

/* Add a child to the bottom or right pane with default parameters.
 * This is equivalent to gtk_paned_pack1(paned, FALSE, FALSE); */
int
clip_GTK_PANEDADD2(ClipMachine * ClipMachineMemory)
{
   C_widget *cpan = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(cpan, GTK_IS_PANED);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_paned_add2(GTK_PANED(cpan->widget), cwid->widget);
   return 0;
 err:
   return 1;
}

/* Add a child to the top or left pane. */
/* paned : a paned widget
   child : the child to add
  resize : should this child expand when the paned widget is resized.
  shrink : can this child be made smaller than its requsition.  */
int
clip_GTK_PANEDPACK1(ClipMachine * ClipMachineMemory)
{
   C_widget *cpan = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gboolean  resize = _clip_parl(ClipMachineMemory, 3);

   gboolean  shrink = _clip_parl(ClipMachineMemory, 4);

   CHECKCWID(cpan, GTK_IS_PANED);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKOPT(3, LOGICAL_type_of_ClipVarType);
   CHECKOPT(4, LOGICAL_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      resize = TRUE;
   if (_clip_parinfo(ClipMachineMemory, 4) == UNDEF_type_of_ClipVarType)
      shrink = TRUE;
   gtk_paned_pack1(GTK_PANED(cpan->widget), cwid->widget, resize, shrink);
   return 0;
 err:
   return 1;
}

/* Add a child to the bottom or right pane. */
int
clip_GTK_PANEDPACK2(ClipMachine * ClipMachineMemory)
{
   C_widget *cpan = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gboolean  resize = _clip_parl(ClipMachineMemory, 3);

   gboolean  shrink = _clip_parl(ClipMachineMemory, 4);

   CHECKCWID(cpan, GTK_IS_PANED);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKOPT(3, LOGICAL_type_of_ClipVarType);
   CHECKOPT(4, LOGICAL_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      resize = TRUE;
   if (_clip_parinfo(ClipMachineMemory, 4) == UNDEF_type_of_ClipVarType)
      shrink = TRUE;
   gtk_paned_pack2(GTK_PANED(cpan->widget), cwid->widget, resize, shrink);
   return 0;
 err:
   return 1;
}

/* Set the width of the gutter. (The area between the two panes). */
int
clip_GTK_PANEDSETGUTTERSIZE(ClipMachine * ClipMachineMemory)
{
   C_widget *cpan = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cpan, GTK_IS_PANED);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   gtk_paned_set_gutter_size(GTK_PANED(cpan->widget), _clip_parni(ClipMachineMemory, 2));
   return 0;
 err:
   return 1;
}

#ifndef OS_CYGWIN
int
clip_GTK_PANEDGETGUTTERSIZE(ClipMachine * ClipMachineMemory)
{
   C_widget *cpan = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cpan, GTK_IS_PANED);
   _clip_retni(ClipMachineMemory, GTK_PANED(cpan->widget)->gutter_size);
   return 0;
 err:
   return 1;
}
#endif

/* Set the the handle size to size x size pixels. */
int
clip_GTK_PANEDSETHANDLESIZE(ClipMachine * ClipMachineMemory)
{
   C_widget *cpan = _fetch_cw_arg(ClipMachineMemory);

   guint16   size = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(cpan, GTK_IS_PANED);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   gtk_paned_set_handle_size(GTK_PANED(cpan->widget), size);
   return 0;
 err:
   return 1;
}

int
clip_GTK_PANEDGETHANDLESIZE(ClipMachine * ClipMachineMemory)
{
   C_widget *cpan = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cpan, GTK_IS_PANED);
   _clip_retni(ClipMachineMemory, GTK_PANED(cpan->widget)->handle_size);
   return 0;
 err:
   return 1;
}

/* Set the position of the separator, as if set by the user. If position is
 * negative, the remembered position is forgotten, and the division is
 * recomputed from the the requisitions of the children. */
int
clip_GTK_PANEDSETPOSITION(ClipMachine * ClipMachineMemory)
{
   C_widget *cpan = _fetch_cw_arg(ClipMachineMemory);

   gint      position = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(cpan, GTK_IS_PANED);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      position = -1;
   gtk_paned_set_position(GTK_PANED(cpan->widget), position);
   return 0;
 err:
   return 1;
}

/* Alena */
/* Get size child1 (top or left)*/
int
clip_GTK_PANEDGETSIZECHILD1(ClipMachine * ClipMachineMemory)
{
   C_widget *cpan = _fetch_cw_arg(ClipMachineMemory);

   gint      width = 0, height = 0;

   GtkPaned *paned;

   GtkRequisition child_requisition;

   CHECKCWID(cpan, GTK_IS_PANED);
   paned = GTK_PANED(cpan->widget);

   if (paned->child1 && GTK_WIDGET_VISIBLE(paned->child1))
    {
       gtk_widget_size_request(paned->child1, &child_requisition);

       height = child_requisition.height;
       width = child_requisition.width;
    }
   _clip_storni(ClipMachineMemory, width, 2, 0);
   _clip_storni(ClipMachineMemory, height, 3, 0);
   return 0;
 err:
   return 1;
}

/* Get size child2 (bottom or right)*/
int
clip_GTK_PANEDGETSIZECHILD2(ClipMachine * ClipMachineMemory)
{
   C_widget *cpan = _fetch_cw_arg(ClipMachineMemory);

   gint      width = 0, height = 0;

   GtkPaned *paned;

   GtkRequisition child_requisition;

   CHECKCWID(cpan, GTK_IS_PANED);
   paned = GTK_PANED(cpan->widget);

   if (paned->child2 && GTK_WIDGET_VISIBLE(paned->child2))
    {
       gtk_widget_size_request(paned->child2, &child_requisition);

       height = child_requisition.height;
       width = child_requisition.width;
    }
   _clip_storni(ClipMachineMemory, width, 2, 0);
   _clip_storni(ClipMachineMemory, height, 3, 0);
   return 0;
 err:
   return 1;
}
