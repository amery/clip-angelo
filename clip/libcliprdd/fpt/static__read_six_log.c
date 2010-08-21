static void
_read_six_log(ClipVar * vp, char **str)
{
   vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
   *str += 6;
   vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = _rdd_ushort((unsigned char *) (*str));
   *str += 8;
}
