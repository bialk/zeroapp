// generated by Fast Light User Interface Designer (fluid) version 1.0107

#include "frmPrjTreeUI.h"

frmPrjTreeUI::~frmPrjTreeUI() {
  delete main;
}

Fl_Double_Window* frmPrjTreeUI::Init() {
  Fl_Double_Window* w;
  { Fl_Double_Window* o = main = new Fl_Double_Window(185, 195, "Project Tree");
    w = o;
    o->user_data((void*)(this));
    o->align(FL_ALIGN_CENTER);
    { Flu_Tree_Browser* o = tree = new Flu_Tree_Browser(0, 0, 185, 195);
      o->box(FL_DOWN_BOX);
      o->color(FL_BACKGROUND_COLOR);
      o->selection_color(FL_BACKGROUND_COLOR);
      o->labeltype(FL_NORMAL_LABEL);
      o->labelfont(0);
      o->labelsize(14);
      o->labelcolor(FL_FOREGROUND_COLOR);
      o->align(FL_ALIGN_TOP);
      o->when(FL_WHEN_ENTER_KEY);
      o->end();
    }
    o->end();
    o->resizable(o);
  }
  return w;
}
