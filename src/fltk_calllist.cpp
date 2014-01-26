#include "fltk_calllist.h"
#include "FL/Fl.H"


// Fltk_CallList
//======================================================================

Fltk_CallList fltk_calllist;

void fl_callback(Fl_Widget *w, void *data){
  fltk_calllist[w]->call(w,data);
}


// FltkIdleTask
//======================================================================

FltkIdleTask::FltkIdleTask(){
  itnext=callback.slots().begin();
}

void FltkIdleTask::exit(){
  it->disconnect();
}

void FltkIdleTask::run_idle(sigc::slot<void, FltkIdleTask*> const &slot){
  if(callback.slots().begin()==callback.slots().end()) {
    Fl::add_idle(idle_call,this);
  }
  callback.slots().push_front(slot);
}

void FltkIdleTask::idle_call(void *v){
  FltkIdleTask *t = (FltkIdleTask*) v;
  if(t->callback.slots().begin()==t->callback.slots().end()) {
    Fl::remove_idle(idle_call,v);
    t->itnext=t->callback.slots().begin();
    return;
  }
  if(t->itnext==t->callback.slots().end())  
    t->itnext = t->callback.slots().begin();    
  t->it=t->itnext;
  t->itnext++;
  (*(t->it))(t);
}
