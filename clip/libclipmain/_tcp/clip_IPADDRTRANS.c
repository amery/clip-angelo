/*****************************/
int
clip_IPADDRTRANS(ClipMachine * ClipMachineMemory)
{
   char *addr = _clip_parc(ClipMachineMemory, 1), *cf = _clip_parc(ClipMachineMemory, 2);

   char *ret = "", buf[100], tmpl[30];

   unsigned long b1, b2, b3, b4, nf;

   if (addr != NULL)
   {

      nf = sscanf(addr, "%lu%*[.]%lu%*[.]%lu%*[.]%lu", &b1, &b2, &b3, &b4);
      if (nf == 4)
      {
	 if (cf == NULL)
	    cf = "";
	 snprintf(tmpl, sizeof(tmpl), "%%%sd.%%%sd.%%%sd.%%%sd", cf, cf, cf, cf);
	 snprintf(buf, sizeof(buf), tmpl, b1, b2, b3, b4);
	 ret = buf;
      }
   }

   _clip_retc(ClipMachineMemory, ret);
   return 0;
}
