/*********************************************/
integer *
rational_toInteger(rational * data)
{
   integer *ret = integer_copy(data->num);

   integer_diva(ret, data->den);
   return ret;
}
