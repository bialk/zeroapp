#ifndef frmMatting_h
#define frmMatting_h
#ifdef _WIN32
#include <windows.h>
#endif
#include "fltk_calllist.h"
#include "frmMattingUI.h"
#include <memory>
#include <sigc++/sigc++.h>

class Matting;
class Lights;
class Serializer;
class TSOCntx;
class DispView;
class frmMainDisplay;
class FltkIdleTask;

class frmMatting{
 public:  
  int w_x,w_y,w_h,w_w;
  //int do_not_hide;

  std::auto_ptr<frmMattingUI> ui;
  frmMainDisplay *frmmain;
  DispView *dv;  
  Matting *matting;

  frmMatting(frmMainDisplay *v);
  ~frmMatting();

  void Init();
  void SyncUI();

  void TreeScan(TSOCntx* cntx);
  void AskForData(Serializer *s);  

  void select(int v);

  //FltkIdleTask idletask;

  PUBLIC_SLOTS(frmMatting):
  void imagefiledlg_src(Fl_Widget* o, void* v);
  void imagefiledlg_matte(Fl_Widget* o, void* v);
  void go_btn(Fl_Widget* o, void* v);
  void reset_btn(Fl_Widget* o, void* v);

  void show_off(Fl_Widget* o, void* v);
  void show_src(Fl_Widget* o, void* v);
  void show_src_matte(Fl_Widget* o, void* v);
  void show_matte(Fl_Widget* o, void* v);

};

#endif
