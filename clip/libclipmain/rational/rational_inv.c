/*********************************************/
rational *
rational_inv(rational * data)
{
   rational *ret = rational_copy(data);

   integer_inverse(ret->num);
   return ret;
}
