int
clip_POSDIFF(ClipMachine * ClipMachineMemory)
{
   int l1, l2;

   unsigned char *e1, *e2, *beg, *end;

   unsigned char *s1 = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l1);

   unsigned char *s2 = (unsigned char *) _clip_parcl(ClipMachineMemory, 2, &l2);

   int ignore = _clip_parni(ClipMachineMemory, 3);

   if (s1 == NULL || s2 == NULL)
   {
      _clip_retni(ClipMachineMemory, -1);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "POSDIFF");
   }
   if (l1 > l2)
   {
      beg = s2;
      end = s2 + l2;
      e1 = s2;
      e2 = s1;
   }
   else
   {
      beg = s1;
      end = s1 + l1;
      e1 = s1;
      e2 = s2;
   }
   e1 += ignore;
   for (; e1 < end && *e1 == *e2; e1++, e2++);
   if (l1 == l2 && e1 == end)
      _clip_retni(ClipMachineMemory, 0);
   else
      _clip_retni(ClipMachineMemory, e1 - beg + 1);
   return 0;
}
