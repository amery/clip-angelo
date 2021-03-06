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

/***          SpinButton have no own signals            ***/

/**********************************************************/
CLIP_DLLEXPORT GtkType
_gtk_type_spin_button()
{
   return GTK_TYPE_SPIN_BUTTON;
}

long
_clip_type_spin_button()
{
   return GTK_WIDGET_SPIN_BUTTON;
}

const char *
_clip_type_name_spin_button()
{
   return "GTK_TYPE_SPIN_BUTTON";
}

/* Register spin button in global table */
int
clip_INIT___SPIN_BUTTON(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_spin_button, _clip_type_name_spin_button, _gtk_type_spin_button, _gtk_type_entry, NULL);
   return 0;
}

/**** ------------------ ****/
/**** SPIN BUTTON constructor ****/
int
clip_GTK_SPINBUTTONNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   C_widget *cadj = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   gfloat    climb_rate = DBL_OPTION(ClipMachineMemory, 3, 1);

   guint     digits = INT_OPTION(ClipMachineMemory, 4, 1);

   GtkWidget *wid = NULL;

   GtkAdjustment *adj;

   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cadj, GTK_IS_ADJUSTMENT);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, NUMERIC_type_of_ClipVarType);
   adj = cadj ? GTK_ADJUSTMENT(cadj->widget) : NULL;
   wid = gtk_spin_button_new(adj, climb_rate, digits);
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

/* Changes which GtkAdjustment is associated with a spin button. */
int
clip_GTK_SPINBUTTONSETADJUSTMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *cspb = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cadj = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(cspb, GTK_IS_SPIN_BUTTON);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(cadj, GTK_IS_ADJUSTMENT);
   gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(cspb->widget), GTK_ADJUSTMENT(cadj->widget));
   return 0;
 err:
   return 1;
}

/* Retrieves the GtkAdjustment used by a given spin button. */
int
clip_GTK_SPINBUTTONGETADJUSTMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *cspb = _fetch_cw_arg(ClipMachineMemory);

   ClipVar  *cv = RETPTR(ClipMachineMemory);

   GtkAdjustment *adj;

   C_widget *cadj;

   CHECKCWID(cspb, GTK_IS_SPIN_BUTTON);
   adj = gtk_spin_button_get_adjustment(GTK_SPIN_BUTTON(cspb->widget));
   cadj = _list_get_cwidget(ClipMachineMemory, adj);
   if (!cadj)
      cadj = _register_widget(ClipMachineMemory, (GtkWidget *) adj, NULL);
   if (cadj)
      _clip_mclone(ClipMachineMemory, cv, &cadj->obj);
   return 0;
 err:
   return 1;
}

/* Alters the number of decimal places that are displayed in a spin button. */
int
clip_GTK_SPINBUTTONSETDIGITS(ClipMachine * ClipMachineMemory)
{
   C_widget *cspb = _fetch_cw_arg(ClipMachineMemory);

   guint     digits = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(cspb, GTK_IS_SPIN_BUTTON);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_spin_button_set_digits(GTK_SPIN_BUTTON(cspb->widget), digits);
   return 0;
 err:
   return 1;
}

/* Retrieves the current value of a GtkSpinButton. */
int
clip_GTK_SPINBUTTONGETVALUE(ClipMachine * ClipMachineMemory)
{
   C_widget *cspb = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cspb, GTK_IS_SPIN_BUTTON);
   _clip_retnd(ClipMachineMemory, gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(cspb->widget)));
   return 0;
 err:
   return 1;
}

/* Sets the value of a spin button. */
int
clip_GTK_SPINBUTTONSETVALUE(ClipMachine * ClipMachineMemory)
{
   C_widget *cspb = _fetch_cw_arg(ClipMachineMemory);

   gfloat    value = _clip_parnd(ClipMachineMemory, 2);

   CHECKCWID(cspb, GTK_IS_SPIN_BUTTON);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_spin_button_set_value(GTK_SPIN_BUTTON(cspb->widget), value);
   return 0;
 err:
   return 1;
}

/* Changes the way a spin button refreshes and updates itself. */
int
clip_GTK_SPINBUTTONSETUPDATEPOLICY(ClipMachine * ClipMachineMemory)
{
   C_widget *cspb = _fetch_cw_arg(ClipMachineMemory);

   GtkSpinButtonUpdatePolicy policy = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(cspb, GTK_IS_SPIN_BUTTON);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_spin_button_set_update_policy(GTK_SPIN_BUTTON(cspb->widget), policy);
   return 0;
 err:
   return 1;
}

/* Sets how the spin button's GtkEntry reacts to alphabetic characters.
 * A value of TRUE to numeric means that all non-numeric characters
 * (except '-' and a decimal point) are ignored. */
int
clip_GTK_SPINBUTTONSETNUMERIC(ClipMachine * ClipMachineMemory)
{
   C_widget *cspb = _fetch_cw_arg(ClipMachineMemory);

   gboolean  numeric = BOOL_OPTION(ClipMachineMemory, 2, TRUE);

   CHECKCWID(cspb, GTK_IS_SPIN_BUTTON);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(cspb->widget), numeric);
   return 0;
 err:
   return 1;
}

/* Performs an explicit 'spin' on a spin button. */
int
clip_GTK_SPINBUTTONSPIN(ClipMachine * ClipMachineMemory)
{
   C_widget *cspb = _fetch_cw_arg(ClipMachineMemory);

   GtkSpinType direction = _clip_parni(ClipMachineMemory, 2);

   gfloat    increment = _clip_parnd(ClipMachineMemory, 3);

   CHECKCWID(cspb, GTK_IS_SPIN_BUTTON);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   gtk_spin_button_spin(GTK_SPIN_BUTTON(cspb->widget), direction, increment);
   return 0;
 err:
   return 1;
}

