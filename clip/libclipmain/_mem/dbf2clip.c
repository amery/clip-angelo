int
dbf2clip(DbfData * dp, ClipVar * vp)
{
   if (!dp || !vp)
      return 1;
   memset(vp, 0, sizeof(ClipVar));
   switch (dp->type)
   {
   case 'C':
      vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
      vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = dp->u.c.str;
      dp->u.c.str = 0;
      vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = dp->u.c.len;
      break;
   case 'M':
      vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
      vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = dp->u.c.str;
      dp->u.c.str = 0;
      vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = dp->u.c.len;
      vp->ClipType_t_of_ClipVar.memo_of_ClipType = 1;
      break;
   case 'N':
      vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
      vp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
      /*
         vp->t.memo_of_ClipType= 1;
         vp->r.r = rational_double_init(dp->u.n);
         vp->t.len_of_ClipType = dp->u.c.len;
       */
      vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = dp->u.n;
      //vp->t.dec = dp->u.c.dec;   ???????? ��� dec � dbf ?????
      break;
   case 'L':
      vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
      vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = dp->u.l;
      break;
   case 'D':
      vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATE_type_of_ClipVarType;
      vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = dp->u.d;
      break;
   default:
      return 1;
   }
   vp->ClipType_t_of_ClipVar.len_of_ClipType = dp->len;
   vp->ClipType_t_of_ClipVar.dec_of_ClipType = dp->dec;

   return 0;
}
