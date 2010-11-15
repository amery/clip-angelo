/*
    Copyright (C) 2002  ITK
    Author  : Alexey M. Tkachenko <alexey@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg.h"

#include <gtk/gtk.h>
#include <string.h>

#include "ci_clip-gtk.ch"
#include "ci_clip-gtk.h"

/*********************** SIGNALS **************************/

/* Signal handlers */
gint
handle_orientation_changed_signal(GtkWidget * widget, GtkOrientation orientation, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_ORIENTATION, orientation);
   INVOKESIGHANDLER(widget, cs, cv);
}

gint
handle_style_changed_signal(GtkWidget * widget, GtkToolbarStyle style, C_signal * cs)
{
   PREPARECV(cs, cv);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_STYLE, style);
   INVOKESIGHANDLER(widget, cs, cv);
}

/* Signals table */
static SignalTable toolbar_signals[] = {
   {"orientation-changed", GSF(handle_orientation_changed_signal),
    ESF(object_emit_signal), GTK_ORIENTATION_CHANGED_SIGNAL},
   {"style-changed", GSF(handle_style_changed_signal),
    ESF(object_emit_signal), GTK_STYLE_CHANGED_SIGNAL},
   {"", NULL, NULL, 0}
};

/**********************************************************/
CLIP_DLLEXPORT GtkType
_gtk_type_toolbar()
{
   return GTK_TYPE_TOOLBAR;
}

long
_clip_type_toolbar()
{
   return GTK_WIDGET_TOOLBAR;
}

const char *
_clip_type_name_toolbar()
{
   return "GTK_TYPE_TOOLBAR";
}

/* Register toolbar in global table */
int
clip_INIT___TOOLBAR(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_toolbar, _clip_type_name_toolbar, _gtk_type_toolbar, _gtk_type_container, toolbar_signals);
   return 0;
}

/**** ------------------ ****/
/**** TOOLBAR constructor ****/
int
clip_GTK_TOOLBARNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   GtkOrientation orientation = INT_OPTION(ClipMachineMemory, 2, GTK_ORIENTATION_HORIZONTAL);
   GtkToolbarStyle style = INT_OPTION(ClipMachineMemory, 3, GTK_TOOLBAR_ICONS);
   GtkWidget *wid = NULL;
   C_widget *cwid;
   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   wid = gtk_toolbar_new(orientation, style);
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

static void
_toolbar_child_destroy(ClipMachine * ClipMachineMemory, C_widget * cw)
{
   if (cw && cw->data)
      {
	 free(cw->data);
      }
}

static void
_toolbar_child_callback(GtkWidget * widget, gpointer data)
{
   C_var *c = (C_var *) data;
   ClipVar stack[1];
   ClipVar res;
   if (&(c->cfunc))
      {
	 memset(&stack, 0, sizeof(stack));
	 memset(&res, 0, sizeof(ClipVar));
	 _clip_mclone(c->cw->cmachine, &stack[0], &c->cw->obj);
	//stack[0]  = c->cw->obj;
	 _clip_eval(c->cw->cmachine, &(c->cfunc), 1, stack, &res);
	 _clip_destroy(c->cw->cmachine, &res);
      }
}

/* adds a new button to the beginning (left or top edges) of the given toolbar. */
int
clip_GTK_TOOLBARAPPENDITEM(ClipMachine * ClipMachineMemory)
{
   C_widget *ctlb = _fetch_cw_arg(ClipMachineMemory);
   char *text = CHAR_OPTION(ClipMachineMemory, 2, NULL);
   char *tooltip_text = CHAR_OPTION(ClipMachineMemory, 3, NULL);
   char *tooltip_private_text = CHAR_OPTION(ClipMachineMemory, 4, NULL);
   C_widget *cicon = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 5));
   ClipVar *cfunc = _clip_spar(ClipMachineMemory, 6);
   C_var *c = NEW(C_var);
   GtkWidget *child;
   CHECKCWID(ctlb, GTK_IS_TOOLBAR);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT(3, CHARACTER_type_of_ClipVarType);
   CHECKOPT(4, CHARACTER_type_of_ClipVarType);
   CHECKOPT2(5, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cicon, GTK_IS_WIDGET);
   CHECKOPT2(6, PCODE_type_of_ClipVarType, CCODE_type_of_ClipVarType);
   c->ClipMachineMemory = ClipMachineMemory;
   c->cw = ctlb;
   _clip_mclone(ClipMachineMemory, &c->cfunc, cfunc);

   if (text)
      LOCALE_TO_UTF(text);
   if (tooltip_text)
      LOCALE_TO_UTF(tooltip_text);
   if (tooltip_private_text)
      LOCALE_TO_UTF(tooltip_private_text);
   child = gtk_toolbar_append_item(GTK_TOOLBAR(ctlb->widget), text, tooltip_text, tooltip_private_text, cicon->widget, (GtkSignalFunc) _toolbar_child_callback, c);
   if (text)
      FREE_TEXT(text);
   if (tooltip_text)
      FREE_TEXT(tooltip_text);
   if (tooltip_private_text)
      FREE_TEXT(tooltip_private_text);

   if (child)
      {
	 C_widget *cchild = _register_widget(ClipMachineMemory, child, NULL);
	 cchild->destroy = _toolbar_child_destroy;
	 cchild->data = c;
	 _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cchild->obj);
      }
   return 0;
 err:
   return 1;
}

