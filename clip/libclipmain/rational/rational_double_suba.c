/*********************************************/
rational *
rational_double_suba(rational * data, double y)
{
   rational *a = rational_double_init(y);

   rational_adda(data, a);
   rational_destroy(a);
   return data;
}
