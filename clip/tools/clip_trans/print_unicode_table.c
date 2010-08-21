int
print_unicode_table(int *tbl)
{
   int i, j;

   for (i = 0; i < 32; i++)
   {
      for (j = 0; j < 8; j++)
      {
	 if (j > 2)
	    printf(" %c=%4d", (char) j * 32 + i, tbl[j * 32 + i]);
	 else
	    printf("   =%4d", tbl[j * 32 + i]);

      }
      printf("\n");
   }
   printf("\n");
   return 0;
}
