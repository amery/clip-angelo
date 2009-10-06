/*
    Copyright (C) 2005  ITK
    Author  : Elena V. Kornilova <alena@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include <string.h>
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg2.h"

#include <gtk/gtk.h>

#include "ci_clip-gtk2.ch"
#include "ci_clip-gtk2.h"

int
clip_GTK_RCPARSESTRING(ClipMachine * cm)
{
        const gchar *str = _clip_parc(cm, 1);

        CHECKARG(1, CHARACTER_t);

	gtk_rc_parse_string(str);

	return 0;
err:
	return 1;
}


