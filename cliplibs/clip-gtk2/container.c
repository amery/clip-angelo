/*
    Copyright (C) 2001 - 2004  ITK
    Author  : Alexey M. Tkachenko <alexey@itk.ru>
    	      Elena V. Kornilova <alena@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include <string.h>
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg2.h"

#include <gtk/gtk.h>

#include "ci_clip-gtk2.ch"
#include "ci_clip-gtk2.h"

/*********************** SIGNALS **************************/
static gint
container_handle_signal(GtkContainer * container, GtkWidget * widget, C_signal * cs)
{
   C_widget *cwid;
   PREPARECV(cs, cv);
   cwid = _list_get_cwidget(cs->cw->cmachine, widget);
   if (!cwid)
      cwid = _register_widget(cs->cw->cmachine, widget, NULL);
   if (cwid)
      _clip_madd(cs->cw->cmachine, &cv, HASH_WIDGET, &cwid->obj);
   INVOKESIGHANDLER(GTK_WIDGET(container), cs, cv);
}

static SignalTable container_signals[] = {
   {"add", GSF(container_handle_signal), ESF(object_emit_signal),
    GTK_ADD_SIGNAL},
   {"remove", GSF(container_handle_signal), ESF(object_emit_signal),
    GTK_REMOVE_SIGNAL},
   {"check-resize", GSF(widget_signal_handler), ESF(object_emit_signal),
    GTK_CHECK_RESIZE_SIGNAL},
   {"focus", GSF(container_handle_signal), ESF(object_emit_signal),
    GTK_FOCUS_SIGNAL},
   {"set-focus-child", GSF(container_handle_signal), ESF(object_emit_signal),
    GTK_SET_FOCUS_CHILD_SIGNAL},
   {"", NULL, NULL, 0}
};

/* Register container signals table in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_container()
{
   return GTK_TYPE_CONTAINER;
}

long
_clip_type_container()
{
   return GTK_WIDGET_CONTAINER;
}

const char *
_clip_type_name_container()
{
   return "GTK_WIDGET_CONTAINER";
}

CLIP_DLLEXPORT GtkType
_gtk_type_event_box()
{
   return GTK_TYPE_EVENT_BOX;
}

long
_clip_type_event_box()
{
   return GTK_WIDGET_EVENT_BOX;
}

const char *
_clip_type_name_event_box()
{
   return "GTK_WIDGET_EVENT_BOX";
}

int
clip_INIT___CONTAINER(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_container, _clip_type_name_container, _gtk_type_container, _gtk_type_widget, container_signals);
   _wtype_table_put(_clip_type_event_box, _clip_type_name_event_box, _gtk_type_event_box, _gtk_type_bin, NULL);
//      _wtype_table_put(_clip_type_event_box, _clip_type_name_event_box, _gtk_type_event_box, _gtk_type_container, NULL);
   return 0;
}

/**********************************************************/

/**** Event Box constructor ****/
int
clip_GTK_EVENTBOXNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   GtkWidget *wid = NULL;
   C_widget *cwid;
   CHECKOPT(1, MAP_type_of_ClipVarType);

   wid = gtk_event_box_new();
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
clip_GTK_CONTAINERADD(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKCWID(ccon, GTK_IS_CONTAINER);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_container_add(GTK_CONTAINER(ccon->widget), cwid->widget);
   return 0;
 err:
   return 1;
}

int
clip_GTK_CONTAINERREMOVE(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKCWID(ccon, GTK_IS_CONTAINER);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_container_remove(GTK_CONTAINER(ccon->widget), cwid->widget);
   return 0;
 err:
   return 1;
}

int
clip_GTK_CONTAINERADDWITHPROPERTIES(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   ClipVar *cv = _clip_par(ClipMachineMemory, 3);
   gchar *pname[20];
   gint i;

   CHECKCWID(ccon, GTK_IS_CONTAINER);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKARG(3, ARRAY_type_of_ClipVarType);

   memset(pname, 0, sizeof(pname));
	for (i = 0; i < cv->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar; i++)
		pname[i] = cv->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar[i].ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;

   gtk_container_add_with_properties(GTK_CONTAINER(ccon->widget),
				     cwid->widget, pname[0], pname[1],
				     pname[2], pname[3], pname[4], pname[5],
				     pname[6], pname[7], pname[8], pname[9], pname[10], pname[11], pname[12], pname[13], pname[14], pname[15], pname[16], pname[17], pname[18], pname[19], NULL);
   return 0;
 err:
   return 1;
}

