typedef struct _RDD_MEMO_VTBL_
{
   char id[4];
   char suff[5];
   char desc[81];
   int version;
   int (*create) (ClipMachine * ClipMachineMemory, char *name, const char *__PROC__);
   int (*pack) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_MEMO * rm, int tmpfd, int bsize, ClipVar * block, int step, const char *__PROC__);
   int (*zap) (ClipMachine * ClipMachineMemory, RDD_MEMO * rm, const char *__PROC__);
   int (*open) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_MEMO * rm, const char *__PROC__);
   int (*close) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_MEMO * rm, const char *__PROC__);
   int (*getvalue) (ClipMachine * ClipMachineMemory, RDD_MEMO * rm, int id, ClipVar * vp, const char *__PROC__);
   int (*setvalue) (ClipMachine * ClipMachineMemory, RDD_MEMO * rm, int *id, ClipVar * vp, int binary, const char *__PROC__);
   int (*getvchar) (ClipMachine * ClipMachineMemory, RDD_MEMO * rm, int len, unsigned int id, char *buf, const char *__PROC__);
   int (*setvchar) (ClipMachine * ClipMachineMemory, RDD_MEMO * rm, int len, int oldlen, unsigned int *id, char *buf, const char *__PROC__);
   int (*getvalue64) (ClipMachine * ClipMachineMemory, RDD_MEMO * rm, u8 id, ClipVar * vp, const char *__PROC__);
   int (*setvalue64) (ClipMachine * ClipMachineMemory, RDD_MEMO * rm, u8 * id, ClipVar * vp, int binary, const char *__PROC__);
   int (*getvchar64) (ClipMachine * ClipMachineMemory, RDD_MEMO * rm, int len, u8 id, char *buf, const char *__PROC__);
   int (*setvchar64) (ClipMachine * ClipMachineMemory, RDD_MEMO * rm, int len, int oldlen, u8 * id, char *buf, const char *__PROC__);
   int (*info) (ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_MEMO * rm, int cmd, const char *__PROC__);
} RDD_MEMO_VTBL;
