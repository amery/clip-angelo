/*
    Copyright (C) 2001  ITK
    Author   : Uri (uri@itk.ru)
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
**********************************************************************
#include "ci_fileio.ch"
#include "ci_foxsql.ch"
#include "ci_error.ch"
**********************************************************************
//#define DEBUG_WHERE
//#define DEBUG_OPTIMIZE_WHERE
//#define DEBUG_FILTERS
//#define DEBUG_GROUP
//#define DEBUG_ORDER
**********************************************************************
**********************************************************************
#include "foxsel/static___sql_order_by.ch"
#include "foxsel/static_anymap_newLine.ch"
#include "foxsel/static_anymap_out.ch"
#include "foxsel/static_arr_newLine.ch"
#include "foxsel/static_arr_out.ch"
#include "foxsel/static_dev_newLine.ch"
#include "foxsel/static_dev_out.ch"
#include "foxsel/static_empty_newLine.ch"
#include "foxsel/static_empty_out.ch"
#include "foxsel/static_file_newLine.ch"
#include "foxsel/static_file_out.ch"
#include "foxsel/static_fx_sql_error.ch"
#include "foxsel/static_makeSqlSubSelect.ch"
#include "foxsel/static_map_newLine.ch"
#include "foxsel/static_map_out.ch"
#include "foxsel/static_out_recursive.ch"
#include "foxsel/static_sql_append.ch"
#include "foxsel/static_sql_closeDB.ch"
#include "foxsel/static_sql_closeOut.ch"
#include "foxsel/static_sql_destroy.ch"
#include "foxsel/static_sql_initOut.ch"
#include "foxsel/static_sql_logicalOptimizeExpr.ch"
#include "foxsel/static_sql_makeFilters.ch"
#include "foxsel/static_sql_openDB.ch"
#include "foxsel/static_sql_outResult.ch"
#include "foxsel/static_sql_parse.ch"
#include "foxsel/static_sql_parseExpr.ch"
#include "foxsel/static_sql_parseFields.ch"
#include "foxsel/static_sql_parseFrom.ch"
#include "foxsel/static_sql_parseGroup.ch"
#include "foxsel/static_sql_parseOrder.ch"
#include "foxsel/static_sql_parseTo.ch"
#include "foxsel/static_sql_run.ch"
#include "foxsel/static_sql_toBuffer.ch"
#include "foxsel/static_tbl_newLine.ch"
#include "foxsel/static_tbl_out.ch"
**********************************************************************
#include "foxsel/__sql_avg.ch"
#include "foxsel/__sql_count.ch"
#include "foxsel/__sql_fieldDesc.ch"
#include "foxsel/__sql_getValue.ch"
#include "foxsel/__sql_max.ch"
#include "foxsel/__sql_min.ch"
#include "foxsel/__sql_run_subQuery.ch"
#include "foxsel/__sql_sum.ch"
#include "foxsel/sqlQueryNew.ch"
**********************************************************************
