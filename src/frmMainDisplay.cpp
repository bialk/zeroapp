//#include "config.h"
#include "frmMainDisplay.h"
#include "apputil/serializer.h"
#include "apputil/AppSysSet.h"
#include "dispview.h"
#include <FL/Fl_File_Chooser.H>
#include "fltk_animator.h"
#include "fltk_defs.h"
#include "frmMainDisplayUI.h"
#include <math.h>
#include <pthread.h>

#include "viewctrl.h"
#include "lights.h"
#include "imageplane.h"

//GLDisplay
//===================================================================


void GLDisplay::draw(void){
  frmMainDisplay *frm = 
    ((MainDisplay *)((Fl_Double_Window*)parent())->
     user_data())->frmmaindisplay;

  if(frm->dv->redisplay==-1) return;

  if(!valid()){
    // resize window event
    frm->eventball.y=h();
    frm->eventball.x=w();
    handle(-34);
  }

  frm->dv->Draw(0);
}

int GLDisplay::handle(int event){

  frmMainDisplay *frm = 
    ((MainDisplay *)((Fl_Double_Window*)parent())->
     user_data())->frmmaindisplay;

  if(event==FL_ENTER){
    Fl::focus(this);
    return 1;
  }
  if(event==-34) {
    frm->eventball.stop();
    frm->eventball.setstate(event);
  }
  else frm->eventball.Handle(event);

  frm->dv->SelectRst();  // reset selection flag
  frm->ehq_view.Handle(&frm->eventball);
  if(event==-34) frm->eventball.setstate(0);

  return 1;
}

// frmMainDisplay
//====================================================================

void Layout::AskForData(Serializer *s){
  s->Item("mwndx",Sync(&mwndx));
  s->Item("mwndy",Sync(&mwndy));
  s->Item("mwndw",Sync(&mwndw));
  s->Item("mwndh",Sync(&mwndh));
}  


frmMainDisplay::frmMainDisplay(DispView *v):
  ui(0),
  frmprjtree(0),
  dv(v)
{  
  lout.mwndw=0;
}

frmMainDisplay::~frmMainDisplay(){
  if(ui) delete ui;
  if(frmprjtree) delete frmprjtree;
}

void frmMainDisplay::Init(){
  ui = new MainDisplay(this);

  frmprjtree = new frmPrjTree(this);
  frmprjtree->Init();

  ui->build->label("Build: "__DATE__);
  ui->mainwnd->label("3D Zero Application " PACKAGE_VERSION);
  ui->aboutwnd->label("About 3D Zero Application " PACKAGE_VERSION);
  ui->aboutwndbox->label("3D Zero Application ver. " PACKAGE_VERSION);

  CONNECT(ui->mainmenu,frmMainDisplay::mainmenu);
  CONNECT(ui->mainwnd,frmMainDisplay::quit);
  CONNECT(ui->btn_about_close,frmMainDisplay::btn_about_close);
  CONNECT(ui->btn_msgwnd_close,frmMainDisplay::btn_msgwnd_close);

  dv->redraw.connect(sigc::mem_fun(this,&frmMainDisplay::redraw));
  dv->quit.connect
    (sigc::mem_fun<void,frmMainDisplay>(this,&frmMainDisplay::quit));

  LayoutLoad();

  SyncUI();
  
  ui->mainwnd->show();
  ui->output->mode(FL_DOUBLE|FL_RGB|FL_DEPTH|FL_MULTISAMPLE);
  ui->output->show();  

  ehq_view.push(EvthList());
  ehq_view.top().ins_top(&dv->viewctrl->viewctrleh);
  ehq_view.top().ins_top(&dv->lights->lighteh);
  ehq_view.top().ins_top(&dv->dispvieweh);
  ehq_view.top().ins_top(&ehq_scene);
  ehq_scene.push(EvthList());
  // event handler for scene object should be added dynamicaly
  // while in the frmPrjTree while tree item is selected.
  // Example:
  // ehq_scene.top().ins_top(&dv->imageplane->eh);

}

void frmMainDisplay::redraw(){  ui->output->redraw();}
void frmMainDisplay::quit(){ quit(ui->mainwnd,0);}

void frmMainDisplay::SyncUI(){
  frmprjtree->SyncUI();
}


void frmMainDisplay::mainmenu(Fl_Widget* o, void* v){
  if(ui->mainmenu->mvalue()==ui->menu_new){
    PrjNew();
  }
  if(ui->mainmenu->mvalue()==ui->menu_openproject){
    PrjLoad();
  }
  else if(ui->mainmenu->mvalue()==ui->menu_saveproject){
    PrjStore();
  }
  else if(ui->mainmenu->mvalue()==ui->menu_saveproject_as){
    PrjStoreAs();
  }
  else if(ui->mainmenu->mvalue()==ui->menu_exit){
    quit(o,v);
  }
  else if(ui->mainmenu->mvalue()==ui->menu_help){
    ui->help.load("index.html");
    ui->help.show();
  }
  else if(ui->mainmenu->mvalue()==ui->menu_about){
    ui->aboutwnd->position(
	 ui->mainwnd->x()+(ui->mainwnd->w()-ui->aboutwnd->w())/2,
	 ui->mainwnd->y()+(ui->mainwnd->h()-ui->aboutwnd->h())/2);
    ui->aboutwnd->show();
  }
  else  
    printf("menu_otherwise\n");
}


