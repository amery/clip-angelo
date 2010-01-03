int
clip_FOUND(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "FOUND";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int       er;


   _clip_retl(ClipMachineMemory, 0);
   if (!wa)
      return 0;

   if (wa->rd->pending_child_parent)
      if ((er = rdd_child_duty(ClipMachineMemory, wa->rd, __PROC__)))
	 return er;

   _clip_retl(ClipMachineMemory, wa->found ? 1 : 0);
   return 0;
}
