CLIP_DLLEXPORT int
_clip_eval(ClipMachine * ClipMachineMemory, ClipVar * blockp, int argc, ClipVar * stackp, ClipVar * retp)
{
#ifdef OS_MINGW
   ClipVar *stack = (ClipVar *) malloc(sizeof(ClipVar) * (argc + 1));
#else
   ClipVar *stack = (ClipVar *) alloca(sizeof(ClipVar) * (argc + 1));
#endif
   ClipFrame frame = { stack, stack + 1 + argc, __file__, __LINE__, 0, 0, 0, 0, 0, 0, "eval",
      argc + 1, 0
   };
   enum ClipVarType type = _clip_type(blockp);

   int i, j, r;

   if (type != PCODE_type_of_ClipVarType && type != CCODE_type_of_ClipVarType)
   {
      _clip_trap_str(ClipMachineMemory, __file__, __LINE__, "clip_eval: non-code argument");
#ifdef OS_MINGW
      free(stack);
#endif
      return _clip_call_errblock(ClipMachineMemory, 1);
   }

   if (_clip_profiler)
   {
      _clip_stop_profiler(ClipMachineMemory);
   }

   blockp = _clip_vptr(blockp);
   memset(stack, 0, sizeof(ClipVar) * (argc + 1));

   for (i = 1, j = 0; j < argc; ++j, ++i)
   {
      _clip_refclone(ClipMachineMemory, stack + i, stackp + j);
   }

   frame.ClipFrame_up_of_ClipFrame = ClipMachineMemory->fp;
   ClipMachineMemory->fp = &frame;

   if (_clip_profiler)
   {
      _clip_stop_profiler(ClipMachineMemory);
   }

   if (type == CCODE_type_of_ClipVarType)
      r = _clip_func(ClipMachineMemory, blockp->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipFunction_function_of_ClipCodeVar, argc, 0,
		     blockp->ClipCodeVar_c_of_ClipVar.ClipVarFrame_uplocals_of_ClipCodeVar);
   else
      r = _clip_code_func(ClipMachineMemory, blockp->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipBlock_block_of_ClipCodeVar, argc, 0,
			  blockp->ClipCodeVar_c_of_ClipVar.ClipVarFrame_uplocals_of_ClipCodeVar);

   ClipMachineMemory->fp = frame.ClipFrame_up_of_ClipFrame;
   if (retp)
      *retp = stack[0];
   else
      _clip_destroy(ClipMachineMemory, stack + 0);

   if (_clip_profiler)
   {
      _clip_start_profiler(ClipMachineMemory);
      ClipMachineMemory->pbucket->callno_of_ProfileBucket--;
   }

#ifdef OS_MINGW
   free(stack);
#endif
   return r;
}
