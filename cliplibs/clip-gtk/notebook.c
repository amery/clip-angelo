/*
    Copyright (C) 2001  ITK
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
gint
handle_switch_page_signal(GtkNotebook * notebook, GtkNotebookPage * page, gint page_num, C_signal * cs)
{
   ClipVar   cv, mpage, mreq, mallc;

   int       ret;

   C_widget *cwid;

   memset(&cv, 0, sizeof(ClipVar));
   _clip_map(cs->cw->cmachine, &cv);
   memset(&mpage, 0, sizeof(ClipVar));
   _clip_map(cs->cw->cmachine, &mpage);
   memset(&mreq, 0, sizeof(ClipVar));
   _clip_map(cs->cw->cmachine, &mreq);
   memset(&mallc, 0, sizeof(ClipVar));
   _clip_map(cs->cw->cmachine, &mallc);
   cwid = _list_get_cwidget(cs->cw->cmachine, page->child);
   if (!cwid)
      cwid = _register_widget(cs->cw->cmachine, page->child, NULL);
   if (cwid)
      _clip_madd(cs->cw->cmachine, &mpage, HASH_CHILD, &cwid->obj);
   cwid = _list_get_cwidget(cs->cw->cmachine, page->tab_label);
   if (!cwid)
      cwid = _register_widget(cs->cw->cmachine, page->tab_label, NULL);
   if (cwid)
      _clip_madd(cs->cw->cmachine, &mpage, HASH_TABLABEL, &cwid->obj);
   cwid = _list_get_cwidget(cs->cw->cmachine, page->menu_label);
   if (!cwid)
      cwid = _register_widget(cs->cw->cmachine, page->menu_label, NULL);
   if (cwid)
      _clip_madd(cs->cw->cmachine, &mpage, HASH_MENULABEL, &cwid->obj);
   _clip_mputn(cs->cw->cmachine, &mreq, HASH_WIDTH, page->requisition.width);
   _clip_mputn(cs->cw->cmachine, &mreq, HASH_HEIGHT, page->requisition.height);
   _clip_madd(cs->cw->cmachine, &mpage, HASH_REQUISITION, &mreq);
   _clip_mputn(cs->cw->cmachine, &mallc, HASH_X, page->allocation.x);
   _clip_mputn(cs->cw->cmachine, &mallc, HASH_Y, page->allocation.y);
   _clip_mputn(cs->cw->cmachine, &mallc, HASH_WIDTH, page->allocation.width);
   _clip_mputn(cs->cw->cmachine, &mallc, HASH_HEIGHT, page->allocation.height);
   _clip_madd(cs->cw->cmachine, &mpage, HASH_ALLOCATION, &mallc);
   _clip_mputn(cs->cw->cmachine, &mpage, HASH_DEFAULTMENU, page->default_menu);
   _clip_mputn(cs->cw->cmachine, &mpage, HASH_DEFAULTTAB, page->default_tab);
   _clip_mputn(cs->cw->cmachine, &mpage, HASH_EXPAND, page->expand);
   _clip_mputn(cs->cw->cmachine, &mpage, HASH_PACK, page->pack);
   _clip_mputn(cs->cw->cmachine, &mpage, HASH_FILL, page->fill);
   _clip_madd(cs->cw->cmachine, &cv, HASH_PAGE, &mpage);
   _clip_mputn(cs->cw->cmachine, &cv, HASH_PAGENUM, page_num + 1);
   ret = handle_signals(GTK_WIDGET(notebook), cs, &cv);
   _clip_destroy(cs->cw->cmachine, &cv);
   _clip_destroy(cs->cw->cmachine, &mpage);
   _clip_destroy(cs->cw->cmachine, &mreq);
   _clip_destroy(cs->cw->cmachine, &mallc);
   return ret;
}

static SignalTable notebook_signals[] = {
   {"switch-page", GSF(handle_switch_page_signal), ESF(object_emit_signal),
    GTK_SWITCH_PAGE_SIGNAL},
   {"", NULL, NULL, 0}
};

/* Register notebook in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_notebook()
{
   return GTK_TYPE_NOTEBOOK;
}

long
_clip_type_notebook()
{
   return GTK_WIDGET_NOTEBOOK;
}

const char *
_clip_type_name_notebook()
{
   return "GTK_WIDGET_NOTEBOOK";
}

int
clip_INIT___NOTEBOOK(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_notebook, _clip_type_name_notebook, _gtk_type_notebook, _gtk_type_container, notebook_signals);
   return 0;
}

/**********************************************************/

