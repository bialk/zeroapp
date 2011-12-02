bin_PROGRAMS = zeroapp

zeroapp_SOURCES = mainini.cpp \
../apputil/AppSysSet.cpp ../apputil/AppSysSet.h \
../apputil/serializer.cpp ../apputil/serializer.h \
eventlog.cpp eventlog.h \
toolspanel.cpp \
mathstruct.cpp \
mousectrl.cpp mousectrl.h \
savejpeg.cpp \
glhelper.cpp savejpeg.h LoadImageToVec.h \
editviewobj.cpp editviewobj.h \
glhelper.h  mathstruct.h  toolspanel.h \
dispview.cpp dispview.h \
eventhnd.cpp eventhnd.h\
viewctrl.cpp viewctrl.h \
lights.cpp lights.h\
viewsurf.cpp viewsurf.h \
imageplane.cpp imageplane.h \
matting.cpp matting.h \
shapefromshade.cpp shapefromshade.h normsfromshade.h \
lineqsol.cpp \
ImageTile.cpp ImageTile.h \
../apputil/eventballfltk.cpp ../apputil/eventballfltk.h \
fltk_calllist.cpp fltk_calllist.h fltk_animator.h fltk_defs.h \
frmMainDisplayUI.cxx frmMainDisplayUI.h \
frmMainDisplay.cpp frmMainDisplay.h \
frmSurfCtrlUI.cxx frmSurfCtrlUI.h \
frmSurfCtrl.cpp frmSurfCtrl.h \
frmPrjTreeUI.h frmPrjTreeUI.cxx \
frmPrjTree.h frmPrjTree.cpp \
frmViewCtrlUI.cxx frmViewCtrlUI.h \
frmViewCtrl.cpp frmViewCtrl.h \
frmImagePlaneUI.cxx frmImagePlaneUI.h \
frmImagePlane.cpp frmImagePlane.h \
frmMattingUI.cxx frmMattingUI.h \
frmMatting.cpp frmMatting.h

zeroappdir = $(datadir)/$(PACKAGE_NAME)
zeroapp_DATA = torch.ico index.html 
PACKAGE_SHAREDIR = $(zeroappdir)

if MINGW32

zeroapp_CPPFLAGS = -w -I$(top_srcdir) -I$(HOME)/include -I$(HOME)/lib/sigc++-2.0/include -DPACKAGE_SHAREDIR=\"$(zeroappdir)\"
zeroapp_LDFLAGS =  -mwindows  -mno-cygwin -L/usr/X11R6/lib -L$(HOME)/lib 
# -mconsole this flag needs for console
# the sequense "-llapack -lg2c -lblas"  was added to solve linker problem
# the sequense "-lfltk_jpeg -lfltk_png -lz"  was added to support Fl_Help_Dialog 
# fltk_jpeg must be replaced with jpeg to avoid conflicts
zeroapp_LDADD =  $(LIBS) $(SIGC_LIBS) -lfltk_png -lz -llapack -lg2c -lblas -lglu32 -lopengl32  -lole32 -luuid -lcomctl32 -lwsock32 
#-lpthreadGCE2 

else

zeroapp_CPPFLAGS = -w  -I$(top_srcdir) -I$(HOME)/include -I$(HOME)/lib/sigc++-2.0/include -DPACKAGE_SHAREDIR=\"$(zeroapp)\"
zeroapp_LDFLAGS = -L/usr/X11R6/lib64 -L/usr/X11R6/lib -L$(HOME)/lib 
zeroapp_LDADD = $(LIBS) $(SIGC_LIBS)

endif

