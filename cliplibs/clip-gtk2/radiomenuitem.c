/*
    Copyright (C) 2001-2004  ITK
    Author  : Alexey M. Tkachenko <alexey@itk.ru>
              Elena V. Kornilova <alena@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg2.h"

#include <gtk/gtk.h>

#include "ci_clip-gtk2.ch"
#include "ci_clip-gtk2.h"

/**********************************************************/
/****          Radio menu item has no signals          ****/
/**********************************************************/

/* Register radio menu item in global table */
CLIP_DLLEXPORT GtkType _gtk_type_radio_menu_item() { return GTK_TYPE_RADIO_MENU_ITEM; }
long _clip_type_radio_menu_item() { return GTK_WIDGET_RADIO_MENU_ITEM; }
const char * _clip_type_name_radio_menu_item() { return "GTK_WIDGET_RADIO_MENU_ITEM"; }

CLIP_DLLEXPORT GtkType _gtk_type_radio_menu_group() { return GTK_WIDGET_RADIO_MENU_GROUP; }
long _clip_type_radio_menu_group() { return GTK_WIDGET_RADIO_MENU_GROUP; }
const char * _clip_type_name_radio_menu_group() { return "GTK_WIDGET_RADIO_MENU_GROUP"; }

int
clip_INIT___RADIO_MENU_ITEM(ClipMachine *cm)
{
	_wtype_table_put(_clip_type_radio_menu_item, _clip_type_name_radio_menu_item, _gtk_type_radio_menu_item, _gtk_type_check_menu_item, NULL);
	_wtype_table_put(_clip_type_radio_menu_group, _clip_type_name_radio_menu_group, _gtk_type_radio_menu_group, NULL, NULL);
	return 0;
}
/**********************************************************/

/**** Radio menu item constructor ****/
int
clip_GTK_RADIOMENUITEMNEW(ClipMachine * cm)
{
	ClipVar * cv   = _clip_spar(cm, 1);
	C_widget *cgrp = _fetch_cwidget(cm,_clip_spar(cm, 2));
	char *   label = _clip_parc(cm, 3);
	GtkWidget *wid = NULL;
        C_widget *cwid;
        GSList  *group = NULL;
	CHECKOPT(1,MAP_t);
	CHECKOPT2(2,MAP_t,NUMERIC_t);
	CHECKOPT(3,CHARACTER_t);

	if (cgrp && cgrp->type != GTK_WIDGET_RADIO_MENU_GROUP) goto err;

	if (_clip_parinfo(cm,3) == CHARACTER_t)
	{
		LOCALE_TO_UTF(label);
		if (cgrp && cgrp->data)
			group = gtk_radio_menu_item_get_group(
				GTK_RADIO_MENU_ITEM(((GSList*)(cgrp->data))->data));
		wid = gtk_radio_menu_item_new_with_label(group,label);
        	FREE_TEXT(label);
        }
        else
        {
		if (cgrp && cgrp->data)
			group = gtk_radio_menu_item_get_group(
				GTK_RADIO_MENU_ITEM(((GSList*)(cgrp->data))->data));
		wid = gtk_radio_menu_item_new(group);
	}
	if (cgrp && !cgrp->data)
		cgrp->data = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(wid));

        if (!wid) goto err;

	cwid = _register_widget(cm, wid, cv);
        _clip_mclone(cm,RETPTR(cm),&cwid->obj);

	return 0;
err:
	return 1;
}
int
clip_GTK_RADIOMENUITEMNEWWITHMNEMONIC(ClipMachine * cm)
{
	ClipVar * cv   = _clip_spar(cm, 1);
	C_widget *cgrp = _fetch_cwidget(cm,_clip_spar(cm, 2));
	char *   label = _clip_parc(cm, 3);
	GtkWidget *wid = NULL;
        C_widget *cwid;
        GSList  *group = NULL;
	CHECKOPT(1,MAP_t);
	CHECKOPT2(2,MAP_t,NUMERIC_t);
	CHECKARG(3,CHARACTER_t);

	if (cgrp && cgrp->type != GTK_WIDGET_RADIO_MENU_GROUP) goto err;

	LOCALE_TO_UTF(label);
	if (cgrp && cgrp->data)
		group = gtk_radio_menu_item_get_group(
			GTK_RADIO_MENU_ITEM(((GSList*)(cgrp->data))->data));
	wid = gtk_radio_menu_item_new_with_mnemonic(group,label);
        FREE_TEXT(label);

	if (cgrp && !cgrp->data)
		cgrp->data = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(wid));

        if (!wid) goto err;

	cwid = _register_widget(cm, wid, cv);
        _clip_mclone(cm,RETPTR(cm),&cwid->obj);

	return 0;
err:
	return 1;
}
/**** ------------------ ****/

/**** Radio menu group constructor ****/
int
clip_GTK_RADIOMENUGROUPNEW(ClipMachine * cm)
{
	ClipVar * cv   = _clip_spar(cm, 1);
        C_widget *cwid;
	CHECKOPT(1,MAP_t);

	cwid = _register_widget(cm, NULL, cv);
        cwid->type = _clip_type_radio_menu_group();
	_clip_mclone(cm,RETPTR(cm),&cwid->obj);

	return 0;
err:
	return 1;
}
/**** ------------------ ****/

/* Sets a RadioButton's group. It should be noted that this does not
 * change the layout of your interface in any way, so if you are
 * changing the group, it is likely you will need to re-arrange the
 * user interface to reflect these changes. */
