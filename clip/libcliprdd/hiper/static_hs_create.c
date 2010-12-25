static int
hs_create(ClipMachine * ClipMachineMemory, const char *fname, int pagesize, int lcase, int filtset, HIPER ** hsp, const char *__PROC__)
{
   HIPER *hs = calloc(1, sizeof(HIPER));

   HS_HEADER hdr;

   unsigned char pairs[0x10000 * 4];

   int er;

   hs->file.fd = -1;
   if (filtset == 1)
      hs->filtset = 1;		/* text */
   else
      hs->filtset = 2;		/* binary */
   hs->lcase = ((filtset == 1) ? lcase : 0);
   hs->pagesize = pagesize;
   hs->shared = 0;
   hs->intsonpage = hs->pagesize / 4;

   memset(&hdr, 0, sizeof(HS_HEADER));
   memcpy(hdr.sig, "HS", 2);
   hdr.lcase = hs->lcase;
   hdr.filtset = hs->filtset;
   _rdd_put_ushort(hdr.pagesize, hs->pagesize);

   if ((er = _rdd_parsepath(ClipMachineMemory, fname, ".htx", &hs->path, &hs->name, EG_CREATE, __PROC__)))
      goto err;

#ifdef _WIN32
   hs->file.fd = open(hs->path, O_CREAT | O_TRUNC | O_RDWR | O_BINARY, ClipMachineMemory->fileCreateMode);
#else
   hs->file.fd = open(hs->path, O_CREAT | O_TRUNC | O_RDWR, ClipMachineMemory->fileCreateMode);
#endif
   if (hs->file.fd == -1)
      goto err_create;
   hs->file.mapsize = sizeof(HS_HEADER);
   if (lseek(hs->file.fd, hs->file.mapsize - 1, SEEK_SET) == -1)
      goto err_create;
   if (write(hs->file.fd, "", 1) == -1)
      goto err_create;
#ifdef HAVE_MMAN_H
   hs->file.md = (caddr_t) mmap(0, hs->file.mapsize, PROT_READ | PROT_WRITE, MAP_SHARED, hs->file.fd, 0);
   /* If failed use non-mapped file (md==-1) */
#endif
   if ((er = rdd_write(ClipMachineMemory, &hs->file, 0, sizeof(HS_HEADER), &hdr, __PROC__)))
      goto err;

   memset(pairs, 0, sizeof(pairs));
   if ((er = rdd_write(ClipMachineMemory, &hs->file, sizeof(HS_HEADER), sizeof(pairs), pairs, __PROC__)))
      goto err;

   *hsp = hs;
   return 0;
 err_create:
   er = rdd_err(ClipMachineMemory, EG_CREATE, errno, __FILE__, __LINE__, __PROC__, hs->path);
 err:
   if (hs->file.md)
#ifdef OS_MINGW
      free(hs->file.md);
#else
      munmap(hs->file.md, hs->file.mapsize);
#endif
   if (hs->file.fd != -1)
      close(hs->file.fd);
   if (hs->name)
      free(hs->name);
   if (hs->path)
      free(hs->path);
   free(hs);
   return er;
}
