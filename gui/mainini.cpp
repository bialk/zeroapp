#include "frmMainDisplay.h"
#include "frmMainDisplayUI.h"
#include "../apputil/AppSysSet.h"
#include "editviewobj.h"

AppSysSet appsysset("bialk","zeroapp");

static void debugoutputstab(const char* s){
  std::cout << s;
}

int main(int argc, char* argv[], char *envp[]){

  debugout.connect(sigc::ptr_fun(&debugoutputstab));
  err_printf(("Debug output is connected!\n"));

  // system settings initialization
  appsysset.Init(argc,argv);

  // aplication initialization
  DispView dv;
  dv.TreeScan(&TSOCntx::TSO_Init);  

  // ui initialization
  frmMainDisplay fm(&dv);
  fm.Init();

  // user warning message subsystem
  debugout.clear();
  debugout.connect(sigc::mem_fun(&fm,&frmMainDisplay::Message));
  err_printf(("Application Started!\n"));

  //EVENT LOOP
  return Fl::run();
}


