int
clip_RGCOMP(ClipMachine * ClipMachineMemory)
{
   int ret, sl, rc, i;

   unsigned char *s = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &sl);	/* pattern */

   unsigned char *buf;

   regex_t *preg = NULL;

   if (s == NULL)
   {
      _clip_retl(ClipMachineMemory, 0);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "RGCOMP");
   }
   preg = (regex_t *) malloc(sizeof(regex_t));

   buf = (unsigned char *) malloc(sl + 1);
   memcpy(buf, s, sl);
   buf[sl] = 0;

   for (i = 0; i < sl; i++, s++)
   {
      if ((*s) > 127)
	 buf[i] = _clip_cmptbl[(*s)];
   }
   rc = regcomp(preg, (const char *) buf, REG_EXTENDED | (buf[0] == '^' ? REG_NEWLINE : 0));

   ret = _clip_store_c_item(ClipMachineMemory, preg, _C_ITEM_TYPE_REGEX, destroy_c_regex);
   _clip_retni(ClipMachineMemory, ret);
   free(buf);

   return 0;
}
