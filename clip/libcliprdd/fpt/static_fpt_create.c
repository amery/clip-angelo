static int
fpt_create(ClipMachine * ClipMachineMemory, char *name, const char *__PROC__)
{
   RDD_FILE file;

   FPT_HEADER hdr;

   int fuu, er;

   FPT_HEADER dum;

   memset(&hdr, 0, sizeof(FPT_HEADER));
   memset(&dum, 0, sizeof(FPT_HEADER));
   fuu = sizeof(FPT_HEADER) / ClipMachineMemory->mblocksize;
   fuu += (fuu * ClipMachineMemory->mblocksize < sizeof(FPT_HEADER)) ? 1 : 0;
   _rdd_put_backuint((unsigned char *) hdr.fuu, fuu);
   _rdd_put_backushort((unsigned char *) hdr.blocksize, ClipMachineMemory->mblocksize);
   strcpy(hdr.sig, "FlexFile3");
   strcpy(hdr.sig0, "Made by CLIP1");
   hdr.sig[9] = 3;
   _rdd_put_uint((unsigned char *) hdr.sig1, 0x10);

   memset(&file, 0, sizeof(RDD_FILE));
   file.md = (char *) -1;
#ifdef _WIN32
   file.fd = open(name, O_CREAT | O_TRUNC | O_RDWR | O_BINARY, ClipMachineMemory->fileCreateMode);
#else
   file.fd = open(name, O_CREAT | O_TRUNC | O_RDWR, ClipMachineMemory->fileCreateMode);
#endif
   if (file.fd == -1)
      goto err;
   if ((er = rdd_write(ClipMachineMemory, &file, 0, sizeof(FPT_HEADER), &hdr, __PROC__)))
      return er;
   if ((er = rdd_write(ClipMachineMemory, &file, sizeof(FPT_HEADER), fuu * ClipMachineMemory->mblocksize - sizeof(FPT_HEADER), &dum, __PROC__)))
      return er;
   if (close(file.fd) == -1)
      goto err;
   return 0;
 err:
   return rdd_err(ClipMachineMemory, EG_CREATE, errno, __FILE__, __LINE__, __PROC__, name);
}