/**** Notebook constructor ****/
int
clip_GTK_NOTEBOOKNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   GtkWidget *wid = NULL;

   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);

   wid = gtk_notebook_new();
   if (!wid)
      goto err;

   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

/* Appends to notebook a page whose content is child, and whose
 * bookmark is label. */
int
clip_GTK_NOTEBOOKAPPENDPAGE(ClipMachine * ClipMachineMemory)
{
   C_widget *cntb = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cchild = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_widget *clabel;

   GtkWidget *label;

   char     *pchar = _clip_parc(ClipMachineMemory, 4);

   guint     accel_key = 0;

   CHECKCWID(cntb, GTK_IS_NOTEBOOK);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cchild, GTK_IS_WIDGET);
   CHECKARG3(3, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   CHECKOPT(4, CHARACTER_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 3) == CHARACTER_type_of_ClipVarType)
    {
       gchar    *text;

       char     *caption = _clip_parc(ClipMachineMemory, 3);

       LOCALE_TO_UTF(caption);
       label = gtk_label_new(caption);
       gtk_label_get(GTK_LABEL(label), &text);
       if (pchar)
	{
	   char     *pc;

	   for (pc = text; pc && *pc; pc++)
	      if (*pc == *pchar)
		 *pc = '_';
	}

       if (pchar)
	  accel_key = gtk_label_parse_uline(GTK_LABEL(label), text);
       FREE_TEXT(caption);

    }
   else
    {
       gchar    *text;

       clabel = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));
       CHECKCWID(clabel, GTK_IS_WIDGET);

       if (clabel)
	  label = clabel->widget;
       gtk_label_get(GTK_LABEL(label), &text);
       if (pchar)
	{
	   char     *pc;

	   for (pc = text; pc && *pc; pc++)
	      if (*pc == *pchar)
		 *pc = '_';
	}

       if (pchar)
	  accel_key = gtk_label_parse_uline(GTK_LABEL(label), text);

    }
   gtk_notebook_append_page(GTK_NOTEBOOK(cntb->widget), cchild->widget, label);

//      cwid = _register_widget(ClipMachineMemory, wid, cv);
//      _clip_mclone(ClipMachineMemory,RETPTR(ClipMachineMemory),&cwid->obj);
   _clip_mputn(ClipMachineMemory, &cntb->obj, HASH_ACCELKEY, accel_key);
   return 0;
 err:
   return 1;
}

/* Appends to notebook a page whose content is child, whose bookmark
 * is tab_label, and whose menu label is menu_label. */
int
clip_GTK_NOTEBOOKAPPENDPAGEMENU(ClipMachine * ClipMachineMemory)
{
   C_widget *cntb = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cchild = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_widget *ctab_label;

   GtkWidget *tab_label;

   C_widget *cmenu_label;

   GtkWidget *menu_label;

   CHECKCWID(cntb, GTK_IS_NOTEBOOK);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cchild, GTK_IS_WIDGET);
   CHECKARG3(3, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   CHECKARG3(4, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 3) == CHARACTER_type_of_ClipVarType)
    {
       char     *caption = _clip_parc(ClipMachineMemory, 3);

       LOCALE_TO_UTF(caption);
       tab_label = gtk_label_new(caption);
       FREE_TEXT(caption);
    }
   else
    {
       ctab_label = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));
       CHECKCWID(ctab_label, GTK_IS_WIDGET);
       if (ctab_label)
	  tab_label = ctab_label->widget;
    }
   if (_clip_parinfo(ClipMachineMemory, 4) == CHARACTER_type_of_ClipVarType)
    {
       char     *menu_text = _clip_parc(ClipMachineMemory, 4);

       LOCALE_TO_UTF(menu_text);
       menu_label = gtk_label_new(menu_text);
       FREE_TEXT(menu_text);
    }
   else
    {
       cmenu_label = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 4));
       CHECKCWID(cmenu_label, GTK_IS_WIDGET);
       if (cmenu_label)
	  menu_label = cmenu_label->widget;
    }
   gtk_notebook_append_page_menu(GTK_NOTEBOOK(cntb->widget), cchild->widget, tab_label, menu_label);
   return 0;
 err:
   return 1;
}

