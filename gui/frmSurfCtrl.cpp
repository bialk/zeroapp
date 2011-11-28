#include "frmSurfCtrl.h"
#include "frmSurfCtrlUI.h"
#include "viewsurf.h"
#include "eventhnd.h"
#include "dispview.h"
#include "frmMainDisplay.h"
#include "frmMainDisplayUI.h"
#include <Fl/Fl_File_Chooser.h>
#include "apputil/serializer.h"

frmSurfCtrl::frmSurfCtrl(frmMainDisplay *f):
  frmmain(f),
  w_w(0),
  ui(new frmSurfCtrlUI),
  editslotlinks(new frmEditSlotLinks){
  ui->pnl->label("Surface");
  ui->pnl->position(20,20);
  viewsurf = frmmain->dv->viewsurf.get();
}
frmSurfCtrl::~frmSurfCtrl(){}


void frmSurfCtrl::Init(){

  CONNECT(ui->viewsurf_show,frmSurfCtrl::viewsurf_show);
  CONNECT(ui->viewsurf_clipplane,frmSurfCtrl::viewsurf_clipplane);
  CONNECT(ui->viewsurf_norms,frmSurfCtrl::viewsurf_norms);
  CONNECT(ui->viewsurf_texture,frmSurfCtrl::viewsurf_texture);
  CONNECT(ui->btn_slota,frmSurfCtrl::btn_slota);
  CONNECT(ui->btn_slotb,frmSurfCtrl::btn_slotb);
  CONNECT(ui->btn_slotc,frmSurfCtrl::btn_slotc);
  CONNECT(ui->btn_slotd,frmSurfCtrl::btn_slotd);
  CONNECT(ui->btn_slote,frmSurfCtrl::btn_slote);
  CONNECT(ui->btn_slotf,frmSurfCtrl::btn_slotf);
  CONNECT(ui->btn_edit_slot_links,frmSurfCtrl::btn_edit_slot_links);

  CONNECT(editslotlinks->edit_slotfilea,frmSurfCtrl::edit_slotfilea);
  CONNECT(editslotlinks->btn_slotfilea,frmSurfCtrl::btn_slotfilea);
  CONNECT(editslotlinks->edit_slotfileb,frmSurfCtrl::edit_slotfileb);
  CONNECT(editslotlinks->btn_slotfileb,frmSurfCtrl::btn_slotfileb);
  CONNECT(editslotlinks->edit_slotfilec,frmSurfCtrl::edit_slotfilec);
  CONNECT(editslotlinks->btn_slotfilec,frmSurfCtrl::btn_slotfilec);
  CONNECT(editslotlinks->edit_slotfiled,frmSurfCtrl::edit_slotfiled);
  CONNECT(editslotlinks->btn_slotfiled,frmSurfCtrl::btn_slotfiled);
  CONNECT(editslotlinks->edit_slotfilee,frmSurfCtrl::edit_slotfilee);
  CONNECT(editslotlinks->btn_slotfilee,frmSurfCtrl::btn_slotfilee);
  CONNECT(editslotlinks->edit_slotfilef,frmSurfCtrl::edit_slotfilef);
  CONNECT(editslotlinks->btn_slotfilef,frmSurfCtrl::btn_slotfilef);
  CONNECT(editslotlinks->btn_editslots_close,frmSurfCtrl::btn_editslots_close);
  CONNECT(editslotlinks->edit_surftxtrfile,frmSurfCtrl::edit_surftxtrfile);
  CONNECT(editslotlinks->btn_surftxtrfile,frmSurfCtrl::btn_surftxtrfile);
  CONNECT(editslotlinks->edit_surftxtrmapfile,frmSurfCtrl::edit_surftxtrmapfile);
  CONNECT(editslotlinks->btn_surftxtrmapfile,frmSurfCtrl::btn_surftxtrmapfile);

}