void frmMainDisplay::PrjNew(){
    dv->TreeScan(&TSOCntx::TSO_ProjectNew);
    prjname.clear();
    ui->mainwnd->label(SPrintF()("3D Zero Application " PACKAGE_VERSION));
    ui->menu_saveproject->deactivate();
    SyncUI();
    ui->output->redraw();
}

void frmMainDisplay::PrjLoad(){

  Fl_File_Chooser f(".","*.psr",FL_SINGLE,"Open Project");
  f.show();  
  if(!prjname.empty()) f.value(prjname.c_str());  
  while(f.shown()) { Fl::wait(); }
  if ( f.value() != NULL ){
    StorageStreamSimpleXML ss;
    Serializer srlz(&ss);
    srlz.Item("PsurgerySumlatorProject", ::Sync(dv));
    ss.storageid=SRLZ_PROJECT;
    if(!ss.OpenForRead(f.value())){
      srlz.Load();
      ss.Close();
      dv->TreeScan(&TSOCntx::TSO_ProjectLoad);
      ui->mainwnd->label(SPrintF()("3D Zero Application " PACKAGE_VERSION":%s",
				   f.value()));
      ui->menu_saveproject->activate();
      // synchronise UI with data
      prjname=f.value();
      SyncUI();
      ui->output->redraw();
    }    
  }
}

void frmMainDisplay::PrjStore(){

  if(!prjname.empty()){
    printf("project stored\n");

    StorageStreamSimpleXML ss;
    ss.storageid=SRLZ_PROJECT;
    Serializer srlz(&ss);
    srlz.Item("PsurgerySumlatorProject", ::Sync(dv));
    if(!ss.OpenForWrite(prjname.c_str())){
      dv->TreeScan(&TSOCntx::TSO_ProjectStore);
      srlz.Store();
      ss.Close();
    }
  }
}

void frmMainDisplay::PrjStoreAs(){

  Fl_File_Chooser f( ".","*.psr",Fl_File_Chooser::SINGLE | Fl_File_Chooser::CREATE,"Save Project");
  f.show();  
  if(!prjname.empty()) f.value(prjname.c_str());
  while(f.shown()) { Fl::wait(); }

  if ( f.value() != NULL ){

    StorageStreamSimpleXML ss;
    ss.storageid=SRLZ_PROJECT;
    Serializer srlz(&ss);
    srlz.Item("PsurgerySumlatorProject", ::Sync(dv));
    if(!ss.OpenForWrite(f.value())){
      dv->TreeScan(&TSOCntx::TSO_ProjectStore);
      srlz.Store();
      ss.Close();
      prjname=f.value();
      ui->mainwnd->label(SPrintF()("3D Zero Application " PACKAGE_VERSION":%s",prjname.c_str()));
      ui->menu_saveproject->activate();
    }
  }
}


void frmMainDisplay::TreeScan(TSOCntx* cntx){
  if(cntx == &TSOCntx::TSO_LayoutLoad) {
    // windows positioning
    if(lout.mwndw){
      ui->mainwnd->resize(lout.mwndx,lout.mwndy,lout.mwndw,lout.mwndh);
    }
  }
  else if(cntx == &TSOCntx::TSO_LayoutStore) {
    lout.mwndx=ui->mainwnd->x();
    lout.mwndy=ui->mainwnd->y();
    lout.mwndw=ui->mainwnd->w();
    lout.mwndh=ui->mainwnd->h();
  }
  frmprjtree->TreeScan(cntx);
}

void frmMainDisplay::LayoutLoad(){

  //restore layout
  try{
    StorageStreamSimpleXML ss;
    Serializer srlz(&ss);
    ss.storageid=SRLZ_LAYOUT;
    srlz.Item("PsurgeryLayout", ::Sync(&lout));
    srlz.Item("frmprjtree", ::Sync(frmprjtree));
    srlz.Item("DispView",::Sync(dv));

    if(!ss.OpenForRead((appsysset.path_localset+"/layout.xml").c_str())){
      srlz.Load();
      ss.Close();
    }
  }
  catch(...){
    fl_message("Layout can not be restored. "
	       "The file " PACKAGE_NAME ".opt s corrupted. "
	       "Ignoring!");
  }

  dv->TreeScan(&TSOCntx::TSO_LayoutLoad);
  TreeScan(&TSOCntx::TSO_LayoutLoad);
}

