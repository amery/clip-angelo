/*********************************************/
integer *
integer_mod(integer * data, integer * y)
{
   integer *ret = NULL;

   integer *a = integer_copy(data);

   integer_sdivide(a, y, &ret);
   integer_destroy(a);
   return ret;
}
