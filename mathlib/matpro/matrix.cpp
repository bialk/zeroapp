//---------------------------------------------------------------------------
// developed by Alex Kostin    March 11, 1999
//---------------------------------------------------------------------------
#include <math.h>
//#include <mem.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
//#include <vcl/sysutils.hpp>
#pragma hdrstop
#include "matrix.h"
long double minv(long double *a,int n);
#define Exception (char*)
TMatrix &TMatrix::operator*= (long double Koef)
{
   // ��������� ������� �� �����
   long double *Dest = mat;
   for (int i = NumElem; i > 0; i--) *(Dest++) *= Koef;
   return *this;
}
void TMatrix::Diagonal(long double DiagElem)
{
   // ��������� ��� ������������ ��������
   //  DiagElem'�� � ��� ��������� ������
   for (int i = 0; i < sizeV; i++)
   for (int j = 0; j < sizeH; j++)
   {
       WriteElem(i, j, (i != j) ? 0. : DiagElem);
   }
}
TMatrix BlockDiag(const TMatrix &mat1, const TMatrix &mat2)
{
   // ����������
   // ������� �'������ �������� , ��������� �� ������ mat1 � mat2
   int SizeV1 = mat1.GetSizeV();
   int SizeV2 = mat2.GetSizeV();
   int SizeH1 = mat1.GetSizeH();
   int SizeH2 = mat2.GetSizeH();

   int SizeResV = SizeV1 + SizeV2;
   int SizeResH = SizeH1 + SizeH2;

   TMatrix Result (SizeResV, SizeResH);
	 int i,j;
   for (i = 0; i < SizeV1; i++)
   for (j = 0; j < SizeH1; j++)                       // ����������
   {                                                      // |1 0|
      Result.WriteElem(i,j, mat1.ReadElem(i,j));          // |0 0|
   }
   int i2 = 0;
   int j2 = 0;
   for (i = SizeV1; i < SizeResV; i++, i2++)
   for (int j = SizeH1; j < SizeResH; j++, j2++)          // ����������
   {                                                      // |0 0|
      Result.WriteElem(i,j, mat2.ReadElem(i2,j2));        // |0 1|
   }
   for (i = 0;      i < SizeV1;   i++)
   for (int j = SizeH1; j < SizeResH; j++)                // ����������
   {                                                      // |0 1|
      Result.WriteElem(i,j, 0.f);                         // |0 0|
   }
   for (i = SizeV1; i < SizeResV; i++)
   for (int j = 0;      j < SizeH1;   j++)                // ����������
   {                                                      // |0 0|
      Result.WriteElem(i,j, 0.f);                         // |1 0|
   }
   return Result;
}
//---------------------------------------------------------------------------
void TMatrix::SetSize (int y, int x)
{
   sizeV = y;
   sizeH = x;
   NumElem = x*y;
   if (mat != NULL)
   {
      delete mat;
      mat = NULL;
   }
   mat = new long double[sizeV * sizeH];
   if (mat == NULL)
   {
      // ������ ���� ������������ ��������� ������
      throw Exception("��� ������ ��� �������� �������!");
   }
}
//---------------------------------------------------------------------------
TMatrix::TMatrix (const TMatrix &matR) // ����������� �����������
{
   mat = NULL;
   SetSize(matR.sizeV, matR.sizeH); //��������� ������
   if (!memcpy(mat, matR.mat, NumElem*sizeof(long double))) //����������� �������
       throw Exception ("������ ����������� �������");
}
//---------------------------------------------------------------------------
TMatrix::TMatrix (int _sizeV, int _sizeH)   // �����������
{
   mat = NULL;
   SetSize(_sizeV, _sizeH);
}
//---------------------------------------------------------------------------
TMatrix::~TMatrix() // ����������
{
  if (mat != NULL)
     delete[] mat;
}