void frmMainDisplay::LayoutStore(){
  try{
    StorageStreamSimpleXML ss;
    Serializer srlz(&ss);
    ss.storageid=SRLZ_LAYOUT;
    srlz.Item("PsurgeryLayout", ::Sync(&lout));
    srlz.Item("frmprjtree", ::Sync(frmprjtree));
    srlz.Item("DispView",::Sync(dv));

    if(!ss.OpenForWrite((appsysset.path_localset+"/layout.xml").c_str())){
      dv->TreeScan(&TSOCntx::TSO_LayoutStore);      
      TreeScan(&TSOCntx::TSO_LayoutStore);
      srlz.Store();
      ss.Close();
    }
  }
  catch(...){
    fl_message("Layout can not be stored. "
	       "Error of writing the file " PACKAGE_NAME ".opt. "
	       "Ignoring!");
  }
}


void frmMainDisplay::quit(Fl_Widget* o, void* v){  
  printf("Quit!\n");
  /*
  if(adisp.ehstack.top().modal){

    //fl_message("Please, finish current operation!");
    //new FadeErrorMsg("Please, finish current operation!");
    stackbuffer.Message("Please, finish current operation!");
    return;
  }
  */
  static int once = 1;
  if(once){
    LayoutStore();
    once = 0;
  }
  
  dv->redisplay=0;

  // hide all windows here
  Fl_Window *win;
  for(win=Fl::first_window();win; win=Fl::first_window()){
    win->hide();
  }
}

void frmMainDisplay::btn_about_close(Fl_Widget* o, void* v){  
  ui->aboutwnd->hide();
}

void frmMainDisplay::btn_msgwnd_close(Fl_Widget* o, void* v){  
  ui->msgwnd->hide();
}



//  StackBuffer
//==============================================

//static pthread_mutex_t eventlog_mutex = PTHREAD_MUTEX_INITIALIZER;

static pthread_mutex_t eventlog_mutex;

StackBuffer::StackBuffer(){

  int                   rc=0;
  pthread_mutexattr_t   mta;

  rc = pthread_mutexattr_init(&mta);
  rc = pthread_mutexattr_settype(&mta, PTHREAD_MUTEX_RECURSIVE);
  //rc = pthread_mutexattr_setname_np(&mta, "RECURSIVE ONE");
  rc = pthread_mutex_init(&eventlog_mutex, &mta);

  buffer[0]=0;
  //debugout.connect(sigc::mem_fun(this, &StackBuffer::text2buffer));
}


const char* StackBuffer::Buffer(){
  return buffer;
}


void StackBuffer::Lock(){
  pthread_mutex_lock(&eventlog_mutex);
}
void StackBuffer::Unlock(){
  pthread_mutex_unlock(&eventlog_mutex);
}

void StackBuffer::text2buffer(const char *m){
  Lock();  
  int shift=strlen(m)+1;
  int i;
  for(i=4095;i>=shift;i--)
    buffer[i]=buffer[i-shift];
  memcpy(buffer,m,shift);
  buffer[shift-1]='\n';
  buffer[4095]=0;  
  Unlock();
}


void StackBuffer::Message(const char *m){
  text2buffer("=====================\n\n");
  text2buffer(m);
}


//Message warining display 
//==============================================

int MsgGroupUI::handle(int event){
  switch( event){
  case FL_PUSH:
    if(mouse_with_state(1)){
      frmMainDisplay *frmmain = 
      	((MainDisplay *)((Fl_Double_Window*)parent())->
      	 user_data())->frmmaindisplay;
      frmmain->ui->msgwnd->show();
      frmmain->ui->msgwnd->show();
    }
  }
  return 0;
}


void frmMainDisplay::MessageFade(fltk_animator* a){
  int n = a->counter();
  Fl_Color c0 = fl_color_average( FL_WHITE,FL_BLACK, 
				  sin(M_PI*( n/20.)));
  Fl_Color c1 = fl_color_average( FL_RED,FL_BACKGROUND_COLOR, 
				  sin(M_PI*( n/20.)));
  ui->msggroupui->labelcolor(c0);
  ui->msggroupui->color(c1);
  ui->msggroupui->redraw_label();
  if(n<20) a->repeate(0.1);
}

void frmMainDisplay::Message(const char *m){
  stackbuffer.Lock();
  stackbuffer.Message(m);
  //passing event to the user interface
  ui->msgtb.text(stackbuffer.Buffer());
  ui->msgdsp->buffer(ui->msgtb);

  char s[200];  strcpy(s,m);  strtok(s,"\n\r");
  ui->msggroupui->copy_label(s);
  new fltk_animator(sigc::mem_fun(this,&frmMainDisplay::MessageFade));

  stackbuffer.Unlock();
}



