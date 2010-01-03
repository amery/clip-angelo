#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include "ci_clip.h"
#include "ci_coll.h"
#include "../libclipscreen/ci_charset.h"
#include "ci_dbfsql.h"
#include "ci_error.ch"

const char subsys[] = "DBFSQL";

const char er_connect[] = "Can't connect to database server";

const char er_noconnect[] = "No such connection";

const char er_nosql[] = "No SQL statement";

const char er_nostatement[] = "No statement. PREPARE must be executed first";

const char er_badselect[] = "SELECT statement expected";

const char er_norowset[] = "No such rowset";

const char er_nofield[] = "No field";

const char er_typemismatch[] = "RDBMS and Clipper types are incompatible or RDBMS type is not supported";

const char er_badstatement[] = "Non SELECT statement expected";

const char er_orderexist[] = "Order exists";

const char er_noorder[] = "No order";

const char er_badorderlen[] = "Bad length of order key value";

const char er_badordertype[] = "Unsupported type of order key value";

const char er_badorders[] = "Bad orders";

const char er_internal[] = "Internal error (no row)";

#include "dbfsql_c/static_sql_fillorders.h"

#include "dbfsql_c/clip_SQLLIST.h"
#include "dbfsql_c/clip_SQLCREATECONN.h"
#include "dbfsql_c/clip_SQLDESTROYCONN.h"
#include "dbfsql_c/clip_SQLPREPARE.h"
#include "dbfsql_c/clip_SQLFREESTMT.h"
#include "dbfsql_c/clip_SQLCOMMAND.h"
#include "dbfsql_c/clip_SQLTESTPARSER.h"
#include "dbfsql_c/_sql_fetch.h"
#include "dbfsql_c/clip_SQLCREATEROWSET.h"
#include "dbfsql_c/clip_SQLGETHOT.h"
#include "dbfsql_c/clip_SQLSETHOT.h"
#include "dbfsql_c/clip_SQLFILLORDERS.h"
#include "dbfsql_c/clip_SQLDESTROYROWSET.h"
#include "dbfsql_c/clip_SQLGOTOP.h"
#include "dbfsql_c/clip_SQLGOBOTTOM.h"
#include "dbfsql_c/static_sql_searchtree.h"
#include "dbfsql_c/clip_SQLSKIP.h"
#include "dbfsql_c/clip_SQLGOTO.h"
#include "dbfsql_c/clip_SQLBOF.h"
#include "dbfsql_c/clip_SQLEOF.h"
#include "dbfsql_c/clip_SQLRECNO.h"
#include "dbfsql_c/clip_SQLLASTREC.h"
#include "dbfsql_c/clip_SQLGETVALUE.h"
#include "dbfsql_c/static_sql_orderdel.h"
#include "dbfsql_c/clip_SQLSETVALUE.h"
#include "dbfsql_c/clip_SQLADDKEYS.h"
#include "dbfsql_c/clip_SQLAPPEND.h"
#include "dbfsql_c/clip_SQLDELETE.h"
#include "dbfsql_c/clip_SQLNFIELDS.h"
#include "dbfsql_c/clip_SQLFIELDNO.h"
#include "dbfsql_c/clip_SQLFIELDNAME.h"
#include "dbfsql_c/clip_SQLFIELDTYPE.h"
#include "dbfsql_c/clip_SQLFIELDTYPESQL.h"
#include "dbfsql_c/clip_SQLFIELDNULLABLE.h"
#include "dbfsql_c/clip_SQLFIELDUNSIGNED.h"
#include "dbfsql_c/clip_SQLFIELDBINARY.h"
#include "dbfsql_c/clip_SQLFIELDLEN.h"
#include "dbfsql_c/clip_SQLFIELDDEC.h"
#include "dbfsql_c/clip_SQLROWID.h"
#include "dbfsql_c/clip_SQLNEWID.h"
#include "dbfsql_c/clip_SQLSETLENDEC.h"
#include "dbfsql_c/clip_STRFTIME.h"
#include "dbfsql_c/static_SQLLocale_koi_locale.h"
#include "dbfsql_c/typedef_struct_LocData.h"
#include "dbfsql_c/static_cmp_locale.h"
#include "dbfsql_c/SQL_get_locale.h"
#include "dbfsql_c/clip_SQLLOCREAD.h"
#include "dbfsql_c/clip_SQLLOCWRITE.h"
#include "dbfsql_c/clip_SQLREFRESH.h"
#include "dbfsql_c/sql_createorder.h"
#include "dbfsql_c/static_sql_cmp.h"
#include "dbfsql_c/static__sql_char_compare.h"
#include "dbfsql_c/static__sql_num_compare.h"
#include "dbfsql_c/static__sql_date_compare.h"
#include "dbfsql_c/static__sql_dt_compare.h"
#include "dbfsql_c/sql_orderadd.h"
#include "dbfsql_c/clip_SQLCREATEORDER.h"
#include "dbfsql_c/sql_seek.h"
#include "dbfsql_c/clip_SQLSEEK.h"
#include "dbfsql_c/clip_SQLSETORDER.h"
#include "dbfsql_c/clip_SQLGENID.h"
#include "dbfsql_c/clip_SQLSTART.h"
#include "dbfsql_c/clip_SQLCOMMIT.h"
#include "dbfsql_c/clip_SQLROLLBACK.h"
#include "dbfsql_c/clip_SQLFETCH.h"
#include "dbfsql_c/clip_SQLFETCHED.h"
#include "dbfsql_c/clip_SQLKEYNO.h"

/* ***************************************************************************** */
/*
  Some functionality for BLOB's
*/
/* *************************************************************************** */
#include "dbfsql_c/clip_SQLBLOBOPEN.h"

/* *************************************************************************** */
#include "dbfsql_c/clip_SQLBLOBCREATE.h"

/* *************************************************************************** */
#include "dbfsql_c/clip_SQLBLOBIMPORT.h"

/* *************************************************************************** */
#include "dbfsql_c/clip_SQLBLOBEXPORT.h"

/* *************************************************************************** */
#include "dbfsql_c/clip_SQLBLOBWRITE.h"

/* *************************************************************************** */

#include "dbfsql_c/clip_SQLBLOBREAD.h"

/* *************************************************************************** */

#include "dbfsql_c/clip_SQLBLOBSEEK.h"

/* *************************************************************************** */
#include "dbfsql_c/clip_SQLBLOBTELL.h"

/* *************************************************************************** */
#include "dbfsql_c/clip_SQLBLOBCLOSE.h"

/* *************************************************************************** */
#include "dbfsql_c/clip_SQLBLOBUNLINK.h"

/* *************************************************************************** */
