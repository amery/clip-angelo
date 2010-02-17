/*
    Copyright (C) 2004  ITK
    Author  : Elena V. Kornilova <alena@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg.h"

#include <string.h>
#include <gtk/gtk.h>

#include "ci_clip-gtk.ch"
#include "ci_clip-gtk.h"

/*********************** SIGNALS **************************/

/* ItemFactory has no signals */

/**********************************************************/
CLIP_DLLEXPORT GtkType
_gtk_type_item_factory()
{
   return GTK_TYPE_ITEM_FACTORY;
}

long
_clip_type_item_factory()
{
   return GTK_OBJECT_ITEM_FACTORY;
}

const char *
_clip_type_name_item_factory()
{
   return "GTK_TYPE_ITEM_FACTORY";
}

int
clip_INIT___ITEMFACTORY(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_item_factory, _clip_type_name_item_factory, _gtk_type_item_factory, NULL, NULL);
   return 0;
}

static void
_item_factory_callback1(gpointer data, guint callback_action, GtkWidget * wid)
{
   C_var *c = (C_var *) data;
   C_widget *c_wid = _list_get_cwidget(c->ClipMachineMemory, wid);
   ClipVar stack[2];
   ClipVar res;
   if (!c_wid)
      c_wid = _register_widget(c->ClipMachineMemory, wid, NULL);
   if (c_wid)
      {
	 memset(&stack, 0, sizeof(stack));
	 memset(&res, 0, sizeof(ClipVar));
	 _clip_mclone(c->co->cmachine, &stack[0], &c->co->obj);
	 stack[1] = c_wid->obj;
	 _clip_eval(c->ClipMachineMemory, &(c->cfunc), 2, stack, &res);
	 _clip_destroy(c->ClipMachineMemory, &res);
      }
}

static void
_item_factory_callback2(GtkWidget * wid, gpointer data, guint callback_action)
{
   C_var *c = (C_var *) data;
   C_widget *c_wid = _list_get_cwidget(c->ClipMachineMemory, wid);
   ClipVar stack[2];
   ClipVar res;
   if (!c_wid)
      c_wid = _register_widget(c->ClipMachineMemory, wid, NULL);
   if (c_wid)
      {
	 memset(&stack, 0, sizeof(stack));
	 memset(&res, 0, sizeof(ClipVar));
	 _clip_mclone(c->co->cmachine, &stack[0], &c->co->obj);
	 stack[1] = c_wid->obj;
	 _clip_eval(c->ClipMachineMemory, &(c->cfunc), 2, stack, &res);
	 _clip_destroy(c->ClipMachineMemory, &res);
      }
}

