/*********************************************/
integer *
integer_diva(integer * data, integer * y)
{
   integer *ost = NULL;

   integer_sdivide(data, y, &ost);
   integer_destroy(ost);
   return data;
}
