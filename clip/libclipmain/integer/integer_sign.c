/*************************************************/
int
integer_sign(integer * data)
{
	return (integer_empty(data) ? 0 : (data->sign_of_integer ? -1 : 1));
}