void frmSurfCtrl::SyncUI(){
  ui->viewsurf_show->value(viewsurf->show);
  ui->viewsurf_clipplane->value(viewsurf->is_clip);
  ui->viewsurf_norms->value(viewsurf->norm_smooth_on);
  ui->viewsurf_texture->value(viewsurf->texture_on);

  editslotlinks->edit_slotfilea->value(viewsurf->filesurfa.c_str());
  editslotlinks->edit_slotfileb->value(viewsurf->filesurfb.c_str());
  editslotlinks->edit_slotfilec->value(viewsurf->filesurfc.c_str());
  editslotlinks->edit_slotfiled->value(viewsurf->filesurfd.c_str());
  editslotlinks->edit_slotfilee->value(viewsurf->filesurfe.c_str());
  editslotlinks->edit_slotfilef->value(viewsurf->filesurff.c_str());
  editslotlinks->edit_surftxtrfile->value(viewsurf->filetexture.c_str());
  editslotlinks->edit_surftxtrmapfile->value(viewsurf->filemaptxtr.c_str());

}


void frmSurfCtrl::TreeScan(TSOCntx* cntx){
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

void frmSurfCtrl::AskForData(Serializer *s){
  if(s->ss->storageid==SRLZ_LAYOUT){
    s->Item("w_x",Sync(&w_x));
    s->Item("w_y",Sync(&w_y));
    s->Item("w_w",Sync(&w_w));
    s->Item("w_h",Sync(&w_h));
  }
}

void frmSurfCtrl::select(int v){
  if(v){
    ui->pnl->hide();
    ui->pnl->show();
    
  }
  else{
    ui->pnl->hide();  
    editslotlinks->pnl->hide();
  }
  
}

void frmSurfCtrl::viewsurf_show(Fl_Widget* o, void* v){
  viewsurf->show = ui->viewsurf_show->value();  
  frmmain->ui->output->redraw();
}

void frmSurfCtrl::viewsurf_clipplane(Fl_Widget* o, void* v){
  viewsurf->is_clip = ui->viewsurf_clipplane->value();
  frmmain->ui->output->redraw();
}
void frmSurfCtrl::viewsurf_norms(Fl_Widget* o, void* v){
  viewsurf->norm_smooth_on = ui->viewsurf_norms->value();
  viewsurf->BuildGLList();
  frmmain->ui->output->redraw();
}
void frmSurfCtrl::viewsurf_texture(Fl_Widget* o, void* v){
  viewsurf->texture_on=ui->viewsurf_texture->value();
  viewsurf->BuildGLList();
  frmmain->ui->output->redraw();
}
void frmSurfCtrl::btn_slota(Fl_Widget* o, void* v){
  viewsurf->SwitchSlotA();
  frmmain->ui->output->redraw();
}
void frmSurfCtrl::btn_slotb(Fl_Widget* o, void* v){
  viewsurf->SwitchSlotB();
  frmmain->ui->output->redraw();
}
void frmSurfCtrl::btn_slotc(Fl_Widget* o, void* v){
  viewsurf->SwitchSlotC();
  frmmain->ui->output->redraw();
}
void frmSurfCtrl::btn_slotd(Fl_Widget* o, void* v){
  viewsurf->SwitchSlotD();
  frmmain->ui->output->redraw();
}
void frmSurfCtrl::btn_slote(Fl_Widget* o, void* v){
  viewsurf->SwitchSlotE();
  frmmain->ui->output->redraw();
}
void frmSurfCtrl::btn_slotf(Fl_Widget* o, void* v){
  viewsurf->SwitchSlotF();
  frmmain->ui->output->redraw();
}
void frmSurfCtrl::btn_edit_slot_links(Fl_Widget* o, void* v){
  editslotlinks->pnl->position(
  	  ui->pnl->x()+5,ui->pnl->y()+10);
  editslotlinks->pnl->show();
}

void frmSurfCtrl::edit_slotfilea(Fl_Widget* o, void* v){
  viewsurf->filesurfa = editslotlinks->edit_slotfilea->value();
}
void frmSurfCtrl::btn_slotfilea(Fl_Widget* o, void* v){
  const char *n = fl_file_chooser("Surface for slot A","*.srf",
				  viewsurf->filesurfa.c_str(),1);
  if(n){ 
    editslotlinks->edit_slotfilea->value(n);
    viewsurf->filesurfa = n;
  }
}
void frmSurfCtrl::edit_slotfileb(Fl_Widget* o, void* v){
  viewsurf->filesurfb = editslotlinks->edit_slotfileb->value();
}
void frmSurfCtrl::btn_slotfileb(Fl_Widget* o, void* v){
  const char *n = fl_file_chooser("Surface for slot B","*.srf",
				  viewsurf->filesurfb.c_str(),1);
  if(n){ 
    editslotlinks->edit_slotfileb->value(n);
    viewsurf->filesurfb = n;
  }
}
void frmSurfCtrl::edit_slotfilec(Fl_Widget* o, void* v){
  viewsurf->filesurfc = editslotlinks->edit_slotfilec->value();
}
void frmSurfCtrl::btn_slotfilec(Fl_Widget* o, void* v){
  const char *n = fl_file_chooser("Surface for slot C","*.srf",
				  viewsurf->filesurfc.c_str(),1);
  if(n){ 
    editslotlinks->edit_slotfilec->value(n);
    viewsurf->filesurfc = n;
  }
}
void frmSurfCtrl::edit_slotfiled(Fl_Widget* o, void* v){
  viewsurf->filesurfd = editslotlinks->edit_slotfiled->value();
}
void frmSurfCtrl::btn_slotfiled(Fl_Widget* o, void* v){
  const char *n = fl_file_chooser("Surface for slot D","*.srf",
				  viewsurf->filesurfd.c_str(),1);
  if(n){ 
    editslotlinks->edit_slotfiled->value(n);
    viewsurf->filesurfd = n;
  }
}
void frmSurfCtrl::edit_slotfilee(Fl_Widget* o, void* v){
  viewsurf->filesurfe = editslotlinks->edit_slotfilee->value();
}
void frmSurfCtrl::btn_slotfilee(Fl_Widget* o, void* v){
  const char *n = fl_file_chooser("Surface for slot E","*.srf",
				  viewsurf->filesurfe.c_str(),1);
  if(n){ 
    editslotlinks->edit_slotfilee->value(n);
    viewsurf->filesurfe = n;
  }
}
void frmSurfCtrl::edit_slotfilef(Fl_Widget* o, void* v){
  viewsurf->filesurff = editslotlinks->edit_slotfilef->value();
}
void frmSurfCtrl::btn_slotfilef(Fl_Widget* o, void* v){
  const char *n = fl_file_chooser("Surface for slot F","*.srf",
				  viewsurf->filesurff.c_str(),1);
  if(n){ 
    editslotlinks->edit_slotfilef->value(n);
    viewsurf->filesurff = n;
  }
}
void frmSurfCtrl::btn_editslots_close(Fl_Widget* o, void* v){
  editslotlinks->pnl->hide();
}
void frmSurfCtrl::edit_surftxtrfile(Fl_Widget* o, void* v){
  viewsurf->filetexture = editslotlinks->edit_surftxtrfile->value();
}
void frmSurfCtrl::btn_surftxtrfile(Fl_Widget* o, void* v){
  const char *n = fl_file_chooser("Texture image for all slots","*.ppm",
				  viewsurf->filetexture.c_str(),1);
  if(n){ 
    editslotlinks->edit_surftxtrfile->value(n);
  }
}
void frmSurfCtrl::edit_surftxtrmapfile(Fl_Widget* o, void* v){
  viewsurf->filemaptxtr  = editslotlinks->edit_surftxtrmapfile->value();
}
void frmSurfCtrl::btn_surftxtrmapfile(Fl_Widget* o, void* v){
  const char *n = fl_file_chooser("Texture Mapping File","*.txr",
				  viewsurf->filemaptxtr.c_str(),1);
  if(n){ 
    editslotlinks->edit_surftxtrmapfile->value(n);
    viewsurf->filemaptxtr = n;
  }
}
