int
clip_SX_SLIMFAST(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_SLIMFAST";

   const char *str = _clip_parc(ClipMachineMemory, 1);

   const char *s;

   char *ret, *r;

   char quote = 0;

   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, CHARACTER_type_of_ClipVarType);

   if (!str)
      str = "";

   ret = calloc(1, strlen(str) + 1);
   for (s = str, r = ret; *s; s++)
   {
      if (quote && ((*s == quote) || ((quote == '[') && (*s == ']'))))
      {
	 quote = 0;
	 *(r++) = *s;
	 continue;
      }
      if (*s == '[' || *s == '"' || *s == '\'')
	 quote = *s;
      if (quote)
      {
	 *(r++) = *s;
	 continue;
      }
      if ((*s == ' ') && ((r > ret) && (*(r - 1) == ' ')))
	 continue;
      *(r++) = _clip_toupper(*s);
   }
   *r = 0;
   _clip_retc(ClipMachineMemory, ret);
   free(ret);
   return 0;
 err:
   return er;
}
