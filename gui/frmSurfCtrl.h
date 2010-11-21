#ifndef frmSurfCtrl_h
#define frmSurfCtrl_h
#include "fltk_calllist.h"
#include <memory>

class frmSurfCtrlUI;
class frmEditSlotLinks;
class ViewSurf;
class Serializer;
class TSOCntx;
class DispView;
class frmMainDisplay;

class frmSurfCtrl{
 public:
  int w_x,w_y,w_h,w_w;

  std::auto_ptr<frmSurfCtrlUI> ui;
  std::auto_ptr<frmEditSlotLinks> editslotlinks;
  frmMainDisplay *frmmain;
  ViewSurf       *viewsurf;
  
  frmSurfCtrl(frmMainDisplay *f);
  ~frmSurfCtrl();

  void Init();
  void SyncUI();
  void TreeScan(TSOCntx* cntx);
  void AskForData(Serializer *s);  

  void select(int v);

  PUBLIC_SLOTS(frmSurfCtrl):
  void viewsurf_show(Fl_Widget* o, void* v);
  void viewsurf_clipplane(Fl_Widget* o, void* v);
  void viewsurf_norms(Fl_Widget* o, void* v);
  void viewsurf_texture(Fl_Widget* o, void* v);
  void btn_slota(Fl_Widget* o, void* v);
  void btn_slotb(Fl_Widget* o, void* v);
  void btn_slotc(Fl_Widget* o, void* v);
  void btn_slotd(Fl_Widget* o, void* v);
  void btn_slote(Fl_Widget* o, void* v);
  void btn_slotf(Fl_Widget* o, void* v);
  void btn_edit_slot_links(Fl_Widget* o, void* v);
  void edit_slotfilea(Fl_Widget* o, void* v);
  void btn_slotfilea(Fl_Widget* o, void* v);
  void edit_slotfileb(Fl_Widget* o, void* v);
  void btn_slotfileb(Fl_Widget* o, void* v);
  void edit_slotfilec(Fl_Widget* o, void* v);
  void btn_slotfilec(Fl_Widget* o, void* v);
  void edit_slotfiled(Fl_Widget* o, void* v);
  void btn_slotfiled(Fl_Widget* o, void* v);
  void edit_slotfilee(Fl_Widget* o, void* v);
  void btn_slotfilee(Fl_Widget* o, void* v);
  void edit_slotfilef(Fl_Widget* o, void* v);
  void btn_slotfilef(Fl_Widget* o, void* v);
  void btn_editslots_close(Fl_Widget* o, void* v);
  void edit_surftxtrfile(Fl_Widget* o, void* v);
  void btn_surftxtrfile(Fl_Widget* o, void* v);
  void edit_surftxtrmapfile(Fl_Widget* o, void* v);
  void btn_surftxtrmapfile(Fl_Widget* o, void* v);
};

#endif
