#ifndef fltk_calllist_h
#define fltk_calllist_h
#include <map>
#include <list>
#include <FL/Fl_Widget.H>
#include <sigc++/sigc++.h>


class fltk_connector_base{
public:
  virtual void call(Fl_Widget *w, void *data)=0; 
};


template<class T> class fltk_connector: public  fltk_connector_base{
public:
  T *obj;
  Fl_Widget *wdg;
  void (T::*f_ptr)(Fl_Widget *, void *);

 fltk_connector(T *w, void (T::*f)(Fl_Widget *, void *) ):obj(w),f_ptr(f){};
  virtual void call(Fl_Widget *w, void *data){
    (obj->*f_ptr)(w,data);
  } 
};


class Fltk_CallList: public std::map<Fl_Widget *, fltk_connector_base*>{};

extern Fltk_CallList fltk_calllist;

void fl_callback(Fl_Widget *w, void *data);

template<class T> void set_signal(Fl_Widget *w, T *o, void (T::*f)(Fl_Widget *, void *)){
  o->calllist.push_front(fltk_connector<T>(o,f));
  fltk_connector<T> *t = &*o->calllist.begin();
  t->wdg=w;
  t->obj=o;
  t->f_ptr=f;
  fltk_calllist[w]=t;
  w->callback(fl_callback);  
};

template<typename T> class fltk_connectors: public  std::list<fltk_connector<T> >{
 public:
  ~fltk_connectors(){
    typename std::list<fltk_connector<T> >::iterator it;
    for(it=std::list<fltk_connector<T> >::begin(); 
	it!=std::list<fltk_connector<T> >::end();it++){
      fltk_calllist.erase(it->wdg);
    }
   }
};

#define PUBLIC_SLOTS(T)   fltk_connectors<T> calllist; public
#define CONNECT(sender,func) set_signal(sender,this,&func)



class FltkIdleTask{
private:
  sigc::slot_list<sigc::slot<void, FltkIdleTask*> >::iterator it,itnext;
  sigc::signal<void, FltkIdleTask*> callback;
public:

  FltkIdleTask();
  void exit();
  void run_idle(sigc::slot<void, FltkIdleTask*> const &slot);
  static void idle_call(void *v);
};

#endif