/* Prepends to notebook a page whose content is child, and whose
 * bookmark is label. */
int
clip_GTK_NOTEBOOKPREPENDPAGE(ClipMachine * ClipMachineMemory)
{
   C_widget *cntb = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cchild = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_widget *clabel;

   GtkWidget *label;

   CHECKCWID(cntb, GTK_IS_NOTEBOOK);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cchild, GTK_IS_WIDGET);
   CHECKARG3(3, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 3) == CHARACTER_type_of_ClipVarType)
    {
       char     *caption = _clip_parc(ClipMachineMemory, 3);

       LOCALE_TO_UTF(caption);
       label = gtk_label_new(caption);
       FREE_TEXT(caption);
    }
   else
    {
       clabel = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));
       CHECKCWID(clabel, GTK_IS_WIDGET);
       if (clabel)
	  label = clabel->widget;
    }
   gtk_notebook_prepend_page(GTK_NOTEBOOK(cntb->widget), cchild->widget, label);
   return 0;
 err:
   return 1;
}

/* Prepends to notebook a page whose content is child, whose bookmark
 * is tab_label, and whose menu label is menu_label. */
int
clip_GTK_NOTEBOOKPREPENDPAGEMENU(ClipMachine * ClipMachineMemory)
{
   C_widget *cntb = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cchild = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_widget *ctab_label;

   GtkWidget *tab_label;

   C_widget *cmenu_label;

   GtkWidget *menu_label;

   CHECKCWID(cntb, GTK_IS_NOTEBOOK);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cchild, GTK_IS_WIDGET);
   CHECKARG3(3, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   CHECKARG3(4, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 3) == CHARACTER_type_of_ClipVarType)
    {
       char     *caption = _clip_parc(ClipMachineMemory, 3);

       LOCALE_TO_UTF(caption);
       tab_label = gtk_label_new(caption);
       FREE_TEXT(caption);
    }
   else
    {
       ctab_label = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));
       CHECKCWID(ctab_label, GTK_IS_WIDGET);
       if (ctab_label)
	  tab_label = ctab_label->widget;
    }
   if (_clip_parinfo(ClipMachineMemory, 4) == CHARACTER_type_of_ClipVarType)
    {
       char     *menu_text = _clip_parc(ClipMachineMemory, 4);

       LOCALE_TO_UTF(menu_text);
       menu_label = gtk_label_new(menu_text);
       FREE_TEXT(menu_text);
    }
   else
    {
       cmenu_label = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 4));
       CHECKCWID(cmenu_label, GTK_IS_WIDGET);
       if (cmenu_label)
	  menu_label = cmenu_label->widget;
    }
   gtk_notebook_prepend_page_menu(GTK_NOTEBOOK(cntb->widget), cchild->widget, tab_label, menu_label);
   return 0;
 err:
   return 1;
}

/* Inserts in notebook a new page whose content is child, and whose bookmark
 * is tab_label. The page is inserted just before the page number position,
 * starting with 0. If position is out of bounds, it is assumed to be the
 * current number of pages.  */
int
clip_GTK_NOTEBOOKINSERTPAGE(ClipMachine * ClipMachineMemory)
{
   C_widget *cntb = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cchild = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_widget *clabel;

   GtkWidget *label;

   gint      position = _clip_parni(ClipMachineMemory, 4);

   CHECKCWID(cntb, GTK_IS_NOTEBOOK);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cchild, GTK_IS_WIDGET);
   CHECKARG3(3, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   CHECKOPT(4, NUMERIC_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 3) == CHARACTER_type_of_ClipVarType)
    {
       char     *caption = _clip_parc(ClipMachineMemory, 3);

       LOCALE_TO_UTF(caption);
       label = gtk_label_new(caption);
       FREE_TEXT(caption);
    }
   else
    {
       clabel = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));
       CHECKCWID(clabel, GTK_IS_WIDGET);
       if (clabel)
	  label = clabel->widget;
    }
   if (_clip_parinfo(ClipMachineMemory, 4) == UNDEF_type_of_ClipVarType)
      position = 1;
   gtk_notebook_insert_page(GTK_NOTEBOOK(cntb->widget), cchild->widget, label, position - 1);
   return 0;
 err:
   return 1;
}

