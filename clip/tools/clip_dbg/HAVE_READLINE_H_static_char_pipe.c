static void
char_pipe(int ch)
{
   switch (pipe_state)
   {
   case P_Norm:
      if (ch == '\n')
	 pipe_state = P_Nl;
      fputc(ch, pipe_tmp);
      break;
   case P_Nl:
      if (ch == '.')
	 pipe_state = P_NlDot;
      else
      {
	 if (ch != '\n')
	    pipe_state = P_Norm;
	 fputc(ch, pipe_tmp);
      }
      break;
   case P_NlDot:
      pipe_state = P_Norm;
      if (ch == '\n')
      {
	 fclose(pipe_tmp);
	 system(pipe_command);
	 remove(pipe_name);
	 pipe_flag = 0;
	 write(1, "\n", 1);
	 rl_forced_update_display();
      }
      else
      {
	 fputc('.', pipe_tmp);
	 fputc(ch, pipe_tmp);
      }
      break;
   }

}
