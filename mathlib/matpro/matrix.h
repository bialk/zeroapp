//---------------------------------------------------------------------------
// developed by Alex Kostin    March 11, 1999
//---------------------------------------------------------------------------
#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <stddef.h>

class TMatrix;

TMatrix BlockDiag(const TMatrix &mat1, const TMatrix &mat2); // ����������
// ������� �'������ �������� , ��������� �� ������������ ������ mat1 � mat2

class TMatrix
{
public:
  long double *mat;
  int sizeV, sizeH;
  int NumElem;

  TMatrix (const TMatrix &matR);            // copy constuctor
  TMatrix (int sizeV, int sizeH);
  TMatrix()
  {
    mat = NULL;
    NumElem = sizeV = sizeH = 0;
  }

  ~TMatrix();

  void SetSize (int y, int x);
  TMatrix &operator= (const TMatrix &matR);  // ��������� ������
  TMatrix &operator= (double *matR);         // ��������� ������
  TMatrix &operator= (long double *matR);          // ��������� ������
  TMatrix  operator+ (const TMatrix &matR) const; //�������� ������
  TMatrix  operator- (const TMatrix &matR) const; //��������� ������
  TMatrix &operator+= (const TMatrix &matR);  // ��������� ������ �� ���������
  TMatrix &operator*= (long double Koef);           // ��������� ������� �� �����
  TMatrix &operator-= (const TMatrix &matR);  // ��������� ������ � ����������
  TMatrix &Transpose();                       // ���������������� ����� �������
  TMatrix  operator~ () const;                // ���������������� �������

  TMatrix  operator! () const;                // ��������� �������
  TMatrix  operator* (const TMatrix &matR) const;  // ������������ ������
  long double   &operator[] (int x){return mat[x];}

  long double ReadElem(int y, int x) const
  {
    return mat[y * sizeH + x];
  }
  void WriteElem(int y, int x, long double value)
  {
    mat[y * sizeH + x] = value;
  }

  void Diagonal(long double DiagElem);    // ��������� ��� ������������ ��������
                                    //  DiagElem'�� � ��� ��������� ������
  void madd (TMatrix &op1);
  void msub (TMatrix &op1);
  void madd (TMatrix &op1, TMatrix &op2);
  void msub (TMatrix &op1, TMatrix &op2);
  void mmul (const TMatrix &op1, const TMatrix &op2);
  void mtra (const TMatrix &op1);
  long double Inverse();
  int GetSizeV() const {return sizeV;}
  int GetSizeH() const {return sizeH;}
  void fill(long double val);
  void addDiag(long double val);
  long double norm();
  void vecMul(TMatrix &op1);
};

#endif
