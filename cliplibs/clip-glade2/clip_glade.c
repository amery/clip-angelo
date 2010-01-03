/*

 clip_glade.c
 Copyright (C) - Sergio Zayas Tor� - 2.005

 function: static SignalTable *
    _sig_table_by_name(GtkType wtype, const gchar *signame)
 Copyright (C) 2001-2004 ITK

 License :
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

*/

/*
   Implementation state (Estado de la implementaci�n)

State  = XXX-Ok || DDD-Not checked

            function                            State

void        glade_init                          XXX
void        glade_require                       XXX
GladeXML*   glade_xml_new                       XXX
GladeXML*   glade_xml_new_from_buffer           XXX
gboolean    glade_xml_construct                 DDD

void        glade_xml_signal_connect            XXX
void        glade_xml_signal_connect_data       Not needed in Clip
void        glade_xml_signal_autoconnect        XXX

GtkWidget*  glade_xml_get_widget                XXX
GList*      glade_xml_get_widget_prefix         XXX
const char* glade_get_widget_name               XXX
GladeXML*   glade_get_widget_tree               DDD

void        glade_xml_signal_connect_full       Internal use
void        glade_xml_signal_autoconnect_full   Internal use

void        glade_set_custom_handler

*/

#include <string.h>

#include <gtk/gtk.h>
#include <gtk/gtkprivate.h>

#include <glade/glade-xml.h>
#include <glade/glade-init.h>

#include "ci_clip.h"
#include "ci_clip-gtk2.ch"
#include "ci_clip-gtk2.h"

/* Previous declarations. */
static SignalTable *_sig_table_by_name(GtkType wtype, const gchar * signame);

static void _signal_connect_func(const gchar * handler_name,
				 GObject * object,
				 const gchar * signal_name,
				 const gchar * signal_data, GObject * connect_object, gboolean after, gpointer user_data);
static void _signal_autoconnect_func(const gchar * handler_name,
				     GObject * object,
				     const gchar * signal_name,
				     const gchar * signal_data, GObject * connect_object, gboolean after, gpointer user_data);
/* End previous declarations. */

/*
void glade_init (void);
*/
int
clip_GLADE_INIT(ClipMachine * ClipMachineMemory)
{
   glade_init();
   return 0;
}

/*
void glade_require (const gchar *library);
*/
int
clip_GLADE_REQUIRE(ClipMachine * ClipMachineMemory)
{
   char     *lib = _clip_parc(ClipMachineMemory, 1);

   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   glade_require(lib);

   return 0;
 err:
   return 1;
}

/*
GladeXML*   glade_xml_new (const char *fname,
                           const char *root,
                           const char *domain);
*/
int
clip_GLADE_XML_NEW(ClipMachine * ClipMachineMemory)
{
   GladeXML *xml;

   char     *fname = _clip_parc(ClipMachineMemory, 1);

   char     *root = _clip_parc(ClipMachineMemory, 2);

   char     *domain = _clip_parc(ClipMachineMemory, 3);

   CHECKARG(1, CHARACTER_type_of_ClipVarType);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT(3, CHARACTER_type_of_ClipVarType);

   xml = glade_xml_new(fname, root, domain);

   if (xml)
      _clip_retnl(ClipMachineMemory, (long) xml);

   return 0;

 err:
   return 1;
}

/*
GladeXML*   glade_xml_new_from_buffer     (const char *buffer,
                                             int size,
                                             const char *root,
                                             const char *domain);
*/
int
clip_GLADE_XML_NEW_FROM_BUFFER(ClipMachine * ClipMachineMemory)
{
   GladeXML *xml;

   char     *buffer = _clip_parc(ClipMachineMemory, 1);

   int       size = _clip_parni(ClipMachineMemory, 2);

   char     *root = _clip_parc(ClipMachineMemory, 3);

   char     *domain = _clip_parc(ClipMachineMemory, 4);

   CHECKARG(1, CHARACTER_type_of_ClipVarType);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, CHARACTER_type_of_ClipVarType);
   CHECKOPT(4, CHARACTER_type_of_ClipVarType);

   xml = glade_xml_new_from_buffer(buffer, size, root, domain);

   if (xml)
      _clip_retnl(ClipMachineMemory, (long) xml);

   return 0;

 err:
   return 1;
}

