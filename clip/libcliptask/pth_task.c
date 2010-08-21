/*
   Copyright (C) 2007  ITK
   Author   : Uri <uri@itk.ru>
   License : (GPL) http://www.itk.ru/clipper/license.html
 */
/*
 $Log: pth_task.c,v $
 Revision 1.9  2007/05/07 06:05:17  itk
 uri: HZ:).

 Revision 1.7  2007/02/12 16:13:08  itk
 uri: some fix about task_select and some fix for amd64

 Revision 1.6  2007/02/08 11:50:00  itk
 uri: new task model based on PTH .... finished (may be finished :))

 Revision 1.5  2007/01/31 13:48:21  itk
 uri:some new code for pth

 Revision 1.4  2007/01/30 13:43:06  itk
 *** empty log message ***

 Revision 1.3  2007/01/24 13:05:45  itk
 uri: new task model under PTH minimal works !

 Revision 1.2  2007/01/23 14:12:11  itk
 uri: some new code for new tasks

 Revision 1.1  2007/01/23 10:46:22  itk
 uri: some redisign for new task model

 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pth.h>
#include <ci_cl_cfg.h>
#include <ci_task.h>
#include <ci_task2.h>

//#define _LDEBUG
//#define _MDEBUG
#include <pth_task/struct_TaskMessage.h>
#include <pth_task/struct_Task.h>

static int seqNo = 2;		/*  �������������� */

static int pth_inited = 0;

static int stopcount = 0;

static int canSwitch = 0;	/*  task switch disabled flag */

static List Tasks = { NULL, NULL };

#include <pth_task/static_local_print_List.c>
#include <pth_task/static_seach_task_in_list.c>
#include <pth_task/static_calcTv.c>
#include <pth_task/static_task_pth_stop.c>
#include <pth_task/static_task_pth_start.c>
#include <pth_task/Task_INIT.c>
#include <pth_task/Task_version.c>
#include <pth_task/Task_yield.c>
#include <pth_task/Task_ID.c>
#include <pth_task/Task_get_id.c>
#include <pth_task/Task_sleep.c>
#include <pth_task/Task_start_sheduler.c>
#include <pth_task/Task_stop_sheduler.c>
#include <pth_task/task_pth_run.c>
#include <pth_task/Task_new.c>
#include <pth_task/Task_start.c>
#include <pth_task/Task_STOP.c>
#include <pth_task/Task_START.c>
#include <pth_task/task_select.c>
#include <pth_task/clip_task_select_if.c>
#include <pth_task/Task_respond.c>
#include <pth_task/__Task_sendMessage.c>
#include <pth_task/Task_sendMessage.c>
#include <pth_task/Task_sendMessageWait.c>
#include <pth_task/TaskMessage_get_data.c>
#include <pth_task/TaskMessage_new.c>
#include <pth_task/Task_peekMessage.c>
#include <pth_task/Task_getMessage.c>
