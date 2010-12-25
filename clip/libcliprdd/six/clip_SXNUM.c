int
clip_SXNUM(ClipMachine * ClipMachineMemory)
{
   ClipVar *v = _clip_par(ClipMachineMemory, 1);

   ClipMachineMemory->m6_error = 0;
   switch (_clip_parinfo(ClipMachineMemory, 1))
   {
   case NUMERIC_type_of_ClipVarType:
      _clip_retnd(ClipMachineMemory, v->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
      break;
   case CHARACTER_type_of_ClipVarType:
      {
	 int d;

	 _clip_retnd(ClipMachineMemory, _clip_strtod(v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, &d));
	 RETPTR(ClipMachineMemory)->ClipType_t_of_ClipVar.dec_of_ClipType = max(0, v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf - d - 1);
	 break;
      }
   case DATE_type_of_ClipVarType:
      {
	 int dd, mm, yy, ww, a, b;

	 long r;

	 _clip_cdate(v->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar, &dd, &mm, &yy, &ww);

	 if (mm <= 2)
	 {
	    yy--;
	    mm += 12;
	 }
	 a = yy / 100;
	 b = 2 - a + a / 4;
	 r = ((long) (365.25 * yy - (yy ? 0.0 : 0.75)) + (long) (30.6001 * (mm + 1) + dd + 1720995 + b));
	 _clip_retnd(ClipMachineMemory, r);
	 break;
      }
   case LOGICAL_type_of_ClipVarType:
      _clip_retnd(ClipMachineMemory, v->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar);
      break;
   default:
      _clip_retnd(ClipMachineMemory, 0);
      break;
   }
   return 0;
}
