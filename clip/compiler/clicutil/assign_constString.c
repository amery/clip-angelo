void
assign_constString(char **oldstr, const char *newstr)
{
   if (*oldstr)
      free(*oldstr);
   *oldstr = strdup(newstr);
}
