static int
dbt_create(ClipMachine * ClipMachineMemory, char *name, const char *__PROC__)
{
   RDD_FILE file;

   DBT_HEADER hdr;

   int er;

   memset(&hdr, 0, sizeof(DBT_HEADER));
   _rdd_put_uint((unsigned char *) hdr.fuu, 1);
   hdr.version = 0x50;

   memset(&file, 0, sizeof(RDD_FILE));
   file.md = (char *) -1;
#ifdef OS_CYGWIN
   file.fd = open(name, O_CREAT | O_TRUNC | O_RDWR | O_BINARY, ClipMachineMemory->fileCreateMode);
#else
   file.fd = open(name, O_CREAT | O_TRUNC | O_RDWR, ClipMachineMemory->fileCreateMode);
#endif
   if (file.fd == -1)
      goto err;
   if ((er = rdd_write(ClipMachineMemory, &file, 0, sizeof(DBT_HEADER), (char *) &hdr, __PROC__)))
      return er;
   if (close(file.fd) == -1)
      goto err;
   return 0;
 err:
   return rdd_err(ClipMachineMemory, EG_CREATE, errno, __FILE__, __LINE__, __PROC__, name);
}
