/*********************************************/
integer *
integer_div(integer * data, integer * y)
{
   integer *ret = integer_copy(data);

   integer *ost = NULL;

   integer_sdivide(ret, y, &ost);
   integer_destroy(ost);
   return ret;
}
