int
clip_FOPEN(ClipMachine * ClipMachineMemory)
{
   FILE *f = NULL;

   C_FILE *cf = NULL;

   int fd = -1, ret = -1, *err = NULL;

   int mode = 0, share_mode = 0, type = FT_NONE;

   pid_t pid = -1;

   char buf[PATH_MAX];

   char *fmode = NULL, *fname = _clip_parc(ClipMachineMemory, 1);

   int m = _clip_parni(ClipMachineMemory, 2);

   int lbuf, lposix;

   if (fname == NULL)
   {
      _clip_retni(ClipMachineMemory, -1);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "FOPEN");
   }
   share_mode = m & 0xfff8;
   m &= 7;			/* read/write access */

   lposix = ((share_mode & FO_POSIX) != 0 || (FOPENMODE_FLAG & ClipMachineMemory->flags1) != 0);
   /* set fopen mode to POSIX */

   if ((share_mode & FO_NOBUFF) != 0)
      lbuf = 1;			/*((share_mode & FO_NOBUFF) != 0); */
   else
   {
      if ((share_mode & FO_BUFFERED) != 0)
	 lbuf = 0;		/*((share_mode & FO_BUFFERED) != 0); */
      else
	 lbuf = ((BUFFERING_FLAG & ClipMachineMemory->flags1) != 0);
   }

   _clip_translate_path(ClipMachineMemory, fname, buf, sizeof(buf));

   switch (m)
   {
   case 0:
      mode = O_RDONLY;		/* ���������� �� ������� */
      /*mode = O_RDWR; *//* ���� ������ ���� ��� */
      fmode = "r";		/* �exclusive ���� ����� � */
      break;			/* ���, �����, � �� �� */
   case 1:
      mode = O_WRONLY;
      /*mode = O_RDWR; */
      fmode = "w";
      break;
   case 2:
      mode = O_RDWR;
      fmode = "r+";
      break;
   default:
      _clip_retni(ClipMachineMemory, -1);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "FOPEN");
   }

   if (!strcmp(buf, "-|"))
   {				/* stdin */
      if (mode != O_RDONLY)
      {
	 errno = EACCES;
	 goto err;
      }
      fd = dup(0);
      type = FT_PIPE;
   }
   else if (!strcmp(buf, "|-"))
   {				/* stdout */
      if (mode != O_WRONLY)
      {
	 errno = EACCES;
	 goto err;
      }
      fd = dup(1);
      type = FT_PIPE;
   }
   else
   {
      int blen = strlen(buf);

      if (buf[0] == '|')
      {				/* output to child */
	 if (mode != O_WRONLY)
	 {
	    errno = EACCES;
	    goto err;
	 }
	 type = FT_PIPE;
	 memmove(buf, buf + 1, blen);
      }
      else if (buf[blen - 1] == '|')
      {				/* input from child */
	 mode = O_RDONLY;
	 /*
	    if (mode != O_RDONLY)
	    {
	    errno = EACCES;
	    goto err;
	    }
	  */
	 type = FT_PIPE;
	 buf[blen - 1] = 0;
      }
   }

   if (type == FT_NONE)
      type = FT_FILE;

   if (fd == -1)
   {
#ifndef OS_MINGW
      if (type == FT_PIPE)
      {
	 int pfd[2];

	 if (pipe(pfd) == -1)
	    goto err;

	 if ((pid = fork()) == -1)
	 {
	    close(pfd[0]);
	    close(pfd[1]);
	    goto err;
	 }
	 else if (pid == 0)
	 {			/* child */
	    int i = 0;

	    setuid(getuid());
	    setgid(getgid());
	    if (mode == O_WRONLY)
	    {			/* input from parent */
	       dup2(pfd[0], 0);
	       close(pfd[0]);
	    }
	    else if (mode == O_RDONLY)
	    {			/* output to parent */
	       dup2(pfd[1], 1);
	       dup2(pfd[1], 2);
	       close(pfd[1]);
	    }
	    for (i = getdtablesize() - 1; i >= 3; i--)
	       close(i);

	    execlp("sh", "sh", "-c", buf, NULL);
	    exit(0);
	 }

	 if (mode == O_WRONLY)
	 {			/* output to child */
	    close(pfd[0]);
	    fd = pfd[1];
	 }
	 else if (mode == O_RDONLY)
	 {			/* input from child */
	    close(pfd[1]);
	    fd = pfd[0];
	 }
	 if ((mode = fcntl(fd, F_GETFL, 0)) == -1)
	    goto err;
	 fcntl(fd, F_SETFL, mode | O_NONBLOCK);
      }
      else if (type == FT_FILE)
#else
      if (type == FT_FILE)
#endif
      {
#define FO_COMPAT	 0	/* ����������(� ����� */
#define FO_EXCLUSIVE	16	/* �������������(Exclusive) */
#define FO_DENYWRITE	32	/* ������� � ��� ��������� */
#define FO_DENYREAD	48	/* ������� � ��� ��������� */
#define FO_SHARED	64	/* ����� ��� ���� ��������� */
#define FO_DENYNONE	64	/* (� � ��� ���FO_SHARED) */
	 /*int l_type = F_WRLCK; */
	 int l_type = -1;

	 if (!lposix)
	 {
	    switch (share_mode & 0x70)
	    {
	    case FO_DENYREAD:
	    case FO_EXCLUSIVE:
	    case FO_DENYWRITE:
	       l_type = F_WRLCK;
	       break;
	    case FO_COMPAT:
	    case FO_SHARED:
	       l_type = F_RDLCK;
	       break;
	    }
	 }
#ifdef _WIN32
	 mode |= ((share_mode & FO_TEXT) != 0) ? O_TEXT : O_BINARY;
#endif
	 mode |= O_NONBLOCK;
	 mode |= ((share_mode & FO_TRUNC) != 0) ? O_TRUNC : 0;

	 fd = _clip_open(ClipMachineMemory, buf, mode, 0, l_type == F_WRLCK);
      }
   }

   if (fd != -1)
   {
      if (lbuf)
      {
	 if ((f = fdopen(fd, fmode)) == NULL)
	    goto err;
      }
      if (type == FT_FILE && m == 1)	/* O_WRONLY */
      {
	 if (lbuf)
	    fseek(f, 0, SEEK_SET);
	 else
	    lseek(fd, 0, SEEK_SET);
      }
      cf = (C_FILE *) calloc(1, sizeof(C_FILE));
      cf->fileno_of_C_FILE = fd;
      cf->file_of_C_FILE = f;
      cf->stat_of_C_FILE = (lbuf ? FS_BUFFERED : 0);
      cf->type_of_C_FILE = _clip_ftype(fd);
      cf->pid_of_C_FILE = pid;
      cf->timeout_of_C_FILE = 0;	/* (type == FT_FILE) ? 0 : -1; */
      cf->hash_of_C_FILE = _clip_hashstr(buf);
      cf->ClipMachineMemory_of_C_FILE = ClipMachineMemory;
      ret = _clip_store_c_item(ClipMachineMemory, cf, _C_ITEM_TYPE_FILE, destroy_c_file);
   }

 err:
   err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);
   if (ret != -1)
      *err = 0;
   else
   {
      *err = errno;
      if (f != NULL)
	 fclose(f);
      else if (fd != -1)
	 close(fd);
   }
   _clip_retni(ClipMachineMemory, ret);

   return 0;
}