/******************************************************************************
* gtk_ItemFactoryNew( container_type, spath, accel_group ) --> ItemFactoryNewObject
* container_type is  one of :GTK_TYPE_MENU_BAR, GTK_TYPE_MENU or GTK_TYPE_OPTION_MENU
******************************************************************************/
int
clip_GTK_ITEMFACTORYNEW(ClipMachine * ClipMachineMemory)
{
   gint itype = INT_OPTION(ClipMachineMemory, 1, CLIP_GTK_TYPE_MENU_BAR);
   const gchar *path = _clip_parc(ClipMachineMemory, 2);
   C_object *caccel = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));
   C_object *citem;
   GtkItemFactory *item;
   GtkType type = 0;

   CHECKARG(1, NUMERIC_type_of_ClipVarType);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT(3, MAP_type_of_ClipVarType);
   CHECKCOBJOPT(caccel, GTK_IS_ACCEL_GROUP(caccel));

   switch (itype)
      {
      case CLIP_GTK_TYPE_MENU_BAR:
	 type = GTK_TYPE_MENU_BAR;
	 break;
      case CLIP_GTK_TYPE_MENU:
	 type = GTK_TYPE_MENU;
	 break;
      case CLIP_GTK_TYPE_OPTION_MENU:
	 type = GTK_TYPE_OPTION_MENU;
	 break;
      }

   item = gtk_item_factory_new(type, path, GTK_ACCEL_GROUP(caccel->object));

   if (item)
      {
	 citem = _list_get_cobject(ClipMachineMemory, item);
	 if (!citem)
	    citem = _register_object(ClipMachineMemory, item, GTK_TYPE_ITEM_FACTORY, NULL, NULL);
	 if (citem)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &citem->obj);
      }
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ItemFactoryConstruct( itemFactory, container_type, spath, accel_group ) --> NIL
* container_type is  one of :GTK_TYPE_MENU_BAR, GTK_TYPE_MENU or GTK_TYPE_OPTION_MENU
******************************************************************************/
int
clip_GTK_ITEMFACTORYCONSTRUCT(ClipMachine * ClipMachineMemory)
{
   C_object *citem = _fetch_co_arg(ClipMachineMemory);
   GtkType type = INT_OPTION(ClipMachineMemory, 2, GTK_TYPE_MENU_BAR);
   const gchar *path = _clip_parc(ClipMachineMemory, 3);
   C_object *caccel = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 4));

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citem, GTK_IS_ITEM_FACTORY(citem->object));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, CHARACTER_type_of_ClipVarType);
   CHECKOPT(4, MAP_type_of_ClipVarType);
   CHECKCOBJOPT(caccel, GTK_IS_ACCEL_GROUP(caccel->object));

   gtk_item_factory_construct(GTK_ITEM_FACTORY(citem->object), type, path, GTK_ACCEL_GROUP(caccel->object));

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ItemFactoryAddForeign( accel_widget, sfull_path, accel_group, nkeyvalue, modifiers ) --> NIL
******************************************************************************/
int
clip_GTK_ITEMFACTORYADDFOREIGN(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   const gchar *path = _clip_parc(ClipMachineMemory, 2);
   C_object *caccel = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));
   gint keyval = _clip_parni(ClipMachineMemory, 4);
   GdkModifierType type = _clip_parni(ClipMachineMemory, 5);

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT(3, MAP_type_of_ClipVarType);
   CHECKCOBJOPT(caccel, GTK_IS_ACCEL_GROUP(caccel->object));
   CHECKARG(4, NUMERIC_type_of_ClipVarType);
   CHECKARG(5, NUMERIC_type_of_ClipVarType);

   gtk_item_factory_add_foreign(GTK_WIDGET(cwid->widget), path, GTK_ACCEL_GROUP(caccel->object), keyval, type);

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ItemFactoryFromWidget( widget ) --> itemFactory
******************************************************************************/
int
clip_GTK_ITEMFACTORYFROMWIDGET(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   C_object *citem;
   GtkItemFactory *item;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);

   item = gtk_item_factory_from_widget(GTK_WIDGET(cwid->widget));

   if (item)
      {
	 citem = _list_get_cobject(ClipMachineMemory, item);
	 if (!citem)
	    citem = _register_object(ClipMachineMemory, item, GTK_TYPE_ITEM_FACTORY, NULL, NULL);
	 if (citem)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &citem->obj);
      }
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ItemFactoryPathFromWidget( widget ) --> spath
******************************************************************************/
int
clip_GTK_ITEMFACTORYPATHFROMWIDGET(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   gchar *str;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);

   str = (gchar *) gtk_item_factory_path_from_widget(GTK_WIDGET(cwid->widget));

   _clip_retc(ClipMachineMemory, str);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ItemFactoryGetItem( itemFactory, spath ) --> widget