//---------------------------------------------------------------------------
TMatrix &TMatrix::operator= (const TMatrix &matR)     // ��������� ������
{
  if (sizeV != matR.sizeV || sizeH != matR.sizeH)
  {
    throw Exception ("The matrices have different dimensions!");
//        SetSize(matR.sizeV, matR.sizeH);
  }
  //printf("memcpy pass\n");
  memcpy(mat, matR.mat, NumElem * sizeof(long double));
  return *this;
}
//---------------------------------------------------------------------------
TMatrix &TMatrix::operator=(long double *matR)     // ��������� ������
{
  memcpy(mat, matR, NumElem * sizeof(long double));
  return *this;
}

//---------------------------------------------------------------------------
TMatrix &TMatrix::operator=(double *matR)      // ��������� ������
{
  long double *Dest = mat;
  for (int i = NumElem; i > 0; i--)
    *(Dest++) = (long double)(*(matR++));
  return *this;
}

//---------------------------------------------------------------------------
TMatrix TMatrix::operator+(const TMatrix &matR) const
{
  if (sizeV != matR.sizeV || sizeH != matR.sizeH)
     throw Exception ("The matrices have different dimensions!");
  TMatrix Result(sizeV, sizeH);
  long double *Dest = Result.mat;
  long double *Op1 = mat;
  long double *Op2 = matR.mat;
  for (int i = NumElem; i > 0; i--)
    *(Dest++) = *(Op1++) + *(Op2++);
  return Result;
}
//---------------------------------------------------------------------------
TMatrix &TMatrix::operator += (const TMatrix &matR) // ��������� ������ �� ���������
{
  //if (NumElem != matR.NumElem)
  if (sizeV != matR.sizeV || sizeH != matR.sizeH)
     throw Exception ("The matrices have different dimensions!");
  long double *Dest = mat;
  long double *Src  = matR.mat;
  for (int i = NumElem; i > 0; i--)
    *(Dest++) += *(Src++);
  return *this;
}
//---------------------------------------------------------------------------
TMatrix &TMatrix::operator-=(const TMatrix &matR)
{                                       // ��������� ������ � ����������
  if (sizeV != matR.sizeV || sizeH != matR.sizeH)
     throw Exception ("The matrices have different dimensions!");
  long double *Dest = mat;
  long double *Src  = matR.mat;
  for (int i = NumElem; i > 0; i--)
    *(Dest++) -= *(Src++);
  return *this;
}
//---------------------------------------------------------------------------
TMatrix TMatrix::operator-(const TMatrix &matR) const
{
  if (sizeV != matR.sizeV || sizeH != matR.sizeH)
     throw Exception ("The matrices have different dimensions!");
  TMatrix Result(sizeV, sizeH);
  long double *Dest = Result.mat;
  long double *Op1 = matR.mat;
  long double *Op2 = mat;
  for (int i = NumElem; i > 0; i--)
    *(Dest++) = *(Op1++) + *(Op2++);
  return Result;
}
//---------------------------------------------------------------------------
TMatrix& TMatrix::Transpose()      // ���������������� �������
{
  long double *pj, *pi, hold;
  int nm = NumElem - 1;
  int k = 1;
  int n = 0;
  int i, j;
  do
  {
    n++;
    j = k;                          // ����� ���������
    i = (j * sizeH) % nm;           // ����� ���������
    pj = mat + j;                   // ����� ���������
    hold = *pj;                     // ���������� ���������
    do
    {
      pi = mat + i;           // ����� ���������
      *pj = *pi;              // ���������
      n++;                    // ����� ����������� ����.
      j = i;                  // �������� ���� ����������
      pj = pi;                // ---------- // ----------
      i =  (j * sizeH)  % nm; // ����� ������ ���������
    } while(i != k);
    *pj = hold;
    k++;
  } while(n < nm - 2);
  i = sizeH;
  sizeH = sizeV;
  sizeV = i;
  return *this;
}

