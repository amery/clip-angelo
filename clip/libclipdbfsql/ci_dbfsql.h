#include "ci_hash.h"
#include "ci_btree.h"

#ifndef CI_DBFSQL_H
#define CI_DBFSQL_H

/* Errors (e:subCode) */
#define ER_NOLIB		1000
#define ER_CONNECT      1001
#define ER_NOCONNECT    1002
#define ER_NOSQL        1003
#define ER_NOSTATEMENT  1004
#define ER_BADSTATEMENT 1005
#define ER_BADSELECT    1006
#define ER_NOROWSET     1007
#define ER_NOFIELD      1008
#define ER_TYPEMISMATCH 1009
#define ER_RDBMSERROR   1010
#define ER_ORDEREXIST   1011
#define ER_NOORDER      1012
#define ER_BADORDERLEN  1013
#define ER_BADORDERTYPE 1014
#define ER_START        1015
#define ER_COMMIT       1016
#define ER_ROLLBACK     1017
#define ER_FETCH        1018

#define ER_INTERNAL		1100

#define MAXFIELDNAME	20	// (without terminating zero)

#define SQLBLOB_SEEKTOP		1
#define SQLBLOB_SEEKCURRENT	2
#define SQLBLOB_SEEKBOTTOM	3

#ifndef min
#define min(a,b) (a)<(b)?(a):(b)
#endif
#ifndef max
#define max(a,b) (a)<(b)?(a):(b)
#endif

struct tagSQLVTBL;

typedef struct
{
   unsigned char cmp[128];
   unsigned char upper[128];
   unsigned char lower[128];
   unsigned char read[128];
   unsigned char write[128];
}
SQLLocale;

typedef struct tagSQLFIELD
{
   char name[MAXFIELDNAME + 1];
   int type;
   int len;
   int dec;
   unsigned int ops;
   int buflen;
   int unsign;
   int notnull;
   int binary;
   char ctype[2];
   void *cargo;
} SQLFIELD;

typedef struct tagSQLSTMT
{
   int stmt_item;
   struct tagSQLCONN *conn;
} SQLSTMT;

struct tagSQLORDER;
typedef struct tagSQLROWSET
{
   int rowset_item;
   struct tagSQLCONN *conn;
   struct tagSQLSTMT *stmt;
   int recno;
   int lastrec;
   int loaded;
   int unknownrows;
   int done;
   int bof;
   int eof;
   int nfields;
   SQLFIELD *fields;
   int id;
   int nids;
   int *ids;
   HashTable *orders;
   long *taghashes;
   int ntags;
   BTREE *bt;
   struct tagSQLORDER *curord;
   int hot;
   int newrec;
} SQLROWSET;

typedef struct tagSQLCONN
{
   struct tagSQLVTBL *vtbl;
   SQLLocale *loc;
   int at;
} SQLCONN;

typedef struct tagSQLORDER
{
   char *name;
   SQLLocale *loc;
   ClipVar *block;
   ClipVar *rmap;
   BTREE *bt;
   int len;
} SQLORDER;

typedef struct tagSQLVTBL
{
   int sizeof_rowset;
   void (*destroyconn) (SQLCONN * conn);
   int (*prepare) (ClipMachine * ClipMachineMemory, SQLCONN * conn, char *sql);
   int (*command) (ClipMachine * ClipMachineMemory, SQLSTMT * stmt, ClipVar * ap);
   int (*createrowset) (ClipMachine * ClipMachineMemory, SQLROWSET * rs, ClipVar * ap, ClipVar * idname, const char *gen_idSQL);
   char *(*testparser) (ClipMachine * ClipMachineMemory, char *sql, ClipVar * ap);
   char *(*getvalue) (SQLROWSET * rowset, int fieldno, int *len);
   void (*setvalue) (SQLROWSET * rowset, int fieldno, char *value, int len);
   void (*append) (SQLROWSET * rowset);
   void (*delete_) (SQLROWSET * rowset);
   void (*newid) (ClipMachine * ClipMachineMemory, SQLSTMT * stmt);
   int (*refresh) (ClipMachine * ClipMachineMemory, SQLROWSET * rowset, SQLSTMT * stmt, ClipVar * ap);
   int (*genid) (ClipMachine * ClipMachineMemory, SQLROWSET * rowset);
   int (*start) (ClipMachine * ClipMachineMemory, SQLCONN * conn, const char *p1, const char *p2);
   int (*commit) (ClipMachine * ClipMachineMemory, SQLCONN * conn);
   int (*rollback) (ClipMachine * ClipMachineMemory, SQLCONN * conn);
   int (*fetch) (ClipMachine * ClipMachineMemory, SQLROWSET * rs, int recs, ClipVar * eval, int every, ClipVar * ors);

/*
  Some functionality for BLOB's
*/
   int (*blob_create) (ClipMachine * ClipMachineMemory, SQLCONN * conn, unsigned int OID);
   int (*blob_import) (ClipMachine * ClipMachineMemory, SQLCONN * conn, const char *filename);
   int (*blob_export) (ClipMachine * ClipMachineMemory, SQLCONN * conn, unsigned int OID, const char *filename);
   int (*blob_open) (ClipMachine * ClipMachineMemory, SQLCONN * conn, unsigned int OID, int mode);
   int (*blob_write) (ClipMachine * ClipMachineMemory, SQLCONN * conn, int oid_fd, const char *buffer, int length);
   int (*blob_read) (ClipMachine * ClipMachineMemory, SQLCONN * conn, int oid_fd, char *buffer, int length);
   int (*blob_seek) (ClipMachine * ClipMachineMemory, SQLCONN * conn, int oid_fd, int offset, int whence);
   int (*blob_tell) (ClipMachine * ClipMachineMemory, SQLCONN * conn, int oid_fd);
   int (*blob_close) (ClipMachine * ClipMachineMemory, SQLCONN * conn, int oid_fd);
   int (*blob_unlink) (ClipMachine * ClipMachineMemory, SQLCONN * conn, unsigned int OID);
} SQLVTBL;

SQLLocale *SQL_get_locale(ClipMachine * ClipMachineMemory, const char *sqlcs);
int sql_createorder(ClipMachine * ClipMachineMemory, ClipVar * rmap, SQLROWSET * rowset, char *tagname, ClipVar * expr, int len);
int sql_orderadd(ClipMachine * ClipMachineMemory, SQLROWSET * rowset, long taghash);

#endif
