int
clip_CHARRELA(ClipMachine * ClipMachineMemory)
{
   int l1, l2, l3, l4, ret = 0;

   unsigned char *e1, *e2, *e3 = NULL, *e4;

   unsigned char *ee1, *ee3;

   unsigned char *end1, *end2, *end3, *end4;

   unsigned char *str1 = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l1);

   unsigned char *str2 = (unsigned char *) _clip_parcl(ClipMachineMemory, 2, &l2);

   unsigned char *str3 = (unsigned char *) _clip_parcl(ClipMachineMemory, 3, &l3);

   unsigned char *str4 = (unsigned char *) _clip_parcl(ClipMachineMemory, 4, &l4);

   if (str1 == NULL || str2 == NULL || str3 == NULL || str4 == NULL)
   {
      _clip_retni(ClipMachineMemory, 0);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "CHARRELA");
   }
   if (l1 == 0 || l3 == 0)
   {
      _clip_retni(ClipMachineMemory, 0);
      return 0;
   }
   end1 = str1 + l1;
   end3 = str3 + l3;
   end2 = str2 + l2 - l1 + 1;
   end4 = str4 + l4 - l3 + 1;
   for (e2 = str2, e4 = str4; e2 < end2; e2++, e4++)
   {
      for (e1 = str1, ee1 = e2; e1 < end1 && *e1 == *ee1; e1++, ee1++);
      if (e1 == end1)
	 for (e3 = str3, ee3 = e4; e3 < end3 && *e3 == *ee3; e3++, ee3++);
      if (e1 == end1 && e3 == end3)
      {
	 ret = e2 - str2 + 1;
	 break;
      }

   }
   _clip_retni(ClipMachineMemory, ret);
   return 0;
}