/* adds a new button to the beginning of the given toolbar. */
int
clip_GTK_TOOLBARPREPENDITEM(ClipMachine * ClipMachineMemory)
{
   C_widget *ctlb = _fetch_cw_arg(ClipMachineMemory);
   char *text = CHAR_OPTION(ClipMachineMemory, 2, NULL);
   char *tooltip_text = CHAR_OPTION(ClipMachineMemory, 3, NULL);
   char *tooltip_private_text = CHAR_OPTION(ClipMachineMemory, 4, NULL);
   C_widget *cicon = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 5));
   ClipVar *cfunc = _clip_spar(ClipMachineMemory, 6);
   C_var *c = NEW(C_var);
   GtkWidget *child;
   CHECKCWID(ctlb, GTK_IS_TOOLBAR);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT(3, CHARACTER_type_of_ClipVarType);
   CHECKOPT(4, CHARACTER_type_of_ClipVarType);
   CHECKOPT2(5, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cicon, GTK_IS_WIDGET);
   CHECKOPT2(6, PCODE_type_of_ClipVarType, CCODE_type_of_ClipVarType);
   c->ClipMachineMemory = ClipMachineMemory;
   c->cw = ctlb;
   _clip_mclone(ClipMachineMemory, &c->cfunc, cfunc);

   if (text)
      LOCALE_TO_UTF(text);
   if (tooltip_text)
      LOCALE_TO_UTF(tooltip_text);
   if (tooltip_private_text)
      LOCALE_TO_UTF(tooltip_private_text);
   child = gtk_toolbar_prepend_item(GTK_TOOLBAR(ctlb->widget), text, tooltip_text, tooltip_private_text, cicon->widget, (GtkSignalFunc) _toolbar_child_callback, c);
   if (text)
      FREE_TEXT(text);
   if (tooltip_text)
      FREE_TEXT(tooltip_text);
   if (tooltip_private_text)
      FREE_TEXT(tooltip_private_text);

   if (child)
      {
	 C_widget *cchild = _register_widget(ClipMachineMemory, child, NULL);
	 cchild->destroy = _toolbar_child_destroy;
	 cchild->data = c;
	 _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cchild->obj);
      }
   return 0;
 err:
   return 1;
}

/* inserts a new item into the toolbar. You may specify the
 * position in the toolbar where it will be inserted. */
