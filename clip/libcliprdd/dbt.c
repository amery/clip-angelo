/*
	Copyright (C) 2001  ITK
	Author  : Rushad Nabiullin <rust@itk.ru>
	License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ci_rdd.h>
#include <ci_error.ch>
#include <ci_dbinfo.ch>

#define DBT_PAGE_SIZE 512

static RDD_MEMO_VTBL *dbt_vtbl();

#include <dbt/clip_INIT_DBT.c>
#include <dbt/typedef_struct_DBT_HEADER.h>
#include <dbt/static_dbt_create.c>
#include <dbt/static_dbt_zap.c>
#include <dbt/static_dbt_open.c>
#include <dbt/static_dbt_close.c>
#include <dbt/static_dbt_getvalue.c>
#include <dbt/static_dbt_setvalue.c>
#include <dbt/dbt_info.c>
#include <dbt/static_dbt_vtbl.c>
