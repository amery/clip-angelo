/*********************************************/
rational *
rational_double_adda(rational * data, double y)
{
   rational *a = rational_double_init(y);

   rational_adda(data, a);
   rational_destroy(a);
   return data;

}
