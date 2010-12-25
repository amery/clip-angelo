/*
 * ���������� ��� �������� �� ������� ���� � �������� ������� unix,
 * ������� ��� ���� �������, ��� ����������� �����������
 */

char *
_get_path(const char *fname)
{
   char *path = (char *) malloc(MAXPATHLEN);

   int i = 0, j = 0;

   for (; fname[i]; i++)
   {
      if ((path[i] = fname[i]) == '/')
	 j = i;
   }
   if (!j)
   {
      free(path);
      return NULL;
   }
   path[j] = 0;
   return path;
}
