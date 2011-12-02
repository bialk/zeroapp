#ifndef LoadImageToVec_h

#include <FreeImage.h>
#include <sys/stat.h>


template<typename T>
void LoadImageToVec(std::string &fname,  std::vector<T> &img, int &w, int &h){
  
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

#endif
