/***********************************************/
rational *
rational_double_mula(rational * data, double y)
{
   rational *a = rational_double_init(y);

   rational_mula(data, a);
   rational_destroy(a);
   return data;
}