int
clip_GTK_CONTAINERGETRESIZEMODE(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(ccon, GTK_IS_CONTAINER);
   _clip_retni(ClipMachineMemory, (int) gtk_container_get_resize_mode(GTK_CONTAINER(ccon->widget)));
   return 0;
 err:
   return 1;
}

int
clip_GTK_CONTAINERSETRESIZEMODE(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   GtkResizeMode mode = _clip_parni(ClipMachineMemory, 2);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ccon, GTK_IS_CONTAINER);
   gtk_container_set_resize_mode(GTK_CONTAINER(ccon->widget), mode);
   return 0;
 err:
   return 1;
}

int
clip_GTK_CONTAINERCHECKRESIZE(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(ccon, GTK_IS_CONTAINER);
   gtk_container_check_resize(GTK_CONTAINER(ccon->widget));
   return 0;
 err:
   return 1;
}

/*
static void
_container_children_func(GtkWidget *wid, gpointer data)
{
	C_var *c = (C_var*)data;
	C_widget *c_wid = _list_get_cwidget(c->ClipMachineMemory, wid);
	if (!c_wid)
		c_wid = _register_widget(c->ClipMachineMemory,wid,NULL);
	if (c_wid)
		_clip_aadd(c->ClipMachineMemory, c->cv, &c_wid->obj);
}
*/
int
clip_GTK_CONTAINERGETCHILDREN(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   GList *list;
   ClipVar *cv = RETPTR(ClipMachineMemory);
   long l;

   CHECKCWID(ccon, GTK_IS_CONTAINER);

   list = gtk_container_get_children(GTK_CONTAINER(ccon->widget));
   l = g_list_length(list);
   _clip_array(ClipMachineMemory, cv, 1, &l);
  //memset(cv, 0, sizeof(cv));
   for (l = 0; list; list = g_list_next(list), l++)
      {
	 C_widget *cwid = 0;
	 GtkWidget *wid;
	 wid = (GtkWidget *) list->data;
	 if (wid)
	    {
	       cwid = _list_get_cwidget(ClipMachineMemory, cwid);
	       if (!cwid)
		  cwid = _register_widget(ClipMachineMemory, wid, NULL);
	       if (cwid)
		  _clip_aset(ClipMachineMemory, cv, &cwid->obj, 1, &l);
	    }
      }
   free(list);
   return 0;
 err:
   return 1;
}

// Returns a list of container children
int
clip_GTK_CONTAINERCHILDREN(ClipMachine * ClipMachineMemory)
{
   return clip_GTK_CONTAINERGETCHILDREN(ClipMachineMemory);
/*
	C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
	long n=0;
	ClipVar *a = RETPTR(ClipMachineMemory);
	C_var c;

	CHECKCWID(ccon,GTK_IS_CONTAINER);

	_clip_array(ClipMachineMemory, a, 1, &n);
	c.ClipMachineMemory = ClipMachineMemory; c.cv = a;
	gtk_container_foreach(GTK_CONTAINER(ccon->widget),
		(GtkCallback)_container_children_func,&c);
	return 0;
err:
	return 1;
*/
}

static void
_container_for_each_func(GtkWidget * wid, gpointer data)
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
	 _clip_mclone(c->cw->cmachine, &stack[0], &c->cw->obj);
	 _clip_mclone(c->cw->cmachine, &stack[1], &c_wid->obj);
	//stack[0] = c->cw->obj;
	//stack[1] = c_wid->obj;
	 _clip_eval(c->ClipMachineMemory, &(c->cfunc), 2, stack, &res);
	 _clip_destroy(c->ClipMachineMemory, &res);
	 _clip_destroy(c->ClipMachineMemory, &stack[0]);
	 _clip_destroy(c->ClipMachineMemory, &stack[1]);
      }
}

