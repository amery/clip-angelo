/*
    Copyright (C) 2003  ITK
    Author   : Uri (uri@itk.ru)
    License : (GPL) http://www.itk.ru/clipper/license.html
*/

/* Virtual metods for CODB object over DBF files */

#include <ci_codbcfg.ch>

#ifndef __1

static data

#include <codb_cache/codb_CacheAdd.prg>
#include <codb_cache/codb_CacheGet.prg>
#include <codb_cache/codb_CacheDel.prg>
#include <codb_cache/codb_CacheDestroy.prg>
#include <codb_cache/static___check_data.prg>

#else

static data:=map()

#include <codb_cache/__1_codb_CacheAdd.prg>
#include <codb_cache/__1_codb_CacheGet.prg>
#include <codb_cache/__1_codb_CacheDel.prg>
#include <codb_cache/__1_codb_CacheDestroy.prg>

#endif
