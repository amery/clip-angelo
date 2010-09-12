typedef struct _RDD_INDEX_VTBL_
{
   char id[6];
   char suff[5];
   char sing_suff[5];
   char desc[81];
   int version;
   char ismulti;		/* is multi orderal RDD */
   /* open/close */
   int (*open) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *tag, const char *__PROC__);
   int (*close) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__);
   int (*create) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri,
		  RDD_ORDER ** rop, const char *tag, const char *expr, ClipVar * block, int unique, int first, unsigned int header, const char *__PROC__);
   int (*reindex) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__);
   int (*zap) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__);
   /* navigation */
   int (*gotop) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__);
   int (*gobottom) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__);
   int (*next) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__);
   int (*prev) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__);
   int (*seek) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, ClipVar * v, int soft, int last, int *found, const char *__PROC__);
   int (*gotokey) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, unsigned int keyno, int *ok, const char *__PROC__);
   int (*descend) (ClipMachine * ClipMachineMemory, RDD_ORDER * ro, int descend, int *res, const char *__PROC__);
   int (*buildtree) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__);
   int (*wildskip) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *pattern, int regular, const char *s, ClipVar * block, int *found, const char *__PROC__);
   /* update */
   int (*addkey) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, ClipVar * v, const char *__PROC__);
   int (*delkey) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__);
   int (*destroy) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__);
   /* format key */
   int (*formatkey) (ClipMachine * ClipMachineMemory, RDD_ORDER * ro, ClipVar * v, void *res, const char *__PROC__);
   /* info */
   int (*keyno) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, int *keyno, const char *__PROC__);
   int (*lastkey) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, int *lastkey, const char *__PROC__);
   int (*info) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, int cmd, const char *__PROC__);
   int (*keyvalue) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, ClipVar * v, const char *__PROC__);
   /* rushmore */
   int (*setscope) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, ClipVar * top, ClipVar * bottom, unsigned int *map, int bytes, int exact, const char *__PROC__);
   /* Independed Indexes */
   int (*ii_create) (ClipMachine * ClipMachineMemory, RDD_INDEX * ri, const char *__PROC__);
   int (*ii_createtag) (ClipMachine * ClipMachineMemory, RDD_INDEX * ri, const char *tag, const char *expr, RDD_ORDER ** rop, const char *__PROC__);
   int (*ii_open) (ClipMachine * ClipMachineMemory, RDD_INDEX * ri, const char *__PROC__);
   int (*ii_opentag) (ClipMachine * ClipMachineMemory, RDD_INDEX * ri, const char *tag, RDD_ORDER ** rop, const char *__PROC__);
   int (*ii_addkey) (ClipMachine * ClipMachineMemory, RDD_ORDER * ro, const char *id, ClipVar * key, const char *__PROC__);
   int (*ii_delkey) (ClipMachine * ClipMachineMemory, RDD_ORDER * ro, const char *__PROC__);
   int (*ii_gotop) (ClipMachine * ClipMachineMemory, RDD_ORDER * ro, const char *__PROC__);
   int (*ii_gobottom) (ClipMachine * ClipMachineMemory, RDD_ORDER * ro, const char *__PROC__);
   int (*ii_id) (ClipMachine * ClipMachineMemory, RDD_ORDER * ro, ClipVar * v, const char *__PROC__);
   int (*ii_key) (ClipMachine * ClipMachineMemory, RDD_ORDER * ro, ClipVar * v, const char *__PROC__);
   int (*ii_next) (ClipMachine * ClipMachineMemory, RDD_ORDER * ro, const char *__PROC__);
   int (*ii_prev) (ClipMachine * ClipMachineMemory, RDD_ORDER * ro, const char *__PROC__);
   /* for filter optimization */
   int (*calcfiltlist) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, RDD_FILTER * fp, const char *__PROC__);
   /* index locks */
   int (*_rlock) (ClipMachine * ClipMachineMemory, RDD_ORDER * ro, const char *__PROC__);
   int (*_wlock) (ClipMachine * ClipMachineMemory, RDD_ORDER * ro, const char *__PROC__);
   int (*_ulock) (ClipMachine * ClipMachineMemory, RDD_ORDER * ro, const char *__PROC__);
} RDD_INDEX_VTBL;