/* Inserts in notebook a new page whose content is child, whose bookmark is
 * tab_label, and whose menu label is menu_label. The page is inserted just
 * before the page number position, starting with 0. If position is out of
 * bounds, it is assumed to be the current number of pages. */
int
clip_GTK_NOTEBOOKINSERTPAGEMENU(ClipMachine * ClipMachineMemory)
{
   C_widget *cntb = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cchild = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_widget *ctab_label;

   GtkWidget *tab_label;

   C_widget *cmenu_label;

   GtkWidget *menu_label;

   gint      position = _clip_parni(ClipMachineMemory, 5);

   CHECKCWID(cntb, GTK_IS_NOTEBOOK);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cchild, GTK_IS_WIDGET);
   CHECKARG3(3, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   CHECKARG3(4, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   CHECKOPT(5, NUMERIC_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 3) == CHARACTER_type_of_ClipVarType)
    {
       char     *caption = _clip_parc(ClipMachineMemory, 3);

       LOCALE_TO_UTF(caption);
       tab_label = gtk_label_new(caption);
       FREE_TEXT(caption);
    }
   else
    {
       ctab_label = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));
       CHECKCWID(ctab_label, GTK_IS_WIDGET);
       if (ctab_label)
	  tab_label = ctab_label->widget;
    }
   if (_clip_parinfo(ClipMachineMemory, 4) == CHARACTER_type_of_ClipVarType)
    {
       char     *menu_text = _clip_parc(ClipMachineMemory, 4);

       LOCALE_TO_UTF(menu_text);
       menu_label = gtk_label_new(menu_text);
       FREE_TEXT(menu_text);
    }
   else
    {
       cmenu_label = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 4));
       CHECKCWID(cmenu_label, GTK_IS_WIDGET);
       if (cmenu_label)
	  menu_label = cmenu_label->widget;
    }
   if (_clip_parinfo(ClipMachineMemory, 5) == UNDEF_type_of_ClipVarType)
      position = 1;
   gtk_notebook_insert_page_menu(GTK_NOTEBOOK(cntb->widget), cchild->widget, tab_label, menu_label, position - 1);
   return 0;
 err:
   return 1;
}

/* Removes the page page_num form notebook. Pages are numbered starting at 1.
 * Negative values stand for the last page; too large values are ignored. */
int
clip_GTK_NOTEBOOKREMOVETPAGE(ClipMachine * ClipMachineMemory)
{
   C_widget *cntb = _fetch_cw_arg(ClipMachineMemory);

   gint      page_num = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(cntb, GTK_IS_NOTEBOOK);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      page_num = 1;
   gtk_notebook_remove_page(GTK_NOTEBOOK(cntb->widget), page_num - 1);
   return 0;
 err:
   return 1;
}

/* Returns the page number of child in notebook. */
int
clip_GTK_NOTEBOOKPAGENUM(ClipMachine * ClipMachineMemory)
{
   C_widget *cntb = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cchild = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(cntb, GTK_IS_NOTEBOOK);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cchild, GTK_IS_WIDGET);
   _clip_retni(ClipMachineMemory, gtk_notebook_page_num(GTK_NOTEBOOK(cntb->widget), cchild->widget) + 1);
   return 0;
 err:
   return 1;
}

/* Switches to the page number page_num. Negative values stand for the
 * last page; too large values are ignored. */
int
clip_GTK_NOTEBOOKSETPAGE(ClipMachine * ClipMachineMemory)
{
   C_widget *cntb = _fetch_cw_arg(ClipMachineMemory);

   gint      page_num = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(cntb, GTK_IS_NOTEBOOK);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      page_num = 1;
   gtk_notebook_set_page(GTK_NOTEBOOK(cntb->widget), (page_num > 0 ? page_num - 1 : page_num));
   return 0;
 err:
   return 1;
}

/* Switches to the next page.
 * Nothing happens if the current page is the last page. */
int
clip_GTK_NOTEBOOKNEXTPAGE(ClipMachine * ClipMachineMemory)
{
   C_widget *cntb = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cntb, GTK_IS_NOTEBOOK);
   gtk_notebook_next_page(GTK_NOTEBOOK(cntb->widget));
   return 0;
 err:
   return 1;
}

