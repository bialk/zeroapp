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
#include <FreeImage.h>
#include <sys/stat.h>



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

template<typename T>
void LoadImage(std::string &fname,  std::vector<T> &img, int &w, int &h){
  
  img.clear();

  struct stat st;
  if(stat(fname.c_str(),&st) ) {
    err_printf(("image file \"%s\"  not found",fname.c_str()));
    return;
  }

  //Format Detecting
  //Automatocally detects the format(from over 20 formats!)
  FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(fname.c_str(),0);
  
  if(formato == FIF_UNKNOWN) {
    err_printf(("image format unknown"));
    return;
  }

  //Image Loading
  FIBITMAP* imagen = FreeImage_Load(formato, fname.c_str());
  FIBITMAP* temp = imagen;
  if(!imagen){
    err_printf(("image reading error"));
    return;
  }
  imagen = FreeImage_ConvertTo32Bits(imagen);
  if(!temp) {
    err_printf(("image reading error"));
    return;
  }
  FreeImage_Unload(temp);
	
  w = FreeImage_GetWidth(imagen);
  h = FreeImage_GetHeight(imagen);

  if(w*h>2048*2048*20) {
    err_printf(("image reading error - too big image size (%ix%i)",w,h));
    return;
  }
  if(0) { // debug code
    printf("load image (%ix%i)\n", w, h);
  }  
	

  img.resize(4*w*h);
  GLubyte* img_ptr = &img[0];
  char* pixeles = (char*)FreeImage_GetBits(imagen);
  //FreeImage loads in BGR format, so you need to swap some bytes(Or use GL_BGR).
  /*	
  for(int j= 0; j<w*h; j++){
    textura[j*4+0]= pixeles[j*4+2];
    textura[j*4+1]= pixeles[j*4+1];
    textura[j*4+2]= pixeles[j*4+0];
    textura[j*4+3]= pixeles[j*4+3];
  }
  */
  memcpy(img_ptr,pixeles,4*w*h);
  FreeImage_Unload(imagen);
}


class Matting: public EditViewObj{
 public:

  // input data 
  int w,h;
  std::string fname_src;
  std::vector<unsigned char> img_src;
  std::string fname_matte;
  std::vector<unsigned char> img_matte;
  float lights[4][3];

  //GLTex2D image_tex;
  ImageTile image_tile;
  //GLTex2D albedo_tex; // it is not implemented yet!

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
    edit_mode_on
  };

  int show_mode;
  int edit_mode;

  //GLListHandle shape;
  //void BuildShape();

  MattingEH  eh;

  int  cache_slot;
  void OpenSrc();  
  void LoadTxtSrc();

  //GLuint glsel_lightcur;
  //GLuint glsel_light[4];
  //void DrawLightPoints();

  void Build();
};

#endif
