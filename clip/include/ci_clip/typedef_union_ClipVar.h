typedef union ClipVar
{
   ClipType ClipType_t_of_ClipVar;	/* t */
   ClipNumVar ClipNumVar_n_of_ClipVar;	/* n */
   ClipLongVar ClipLongVar_lv_of_ClipVar;	/* lv */
   ClipRationalVar ClipRationalVar_r_of_ClipVar;	/* r */
   ClipStrVar ClipStrVar_s_of_ClipVar;	/* s */
   ClipLogVar ClipLogVar_l_of_ClipVar;	/* l */
   ClipDateVar ClipDateVar_d_of_ClipVar;	/* d */
   ClipObjVar ClipObjVar_o_of_ClipVar;	/* o */
   ClipCodeVar ClipCodeVar_c_of_ClipVar;	/* c */
   ClipRefVar ClipRefVar_p_of_ClipVar;	/* p */
   ClipArrVar ClipArrVar_a_of_ClipVar;	/* a */
   ClipMapVar ClipMapVar_m_of_ClipVar;	/* m */
   ClipDateTimeVar ClipDateTimeVar_dt_of_ClipVar;	/* dt */
}
ClipVar;