int
clip_GTK_TOOLBARINSERTITEM(ClipMachine * ClipMachineMemory)
{
   C_widget *ctlb = _fetch_cw_arg(ClipMachineMemory);
   char *text = CHAR_OPTION(ClipMachineMemory, 2, NULL);
   char *tooltip_text = CHAR_OPTION(ClipMachineMemory, 3, NULL);
   char *tooltip_private_text = CHAR_OPTION(ClipMachineMemory, 4, NULL);
   C_widget *cicon = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 5));
   ClipVar *cfunc = _clip_spar(ClipMachineMemory, 6);
   gint position = _clip_parni(ClipMachineMemory, 7);
   C_var *c = NEW(C_var);
   GtkWidget *child;
   CHECKCWID(ctlb, GTK_IS_TOOLBAR);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT(3, CHARACTER_type_of_ClipVarType);
   CHECKOPT(4, CHARACTER_type_of_ClipVarType);
   CHECKOPT2(5, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cicon, GTK_IS_WIDGET);
   CHECKOPT2(6, PCODE_type_of_ClipVarType, CCODE_type_of_ClipVarType);
   CHECKOPT(7, NUMERIC_type_of_ClipVarType);
   c->ClipMachineMemory = ClipMachineMemory;
   c->cw = ctlb;
   _clip_mclone(ClipMachineMemory, &c->cfunc, cfunc);

   if (text)
      LOCALE_TO_UTF(text);
   if (tooltip_text)
      LOCALE_TO_UTF(tooltip_text);
   if (tooltip_private_text)
      LOCALE_TO_UTF(tooltip_private_text);
   child = gtk_toolbar_insert_item(GTK_TOOLBAR(ctlb->widget), text, tooltip_text, tooltip_private_text, cicon->widget, (GtkSignalFunc) _toolbar_child_callback, c, position);
   if (text)
      FREE_TEXT(text);
   if (tooltip_text)
      FREE_TEXT(tooltip_text);
   if (tooltip_private_text)
      FREE_TEXT(tooltip_private_text);

   if (child)
      {
	 C_widget *cchild = _register_widget(ClipMachineMemory, child, NULL);
	 cchild->destroy = _toolbar_child_destroy;
	 cchild->data = c;
	 _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cchild->obj);
      }
   return 0;
 err:
   return 1;
}

/* adds a new space to the end of the toolbar. */
int
clip_GTK_TOOLBARAPPENDSPACE(ClipMachine * ClipMachineMemory)
{
   C_widget *ctlb = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(ctlb, GTK_IS_TOOLBAR);
   gtk_toolbar_append_space(GTK_TOOLBAR(ctlb->widget));
   return 0;
 err:
   return 1;
}

/* adds a new space to the beginning of the toolbar. */
int
clip_GTK_TOOLBARPREPENDSPACE(ClipMachine * ClipMachineMemory)
{
   C_widget *ctlb = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(ctlb, GTK_IS_TOOLBAR);
   gtk_toolbar_prepend_space(GTK_TOOLBAR(ctlb->widget));
   return 0;
 err:
   return 1;
}

/* inserts a new space in the toolbar at the specified position. */
int
clip_GTK_TOOLBARINSERTSPACE(ClipMachine * ClipMachineMemory)
{
   C_widget *ctlb = _fetch_cw_arg(ClipMachineMemory);
   gint position = _clip_parni(ClipMachineMemory, 2);
   CHECKCWID(ctlb, GTK_IS_TOOLBAR);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_toolbar_insert_space(GTK_TOOLBAR(ctlb->widget), position);
   return 0;
 err:
   return 1;
}

/* adds a new element to the end of a toolbar. */
int
clip_GTK_TOOLBARAPPENDELEMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *ctlb = _fetch_cw_arg(ClipMachineMemory);
   GtkToolbarChildType type = INT_OPTION(ClipMachineMemory, 2, GTK_TOOLBAR_CHILD_SPACE);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));
   char *text = CHAR_OPTION(ClipMachineMemory, 4, NULL);
   char *tooltip_text = CHAR_OPTION(ClipMachineMemory, 5, NULL);
   char *tooltip_private_text = CHAR_OPTION(ClipMachineMemory, 6, NULL);
   C_widget *cicon = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 7));
   ClipVar *cfunc = _clip_spar(ClipMachineMemory, 8);
   C_var *c = NEW(C_var);
   GtkWidget *child;
   GtkWidget *icon = cicon ? cicon->widget : NULL;
   CHECKCWID(ctlb, GTK_IS_TOOLBAR);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT2(3, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cwid, GTK_IS_WIDGET);
   CHECKOPT(4, CHARACTER_type_of_ClipVarType);
   CHECKOPT(5, CHARACTER_type_of_ClipVarType);
   CHECKOPT(6, CHARACTER_type_of_ClipVarType);
   CHECKOPT2(7, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cicon, GTK_IS_WIDGET);
   CHECKOPT2(8, PCODE_type_of_ClipVarType, CCODE_type_of_ClipVarType);
   c->ClipMachineMemory = ClipMachineMemory;
   c->cw = ctlb;
   _clip_mclone(ClipMachineMemory, &c->cfunc, cfunc);

   if (text)
      LOCALE_TO_UTF(text);
   if (tooltip_text)
      LOCALE_TO_UTF(tooltip_text);
   if (tooltip_private_text)
      LOCALE_TO_UTF(tooltip_private_text);
   child = gtk_toolbar_append_element(GTK_TOOLBAR(ctlb->widget), type, cwid->widget, text, tooltip_text, tooltip_private_text, icon, (GtkSignalFunc) _toolbar_child_callback, c);
   if (text)
      FREE_TEXT(text);
   if (tooltip_text)
      FREE_TEXT(tooltip_text);
   if (tooltip_private_text)
      FREE_TEXT(tooltip_private_text);

   if (child)
      {
	 C_widget *cchild = _register_widget(ClipMachineMemory, child, NULL);
	 cchild->destroy = _toolbar_child_destroy;
	 cchild->data = c;
	 _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cchild->obj);
      }
   return 0;
 err:
   return 1;
}

