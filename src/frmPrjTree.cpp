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
  , ww(0)
  , wx2(-1), wy2(-1),ww2(-1),wh2(-1)
{}

frmPrjTree::~frmPrjTree(){}

void frmPrjTree::Init(){
  ui.Init();
  frmviewctrl->Init();
  frmsurfctrl->Init();
  frmimageplane->Init();
  frmmatting->Init();
  //insert here for new panel

  ui2.reset(new frmPrjTreeUI2);
  //ui2->tree->activate();
  ui2->tree->showroot(1);
  ui2->tree->root_label("Scene");
  ui2->tree->add("View");
  ui2->tree->add("SFS Builder");
  ui2->tree->add("Matting");
  ui2->tree->add("Surface");
  CONNECT(ui2->tree,frmPrjTree::tree_event2);

  ui2->win->show();
  ui2->tree->show();

  CONNECT(ui.tree,frmPrjTree::tree_event);

  // construct tree  
  ui.tree->selection_mode(FLU_SINGLE_SELECT);
  ui.tree->box( FL_DOWN_BOX );
  ui.tree->auto_branches( true );
  ui.tree->insertion_mode(FLU_INSERT_BACK);
  //ui.tree->label( "Control Tree" );  
  ui.tree->always_open( true );
  ui.tree->show_root(false);
  

  Flu_Tree_Browser::Node *n;

  n = ui.tree->add("Scene");
  n->add("View");
  {
    Flu_Tree_Browser::Node *n1 = n->add("SFS Builder");
    //n1->add("Photo2");
    //n1->add("Photo3");
    //n1->add("Photo4");

    n1->open(true);
  }
  n->add("Matting");
  n->add("Surface");

  n->open(true);
  {
    //Flu_Tree_Browser::Node *n1 = n->add("Jaw");
    //n1->add("Model");
    //n1->open(true);
  }
  
  ui.main->show();

  //syncronize data
  SyncUI();
}


void frmPrjTree::TreeScan(TSOCntx* cntx){

  if(cntx == &TSOCntx::TSO_LayoutLoad) {
    // window positioning
    if(ww && wh){
      ui.main->resize(wx,wy,ww,wh);
      ui2->win->resize(wx2,wy2,ww2,wh2);
      //ui.tree->resize(0,0,ww,sliderh);
      //ui.panel->resize(0,wh,ww,wh-sliderh);
    }
  }
  else if(cntx == &TSOCntx::TSO_LayoutStore) {
    wx=ui.main->x();
    wy=ui.main->y();
    ww=ui.main->w();
    wh=ui.main->h();

    wx2=ui2->win->x();
    wy2=ui2->win->y();
    ww2=ui2->win->w();
    wh2=ui2->win->h();

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
  s->Item("wx2", Sync(&wx2));
  s->Item("wy2", Sync(&wy2));
  s->Item("ww2", Sync(&ww2));
  s->Item("wh2", Sync(&wh2));
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
frmPrjTree::tree_event2(Fl_Widget* w, void* v)
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

void frmPrjTree::tree_event(Fl_Widget* w, void* v){
  Flu_Tree_Browser* t=(Flu_Tree_Browser*) w;
  int reason = t->callback_reason();
  Flu_Tree_Browser::Node *n = t->callback_node();

  //if( tree->num_selected() )
  //  deleteNode->activate();
  //else
  //  deleteNode->deactivate();

  switch( reason )
    {
    case FLU_HILIGHTED:
      printf( "%s hilighted\n", n->label() );
      break;

    case FLU_UNHILIGHTED:
      printf( "%s unhilighted\n", n->label() );
      break;

    case FLU_SELECTED:
      // add clean up scene queue 
      frmmain->eventball.lastev=Q_DEL;
      frmmain->ehq_scene.top().Handle(&frmmain->eventball);
      frmmain->ehq_scene.top().clear();

      printf( "%s selected\n", n->label() );
      if(!strcmp(n->label(),"View")){
	frmviewctrl->select(true);
      }
      else if(!strcmp(n->label(),"Surface")){
	frmsurfctrl->select(true);
      }
      else if(!strcmp(n->label(),"SFS Builder")){
	// select ui panel
	frmimageplane->select(true);
	// add event handler to the scene queue
	frmmain->ehq_scene.top().ins_top(&frmmain->dv->imageplane->eh);
	frmmain->eventball.lastev=Q_ADD;
	frmmain->dv->imageplane->eh.Handle(&frmmain->eventball);
      }
      else if(!strcmp(n->label(),"Matting")){
	frmmatting->select(true);
	frmmain->ehq_scene.top().ins_top(&frmmain->dv->matting->eh);
	frmmain->eventball.lastev=Q_ADD;
	frmmain->dv->matting->eh.Handle(&frmmain->eventball);
      }
      //insert here for new panel
      break;

    case FLU_UNSELECTED:
      printf( "%s unselected\n", n->label() );
      if(!strcmp(n->label(),"View")){
	frmviewctrl->select(false);
      }
      else if(!strcmp(n->label(),"Surface")){
	frmsurfctrl->select(false);
      }
      else if(!strcmp(n->label(),"SFS Builder")){
	frmimageplane->select(false);
      }
      else if(!strcmp(n->label(),"Matting")){
	frmmatting->select(false);
      }
      //insert here for new panel
      break;

    case FLU_OPENED:
      printf( "%s opened\n", n->label() );
      break;

    case FLU_CLOSED:
      printf( "%s closed\n", n->label() );
      break;

    case FLU_DOUBLE_CLICK:
      printf( "%s double-clicked\n", n->label() );
      break;

    case FLU_WIDGET_CALLBACK:
      printf( "%s widget callback\n", n->label() );
      break;

    case FLU_MOVED_NODE:
      printf( "%s moved\n", n->label() );
      break;

    case FLU_NEW_NODE:
      printf( "node '%s' added to the tree\n", n->label() );
      break;
    }  
}