******************************************************************************/
int
clip_GTK_ITEMFACTORYGETITEM(ClipMachine * ClipMachineMemory)
{
   C_object *citem = _fetch_co_arg(ClipMachineMemory);
   const gchar *path = _clip_parc(ClipMachineMemory, 2);
   GtkWidget *wid;
   C_widget *cwid;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citem, GTK_IS_ITEM_FACTORY(citem->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   wid = gtk_item_factory_get_item(GTK_ITEM_FACTORY(citem->object), path);

   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, NULL);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ItemFactoryGetWidget( itemFactory, spath ) --> widget
******************************************************************************/
int
clip_GTK_ITEMFACTORYGETWIDGET(ClipMachine * ClipMachineMemory)
{
   C_object *citem = _fetch_co_arg(ClipMachineMemory);
   const gchar *path = _clip_parc(ClipMachineMemory, 2);
   GtkWidget *wid;
   C_widget *cwid;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citem, GTK_IS_ITEM_FACTORY(citem->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   wid = gtk_item_factory_get_widget(GTK_ITEM_FACTORY(citem->object), path);

   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, NULL);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ItemFactoryGetWidgetByAction( itemFactory, naction ) --> widget
******************************************************************************/
int
clip_GTK_ITEMFACTORYGETWIDGETBYACTION(ClipMachine * ClipMachineMemory)
{
   C_object *citem = _fetch_co_arg(ClipMachineMemory);
   gint action = _clip_parni(ClipMachineMemory, 2);
   GtkWidget *wid;
   C_widget *cwid;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citem, GTK_IS_ITEM_FACTORY(citem->object));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   wid = gtk_item_factory_get_widget_by_action(GTK_ITEM_FACTORY(citem->object), action);

   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, NULL);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ItemFactoryGetItemByAction( itemFactory, naction ) --> widget
******************************************************************************/
int
clip_GTK_ITEMFACTORYGETITEMBYACTION(ClipMachine * ClipMachineMemory)
{
   C_object *citem = _fetch_co_arg(ClipMachineMemory);
   gint action = _clip_parni(ClipMachineMemory, 2);
   GtkWidget *wid;
   C_widget *cwid;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citem, GTK_IS_ITEM_FACTORY(citem->object));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   wid = gtk_item_factory_get_item_by_action(GTK_ITEM_FACTORY(citem->object), action);

   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, NULL);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ItemFactoryCreateItem( itemFactory, entry, ncallback_type ) --> NIL
* entry is array with elements:
* - path, string    (f.e. "_File")
* - accelerator, string or NIL (f.e. "<control>N" )
* - callback function, code block or 0 (f.e code block or 0)
* - callback action, numeric (f.e. 1)
* - item type, string or NIL (f.e. "<RadioItem>")
* 	possible values:
* 	NIL		-> "<Item>"
* 	""		-> "<Item>"
* 	"<Title>"	-> create a title item
* 	"<Item>"	-> create a simple item
* 	"<ImageItem>"	-> create an item holding an image
* 	"<StockItem>"	-> create an item holding a stock image
* 	"<CheckItem>"	-> create a check item
* 	"<ToggleItem>"	-> create a toggle item
* 	"<RadioItem>"	-> create a radio item
* 	<path>		-> path of a radio item to link against
* 	"<Separator>"	-> create a separator
* 	"<Tearoff>"	-> create a tearoff separator
* 	"<Branch>"	-> create an item to hold sub items
* 	"<LastBranch>"	-> create a right justified item to hold sub items
*****************************************************************************/
int
clip_GTK_ITEMFACTORYCREATEITEM(ClipMachine * ClipMachineMemory)
{
   C_object *citem = _fetch_co_arg(ClipMachineMemory);
   ClipArrVar *cventry = (ClipArrVar *) _clip_vptr(_clip_par(ClipMachineMemory, 2));
   gint type = INT_OPTION(ClipMachineMemory, 3, 1);
   GtkItemFactoryEntry entry;
   ClipVar *cfunc;
   C_var *data;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citem, GTK_IS_ITEM_FACTORY(citem->object));
   CHECKARG(2, ARRAY_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);

   entry.path = (cventry->ClipVar_items_of_ClipArrVar[0].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType) ? NULL :
cventry->ClipVar_items_of_ClipArrVar[0].ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
   entry.accelerator = (cventry->ClipVar_items_of_ClipArrVar[1].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType) ? NULL :
cventry->ClipVar_items_of_ClipArrVar[1].ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;

   cfunc = &cventry->ClipVar_items_of_ClipArrVar[2];

   data = NEW(C_var);
   data->ClipMachineMemory = ClipMachineMemory;
   data->co = citem;
   _clip_mclone(ClipMachineMemory, &data->cfunc, cfunc);

   if (type == 1)
      entry.callback = (cfunc == NULL) ? 0 : (GtkItemFactoryCallback1) _item_factory_callback1;
   else
      entry.callback = (cfunc == NULL) ? 0 : (GtkItemFactoryCallback2) _item_factory_callback2;

	entry.callback_action = (cventry->ClipVar_items_of_ClipArrVar[3].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType) ? 0 : (guint)
