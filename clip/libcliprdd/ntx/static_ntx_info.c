static int
ntx_info(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, int cmd, const char *__PROC__)
{
   switch (cmd)
   {
   case DBOI_CONDITION:
      {
	 _clip_retc(ClipMachineMemory, "");
	 break;
      }
   case DBOI_CUSTOM:
      {
	 _clip_retl(ClipMachineMemory, 0);
	 break;
      }
   case DBOI_HPLOCKING:
      {
	 _clip_retl(ClipMachineMemory, 0);
	 break;
      }
   case DBOI_ISCOND:
      {
	 _clip_retl(ClipMachineMemory, 0);
	 break;
      }
   case DBOI_ISDESC:
      {
	 _clip_retl(ClipMachineMemory, 0);
	 break;
      }
   case DBOI_ORDERCOUNT:
      {
	 _clip_retni(ClipMachineMemory, 1);
	 break;
      }
   case DBOI_SCOPEBOTTOM:
      {
	 _clip_clone(ClipMachineMemory, RETPTR(ClipMachineMemory), &ro->scopebottomvar);
	 break;
      }
   case DBOI_SCOPETOP:
      {
	 _clip_clone(ClipMachineMemory, RETPTR(ClipMachineMemory), &ro->scopetopvar);
	 break;
      }
   case DBOI_SKIPUNIQUE:
      {
	 _clip_retl(ClipMachineMemory, 1);
	 break;
      }
   }
   return 0;
}
