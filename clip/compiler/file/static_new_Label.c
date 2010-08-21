static Label *
new_Label(char *str)
{
   Label *ret;

   ret = (Label *) calloc(1, sizeof(Label));
	ret->hash_of_Label = hashstr(str + 5);
	ret->str_of_Label = str;

   return ret;
}
