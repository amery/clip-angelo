/*
   var2str(any_var) -> cStringRepresentation
 */
int
clip_VAR2STR(ClipMachine * ClipMachineMemory)
{
   ClipVar *vp = _clip_par(ClipMachineMemory, 1);

   char *str = 0;

   long len = 0;

   if (!vp)
   {
      _clip_retc(ClipMachineMemory, "");
      return 0;
   }

   _clip_var2str(ClipMachineMemory, vp, &str, &len, 1);
   _clip_retcn_m(ClipMachineMemory, str, len);

   return 0;
}
