void
assign_String(char **oldstr, char *newstr)
{
   if (*oldstr)
      free(*oldstr);
   *oldstr = newstr;
}
