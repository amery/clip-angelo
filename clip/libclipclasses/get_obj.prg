//#define DEBUG

/*
    Copyright (C) 2001  ITK
    Author   : Uri (uri@itk.ru)
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
******************************************************************
#include <ci_set.ch>
#include <ci_lang.ch>
#include <ci_getexit.ch>
#include <ci_date.ch>
#include <ci_button.ch>
#include <ci_config.ch>
#include <ci_inkey.ch>
**********************************************************************
#include <get_obj/static___analizepic.prg>
#include <get_obj/static___checksym.prg>
#include <get_obj/static___fillbuffer.prg>
#include <get_obj/static___get_split_by_picture.prg>
#include <get_obj/static___set_default_keys.prg>
#include <get_obj/static___setkey.prg>
#include <get_obj/static___tostring.prg>
#include <get_obj/static___updateinfo.prg>
#include <get_obj/static__display.prg>
#include <get_obj/static_assign.prg>
#include <get_obj/static_backspace.prg>
#include <get_obj/static_baddate.prg>
#include <get_obj/static_colordisp.prg>
#include <get_obj/static_cur_end.prg>
#include <get_obj/static_cur_goto.prg>
#include <get_obj/static_cur_home.prg>
#include <get_obj/static_cur_left.prg>
#include <get_obj/static_cur_right.prg>
#include <get_obj/static_cur_setpos.prg>
#include <get_obj/static_delend.prg>
#include <get_obj/static_delright.prg>
#include <get_obj/static_delwordleft.prg>
#include <get_obj/static_delwordright.prg>
#include <get_obj/static_display.prg>
#include <get_obj/static_get_hittest.prg>
#include <get_obj/static_get_setcolor.prg>
#include <get_obj/static_get_undo.prg>
#include <get_obj/static_get_width.prg>
#include <get_obj/static_insert.prg>
#include <get_obj/static_killfocus.prg>
#include <get_obj/static_overstrike.prg>
#include <get_obj/static_reload.prg>
#include <get_obj/static_reset.prg>
#include <get_obj/static_setfocus.prg>
#include <get_obj/static_todecpos.prg>
#include <get_obj/static_untransform.prg>
#include <get_obj/static_updatebuffer.prg>
#include <get_obj/static_varget.prg>
#include <get_obj/static_varput.prg>
#include <get_obj/static_wordleft.prg>
#include <get_obj/static_wordright.prg>
**********************************************************************
#include <get_obj/___get.prg>
#include <get_obj/__cget__.prg>
#include <get_obj/__get.prg>
#include <get_obj/__get__.prg>
#include <get_obj/__caplength.prg>
#include <get_obj/__saycaption.prg>
#include <get_obj/__splitcolors.prg>
#include <get_obj/_recover_get.prg>
#include <get_obj/devoutpict.prg>
#include <get_obj/getreadvar.prg>
#include <get_obj/getnew.prg>
#include <get_obj/trans.prg>
#include <get_obj/transform.prg>
**********************************************************************
