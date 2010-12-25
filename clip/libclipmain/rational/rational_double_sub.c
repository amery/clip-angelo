/***********************************************/
rational *
rational_double_sub(rational * data, double y)
{
   rational *ret = rational_copy(data);

   rational *a = rational_double_init(y);

   rational_suba(ret, a);
   rational_destroy(a);
   return ret;
}
