int
clip_STRFTIME(ClipMachine * ClipMachineMemory)
{
   char *frm = _clip_parc(ClipMachineMemory, 1);

   int offset = _clip_parni(ClipMachineMemory, 2);

   time_t t = time(NULL) + offset;

   struct tm *tt = gmtime(&t);

   char s[81];

   strftime(s, sizeof(s), frm, tt);
   _clip_retc(ClipMachineMemory, s);
   return 0;
}
