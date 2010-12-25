/*
	   MSTATE() --> aState | 0
*/
int
clip_MSTATE(ClipMachine * ClipMachineMemory)
{
   ScreenBase *sp;

   long dim;

   ClipVar *rp, var;

   _clip_fullscreen(ClipMachineMemory);
   sp = ClipMachineMemory->screen->base;

   if (!sp->mouse_present)
   {
      _clip_retni(ClipMachineMemory, 0);
      return 0;
   }

   dim = 11;

   rp = RETPTR(ClipMachineMemory);
   _clip_array(ClipMachineMemory, rp, 1, &dim);

   CLEAR_CLIPVAR(&var);

   /* LLM_STATE_X */
   _clip_var_num(sp->mouse_x, &var);
   dim = 0;
   _clip_aset(ClipMachineMemory, rp, &var, 1, &dim);
   _clip_destroy(ClipMachineMemory, &var);

   /* LLM_STATE_Y */
   _clip_var_num(sp->mouse_y, &var);
   dim = 1;
   _clip_aset(ClipMachineMemory, rp, &var, 1, &dim);
   _clip_destroy(ClipMachineMemory, &var);

   /* LLM_STATE_ROW */
   _clip_var_num(sp->mouse_y, &var);
   dim = 2;
   _clip_aset(ClipMachineMemory, rp, &var, 1, &dim);
   _clip_destroy(ClipMachineMemory, &var);

   /* LLM_STATE_COL */
   _clip_var_num(sp->mouse_x, &var);
   dim = 3;
   _clip_aset(ClipMachineMemory, rp, &var, 1, &dim);
   _clip_destroy(ClipMachineMemory, &var);

   /* LLM_STATE_LEFT */
   _clip_var_num((sp->mouse_buttons & MOUSE_BUTTONS_LEFT) ? 1 : 0, &var);
   dim = 4;
   _clip_aset(ClipMachineMemory, rp, &var, 1, &dim);
   _clip_destroy(ClipMachineMemory, &var);

   /* LLM_STATE_RIGHT */
   _clip_var_num((sp->mouse_buttons & MOUSE_BUTTONS_RIGHT) ? 1 : 0, &var);
   dim = 5;
   _clip_aset(ClipMachineMemory, rp, &var, 1, &dim);
   _clip_destroy(ClipMachineMemory, &var);

   /* LLM_STATE_VISIBLE */
   _clip_var_num(sp->mouse_visible, &var);
   dim = 6;
   _clip_aset(ClipMachineMemory, rp, &var, 1, &dim);
   _clip_destroy(ClipMachineMemory, &var);

   /* LLM_STATE_DRIVER */
   _clip_var_str(sp->mouse_driver, strlen(sp->mouse_driver), &var);
   dim = 7;
   _clip_aset(ClipMachineMemory, rp, &var, 1, &dim);
   _clip_destroy(ClipMachineMemory, &var);

   /* LLM_STATE_SHAPE */
   _clip_var_num(1, &var);
   dim = 8;
   _clip_aset(ClipMachineMemory, rp, &var, 1, &dim);
   _clip_destroy(ClipMachineMemory, &var);

   /* LLM_STATE_CLICKS_LEFT */
   _clip_var_num(sp->mouse_clicks_left, &var);
   sp->mouse_clicks_left = 0;
   dim = 9;
   _clip_aset(ClipMachineMemory, rp, &var, 1, &dim);
   _clip_destroy(ClipMachineMemory, &var);

   /* LLM_STATE_CLICKS_RIGHT */
   _clip_var_num(sp->mouse_clicks_right, &var);
   sp->mouse_clicks_right = 0;
   dim = 10;
   _clip_aset(ClipMachineMemory, rp, &var, 1, &dim);
   _clip_destroy(ClipMachineMemory, &var);

   return 0;
}
