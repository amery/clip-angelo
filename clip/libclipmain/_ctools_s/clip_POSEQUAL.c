int
clip_POSEQUAL(ClipMachine * ClipMachineMemory)
{
   int l1, l2, sovp = 0;

   unsigned char *e1, *e2, *end1, *end2, *beg = NULL;

   unsigned char *s1 = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l1);

   unsigned char *s2 = (unsigned char *) _clip_parcl(ClipMachineMemory, 2, &l2);

   int count = _clip_parni(ClipMachineMemory, 3);

   int ignore = _clip_parni(ClipMachineMemory, 4);

   if (s1 == NULL || s2 == NULL)
   {
      _clip_retni(ClipMachineMemory, -1);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "POSEQUAL");
   }
   if (count <= 0)
      count = l1 > l2 ? l2 : l1;
   end1 = s1 + l1;
   end2 = s2 + l2;
   for (e1 = s1 + ignore, e2 = s2 + ignore; e1 < end1 && e2 < end2; e1++, e2++)
   {
      if (*e1 == *e2)
      {
	 sovp++;
	 if (beg == 0)
	    beg = e1;
      }
      else
      {
	 beg = 0;
	 sovp = 0;
      }
      if (sovp == count)
	 break;
   }
   if (sovp < count)
      _clip_retni(ClipMachineMemory, 0);
   else
      _clip_retni(ClipMachineMemory, beg - s1 + 1);
   return 0;
}
