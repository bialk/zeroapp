#ifndef dispview_h
#define dispview_h

#include <list>
#include <Fl/gl.h>
#include "editviewobj.h"
#include "eventhnd.h"
#include <sigc++/sigc++.h>
#include <memory>

class ToolPanel;
class EditViewObj;
class ViewCtrl;
class Lights;
class Serializer;
class ViewSurf;
class ImagePlane;
class Matting;

class GLScene: public std::list<EditViewObj*>{
 public:
  void Add(EditViewObj* globj);
  void Remove(EditViewObj* globj);
};

class DispView;
class DispViewEH: public EvtHandle{
 public:
  DispView *dv;
  DispViewEH(DispView* v);
  virtual void Handle(EventBall *eventball);
};


class DispView{
 public:
  // signaling interface
  sigc::signal<void> redraw;
  sigc::signal<void> quit;
  sigc::signal<void, const char*> dbgmsg;


  DispView();
  ~DispView();

  //main window event handler list

  // common functionality
  int noredisplay;
  int redisplay;

  int drawsimple;

  GLScene scene;

  void Draw(DrawCntx *cntx);
  void TreeScan(TSOCntx *cntx);

  // main application entities
  std::auto_ptr<ToolPanel> toolpanel;
  std::auto_ptr<ViewCtrl>  viewctrl;
  std::auto_ptr<Lights>    lights;
  std::auto_ptr<ViewSurf>  viewsurf;  
  std::auto_ptr<ImagePlane>  imageplane;  
  std::auto_ptr<Matting>  matting;  

  // object selection mechanics
  GLuint glnamecount;
  GLuint GetNewName();
  sigc::signal<void> reselect;
  int selectid;
  void SelectRst();
  int SelectObj(int x, int y);

  void AskForData(Serializer *s);

  // snapshot of the screen  
  long nsnap;
  void snapshot();

  DispViewEH   dispvieweh;

  //EvthStack     evq_view;
  //EvthStack     evq_scene;
};

#endif


