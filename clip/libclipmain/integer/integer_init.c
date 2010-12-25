/**********************************************/
integer *
integer_init()
{
   integer *ret = (integer *) calloc(sizeof(integer), 1);

	ret->sign_of_integer = 0;
	ret->len_of_integer = 0;
	ret->vec_of_integer = NULL;
   return ret;
}
