static void
_read_six_double(ClipVar * vp, int dec, char **str)
{
   vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
   *str += 6;
   vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = *(double *) *str;
   vp->ClipType_t_of_ClipVar.dec_of_ClipType = dec;
   *str += 8;
}