//---------------------------------------------------------------------------
TMatrix TMatrix::operator!() const                    // ��������� �������
{
  if (sizeV != sizeH)
     throw Exception("������� �������� ������������ �������!");
  TMatrix Result(*this);
  minv (Result.mat, sizeV);
  return Result;
}
//---------------------------------------------------------------------------
long double TMatrix::Inverse()
{
  if (sizeV != sizeH)
     throw Exception("������� �������� ������������ �������!");
  return minv (mat, sizeV);
}
//---------------------------------------------------------------------------
void TMatrix::madd (TMatrix &op1)                 // �������� ������
{
  for(int i = 0; i < NumElem; i++)
    mat[i] += op1.mat[i];
}
//---------------------------------------------------------------------------
void TMatrix::msub (TMatrix &op1)                 // ��������� ������
{
  for(int i = 0; i < NumElem; i++)
    mat[i] -= op1.mat[i];
}
//---------------------------------------------------------------------------
void TMatrix::madd (TMatrix &op1, TMatrix &op2)    // �������� ������
{
  for (int i = 0; i < NumElem; i++)
    mat[i] = op1.mat[i] + op2.mat[i];
}
//---------------------------------------------------------------------------
void TMatrix::msub (TMatrix &op1, TMatrix &op2)    // ��������� ������
{
  for(int i = 0; i < NumElem; i++)
    mat[i] = op1.mat[i] - op2.mat[i];
}
//---------------------------------------------------------------------------
void TMatrix::mtra (const TMatrix &op1)  // ���������������� �������
{
  long double *ij;
  long double *a = op1.mat;
  long double *b = mat;
  int j;
  for(int i = 0; i < sizeH; i++)
  {
    ij = a;
    for(j = 0; j < sizeV; j++)  // ������� ������� � ������
    {
      *b++ = *ij;
      ij += sizeH;
    }
    a++;
  }
}
//---------------------------------------------------------------------------
// ��������� ������ a(n*m); b(m*l); r(n*l) = a(n*m) * b(m*l)
void TMatrix::mmul (const TMatrix &op1, const TMatrix &op2)
{
  long double *ib;
  long double *j2;
  long double *a = op1.mat;           // ������ ������ �
  long double *b = op2.mat;
  long double *r = mat;
  long double *j1 = a;
  long double *ik;
  int j;
  int k;
  int m = op1.sizeH;
  for(int i = 0; i < sizeV; i++)
  {
    ik = b;                     // ������ ������� �
    for(j = 0; j < sizeH; j++)  // �������� �� ������� �
    {
      ib = ik;                  // ����� ��. �
      j2 = j1;                  // ����� ��. �
      *r = 0;                   // ��������� �����
      for(k = 0; k < m; k++)    // ���������� �����
      {
        *r += *j2 * *ib;  // ����������� ��������
        j2++;             // ��������� ��. �
        ib += op2.sizeH;  // ��������� ��. B
      }
      r++;                      // ��������� ��. R
      ik++;                     // ��������� ������� B
    }
    j1+=op1.sizeH;              // ��������� ������ �
  }
};
//---------------------------------------------------------------------------
TMatrix TMatrix::operator~() const //���������������� �������
{                                    // ���������� ����������������� �������
                                     // �� ������� ������������ �������
   TMatrix Result (sizeH, sizeV);    //
   for (int i = 0; i < sizeV; i++)
     for (int j = 0; j < sizeH; j++)
     {
        Result.WriteElem (j, i, ReadElem(i,j));
     }
   return Result;
}
//---------------------------------------------------------------------------
TMatrix  TMatrix::operator* (const TMatrix &matR) const
{
   TMatrix Result(sizeV, matR.GetSizeH());
   Result.mmul(*this, matR);
   return Result;
}
//---------------------------------------------------------------------------
void TMatrix::fill(long double val)
{
  for(int i = 0; i < NumElem; mat[i++] = val);
}
//---------------------------------------------------------------------------
void TMatrix::addDiag(long double val)
{
  for(int x = 0; x < sizeH; x++)
  {
    mat[x * (sizeH + 1)] += val;
  }
}
//---------------------------------------------------------------------------
long double TMatrix::norm()
{
  long double retV =0;
  for(int y = 0; y < sizeV; y++)
  {
    long double t = mat[y];
    retV += t * t;
  }
  return retV;
}
//---------------------------------------------------------------------------
void TMatrix::vecMul(TMatrix &op1)
{
  long double *op1_m = op1.mat;
  for(int x = 0; x < sizeH; x++)
    for(int y = 0; y < sizeV; y++)
    {
      mat[y * sizeH + x] = op1_m[y] * op1_m[x];
    }
}
//---------------------------------------------------------------------------
long double minv(long double *a, int n)
{
int i,j,k,l[30],m[30],ij,ik,iz,ji,jk,jp,jr,jq,nk,ki,kj,kk;
long double biga,hold;
long double d;

/* 1 */

d=1.0;
nk=-n;
for(k=1;k<=n;k++)
{
  nk=nk+n;
  l[k-1]=k;
  m[k-1]=k;
  kk=nk+k;
  biga=a[kk-1];
  for(j=k;j<=n;j++)
  {
    iz=n*(j-1);
    for(i=k;i<=n;i++)
    {
      ij=iz+i;
      if(fabs(biga)-fabs(a[ij-1])<0)
      {
        biga=a[ij-1];
        l[k-1]=i;
        m[k-1]=j;
      }
    }
  }

  /* 2 */

  j=l[k-1];
  if(j-k>0)
  {
    ki=k-n;
    for(i=1;i<=n;i++)
    {
      ki+=n;
      hold=-a[ki-1];
      ji=ki-k+j;
      a[ki-1]=a[ji-1];
      a[ji-1]=hold;
    }
  }

  /* 3 */

  i=m[k-1];
  if(i-k>0)
  {
    jp=n*(i-1);
    for(j=1;j<=n;j++)
    {
      jk=nk+j;
      ji=jp+j;
      hold=-a[jk-1];
      a[jk-1]=a[ji-1];
      a[ji-1]=hold;
    }
  }

  /* 4 */

  if(biga==0)
  {
    d=0.0;
    return d;
//    throw "Discriminant equals to zero!!";
  }
  for(i=1;i<=n;i++)
  {
    if(i-k!=0)
    {
      ik=nk+i;
      a[ik-1]=a[ik-1]/(-biga);
    }
  }

  /* 5 */

  for(i=1;i<=n;i++)
  {
    ik=nk+i;
    hold=a[ik-1];
    ij=i-n;
    for(j=1;j<=n;j++)
    {
      ij+=n;
      if(i-k!=0)
      {
        if(j-k!=0)
        {
          kj=ij-i+k;
          a[ij-1]=hold*a[kj-1]+a[ij-1];
        }
      }
    }
  }

  /* 6 */

  kj=k-n;
  for(j=1;j<=n;j++)
  {
    kj+=n;
    if(j-k!=0) a[kj-1]/=biga;
  }

  /* 7 */

  d*=biga;

  /* 8 */
  if (biga == 0)
  {
    return 0;
//    throw Exception("Error!");
  }
  a[kk-1]=1.0/biga;
}

/* 9 */

  k=n;
  met1: k--;
  if(k<=0)
  {
    return(d);
  }
  i=l[k-1];
  if(i-k>0)
  {
    jq=n*(k-1);
    jr=n*(i-1);
    for(j=1;j<=n;j++)
    {
      jk=jq+j;
      hold=a[jk-1];
      ji=jr+j;
      a[jk-1]=-a[ji-1];
      a[ji-1]=hold;
    }
  }
  j=m[k-1];
  if(j-k>0)
  {
    ki=k-n;
    for(i=1;i<=n;i++)
    {
      ki+=n;
      hold=a[ki-1];
      ji=ki-k+j;
      a[ki-1]=-a[ji-1];
      a[ji-1]=hold;
    }
  }
  goto met1;
}
//---------------------------------------------------------------------------

