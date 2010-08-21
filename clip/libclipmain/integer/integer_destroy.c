void
integer_destroy(integer * data)
{
   if (data == NULL)
      return;
	if (data->vec_of_integer != NULL)
		free(data->vec_of_integer);
   if (data != 0)
      free(data);
   data = NULL;
}
