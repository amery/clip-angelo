/*
   Copyright (C) 2001  ITK
   Author   : Paul Lasarev <paul@itk.ru>
   License : (GPL) http://www.itk.ru/clipper/license.html
 */
/*
 $Log: cl_task.c,v $
 Revision 1.6  2007/02/12 16:13:08  itk
 uri: some fix about task_select and some fix for amd64

 Revision 1.5  2007/01/31 13:48:21  itk
 uri:some new code for pth

 Revision 1.4  2007/01/30 13:43:06  itk
 *** empty log message ***

 Revision 1.3  2007/01/23 14:12:10  itk
 uri: some new code for new tasks

 Revision 1.2  2007/01/23 10:46:22  itk
 uri: some redisign for new task model

 Revision 1.1  2007/01/22 12:38:02  itk
 uri: initial pth support

 Revision 1.1  2006/06/22 19:35:35  itk
 uri: init sf.net repository

 Revision 1.24  2006/05/25 14:16:15  clip
 uri: small fix

 Revision 1.23  2005/11/26 16:25:48  clip
 uri: rename task_select_if -> clip_task_select_if
      It is openBsd pheatures.

 Revision 1.22  2005/07/11 09:07:23  clip
 uri: small fix fo gcc4

 Revision 1.21  2004/05/21 10:46:35  clip
 rust: another memory leak fixed

 Revision 1.20  2004/05/20 14:41:10  clip
 rust: memory leak fixed

 Revision 1.19  2003/09/18 10:52:04  clip
 uri: small fix

 Revision 1.18  2003/09/17 07:54:50  clip
 uri: some fixes for Solaris and Ukraina cp1251

 Revision 1.17  2003/09/05 12:11:53  clip
 uri: initial fixes for mingw+win32 from uri

 Revision 1.14  2003/07/03 07:39:06  clip
 new release
 paul

 Revision 1.13  2003/07/02 07:05:04  clip
 fix some memleaks
 paul

 Revision 1.12  2003/04/29 11:09:39  clip
 memleak on start()
 possibly closes #140
 paul

 Revision 1.11  2003/03/17 08:24:59  clip
 Solaris 8 patches
 paul

 Revision 1.10  2002/12/25 13:26:10  clip
 fixes for Solaris tasks
 paul

 Revision 1.9  2001/12/24 13:26:25  clip
 add TASK_DLLEXPORT modifier for task library
 paul

 Revision 1.8  2001/12/20 12:10:53  clip
 fix for OpenBSD
 paul

 Revision 1.7  2001/11/16 13:06:26  clip
 CYGWIN w32_hStdIn initialsation
 paul

 Revision 1.6  2001/11/16 11:36:01  clip
 dirty hack for w32 happiness..
 paul

 Revision 1.5  2001/11/16 08:14:34  clip
 fix for CYGWIN
 paul

 Revision 1.4  2001/11/14 06:38:02  clip
 fix crash on exit after START
 paul

 Revision 1.3  2001/11/13 11:22:12  clip
 TASKSTOP() stop task switching
 TASKSTART() start task switching
 TASKSTOP()/TASKSTART() pairs may be nested
 paul

 Revision 1.2  2001/11/13 09:31:37  clip
 START('funcname'[, arg1 ... argN])
 can now put arg1-argN to call of funcname
 paul

 Revision 1.1  2001/11/12 14:13:46  clip
 START(cFuncname) now work :)
 paul
 */

#include <stdio.h>
#include <unistd.h>
#include <setjmp.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>

#include <../include.h/ci_list.h>
#include <../include.h/ci_coll.h>
#include <../include.h/ci_hash.h>

#include <ci_cl_cfg.h>
#ifdef OS_MINGW
#include <../include.h/ci__win32.h>
#else
#include <sys/times.h>
#endif
#include <ci_task.h>
#include <ci_task2.h>

#undef TASK_STACK_MIN

#ifdef OS_LINUX
#ifdef ARCH_S390
#define TASK_STACK_MIN        65536
#else
#define TASK_STACK_MIN        16384
#endif
#endif

#ifdef OS_AIX
#define TASK_STACK_MIN     49152
#endif

#ifdef OS_SUNOS
#define TASK_STACK_MIN        16384
#endif

#ifdef OS_SOLARIS_9
#define OS_SOLARIS_8
#endif

#ifdef OS_SOLARIS_8
#define TASK_STACK_MIN        16384*2
#endif

#ifndef TASK_STACK_MIN
#define TASK_STACK_MIN 4096
#endif

#include <cl_task/struct_TaskMessage.h>

#include <cl_task/typedef_enum_TaskState.h>

#include <cl_task/struct_Task.h>

static List readyTasks = { 0, 0 };	/*  ���� ��� */
static List waitTasks = { 0, 0 };	/*  ���������� */
static List msgTasks = { 0, 0 };	/*  ��������� */
static List respTasks = { 0, 0 };	/*  ������� � ����� */
static List zombieTasks = { 0, 0 };	/*  ������ */

static HashTable *hashs;	/*  ����� � ��� */
static Coll allTasks = { 0, 0, 0, 0, 0, 0 };	/*  ������� ��� */

static Task *currTask = 0;	/*  ������� */

static Task *mainTask = 0;	/*  ������� */

static Task *sheduler = 0;	/*  ���-������ */

