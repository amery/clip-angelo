int
clip_TRANSLATE_CHARSET(ClipMachine * ClipMachineMemory)
{
   int len = 0, r;

   char *p1 = _clip_parc(ClipMachineMemory, 1);

   char *p2 = _clip_parc(ClipMachineMemory, 2);

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 3, &len);

   unsigned char *s;

   if (!p1 || !p2 || !str)
      return EG_ARG;

   if (!strcasecmp(p1, p2))
   {
      _clip_retcn(ClipMachineMemory, (char *) str, len);
      return 0;
   }

   if (!strcasecmp(p1, "utf-8"))
   {
      char *result;

      if (!_clip_translate_fromutf8(p2, (char *) str, len, &result))
      {
	 _clip_retc(ClipMachineMemory, result);
	 free(result);
      }
      else
	 _clip_retc(ClipMachineMemory, (char *) str);
      return 0;
   }

   if (!strcasecmp(p2, "utf-8"))
   {
      char *result;

      if (!_clip_translate_toutf8(p1, str, len, &result))
      {
	 _clip_retc(ClipMachineMemory, result);
	 free(result);
      }
      else
	 _clip_retc(ClipMachineMemory, (char *) str);
      return 0;
   }

   s = (unsigned char *) malloc(len + 1);
   s[len] = 0;

   if ((r = _clip_translate_charset(p1, p2, str, s, len)))
      return r;

   _clip_retcn_m(ClipMachineMemory, (char *) s, len);

   return 0;
}
