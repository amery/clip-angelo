/***********************************************/
rational *
rational_double_diva(rational * data, double y)
{
   rational *a = rational_double_init(y);

   rational_diva(data, a);
   rational_destroy(a);
   return data;
}
