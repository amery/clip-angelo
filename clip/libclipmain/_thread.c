/*
    Copyright (C) 2001  ITK
    Author   : Paul Lasarev <paul@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/
#include <string.h>

#include "ci_clip.h"
#include "ci_error.ch"
#include "ci_hashcode.h"

#ifdef USE_TASKS

#include "../libcliptask/ci_task.h"

typedef struct
{
   ClipMachine *ClipMachineMemory;
   char     *name;
   ClipVar   block;
   ClipVar  *stack;
   int       argc;
}
start_data;

static void *
task_run(void *data)
{
   start_data *sp = (start_data *) data;

   ClipMachine *ClipMachineMemory = sp->ClipMachineMemory;

   int       r;

  void     *ret;

   sp->ClipMachineMemory = new_ClipMachine(ClipMachineMemory->screen);

   if (sp->name)
      ret = _clip_clip(sp->ClipMachineMemory, sp->name, sp->argc, sp->stack, 0);
   else
      ret = _clip_eval(sp->ClipMachineMemory, &sp->block, sp->argc, sp->stack, 0);

   if (r)
      _clip_logg(0, "task_run: cannot start function '%s'", sp->name);

  /*printf("\ntask_run done\n"); */
  /*	   return r; */
return ret;
}

static void
task_destroy(void *data)
{
   start_data *sp = (start_data *) data;

   int       i;

  /*printf("\ntask_destroy\n"); */

   for (i = 0; i < sp->argc + 1; i++)
      _clip_destroy(sp->ClipMachineMemory, sp->stack + i);

   if (sp->name)
      free(sp->name);
   else
      _clip_destroy(sp->ClipMachineMemory, &sp->block);

#if 1
   delete_ClipMachine(sp->ClipMachineMemory);
#endif
   free(sp->stack);
   free(sp);
}

/*
 *    taskid := start(main_func)
 */
int
clip_START(ClipMachine * ClipMachineMemory)
{
   Task     *tp;

   char     *name, *tname = "block";

   start_data *sp;

   int       argc = ClipMachineMemory->argc - 1;

   int       i;

   ClipVar  *stack;

   static int first = 1;

   int       type;

   type = _clip_parinfo(ClipMachineMemory, 1);
   if (type != CHARACTER_type_of_ClipVarType && type != CCODE_type_of_ClipVarType && type != PCODE_type_of_ClipVarType)
      return EG_ARG;

   name = _clip_parc(ClipMachineMemory, 1);

   stack = (ClipVar *) malloc(sizeof(ClipVar) * (argc + 1));
   memset(stack, 0, sizeof(ClipVar) * (argc + 1));

   for (i = 0; i < argc; ++i)
      _clip_clone(ClipMachineMemory, stack + i, ARGPTR(ClipMachineMemory, i + 2));

   sp = (start_data *) calloc(sizeof(start_data), 1);
   sp->stack = stack;
   sp->argc = argc;
   if (name)
      tname = sp->name = strdup(name);
   else
      _clip_clone(ClipMachineMemory, &sp->block, _clip_spar(ClipMachineMemory, 1));
   sp->ClipMachineMemory = ClipMachineMemory;

   tp = Task_new(tname, 4096 * 32, sp, task_run, task_destroy);

   if (tp == NULL)
      _clip_retnl(ClipMachineMemory, -1);

   _clip_retnl(ClipMachineMemory, Task_get_id(tp));

   Task_start(tp);
   if (first)
    {
       Task_start_sheduler();
       first = 0;
    }
   Task_yield();

   return 0;
}

#else

int
clip_START(ClipMachine * ClipMachineMemory)
{
   _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "TASK sheduler not implemented for OS %s", OSNAME);
   return _clip_call_errblock(ClipMachineMemory, 1);
}

#endif

int
clip_TASKSTOP(ClipMachine * ClipMachineMemory)
{
#ifdef USE_TASKS
   Task_STOP();
#endif
   return 0;
}

int
clip_TASKSTART(ClipMachine * ClipMachineMemory)
{
#ifdef USE_TASKS
   Task_START();
#endif
   return 0;
}

/*
	TASKID() -> nID (-1 if no active tasks)
*/
int
clip_TASKID(ClipMachine * ClipMachineMemory)
{
#ifdef USE_TASKS
   _clip_retnl(ClipMachineMemory, Task_ID());
#endif
   return 0;
}

int
clip_TASKVERSION(ClipMachine * ClipMachineMemory)
{
#ifdef USE_TASKS
   _clip_retni(ClipMachineMemory, Task_version());
#endif
   return 0;
}

#ifdef USE_TASKS

typedef struct
{
   ClipMachine *ClipMachineMemory;
   ClipVar   var;
}
msg_data;

static void
msg_data_destroy(void *data)
{
   msg_data *m = (msg_data *) data;

   _clip_destroy(m->ClipMachineMemory, &m->var);
   free(m);
}

static msg_data *
msg_data_new(ClipMachine * ClipMachineMemory, ClipVar * vp)
{
   msg_data *rp;

   rp = (msg_data *) calloc(sizeof(msg_data), 1);
   rp->ClipMachineMemory = ClipMachineMemory;
   _clip_clone(ClipMachineMemory, &rp->var, vp);

   return rp;
}

#endif

/*
	TaskSendMsg( nReceiverID, vMsg [, lWaitProcessed] ) --> lResult
	send var vMsg to task with id nReceiverID
*/
int
clip_TASKSENDMSG(ClipMachine * ClipMachineMemory)
{
#ifdef USE_TASKS
   long      receiver;

   ClipVar  *vp;

   int       wait = 0, r;

   TaskMessage *tp;

   if (ClipMachineMemory->argc < 2)
      return EG_ARG;

   receiver = _clip_parnl(ClipMachineMemory, 1);
   vp = _clip_par(ClipMachineMemory, 2);
   wait = _clip_parl(ClipMachineMemory, 3);
   tp = TaskMessage_new(HASH_VarMessage, msg_data_new(ClipMachineMemory, vp), msg_data_destroy);

   if (wait)
      r = Task_sendMessageWait(receiver, tp);
   else
      r = Task_sendMessage(receiver, tp);

   _clip_retl(ClipMachineMemory, r);

#endif
   return 0;
}

/*
	TaskPeekMsg() -> vMsg|NIL
	try peek message from task input queue
	If no messages available, immediate return NIL
*/
int
clip_TASKPEEKMSG(ClipMachine * ClipMachineMemory)
{
#ifdef USE_TASKS
   TaskMessage *tp;

   tp = Task_peekMessage();
   if (tp)
    {
       msg_data *m;

       m = (msg_data *) TaskMessage_get_data(tp);
       _clip_clone(ClipMachineMemory, RETPTR(ClipMachineMemory), &m->var);
       Task_respond(tp);
    }

#endif
   return 0;
}

/*
	TaskGetMsg() -> vMsg
	get message from task input queue
	If no messages available, wait forever
*/
int
clip_TASKGETMSG(ClipMachine * ClipMachineMemory)
{
#ifdef USE_TASKS
   TaskMessage *tp;

   tp = Task_getMessage();
   if (tp)
    {
       msg_data *m;

       m = (msg_data *) TaskMessage_get_data(tp);
       _clip_clone(ClipMachineMemory, RETPTR(ClipMachineMemory), &m->var);
       Task_respond(tp);
    }

#endif
   return 0;
}
