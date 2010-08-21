int
clip_SX_KILLTAG(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_KILLTAG";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int lAll = _clip_parl(ClipMachineMemory, 1);

   ClipVar *order = _clip_par(ClipMachineMemory, 1);

   ClipVar *index = _clip_par(ClipMachineMemory, 2);

   int t1 = _clip_parinfo(ClipMachineMemory, 1);

   RDD_INDEX *ri;

   int ord = 0, i, er;

   ClipMachineMemory->m6_error = 0;
   CHECKARG2(1, CHARACTER_type_of_ClipVarType, LOGICAL_type_of_ClipVarType);
   _clip_retl(ClipMachineMemory, 0);
   CHECKWA(wa);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);

   if (t1 == LOGICAL_type_of_ClipVarType)
   {
      if (!lAll)
	 return 0;
   }
   else
   {
      lAll = 0;
   }

   if (lAll && (_clip_parinfo(ClipMachineMemory, 2) == CHARACTER_type_of_ClipVarType))
   {
      ri = NULL;
      for (i = 0; i < wa->rd->idxs_opened; i++)
      {
	 if (strcasecmp(wa->rd->indices[i]->name, index->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf) == 0)
	 {
	    ri = wa->rd->indices[i];
	    break;
	 }
      }
      if (!ri)
	 return 0;
   }
   else
   {
      ord = get_orderno(wa, order, index);
      if (ord == -1)
	 ord = wa->rd->curord;
      if (ord == -1)
	 return 0;
      ri = wa->rd->orders[ord]->index;
   }

   WRITELOCK;
   if (lAll)
   {
      while (ri->norders > 0)
      {
	 if ((er = rdd_orddestroy(ClipMachineMemory, wa->rd, ri->orders[0], __PROC__)))
	    goto err_unlock;
      }
   }
   else
   {
      if ((er = rdd_orddestroy(ClipMachineMemory, wa->rd, wa->rd->orders[ord], __PROC__)))
	 goto err_unlock;
   }
   UNLOCK;

   if (ri->norders == 0)
   {
#ifdef HAVE_MMAN_H
      if ((ri->file.md) != (caddr_t) - 1)
      {
	 if (munmap(ri->file.md, ri->file.mapsize) == -1)
	    goto err_close;
      }
#endif
      if (_clip_close(ClipMachineMemory, ri->file.filehash, ri->file.fd) == -1)
	 goto err_close;
      if (remove(ri->path) == -1)
	 goto err_close;
      if ((er = ri->vtbl->close(ClipMachineMemory, wa->rd, ri, __PROC__)))
	 return er;
      for (i = ri->indexno; i < wa->rd->idxs_opened - 1; i++)
      {
	 wa->rd->indices[i] = wa->rd->indices[i + 1];
	 wa->rd->indices[i]->indexno = i;
      }
      wa->rd->idxs_opened--;
      wa->rd->indices = (RDD_INDEX **) realloc(wa->rd->indices, wa->rd->idxs_opened * sizeof(RDD_INDEX *));
   }
   _clip_retl(ClipMachineMemory, 1);
   return 0;

 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
   goto err;
 err_close:
   er = rdd_err(ClipMachineMemory, EG_CLOSE, errno, __FILE__, __LINE__, __PROC__, ri->path);
 err:
   return er;
}
