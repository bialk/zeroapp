#ifndef matting_h
#define matting_h

#include "editviewobj.h"
//#include "mathlib/mathutl/mymath.h"
//#include "mathlib/mathutl/meshandalg.h"
//#include "glhelper.h"
#include "eventhnd.h"
#include "glhelper.h"
#include "ImageTile.h"
#include <GL/gl.h>
#include <vector>
#include <sigc++/sigc++.h>



//================================================================
class StrokePatch;
class StrokePatchEH: public EvtHandle{
 public:
  StrokePatch *ev;
  int state_drag;
  sigc::signal<void> SyncUI;

  StrokePatchEH(StrokePatch *v);
  virtual void Handle(EventBall *eventball);
};


class StrokePatch{
 public:
  struct xy{float x; float y;};
  // point set
  int last_pset;
  int last_pnum;
  std::vector<std::vector<StrokePatch::xy> > pset;

  StrokePatch();
  void strokeStart(int x, int y);
  void strokeContinue(int x, int y);
  
  
  virtual void Draw(DrawCntx *cntx);
  void TreeScan(TSOCntx *cntx);
};


// ===============================================================
class Matting;
class MattingEH: public EvtHandle{
 public:
  Matting *ev;
  int state_drag;
  sigc::signal<void> SyncUI;

  MattingEH(Matting *v);
  virtual void Handle(EventBall *eventball);
};


class Matting: public EditViewObj{
 public:

  // input data 
  int w,h;
  std::string fname_src;
  std::vector<unsigned char> img_src;
  std::string fname_matte;
  std::vector<unsigned char> img_matte;
  float lights[4][3];

  ImageTile image_tile;

  Matting();
  ~Matting();

  virtual void AskForData(Serializer *s);
  virtual void TreeScan(TSOCntx *cntx);
  virtual void Draw(DrawCntx *cntx);

  enum {
    show_mode_off,
    show_mode_src,
    show_mode_matte,
    show_mode_src_matte,

    edit_mode_off,
    edit_mode_on,

    event_image_src_updated
  };

  std::set<int> eventset;

  int show_mode;
  int edit_mode;

  //GLListHandle shape;
  //void BuildShape();

  MattingEH  eh;

  void reopenSrc();  
  void loadTxtSrc();

  //GLuint glsel_lightcur;
  //GLuint glsel_light[4];
  //void DrawLightPoints();

  void Build();
};

#endif
