typedef struct C_FILE
{
   int type_of_C_FILE;		/* type of opened file, see fileio.ch:FT_* */
   int fileno_of_C_FILE;	/* real file or socket number */
   FILE *file_of_C_FILE;	/* STDIO lib buffer */
   pid_t pid_of_C_FILE;		/* pid for pipe children process */
   int stat_of_C_FILE;		/* see fileio.ch:FS_* flags */
   int timeout_of_C_FILE;	/* default timeout for eoperations */
   long hash_of_C_FILE;		/* hash code for the full file name (with path) */
   struct ClipMachine *ClipMachineMemory_of_C_FILE;
} C_FILE;
