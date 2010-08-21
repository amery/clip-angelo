/*
	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI
*/
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ci_rdd.h>
#include <ci_error.ch>
#ifdef HAVE_MMAN_H
#include <sys/mman.h>
#endif

extern DBWorkArea *cur_area(ClipMachine * ClipMachineMemory);

#include <hiper/typedef_struct_HS_HEADER.h>
#include <hiper/typedef_struct_HIPER.h>

static int _hs_add(ClipMachine * ClipMachineMemory, HIPER * hs, const char *str, int len, unsigned int rec, const char *__PROC__);

static int hs_add(ClipMachine * ClipMachineMemory, HIPER * hs, const char *str, int len, unsigned int *rno, const char *__PROC__);

static int hs_close(ClipMachine * ClipMachineMemory, HIPER * hs, const char *__PROC__);

static int hs_create(ClipMachine * ClipMachineMemory, const char *fname, int pagesize, int lcase, int filtset, HIPER ** hsp, const char *__PROC__);

static int hs_filter(ClipMachine * ClipMachineMemory, HIPER * hs, const char *val, int len, unsigned int **map, unsigned int lastrec, unsigned int *count, const char *__PROC__);

static int hs_open(ClipMachine * ClipMachineMemory, const char *fname, int mode, HIPER ** hsp, const char *__PROC__);

static int hs_remove(ClipMachine * ClipMachineMemory, HIPER * hs, unsigned int rec, const char *__PROC__);

static void destroy_hiper(void *);

static const char *er_badhiper = "Bad HiPer handle";

static const char *er_baddata = "Expression returns non-character value";

static const char *er_ioerror = "I/O error";

static const char *er_badsig = "Bad HTX file (wrong signature)";

#include <hiper/clip_HS_ADD.c>
#include <hiper/clip_HS_CLOSE.c>
#include <hiper/clip_HS_CREATE.c>
#include <hiper/clip_HS_DELETE.c>
#include <hiper/clip_HS_FILTER.c>
#include <hiper/clip_HS_IFDEL.c>
#include <hiper/clip_HS_INDEX.c>
#include <hiper/clip_HS_KEYCOUNT.c>
#include <hiper/clip_HS_NEXT.c>
#include <hiper/clip_HS_REMOVE.c>
#include <hiper/clip_HS_REPLACE.c>
#include <hiper/clip_HS_SET.c>
#include <hiper/clip_HS_UNDELETE.c>
#include <hiper/clip_HS_VERIFY.c>
#include <hiper/static__hs_getfuu.c>
#include <hiper/static__hs_loadbits.c>
#include <hiper/static__hs_tobitstyle.c>
#include <hiper/static__hs_compare.c>
#include <hiper/static__hs_insert.c>
#include <hiper/static__hs_setpair.c>
#include <hiper/static__hs_addpair.c>
#include <hiper/static__hs_delete.c>
#include <hiper/static_destroy_hiper.c>
#include <hiper/static__hs_add.c>
#include <hiper/static_hs_add.c>
#include <hiper/static_hs_close.c>
#include <hiper/static_hs_create.c>
#include <hiper/static_hs_filter.c>
#include <hiper/static_hs_open.c>
#include <hiper/static_hs_remove.c>
