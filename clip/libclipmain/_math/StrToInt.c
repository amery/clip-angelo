int
StrToInt(char *s, int len, int *res)
{
   unsigned int i;

   int j;

   for (i = j = 0; j < len && j < sizeof(int) * 2; j++)

      switch (s[j])
      {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
	 i = (i << 4) + (s[j] - '0');
	 break;
      case 'A':
      case 'B':
      case 'C':
      case 'D':
      case 'E':
      case 'F':
	 i = (i << 4) + s[j] - 'A' + 10;
	 break;
      case 'a':
      case 'b':
      case 'c':
      case 'd':
      case 'e':
      case 'f':
	 i = (i << 4) + s[j] - 'a' + 10;
	 break;
      default:
	 if (j == 0)
	    return j + 1;
	 else
	 {
	    *res = i;
	    return 0;
	 }
      }
   *res = i;
   return 0;
}