cventry->ClipVar_items_of_ClipArrVar[3].ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
   entry.item_type = (cventry->ClipVar_items_of_ClipArrVar[4].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType) ? NULL :
cventry->ClipVar_items_of_ClipArrVar[4].ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;

   gtk_item_factory_create_item(GTK_ITEM_FACTORY(citem->object), &entry, data, type);

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ItemFactoryCreateItems( itemFactory, array_entry ) --> NIL
******************************************************************************/
int
clip_GTK_ITEMFACTORYCREATEITEMS(ClipMachine * ClipMachineMemory)
{
   C_object *citem = _fetch_co_arg(ClipMachineMemory);
   ClipArrVar *cventry = (ClipArrVar *) _clip_vptr(_clip_par(ClipMachineMemory, 2));
   gint i, n;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citem, GTK_IS_ITEM_FACTORY(citem->object));
   CHECKARG(2, ARRAY_type_of_ClipVarType);

   n = cventry->count_of_ClipArrVar;

   for (i = 0; i < n; i++)
      {
	 ClipArrVar *it = (ClipArrVar *) _clip_vptr(cventry->ClipVar_items_of_ClipArrVar[i].ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar);
	 ClipVar *cfunc;
	 C_var *c;
	 GtkItemFactoryEntry entry;

	 entry.path = (it->ClipVar_items_of_ClipArrVar[0].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType) ? NULL :
it->ClipVar_items_of_ClipArrVar[0].ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
	 entry.accelerator = (it->ClipVar_items_of_ClipArrVar[1].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType) ? NULL :
it->ClipVar_items_of_ClipArrVar[1].ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;

	 cfunc = (it->ClipVar_items_of_ClipArrVar[2].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType) ? NULL : &it->ClipVar_items_of_ClipArrVar[2];

	 c = NEW(C_var);
	 c->ClipMachineMemory = ClipMachineMemory;
	 c->co = citem;
	 _clip_mclone(ClipMachineMemory, &c->cfunc, cfunc);

	 entry.callback = (cfunc == NULL) ? 0 : (GtkItemFactoryCallback1) _item_factory_callback1;

	 entry.callback_action = 1;
	 entry.item_type = (it->ClipVar_items_of_ClipArrVar[4].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType) ? 0 :
it->ClipVar_items_of_ClipArrVar[4].ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
	 gtk_item_factory_create_item(GTK_ITEM_FACTORY(citem->object), &entry, c, 1);
      }

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ItemFactoryDeleteItem( itemFactory, spath ) --> NIL
******************************************************************************/
int
clip_GTK_ITEMFACTORYDELETEITEM(ClipMachine * ClipMachineMemory)
{
   C_object *citem = _fetch_co_arg(ClipMachineMemory);
   const gchar *path = _clip_parc(ClipMachineMemory, 2);

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citem, GTK_IS_ITEM_FACTORY(citem->object));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   gtk_item_factory_delete_item(GTK_ITEM_FACTORY(citem->object), path);

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ItemFactoryDeleteEntry( itemFactory, entry ) --> NIL
*****************************************************************************/
int
clip_GTK_ITEMFACTORYDELETEENTRY(ClipMachine * ClipMachineMemory)
{
   C_object *citem = _fetch_co_arg(ClipMachineMemory);
   ClipArrVar *cventry = (ClipArrVar *) _clip_vptr(_clip_par(ClipMachineMemory, 2));
   GtkItemFactoryEntry entry;
   ClipVar *cfunc;
   C_var *data;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citem, GTK_IS_ITEM_FACTORY(citem->object));
   CHECKARG(2, ARRAY_type_of_ClipVarType);

   entry.path = (cventry->ClipVar_items_of_ClipArrVar[0].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType) ? NULL :
