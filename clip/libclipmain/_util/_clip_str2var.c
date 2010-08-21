void
_clip_str2var(ClipMachine * ClipMachineMemory, ClipVar * vp, char *str, long len, int method)
{
   char *buf = 0, *b;

   long buflen = 0;

   switch (method)
   {
   case 1:			/* uuencode */
      _clip_uudecode(str, len, &buf, &buflen);
      break;
   case 2:			/* compress */
   default:			/* no either uuencode nor compress */
      buf = str;
      buflen = len;
      break;
   }

   b = buf;
   get_var(ClipMachineMemory, vp, &buf, &buflen);

   if (_clip_type(vp) == MAP_type_of_ClipVarType)
   {
      ClipVar *np;

      char *s = 0;

      int l = 0;

      np = _clip_mget(ClipMachineMemory, vp, HASH_CLASSNAME);
      if (!_clip_strFromVar(ClipMachineMemory, np, &s, &l) && l)
      {
	 buflen = l + RECOVER_PREFIX_LEN + 1;
	 b = (char *) realloc(b, buflen);
	 memcpy(b, RECOVER_PREFIX, RECOVER_PREFIX_LEN);
	 memcpy(b + RECOVER_PREFIX_LEN, s, l);
	 b[RECOVER_PREFIX_LEN + l] = 0;

	 _clip_clip(ClipMachineMemory, b, 1, vp, 0);
      }
      free(s);
   }
   if (method == 1 /*|| method == 2 */ )
      free(b);
}
