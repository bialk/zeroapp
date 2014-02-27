#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>

#ifdef _WIN32
#include <io.h>
#define O_NOCTTY O_BINARY
#include <malloc.h>
#else
#include <unistd.h>
#endif

#include <math.h>
#include <algorithm>
#include <vector>
#include <set>

#include "eventlog.h"
#include "mathlib/mathutl/mymath.h"
#include "mathlib/mathutl/matchalg.h"
#include "mathlib/mathutl/imageandfft.h"

#include "dispview.h"
#include "glhelper.h"
#include "viewsurf.h"
#include "apputil/serializer.h"

//===========================  UI ROUTINES  =============================


void ViewSurf::AskForData(Serializer *s){
  if(s->ss->storageid==SRLZ_LAYOUT){
    s->Item("is_clip",Sync(&is_clip));
    s->Item("show",Sync(&show));
    s->Item("norm_smooth_on",Sync(&norm_smooth_on));
    s->Item("texture_on",Sync(&texture_on));
  }
  if(s->ss->storageid==SRLZ_PROJECT){
    s->Item("filemaptxtr",Sync(&filemaptxtr));
    s->Item("filetexture",Sync(&filetexture));
    s->Item("filesurfa",Sync(&filesurfa));
    s->Item("filesurfb",Sync(&filesurfb));
    s->Item("filesurfc",Sync(&filesurfc));
    s->Item("filesurfd",Sync(&filesurfd));
    s->Item("filesurfe",Sync(&filesurfe));
    s->Item("filesurff",Sync(&filesurff));
  }
}

void ViewSurf::TreeScan(TSOCntx *cntx){
  if(cntx==&TSOCntx::TSO_Init){
  }
}


/*

// EXAMLES OF CODE FOR WORKING WITH PIPES

static void tdm00(int i){
  FILE* f = popen("../tdmmodel00/tdmmodel","w");
  fprintf(f,
	  "data_src_name=../data/shan/Piroddi/head.3i2\n"
	  "data_surf_name=../data/out/Piroddi/head_tdm00.srf\n"
	  "showstat=0\n");
  pclose(f);
  SurfLoad()(&((ViewSurf*)(i))->surf,"../data/out/Piroddi/head_tdm00.srf");
  ((ViewSurf*)(i))->BuildGLList();
}

static void tdm01(int i){
  FILE* f = popen("../tdmmodel01/tdmmodel","w");
  fprintf(f,
	  "showstat=0\n"
	  "iter=1\n"
	  "data_src_name=../data/shan/Blinov/head.3i2\n"
	  "data_surfin_name=../data/out/Blinov/head_tdm00.srf\n"
	  "data_surfout_name=../data/out/Blinov/head_tdm01.srf\n");
  pclose(f);
  SurfLoad()(&((ViewSurf*)(i))->surf,"../data/out/Blinov/head_tdm01.srf");
  ((ViewSurf*)(i))->BuildGLList();
}
*/

void ViewSurf::SwitchSlotA(){
  if(SurfLoad()(&surf,filesurfa.c_str())) return;
  BuildGLList();
}
void ViewSurf::SwitchSlotB(){
  if(SurfLoad()(&surf,filesurfb.c_str())) return;
  BuildGLList();
}
void ViewSurf::SwitchSlotC(){
  if(SurfLoad()(&surf,filesurfc.c_str())) return;
  BuildGLList();
}
void ViewSurf::SwitchSlotD(){
  if(SurfLoad()(&surf,filesurfd.c_str())) return;
  BuildGLList();
}
void ViewSurf::SwitchSlotE(){
  if(SurfLoad()(&surf,filesurfd.c_str())) return;
  BuildGLList();
}
void ViewSurf::SwitchSlotF(){
  if(SurfLoad()(&surf,filesurff.c_str())) return;
  BuildGLList();
}




//=======================  START   PROCESSING V1  ========================

void ViewSurf::BuildGLList(){

  texture.Load(filetexture.c_str());
  mtxtr.Load(filemaptxtr.c_str());

  // draw surface
  if(!gllist) gllist = glGenLists(1);
  glNewList(gllist,GL_COMPILE_AND_EXECUTE);

  glPushMatrix();
  // scale the space in according real data set
  /*
  TBaseData *data = GL3DFIND(SrcVolume)->data_src_vol;
  if(data){
    glTranslatef(data->GetFirstX(),data->GetFirstT(),data->GetFirstY());
    glScalef(data->GetDistX(),data->GetDistT(),data->GetDistY());
  }
  else {
    glTranslatef(0,0,0);
    glScalef(1,1,1);
    }*/



  // define color coordinates
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
    if(texture_on){
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_emission); 
    }else{  
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_emissionhalf);      
      //glColorMaterial(GL_FRONT_AND_BACK,GL_DIFFUSE);
      //glEnable(GL_COLOR_MATERIAL);
      //glColor3f(0.5,0.5,0.5);
    }
  }


  Surf2GL s2gl;
  if(texture_on) 
    s2gl.SetTxtr(&surf,&texture,&mtxtr);
  else
    s2gl.SetTxtr(&surf,0,&mtxtr);
  if(norm_smooth_on) s2gl.SmoothNorms();
  s2gl();

  glPopMatrix();	
  glEndList();

  return;
}


//============================ END OF PROCESSING ============================


ViewSurf::~ViewSurf(){ }

ViewSurf::ViewSurf():
  show(0),is_clip(0),norm_smooth_on(0),
  texture_on(0),gllist(0),glroshow(0),
  viewsurfeh(this){}

void ViewSurf::DrawFrame(){}



void ViewSurf::Draw(DrawCntx* cntx){
  /*
  if(is_clip && srcvolume->data_src_vol) {  
    glPushMatrix();
    TBaseData *data = srcvolume->data_src_vol;    
    glTranslatef(data->GetFirstX(),data->GetFirstT(),data->GetFirstY());
    glScalef(data->GetDistX(),data->GetDistT(),data->GetDistY());
    double eqn[]={0,0,-1,srcvolume->sect_y+2};
    glClipPlane(GL_CLIP_PLANE0,eqn);
    glEnable(GL_CLIP_PLANE0);  
    glPopMatrix();    
  }
  */
  if (show) glCallList(gllist);
  //if(is_clip)
  //  glDisable(GL_CLIP_PLANE0);
}



// event handler

ViewSurfEH::ViewSurfEH(ViewSurf *v){
  vs= v;
}

void ViewSurfEH::Handle(void *event){}
