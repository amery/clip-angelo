/**********************************************/
integer *
integer_assign(integer * dst, integer * src)
{
	if (dst->vec_of_integer != NULL)
		free(dst->vec_of_integer);
	dst->vec_of_integer = (INTEGER_VECTOR) malloc(src->len_of_integer * sizeof(INTEGER_HALF_TYPE));
	memcpy(dst->vec_of_integer, src->vec_of_integer, src->len_of_integer * sizeof(INTEGER_HALF_TYPE));
	dst->len_of_integer = src->len_of_integer;
	dst->sign_of_integer = src->sign_of_integer;
   return dst;
}
