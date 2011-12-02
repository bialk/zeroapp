#include "shapefromshade.h"
#include "normfromshade.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <map>
//#include <unistd.h> // is definition of function sleep()
#include "lineqsol.h"


//======================================================================

class indx{
public:
  int i,j;
  indx(): i(0),j(0){}
  indx(int ii, int jj):i(ii),j(jj){}
  bool operator < (const indx &p) const{
    return i<p.i || (i==p.i && j<p.j);
  }
};

//sparce matrix & righthand vector  
class KMtrx: public std::map<indx,float>{
public:
  float& operator()(int i, int j){      
    iterator it =  insert(value_type(indx(i,j),0)).first;
    return it->second;
  }
};

class BMtrx: public std::map<int,float>{
public:
  float& operator()(int i){      
    iterator it =  insert(value_type(i,0)).first;
    return it->second;
  }
};



//======================================================================

int ShapeFromShade::build(){

  // calculating normals and albedo from shadow
  int ix,iy;
  {
    NormFromShade  nfs, nfs_alb;
    nfs.Init(&s[0][0],4);
    nfs_alb.Init(&s_alb[0][0],4);

    // scanning pixels and calculating albedo
    for(ix=0;ix<w;ix++){
      for(iy=0;iy<h;iy++){

	int iy2 = iy; //h-iy-1; // image may be vertically fliped
	// reading image pixels
	float *i1= &image[0][(w*iy2+ix)*3];
	float *i2= &image[1][(w*iy2+ix)*3];
	float *i3= &image[2][(w*iy2+ix)*3];
	float *i4= &image[3][(w*iy2+ix)*3];
	
	float *albd= &albedo[(w*iy2+ix)*3];

	float ilm[] = { i1[0],i1[1],i1[2], 
			i2[0],i2[1],i2[2], 
			i3[0],i3[1],i3[2],
			i4[0],i4[1],i4[2] };	  

	float norm[3];	

	// normal and albedo estimation
	{
	  nfs.Norm4(ilm,norm,albd);
	  float sum=sqrt(norm[0]*norm[0]+norm[1]*norm[1]+norm[2]*norm[2]);
	  if(sum==0) { sum=1;}

	  // writting normal into array
	  float *out = data_norm + (w*iy+ix)*3;
	  out[0]=norm[0]/sum;
	  out[1]=norm[1]/sum;
	  out[2]=norm[2]/sum;
	  data_attd[w*iy+ix]=0;
	}
      }
    }
  }
  


  // assembling matrix A
  KMtrx mtrk;
  BMtrx mtrb;

  class TCntrElem{    
  public:
    int szy;
    int w;
    float *d;
    float GetElem(int x, int y, int z){
      return d[(w*y+x)*3+z];
    }
    void add2mtrx(KMtrx &m,BMtrx &b, int x, int y){
      int i,j;
      int x1,x2,y1,y2;
      for(i=0;i<4;i++){
	x1 = i/2; y1 = i%2;	  
	int idx1=(x+x1)*szy+(y+y1); // '-1' the matrix has size (szy*szx - 1)
	float z  = GetElem(x+x1,y+y1,2); 
	if(z<0.1) z=0.1;
	float gx = -GetElem(x+x1,y+y1,0)/z;
	float gy = -GetElem(x+x1,y+y1,1)/z;
	float sum = ((0.5-x1)*gx+(0.5-y1)*gy)*3;	
	m(idx1,idx1) -= 3;
	for(j=0;j<4;j++){
	  if(i==j) continue;
	  x2 = j/2; y2 = j%2;	  
	  int idx2=(x+x2)*szy+(y+y2);
	  m(idx1,idx2)+= 1;
	  z  = GetElem(x+x2,y+y2,2);  if(z<0.1) z=0.1;
	  gx = -GetElem(x+x2,y+y2,0)/z;
	  gy = -GetElem(x+x2,y+y2,1)/z;	  
	  sum -= ((0.5-x2)*gx+(0.5-y2)*gy);
	}
	if(sum!=0.0) b(idx1) += sum; 
      }
    }
  } cntrelem; 
  cntrelem.w=w;
  cntrelem.d=data_norm; 
  cntrelem.szy=h;

  //scanning all internal nodes
  for(ix=0;ix<w-1;ix++){
    for(iy=0;iy<h-1;iy++){
      cntrelem.add2mtrx(mtrk,mtrb,ix,iy);	
    }
  }
  // add boundary condition
  int lasteqn = h*w;
  mtrk(0,0)+=2;
  //mtrb(lasteqn)=1;


  LinSolver lsvr;

  // fill up matrix A
  KMtrx::iterator it;
  int sizem = h*w;
  lsvr.MtrxA(sizem,sizem,mtrk.size());
  for(it=mtrk.begin();it!=mtrk.end();it++){    
    lsvr.A(it->first.i,it->first.j,it->second);
  }
  mtrk.clear();

  // fill up vector B
  BMtrx::iterator itb;
  lsvr.MtrxB();
  for(itb=mtrb.begin();itb!=mtrb.end();itb++){
    if(itb->second!=0)
      lsvr.B(itb->first,itb->second);    
  }
  mtrb.clear();

  // solving system of linear equations
  lsvr.solve();

  //moving result into into attitude data set
  for(ix=0;ix<w;ix++)
    for(iy=0;iy<h;iy++){
      float a;
      a = lsvr.X(h*ix+iy);
      data_attd[w*iy+ix]=a;
    }
}
