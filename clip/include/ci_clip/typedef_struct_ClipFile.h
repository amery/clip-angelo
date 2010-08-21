typedef struct ClipFile
{
   int refCount_of_ClipFile;
   char *body_of_ClipFile;
   char *mem_of_ClipFile;
   long bodySize_of_ClipFile;
   int bodyMem_of_ClipFile;	/* 0 -> mmap, 1 -> malloc, 3 -> static mem */
   char *name_of_ClipFile;
   union ClipVar *ClipVar_statics_of_ClipFile;
   int staticsMem_of_ClipFile;
   int nstatics_of_ClipFile;
   long *pos_of_ClipFile;
   long *end_of_ClipFile;
   void *cargo_of_ClipFile;
   ClipHashNames *hash_names_of_ClipFile;
   ClipVarDef *ClipVarDef_staticDefs_of_ClipFile;
   int fd_of_ClipFile;
}
ClipFile;
