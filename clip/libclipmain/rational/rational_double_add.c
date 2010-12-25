/***********************************************/
rational *
rational_double_add(rational * data, double y)
{
   rational *ret = rational_copy(data);

   rational *a = rational_double_init(y);

   rational_adda(ret, a);
   rational_destroy(a);
   return ret;
}
