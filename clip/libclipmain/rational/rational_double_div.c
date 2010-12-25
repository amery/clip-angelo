/***********************************************/
rational *
rational_double_div(rational * data, double y)
{
   rational *ret = rational_copy(data);

   rational *a = rational_double_init(y);

   rational_diva(ret, a);
   rational_destroy(a);
   return ret;
}
