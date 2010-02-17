
#include <stdio.h>
#include "expat.h"

static void
startElement(void *userData, const char *name, const char **atts)
{
}

static void
endElement(void *userData, const char *name)
{
}

int
main(int argc, char *argv[])
{
  char buf[BUFSIZ];
  XML_Parser parser = XML_ParserCreate(NULL);
  int done;
  int depth = 0;
  XML_SetUserData(parser, &depth);
  XML_SetElementHandler(parser, startElement, endElement);
  done = XML_STATUS_ERROR;
  XML_ParserFree(parser);
  return 0;
}

