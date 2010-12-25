static void
_read_six_date(ClipVar * vp, char **str)
{
   vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATE_type_of_ClipVarType;
   *str += 6;
   vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = _rdd_uint((unsigned char *) (*str));
   *str += 8;
}
