#include "matting.h"
#include <FreeImage.h>
#include <GL/glext.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "apputil/serializer.h"
#include "glhelper.h"
#include "dispview.h"
#include "viewctrl.h"
#include "shapefromshade.h"



//===============================================================

StrokePatch::StrokePatch():
  last_pset(0)
  ,pset(200)
{
  //last_pset(0);
  //pset.resize(200);
}

void StrokePatch::strokeStart(int x, int y){
  pset[2][3].x = x;
  pset[2][3].y = y;
}
void StrokePatch::strokeContinue(int x, int y){}
 
void StrokePatch::Draw(DrawCntx *cntx){}
void StrokePatch::TreeScan(TSOCntx *cntx){}



//===============================================================
Matting::Matting():
  eh(this)
  ,w(0),h(0)
  ,show_mode(show_mode_src)
  ,edit_mode(edit_mode_off)
  ,cache_slot(-1)
{
}

Matting::~Matting(){}


void Matting::AskForData(Serializer *s){
  if(s->ss->storageid==SRLZ_LAYOUT){
  }
  if(s->ss->storageid==SRLZ_PROJECT){
    s->Item("fname_src",Sync(&fname_src));
    s->Item("fname_matte",Sync(&fname_matte));
  } 
}

void Matting::TreeScan(TSOCntx *cntx){ 
  if(cntx==&TSOCntx::TSO_Init){
  }
  else if(cntx==&TSOCntx::TSO_ProjectNew){
    img_src.clear();
    img_matte.clear();
    image_tile.clear();
    w=h=0;
  }
  else if(cntx==&TSOCntx::TSO_ProjectLoad){
    Open(0);
    //Open(1);
    //Open(2);
    //Open(3);
   }
};


void Matting::Open(int slot){
#if 0
  if(cache_slot==slot)  cache_slot = -1;
  
  img[slot].clear();

  struct stat st;
  if(stat(imagefname[slot].c_str(),&st) ) {
    err_printf(("image file \"%s\"  not found",imagefname[slot].c_str()));
    return;
  }

  //Format Detecting
  //Automatocally detects the format(from over 20 formats!)
  FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(imagefname[slot].c_str(),0);
  
  if(formato == FIF_UNKNOWN) {
    err_printf(("image format unknown"));
    return;
  }

  //Image Loading
  FIBITMAP* imagen = FreeImage_Load(formato, imagefname[slot].c_str());
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
    printf("load image (%ix%i) for slot %i\n", w, h, slot);
  }  
	

  img[slot].resize(4*w*h);
  GLubyte* img_ptr = &img[slot][0];
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
#endif
}
	
void Matting::LoadTxt(int slot){
#if 0
  if(img[slot].empty()) return;
  if( img[slot].size()!=w*h*4 ) {
    err_printf(("texture loading error"));
    return;    
  }

  /*
  glBindTexture(GL_TEXTURE_RECTANGLE_ARB, image_tex());
     
  // set texture parameters 
  glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, 
		  GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
  glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, 
		  GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
  glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, 
		  GL_TEXTURE_WRAP_S, GL_CLAMP); 
  glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, 
		  GL_TEXTURE_WRAP_T, GL_CLAMP); 
  // load rectangle texture
  glTexImage2D(GL_TEXTURE_RECTANGLE_ARB,0,GL_RGBA, w, h, 0, GL_BGRA,GL_UNSIGNED_BYTE,(GLvoid*)&img[slot][0] );
  */
  image_tile.LoadBGRA(&img[slot][0], w, h);
#endif
}

#if 0
class TaleTxt{
public:
  void Set(unsigned char* image);
  void RenderTriangle(float *vtx, float *txt);  
  void RenderQuadriplet(float *vtx, float *txt);  
};
#endif

