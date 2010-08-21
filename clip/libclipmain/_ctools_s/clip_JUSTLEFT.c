int
clip_JUSTLEFT(ClipMachine * ClipMachineMemory)
{
   int l1, l2, i;

   unsigned char *e, *ret, *end, *cur;

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l1);

   int ch = _clip_parni(ClipMachineMemory, 2);

   unsigned char *s = (unsigned char *) _clip_parcl(ClipMachineMemory, 2, &l2);

   int rset = (*(char *) (_clip_fetch_item(ClipMachineMemory, HASH_csetref)) == 't');

   if (s != NULL)
      ch = *s;
   if (ch == 0)
      ch = ' ';

   if (str == NULL || l1 == 0)
   {
      _clip_retc(ClipMachineMemory, "");
      return 0;
   }
   ret = malloc(l1 + 1);
   for (e = str, end = str + l1; e < end && *e == ch; e++);
   i = e - str;
   for (cur = ret; e < end; e++, cur++)
      *cur = *e;
   for (; i > 0; i--, cur++)
      *cur = ch;
   ret[l1] = 0;
   if (rset && _clip_par_isref(ClipMachineMemory, 1))
      _clip_par_assign_str(ClipMachineMemory, 1, (char *) ret, l1);
   _clip_retcn_m(ClipMachineMemory, (char *) ret, l1);
   return 0;
}
