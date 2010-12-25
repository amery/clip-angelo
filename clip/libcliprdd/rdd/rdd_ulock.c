int
rdd_ulock(ClipMachine * ClipMachineMemory, RDD_DATA * rd, unsigned int rec, int flock, const char *__PROC__)
{
/*
	int i;
#ifdef HAVE_MMAN_H
	if(rd->shared && (int)rd->file.md!=-1){
		if(msync(rd->file.md,rd->file.mapsize,MS_ASYNC)==-1)
			return rdd_err(ClipMachineMemory,EG_LOCK,errno,__FILE__,__LINE__,__PROC__,er_ioerror);
		if(rd->memo){
			if(msync(rd->memo->file.md,rd->memo->file.mapsize,MS_ASYNC)==-1)
				return rdd_err(ClipMachineMemory,EG_LOCK,errno,__FILE__,__LINE__,__PROC__,er_ioerror);
		}
		for(i=0;i<rd->idxs_opened;i++){
			if(msync(rd->indices[i]->file.md,rd->indices[i]->file.mapsize,MS_ASYNC)==-1)
				return rdd_err(ClipMachineMemory,EG_LOCK,errno,__FILE__,__LINE__,__PROC__,er_ioerror);
		}
	}
#endif
*/
   if (rec < 0)
      return 0;
   return rd->vtbl->ulock(ClipMachineMemory, rd, rec, flock, __PROC__);
}
