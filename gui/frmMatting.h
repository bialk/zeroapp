#ifndef frmMatting_h
#define frmMatting_h
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
  void select_image(Fl_Widget* o, void* v);
  void imagefile_dlg(Fl_Widget* o, void* v);
  void light_edt(Fl_Widget* o, void* v);
  void build_shape_btn(Fl_Widget* o, void* v);

  void image_off(Fl_Widget* o, void* v);
  void image_albedo(Fl_Widget* o, void* v);
  void image_image(Fl_Widget* o, void* v);

  void shape_off(Fl_Widget* o, void* v);
  void shape_notex(Fl_Widget* o, void* v);
  void shape_albedo(Fl_Widget* o, void* v);
  void shape_image(Fl_Widget* o, void* v);

};

#endif
