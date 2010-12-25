int
rdd_commit(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   int er;

#ifdef HAVE_MMAN_H
   int i;
#endif

   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      return er;
   if (rd->changed)
   {
      if ((er = rd->vtbl->setrecord(ClipMachineMemory, rd, __PROC__)))
	 return er;
      rd->changed = 0;
   }
#ifdef HAVE_MMAN_H
   if (rd->shared && (rd->file.md != (caddr_t) - 1))
   {
      if (msync(rd->file.md, rd->file.mapsize, MS_ASYNC) == -1)
	 return rdd_err(ClipMachineMemory, EG_WRITE, errno, __FILE__, __LINE__, __PROC__, er_ioerror);
      if (rd->memo)
      {
	 if (msync(rd->memo->file.md, rd->memo->file.mapsize, MS_ASYNC) == -1)
	    return rdd_err(ClipMachineMemory, EG_WRITE, errno, __FILE__, __LINE__, __PROC__, er_ioerror);
      }
      for (i = 0; i < rd->idxs_opened; i++)
      {
	 if (msync(rd->indices[i]->file.md, rd->indices[i]->file.mapsize, MS_ASYNC))
	    return rdd_err(ClipMachineMemory, EG_WRITE, errno, __FILE__, __LINE__, __PROC__, er_ioerror);
      }
   }
#endif
   return 0;
}
