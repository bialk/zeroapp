#ifndef frmMainDisplay_h
#define frmMainDisplay_h

#include "fltk_calllist.h"
#include "frmPrjTree.h"
#include "frmViewCtrl.h"
#include "frmSurfCtrl.h"
#include "../apputil/eventballfltk.h"
#include <string>
#include "eventhnd.h"


class MainDisplay;
class Serializer;
class TSOCntx;
class fltk_animator;

//  Layout
//==============================================

class Layout{
public:
  int mwndx,
    mwndy,
    mwndw,
    mwndh,

    softtissueuix,
    softtissueuiy,
    softtissueuishow;

  void AskForData(Serializer *s);
};

//  StackBuffer
//==============================================

class StackBuffer{
 private:
  char buffer[4096];
  void text2buffer(const char *m);

 public:
  StackBuffer();
  const char* Buffer();
  void Lock();
  void Unlock();
  void Message(const char *m);
};


//  frmMainDisplay
//==============================================
class DispView;
class frmMainDisplay{
 public:
  DispView    *dv;
  MainDisplay *ui;
  frmPrjTree  *frmprjtree;

  Layout lout;
  StackBuffer  stackbuffer;
  EventBallFltk eventball;

  void Init();
  void SyncUI();
  void redraw();
  void quit();

  frmMainDisplay(DispView *v);
  ~frmMainDisplay();

  EvthStack     ehq_view;
  EvthStack     ehq_scene;

  void TreeScan(TSOCntx* cntx);

  std::string prjname;

  void PrjNew();
  void PrjStore();
  void PrjStoreAs();
  void PrjLoad();
  void LayoutStore();
  void LayoutLoad();

  void Message(const char *m);
  void MessageFade(fltk_animator* a);

 PUBLIC_SLOTS(frmMainDisplay):
  void mainmenu(Fl_Widget* o, void* v);
  void quit(Fl_Widget* o, void* v);
  void btn_about_close(Fl_Widget* o, void* v);  
  void btn_msgwnd_close(Fl_Widget* o, void* v);  
};


#endif