void Matting::Draw(DrawCntx *cntx){

  if(w==0 || h==0) return;

  // setup reflective surfaces
  { 
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 0.0 };
    
    GLfloat mat_emission[] = {1., 1., 1., 1.0}; 
    GLfloat mat_emissionhalf[] = {0.5, 0.5, 0.5, 1.0}; 

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, no_mat); 
    glMaterialfv(GL_BACK,  GL_DIFFUSE, no_mat); 
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, no_mat );
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, no_mat); 
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,  no_mat); 
    glDisable(GL_COLOR_MATERIAL);
    if(0/*texture_on*/){
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_emission); 
    }else{  
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_emissionhalf);      
      //glColorMaterial(GL_FRONT_AND_BACK,GL_DIFFUSE);
      //glEnable(GL_COLOR_MATERIAL);
      //glColor3f(0.5,0.5,0.5);
    }
  }


  // draw image
  if(show_mode!=show_mode_off){

    if(show_mode == show_mode_src){
      //LoadTxt(curslot);
    }

    glDisable(GL_LIGHTING);
    /*
    glEnable(GL_TEXTURE_RECTANGLE_ARB);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_RECTANGLE_ARB, image_tex());


    glColor4f(1,1,1,1);
    glBegin (GL_QUADS);  
    glTexCoord2f (0.0, 0.0);
    glVertex3f (0.0, 0.0, 0.0);
    glTexCoord2f (w, 0.0);
    glVertex3f (w, 0.0, 0.0);
    glTexCoord2f (w, h);
    glVertex3f (w, h, 0.0);
    glTexCoord2f (0.0, h);
    glVertex3f (0.0, h, 0.0);
    glEnd ();
    glDisable(GL_TEXTURE_RECTANGLE_ARB);
    */
    image_tile.Draw();
    glEnable(GL_LIGHTING);
  }

};
  

void Matting::Build(){
}



// event handler
//=================================================
MattingEH::MattingEH(Matting *v):
state_drag(-1),
ev(v)
{}

void MattingEH::Handle(EventBall *eventball){
  if(eventball->state(-34)){
  }
  else if(eventball->event(Q_ADD)){
    // event handler removed from the queue
    ev->edit_mode=Matting::edit_mode_on;
    eventball->stop();
    ev->dv->redraw();
    return;
  }
  else if(eventball->event(Q_DEL)){
    // event handler removed from the queue
    ev->edit_mode=Matting::edit_mode_off;
    eventball->stop();
    ev->dv->redraw();
    return;
  }
  else if(eventball->event(M_DRAG) && 
          eventball->state(state_drag)){
    // continue drag
#if 0
    float x=eventball->x, y=eventball->y, z=0;
    ev->dv->viewctrl->hitscene(x,y,z);
    float h=ev->h/2;
    float w=ev->w/2;
    x-=w; y-=h;    
    float d = sqrt(x*x+y*y);
    if(d<h && fabs(z)<0.2f){
      z = sqrt(h*h - d*d);
    }

    int slot;
    if(ev->glsel_lightcur == ev->glsel_light[0]) slot = 0;
    if(ev->glsel_lightcur == ev->glsel_light[1]) slot = 1;
    if(ev->glsel_lightcur == ev->glsel_light[2]) slot = 2;
    if(ev->glsel_lightcur == ev->glsel_light[3]) slot = 3;
    
    x = ev->lights[slot][0]=x/h;
    y = ev->lights[slot][1]=y/h;
    z = ev->lights[slot][2]=z/h;
    //printf("x=%f y=%f z=%f\n",x,y,z);
    SyncUI();
    ev->dv->redraw();
    eventball->stop();
#endif
  }
  else if(eventball->event(M_UP) &&
          eventball->state(state_drag)){
    // end drag
#if 0
    eventball->setstate(0);
    ev->glsel_lightcur=0;
    ev->dv->redraw();
    eventball->stop();
#endif
  }
  else if(!eventball->state(0)){
    // move event futher down
    return;
  }
  else if(eventball->event(M_DOWN) && 
          eventball->keys("") && 
	  eventball->kmod(0) &&
          eventball->mbtn(1)){

    // start drag
#if 0
    int id =ev->dv->SelectObj(eventball->x,eventball->y);
    if(id == ev->glsel_light[0] ||
       id == ev->glsel_light[1] ||
       id == ev->glsel_light[2] ||
       id == ev->glsel_light[3]){
      if(ev->glsel_lightcur != id){
	eventball->genstate(state_drag);
	ev->glsel_lightcur=id;
	ev->curslot=id-ev->glsel_light[0];	
	ev->dv->redraw();
	SyncUI();
	eventball->stop();
      }
    }
#endif 
  }
}
