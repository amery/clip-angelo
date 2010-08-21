int
clip_ATTOKEN(ClipMachine * ClipMachineMemory)
{
   int l1, l2, ret;

   /*int lflag = 0 ; */
   unsigned char *beg;

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l1);

   unsigned char *dstr = (unsigned char *) _clip_parcl(ClipMachineMemory, 2, &l2);

   int count = 0xffff;

   if (_clip_parinfo(ClipMachineMemory, 2) == NUMERIC_type_of_ClipVarType)
      count = _clip_parni(ClipMachineMemory, 2);
   if (_clip_parinfo(ClipMachineMemory, 3) == NUMERIC_type_of_ClipVarType)
      count = _clip_parni(ClipMachineMemory, 3);

   if (str == NULL)
   {
      _clip_retni(ClipMachineMemory, 0);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "ATTOKEN");
   }
   if (dstr == NULL)
   {
      dstr = (unsigned char *) ("\\ \t\n\r,.;:!?/<<>>()^#&%+-*");
      l2 = strlen((const char *) dstr);
   }

   if (count <= 0)
   {
      _clip_retni(ClipMachineMemory, 0);
      return 0;
   }

   /*
      if ( count != 0xFFFF )
      {
      if ( l2 == 1 && dstr[0] == 10 )
      {
      count --;
      lflag = 1;
      }
      if ( l2 == 2 && dstr[0] == 13 && dstr[1] == 10 )
      {
      count --;
      lflag = 1;
      }
      //count --;
      }
    */
   beg = _clip_attoken(str, l1, dstr, l2, count);
   /*
      printf("\ncount=%d,l1=%d,beg=%s,n=%d\n",count,l1,beg,beg +l2 - (str + l1));
    */
   ret = 0;
   if (beg != NULL)
      ret = beg - str + 1;
   /*
      if ( beg + l2 + (count==0xFFFF ? -1: 0) < str + l1)
      ret = beg - str + l2+1;
    */
   if (ret > l1)
      ret = l1;
   _clip_retni(ClipMachineMemory, ret);
   return 0;
}
