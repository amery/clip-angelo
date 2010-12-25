/*
	GETTEXT(cMsg [,cModule])->cTranslated
*/
int
clip_GETTEXT(ClipMachine * ClipMachineMemory)
{
   char *msgid = _clip_parc(ClipMachineMemory, 1);

   char *module = _clip_parc(ClipMachineMemory, 2);

   if (msgid && module)
   {
      char *rp = 0;

      int l;

      _clip_locale_msg(module, msgid, &rp);
      l = strlen(rp);
      _clip_retcn_m(ClipMachineMemory, rp, l);
   }
   else
      _clip_retc(ClipMachineMemory, msgid);

   return 0;
}