/*
gboolean    glade_xml_construct  (GladeXML *self,
                                  const char *fname,
                                  const char *root,
                                  const char *domain);
*/
int
clip_GLADE_XML_CONSTRUCT(ClipMachine * ClipMachineMemory)
{
   GladeXML *xml = (GladeXML *) _clip_parnl(ClipMachineMemory, 1);

   char     *fname = _clip_parc(ClipMachineMemory, 2);

   char     *root = _clip_parc(ClipMachineMemory, 3);

   char     *domain = _clip_parc(ClipMachineMemory, 4);

   CHECKARG(1, NUMERIC_type_of_ClipVarType);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT(3, CHARACTER_type_of_ClipVarType);
   CHECKOPT(4, CHARACTER_type_of_ClipVarType);

   _clip_retl(ClipMachineMemory, glade_xml_construct(xml, fname, root, domain));

   return 0;

 err:
   return 1;
}

/*
GtkWidget*  glade_xml_get_widget            (GladeXML *self,
                                             const char *name);
*/
int
clip_GLADE_XML_GET_WIDGET(ClipMachine * ClipMachineMemory)
{

   GladeXML *xml;

   char     *wname;

   GtkWidget *wid;

   C_widget *cwid;

   CHECKARG(1, NUMERIC_type_of_ClipVarType);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   xml = (GladeXML *) _clip_parnl(ClipMachineMemory, 1);
   wname = _clip_parc(ClipMachineMemory, 2);

   wid = glade_xml_get_widget(xml, wname);

   if (wid)
    {

      /*
         Aqu� Clip crea sus estructuras para guardar el widget,
         pero se produce una creaci�n parcial si el widget es
         'compuesto'. p.ej. si es un widget GtkFileSelection
         no se crean (en Clip) los widgets internos como filesel:okButton,
         por tanto no se le puede asignar una se�al con gtk_signalConnect();
         hay que hacerlo con glade_xml_signal_connect() � bien generar el
         widget con glade_xml_get_widget() y conectarlo con gtk_signalConnect().
       */
       cwid = _register_widget(ClipMachineMemory, wid, NULL);

       _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
    }

   return 0;
 err:
   return 1;
}

/*
GList*      glade_xml_get_widget_prefix     (GladeXML *self,
                                             const char *name);
*/
int
clip_GLADE_XML_GET_WIDGET_PREFIX(ClipMachine * ClipMachineMemory)
{
   GladeXML *xml;

   char     *prefix;

   GList    *list;

   ClipVar  *arr = RETPTR(ClipMachineMemory);

   long      ln;

   CHECKARG(1, NUMERIC_type_of_ClipVarType);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   xml = (GladeXML *) _clip_parnl(ClipMachineMemory, 1);
   prefix = _clip_parc(ClipMachineMemory, 2);

   list = glade_xml_get_widget_prefix(xml, prefix);
   if (!list)
      return 0;

   ln = g_list_length(list);
   _clip_array(ClipMachineMemory, arr, 1, &ln);

   for (ln = 0; list; list = g_list_next(list), ln++)
    {
       GtkWidget *wid;

       C_widget *cwid;

       wid = GTK_WIDGET(list->data);
       if (wid)
	{
	   cwid = _list_get_cwidget(ClipMachineMemory, wid);
	   if (!cwid)
	      cwid = _register_widget(ClipMachineMemory, wid, NULL);
	   if (cwid)
	      _clip_aset(ClipMachineMemory, arr, &cwid->obj, 1, &ln);
	}

    }
   free(list);
   return 0;
 err:
   return 1;
}

