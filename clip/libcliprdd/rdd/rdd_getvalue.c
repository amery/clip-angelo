int
rdd_getvalue(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int no, const char *__PROC__)
{
   int eof, r, er;

   if (rd->pending_child_parent)
      if ((er = rdd_child_duty(ClipMachineMemory, rd, __PROC__)))
	 return er;

   if ((er = rdd_eof(ClipMachineMemory, rd, &eof, __PROC__)))
      return er;
   if (eof)
   {
      char *str;

      switch (rd->fields[no].type)
      {
      case 'C':
	 str = (char *) malloc(rd->fields[no].len + 1);
	 memset(str, 32, rd->fields[no].len);
	 str[rd->fields[no].len] = 0;
	 _clip_retcn_m(ClipMachineMemory, str, rd->fields[no].len);
	 break;
      case 'M':
	 _clip_retcn(ClipMachineMemory, "", 0);
	 break;
      case 'N':
	 _clip_retndp(ClipMachineMemory, 0, rd->fields[no].len, rd->fields[no].dec);
	 break;
      case 'D':
	 _clip_retdj(ClipMachineMemory, 0);
	 break;
      case 'L':
	 _clip_retl(ClipMachineMemory, 0);
	 break;
      }
   }
   else
   {
      if ((er = rd->vtbl->getvalue(ClipMachineMemory, rd, no, RETPTR(ClipMachineMemory), __PROC__)))
	 return er;
   }
   if ((er = rdd_event(ClipMachineMemory, EVENT_GET, rd->area, no + 1, RETPTR(ClipMachineMemory), &r, __PROC__)))
      return er;
   return 0;
}