/* adds a new element to the beginning of a toolbar. */
int
clip_GTK_TOOLBARPREPENDELEMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *ctlb = _fetch_cw_arg(ClipMachineMemory);
   GtkToolbarChildType type = INT_OPTION(ClipMachineMemory, 2, GTK_TOOLBAR_CHILD_SPACE);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));
   char *text = CHAR_OPTION(ClipMachineMemory, 4, NULL);
   char *tooltip_text = CHAR_OPTION(ClipMachineMemory, 5, NULL);
   char *tooltip_private_text = CHAR_OPTION(ClipMachineMemory, 6, NULL);
   C_widget *cicon = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 7));
   ClipVar *cfunc = _clip_spar(ClipMachineMemory, 8);
   C_var *c = NEW(C_var);
   GtkWidget *child;
   CHECKCWID(ctlb, GTK_IS_TOOLBAR);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT2(3, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cwid, GTK_IS_WIDGET);
   CHECKOPT(4, CHARACTER_type_of_ClipVarType);
   CHECKOPT(5, CHARACTER_type_of_ClipVarType);
   CHECKOPT(6, CHARACTER_type_of_ClipVarType);
   CHECKOPT2(7, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cicon, GTK_IS_WIDGET);
   CHECKOPT2(8, PCODE_type_of_ClipVarType, CCODE_type_of_ClipVarType);
   c->ClipMachineMemory = ClipMachineMemory;
   c->cw = ctlb;
   _clip_mclone(ClipMachineMemory, &c->cfunc, cfunc);

   if (text)
      LOCALE_TO_UTF(text);
   if (tooltip_text)
      LOCALE_TO_UTF(tooltip_text);
   if (tooltip_private_text)
      LOCALE_TO_UTF(tooltip_private_text);
   child = gtk_toolbar_prepend_element(GTK_TOOLBAR(ctlb->widget), type, cwid->widget, text, tooltip_text, tooltip_private_text, cicon->widget, (GtkSignalFunc) _toolbar_child_callback, c);
   if (text)
      FREE_TEXT(text);
   if (tooltip_text)
      FREE_TEXT(tooltip_text);
   if (tooltip_private_text)
      FREE_TEXT(tooltip_private_text);

   if (child)
      {
	 C_widget *cchild = _register_widget(ClipMachineMemory, child, NULL);
	 cchild->destroy = _toolbar_child_destroy;
	 cchild->data = c;
	 _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cchild->obj);
      }
   return 0;
 err:
   return 1;
}

