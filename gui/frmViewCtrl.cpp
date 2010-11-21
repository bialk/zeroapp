#include "frmViewCtrl.h"
#include "viewctrl.h"
#include "lights.h"
#include "frmMainDisplay.h"
#include "frmMainDisplayUI.h"
#include "../apputil/serializer.h"


void frmViewCtrl::TreeScan(TSOCntx* cntx){
  if(cntx == &TSOCntx::TSO_LayoutLoad) {
    if(w_w){
      ui->pnl->position(w_x,w_y);
      ui->pnl->size(w_w,w_h);
      ui->do_not_hide->value(do_not_hide);
      if(ui->do_not_hide->value()) ui->pnl->show();
    }
  }
  else if(cntx == &TSOCntx::TSO_LayoutStore) {
    w_x = ui->pnl->x();    
    w_y = ui->pnl->y();    
    w_w = ui->pnl->w();    
    w_h = ui->pnl->h();
    do_not_hide = ui->do_not_hide->value();
  }
}

void frmViewCtrl::AskForData(Serializer *s){
  if(s->ss->storageid==SRLZ_LAYOUT){
    s->Item("w_x",Sync(&w_x));
    s->Item("w_y",Sync(&w_y));
    s->Item("w_w",Sync(&w_w));
    s->Item("w_h",Sync(&w_h));
    s->Item("do_not_hide",Sync(&do_not_hide));
  }
}

frmViewCtrl::frmViewCtrl(frmMainDisplay *f):
  frmmain(f),
  dv(f->dv),
  w_w(0),
  ui(new frmViewCtrlUI){
  ui->pnl->label("View");
  ui->pnl->position(20,20);
}

frmViewCtrl::~frmViewCtrl(){}

void frmViewCtrl::Init(){
  viewctrl = dv->viewctrl.get();
  lights   = dv->lights.get();

  CONNECT(ui->backgrnd,frmViewCtrl::backgrnd);
  CONNECT(ui->showtool,frmViewCtrl::showtool);
  CONNECT(ui->drawsimple,frmViewCtrl::drawsimple);
  CONNECT(ui->orthoprojection,frmViewCtrl::orthoprojection);
  CONNECT(ui->zoom_plus,frmViewCtrl::zoom_plus);
  CONNECT(ui->zoom_minus,frmViewCtrl::zoom_minus);
  CONNECT(ui->animmethod,frmViewCtrl::animmethod);
  CONNECT(ui->animstart,frmViewCtrl::anim_start);
  CONNECT(ui->light1,frmViewCtrl::light1);
  CONNECT(ui->light2,frmViewCtrl::light2);
  CONNECT(ui->reset,frmViewCtrl::reset);
}

void frmViewCtrl::SyncUI(){
  ui->backgrnd->value(viewctrl->background);
  ui->showtool->value(dv->toolpanel->show);
  ui->drawsimple->value(viewctrl->drawsimple);
  ui->orthoprojection->value(viewctrl->prjtype);
  ui->animmethod->value(viewctrl->play_method);
  ui->light1->value(lights->show1);
  ui->light2->value(lights->show2);
}

void frmViewCtrl::select(int v){
  if(v){
    ui->pnl->hide();
    ui->pnl->show();
  }
  else{
    if(!ui->do_not_hide->value())
      ui->pnl->hide();
  }
}

void frmViewCtrl::backgrnd(Fl_Widget* o, void* v){
  viewctrl->background=ui->backgrnd->value();
  frmmain->ui->output->redraw();
}
void frmViewCtrl::showtool(Fl_Widget* o, void* v){
  dv->toolpanel->show=ui->showtool->value();    
  frmmain->ui->output->redraw();
}
void frmViewCtrl::drawsimple(Fl_Widget* o, void* v){
  viewctrl->drawsimple = ui->drawsimple->value();
  frmmain->ui->output->redraw();
}
void frmViewCtrl::orthoprojection(Fl_Widget* o, void* v){
  viewctrl->prjtype=ui->orthoprojection->value();
  viewctrl->SetProjectionMatrix();
  frmmain->ui->output->redraw();
}
void frmViewCtrl::zoom_plus(Fl_Widget* o, void* v){
  viewctrl->Zoom(0.85);
  frmmain->ui->output->redraw();
}
void frmViewCtrl::zoom_minus(Fl_Widget* o, void* v){
  viewctrl->Zoom(1.15);
  frmmain->ui->output->redraw();
}
void frmViewCtrl::animmethod(Fl_Widget* o, void* v){
  viewctrl->play_method = ui->animmethod->value();
  frmmain->ui->output->redraw();
}
void frmViewCtrl::anim_step(FltkIdleTask* t){
  if(frmmain->dv->redisplay<=0) {
    t->exit();
  }
  else {
    frmmain->ui->output->redraw();
  }
}
void frmViewCtrl::anim_start(Fl_Widget* o, void* v){
  viewctrl->fplay();
  dv->redisplay=1;
  idletask.run_idle(sigc::mem_fun(this,&frmViewCtrl::anim_step));
}
void frmViewCtrl::light1(Fl_Widget* o, void* v){
  lights->show1=ui->light1->value();
  frmmain->ui->output->redraw();
}
void frmViewCtrl::light2(Fl_Widget* o, void* v){
  lights->show2=ui->light2->value();
  frmmain->ui->output->redraw();
}
void frmViewCtrl::reset(Fl_Widget* o, void* v){
  viewctrl->reset();
  frmmain->ui->output->redraw();
}