int
clip_GTK_RADIOMENUITEMSETGROUP(ClipMachine * cm)
{
	C_widget *citm = _fetch_cw_arg(cm);
	C_widget *cgrp = _fetch_cwidget(cm,_clip_spar(cm,2));
        GSList * group = NULL, *item;
        GtkRadioMenuItem *rmitem, *active_item = NULL;
	CHECKARG2(2,MAP_t,NUMERIC_t);
        CHECKCWID(citm,GTK_IS_RADIO_MENU_ITEM);
        if (cgrp && cgrp->type != GTK_WIDGET_RADIO_MENU_GROUP) goto err;
	if (cgrp && cgrp->data)
        {
		group = gtk_radio_menu_item_get_group(
			GTK_RADIO_MENU_ITEM(((GSList*)(cgrp->data))->data));
		for (item=group; item; item=item->next)
        	{
        		rmitem = GTK_RADIO_MENU_ITEM(item->data);
        		if (GTK_CHECK_MENU_ITEM(rmitem)->active) active_item = rmitem;
        	}
	}
	gtk_radio_menu_item_set_group(GTK_RADIO_MENU_ITEM(citm->widget),group);
	if (group)
		for (item=group; item; item=item->next)
        	{
        		rmitem = GTK_RADIO_MENU_ITEM(item->data);
                	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(rmitem), FALSE);
        	}
        if (active_item)
         	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(active_item), TRUE);
	if (cgrp && cgrp->data)
		cgrp->data = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(citm->widget));
        return 0;
err:
	return 1;
}

int
clip_GTK_RADIOMENUITEMGETGROUP(ClipMachine * cm)
{
	C_widget *citm = _fetch_cw_arg(cm);
        GSList * group = NULL;
        ClipVar    *cv = RETPTR(cm);
        long         l ;

	CHECKARG2(1,MAP_t,NUMERIC_t);
        CHECKCWID(citm,GTK_IS_RADIO_MENU_ITEM);

	group = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(citm->widget));

	l = g_slist_length(group);
        _clip_array(cm, cv, 1, &l);
	for (l=0; group; group=g_slist_next(group), l++)
        {
        	GtkRadioMenuItem *rmitem;
                C_widget        *crmitem;

        	rmitem = GTK_RADIO_MENU_ITEM(group->data);
                crmitem = _register_widget(cm, GTK_WIDGET(rmitem), NULL);
                _clip_aset(cm, cv, &crmitem->obj, 1, &l);
        }

        return 0;
err:
	return 1;
}

int
clip_GTK_RADIOMENUITEMSETSTYLE(ClipMachine * cm)
{
	C_widget   *citm = _fetch_cw_arg(cm);
	ClipVar  *mstyle = _clip_spar(cm,2);
	GtkStyle *style;
        GtkWidget *wid;
        CHECKCWID(citm,GTK_IS_ITEM);
	CHECKARG(2,MAP_t);
        wid = GTK_BIN(&(GTK_ITEM(citm->widget)->bin))->child;
	style = gtk_style_copy(wid->style);
        //gtk_style_unref(wid->style);
	_map_to_style(cm, mstyle, style);
	gtk_widget_set_style (wid, style);
	return 0;
err:
	return 1;
}

#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 4)
int
clip_GTK_RADIOMENUITEMNEWFROMWIDGET(ClipMachine * cm)
{
	C_widget *citm = _fetch_cw_arg(cm);
	GtkWidget *wid = NULL;
        C_widget *cwid;

	CHECKARG2(1,MAP_t,NUMERIC_t);
        CHECKCWID(citm,GTK_IS_RADIO_MENU_ITEM);

	wid = gtk_radio_menu_item_new_from_widget(GTK_RADIO_MENU_ITEM(citm->widget));


        if (!wid) goto err;

	cwid = _register_widget(cm, wid, NULL);
        _clip_mclone(cm,RETPTR(cm),&cwid->obj);
        return 0;
err:
	return 1;
}

int
clip_GTK_RADIOMENUITEMNEWWITHLABELFROMWIDGET(ClipMachine * cm)
{
	C_widget *citm = _fetch_cw_arg(cm);
        gchar   *label = _clip_parc(cm, 2);
	GtkWidget *wid = NULL;
        C_widget *cwid;

	CHECKARG2(1,MAP_t,NUMERIC_t);
        CHECKCWID(citm,GTK_IS_RADIO_MENU_ITEM);
        CHECKARG(2, CHARACTER_t);

	LOCALE_TO_UTF(label);
	wid = gtk_radio_menu_item_new_with_label_from_widget(GTK_RADIO_MENU_ITEM(citm->widget), label);
        FREE_TEXT(label);

        if (!wid) goto err;

	cwid = _register_widget(cm, wid, NULL);
        _clip_mclone(cm,RETPTR(cm),&cwid->obj);
        return 0;
err:
	return 1;
}

int
clip_GTK_RADIOMENUITEMNEWWITHMNEMONICFROMWIDGET(ClipMachine * cm)
{
	C_widget *citm = _fetch_cw_arg(cm);
        gchar   *label = _clip_parc(cm, 2);
	GtkWidget *wid = NULL;
        C_widget *cwid;

	CHECKARG2(1,MAP_t,NUMERIC_t);
        CHECKCWID(citm,GTK_IS_RADIO_MENU_ITEM);
        CHECKARG(2, CHARACTER_t);

	LOCALE_TO_UTF(label);
	wid = gtk_radio_menu_item_new_with_mnemonic_from_widget(GTK_RADIO_MENU_ITEM(citm->widget), label);
        FREE_TEXT(label);

        if (!wid) goto err;

	cwid = _register_widget(cm, wid, NULL);
        _clip_mclone(cm,RETPTR(cm),&cwid->obj);
        return 0;
err:
	return 1;
}
#endif

