/*********************************************/
integer *
integer_setBit(integer * data, int ind, int value)
{
   integer *ret = integer_copy(data);

   integer_setBita(ret, ind, value);
   return ret;
}
