/*********************************************/
void
rational_destroy(rational * data)
{
   if (data == NULL)
      return;
   integer_destroy(data->num);
   integer_destroy(data->den);
   free(data);
   return;
}
