/***********************************************/
rational *
rational_sub(rational * data, rational * y)
{
   rational *ret = rational_copy(data);

   rational_suba(ret, y);
   return ret;
}