/* Sets a spin button's value to the lower limit when
 * it's upper limit is reached, and vice versa. */
int
clip_GTK_SPINBUTTONSETWRAP(ClipMachine * ClipMachineMemory)
{
   C_widget *cspb = _fetch_cw_arg(ClipMachineMemory);

   gboolean  wrap = BOOL_OPTION(ClipMachineMemory, 2, TRUE);

   CHECKCWID(cspb, GTK_IS_SPIN_BUTTON);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   gtk_spin_button_set_wrap(GTK_SPIN_BUTTON(cspb->widget), wrap);
   return 0;
 err:
   return 1;
}

/* Creates a border around the arrows of a GtkSpinButton.
 * The type of border is determined by shadow_type. */
int
clip_GTK_SPINBUTTONSETSHADOWTYPE(ClipMachine * ClipMachineMemory)
{
   C_widget *cspb = _fetch_cw_arg(ClipMachineMemory);

   GtkShadowType shadow_type = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(cspb, GTK_IS_SPIN_BUTTON);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_spin_button_set_shadow_type(GTK_SPIN_BUTTON(cspb->widget), shadow_type);
   return 0;
 err:
   return 1;
}

int
clip_GTK_SPINBUTTONSETCLIMBRATE(ClipMachine * ClipMachineMemory)
{
   C_widget *cspb = _fetch_cw_arg(ClipMachineMemory);

   gfloat    climb_rate = _clip_parnd(ClipMachineMemory, 2);

   CHECKCWID(cspb, GTK_IS_SPIN_BUTTON);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_widget_set(cspb->widget, "climb-rate", climb_rate, NULL);
   return 0;
 err:
   return 1;
}

int
clip_GTK_SPINBUTTONGETCLIMBRATE(ClipMachine * ClipMachineMemory)
{
   C_widget *cspb = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cspb, GTK_IS_SPIN_BUTTON);
   _clip_retnd(ClipMachineMemory, GTK_SPIN_BUTTON(cspb->widget)->climb_rate);
   return 0;
 err:
   return 1;
}

int
clip_GTK_SPINBUTTONGETDIGITS(ClipMachine * ClipMachineMemory)
{
   C_widget *cspb = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cspb, GTK_IS_SPIN_BUTTON);
   _clip_retni(ClipMachineMemory, GTK_SPIN_BUTTON(cspb->widget)->digits);
   return 0;
 err:
   return 1;
}

int
clip_GTK_SPINBUTTONGETSNAPTOTICKS(ClipMachine * ClipMachineMemory)
{
   C_widget *cspb = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cspb, GTK_IS_SPIN_BUTTON);
   _clip_retnl(ClipMachineMemory, GTK_SPIN_BUTTON(cspb->widget)->snap_to_ticks);
   return 0;
 err:
   return 1;
}

int
clip_GTK_SPINBUTTONGETNUMERIC(ClipMachine * ClipMachineMemory)
{
   C_widget *cspb = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cspb, GTK_IS_SPIN_BUTTON);
   _clip_retnl(ClipMachineMemory, GTK_SPIN_BUTTON(cspb->widget)->numeric);
   return 0;
 err:
   return 1;
}

int
clip_GTK_SPINBUTTONGETWRAP(ClipMachine * ClipMachineMemory)
{
   C_widget *cspb = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cspb, GTK_IS_SPIN_BUTTON);
   _clip_retnl(ClipMachineMemory, GTK_SPIN_BUTTON(cspb->widget)->wrap);
   return 0;
 err:
   return 1;
}

int
clip_GTK_SPINBUTTONGETUPDATEPOLICY(ClipMachine * ClipMachineMemory)
{
   C_widget *cspb = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cspb, GTK_IS_SPIN_BUTTON);
   _clip_retnl(ClipMachineMemory, GTK_SPIN_BUTTON(cspb->widget)->update_policy);
   return 0;
 err:
   return 1;
}

int
clip_GTK_SPINBUTTONGETSHADOWTYPE(ClipMachine * ClipMachineMemory)
{
   C_widget *cspb = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cspb, GTK_IS_SPIN_BUTTON);
   _clip_retnl(ClipMachineMemory, GTK_SPIN_BUTTON(cspb->widget)->shadow_type);
   return 0;
 err:
   return 1;
}

/* Sets whether a number typed into a spin button should
 * be snapped to the nearest step increment. */
int
clip_GTK_SPINBUTTONSETSNAPTOTICKS(ClipMachine * ClipMachineMemory)
{
   C_widget *cspb = _fetch_cw_arg(ClipMachineMemory);

   gboolean  snap_to_ticks = BOOL_OPTION(ClipMachineMemory, 2, TRUE);

   CHECKCWID(cspb, GTK_IS_SPIN_BUTTON);
   CHECKOPT(2, LOGICAL_type_of_ClipVarType);
   gtk_spin_button_set_snap_to_ticks(GTK_SPIN_BUTTON(cspb->widget), snap_to_ticks);
   return 0;
 err:
   return 1;
}

/* Refreshes a spin button. The behaviour of the update is
 * determined by gtk_spin_button_set_update_policy(). */
int
clip_GTK_SPINBUTTONUPDATE(ClipMachine * ClipMachineMemory)
{
   C_widget *cspb = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cspb, GTK_IS_SPIN_BUTTON);
   gtk_spin_button_update(GTK_SPIN_BUTTON(cspb->widget));
   return 0;
 err:
   return 1;
}
