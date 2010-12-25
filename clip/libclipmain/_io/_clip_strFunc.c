char *
_clip_strFunc(ClipMachine * ClipMachineMemory, ClipVar * v, int len, int dec, int pad)
{
   char *buf = NULL;

   int lend = 10, decd;

   char *a, *b;

   lend = v->ClipType_t_of_ClipVar.len_of_ClipType;
   decd = v->ClipType_t_of_ClipVar.dec_of_ClipType;
   if (lend <= 0)
      lend = 10;
   if (len != -999)
   {
      if (len)
	 lend = len;
      if (dec >= 0)
	 decd = dec;
      else
	 decd = 0;
   }
   else
   {
      if (ClipMachineMemory->flags & FIXED_FLAG)
      {
	 if (decd == 0)
	    lend += ClipMachineMemory->decimals + 1;
	 else
	    lend += (ClipMachineMemory->decimals - decd);
	 decd = ClipMachineMemory->decimals;
      }
   }
   if (v->ClipType_t_of_ClipVar.memo_of_ClipType)
   {
      int _lend;

      char *_buf;

      _buf = rational_toString(v->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, 10, decd, 0);
      _lend = strlen(_buf);
      if (_lend < lend)
      {
	 buf = malloc(lend + 1);
	 memset(buf, ' ', lend);
	 memcpy(buf + (lend - _lend), _buf, _lend);
	 free(_buf);
      }
      else
      {
	 buf = _buf;
	 lend = _lend;
      }
   }
   else
   {
      buf = malloc(lend + 1);
      _clip_dtostr(buf, lend, decd, v->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar, 0);
   }
   buf[lend] = 0;
   /* padr() ==2 & padc() ==3 need left orientation */
   /* padl() ==1 & str() == 0 need right orientation */
   if (pad < 2)
      return buf;
   /* formatiing to left */
   for (a = buf, b = buf; *b; b++)
   {
      if (*b != ' ')
      {
	 *a = *b;
	 a++;
      }
   }
   *a = 0;
   return buf;
}
