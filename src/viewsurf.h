#ifndef ViewSurf_h
#define ViewSurf_h

#include "editviewobj.h"
#include "mathlib/mathutl/mymath.h"
#include "mathlib/mathutl/meshandalg.h"
#include "glhelper.h"
#include "eventhnd.h"

//class SrcVolume;
class ViewSurf;
class ViewSurfEH: public EvtHandle{
 public:
  ViewSurf *vs;
  ViewSurfEH(ViewSurf *v);
  virtual void Handle(void *event);
};

class ViewSurf: public EditViewObj{
 public:
  ViewSurf();
  ~ViewSurf();
  virtual void AskForData(Serializer *s);
  virtual void TreeScan(TSOCntx *cntx);
  virtual void Draw(DrawCntx *cntx);

  void Init(DispView *o);

  //SrcVolume *srcvolume;
  Surf surf;

  Texture texture;
  MapTxtrCoord  mtxtr;

  // experimental part for draft calculation and testing
  float *data_exp;

  const char* data_src_name;
  double scale_sig;
  long szX, szY, szZ;
  long sect_x, sect_y, sect_z;
  int show, is_clip, norm_smooth_on, texture_on;

  std::string filesurfa, filesurfb, filesurfc,
    filesurfd, filesurfe, filesurff,
    filemaptxtr,filetexture;

  int gllist;

  void BuildGLList();
  void DrawFrame();

  int glroshow;

  void SwitchSlotA();
  void SwitchSlotB();
  void SwitchSlotC();
  void SwitchSlotD();
  void SwitchSlotE();
  void SwitchSlotF();

  ViewSurfEH  viewsurfeh;

};

#endif