/* Switches to the previous page.
 * Nothing happens if the current page is the first page. */
int
clip_GTK_NOTEBOOKPREVPAGE(ClipMachine * ClipMachineMemory)
{
   C_widget *cntb = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cntb, GTK_IS_NOTEBOOK);
   gtk_notebook_prev_page(GTK_NOTEBOOK(cntb->widget));
   return 0;
 err:
   return 1;
}

/* Moves the page child, so that it appears in position position.
 * Out of bounds position will be clamped. */
int
clip_GTK_NOTEBOOKREORDERCHILD(ClipMachine * ClipMachineMemory)
{
   C_widget *cntb = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cchild = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gint      position = _clip_parni(ClipMachineMemory, 3);

   CHECKCWID(cntb, GTK_IS_NOTEBOOK);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cchild, GTK_IS_WIDGET);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      position = 1;
   gtk_notebook_reorder_child(GTK_NOTEBOOK(cntb->widget), cchild->widget, position);
   return 0;
 err:
   return 1;
}

/* Sets the position of the bookmarks. */
int
clip_GTK_NOTEBOOKSETTABPOS(ClipMachine * ClipMachineMemory)
{
   C_widget *cntb = _fetch_cw_arg(ClipMachineMemory);

   GtkPositionType pos = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(cntb, GTK_IS_NOTEBOOK);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_notebook_set_tab_pos(GTK_NOTEBOOK(cntb->widget), pos);
   return 0;
 err:
   return 1;
}

/* Sets whether to show the bookmarks or not. */
int
clip_GTK_NOTEBOOKSETSHOWTABS(ClipMachine * ClipMachineMemory)
{
   C_widget *cntb = _fetch_cw_arg(ClipMachineMemory);

   gboolean  show_tabs = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(cntb, GTK_IS_NOTEBOOK);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      show_tabs = TRUE;
   gtk_notebook_set_show_tabs(GTK_NOTEBOOK(cntb->widget), show_tabs);
   return 0;
 err:
   return 1;
}

/* Sets whether to show the border of the notebook or not.
 * Bookmarks are in the border. */
int
clip_GTK_NOTEBOOKSETSHOWBORDER(ClipMachine * ClipMachineMemory)
{
   C_widget *cntb = _fetch_cw_arg(ClipMachineMemory);

   gboolean  show_border = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(cntb, GTK_IS_NOTEBOOK);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      show_border = TRUE;
   gtk_notebook_set_show_border(GTK_NOTEBOOK(cntb->widget), show_border);
   return 0;
 err:
   return 1;
}

/* Sets whether the bookmarks area may be scrollable or not if there are
 * too many bookmarks to fit in the allocated area. */
int
clip_GTK_NOTEBOOKSETSCROLLABLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cntb = _fetch_cw_arg(ClipMachineMemory);

   gboolean  scrollable = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(cntb, GTK_IS_NOTEBOOK);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      scrollable = TRUE;
   gtk_notebook_set_scrollable(GTK_NOTEBOOK(cntb->widget), scrollable);
   return 0;
 err:
   return 1;
}

/* Sets whether there should be a border around the bookmarks or not. */
int
clip_GTK_NOTEBOOKSETTABBORDER(ClipMachine * ClipMachineMemory)
{
   C_widget *cntb = _fetch_cw_arg(ClipMachineMemory);

   guint     border_width = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(cntb, GTK_IS_NOTEBOOK);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_notebook_set_tab_border(GTK_NOTEBOOK(cntb->widget), border_width);
   return 0;
 err:
   return 1;
}

/* Enables the popup menu: if the user clicks with the right mouse button
 * on the bookmarks, a menu with all the pages will be popped up. */
int
clip_GTK_NOTEBOOKPOPUPENABLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cntb = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cntb, GTK_IS_NOTEBOOK);
   gtk_notebook_popup_enable(GTK_NOTEBOOK(cntb->widget));
   return 0;
 err:
   return 1;
}

/* Disables the popup menu */
int
clip_GTK_NOTEBOOKPOPUPDISABLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cntb = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cntb, GTK_IS_NOTEBOOK);
   gtk_notebook_popup_disable(GTK_NOTEBOOK(cntb->widget));
   return 0;
 err:
   return 1;
}

