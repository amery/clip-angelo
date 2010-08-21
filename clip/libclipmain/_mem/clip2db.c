int
clip2dbf(ClipVar * vp, DbfData * dp)
{
   if (!vp || !dp)
      return 1;
   vp = _clip_vptr(vp);
   switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
   {
   case CHARACTER_type_of_ClipVarType:
      dp->type = 'C';
      /*
         dp->u.c.str = strdup(vp->s.ClipBuf_str_of_ClipStrVar.buf);
       */
      dp->u.c.str = malloc(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf + 1);
      memcpy(dp->u.c.str, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf + 1);
      dp->u.c.len = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;

      break;
   case NUMERIC_type_of_ClipVarType:
      dp->type = 'N';
      if (vp->ClipType_t_of_ClipVar.memo_of_ClipType)
	 dp->u.n = rational_toDouble(vp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
      else
	 dp->u.n = vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
      break;
   case LOGICAL_type_of_ClipVarType:
      dp->type = 'L';
      dp->u.l = vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
      break;
   case DATE_type_of_ClipVarType:
      dp->type = 'D';
      dp->u.d = vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar;
      break;
   default:
      return 1;
   }
   dp->len = (unsigned char) vp->ClipType_t_of_ClipVar.len_of_ClipType;
   dp->dec = (unsigned char) vp->ClipType_t_of_ClipVar.dec_of_ClipType;
   dp->flags = 0;
   return 0;
}
