int
clip_ISWORD(ClipMachine * ClipMachineMemory)
{
   int ret = 1;

   unsigned char *ptr;

   unsigned char *s = (unsigned char *) _clip_parc(ClipMachineMemory, 1);

   if (!s || !(*s))
   {
      _clip_retl(ClipMachineMemory, 0);
      return 0;
   }
   for (ptr = s; (*ptr); ptr++)
   {
      switch (*ptr)
      {
      case '0'...'9':
	 if (ptr == s)
	    ret = 0;
	 break;
      case 'a'...'z':
      case 'A'...'Z':
      case '_':
	 break;
      default:
	 ret = 0;
	 break;
      }
      if (!ret)
	 break;
   }
   _clip_retl(ClipMachineMemory, ret);
   return 0;
}
