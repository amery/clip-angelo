int
rdd_createindex(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *driver, const char *name, const char *tag, const char *expr, ClipVar * block, int unique, const char *__PROC__)
{
   RDD_INDEX *ri = NULL;

   RDD_ORDER *ro = NULL;

   int alreadyopened = 0;

   int cr = 0, er = EG_UNSUPPORTED;

   int i, j;

   char tagname[11];

   ri = (RDD_INDEX *) calloc(1, sizeof(RDD_INDEX));
   if (rd->area != -1)
   {
      if (!tag && strcasecmp(driver, "CDX") == 0)
      {
	 if (!(ri->vtbl = rdd_indexdriver(ClipMachineMemory, "IDX", __PROC__)))
	    goto err;
      }
      else if (!tag && strcasecmp(driver, "CTX") == 0)
      {
	 if (!(ri->vtbl = rdd_indexdriver(ClipMachineMemory, "NTX", __PROC__)))
	    goto err;
      }
      else
      {
	 if (!(ri->vtbl = rdd_indexdriver(ClipMachineMemory, driver, __PROC__)))
	    goto err;
      }
   }
   else
   {
      if (!(ri->vtbl = rdd_indexdriver(ClipMachineMemory, driver, __PROC__)))
	 goto err;
   }
   if ((er = _rdd_parsepath(ClipMachineMemory, name, ri->vtbl->suff, &ri->path, &ri->name, EG_CREATE, __PROC__)))
      goto err;

   ri->file.filehash = _clip_hashstr(ri->path);

   if (ri->vtbl->ismulti && (access(ri->path, F_OK) == 0))
   {
      free(ri->path);
      free(ri->name);
      free(ri);
      if ((er = rdd_setindex(ClipMachineMemory, rd, &ri, driver, name, NULL, 0, __PROC__)))
	 goto err;
      alreadyopened = ri->indexno + 1;
   }

   ri->rd = rd;
   ri->loc = rd->loc;

   if (!alreadyopened)
   {
      struct stat st;

      ri->file.fd = _clip_creat(ClipMachineMemory, ri->path, O_RDWR, ClipMachineMemory->fileCreateMode, rd->shared);
      if (ri->file.fd == -1)
	 goto err_create;
      ri->file.mapsize = 1024;
      if (fstat(ri->file.fd, &st) == -1)
	 goto err_open;
      if (lseek(ri->file.fd, ri->file.mapsize - 1, SEEK_SET) == -1)
	 goto err_create;
      if (write(ri->file.fd, "", 1) == -1)
	 goto err_create;
      ri->file.md = (caddr_t) - 1;
#ifdef HAVE_MMAN_H
      if ((ClipMachineMemory->flags1 & MAP_FILE_FLAG) && !HashTable_fetch(ClipMachineMemory->tasklocks, rd->file.filehash))
	 ri->file.md = (caddr_t) mmap(0, ri->file.mapsize, PROT_READ | PROT_WRITE, MAP_SHARED, ri->file.fd, 0);
      /* If failed use non-mapped file (md==-1) */
#endif
      cr = 1;
   }

   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      goto err;

   if (tag)
   {
      strncpy(tagname, tag, 10);
      tagname[10] = 0;
      _clip_upstr(tagname, 10);
   }
   rd->indexing = ri->name;
   if ((er = ri->vtbl->create(ClipMachineMemory, rd, ri, &ro, tag ? tagname : NULL, expr, block, unique, cr, 0, __PROC__)))
      goto err;
   rd->indexing = NULL;

   if (ri->structural && !rd->readonly)
   {
      if ((er = rd->vtbl->setstruct(ClipMachineMemory, rd, __PROC__)))
	 goto err;
   }

   if (!rd->os.lAdditive)
   {
      j = rd->idxs_opened;
      for (i = 0; i < j; i++)
      {
	 if (rd->indices[i]->structural || rd->indices[i]->indexno == alreadyopened - 1)
	    continue;
	 rd->idxs_opened--;
	 rd->ords_opened -= rd->indices[i]->norders;
	 if (rd->indices[i] != ri)
	 {
#ifdef HAVE_MMAN_H
	    if ((rd->indices[i]->file.md) != (caddr_t) - 1)
	    {
	       if (munmap(rd->indices[i]->file.md, rd->indices[i]->file.mapsize) == -1)
		  goto err_close;
	    }
#endif
	    if (_clip_close(ClipMachineMemory, rd->indices[i]->file.filehash, rd->indices[i]->file.fd) == -1)
	       goto err_close;
	    if ((er = rd->indices[i]->vtbl->close(ClipMachineMemory, rd, rd->indices[i], __PROC__)))
	       goto err;
	 }
      }
   }
   if (!alreadyopened)
   {
      rd->idxs_opened++;
      rd->indices = realloc(rd->indices, rd->idxs_opened * sizeof(RDD_INDEX *));
      rd->indices[rd->idxs_opened - 1] = ri;
   }
   rd->ords_opened = 0;
   for (i = 0; i < rd->idxs_opened; i++)
   {
      for (j = 0; j < rd->indices[i]->norders; j++)
      {
	 rd->orders = realloc(rd->orders, (rd->ords_opened + 1) * sizeof(RDD_ORDER *));
	 rd->orders[rd->ords_opened] = rd->indices[i]->orders[j];
	 rd->orders[rd->ords_opened]->orderno = rd->ords_opened;
	 if (rd->orders[rd->ords_opened] == ro)
	    rd->curord = rd->ords_opened;
	 rd->ords_opened++;
      }
   }
   if (rd->ords_opened == 1)
      if ((er = ri->vtbl->gotop(ClipMachineMemory, rd, ro, __PROC__)))
	 goto err;
   return 0;
 err_open:
   er = rdd_err(ClipMachineMemory, EG_OPEN, errno, __FILE__, __LINE__, __PROC__, ri->path);
   goto err;
 err_create:
   er = rdd_err(ClipMachineMemory, EG_CREATE, errno, __FILE__, __LINE__, __PROC__, ri->path);
   goto err;
 err_close:
   er = rdd_err(ClipMachineMemory, EG_CLOSE, errno, __FILE__, __LINE__, __PROC__, rd->indices[i]->path);
 err:
   if (!alreadyopened)
   {
      if (ri && ri->name)
	 free(ri->name);
      if (ri && ri->path)
	 free(ri->path);
#ifdef HAVE_MMAN_H
      if (ri && ri->file.md != (char *) -1)
	 munmap(ri->file.md, ri->file.mapsize);
#endif
      if (ri && ri->file.fd != -1)
	 _clip_close(ClipMachineMemory, ri->file.filehash, ri->file.fd);
      if (ri)
	 free(ri);
   }
   return er;
}