static int activeCount = 0;	/*  ����� ���� ���(��zombie ���sheduler) */

static int stopcount = 0;

/* ���*/
static int canSwitch = 0;	/*  ������ �������� */

static int seqNo = 0;		/*  �������������� */

static fd_set readFiles;

static fd_set writeFiles;

static fd_set exceptFiles;

static jmp_buf shedEnv, mainEnv;

static void runSheduler(Task * task);	/*  �����sheduler */

static void initTask(Task * task);	/*  ������� taskEnv */

static int initStack(Task * task);	/*  ������� �� ��� */

static void initMainTask(Task * task);	/*  ������� ������� */

static void resumeTask(Task * task, int code);	/*  ����� ����taskEnv */

static void suicide(Task * task);

/* statics */
static void callTaskRun(Task * t);

static void deathMatch(void);

static void waitEvent(int block);

static long calcWakeup(long msec);

static void addToReady(Task * task);	/*  ��������� ������������� */

static void addToWait(Task * task);

static void addToMsg(Task * task);

static void addToResp(Task * task);

static void addToZombie(Task * task);

static void removeFromList(Task * task);	/*  ����� ���, �������� ����� */

#ifdef _WIN32
static int t_select(int nfds, fd_set * readfds, fd_set * writefds, fd_set * exceptfds, struct timeval *timeout);
#else
#define t_select select
#endif

#include <cl_task/Task_INIT.c>
#include <cl_task/Task_version.c>
#include <cl_task/Task_STOP.c>
#include <cl_task/Task_START.c>
#include <cl_task/Task_ID.c>

/*  ][ Message */

#include <cl_task/TaskMessage_new.c>
#include <cl_task/TaskMessage_delete.c>
#include <cl_task/TaskMessage_get_sender.c>
#include <cl_task/TaskMessage_get_id.c>
#include <cl_task/TaskMessage_get_data.c>

/*   Message ][ public Task */

#include <cl_task/static_FD_CLR_BY.c>
#include <cl_task/static_FD_AND_BY.c>
#include <cl_task/static_FD_SET_BY.c>
#include <cl_task/static_FD_ISSET_BY.c>
#include <cl_task/static_calcTv.c>

static struct tms tms_buf;

#include <cl_task/Task_new.c>
#include <cl_task/static_initMainTask.c>
#include <cl_task/static_initStack.c>
#include <cl_task/Task_delete.c>
#include <cl_task/Task_get_id.c>
#include <cl_task/Task_get_name.c>
#include <cl_task/Task_get_count.c>
#include <cl_task/Task_spawn.c>
#include <cl_task/Task_wakeup.c>

/*  ][ static Task */

static int task_inited = 0;

#include <cl_task/Task_init.c>
#include <cl_task/Task_findTask.c>
#include <cl_task/Task_get_currTask.c>
#include <cl_task/Task_start_sheduler.c>
#include <cl_task/Task_stop_sheduler.c>
#include <cl_task/Task_start.c>
#include <cl_task/Task_kill.c>
#include <cl_task/Task_killAll.c>
#include <cl_task/Task_sleep.c>
#include <cl_task/Task_wait_read.c>
#include <cl_task/Task_wait_write.c>
#include <cl_task/static_calc_wakeup.c>
#include <cl_task/task_select.c>
#include <cl_task/clip_task_select_if.c>
#include <cl_task/Task_sendMessage.c>
#include <cl_task/Task_sendMessageWait.c>
#include <cl_task/Task_sendAll.c>
#include <cl_task/Task_peekMessage.c>
#include <cl_task/Task_getMessage.c>
#include <cl_task/Task_forward.c>
#include <cl_task/Task_respond.c>

/*   ][ private Task */

#include <cl_task/static_callTaskRun.c>

/*
 * SPARC's features
 * Constants defining a stack frame.
 */
#define WINDOWSIZE	(16*4)	/* size of window save area */
#define ARGPUSHSIZE	(6*4)	/* size of arg dump area */
#define ARGPUSH		(WINDOWSIZE+4)	/* arg dump area offset */
#define MINFRAME	(WINDOWSIZE+ARGPUSHSIZE+4)	/* min frame */

/*
 * Stack alignment macros.
 */
#define STACK_ALIGN	8
#define SA(X)	(((X)+(STACK_ALIGN-1)) & ~(STACK_ALIGN-1))

#ifdef OS_SOLARIS_8

#include <cl_task/static_call_curr_task.c>

#endif

#include <cl_task/static_initTask.c>
#include <cl_task/static_resumeTask.c>
#include <cl_task/static_suicide.c>
#include <cl_task/Task_yield.c>
#include <cl_task/static_waitEvent.c>
#include <cl_task/static_calcWakeup.c>
#include <cl_task/static_addToReady.c>
#include <cl_task/static_addToWait.c>
#include <cl_task/static_addToMsg.c>
#include <cl_task/static_addToResp.c>
#include <cl_task/static_addToZombie.c>
#include <cl_task/static_removeFromList.c>
#include <cl_task/static_runSheduler.c>
#include <cl_task/static_deathMatch.c>

#ifdef _WIN32

#ifdef OS_CYGWIN
#include <w32api/windows.h>
#else
#include <windows.h>
#endif

extern HANDLE w32_hStdIn;

#include <cl_task/static_zero_fds.c>
#include <cl_task/static_t_select.c>

#endif

/*  ][ */