cventry->ClipVar_items_of_ClipArrVar[0].ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
   entry.accelerator = (cventry->ClipVar_items_of_ClipArrVar[1].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType) ? NULL :
cventry->ClipVar_items_of_ClipArrVar[1].ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;

   cfunc = &cventry->ClipVar_items_of_ClipArrVar[2];

   data = NEW(C_var);
   data->ClipMachineMemory = ClipMachineMemory;
   data->co = citem;
   _clip_mclone(ClipMachineMemory, &data->cfunc, cfunc);

   entry.callback = (cfunc == NULL) ? 0 : (GtkItemFactoryCallback1) _item_factory_callback1;

	entry.callback_action = (cventry->ClipVar_items_of_ClipArrVar[3].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType) ? 0 : (guint)
cventry->ClipVar_items_of_ClipArrVar[3].ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
   entry.item_type = (cventry->ClipVar_items_of_ClipArrVar[4].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType) ? NULL :
cventry->ClipVar_items_of_ClipArrVar[4].ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;

   gtk_item_factory_delete_entry(GTK_ITEM_FACTORY(citem->object), &entry);

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ItemFactoryDeleteEntries( itemFactory, array_entry ) --> NIL
******************************************************************************/
int
clip_GTK_ITEMFACTORYDELETEENTRIES(ClipMachine * ClipMachineMemory)
{
   C_object *citem = _fetch_co_arg(ClipMachineMemory);
   ClipArrVar *cventry = (ClipArrVar *) _clip_vptr(_clip_par(ClipMachineMemory, 2));
   gint i, n;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citem, GTK_IS_ITEM_FACTORY(citem->object));
   CHECKARG(2, ARRAY_type_of_ClipVarType);

   n = cventry->count_of_ClipArrVar;

   for (i = 0; i < n; i++)
      {
	 ClipArrVar *it = (ClipArrVar *) _clip_vptr(cventry->ClipVar_items_of_ClipArrVar[i].ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar);
	 ClipVar *cfunc;
	 C_var *c;
	 GtkItemFactoryEntry entry;

	 entry.path = (it->ClipVar_items_of_ClipArrVar[0].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType) ? NULL :
it->ClipVar_items_of_ClipArrVar[0].ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
	 entry.accelerator = (it->ClipVar_items_of_ClipArrVar[1].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType) ? NULL :
it->ClipVar_items_of_ClipArrVar[1].ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;

	 cfunc = (it->ClipVar_items_of_ClipArrVar[2].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType) ? NULL : &it->ClipVar_items_of_ClipArrVar[2];

	 c = NEW(C_var);
	 c->ClipMachineMemory = ClipMachineMemory;
	 c->co = citem;
	 _clip_mclone(ClipMachineMemory, &c->cfunc, cfunc);

	 entry.callback = (cfunc == NULL) ? 0 : (GtkItemFactoryCallback1) _item_factory_callback1;

	 entry.callback_action = 1;
	 entry.item_type = (it->ClipVar_items_of_ClipArrVar[4].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType) ? 0 :
it->ClipVar_items_of_ClipArrVar[4].ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
	 gtk_item_factory_delete_entry(GTK_ITEM_FACTORY(citem->object), &entry);
      }

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ItemFactoryPopup( itemFactory, nx, ny, nmouse_button, ntime ) --> NIL
* itemFactory of type GLIP_GTK_TYPE_MENU
******************************************************************************/
int
clip_GTK_ITEMFACTORYPOPUP(ClipMachine * ClipMachineMemory)
{
   C_object *citem = _fetch_co_arg(ClipMachineMemory);
   gint x = _clip_parni(ClipMachineMemory, 2);
   gint y = _clip_parni(ClipMachineMemory, 3);
   gint mouse_button = _clip_parni(ClipMachineMemory, 4);
   guint32 time = _clip_parni(ClipMachineMemory, 5);

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citem, GTK_IS_ITEM_FACTORY(citem->object));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);
   CHECKARG(5, NUMERIC_type_of_ClipVarType);

   gtk_item_factory_popup(GTK_ITEM_FACTORY(citem->object), x, y, mouse_button, time);

   return 0;
 err:
   return 1;
}
