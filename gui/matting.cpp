#include "matting.h"
#include <FreeImage.h>
#include <GL/glext.h>
#include <sys/types.h>
#include <unistd.h>
#include "apputil/serializer.h"
#include "glhelper.h"
#include "dispview.h"
#include "viewctrl.h"
#include "shapefromshade.h"
#include "LoadImageToVec.h"


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
    reopenSrc();
   }
};


void Matting::reopenSrc(){
    LoadImageToVec(fname_src, img_src, w, h);
    if(w && h) eventset.insert(event_image_src_updated);
}
	
void Matting::loadTxtSrc(){

  if(img_src.empty()) return;
  if( img_src.size()!=w*h*4 ) {
    err_printf(("texture loading error: incorrect size"));
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
  image_tile.LoadBGRA(&img_src[0], w, h);
}


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
      if(eventset.count(event_image_src_updated)){
	loadTxtSrc();
        eventset.erase(event_image_src_updated);
      }
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
