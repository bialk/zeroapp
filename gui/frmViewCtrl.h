#ifndef frmViewCtrl_h
#define frmViewCtrl_h
#include "fltk_calllist.h"
#include "frmViewCtrlUI.h"
#include <memory>
#include <sigc++/sigc++.h>

class ViewCtrl;
class Lights;
class Serializer;
class TSOCntx;
class DispView;
class frmMainDisplay;
class FltkIdleTask;

class frmViewCtrl{
 public:  
  int w_x,w_y,w_h,w_w;
  int do_not_hide;

  std::auto_ptr<frmViewCtrlUI> ui;
  frmMainDisplay *frmmain;
  DispView *dv;  
  ViewCtrl *viewctrl;
  Lights   *lights;

  frmViewCtrl(frmMainDisplay *v);
  ~frmViewCtrl();

  void Init();
  void SyncUI();

  void TreeScan(TSOCntx* cntx);
  void AskForData(Serializer *s);  

  void select(int v);

  FltkIdleTask idletask;

  PUBLIC_SLOTS(frmViewCtrl):
  void backgrnd(Fl_Widget* o, void* v);
  void showtool(Fl_Widget* o, void* v);
  void drawsimple(Fl_Widget* o, void* v);
  void orthoprojection(Fl_Widget* o, void* v);
  void zoom_plus(Fl_Widget* o, void* v);
  void zoom_minus(Fl_Widget* o, void* v);
  void animmethod(Fl_Widget* o, void* v);
  void anim_start(Fl_Widget* o, void* v);
  void anim_step(FltkIdleTask *);
  void light1(Fl_Widget* o, void* v);
  void light2(Fl_Widget* o, void* v);
  void reset(Fl_Widget* o, void* v);
};

#endif
