int
clip_CLIP(ClipMachine * ClipMachineMemory)
{
   char *funcname = _clip_parc(ClipMachineMemory, 1);

   int i, parcount = _clip_parinfo(ClipMachineMemory, 0);

   ClipVar *params, *retVal = RETPTR(ClipMachineMemory);

   if (funcname == NULL)
   {
      int r;

      r = _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "CLIP");
      return _clip_call_errblock(ClipMachineMemory, r);
   }
   params = malloc(parcount * sizeof(ClipVar));
   for (i = 2; i <= parcount; i++)
      params[i - 2] = *_clip_spar(ClipMachineMemory, i);

   _clip_clip(ClipMachineMemory, funcname, parcount - 1, params, retVal);

   free(params);
   return 0;
}
