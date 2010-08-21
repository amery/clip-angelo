int
clip___KEYBOARD(ClipMachine * ClipMachineMemory)
{
   /* second parameters as "not clear keyboard buffer" */
   ClipVar *vp = _clip_par(ClipMachineMemory, 1);

   if (!ClipMachineMemory->fullscreen)
      return 0;

   if (!_clip_parl(ClipMachineMemory, 2) || _clip_parinfo(ClipMachineMemory, 0) == 0)
   {
      int tmp = ClipMachineMemory->lastkey;

      while (_clip_key(ClipMachineMemory, 0, 0xFF));
      ClipMachineMemory->lastkey = tmp;
      *ClipMachineMemory->kbdptr = ClipMachineMemory->kbdbuf;
		/*_clip_logg(0,"aaa1 clear kbdbuf,%p,%p",*ClipMachineMemory->kbdptr,ClipMachineMemory->kbdbuf);*/
   }

   if (vp == NULL)
      return 0;

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
   {
      unsigned char *s = (unsigned char *) vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;

      int l = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;

      int i;

      int n = *ClipMachineMemory->kbdptr - ClipMachineMemory->kbdbuf;

		/*_clip_logg(0,"aaa2 clear kbdbuf,%p,%p,%s",*ClipMachineMemory->kbdptr,ClipMachineMemory->kbdbuf,s);*/
      if ((l - n) > ClipMachineMemory->typeahead)
	 l = ClipMachineMemory->typeahead - n;

      for (i = 0, s = s + l - 1; i < l; ++i, --s, (*ClipMachineMemory->kbdptr)++)
	 **ClipMachineMemory->kbdptr = *s;
   }
   else if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
   {
      int key = _clip_parni(ClipMachineMemory, 1);

      int n = *ClipMachineMemory->kbdptr - ClipMachineMemory->kbdbuf;

		/*_clip_logg(0,"aaa3 clear kbdbuf,%p,%p,%d",*ClipMachineMemory->kbdptr,ClipMachineMemory->kbdbuf,key);*/
      if (n < ClipMachineMemory->typeahead)
      {
	 **ClipMachineMemory->kbdptr = key;
	 (*ClipMachineMemory->kbdptr)++;
      }

   }
	/*_clip_logg(0,"aaa3 clear kbdbuf,%p,%p",*ClipMachineMemory->kbdptr,ClipMachineMemory->kbdbuf);*/

   return 0;
}
