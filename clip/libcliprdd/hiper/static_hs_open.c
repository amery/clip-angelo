static int
hs_open(ClipMachine * ClipMachineMemory, const char *fname, int mode, HIPER ** hsp, const char *__PROC__)
{
   HIPER *hs = calloc(1, sizeof(HIPER));

   HS_HEADER hdr;

   int shared = 0;

   int rdwr = 0;

   struct stat st;

   int er;

   if (mode < 0 || mode > 3)
      mode = 2;
   if (mode == 0 || mode == 2)
      shared = 1;
   if (mode == 0 || mode == 1)
      rdwr = 1;

   if ((er = _rdd_parsepath(ClipMachineMemory, fname, ".htx", &hs->path, &hs->name, EG_OPEN, __PROC__)))
      goto err;

#ifdef _WIN32
   hs->file.fd = open(hs->path, (rdwr ? O_RDWR : O_RDONLY) | O_BINARY);
#else
   hs->file.fd = open(hs->path, rdwr ? O_RDWR : O_RDONLY);
#endif
   if (hs->file.fd == -1)
      goto err_open;
   if (fstat(hs->file.fd, &st) == -1)
      goto err_open;
#ifdef HAVE_MMAN_H
   hs->file.mapsize = st.st_size;
   hs->file.md = (caddr_t) mmap(0, hs->file.mapsize, PROT_READ | (rdwr ? PROT_WRITE : 0), MAP_SHARED, hs->file.fd, 0);
   /* If failed use non-mapped file (md==-1) */
#endif

   if ((er = rdd_read(ClipMachineMemory, &hs->file, 0, sizeof(HS_HEADER), &hdr, __PROC__)))
      goto err;
   if (memcmp(hdr.sig, "HS", 2) != 0)
   {
      er = rdd_err(ClipMachineMemory, EG_OPEN, 0, __FILE__, __LINE__, __PROC__, er_badsig);
      goto err;
   }
   hs->lcase = hdr.lcase;
   hs->filtset = hdr.filtset;
   hs->pagesize = _rdd_ushort(hdr.pagesize);
   hs->shared = shared;
   hs->intsonpage = hs->pagesize / 4;

   *hsp = hs;
   return 0;
 err_open:
   er = rdd_err(ClipMachineMemory, EG_OPEN, errno, __FILE__, __LINE__, __PROC__, hs->path);
 err:
#ifdef HAVE_MMAN_H
   if (hs->file.md)
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
