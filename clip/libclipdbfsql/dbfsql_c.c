#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <ci_clip.h>
#include <ci_coll.h>
#include <../libclipscreen/ci_charset.h>
#include <ci_dbfsql.h>
#include <ci_error.ch>

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

#include <dbfsql_c/static_sql_fillorders.c>

#include <dbfsql_c/clip_SQLLIST.c>
#include <dbfsql_c/clip_SQLCREATECONN.c>
#include <dbfsql_c/clip_SQLDESTROYCONN.c>
#include <dbfsql_c/clip_SQLPREPARE.c>
#include <dbfsql_c/clip_SQLFREESTMT.c>
#include <dbfsql_c/clip_SQLCOMMAND.c>
#include <dbfsql_c/clip_SQLTESTPARSER.c>
#include <dbfsql_c/_sql_fetch.c>
#include <dbfsql_c/clip_SQLCREATEROWSET.c>
#include <dbfsql_c/clip_SQLGETHOT.c>
#include <dbfsql_c/clip_SQLSETHOT.c>
#include <dbfsql_c/clip_SQLFILLORDERS.c>
#include <dbfsql_c/clip_SQLDESTROYROWSET.c>
#include <dbfsql_c/clip_SQLGOTOP.c>
#include <dbfsql_c/clip_SQLGOBOTTOM.c>
#include <dbfsql_c/static_sql_searchtree.c>
#include <dbfsql_c/clip_SQLSKIP.c>
#include <dbfsql_c/clip_SQLGOTO.c>
#include <dbfsql_c/clip_SQLBOF.c>
#include <dbfsql_c/clip_SQLEOF.c>
#include <dbfsql_c/clip_SQLRECNO.c>
#include <dbfsql_c/clip_SQLLASTREC.c>
#include <dbfsql_c/clip_SQLGETVALUE.c>
#include <dbfsql_c/static_sql_orderdel.c>
#include <dbfsql_c/clip_SQLSETVALUE.c>
#include <dbfsql_c/clip_SQLADDKEYS.c>
#include <dbfsql_c/clip_SQLAPPEND.c>
#include <dbfsql_c/clip_SQLDELETE.c>
#include <dbfsql_c/clip_SQLNFIELDS.c>
#include <dbfsql_c/clip_SQLFIELDNO.c>
#include <dbfsql_c/clip_SQLFIELDNAME.c>
#include <dbfsql_c/clip_SQLFIELDTYPE.c>
#include <dbfsql_c/clip_SQLFIELDTYPESQL.c>
#include <dbfsql_c/clip_SQLFIELDNULLABLE.c>
#include <dbfsql_c/clip_SQLFIELDUNSIGNED.c>
#include <dbfsql_c/clip_SQLFIELDBINARY.c>
#include <dbfsql_c/clip_SQLFIELDLEN.c>
#include <dbfsql_c/clip_SQLFIELDDEC.c>
#include <dbfsql_c/clip_SQLROWID.c>
#include <dbfsql_c/clip_SQLNEWID.c>
#include <dbfsql_c/clip_SQLSETLENDEC.c>
#include <dbfsql_c/clip_STRFTIME.c>
#include <dbfsql_c/static_SQLLocale_koi_locale.c>
#include <dbfsql_c/typedef_struct_LocData.h>
#include <dbfsql_c/static_cmp_locale.c>
#include <dbfsql_c/SQL_get_locale.c>
#include <dbfsql_c/clip_SQLLOCREAD.c>
#include <dbfsql_c/clip_SQLLOCWRITE.c>
#include <dbfsql_c/clip_SQLREFRESH.c>
#include <dbfsql_c/sql_createorder.c>
#include <dbfsql_c/static_sql_cmp.c>
#include <dbfsql_c/static__sql_char_compare.c>
#include <dbfsql_c/static__sql_num_compare.c>
#include <dbfsql_c/static__sql_date_compare.c>
#include <dbfsql_c/static__sql_dt_compare.c>
#include <dbfsql_c/sql_orderadd.c>
#include <dbfsql_c/clip_SQLCREATEORDER.c>
#include <dbfsql_c/sql_seek.c>
#include <dbfsql_c/clip_SQLSEEK.c>
#include <dbfsql_c/clip_SQLSETORDER.c>
#include <dbfsql_c/clip_SQLGENID.c>
#include <dbfsql_c/clip_SQLSTART.c>
#include <dbfsql_c/clip_SQLCOMMIT.c>
#include <dbfsql_c/clip_SQLROLLBACK.c>
#include <dbfsql_c/clip_SQLFETCH.c>
#include <dbfsql_c/clip_SQLFETCHED.c>
#include <dbfsql_c/clip_SQLKEYNO.c>

/* ***************************************************************************** */
/*
  Some functionality for BLOB's
*/
/* *************************************************************************** */
#include <dbfsql_c/clip_SQLBLOBOPEN.c>

/* *************************************************************************** */
#include <dbfsql_c/clip_SQLBLOBCREATE.c>

/* *************************************************************************** */
#include <dbfsql_c/clip_SQLBLOBIMPORT.c>

/* *************************************************************************** */
#include <dbfsql_c/clip_SQLBLOBEXPORT.c>

/* *************************************************************************** */
#include <dbfsql_c/clip_SQLBLOBWRITE.c>

/* *************************************************************************** */

#include <dbfsql_c/clip_SQLBLOBREAD.c>

/* *************************************************************************** */

#include <dbfsql_c/clip_SQLBLOBSEEK.c>

/* *************************************************************************** */
#include <dbfsql_c/clip_SQLBLOBTELL.c>

/* *************************************************************************** */
#include <dbfsql_c/clip_SQLBLOBCLOSE.c>

/* *************************************************************************** */
#include <dbfsql_c/clip_SQLBLOBUNLINK.c>

/* *************************************************************************** */
