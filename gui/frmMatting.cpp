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
  ui->pnl->label("Matting Test 2");
  ui->pnl->position(20,20);
  ui->norm_x->type(FL_FLOAT_INPUT);
  ui->norm_y->type(FL_FLOAT_INPUT);
  ui->norm_z->type(FL_FLOAT_INPUT);
  ui->image_off->type(FL_RADIO_BUTTON);
  ui->image_albedo->type(FL_RADIO_BUTTON);
  ui->image_image->type(FL_RADIO_BUTTON);

  ui->shape_off->type(FL_RADIO_BUTTON);
  ui->shape_notex->type(FL_RADIO_BUTTON);
  ui->shape_albedo->type(FL_RADIO_BUTTON);
  ui->shape_image->type(FL_RADIO_BUTTON);

  ui->img1->type(FL_RADIO_BUTTON);
  ui->img2->type(FL_RADIO_BUTTON);
  ui->img3->type(FL_RADIO_BUTTON);
  ui->img4->type(FL_RADIO_BUTTON);

}

frmMatting::~frmMatting(){}

void frmMatting::Init(){

  CONNECT(ui->img1,frmMatting::select_image);
  CONNECT(ui->img2,frmMatting::select_image);
  CONNECT(ui->img3,frmMatting::select_image);
  CONNECT(ui->img4,frmMatting::select_image);
  CONNECT(ui->norm_x,frmMatting::light_edt);
  CONNECT(ui->norm_y,frmMatting::light_edt);
  CONNECT(ui->norm_z,frmMatting::light_edt);
  CONNECT(ui->imagefile_dlg,frmMatting::imagefile_dlg);
  CONNECT(ui->build_shape_btn,frmMatting::build_shape_btn);

  CONNECT(ui->image_off,frmMatting::image_off);
  CONNECT(ui->image_albedo,frmMatting::image_albedo);
  CONNECT(ui->image_image,frmMatting::image_image);

  CONNECT(ui->shape_off,frmMatting::shape_off);
  CONNECT(ui->shape_notex,frmMatting::shape_notex);
  CONNECT(ui->shape_image,frmMatting::shape_image);
  CONNECT(ui->shape_albedo,frmMatting::shape_albedo);


  matting->eh.SyncUI.connect(sigc::mem_fun(this,&frmMatting::SyncUI));
}

void frmMatting::SyncUI(){


  switch(matting->image_mode){
  case Matting::image_mode_off: ui->image_off->setonly();break;
  case Matting::image_mode_image: ui->image_image->setonly();break;
  case Matting::image_mode_albedo: ui->image_albedo->setonly();break;
  }

  switch(matting->shape_mode){
  case Matting::shape_mode_off: ui->shape_off->setonly();break;
  case Matting::shape_mode_notex: ui->shape_notex->setonly();break;
  case Matting::shape_mode_image: ui->shape_image->setonly();break;
  case Matting::shape_mode_albedo: ui->shape_albedo->setonly();break;
  }

  int slot = matting->curslot;
  switch(slot){
  case 0: ui->img1->setonly(); break;
  case 1: ui->img2->setonly(); break;
  case 2: ui->img3->setonly(); break;
  case 3: ui->img4->setonly(); break;
  }

  ui->imagefile->value(matting->imagefname[slot].c_str());
  ui->imagefile->position(1000);
  ui->norm_x->value(SPrintF()("%f",matting->lights[slot][0]));
  ui->norm_y->value(SPrintF()("%f",matting->lights[slot][1]));
  ui->norm_z->value(SPrintF()("%f",matting->lights[slot][2]));


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

void frmMatting::select_image(Fl_Widget* o, void* v){
  Fl_Button *btn = (Fl_Button*)o;

  unsigned char s = btn->label()[0];
  matting->curslot=s-(unsigned char)'1';

  SyncUI();
  frmmain->ui->output->redraw();
}

void frmMatting::imagefile_dlg(Fl_Widget* o, void* v){
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
};


void frmMatting::light_edt(Fl_Widget* o, void* v){

  int slot= matting->curslot;
  if(o==ui->norm_x){
    matting->lights[slot][0]=strtod(ui->norm_x->value(),0);
  }
  else if(o==ui->norm_y){
    matting->lights[slot][1]=strtod(ui->norm_y->value(),0);
  }
  else if(o==ui->norm_z){
    matting->lights[slot][2]=strtod(ui->norm_z->value(),0);
  }

}

void frmMatting::build_shape_btn(Fl_Widget* o, void* v){
  //err_printf((" build_shape_bnt is not implemented yet!"));
  matting->Build();
  frmmain->ui->output->redraw();
}



void frmMatting::image_off(Fl_Widget* o, void* v){
  matting->image_mode = Matting::image_mode_off;
  frmmain->ui->output->redraw();
}
void frmMatting::image_albedo(Fl_Widget* o, void* v){
  matting->image_mode = Matting::image_mode_albedo;
  frmmain->ui->output->redraw();
}
void frmMatting::image_image(Fl_Widget* o, void* v){
  matting->image_mode = Matting::image_mode_image;
  frmmain->ui->output->redraw();
}


void frmMatting::shape_off(Fl_Widget* o, void* v){
  matting->shape_mode = Matting::shape_mode_off;
  frmmain->ui->output->redraw();
}
void frmMatting::shape_notex(Fl_Widget* o, void* v){
  matting->shape_mode = Matting::shape_mode_notex;
  frmmain->ui->output->redraw();
}
void frmMatting::shape_albedo(Fl_Widget* o, void* v){
  matting->shape_mode = Matting::shape_mode_albedo;
  frmmain->ui->output->redraw();
}
void frmMatting::shape_image(Fl_Widget* o, void* v){
  matting->shape_mode = Matting::shape_mode_image;
  frmmain->ui->output->redraw();
}
