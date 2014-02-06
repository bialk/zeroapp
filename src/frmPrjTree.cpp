#include "frmPrjTree.h"
#include "dispview.h"
#include "imageplane.h"
#include "matting.h"
#include "frmMainDisplay.h"
#include "frmMainDisplayUI.h"
#include "apputil/serializer.h"
#include "frmViewCtrl.h"
#include "frmSurfCtrl.h"
#include "frmImagePlane.h"
#include "frmMatting.h"

frmPrjTree::frmPrjTree(frmMainDisplay *f)
  : frmmain(f)
  , frmviewctrl(new frmViewCtrl(f))
  , frmsurfctrl(new frmSurfCtrl(f))
  , frmimageplane(new frmImagePlane(f))
  , frmmatting(new frmMatting(f))
  //insert here for new panel
  , wx(-1), wy(-1),ww(-1),wh(-1)
{}

frmPrjTree::~frmPrjTree(){}

void frmPrjTree::Init(){
  frmviewctrl->Init();
  frmsurfctrl->Init();
  frmimageplane->Init();
  frmmatting->Init();
  //insert here for new panel

  ui.reset(new frmPrjTreeUI2);
  //ui->tree->activate();
  ui->tree->showroot(false);
  ui->tree->add("Scene");
  ui->tree->add("Scene/View");
  ui->tree->add("Scene/SFS Builder");
  ui->tree->add("Scene/Matting");
  ui->tree->add("Scene/Surface");
  CONNECT(ui->tree,frmPrjTree::tree_event);

  ui->win->show();
  ui->tree->show();


  //syncronize data
  SyncUI();
}


void frmPrjTree::TreeScan(TSOCntx* cntx){

  if(cntx == &TSOCntx::TSO_LayoutLoad) {
    // window positioning
    if(ww && wh){
      ui->win->resize(wx,wy,ww,wh);
    }
  }
  else if(cntx == &TSOCntx::TSO_LayoutStore) {
    wx=ui->win->x();
    wy=ui->win->y();
    ww=ui->win->w();
    wh=ui->win->h();
    //sliderh = ui.tree->h();
  }
  frmviewctrl->TreeScan(cntx);
  frmsurfctrl->TreeScan(cntx);
  frmimageplane->TreeScan(cntx);
  frmmatting->TreeScan(cntx);
  //insert here for new panel
}
void frmPrjTree::AskForData(Serializer *s){
  s->Item("wx", Sync(&wx));
  s->Item("wy", Sync(&wy));
  s->Item("ww", Sync(&ww));
  s->Item("wh", Sync(&wh));
  s->Item("sliderh", Sync(&sliderh));
  s->Item("frmviewctrl", Sync(&*frmviewctrl));
  s->Item("frmsurfctrl", Sync(&*frmsurfctrl));
  s->Item("frmimageplane", Sync(&*frmimageplane));
  s->Item("frmmatting",Sync(&*frmmatting));
  //insert here for new panel
}

void frmPrjTree::SyncUI(){
  frmviewctrl->SyncUI();
  frmsurfctrl->SyncUI();
  frmimageplane->SyncUI();
  frmmatting->SyncUI();
  //insert here for new panel
}

void
frmPrjTree::tree_event(Fl_Widget* w, void* v)
{
   Fl_Tree *tree = static_cast<Fl_Tree*>(w);
   Fl_Tree_Item *item = tree->callback_item();    // the item changed (can be NULL if more than one item was changed!)
   switch ( tree->callback_reason() ) {           // reason callback was invoked
   case     FL_TREE_REASON_OPENED:
      printf("..item was opened..\n");
      break;
   case FL_TREE_REASON_CLOSED:
      printf("..item was closed..\n");
      break;
   case FL_TREE_REASON_SELECTED:
      frmmain->eventball.lastev=Q_DEL;
      frmmain->ehq_scene.top().Handle(&frmmain->eventball);
      frmmain->ehq_scene.top().clear();

      printf("..item was selected..\n");
      if(std::string(item->label()) == "View")
         frmviewctrl->select(true);
      else if(std::string(item->label()) == "Surface")
         frmsurfctrl->select(true);
      else if(std::string(item->label()) == "SFS Builder"){
         // select ui panel
         frmimageplane->select(true);
         // add event handler to the scene queue
         frmmain->ehq_scene.top().ins_top(&frmmain->dv->imageplane->eh);
         frmmain->eventball.lastev=Q_ADD;
         frmmain->dv->imageplane->eh.Handle(&frmmain->eventball);
      }
      else if(std::string(item->label()) == "Matting"){
         frmmatting->select(true);
         frmmain->ehq_scene.top().ins_top(&frmmain->dv->matting->eh);
         frmmain->eventball.lastev=Q_ADD;
         frmmain->dv->matting->eh.Handle(&frmmain->eventball);
      }

      break;
      //case FL_TREE_REASON_RESELECTED:
      //  printf("..item was reselected (double-clicked, etc)..\n");
      //     break;
   case FL_TREE_REASON_DESELECTED:
      if(std::string(item->label()) == "View")
         frmviewctrl->select(false);
      else if(std::string(item->label()) == "Surface")
         frmsurfctrl->select(false);
      else if(std::string(item->label()) == "SFS Builder")
         frmimageplane->select(false);
      else if(std::string(item->label()) == "Matting")
         frmmatting->select(false);
      printf("..item was deselected..\n");
      break;
   }
}

