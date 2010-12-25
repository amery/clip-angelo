int
clip_MAP(ClipMachine * ClipMachineMemory)
{
   ClipVar *rp;

   rp = RETPTR(ClipMachineMemory);
   _clip_map(ClipMachineMemory, rp);
   /*decount(rp); */

   return 0;
}
