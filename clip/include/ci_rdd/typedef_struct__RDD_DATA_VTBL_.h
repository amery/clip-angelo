typedef struct _RDD_DATA_VTBL_
{
   char      id[4];
   char      suff[5];
   char      desc[81];
   int       version;
   unsigned char dbfsig;
   unsigned char dbfwithmemosig;
  /* open/close */
   int       (*open) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);
   int       (*close) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);
  /* create */
   int       (*create) (ClipMachine * ClipMachineMemory, struct _RDD_DATA_VTBL_ * vtbl,
			char *name, RDD_STRUCT * rddstru, int nfields, const char *__PROC__);
   int       (*pack) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, int tfd, const char *__PROC__);
   int       (*zap) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);
  /* info */
   int       (*lastrec) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *lastrec, const char *__PROC__);
   int       (*deleted) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *deleted, const char *__PROC__);
   int       (*lupdate) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);
   int       (*setstruct) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);
  /* navigation */
   int       (*gotop) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);
   int       (*gobottom) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);
   int       (*next) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, int filtok, const char *__PROC__);
   int       (*prev) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);
   int       (*go) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, unsigned int rec, const char *__PROC__);
   int       (*rawgo) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, unsigned int rec,
		       char valid_stack, const char *__PROC__);
  /* locking */
   int       (*rlock) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, unsigned int rec, int *r, const char *__PROC__);
   int       (*flock) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *r, const char *__PROC__);
   int       (*ulock) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, unsigned int rec, int flock, const char *__PROC__);
   int       (*rlocked) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, unsigned int rec, int *r, const char *__PROC__);
   int       (*forlock) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, unsigned int rec, int *r, const char *__PROC__);
  /* low level locking */
   int       (*_wlock) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);
   int       (*_rlock) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);
   int       (*_ulock) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);
  /* data */
   int       (*append) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);
   int       (*getrecord) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);
   int       (*getvalue) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, int no, ClipVar * vp, const char *__PROC__);
   int       (*setrecord) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);
   int       (*setvalue) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, int no,
			  ClipVar * vp, int binary, const char *__PROC__);
   int       (*getmemoid) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, int no,
			   unsigned int *memoid, unsigned short int *vlen, const char *__PROC__);
   int       (*setmemoid) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, int no, unsigned int memoid, const char *__PROC__);
   int       (*delete) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);
   int       (*recall) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);
   int       (*info) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, int cmd, const char *__PROC__);
   int       (*verify) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *r, const char *__PROC__);
  /* for filter optimization */
   int       (*calcfiltlist) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_FILTER * fp, const char *__PROC__);
} RDD_DATA_VTBL;
