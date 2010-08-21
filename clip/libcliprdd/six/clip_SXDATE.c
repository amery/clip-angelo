int
clip_SXDATE(ClipMachine * ClipMachineMemory)
{
   ClipVar *v = _clip_par(ClipMachineMemory, 1);

   ClipMachineMemory->m6_error = 0;
   switch (_clip_parinfo(ClipMachineMemory, 1))
   {
   case DATE_type_of_ClipVarType:
      _clip_retdj(ClipMachineMemory, v->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar);
      break;
   case CHARACTER_type_of_ClipVarType:
      _clip_retdj(ClipMachineMemory, _clip_str_to_date(v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, ClipMachineMemory->date_format, ClipMachineMemory->epoch));
      break;
   case NUMERIC_type_of_ClipVarType:
      {
	 long alf, a, b, c, d, e, r, dd, mm, yy;

	 long ju = v->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;

	 alf = (long) ((ju - 1867216.25) / 36524.25);
	 a = ju + 1L + alf - alf / 4;
	 b = a + 1524;
	 c = (long) ((b - 122.1) / 365.25);
	 d = (long) (365.25 * c);
	 e = (long) ((b - d) / 30.6001);
	 dd = b - d - (long) (30.6001 * e);
	 if (e < 14)
	    mm = e - 1;
	 else
	    mm = e - 13;
	 if (mm > 2)
	    yy = c - 4716;
	 else
	    yy = c - 4715;
	 r = _clip_jdate(dd, mm, yy);
	 _clip_retdj(ClipMachineMemory, r);
	 break;
      }
   default:
      _clip_retdj(ClipMachineMemory, 0);
      break;
   }
   return 0;
}