/*
const char* glade_get_widget_name           (GtkWidget *widget);
*/
int
clip_GLADE_GET_WIDGET_NAME(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   const char *res;

   CHECKCWID(cwid, GTK_IS_WIDGET);

   res = glade_get_widget_name(cwid->widget);
   if (res)
      _clip_retc(ClipMachineMemory, (char *) res);
   else
      _clip_retc(ClipMachineMemory, "");

   return 0;
 err:
   return 1;
}

/*
GladeXML*   glade_get_widget_tree           (GtkWidget *widget);
*/
int
clip_GLADE_GET_WIDGET_TREE(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);

   GladeXML *xml;

   CHECKCWID(cwid, GTK_IS_WIDGET);

   xml = glade_get_widget_tree(cwid->widget);

   if (xml)
      _clip_retnl(ClipMachineMemory, (long) xml);

   return 0;
 err:
   return 1;
}

/*
void        glade_xml_signal_connect        (GladeXML *self,
                                             const char *handlername,
                                             GCallback func);
*/
int
clip_GLADE_XML_SIGNAL_CONNECT(ClipMachine * ClipMachineMemory)
{
   GladeXML *xml = (GladeXML *) _clip_parnl(ClipMachineMemory, 1);

   char     *handlername = _clip_parc(ClipMachineMemory, 2);

   ClipVar  *clipf = _clip_spar(ClipMachineMemory, 3);

   C_var    *data;

   CHECKARG(1, NUMERIC_type_of_ClipVarType);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);
   CHECKARG2(3, PCODE_type_of_ClipVarType, CCODE_type_of_ClipVarType);

   data = NEW(C_var);
   data->ClipMachineMemory = ClipMachineMemory;
   _clip_mclone(ClipMachineMemory, &data->cfunc, clipf);

   glade_xml_signal_connect_full(xml, handlername, _signal_connect_func, data);

   return 0;
 err:
   return 1;
}

/*
void        glade_xml_signal_autoconnect    (GladeXML *self);
*/
int
clip_GLADE_XML_SIGNAL_AUTOCONNECT(ClipMachine * ClipMachineMemory)
{
   GladeXML *xml = (GladeXML *) _clip_parnl(ClipMachineMemory, 1);

   glade_xml_signal_autoconnect_full(xml, _signal_autoconnect_func, ClipMachineMemory);

   return 0;
}

/* Private functions. */

/*void        (*GladeXMLConnectFunc)          (const gchar *handler_name,
                                             GObject *object,
                                             const gchar *signal_name,
                                             const gchar *signal_data,
                                             GObject *connect_object,
                                             gboolean after,
                                             gpointer user_data);*/
static void
_signal_connect_func(const gchar * handler_name,
		     GObject * object,
		     const gchar * signal_name,
		     const gchar * signal_data, GObject * connect_object, gboolean after, gpointer user_data)
{
   C_var    *c = (C_var *) user_data;

   C_widget *c_wid = _list_get_cwidget(c->ClipMachineMemory, object);

   SignalTable *sig_table = NULL;

   GtkSignalFunc sfunc;

   if (!c_wid)
     /* Aqu� Clip crea sus estructuras para guardar el widget. */
      c_wid = _register_widget(c->ClipMachineMemory, GTK_WIDGET(object), NULL);

   if (c_wid)
    {
       C_signal *cs;

       c_wid->siglist = NEW(C_signal);
       cs = c_wid->siglist;
       if (!c_wid->siglist)
	{
	   c_wid->siglist = NEW(C_signal);
	   cs = c_wid->siglist;
	}
       else
	{
	   for (cs = c_wid->siglist; cs && cs->next; cs = cs->next);
	   cs->next = NEW(C_signal);
	   cs = cs->next;
	}

       sig_table = _sig_table_by_name(c_wid->type, (char *) signal_name);
       if (sig_table)
	{
	   sfunc = sig_table->sigfunction;
	   cs->cw = c_wid;
	   cs->signame = sig_table->signame;
	   cs->sigid = sig_table->sigid;
	   _clip_mclone(c->ClipMachineMemory, &cs->cfunc, &c->cfunc);

	   if (after)
	      g_signal_connect_after(GTK_OBJECT(c_wid->widget), signal_name, GSF(sfunc), cs);
	   else
	      g_signal_connect(GTK_OBJECT(c_wid->widget), signal_name, GSF(sfunc), cs);
	}
    }
}

