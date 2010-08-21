int
clip_SYSCMD(ClipMachine * ClipMachineMemory)
{
   char *cmd;

   int lcmd;

   char *in;

   int lin;

   ClipVar *out, *err, *cenv;

   int have_err = 0;

   char **env = 0;

   int i;

   char *pwd = 0;

#ifndef OS_MINGW
   OutBuf obuf, ebuf;

   pid_t pid;

   int status;

   int infd, outfd, errfd;

   int inpipe[2], outpipe[2], errpipe[2];

   fd_set r_fds, w_fds;

   int arg = 0, r = 0;
#endif

   cmd = _clip_parcl(ClipMachineMemory, 1, &lcmd);
   in = _clip_parcl(ClipMachineMemory, 2, &lin);
   out = _clip_par(ClipMachineMemory, 3);
   err = _clip_par(ClipMachineMemory, 4);
   cenv = _clip_vptr(_clip_par(ClipMachineMemory, 5));
   pwd = _clip_parc(ClipMachineMemory, 6);

   if (!cmd || !lcmd || !in || !out)
      return EG_ARG;

   if (out && (out->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType || out->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType == F_MSTAT_ClipFlags))
      return EG_ARG;

   if (err && (err->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType || err->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType == F_MSTAT_ClipFlags))
      return EG_ARG;

   if (err)
      have_err = 1;

   if (cenv && cenv->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
   {
      ClipVar l, r;

      char *s;

      long dims[2];

      int e1, e2;

      memset(&l, 0, sizeof(ClipVar));
      memset(&r, 0, sizeof(ClipVar));
      env = calloc(cenv->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar + 1, sizeof(char *));
      for (i = 0; i < cenv->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar; i++)
      {
	 dims[0] = i;
	 dims[1] = 0;
	 e1 = _clip_aget(ClipMachineMemory, cenv, &l, 2, dims);
	 dims[1] = 1;
	 e2 = _clip_aget(ClipMachineMemory, cenv, &r, 2, dims);
	 if (e1 || e2 || l.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType || r.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType)
	    continue;
	 s = calloc(1, l.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf + r.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf + 2);
	 memcpy(s, l.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, l.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
	 s[l.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf] = '=';
	 memcpy(s + l.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf + 1,
		r.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, r.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
	 env[i] = s;
	 _clip_destroy(ClipMachineMemory, &l);
	 _clip_destroy(ClipMachineMemory, &r);
      }
   }
#ifdef OS_MINGW
   execlp("command.com", "/c", cmd, 0);
#else
   pipe(inpipe);
   pipe(outpipe);
   if (have_err)
      pipe(errpipe);

   infd = inpipe[1];
   outfd = outpipe[0];
   if (have_err)
      errfd = errpipe[0];
   else
      errfd = -1;

   if (!(pid = fork()))
   {
      int i;

      dup2(inpipe[0], 0);
      dup2(outpipe[1], 1);
      if (have_err)
	 dup2(errpipe[1], 2);
      else
	 dup2(outpipe[1], 2);
      close(inpipe[0]);
      close(outpipe[1]);
      if (have_err)
	 close(errpipe[1]);

      for (i = 3; i < 256; i++)
	 close(i);

      setgid(getgid());
      setuid(getuid());
      if (pwd)
      {
	 if (chdir(pwd))
	    return EG_ARG;
      }

      if (env)
      {
	 execle("/bin/sh", "/bin/sh", "-c", cmd, 0, env, NULL, NULL);
	 i = 0;
	 while (env[i])
	    free(env[i++]);
	 free(env);
      }
      else
	 execlp("/bin/sh", "/bin/sh", "-c", cmd, 0, NULL);
      exit(111);
   }

   close(inpipe[0]);
   close(outpipe[1]);
   if (have_err)
      close(errpipe[1]);

   FD_ZERO(&r_fds);
   FD_ZERO(&w_fds);

   if (lin)
   {
      FD_SET(infd, &w_fds);
      arg = fcntl(infd, F_GETFL, arg);
      fcntl(infd, F_SETFL, arg | O_NONBLOCK);
   }
   else
   {
      close(infd);
      infd = -1;
   }

   FD_SET(outfd, &r_fds);
   arg = fcntl(outfd, F_GETFL, arg);
   fcntl(outfd, F_SETFL, arg | O_NONBLOCK);

   if (have_err)
   {
      FD_SET(errfd, &r_fds);
      arg = fcntl(errfd, F_GETFL, arg);
      fcntl(errfd, F_SETFL, arg | O_NONBLOCK);
   }

   init_Buf(&obuf);
   if (have_err)
      init_Buf(&ebuf);

   for (;;)
   {
      fd_set rfds, wfds;

      if ((infd < 0 || !FD_ISSET(infd, &w_fds)) && (outfd < 0 || !FD_ISSET(outfd, &r_fds)) && (!have_err || errfd < 0 || !FD_ISSET(errfd, &r_fds)))
      {
	 break;
      }

      rfds = r_fds;
      wfds = w_fds;

      r = select(FD_SETSIZE, &rfds, &wfds, 0, 0);

      if (r < 0)
	 break;

      if (infd >= 0 && FD_ISSET(infd, &wfds))
      {
	 if (lin)
	 {
	    r = write(infd, in, lin);
	    if (r <= 0)
	       goto clear_in;
	    lin -= r;
	    in += r;
	    if (!lin)
	       goto clear_in;
	 }
	 else
	  clear_in:
	 {
	    FD_CLR(infd, &w_fds);
	    close(infd);
	    infd = -1;
	 }
      }

      if (outfd >= 0 && FD_ISSET(outfd, &rfds))
      {
	 char buf[512];

	 r = read(outfd, buf, sizeof(buf));
	 if (r <= 0)
	 {
	    FD_CLR(outfd, &r_fds);
	    close(outfd);
	    outfd = -1;
	 }
	 else
	 {
	    putBuf_Buf(&obuf, buf, r);
	 }
      }

      if (have_err && errfd >= 0 && FD_ISSET(errfd, &rfds))
      {
	 char buf[512];

	 r = read(errfd, buf, sizeof(buf));
	 if (r <= 0)
	 {
	    FD_CLR(errfd, &r_fds);
	    close(errfd);
	    errfd = -1;
	 }
	 else
	 {
	    putBuf_Buf(&ebuf, buf, r);
	 }
      }

   }

   if (infd >= 0)
      close(infd);
   if (outfd >= 0)
      close(outfd);
   if (have_err && errfd >= 0)
      close(errfd);

   waitpid(pid, &status, WNOHANG);

   r = WEXITSTATUS(status);
   _clip_retni(ClipMachineMemory, r);

   free(out->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
   putByte_Buf(&obuf, 0);
   out->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = obuf.buf_of_OutBuf;
   out->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = obuf.ptr_of_OutBuf - obuf.buf_of_OutBuf - 1;

   if (have_err)
   {
      free(err->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
      putByte_Buf(&ebuf, 0);
      err->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = ebuf.buf_of_OutBuf;
      err->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = ebuf.ptr_of_OutBuf - ebuf.buf_of_OutBuf - 1;
   }
#endif
   return 0;
}
