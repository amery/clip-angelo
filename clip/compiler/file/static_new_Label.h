static Label *
new_Label(char *str)
{
   Label    *ret;

   ret = (Label *) calloc(1, sizeof(Label));
   ret->hash = hashstr(str + 5);
   ret->str = str;

   return ret;
}
