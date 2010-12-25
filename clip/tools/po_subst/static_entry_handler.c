static int
entry_handler(void *par, char *filename, int mline, char *txt)
{
   ParseData *dp = (ParseData *) par;

	fprintf(dp->out_of_ParseData, "%s", find_msg(txt));

   return 0;
}