int
clip_GTK_TOOLBARINSERTELEMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *ctlb = _fetch_cw_arg(ClipMachineMemory);
   GtkToolbarChildType type = INT_OPTION(ClipMachineMemory, 2, GTK_TOOLBAR_CHILD_SPACE);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));
   char *text = CHAR_OPTION(ClipMachineMemory, 4, NULL);
   char *tooltip_text = CHAR_OPTION(ClipMachineMemory, 5, NULL);
   char *tooltip_private_text = CHAR_OPTION(ClipMachineMemory, 6, NULL);
   C_widget *cicon = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 7));
   ClipVar *cfunc = _clip_spar(ClipMachineMemory, 8);
   gint position = _clip_parni(ClipMachineMemory, 9);
   C_var *c = NEW(C_var);
   GtkWidget *child;
   CHECKCWID(ctlb, GTK_IS_TOOLBAR);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT2(3, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cwid, GTK_IS_WIDGET);
   CHECKOPT(4, CHARACTER_type_of_ClipVarType);
   CHECKOPT(5, CHARACTER_type_of_ClipVarType);
   CHECKOPT(6, CHARACTER_type_of_ClipVarType);
   CHECKOPT2(7, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cicon, GTK_IS_WIDGET);
   CHECKOPT2(8, PCODE_type_of_ClipVarType, CCODE_type_of_ClipVarType);
   CHECKOPT(9, NUMERIC_type_of_ClipVarType);
   c->ClipMachineMemory = ClipMachineMemory;
   c->cw = ctlb;
   _clip_mclone(ClipMachineMemory, &c->cfunc, cfunc);

   if (text)
      LOCALE_TO_UTF(text);
   if (tooltip_text)
      LOCALE_TO_UTF(tooltip_text);
   if (tooltip_private_text)
      LOCALE_TO_UTF(tooltip_private_text);
   child = gtk_toolbar_insert_element(GTK_TOOLBAR(ctlb->widget), type, cwid->widget, text, tooltip_text, tooltip_private_text, cicon->widget, (GtkSignalFunc) _toolbar_child_callback, c, position);
   if (text)
      FREE_TEXT(text);
   if (tooltip_text)
      FREE_TEXT(tooltip_text);
   if (tooltip_private_text)
      FREE_TEXT(tooltip_private_text);

   if (child)
      {
	 C_widget *cchild = _register_widget(ClipMachineMemory, child, NULL);
	 cchild->destroy = _toolbar_child_destroy;
	 cchild->data = c;
	 _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cchild->obj);
      }
   return 0;
 err:
   return 1;
}

int
clip_GTK_TOOLBARAPPENDWIDGET(ClipMachine * ClipMachineMemory)
{
   C_widget *ctlb = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   char *tooltip_text = CHAR_OPTION(ClipMachineMemory, 3, NULL);
   char *tooltip_private_text = CHAR_OPTION(ClipMachineMemory, 4, NULL);
   CHECKCWID(ctlb, GTK_IS_TOOLBAR);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cwid, GTK_IS_WIDGET);
   CHECKOPT(3, CHARACTER_type_of_ClipVarType);
   CHECKOPT(4, CHARACTER_type_of_ClipVarType);

   if (tooltip_text)
      LOCALE_TO_UTF(tooltip_text);
   if (tooltip_private_text)
      LOCALE_TO_UTF(tooltip_private_text);
   gtk_toolbar_append_widget(GTK_TOOLBAR(ctlb->widget), cwid->widget, tooltip_text, tooltip_private_text);
   if (tooltip_text)
      FREE_TEXT(tooltip_text);
   if (tooltip_private_text)
      FREE_TEXT(tooltip_private_text);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TOOLBARPREPENDWIDGET(ClipMachine * ClipMachineMemory)
{
   C_widget *ctlb = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   char *tooltip_text = CHAR_OPTION(ClipMachineMemory, 3, NULL);
   char *tooltip_private_text = CHAR_OPTION(ClipMachineMemory, 4, NULL);
   CHECKCWID(ctlb, GTK_IS_TOOLBAR);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cwid, GTK_IS_WIDGET);
   CHECKOPT(3, CHARACTER_type_of_ClipVarType);
   CHECKOPT(4, CHARACTER_type_of_ClipVarType);

   if (tooltip_text)
      LOCALE_TO_UTF(tooltip_text);
   if (tooltip_private_text)
      LOCALE_TO_UTF(tooltip_private_text);
   gtk_toolbar_prepend_widget(GTK_TOOLBAR(ctlb->widget), cwid->widget, tooltip_text, tooltip_private_text);
   if (tooltip_text)
      FREE_TEXT(tooltip_text);
   if (tooltip_private_text)
      FREE_TEXT(tooltip_private_text);

   return 0;
 err:
   return 1;
}

