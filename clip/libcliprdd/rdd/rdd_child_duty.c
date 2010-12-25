int
rdd_child_duty(ClipMachine * ClipMachineMemory, RDD_DATA * child, const char *__PROC__)
{
   int i, er = 0, found;

   ClipVar v, *vp;

   RDD_DATA *rd = child->pending_child_parent;

   RDD_RELATION *rel = NULL;

   int locked = child->file.rlocked || child->file.wlocked;

   CLEAR_CLIPVAR(&v);
   child->pending_child_parent = NULL;

   if (!rd)
      goto err;
   for (i = 0; i < child->pars_opened; i++)
   {
      if (child->parents[i]->parent == rd)
      {
	 rel = child->parents[i];
	 break;
      }
   }
   if (!rel)
      goto err;

   if (!locked)
   {
      if ((er = child->vtbl->_rlock(ClipMachineMemory, child, __PROC__)))
	 goto err;
   }
   if (rel->simpexpr)
   {
      if ((er = rdd_takevalue(ClipMachineMemory, rd, rel->simpfno, &v, __PROC__)))
	 goto err_unlock;
   }
   else
   {
      if ((er = rdd_calc(ClipMachineMemory, rd->area, &rel->block, &v, 0)))
	 goto err_unlock;
   }
   vp = _clip_vptr(&v);
   if ((child->ords_opened > 0) && (child->curord != -1))
   {
      if ((er = rdd_checkifnew(ClipMachineMemory, child, __PROC__)))
	 goto err_unlock;
      if (rel->scoped)
      {
	 if ((er = rdd_scopetop(ClipMachineMemory, child, vp, __PROC__)))
	    goto err_unlock;
	 if ((er = rdd_scopebottom(ClipMachineMemory, child, vp, __PROC__)))
	    goto err_unlock;
      }
      if (rd->eof)
      {
	 unsigned int lastrec;

	 child->orders[child->curord]->valid_stack = 0;
	 child->eof = 1;
	 if ((er = rdd_lastrec(ClipMachineMemory, child, (int *) (&lastrec), __PROC__)))
	    return er;
	 child->recno = lastrec + 1;
      }
      else
      {
	 if ((er = child->orders[child->curord]->vtbl->seek(ClipMachineMemory, child, child->orders[child->curord], vp, 0, 0, &found, __PROC__)))
	    goto err_unlock;
      }
   }
   else
   {
      if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != NUMERIC_type_of_ClipVarType)
      {
	 er = rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_badrelationexpr);
	 goto err_unlock;
      }
      if ((er = child->vtbl->go(ClipMachineMemory, child, vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar, __PROC__)))
	 goto err_unlock;
      if (child->area != -1)
	 ((DBWorkArea *) (ClipMachineMemory->areas->items_of_ClipVect[child->area]))->found = !child->eof;
   }
   if (!locked)
   {
      if ((er = child->vtbl->_ulock(ClipMachineMemory, child, __PROC__)))
	 goto err;
   }

   _clip_destroy(ClipMachineMemory, &v);
   return 0;

 err_unlock:
   if (!locked)
      child->vtbl->_ulock(ClipMachineMemory, child, __PROC__);
 err:
   _clip_destroy(ClipMachineMemory, &v);
   return er;
}
