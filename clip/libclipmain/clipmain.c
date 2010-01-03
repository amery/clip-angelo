
/*
	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

   Revision 1.2  1999/10/26 19:11:22  paul
   start cvs logging

 */

#include "ci_clip.h"
#include "ci_hashcode.h"

/*extern void *alloca(); */
extern int printf();

extern char **environ;

int
main(int argc, char **argv)
{
   int       ret;

   ClipMachine *ClipMachineMemory = new_ClipMachine(0);

#ifdef USE_TASK
   Task_INIT();
#endif
   ret = _clip_main(ClipMachineMemory, HASH_main, argc, argv, environ);
   return ret;
}
