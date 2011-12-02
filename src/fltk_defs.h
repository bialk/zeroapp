#ifndef fltk_etc_h
#define fltk_etc_h


inline int mouse_with_state(int btn, int sca=0){
  return 
    Fl::event_button()==btn && 
    ((sca&FL_SHIFT) ? Fl::event_state(FL_SHIFT) : !Fl::event_state(FL_SHIFT) ) &&
    ((sca&FL_CTRL) ? Fl::event_state(FL_CTRL)  : !Fl::event_state(FL_CTRL) ) &&
    ((sca&FL_ALT) ? Fl::event_state(FL_ALT)   : !Fl::event_state(FL_ALT) );
}

inline int button_with_state(int btn, int sca=0){
  return 
    Fl::event_key()==btn && 
    ((sca&FL_SHIFT) ? Fl::event_state(FL_SHIFT) : !Fl::event_state(FL_SHIFT) ) &&
    ((sca&FL_CTRL) ? Fl::event_state(FL_CTRL)  : !Fl::event_state(FL_CTRL) ) &&
    ((sca&FL_ALT) ? Fl::event_state(FL_ALT)   : !Fl::event_state(FL_ALT) );
}

#endif fltk_etc_h
