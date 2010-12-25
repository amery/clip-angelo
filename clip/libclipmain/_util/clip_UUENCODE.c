/*
   UUENCODE(cVar [,without_newline]) -> cVar
 */

int
clip_UUENCODE(ClipMachine * ClipMachineMemory)
{
   char *str, *out = 0;

   long ol;

   int l;

   int without_newline = _clip_parl(ClipMachineMemory, 2);

   str = _clip_parcl(ClipMachineMemory, 1, &l);
   if (!str)
      return EG_ARG;

   _clip_uuencode(str, l, &out, &ol, without_newline);
   _clip_retcn_m(ClipMachineMemory, out, ol);

   return 0;
}
