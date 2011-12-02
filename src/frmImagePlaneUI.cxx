// generated by Fast Light User Interface Designer (fluid) version 1.0107

#include "frmImagePlaneUI.h"

frmImagePlaneUI::frmImagePlaneUI() {
  Fl_Double_Window* w;
  { Fl_Double_Window* o = pnl = new Fl_Double_Window(185, 375, "Shape From Shade");
    w = o;
    o->box(FL_UP_BOX);
    o->user_data((void*)(this));
    o->align(FL_ALIGN_BOTTOM);
    { Fl_Group* o = new Fl_Group(6, 20, 80, 71, "Image");
      o->box(FL_PLASTIC_UP_FRAME);
      o->align(FL_ALIGN_TOP_LEFT);
      { Fl_Round_Button* o = image_off = new Fl_Round_Button(11, 26, 70, 20, "off");
        o->down_box(FL_ROUND_DOWN_BOX);
      }
      { Fl_Round_Button* o = image_albedo = new Fl_Round_Button(11, 46, 70, 20, "albedo");
        o->down_box(FL_ROUND_DOWN_BOX);
      }
      { Fl_Round_Button* o = image_image = new Fl_Round_Button(11, 66, 70, 20, "image");
        o->down_box(FL_ROUND_DOWN_BOX);
      }
      o->end();
    }
    { Fl_Group* o = new Fl_Group(96, 20, 80, 71, "Shape");
      o->box(FL_PLASTIC_UP_FRAME);
      o->align(FL_ALIGN_TOP_LEFT);
      { Fl_Round_Button* o = shape_off = new Fl_Round_Button(101, 22, 71, 18, "off");
        o->down_box(FL_ROUND_DOWN_BOX);
      }
      { Fl_Round_Button* o = shape_notex = new Fl_Round_Button(101, 38, 71, 19, "no tex");
        o->down_box(FL_ROUND_DOWN_BOX);
      }
      { Fl_Round_Button* o = shape_albedo = new Fl_Round_Button(101, 54, 71, 19, "albedo");
        o->down_box(FL_ROUND_DOWN_BOX);
      }
      { Fl_Round_Button* o = shape_image = new Fl_Round_Button(101, 70, 71, 18, "image");
        o->down_box(FL_ROUND_DOWN_BOX);
      }
      o->end();
    }
    { Fl_Group* o = new Fl_Group(5, 120, 170, 40, "Image Slots");
      o->box(FL_PLASTIC_UP_FRAME);
      o->align(FL_ALIGN_TOP_LEFT);
      img1 = new Fl_Button(20, 130, 30, 20, "1");
      img2 = new Fl_Button(57, 130, 30, 20, "2");
      img3 = new Fl_Button(94, 130, 30, 20, "3");
      img4 = new Fl_Button(131, 130, 30, 20, "4");
      o->end();
    }
    { Fl_Output* o = imagefile = new Fl_Output(5, 182, 175, 20, "Image File:");
      o->align(FL_ALIGN_TOP_LEFT);
    }
    imagefile_dlg = new Fl_Button(150, 202, 30, 25, "...");
    { Fl_Box* o = new Fl_Box(5, 237, 135, 85, "light direction:");
      o->box(FL_PLASTIC_UP_FRAME);
      o->align(FL_ALIGN_TOP_LEFT);
    }
    { Fl_Float_Input* o = norm_x = new Fl_Float_Input(35, 248, 90, 22, "X:");
      o->box(FL_DOWN_BOX);
      o->color(FL_BACKGROUND2_COLOR);
      o->selection_color(FL_SELECTION_COLOR);
      o->labeltype(FL_NORMAL_LABEL);
      o->labelfont(0);
      o->labelsize(14);
      o->labelcolor(FL_FOREGROUND_COLOR);
      o->align(FL_ALIGN_LEFT);
      o->when(FL_WHEN_RELEASE);
    }
    { Fl_Float_Input* o = norm_y = new Fl_Float_Input(35, 269, 90, 23, "Y:");
      o->box(FL_DOWN_BOX);
      o->color(FL_BACKGROUND2_COLOR);
      o->selection_color(FL_SELECTION_COLOR);
      o->labeltype(FL_NORMAL_LABEL);
      o->labelfont(0);
      o->labelsize(14);
      o->labelcolor(FL_FOREGROUND_COLOR);
      o->align(FL_ALIGN_LEFT);
      o->when(FL_WHEN_RELEASE);
    }
    { Fl_Float_Input* o = norm_z = new Fl_Float_Input(35, 289, 90, 23, "Z:");
      o->box(FL_DOWN_BOX);
      o->color(FL_BACKGROUND2_COLOR);
      o->selection_color(FL_SELECTION_COLOR);
      o->labeltype(FL_NORMAL_LABEL);
      o->labelfont(0);
      o->labelsize(14);
      o->labelcolor(FL_FOREGROUND_COLOR);
      o->align(FL_ALIGN_LEFT);
      o->when(FL_WHEN_RELEASE);
    }
    reset_shape_btn = new Fl_Button(5, 327, 82, 25, "Reset");
    build_shape_btn = new Fl_Button(95, 327, 82, 25, "Build");
    o->end();
    o->resizable(o);
  }
}