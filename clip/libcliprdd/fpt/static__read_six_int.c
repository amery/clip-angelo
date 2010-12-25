static void
_read_six_int(ClipVar * vp, char **str)
{
   vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
   *str += 6;
   vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = (int) _rdd_uint((unsigned char *) (*str));
   *str += 8;
}
