
ENABLE_LANGUAGE( CXX )

INCLUDE (FindPkgConfig)
INCLUDE (FindFLTK)

# searching for fltk_jpeg and FLU
FIND_LIBRARY(FLTKJPEG_LIB fltk_jpeg)
FIND_LIBRARY(FLU_LIB flu flu)
FIND_LIBRARY(FLUGL_LIB flu_gl)

set (CMAKE_LIBRARY_PATH $ENV{HOME}/lib )

FIND_LIBRARY(FREEIMAGE_LIB freeimage)
FIND_PATH(FREEIMAGE_INCLUDE_DIR FreeImage.h)

FIND_LIBRARY(SPOOLES_LIB spooles_I32)
FIND_PATH(SPOOLES_INCLUDE_DIR spooles/InpMtx.h)

FIND_LIBRARY(LAPACK_LIB lapack)

#FIND_LIBRARY(SIGC_LIB sigc-2.0)
#FIND_PATH(SIGC_INCLUDE_DIR sigc++/sigc++.h)
#FIND_PATH(SIGC_CONFIG_DIR sigc++config.h $ENV{HOME}/lib/sigc++-2.0/include)

PKG_SEARCH_MODULE(sigc sigc++-2.0 REQUIRED)


ADD_LIBRARY( lapackcpp
    ../mathlib/lapackcpp/eigenval.cpp
    ../mathlib/lapackcpp/invert.cpp
    ../mathlib/lapackcpp/leastsquare.cpp
    ../mathlib/lapackcpp/svald.cpp
    ../mathlib/lapackcpp/lapackcpp.h
)

ADD_LIBRARY( apputil
    ../apputil/AppSysSet.cpp ../apputil/AppSysSet.h
    ../apputil/serializer.cpp ../apputil/serializer.h
    ../apputil/eventball.h
    ../apputil/eventballfltk.cpp ../apputil/eventballfltk.h
)

fltk_wrap_ui(
    zeroapp
    frmImagePlaneUI.fl
    frmMattingUI.fl
    frmSurfCtrlUI.fl
    frmMainDisplayUI.fl
    frmPrjTreeUI.fl
    frmViewCtrlUI.fl
)

add_custom_command (
  OUTPUT torch.ico
  DEPENDS torch.ico
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/torch.ico ${CMAKE_CURRENT_BINARY_DIR}/torch.ico
  )

ADD_EXECUTABLE( 
	zeroapp
	mainini.cpp
	eventlog.cpp eventlog.h
	toolspanel.cpp
	mathstruct.cpp mathstruct.h
	mousectrl.cpp mousectrl.h
	savejpeg.cpp
	glhelper.cpp savejpeg.h LoadImageToVec.h
	editviewobj.cpp editviewobj.h
	glhelper.h  mathstruct.h  toolspanel.h
	dispview.cpp dispview.h
	eventhnd.cpp eventhnd.h
	viewctrl.cpp viewctrl.h
	lights.cpp lights.h
	viewsurf.cpp viewsurf.h
	imageplane.cpp imageplane.h
	matting.cpp matting.h
	shapefromshade.cpp shapefromshade.h normfromshade.h
	lineqsol.cpp lineqsol.h
	ImageTile.cpp ImageTile.h
	fltk_calllist.cpp fltk_calllist.h fltk_animator.h fltk_defs.h
        frmMainDisplay.cpp frmMainDisplay.h
        frmSurfCtrl.cpp frmSurfCtrl.h
	frmPrjTree.h frmPrjTree.cpp
	frmViewCtrl.cpp frmViewCtrl.h
	frmImagePlane.cpp frmImagePlane.h
	frmMatting.cpp frmMatting.h
        ${zeroapp_FLTK_UI_SRCS}
        ${CMAKE_CURRENT_BINARY_DIR}/torch.ico
)

#MESSAGE("VAR=${FLTK_INCLUDE_DIR}")
#MESSAGE("VAR=${sigc_INCLUDE_DIRS}")

INCLUDE_DIRECTORIES(
                    ${zeroapp_SOURCE_DIR}
                    ${sigc_INCLUDE_DIRS}
                    ${FLTK_INCLUDE_DIR}
                    ${FREEIMAGE_INCLUDE_DIR}
                    ${SPOOLES_INCLUDE_DIR}
)

ADD_DEFINITIONS(
    -DPACKAGE_SHAREDIR="${zeroapp_SHAREDIR}"
    -DPACKAGE_NAME="${PROJECT_NAME}"
    -DPACKAGE_VERSION="${CMAKE_VERSION}"
    ${sigc_DEFINITIONS}
)

TARGET_LINK_LIBRARIES(zeroapp 
  ${FREEIMAGE_LIB}
  ${SPOOLES_LIB}
  ${FLU_LIB} 
  ${FLTK_LIBRARIES}
  ${FLTKJPEG_LIB}
  lapackcpp
  ${LAPACK_LIB}
  apputil
  ${sigc_LIBRARIES}
)

ADD_DEPENDENCIES(zeroapp lapackcpp apputil)