/* Calls the CLIP function for each children */
int
clip_GTK_CONTAINERFOREACH(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   ClipVar *cfunc = _clip_spar(ClipMachineMemory, 2);
   C_var *c = 0;

   CHECKCWID(ccon, GTK_IS_CONTAINER);
   CHECKARG2(2, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);

   c->ClipMachineMemory = ClipMachineMemory;
   c->cw = ccon;
   _clip_mclone(ClipMachineMemory, &c->cfunc, cfunc);
   gtk_container_foreach(GTK_CONTAINER(ccon->widget), (GtkCallback) _container_for_each_func, c);
   return 0;
 err:
   return 1;
}

int
clip_GTK_CONTAINERSETREALLOCATEREDRAWS(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   gboolean needs_redraw = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(ccon, GTK_IS_CONTAINER);
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_container_set_reallocate_redraws(GTK_CONTAINER(ccon->widget), needs_redraw);
   return 0;
 err:
   return 1;
}

int
clip_GTK_CONTAINERSETFOCUSCHILD(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKCWID(ccon, GTK_IS_CONTAINER);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_container_set_focus_child(GTK_CONTAINER(ccon->widget), cwid->widget);
   return 0;
 err:
   return 1;
}

int
clip_GTK_CONTAINERGETFOCUSVADJUSTMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwid;
   GtkAdjustment *adj;
   CHECKCWID(ccon, GTK_IS_CONTAINER);
   adj = gtk_container_get_focus_vadjustment(GTK_CONTAINER(ccon->widget));
   if (adj)
      {
	 cwid = _list_get_cwidget(ClipMachineMemory, GTK_WIDGET(adj));
	 if (!cwid)
	    cwid = _register_widget(ClipMachineMemory, GTK_WIDGET(adj), NULL);
	 if (cwid)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
      }
   return 0;
 err:
   return 1;
}

int
clip_GTK_CONTAINERGETFOCUSHADJUSTMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwid;
   GtkAdjustment *adj;
   CHECKCWID(ccon, GTK_IS_CONTAINER);
   adj = gtk_container_get_focus_hadjustment(GTK_CONTAINER(ccon->widget));
   if (adj)
      {
	 cwid = _list_get_cwidget(ClipMachineMemory, GTK_WIDGET(adj));
	 if (!cwid)
	    cwid = _register_widget(ClipMachineMemory, GTK_WIDGET(adj), NULL);
	 if (cwid)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
      }
   return 0;
 err:
   return 1;
}

