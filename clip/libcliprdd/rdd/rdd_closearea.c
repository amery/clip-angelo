int
rdd_closearea(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   int i, k, er;

#ifdef HAVE_MMAN_H
   int fd;

   char upd;
#endif

   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      return er;
#ifdef HAVE_MMAN_H
   if ((rd->file.md) != (caddr_t) - 1)
   {
      if (msync(rd->file.md, rd->file.mapsize, MS_SYNC | MS_INVALIDATE))
	 goto err;
      if (munmap(rd->file.md, rd->file.mapsize) == -1)
	 goto err;
      rd->file.md = (caddr_t) - 1;
   }
   if ((rd->memo) && ((rd->memo->file.md) != (caddr_t) - 1))
   {
      if (msync(rd->memo->file.md, rd->memo->file.mapsize, MS_SYNC | MS_INVALIDATE))
	 goto err;
      if (munmap(rd->memo->file.md, rd->memo->file.mapsize) == -1)
	 goto err;
      if (rd->memo->updated)
      {
	 fd = rd->memo->file.fd;
	 if (lseek(fd, 0, SEEK_SET))
	    goto err;
	 if (read(fd, &upd, 1) != 1)
	    goto err;
	 if (lseek(fd, 0, SEEK_SET))
	    goto err;
	 if (write(fd, &upd, 1) != 1)
	    goto err;
      }
   }
   for (i = 0; i < rd->idxs_opened; i++)
   {
      if ((rd->indices[i]->file.md) != (caddr_t) - 1)
      {
	 if (msync(rd->indices[i]->file.md, rd->indices[i]->file.mapsize, MS_SYNC | MS_INVALIDATE))
	    goto err;
	 if (munmap(rd->indices[i]->file.md, rd->indices[i]->file.mapsize) == -1)
	    goto err;
	 if (rd->indices[i]->updated)
	 {
	    fd = rd->indices[i]->file.fd;
	    if (lseek(fd, 0, SEEK_SET))
	       goto err;
	    if (read(fd, &upd, 1) != 1)
	       goto err;
	    if (lseek(fd, 0, SEEK_SET))
	       goto err;
	    if (write(fd, &upd, 1) != 1)
	       goto err;
	 }
      }
   }
#endif
   if (rd->memo)
   {
      if (_clip_close(ClipMachineMemory, rd->memo->file.filehash, rd->memo->file.fd) == -1)
	 goto err;
      if (rd->tempo)
      {
	 if (remove(rd->memo->path) == -1)
	    goto err;
      }
   }
   for (i = 0; i < rd->idxs_opened; i++)
   {
      if (_clip_close(ClipMachineMemory, rd->indices[i]->file.filehash, rd->indices[i]->file.fd) == -1)
	 goto err;
      if (rd->tempo)
      {
	 if (remove(rd->indices[i]->path) == -1)
	    goto err;
      }
   }
   if (rd->filter)
   {
      if (rd->filter->active)
      {
	 if ((er = rdd_destroyfilter(ClipMachineMemory, rd->filter, __PROC__)))
	    return er;
      }
   }
   for (k = 0; k < rd->rels_opened; k++)
   {
      RDD_DATA *rel = rd->relations[k]->child;

      rel->pending_child_parent = NULL;
      for (i = 0; i < rel->pars_opened; i++)
      {
	 if (rel->parents[i]->parent == rd)
	 {
	    if (rel->parents[i]->expr)
	       free(rel->parents[i]->expr);
	    _clip_destroy(ClipMachineMemory, &rel->parents[i]->block);
	    free(rel->parents[i]);
	    memmove(rel->parents + i, rel->parents + i + 1, (rel->pars_opened - i - 1) * sizeof(RDD_RELATION *));
	    rel->pars_opened--;
	    rel->parents = realloc(rel->parents, rel->pars_opened * sizeof(void *));
	    i--;
	 }
      }
   }
   if (rd->relations)
      free(rd->relations);

   for (k = 0; k < rd->pars_opened; k++)
   {
      RDD_DATA *par = rd->parents[k]->parent;

      for (i = 0; i < par->rels_opened; i++)
      {
	 if (par->relations[i]->child == rd)
	 {
	    if (par->relations[i]->expr)
	       free(par->relations[i]->expr);
	    _clip_destroy(ClipMachineMemory, &par->relations[i]->block);
	    free(par->relations[i]);
	    memmove(par->relations + i, par->relations + i + 1, (par->rels_opened - i - 1) * sizeof(RDD_RELATION *));
	    par->rels_opened--;
	    par->relations = realloc(par->relations, par->rels_opened * sizeof(void *));
	    i--;
	 }
      }
   }
   if (rd->parents)
      free(rd->parents);

   _clip_destroy(ClipMachineMemory, &rd->os.bForCondition);
   _clip_destroy(ClipMachineMemory, &rd->os.bWhileCondition);
   _clip_destroy(ClipMachineMemory, &rd->os.bEval);

   if ((er = rd->vtbl->close(ClipMachineMemory, rd, __PROC__)))
      return er;
   if ((er = rdd_ulock(ClipMachineMemory, rd, 0, 1, __PROC__)))
      return er;
   if ((er = rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__)))
      return er;
   if (_clip_close(ClipMachineMemory, rd->file.filehash, rd->file.fd) == -1)
      goto err;
   if (rd->tempo)
   {
      if (remove(rd->path) == -1)
	 goto err;
   }
   return 0;
 err:
   return rdd_err(ClipMachineMemory, EG_CLOSE, errno, __FILE__, __LINE__, __PROC__, rd->path);
}
