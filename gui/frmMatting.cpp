#include "frmMatting.h"
#include "matting.h"
#include "frmMainDisplay.h"
#include "frmMainDisplayUI.h"
#include "../apputil/serializer.h"
#include <Fl/Fl_File_Chooser.h>


void frmMatting::TreeScan(TSOCntx* cntx){
  if(cntx == &TSOCntx::TSO_LayoutLoad) {
    if(w_w){
      ui->pnl->position(w_x,w_y);
      ui->pnl->size(w_w,w_h);
    }
  }
  else if(cntx == &TSOCntx::TSO_LayoutStore) {
    w_x = ui->pnl->x();    
    w_y = ui->pnl->y();    
    w_w = ui->pnl->w();    
    w_h = ui->pnl->h();
  }
}

void frmMatting::AskForData(Serializer *s){
  if(s->ss->storageid==SRLZ_LAYOUT){
    s->Item("w_x",Sync(&w_x));
    s->Item("w_y",Sync(&w_y));
    s->Item("w_w",Sync(&w_w));
    s->Item("w_h",Sync(&w_h));

  }
}

frmMatting::frmMatting(frmMainDisplay *f):
  frmmain(f),
  dv(f->dv),
  matting(&*dv->matting),
  w_w(0),
  ui(new frmMattingUI){
  ui->pnl->label("Matting");
  ui->pnl->position(20,20);
  ui->show_off->type(FL_RADIO_BUTTON);
  ui->show_src->type(FL_RADIO_BUTTON);
  ui->show_src_matte->type(FL_RADIO_BUTTON);
  ui->show_matte->type(FL_RADIO_BUTTON);

}

frmMatting::~frmMatting(){}

void frmMatting::Init(){

  CONNECT(ui->imagefiledlg_src,frmMatting::imagefiledlg_src);
  CONNECT(ui->imagefiledlg_matte,frmMatting::imagefiledlg_matte);
  CONNECT(ui->reset_btn,frmMatting::reset_btn);
  CONNECT(ui->go_btn,frmMatting::go_btn);

  CONNECT(ui->show_off,frmMatting::show_off);
  CONNECT(ui->show_src,frmMatting::show_src);
  CONNECT(ui->show_src_matte,frmMatting::show_src_matte);
  CONNECT(ui->show_matte,frmMatting::show_matte);

  matting->eh.SyncUI.connect(sigc::mem_fun(this,&frmMatting::SyncUI));
}

void frmMatting::SyncUI(){


  switch(matting->show_mode){
  case Matting::show_mode_off: ui->show_off->setonly();break;
  case Matting::show_mode_src: ui->show_src->setonly();break;
  case Matting::show_mode_src_matte: ui->show_src->setonly();break;
  case Matting::show_mode_matte: ui->show_matte->setonly();break;
  }


  ui->imagefile_src->value(matting->fname_src.c_str());
  ui->imagefile_src->position(1000);
  ui->imagefile_matte->value(matting->fname_matte.c_str());
  ui->imagefile_matte->position(1000);


}


void frmMatting::select(int v){
  if(v){
    ui->pnl->hide();
    ui->pnl->show();
  }
  else{
    ui->pnl->hide();
  }
}

void frmMatting::imagefiledlg_matte(Fl_Widget* o, void* v){
}

void frmMatting::imagefiledlg_src(Fl_Widget* o, void* v){
#if 0
  int slot = matting->curslot;

  const char * s = ".";
  if(!matting->imagefname[slot].empty())
   s = matting->imagefname[slot].c_str();
  

  Fl_File_Chooser f(s,"Image Files (*.{tiff,tif,ppm,bmp,gif,jpg,jpeg,png})",FL_SINGLE,"Open Image");
  f.show();  
  while(f.shown()) { Fl::wait(); }  
  if ( f.value() != NULL ){
    matting->imagefname[slot]=f.value();
    matting->Open(slot);
    SyncUI();
    frmmain->ui->output->redraw();
  }
#endif
};


void frmMatting::go_btn(Fl_Widget* o, void* v){
  //err_printf((" build_shape_bnt is not implemented yet!"));
  //matting->Build();
  frmmain->ui->output->redraw();
}

void frmMatting::reset_btn(Fl_Widget* o, void* v){
  //err_printf((" build_shape_bnt is not implemented yet!"));
  //matting->Build();
  frmmain->ui->output->redraw();
}



void frmMatting::show_off(Fl_Widget* o, void* v){
  matting->show_mode = Matting::show_mode_off;
  frmmain->ui->output->redraw();
}

void frmMatting::show_src(Fl_Widget* o, void* v){
  matting->show_mode = Matting::show_mode_src;
  frmmain->ui->output->redraw();
}

void frmMatting::show_src_matte(Fl_Widget* o, void* v){
  matting->show_mode = Matting::show_mode_src_matte;
  frmmain->ui->output->redraw();
}

void frmMatting::show_matte(Fl_Widget* o, void* v){
  matting->show_mode = Matting::show_mode_matte;
  frmmain->ui->output->redraw();
}