int
clip_GTK_CONTAINERSETFOCUSVADJUSTMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKCWID(ccon, GTK_IS_CONTAINER);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_ADJUSTMENT);
   gtk_container_set_focus_vadjustment(GTK_CONTAINER(ccon->widget), GTK_ADJUSTMENT(cwid->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_CONTAINERSETFOCUSHADJUSTMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKCWID(ccon, GTK_IS_CONTAINER);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_ADJUSTMENT);
   gtk_container_set_focus_hadjustment(GTK_CONTAINER(ccon->widget), GTK_ADJUSTMENT(cwid->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_CONTAINERRESIZECHILDREN(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   if (!ccon || !GTK_IS_CONTAINER(ccon->widget))
      goto err;
   gtk_container_resize_children(GTK_CONTAINER(ccon->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_CONTAINERCHILDTYPE(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   if (!ccon || !GTK_IS_CONTAINER(ccon->widget))
      goto err;
   _clip_retni(ClipMachineMemory, (int) gtk_container_child_type(GTK_CONTAINER(ccon->widget)));

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ContainerChildGet(containerObj, childWidget, @sPromName1, @sPropName2,... ) -->
*******************************************************************************/
int
clip_GTK_CONTAINERCHILDGET(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   gchar *pname[20];
   gint i, n;

   if (!ccon || !GTK_IS_CONTAINER(ccon->widget))
      goto err;
   CHECKCWID(cwid, GTK_IS_WIDGET);

   gtk_container_child_get(GTK_CONTAINER(ccon->widget), cwid->widget,
			   pname[0], pname[1], pname[2], pname[3], pname[4],
			   pname[5], pname[6], pname[7], pname[8], pname[9], pname[10], pname[11], pname[12], pname[13], pname[14], pname[15], pname[16], pname[17], pname[18], pname[19], NULL);

   n = _clip_parinfo(ClipMachineMemory, 0);
   for (i = 3; i < n; i++)
      _clip_storc(ClipMachineMemory, pname[i - 3], i, 0);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ContainerChildSet(containerObj, childWidget, @sPromName1, @sPropName2,... ) -->
*******************************************************************************/
int
clip_GTK_CONTAINERCHILDSET(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   gchar *pname[20];
   gint i, n;

   if (!ccon || !GTK_IS_CONTAINER(ccon->widget))
      goto err;
   CHECKCWID(cwid, GTK_IS_WIDGET);
   n = _clip_parinfo(ClipMachineMemory, 0);
   for (i = 3; i < n; i++)
      {
	 CHECKOPT2(i, CHARACTER_type_of_ClipVarType, UNDEF_type_of_ClipVarType);
	 if (_clip_parinfo(ClipMachineMemory, i) == UNDEF_type_of_ClipVarType)
	    break;
	 pname[i - 3] = _clip_parc(ClipMachineMemory, i);
      }

   gtk_container_child_set(GTK_CONTAINER(ccon->widget), cwid->widget,
			   pname[0], pname[1], pname[2], pname[3], pname[4],
			   pname[5], pname[6], pname[7], pname[8], pname[9], pname[10], pname[11], pname[12], pname[13], pname[14], pname[15], pname[16], pname[17], pname[18], pname[19], NULL);

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ContainerChildGetPrperty(containerObj, childWidget, sPropName, @value ) -->
*******************************************************************************/
int
clip_GTK_CONTAINERCHILDGETPROPERTY(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   gchar *name = _clip_parc(ClipMachineMemory, 3);
   GValue val;
   GType type;

   if (!ccon || !GTK_IS_CONTAINER(ccon->widget))
      goto err;
   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKARG(3, CHARACTER_type_of_ClipVarType);

   gtk_container_child_get_property(GTK_CONTAINER(ccon->widget), cwid->widget, name, &val);

   type = G_VALUE_TYPE(&val);
   switch (type)
      {
      case G_TYPE_INT:
	 _clip_storni(ClipMachineMemory, g_value_get_int(&val), 3, 0);
	 break;
      case G_TYPE_STRING:
	 _clip_storc(ClipMachineMemory, g_value_get_string(&val), 3, 0);
	 break;
      case G_TYPE_BOOLEAN:
	 _clip_storl(ClipMachineMemory, g_value_get_boolean(&val), 3, 0);
	 break;
      case G_TYPE_LONG:
	 _clip_storni(ClipMachineMemory, g_value_get_long(&val), 3, 0);
	 break;
      case G_TYPE_DOUBLE:
	 _clip_stornd(ClipMachineMemory, g_value_get_double(&val), 3, 0);
	 break;
      }
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_ContainerChildSetPrperty(containerObj, childWidget, sPropName, value ) -->
*******************************************************************************/
int
clip_GTK_CONTAINERCHILDSETPROPERTY(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   gchar *name = _clip_parc(ClipMachineMemory, 3);
   GValue val;
   ClipVar *cv = _clip_par(ClipMachineMemory, 4);
   long type;

   if (!ccon || !GTK_IS_CONTAINER(ccon->widget))
      goto err;
   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKARG(3, CHARACTER_type_of_ClipVarType);

	type = cv->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType;
   switch (type)
      {
      case NUMERIC_type_of_ClipVarType:
	 g_value_init(&val, G_TYPE_INT);
	 g_value_set_int(&val, cv->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
	 break;
      case CHARACTER_type_of_ClipVarType:
	 g_value_init(&val, G_TYPE_STRING);
	 g_value_set_string(&val, cv->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
	 break;
      case LOGICAL_type_of_ClipVarType:
	 g_value_init(&val, G_TYPE_BOOLEAN);
	 g_value_set_boolean(&val, cv->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar);
	 break;
      }
   gtk_container_child_set_property(GTK_CONTAINER(ccon->widget), cwid->widget, name, &val);

   return 0;
 err:
   return 1;
}

int
clip_GTK_CONTAINERFORALL(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   ClipVar *cfunc = _clip_spar(ClipMachineMemory, 2);
   C_var *c = 0;

   CHECKCWID(ccon, GTK_IS_CONTAINER);
   CHECKARG2(2, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);

   c->ClipMachineMemory = ClipMachineMemory;
   c->cw = ccon;
   _clip_mclone(ClipMachineMemory, &c->cfunc, cfunc);
   gtk_container_forall(GTK_CONTAINER(ccon->widget), (GtkCallback) _container_for_each_func, c);
   return 0;
 err:
   return 1;
}

/* Sets a width of container's border */
int
clip_GTK_CONTAINERGETBORDERWIDTH(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(ccon, GTK_IS_CONTAINER);
   _clip_retni(ClipMachineMemory, gtk_container_get_border_width(GTK_CONTAINER(ccon->widget)));
   return 0;
 err:
   return 1;
}

int
clip_GTK_CONTAINERSETBORDERWIDTH(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   int width = _clip_parni(ClipMachineMemory, 2);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKCWID(ccon, GTK_IS_CONTAINER);
   gtk_container_set_border_width(GTK_CONTAINER(ccon->widget), width);
   return 0;
 err:
   return 1;
}

int
clip_GTK_CONTAINERGETFOCUSCHAIN(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   ClipVar *cv = _clip_par(ClipMachineMemory, 2);
   GList *list;
   long l;
   gboolean ret;

   CHECKCWID(ccon, GTK_IS_CONTAINER);

   ret = gtk_container_get_focus_chain(GTK_CONTAINER(ccon->widget), &list);
   list = g_list_first(list);
   l = g_list_length(list);
   _clip_array(ClipMachineMemory, cv, 1, &l);
   for (l = 0; list; list = g_list_next(list), l++)
      {
	 C_widget *cwid;
	 GtkWidget *wid;
	 wid = GTK_WIDGET(list->data);
	 if (wid)
	    {
	       cwid = _list_get_cwidget(ClipMachineMemory, wid);
	       if (!cwid)
		  cwid = _register_widget(ClipMachineMemory, wid, NULL);
	       if (cwid)
		  _clip_aset(ClipMachineMemory, cv, &cwid->obj, 1, &l);
	    }
      }
   g_list_free(list);
   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

int
clip_GTK_CONTAINERSETFOCUSCHAIN(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   ClipArrVar *cv = (ClipArrVar *) _clip_vptr(_clip_spar(ClipMachineMemory, 2));
   GList *list = NULL;
   gint n, l;

   CHECKCWID(ccon, GTK_IS_CONTAINER);
   CHECKARG(2, ARRAY_type_of_ClipVarType);

   n = cv->count_of_ClipArrVar;
   for (l = 0; l < n; l++)
      {
	 C_widget *cwid = _fetch_cwidget(ClipMachineMemory, &cv->ClipVar_items_of_ClipArrVar[l]);
	 CHECKCWID(cwid, GTK_IS_WIDGET);
	 list = g_list_append(list, GTK_WIDGET(cwid->widget));
      }
   gtk_container_set_focus_chain(GTK_CONTAINER(ccon->widget), list);
   return 0;
 err:
   return 1;
}

int
clip_GTK_CONTAINERUNSETFOCUSCHAIN(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(ccon, GTK_IS_CONTAINER);

   gtk_container_unset_focus_chain(GTK_CONTAINER(ccon->widget));
   return 0;
 err:
   return 1;
}

#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 4)
int
clip_GTK_EVENTBOXSETVISIBLE(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   gint visible_window = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(ccon, GTK_IS_EVENT_BOX);
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_event_box_set_visible_window(GTK_EVENT_BOX(ccon->widget), visible_window);

   return 0;
 err:
   return 1;
}

int
clip_GTK_EVENTBOXGETVISIBLE(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(ccon, GTK_IS_EVENT_BOX);

   _clip_retl(ClipMachineMemory, gtk_event_box_get_visible_window(GTK_EVENT_BOX(ccon->widget)));

   return 0;
 err:
   return 1;
}

int
clip_GTK_EVENTBOXSETABOVECHILD(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);
   gboolean above_child = _clip_parl(ClipMachineMemory, 2);

   CHECKCWID(ccon, GTK_IS_EVENT_BOX);
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_event_box_set_above_child(GTK_EVENT_BOX(ccon->widget), above_child);

   return 0;
 err:
   return 1;
}

int
clip_GTK_EVENTBOXGETABOVECHILD(ClipMachine * ClipMachineMemory)
{
   C_widget *ccon = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(ccon, GTK_IS_EVENT_BOX);

   _clip_retl(ClipMachineMemory, gtk_event_box_get_above_child(GTK_EVENT_BOX(ccon->widget)));

   return 0;
 err:
   return 1;
}
#endif
