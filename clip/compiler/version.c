
/*
 	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI
 */

#include "ci_clip.h"
#include <stdio.h>
#include "ci_clipcfg.h"
#include "license.h"

void
printVersion(FILE * file)
{
   fprintf(file, "%s.%s\n", CLIP_VERSION, SEQ_NO);
   fprintf(file, "\n%s\n", CLIP_LICENSE);
}
