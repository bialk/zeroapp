#ifndef eventlog_h
#define eventlog_h

#include <stdio.h>
#include <stdarg.h>
#include <sigc++/sigc++.h>


//Debug Out 
//==============================================
class SPrintF{
 public:
  char out[200];

  const char * operator()(const char *s, ... ){
    va_list ap;
    va_start(ap,s);
    vsprintf(out,s,ap );
    va_end(ap);
    return out;
  }
};

#define err_string(n) SPrintF()("%s:%i %s",__FILE__,__LINE__,SPrintF() n )

extern sigc::signal<void, const char*> debugout;

#define err_printf(n) debugout(err_string(n)); 


#endif