/*
void        glade_xml_signal_connect_full   (GladeXML *self,
                                             const gchar *handler_name,
                                             GladeXMLConnectFunc func,
                                             gpointer user_data);


*/
static void
_signal_autoconnect_func(const gchar * handler_name,
			 GObject * object,
			 const gchar * signal_name,
			 const gchar * signal_data, GObject * connect_object, gboolean after, gpointer user_data)
{
/* Busca un funci�n con el mismo nombre que handler_name y genera el gestor de se�al. */
   ClipMachine *ClipMachineMemory = (ClipMachine *) user_data;

   ClipFunction *fp;

   ClipBlock *bp;

   long      hash;

   C_var    *cv;

   ClipVar  *func;

  // Se busca la funcion
   hash = _clip_casehashbytes(0, handler_name, strlen(handler_name));
   if (!_clip_get_function(ClipMachineMemory, hash, &fp, &bp))
    {
       _clip_logg(3, "glade_xml_signal_autoconnect: not found function %s\n", handler_name);
       return;
    }

   func = NEW(ClipVar);
   func->ClipType_t_of_ClipVar.count_of_ClipType = 1;
   func->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;

   if (bp)
    {
       bp->file_of_ClipBlock->refCount_of_ClipFile++;
       func->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = PCODE_type_of_ClipVarType;
       func->ClipCodeVar_c_of_ClipVar.u.ClipBlock_block_of_ClipCodeVar = NEW(ClipBlock);
       *func->ClipCodeVar_c_of_ClipVar.u.ClipBlock_block_of_ClipCodeVar = *bp;
    }
   else
    {
      /* It's Ok ??  */
       func->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CCODE_type_of_ClipVarType;
       func->ClipCodeVar_c_of_ClipVar.u.ClipFunction_function_of_ClipCodeVar = fp;
    }

   cv = NEW(C_var);
   cv->ClipMachineMemory = ClipMachineMemory;
   _clip_mclone(ClipMachineMemory, &cv->cfunc, func);

  // Esta es la llamada final, conecta la funci�n.
   _signal_connect_func(handler_name, object, signal_name, signal_data, connect_object, after, cv);
}

/* Function from clip-gtk2:object.c - (c) 2001-2004 ITK */
/* Find SignalTable entry by signal string name */
static SignalTable *
_sig_table_by_name(GtkType wtype, const gchar * signame)
{
   WTypeTable *wt_item = NULL;

   SignalTable *sig_table = NULL;

   ExtraSignalTable *extra_sig_table = NULL;

   TypeFunc  tfun = NULL;

   do
    {
      /* Get WTypeTable entry corresponding to type of this widget */
       wt_item = _wtype_table_get(wtype);
       sig_table = wt_item ? wt_item->signal_table : NULL;
      /* Search the signal name in SignalTable */
       for (; sig_table && sig_table->sigfunction; sig_table++)
	{
	   if (strcmp(sig_table->signame, signame) == 0)
	      return sig_table;
	}
       extra_sig_table = wt_item ? wt_item->extra_signal_table : NULL;
      /* Search the signal name in ExtraSignalTable */
       for (; extra_sig_table && extra_sig_table->sigfunction; extra_sig_table = extra_sig_table->next)
	{
	   if (strcmp(extra_sig_table->signame, signame) == 0)
	      return (SignalTable *) extra_sig_table;
	}
      /* if not found, get WTypeTable entry for parent type and continue searching */
       if (wt_item)
	  tfun = wt_item->fsuper_type;
       if (tfun)
	  wtype = tfun();
    }
   while (tfun);
   return NULL;
}
