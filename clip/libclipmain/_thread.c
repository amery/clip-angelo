/*
    Copyright (C) 2001  ITK
    Author   : Paul Lasarev <paul@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/
#include <string.h>

#include <ci_clip.h>
#include <ci_error.ch>
#include <ci_hashcode.h>

#ifdef USE_TASKS

#include <../libcliptask/ci_task.h>

#include <_thread/USE_TASKS_typedef_struct_start_data.h>
#include <_thread/USE_TASKS_static_task_run.c>
#include <_thread/USE_TASKS_static_task_destroy.c>
#include <_thread/USE_TASKS_clip_START.c>

#else

#include <_thread/clip_START.c>

#endif

#include <_thread/clip_TASKSTOP.c>
#include <_thread/clip_TASKSTART.c>
#include <_thread/clip_TASKID.c>
#include <_thread/clip_TASKVERSION.c>

#ifdef USE_TASKS

#include <_thread/USE_TASKS_typedef_struct_msg_data.h>
#include <_thread/USE_TASKS_static_msg_data_destroy.c>
#include <_thread/USE_TASKS_static_msg_data_new.c>

#endif

#include <_thread/clip_TASKSENDMSG.c>
#include <_thread/clip_TASKPEEKMSG.c>
#include <_thread/clip_TASKGETMSG.c>
