static int
norm_handler(void *par, char *txt, int len)
{
   ParseData *dp = (ParseData *) par;

	fwrite(txt, 1, len, dp->out_of_ParseData);

   return 0;
}