/* Returns the page number of the current page. */
int
clip_GTK_NOTEBOOKGETCURRENTPAGE(ClipMachine * ClipMachineMemory)
{
   C_widget *cntb = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cntb, GTK_IS_NOTEBOOK);
   _clip_retni(ClipMachineMemory, gtk_notebook_get_current_page(GTK_NOTEBOOK(cntb->widget)) + 1);
   return 0;
 err:
   return 1;
}

/* Returns the menu label of the page child. NULL is returned if child is
 * not in notebook or NULL if it has the default menu label. */
int
clip_GTK_NOTEBOOKGETMENULABEL(ClipMachine * ClipMachineMemory)
{
   C_widget *cntb = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cchild = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_widget *cwid;

   GtkWidget *wid;

   CHECKCWID(cntb, GTK_IS_NOTEBOOK);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cchild, GTK_IS_WIDGET);
   wid = gtk_notebook_get_menu_label(GTK_NOTEBOOK(cntb->widget), cchild->widget);
   cwid = _list_get_cwidget(ClipMachineMemory, wid);
   if (!cwid)
      cwid = _register_widget(ClipMachineMemory, wid, NULL);
   if (cwid)
      _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

/* Returns the content of the page number page_num, or
 * NULL if page_num is out of bounds. */
int
clip_GTK_NOTEBOOKGETNTHPAGE(ClipMachine * ClipMachineMemory)
{
   C_widget *cntb = _fetch_cw_arg(ClipMachineMemory);

   gint      page_num = _clip_parni(ClipMachineMemory, 2);

   C_widget *cwid;

   GtkWidget *wid;

   CHECKCWID(cntb, GTK_IS_NOTEBOOK);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      page_num = 1;
   wid = gtk_notebook_get_nth_page(GTK_NOTEBOOK(cntb->widget), page_num - 1);
   cwid = _list_get_cwidget(ClipMachineMemory, wid);
   if (!cwid)
      cwid = _register_widget(ClipMachineMemory, wid, NULL);
   if (cwid)
      _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

/* Returns the menu tab of the page child. NULL is returned if child is
 * not in notebook or NULL if it has the default tab label. */
int
clip_GTK_NOTEBOOKGETTABLABEL(ClipMachine * ClipMachineMemory)
{
   C_widget *cntb = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cchild = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_widget *cwid;

   GtkWidget *wid;

   CHECKCWID(cntb, GTK_IS_NOTEBOOK);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cchild, GTK_IS_WIDGET);
   wid = gtk_notebook_get_tab_label(GTK_NOTEBOOK(cntb->widget), cchild->widget);
   cwid = _list_get_cwidget(ClipMachineMemory, wid);
   if (!cwid)
      cwid = _register_widget(ClipMachineMemory, wid, NULL);
   if (cwid)
      _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

/* Looks for the packing attributes of the bookmarks of child. */
int
clip_GTK_NOTEBOOKQUERYTABLABELPACKING(ClipMachine * ClipMachineMemory)
{
   C_widget *cntb = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cchild = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gboolean  expand, fill;

   GtkPackType pack_type;

   CHECKCWID(cntb, GTK_IS_NOTEBOOK);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cchild, GTK_IS_WIDGET);
   gtk_notebook_query_tab_label_packing(GTK_NOTEBOOK(cntb->widget), cchild->widget, &expand, &fill, &pack_type);
   _clip_storni(ClipMachineMemory, expand, 3, 0);
   _clip_storni(ClipMachineMemory, fill, 4, 0);
   _clip_storni(ClipMachineMemory, pack_type, 5, 0);
   return 0;
 err:
   return 1;
}

/* Sets whether the tabs must have all the same size or not. */
int
clip_GTK_NOTEBOOKSETHOMOGENEOUSTABS(ClipMachine * ClipMachineMemory)
{
   C_widget *cntb = _fetch_cw_arg(ClipMachineMemory);

   guint     homogeneous = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(cntb, GTK_IS_NOTEBOOK);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      homogeneous = TRUE;
   gtk_notebook_set_homogeneous_tabs(GTK_NOTEBOOK(cntb->widget), homogeneous);
   return 0;
 err:
   return 1;
}

