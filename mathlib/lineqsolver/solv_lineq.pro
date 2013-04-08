SOURCES        += solv_lineq.cpp
unix {
#  UI_DIR = .ui
#  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}
TEMPLATE       = app
CONFIG         -= qt
CONFIG         += warn_off debug
LANGUAGE       = C++
INCLUDEPATH   += ../ 

#INCLUDEPATH   += /opt/PDmesa/Mesa-4.0.1/include 
#INCLUDEPATH   += /opt/PDmesa/GLUT-3.7/include 
#INCLUDEPATH   += /opt/X11R6/include 
#INCLUDEPATH   += /vol/vssp/raidbuf/ees5ab/download/fftw-3.0.1/api
#LIBS          += -L../Data_Access
#LIBS          += -L../matpro
#LIBS          += -L../jpeg-6b
#LIBS          += -L/opt/PDmesa/Mesa-4.0.1/lib
#LIBS          += -L/opt/PDmesa/GLUT-3.7/lib
#LIBS          += -L/usr/X11R6/lib
#LIBS          += -L/opt/X11R6/lib
#LIBS          += -L/vol/vssp/raidbuf/ees5ab/download/fftw-3.0.1/.libs
LIBS          += -L../../lib
#LIBS          += -ldata3d 
LIBS          += -lm -lspooles_I32
# -lglut  -lGLU -lGL 
# -ljpeg -lmatpro 
#LIBS          += -lfftw3

