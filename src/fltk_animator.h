#ifndef fltk_animator_h
#define fltk_animator_h


class fltk_animator{
protected:
  int n;
  sigc::signal<void, fltk_animator*> callback;

  static void repeate(void *obj){
    fltk_animator *a = (fltk_animator*)obj;

    int oldn=a->n;
    a->callback(a);
    if(oldn==a->n) { 
      delete a; // end of animation
      //printf("animator removed\n");
    }
  }

public:

  fltk_animator(sigc::slot<void, fltk_animator*> const &slot){
    n=0; 
    callback.connect(slot);
    callback(this);
  }
  int counter(){ return n;}
  void repeate(float timeout = 0.1){
    Fl::repeat_timeout(timeout, repeate, this);    
    n++;
  }  
};

#endif //fltk_animator_h
