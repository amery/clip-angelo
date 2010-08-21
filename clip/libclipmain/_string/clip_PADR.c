int
clip_PADR(ClipMachine * ClipMachineMemory)
{
   int bl, i;

   int len = _clip_parni(ClipMachineMemory, 2);

   char *s = _clip_parc(ClipMachineMemory, 3);

   char *buf, *ret, ch;

   int t1 = _clip_parinfo(ClipMachineMemory, 1);

   ClipVar *vp = _clip_par(ClipMachineMemory, 1);

   if (len < 0)
   {
      _clip_retc(ClipMachineMemory, "");
      return 0;
   }

   if (s == NULL)
      ch = ' ';
   else
      ch = s[0];
   switch (t1)
   {
   case NUMERIC_type_of_ClipVarType:
      buf = _clip_strFunc(ClipMachineMemory, vp, len, vp->ClipType_t_of_ClipVar.dec_of_ClipType, 2);
      bl = strlen(buf);
      break;
   case DATE_type_of_ClipVarType:
      buf = _clip_date_to_str(_clip_pardj(ClipMachineMemory, 1), ClipMachineMemory->date_format);
      bl = strlen(buf);
      break;
   case CHARACTER_type_of_ClipVarType:
      buf = _clip_parcl(ClipMachineMemory, 1, &bl);
      break;
   default:
      _clip_retc(ClipMachineMemory, "");
      return 0;
   }
   ret = malloc(len + 1);
   for (i = 0; i < bl && i < len; i++)
      ret[i] = buf[i];
   for (; i < len; i++)
      ret[i] = ch;
   ret[len] = 0;
   _clip_retcn_m(ClipMachineMemory, ret, len);

   if (t1 != CHARACTER_type_of_ClipVarType)
      free(buf);
   return 0;
}
