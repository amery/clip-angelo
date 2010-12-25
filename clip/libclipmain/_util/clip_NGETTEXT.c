/*
	NGETTEXT(cSingleMsg, cPluralMsg, nNum [,cModule])->cTranslated
*/
int
clip_NGETTEXT(ClipMachine * ClipMachineMemory)
{
   char *msgid = _clip_parc(ClipMachineMemory, 1);

   char *msgid_plural = _clip_parc(ClipMachineMemory, 2);

   long n = _clip_parnl(ClipMachineMemory, 3);

   char *module = _clip_parc(ClipMachineMemory, 4);

   if (msgid && msgid_plural && module)
   {
      char *rp = 0;

      int l;

      _clip_locale_msg_plural(module, msgid, msgid_plural, n, &rp);
      l = strlen(rp);
      _clip_retcn_m(ClipMachineMemory, rp, l);
   }
   else if (msgid && msgid_plural)
   {
      if (n == 1)
	 _clip_retc(ClipMachineMemory, msgid);
      else
	 _clip_retc(ClipMachineMemory, msgid_plural);
   }
   else if (msgid)
      _clip_retc(ClipMachineMemory, msgid);

   return 0;
}
