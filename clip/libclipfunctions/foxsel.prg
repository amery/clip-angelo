/*
    Copyright (C) 2001  ITK
    Author   : Uri (uri@itk.ru)
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
**********************************************************************
#include <ci_fileio.ch>
#include <ci_foxsql.ch>
#include <ci_error.ch>
**********************************************************************
//#define DEBUG_WHERE
//#define DEBUG_OPTIMIZE_WHERE
//#define DEBUG_FILTERS
//#define DEBUG_GROUP
//#define DEBUG_ORDER
**********************************************************************
**********************************************************************
#include <foxsel/static___sql_order_by.prg>
#include <foxsel/static_anymap_newLine.prg>
#include <foxsel/static_anymap_out.prg>
#include <foxsel/static_arr_newLine.prg>
#include <foxsel/static_arr_out.prg>
#include <foxsel/static_dev_newLine.prg>
#include <foxsel/static_dev_out.prg>
#include <foxsel/static_empty_newLine.prg>
#include <foxsel/static_empty_out.prg>
#include <foxsel/static_file_newLine.prg>
#include <foxsel/static_file_out.prg>
#include <foxsel/static_fx_sql_error.prg>
#include <foxsel/static_makeSqlSubSelect.prg>
#include <foxsel/static_map_newLine.prg>
#include <foxsel/static_map_out.prg>
#include <foxsel/static_out_recursive.prg>
#include <foxsel/static_sql_append.prg>
#include <foxsel/static_sql_closeDB.prg>
#include <foxsel/static_sql_closeOut.prg>
#include <foxsel/static_sql_destroy.prg>
#include <foxsel/static_sql_initOut.prg>
#include <foxsel/static_sql_logicalOptimizeExpr.prg>
#include <foxsel/static_sql_makeFilters.prg>
#include <foxsel/static_sql_openDB.prg>
#include <foxsel/static_sql_outResult.prg>
#include <foxsel/static_sql_parse.prg>
#include <foxsel/static_sql_parseExpr.prg>
#include <foxsel/static_sql_parseFields.prg>
#include <foxsel/static_sql_parseFrom.prg>
#include <foxsel/static_sql_parseGroup.prg>
#include <foxsel/static_sql_parseOrder.prg>
#include <foxsel/static_sql_parseTo.prg>
#include <foxsel/static_sql_run.prg>
#include <foxsel/static_sql_toBuffer.prg>
#include <foxsel/static_tbl_newLine.prg>
#include <foxsel/static_tbl_out.prg>
**********************************************************************
#include <foxsel/__sql_avg.prg>
#include <foxsel/__sql_count.prg>
#include <foxsel/__sql_fieldDesc.prg>
#include <foxsel/__sql_getValue.prg>
#include <foxsel/__sql_max.prg>
#include <foxsel/__sql_min.prg>
#include <foxsel/__sql_run_subQuery.prg>
#include <foxsel/__sql_sum.prg>
#include <foxsel/sqlQueryNew.prg>
**********************************************************************