/* Changes the menu label of child. Nothing happens if child is not in notebook. */
int
clip_GTK_NOTEBOOKSETMENULABEL(ClipMachine * ClipMachineMemory)
{
   C_widget *cntb = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cchild = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_widget *clabel;

   CHECKCWID(cntb, GTK_IS_NOTEBOOK);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cchild, GTK_IS_WIDGET);
   CHECKARG3(3, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 3) == CHARACTER_type_of_ClipVarType)
    {
       char     *menu_text = _clip_parc(ClipMachineMemory, 3);

       LOCALE_TO_UTF(menu_text);
       gtk_notebook_set_menu_label_text(GTK_NOTEBOOK(cntb->widget), cchild->widget, menu_text);
       FREE_TEXT(menu_text);
    }
   else
    {
       clabel = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));
       CHECKCWID(clabel, GTK_IS_WIDGET);
       if (clabel)
	  gtk_notebook_set_menu_label(GTK_NOTEBOOK(cntb->widget), cchild->widget, clabel->widget);

    }
   return 0;
 err:
   return 1;
}

/* Sets whether the tabs should have a horizontal border. */
int
clip_GTK_NOTEBOOKSETTABHBORDER(ClipMachine * ClipMachineMemory)
{
   C_widget *cntb = _fetch_cw_arg(ClipMachineMemory);

   gboolean  tab_hborder = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(cntb, GTK_IS_NOTEBOOK);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      tab_hborder = TRUE;
   gtk_notebook_set_tab_hborder(GTK_NOTEBOOK(cntb->widget), tab_hborder);
   return 0;
 err:
   return 1;
}

/* Changes the bookmark label of child. Nothing happens if child is not in notebook. */
int
clip_GTK_NOTEBOOKSETTABLABEL(ClipMachine * ClipMachineMemory)
{
   C_widget *cntb = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cchild = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   C_widget *clabel;

   CHECKCWID(cntb, GTK_IS_NOTEBOOK);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cchild, GTK_IS_WIDGET);
   CHECKARG3(3, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 3) == CHARACTER_type_of_ClipVarType)
    {
       char     *caption = _clip_parc(ClipMachineMemory, 3);

       LOCALE_TO_UTF(caption);
       gtk_notebook_set_tab_label_text(GTK_NOTEBOOK(cntb->widget), cchild->widget, caption);
       FREE_TEXT(caption);
    }
   else
    {
       clabel = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));
       CHECKCWID(clabel, GTK_IS_WIDGET);
       if (clabel)
	  gtk_notebook_set_tab_label(GTK_NOTEBOOK(cntb->widget), cchild->widget, clabel->widget);

    }
   return 0;
 err:
   return 1;
}

/* Sets the packing parameters for the bookmark of child.
 * See GtkBoxPackStart for the exact meanings. */
int
clip_GTK_NOTEBOOKSETTABLABELPACKING(ClipMachine * ClipMachineMemory)
{
   C_widget *cntb = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cchild = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gboolean  expand = _clip_parl(ClipMachineMemory, 3);

   gboolean  fill = _clip_parl(ClipMachineMemory, 4);

   guint     packing_type = _clip_parni(ClipMachineMemory, 5);

   CHECKCWID(cntb, GTK_IS_NOTEBOOK);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cchild, GTK_IS_WIDGET);
   CHECKOPT(3, LOGICAL_type_of_ClipVarType);
   CHECKOPT(4, LOGICAL_type_of_ClipVarType);
   CHECKOPT(5, NUMERIC_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      expand = TRUE;
   if (_clip_parinfo(ClipMachineMemory, 4) == UNDEF_type_of_ClipVarType)
      fill = TRUE;
   gtk_notebook_set_tab_label_packing(GTK_NOTEBOOK(cntb->widget), cchild->widget, expand, fill, packing_type);
   return 0;
 err:
   return 1;
}

/* Sets whether the tabs should have a vertical border. */
int
clip_GTK_NOTEBOOKSETTABVBORDER(ClipMachine * ClipMachineMemory)
{
   C_widget *cntb = _fetch_cw_arg(ClipMachineMemory);

   gboolean  tab_vborder = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(cntb, GTK_IS_NOTEBOOK);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      tab_vborder = TRUE;
   gtk_notebook_set_tab_vborder(GTK_NOTEBOOK(cntb->widget), tab_vborder);
   return 0;
 err:
   return 1;
}
