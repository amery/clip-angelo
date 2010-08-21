int
clip_CLIPROOT(ClipMachine * ClipMachineMemory)
{
   char *ret;

   int len1, len2;

   char *s1 = _clip_cygwinroot();

   char *s2 = getenv("CLIPROOT");

   if (s2 == NULL || *s2 == 0)
      s2 = CLIPROOT;

   len1 = strlen(s1);
   len2 = strlen(s2);
   ret = malloc(len1 + len2 + 1);
   memcpy(ret, s1, len1);
   memcpy(ret + len1, s2, len2);
   ret[len1 + len2] = 0;

   _clip_retcn_m(ClipMachineMemory, ret, len1 + len2);
   return 0;
}
