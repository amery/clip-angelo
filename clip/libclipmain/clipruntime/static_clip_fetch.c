static ClipVar *
clip_fetch(ClipMachine * ClipMachineMemory, int dim, int push, int store, ClipVar ** mapp, long *hashp)
{
   ClipFrame *fp = ClipMachineMemory->fp;

   ClipVar *sp = fp->ClipVar_sp_of_ClipFrame;

   ClipVar *ap = sp - dim - 1;

   ClipVar arr;

   ClipVar *vp = NULL;

   /*ClipVar *app; */
   int i;

#ifdef OS_MINGW
   long *vect = (long *) malloc(sizeof(long) * dim);
#else
   long *vect = (long *) alloca(sizeof(long) * dim);
#endif

   memset(&arr, 0, sizeof(ClipVar));
   _clip_clone(ClipMachineMemory, &arr, ap);

   for (i = 0; i < dim; ++i)
   {
      long h;

      void *p;

      h = _clip_hash(ClipMachineMemory, sp - dim + i);
      p = vect + i;

      *(long *) (p) = (h);
      /*SETLONG(vect + i, _clip_hash(ClipMachineMemory, sp - dim + i)); */
   }

   vp = fetch_arr(ClipMachineMemory, &arr, dim, vect, dim, vect, store, mapp, hashp);

   if (!vp)
      return 0;

   for (i = dim + 1; i; --i)
   {
      sp = --(fp->ClipVar_sp_of_ClipFrame);
      _clip_destroy(ClipMachineMemory, sp);
   }

   if (push)
   {
      if (!vp)
      {
	 vp = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame++;
	 CLIP_CHECK_STACK;
	 memset(vp, 0, sizeof(ClipVar));
      }
      else
	 _clip_push(ClipMachineMemory, vp);
   }

   if (!vp)
   {
      _clip_trap_str(ClipMachineMemory, ClipMachineMemory->fp->filename_of_ClipFrame, ClipMachineMemory->fp->line_of_ClipFrame, "map/array fetch error");
      _clip_call_errblock(ClipMachineMemory, EG_BOUND);
   }

#if 0
   app = _clip_vptr(&arr);
   if (app->ClipType_t_of_ClipVar.count == 1 && !push)
#else
   if (clip_count(&arr) == 1 && !push)
#endif
   {
      vp = _clip_add_tempvar(ClipMachineMemory, vp);
   }

   _clip_destroy(ClipMachineMemory, &arr);
#ifdef OS_MINGW
   free(vect);
#endif
   return vp;
}
