#include "frmImagePlane.h"
#include "imageplane.h"
#include "frmMainDisplay.h"
#include "frmMainDisplayUI.h"
#include "../apputil/serializer.h"
#include <Fl/Fl_File_Chooser.h>


void frmImagePlane::TreeScan(TSOCntx* cntx){
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

void frmImagePlane::AskForData(Serializer *s){
  if(s->ss->storageid==SRLZ_LAYOUT){
    s->Item("w_x",Sync(&w_x));
    s->Item("w_y",Sync(&w_y));
    s->Item("w_w",Sync(&w_w));
    s->Item("w_h",Sync(&w_h));
  }
}

frmImagePlane::frmImagePlane(frmMainDisplay *f):
  frmmain(f),
  dv(f->dv),
  imageplane(&*dv->imageplane),
  w_w(0),
  ui(new frmImagePlaneUI){
  //ui->pnl->label("View");
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

frmImagePlane::~frmImagePlane(){}

void frmImagePlane::Init(){

  CONNECT(ui->img1,frmImagePlane::select_image);
  CONNECT(ui->img2,frmImagePlane::select_image);
  CONNECT(ui->img3,frmImagePlane::select_image);
  CONNECT(ui->img4,frmImagePlane::select_image);
  CONNECT(ui->norm_x,frmImagePlane::light_edt);
  CONNECT(ui->norm_y,frmImagePlane::light_edt);
  CONNECT(ui->norm_z,frmImagePlane::light_edt);
  CONNECT(ui->imagefile_dlg,frmImagePlane::imagefile_dlg);
  CONNECT(ui->build_shape_btn,frmImagePlane::build_shape_btn);

  CONNECT(ui->image_off,frmImagePlane::image_off);
  CONNECT(ui->image_albedo,frmImagePlane::image_albedo);
  CONNECT(ui->image_image,frmImagePlane::image_image);

  CONNECT(ui->shape_off,frmImagePlane::shape_off);
  CONNECT(ui->shape_notex,frmImagePlane::shape_notex);
  CONNECT(ui->shape_image,frmImagePlane::shape_image);
  CONNECT(ui->shape_albedo,frmImagePlane::shape_albedo);

  imageplane->eh.SyncUI.connect(sigc::mem_fun(this,&frmImagePlane::SyncUI));
}

void frmImagePlane::SyncUI(){


  switch(imageplane->image_mode){
  case ImagePlane::image_mode_off: ui->image_off->setonly();break;
  case ImagePlane::image_mode_image: ui->image_image->setonly();break;
  case ImagePlane::image_mode_albedo: ui->image_albedo->setonly();break;
  }

  switch(imageplane->shape_mode){
  case ImagePlane::shape_mode_off: ui->shape_off->setonly();break;
  case ImagePlane::shape_mode_notex: ui->shape_notex->setonly();break;
  case ImagePlane::shape_mode_image: ui->shape_image->setonly();break;
  case ImagePlane::shape_mode_albedo: ui->shape_albedo->setonly();break;
  }

  int slot = imageplane->curslot;
  switch(slot){
  case 0: ui->img1->setonly(); break;
  case 1: ui->img2->setonly(); break;
  case 2: ui->img3->setonly(); break;
  case 3: ui->img4->setonly(); break;
  }

  ui->imagefile->value(imageplane->imagefname[slot].c_str());
  ui->imagefile->position(1000);
  ui->norm_x->value(SPrintF()("%f",imageplane->lights[slot][0]));
  ui->norm_y->value(SPrintF()("%f",imageplane->lights[slot][1]));
  ui->norm_z->value(SPrintF()("%f",imageplane->lights[slot][2]));


}


void frmImagePlane::select(int v){
  if(v){
    ui->pnl->hide();
    ui->pnl->show();
  }
  else{
    ui->pnl->hide();
  }
}

void frmImagePlane::select_image(Fl_Widget* o, void* v){
  Fl_Button *btn = (Fl_Button*)o;

  unsigned char s = btn->label()[0];
  imageplane->curslot=s-(unsigned char)'1';

  SyncUI();
  frmmain->ui->output->redraw();
}

void frmImagePlane::imagefile_dlg(Fl_Widget* o, void* v){
  int slot = imageplane->curslot;

  const char * s = ".";
  if(!imageplane->imagefname[slot].empty())
   s = imageplane->imagefname[slot].c_str();
  

  Fl_File_Chooser f(s,"Image Files (*.{tiff,tif,ppm,bmp,gif,jpg,jpeg,png})",FL_SINGLE,"Open Image");
  f.show();  
  while(f.shown()) { Fl::wait(); }  
  if ( f.value() != NULL ){
    imageplane->imagefname[slot]=f.value();
    imageplane->Open(slot);
    SyncUI();
    frmmain->ui->output->redraw();
  }
};


void frmImagePlane::light_edt(Fl_Widget* o, void* v){

  int slot= imageplane->curslot;
  if(o==ui->norm_x){
    imageplane->lights[slot][0]=strtod(ui->norm_x->value(),0);
  }
  else if(o==ui->norm_y){
    imageplane->lights[slot][1]=strtod(ui->norm_y->value(),0);
  }
  else if(o==ui->norm_z){
    imageplane->lights[slot][2]=strtod(ui->norm_z->value(),0);
  }

}

void frmImagePlane::build_shape_btn(Fl_Widget* o, void* v){
  //err_printf((" build_shape_bnt is not implemented yet!"));
  imageplane->Build();
  frmmain->ui->output->redraw();
}



void frmImagePlane::image_off(Fl_Widget* o, void* v){
  imageplane->image_mode = ImagePlane::image_mode_off;
  frmmain->ui->output->redraw();
}
void frmImagePlane::image_albedo(Fl_Widget* o, void* v){
  imageplane->image_mode = ImagePlane::image_mode_albedo;
  frmmain->ui->output->redraw();
}
void frmImagePlane::image_image(Fl_Widget* o, void* v){
  imageplane->image_mode = ImagePlane::image_mode_image;
  frmmain->ui->output->redraw();
}


void frmImagePlane::shape_off(Fl_Widget* o, void* v){
  imageplane->shape_mode = ImagePlane::shape_mode_off;
  frmmain->ui->output->redraw();
}
void frmImagePlane::shape_notex(Fl_Widget* o, void* v){
  imageplane->shape_mode = ImagePlane::shape_mode_notex;
  frmmain->ui->output->redraw();
}
void frmImagePlane::shape_albedo(Fl_Widget* o, void* v){
  imageplane->shape_mode = ImagePlane::shape_mode_albedo;
  frmmain->ui->output->redraw();
}
void frmImagePlane::shape_image(Fl_Widget* o, void* v){
  imageplane->shape_mode = ImagePlane::shape_mode_image;
  frmmain->ui->output->redraw();
}
