/* try to detected remote terminal IP address */
static int
tcp_term_addr(struct in_addr *ina)
{
   char *env_val, *ptr;

   int ret = -1;

   /* REMOTEHOST is set by telnetd */
   if ((env_val = getenv("REMOTEHOST")) != NULL && tcp_host_addr(env_val, ina) == 0)
      ret = 0;

   /* SSH_CLIENT is set by sshd but some of
      implementation doesn't 'export' it */
   if (ret == -1 && (env_val = getenv("SSH_CLIENT")) != NULL)
   {
      if ((ptr = strchr(env_val, ' ')) != NULL)
	 *ptr = '\0';
      if (tcp_host_addr(env_val, ina) == 0)
	 ret = 0;
   }

#ifdef USE_UTMP
   /* If we cannot find a proper address in environment we have
      to check UTMP entries for our current terminal */
   if (ret == -1)
   {
      struct utmp entry, *ut;

      if ((ptr = ttyname(0)) != NULL)
      {
	 if (strncmp(ptr, "/dev/", 5) == 0)
	    ptr += 5;
	 strcpy(entry.ut_line, ptr);
	 setutent();
	 if ((ut = getutline(&entry)) != NULL && ut->ut_addr != 0 && ut->ut_addr != INADDR_NONE && (ptr = inet_ntoa(*(struct in_addr *) &ut->ut_addr)) != NULL && tcp_host_addr(ptr, ina) == 0)
	    ret = 0;
	 endutent();
      }
   }
#endif
   if (ret == -1)
      ina->s_addr = INADDR_NONE;
   return ret;
}
