int
rdd_usearea(ClipMachine * ClipMachineMemory, const char *driver, const char *name, int shared, int readonly, int tempo, RDD_DATA ** rdp, const char *__PROC__)
{
   RDD_DATA *rd = (RDD_DATA *) calloc(1, sizeof(RDD_DATA));

   struct stat st;

   int er = EG_UNSUPPORTED;

   rd->file.fd = -1;
   rd->file.md = (char *) -1;
   rd->locate_next = -1;
   rd->ClipMachineMemory = ClipMachineMemory;

   *rdp = NULL;
   if (!(rd->vtbl = rdd_datadriver(ClipMachineMemory, driver, __PROC__)))
   {
      er = 1;
      goto err;
   }
   rd->loc = dbf_get_locale(ClipMachineMemory);
   rd->lockstyle = ClipMachineMemory->lockstyle;

   if ((er = _rdd_parsepath(ClipMachineMemory, name, rd->vtbl->suff, &rd->path, &rd->name, EG_OPEN, __PROC__)))
      goto err;

   if (readonly && !shared)
      shared = 1;
   rd->shared = shared;
   rd->readonly = readonly;
   rd->tempo = tempo;

   rd->file.dbf = rd->path;
   rd->file.filehash = _clip_hashstr(rd->path);

   if ((er = rdd_open(ClipMachineMemory, rd->path, rd->readonly, rd->shared, &rd->file.fd, __PROC__)))
      goto err;

   if (fstat(rd->file.fd, &st) == -1)
      goto err_open;
   rd->file.mapsize = st.st_size;
   rd->hashes = new_HashTable();

   rd->file.md = (caddr_t) - 1;
#ifdef HAVE_MMAN_H
   if ((ClipMachineMemory->flags1 & MAP_FILE_FLAG) && !HashTable_fetch(ClipMachineMemory->tasklocks, rd->file.filehash))
      rd->file.md = (caddr_t) mmap(0, rd->file.mapsize, PROT_READ | (readonly ? 0 : PROT_WRITE), MAP_SHARED, rd->file.fd, 0);
#endif
   /* If failed use non-mapped file (md==-1) */

   if ((er = rd->vtbl->_rlock(ClipMachineMemory, rd, __PROC__)))
      goto err;
   if ((er = rd->vtbl->open(ClipMachineMemory, rd, __PROC__)))
      goto err_unlock;

   rd->curord = -1;
   rd->idxs_opened = 0;
   rd->ords_opened = 0;
   rd->rels_opened = 0;
   rd->indices = NULL;
   rd->orders = NULL;
   rd->relations = NULL;
   rd->locks = NULL;
   rd->nlocks = 0;
   rd->data = (ClipVar **) calloc(rd->nfields, sizeof(ClipVar *));

   if ((er = rd->vtbl->gotop(ClipMachineMemory, rd, __PROC__)))
      goto err_unlock;
   if ((er = rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__)))
      goto err;
   *rdp = rd;

   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
   goto err;
 err_open:
   er = rdd_err(ClipMachineMemory, EG_OPEN, errno, __FILE__, __LINE__, __PROC__, rd->path);
 err:
#ifdef HAVE_MMAN_H
   if (rd && rd->file.md != (char *) -1)
      munmap(rd->file.md, rd->file.mapsize);
#endif
   if (rd && rd->file.fd != -1)
      _clip_close(ClipMachineMemory, rd->file.filehash, rd->file.fd);
   if (rd && rd->name)
      free(rd->name);
   if (rd && rd->path)
      free(rd->path);
   if (rd)
      free(rd);
   return er;
}