int
clip_GTK_TOOLBARINSERTWIDGET(ClipMachine * ClipMachineMemory)
{
   C_widget *ctlb = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   char *tooltip_text = CHAR_OPTION(ClipMachineMemory, 3, NULL);
   char *tooltip_private_text = CHAR_OPTION(ClipMachineMemory, 4, NULL);
   gint position = _clip_parni(ClipMachineMemory, 5);
   CHECKCWID(ctlb, GTK_IS_TOOLBAR);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cwid, GTK_IS_WIDGET);
   CHECKOPT(3, CHARACTER_type_of_ClipVarType);
   CHECKOPT(4, CHARACTER_type_of_ClipVarType);
   CHECKOPT(5, NUMERIC_type_of_ClipVarType);

   if (tooltip_text)
      LOCALE_TO_UTF(tooltip_text);
   if (tooltip_private_text)
      LOCALE_TO_UTF(tooltip_private_text);
   gtk_toolbar_insert_widget(GTK_TOOLBAR(ctlb->widget), cwid->widget, tooltip_text, tooltip_private_text, position);
   if (tooltip_text)
      FREE_TEXT(tooltip_text);
   if (tooltip_private_text)
      FREE_TEXT(tooltip_private_text);

   return 0;
 err:
   return 1;
}

/* sets whether a toolbar should appear horizontally or vertically. */
int
clip_GTK_TOOLBARSETORIENTATION(ClipMachine * ClipMachineMemory)
{
   C_widget *ctlb = _fetch_cw_arg(ClipMachineMemory);
   GtkOrientation orientation = _clip_parni(ClipMachineMemory, 2);
   CHECKCWID(ctlb, GTK_IS_TOOLBAR);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_toolbar_set_orientation(GTK_TOOLBAR(ctlb->widget), orientation);
   return 0;
 err:
   return 1;
}

/* alters the view of toolbar to display either icons only, text only, or both. */
int
clip_GTK_TOOLBARSETSTYLE(ClipMachine * ClipMachineMemory)
{
   C_widget *ctlb = _fetch_cw_arg(ClipMachineMemory);
   GtkToolbarStyle style = _clip_parni(ClipMachineMemory, 2);
   CHECKCWID(ctlb, GTK_IS_TOOLBAR);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_toolbar_set_style(GTK_TOOLBAR(ctlb->widget), style);
   return 0;
 err:
   return 1;
}

/* should be used to set the number of pixels each item of space takes up in a toolbar. */
int
clip_GTK_TOOLBARSETSPACESIZE(ClipMachine * ClipMachineMemory)
{
   C_widget *ctlb = _fetch_cw_arg(ClipMachineMemory);
   gint space_size = _clip_parni(ClipMachineMemory, 2);
   CHECKCWID(ctlb, GTK_IS_TOOLBAR);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_toolbar_set_space_size(GTK_TOOLBAR(ctlb->widget), space_size);
   return 0;
 err:
   return 1;
}

/* Sets whether a 'space' in the toolbar appears as a line or just plain empty space. */
int
clip_GTK_TOOLBARSETSPACESTYLE(ClipMachine * ClipMachineMemory)
{
   C_widget *ctlb = _fetch_cw_arg(ClipMachineMemory);
   GtkToolbarSpaceStyle space_style = _clip_parni(ClipMachineMemory, 2);
   CHECKCWID(ctlb, GTK_IS_TOOLBAR);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_toolbar_set_space_style(GTK_TOOLBAR(ctlb->widget), space_style);
   return 0;
 err:
   return 1;
}

/* sets if the tooltips of a toolbar should be active or not. */
int
clip_GTK_TOOLBARSETTOOLTIPS(ClipMachine * ClipMachineMemory)
{
   C_widget *ctlb = _fetch_cw_arg(ClipMachineMemory);
   gint enable = BOOL_OPTION(ClipMachineMemory, 2, TRUE);
   CHECKCWID(ctlb, GTK_IS_TOOLBAR);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   gtk_toolbar_set_tooltips(GTK_TOOLBAR(ctlb->widget), enable);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TOOLBARSETBUTTONRELIEF(ClipMachine * ClipMachineMemory)
{
   C_widget *ctlb = _fetch_cw_arg(ClipMachineMemory);
   GtkReliefStyle relief = _clip_parni(ClipMachineMemory, 2);
   CHECKCWID(ctlb, GTK_IS_TOOLBAR);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_toolbar_set_button_relief(GTK_TOOLBAR(ctlb->widget), relief);
   return 0;
 err:
   return 1;
}

int
clip_GTK_TOOLBARGETBUTTONRELIEF(ClipMachine * ClipMachineMemory)
{
   C_widget *ctlb = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(ctlb, GTK_IS_TOOLBAR);
   _clip_retni(ClipMachineMemory, gtk_toolbar_get_button_relief(GTK_TOOLBAR(ctlb->widget)));
   return 0;
 err:
   return 1;
}
