int
clip_TOKENNEXT(ClipMachine * ClipMachineMemory)
{
   int l1, nt, *tmp1, tlen;

#ifndef  D20070313
   unsigned char ch;
#endif
   unsigned char *e, *end, *ret, *tbeg, *tend, *tmp2;

   void *atsep = _clip_fetch_item(ClipMachineMemory, HASH_token_atsep);

   unsigned char *str = _clip_fetch_item(ClipMachineMemory, HASH_token_string);

   unsigned char *buf = (unsigned char *) _clip_fetch_item(ClipMachineMemory, HASH_token_delimiters);

   int ignore = *((int *) atsep);

   tmp2 = atsep + 3 * sizeof(int);

   l1 = *(int *) (tmp2 + 4);
   tlen = *((int *) (tmp2 + 4) + 1);

   if (str == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "TOKENNEXT");
   }

   if (tlen <= 0)
      tlen = CLIP_TOKEN_MAXLEN;

   *tmp2 = 0;
   *(tmp2 + 2) = 0;

   e = str + ignore;
   //printf("\ne=%s,%d ",e,ignore);
   end = str + l1;
   for (nt = 0; nt < tlen && e < end && buf[(int) (*e)]; e++, nt++);
   if (e != str)
      *tmp2 = *(e - 1);
   tbeg = e;
   //printf("tbeg=%s ",tbeg);
   for (; e < end && !buf[(int) (*e)]; e++);
   *(tmp2 + 2) = *(e);
   tend = e;
   //printf("tend=%s",tend);
/*        for(nt=0; nt<tlen && e<end && buf[*e];  e++,nt++);*/
   *((int *) atsep) = e - str;

   tbeg = tbeg > tend ? tend : tbeg;
   ret = malloc(tend - tbeg + 1);
   memcpy(ret, tbeg, tend - tbeg);
   ret[tend - tbeg] = 0;
   _clip_retcn_m(ClipMachineMemory, (char *) ret, tend - tbeg);
#ifndef  D20070313
   for (ch = *e; e < end && ch == *e; e++);
   tend = e;
#endif
   //printf("tend=%s,%d\n",tend,tend - str);
   *((int *) atsep) = tend - str;
   tmp1 = (int *) atsep;
   tmp1++;
   *tmp1 = tbeg - str + 1;
   tmp1++;
   *tmp1 = tend - str + 1;
   return 0;
}
