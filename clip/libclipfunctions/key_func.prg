/*
    Copyright (C) 2001  ITK
    Author   : Uri (uri@itk.ru)
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
**********************************************************
#include <ci_config.ch>
#include <ci_inkey.ch>
#include <ci_set.ch>
**********************************************************
/* for trapshift */
static tp_proc := NIL
static tp_mode := 0
/* for trapanykey */
static ta_proc := NIL
/* trap recursive control */
static t_recursive := 0
/* keysend() buffer */
static t_buffer := ""
**********************************************************
**********************************************************
#include <key_func/trapshift.prg>
#include <key_func/trapanykey.prg>
#include <key_func/inkey.prg>
#include <key_func/keysend.prg>
#include <key_func/sh_stat.prg>
#include <key_func/lsh_stat.prg>
#include <key_func/sh_l_stat.prg>
#include <key_func/rsh_stat.prg>
#include <key_func/sh_r_stat.prg>
#include <key_func/savesetkey.prg>
#include <key_func/restsetkey.prg>
#include <key_func/clearkey.prg>
#include <key_func/key_name.prg>
#include <key_func/__setfunction.prg>
#include <key_func/inkeytrap.prg>
**********************************************************
