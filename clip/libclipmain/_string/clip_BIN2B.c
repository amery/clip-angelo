int
clip_BIN2B(ClipMachine * ClipMachineMemory)
{
   int vl = 0;

   char ret;

   char *vp = _clip_parcl(ClipMachineMemory, 1, &vl);

   if (vp == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "BIN2I");
   }
   ret = *((char *) vp);
   _clip_retni(ClipMachineMemory, ret);
   return 0;
}
