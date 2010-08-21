  #translate VICLASS => CREATE CLASS
  #translate VIENDCLASS => ENDCLASS
  #translate VIEXPORT => EXPORT:
  #translate VIVAR => INSTVAR
  #translate VIMFUN => METHOD FUNCTION
  #translate VIMESSAGE => METHOD
  #translate VIINHERIT => FROM
  #translate (via) =>
  #translate (via,b) =>
  #translate (via,b,c) =>
  #translate (via,b,c,d) =>
  #translate (via,b,c,d,e) =>
  #translate (via,b,c,d,e,f) =>
  #translate (via,b,c,d,e,f,g) =>
  #translate (via,b,c,d,e,f,g,h) =>
  #translate (via,b,c,d,e,f,g,h,i) =>
  #translate (via,b,c,d,e,f,g,h,i,j) =>
  #translate (via,b,c,d,e,f,g,h,i,j,k) =>
  #translate :vinew => :new():init
#ifdef CYRT
  EXTERNAL CSYBPSRT
#endif

//      Main message sent by event handler

#define EM_NULL         0               // no action
#define EM_SHOW         1               // show window
#define EM_HIDE         2               // hide window
#define EM_SETFOCUS     3               // set new focus
#define EM_PAINT        4               // paint window
#define EM_KEY          5               // non-specific key
#define EM_MOUSE        6               // mouse message
#define EM_COMMAND      7               // execute
#define EM_TIMER        8               // timer interrupt
#define EM_ERROR        9               // system error message
#define EM_USER        10               // message originated from USER
#define EM_QUIT        11               // quit application
#define EM_CLOSE       13               // close button
#define EM_OBJKEY      14               // control object key
#define EM_OBJGOTF     15               // ditto got focus
#define EM_OBJLOSTF    16               // ditto lost focus

#define W_ISMIN       512               // for show() paint special

