int
rdd_setindex(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX ** rip, const char *driver, const char *name, const char *tag, int structural, const char *__PROC__)
{
   RDD_INDEX *ri = (RDD_INDEX *) calloc(1, sizeof(RDD_INDEX));

   struct stat st;

   int i, er = EG_UNSUPPORTED;

   int l = strlen(name);

   if (!name || !l)
      return 0;

   ri->file.fd = -1;
   ri->file.md = (char *) -1;
   ri->rd = rd;

   if (!structural)
   {
      if (!(l > 4 && isalnum(name[l - 1]) && isalnum(name[l - 2]) && isalnum(name[l - 3]) && name[l - 4] == '.'))
      {

	 if (!tag && (rd->area != -1) && (strcasecmp(driver, "CDX") == 0))
	 {
	    if (!(ri->vtbl = rdd_indexdriver(ClipMachineMemory, "IDX", __PROC__)))
	       goto err;
	    if ((er = _rdd_parsepath(ClipMachineMemory, name, ri->vtbl->suff, &ri->path, &ri->name, EG_OPEN, __PROC__)))
	       goto err;
	    if (access(ri->path, F_OK))
	    {
	       free(ri->path);
	       ri->path = NULL;
	       free(ri->name);
	       ri->name = NULL;
	       if (!(ri->vtbl = rdd_indexdriver(ClipMachineMemory, driver, __PROC__)))
		  goto err;
	       if ((er = _rdd_parsepath(ClipMachineMemory, name, ri->vtbl->suff, &ri->path, &ri->name, EG_OPEN, __PROC__)))
		  goto err;
	    }
	 }
	 else if (!tag && (rd->area != -1) && (strcasecmp(driver, "CTX") == 0))
	 {
	    if (!(ri->vtbl = rdd_indexdriver(ClipMachineMemory, "NTX", __PROC__)))
	       goto err;
	    if ((er = _rdd_parsepath(ClipMachineMemory, name, ri->vtbl->suff, &ri->path, &ri->name, EG_OPEN, __PROC__)))
	       goto err;
	    if (access(ri->path, F_OK))
	    {
	       free(ri->path);
	       ri->path = NULL;
	       free(ri->name);
	       ri->name = NULL;
	       if (!(ri->vtbl = rdd_indexdriver(ClipMachineMemory, driver, __PROC__)))
		  goto err;
	       if ((er = _rdd_parsepath(ClipMachineMemory, name, ri->vtbl->suff, &ri->path, &ri->name, EG_OPEN, __PROC__)))
		  goto err;
	    }
	 }
	 else
	 {
	    if (!(ri->vtbl = rdd_indexdriver(ClipMachineMemory, driver, __PROC__)))
	       goto err;
	    if ((er = _rdd_parsepath(ClipMachineMemory, name, ri->vtbl->suff, &ri->path, &ri->name, EG_OPEN, __PROC__)))
	       goto err;
	 }
      }
      else
      {
	 if (!(ri->vtbl = rdd_indexdriver(ClipMachineMemory, driver, __PROC__)))
	    goto err;
	 if ((er = _rdd_parsepath(ClipMachineMemory, name, ri->vtbl->suff, &ri->path, &ri->name, EG_OPEN, __PROC__)))
	    goto err;
      }
   }
   else
   {
      if (!(ri->vtbl = rdd_indexdriver(ClipMachineMemory, driver, __PROC__)))
	 goto err;
      ri->path = strdup(name);
      ri->name = strdup(rd->name);
   }

   for (i = 0; i < rd->idxs_opened; i++)
      if (strcmp(rd->indices[i]->path, ri->path) == 0)
      {
	 free(ri->name);
	 free(ri->path);
	 free(ri);
	 if (rip)
	    *rip = rd->indices[i];
	 return rdd_setorder(ClipMachineMemory, rd, rd->indices[i]->orders[0]->orderno + 1, __PROC__);
      }

   ri->file.filehash = _clip_hashstr(ri->path);

   if ((er = rdd_open(ClipMachineMemory, ri->path, rd->readonly, rd->shared, &ri->file.fd, __PROC__)))
      goto err;

   if (fstat(ri->file.fd, &st) == -1)
      goto err_open;
   ri->file.mapsize = st.st_size;
   ri->file.md = (caddr_t) - 1;
#ifdef HAVE_MMAN_H
   if ((ClipMachineMemory->flags1 & MAP_FILE_FLAG) && !HashTable_fetch(ClipMachineMemory->tasklocks, rd->file.filehash))
      ri->file.md = (caddr_t) mmap(0, ri->file.mapsize, PROT_READ | (rd->readonly ? 0 : PROT_WRITE), MAP_SHARED, ri->file.fd, 0);
   /* If failed use non-mapped file (md==-1) */
#endif
   if ((er = ri->vtbl->open(ClipMachineMemory, rd, ri, tag, __PROC__)))
      goto err;
   if (rd->curord == -1)
   {
      rd->curord = ri->orders[0]->orderno;
   }
   rd->idxs_opened++;
   rd->indices = (RDD_INDEX **) realloc(rd->indices, sizeof(RDD_INDEX *) * rd->idxs_opened);
   rd->indices[rd->idxs_opened - 1] = ri;
   ri->indexno = rd->idxs_opened - 1;

   for (i = 0; i < ri->norders; i++)
   {
      RDD_ORDER *ro = ri->orders[i];

      if (ro->cforexpr && ro->cforexpr[0])
      {
	 int r = _clip_parni(ClipMachineMemory, 1);

	 char expr[1024];

	 memset(expr, 0, sizeof(expr));
	 rdd_expandmacro(rd->area, r, ro->cforexpr, expr);

	 if ((er = _clip_eval_macro(ClipMachineMemory, expr, strlen(expr), &ro->bforexpr)))
	    goto err;
      }
   }
   if (rip)
      *rip = ri;
   return 0;
 err_open:
   er = rdd_err(ClipMachineMemory, EG_OPEN, errno, __FILE__, __LINE__, __PROC__, ri->path);
 err:
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
   return er;
}
