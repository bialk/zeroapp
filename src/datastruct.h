#ifndef datastruct_h
#define datastruct_h


template<class T>
class rgbimg: public std::vector<T>{
public:
  int szx,szy;
};

template<class T>
int StorePPM(T *rgb, int szx, int szy, const char *name="test.ppm"){
  if(!rgb) {err_printf(("StorePPM: input array is zero\n")); return 1;}
  FILE *h = fopen(name,"wb");
  if(!h) {err_printf(("StorePPM: can't open file (check the filename (\"%s\")\n",name)); return 1;}
  fprintf(h,"P6\n%i %i\n255\n",szx,szy);
  int i,j;
  for(j=0;j<szy;j++)
    for(i=0;i<szx;i++){
      int idx = (szx*j+i)*3;
      unsigned char v[3]={rgb[idx],rgb[idx+1],rgb[idx+2]};
      fwrite(v,1,3,h);
    }
  fclose(h);
  return 0;
}


template<class T>
int LoadPPM(rgbimg<T> &rgb,const char *name="test.ppm"){
  if(!&rgb) {err_printf(("LoadPPM: input array is zero\n")); return 1;}
  FILE *h = fopen(name,"rb");
  if(!h) {err_printf(("LoadPPM: can't open file (check the filename (\"%s\")\n",name)); return 1;}
  char s[255]; int k;
  fscanf(h,"%s\n%i %i\n %*s\n",s,&rgb.szx,&rgb.szy);
  rgb.resize(rgb.szx*rgb.szy*3);
  int i,j;
  for(j=0;j<rgb.szy;j++)
    for(i=0;i<rgb.szx;i++){
      int idx = (rgb.szx*j+i)*3;
      unsigned char v[3];
      fread(v,1,3,h);
      rgb[idx]=v[0]; rgb[idx+1]=v[1]; rgb[idx+2]=v[2];
    }
  fclose(h);
  return 0;
}


#endif //datastruct_h
