//============================================================================
// Name        : Matrix.h
// Author      : mm
// Copyright   : This tool and Javascript code are licensed under the MIT license.
// Description :
//============================================================================
#ifndef MATRIX_H_
#define MATRIX_H_

#include "CommonInclude.h"

template <class T>
class Matrix;

template <class T>
class Matrix {
public:
	Matrix();
	Matrix(long x, long y);
	virtual ~Matrix();

private:
	long sizeX;
	long sizeY;
	T* matrix;
	bool initialize;

public:
	void init(long x,
			  long y);
	void attach(T* data,
				long x,
				long y);
	bool copy(Matrix<T>* sourceMatrix,
					long fromX,
					long fromY,
					long fromSizeX,
					long fromSizeY,
					long toX,
					long toY);
	Matrix<T>* minimize(long factor,
						long grid);
	T getMatrixValue(long x,
					 long y);
	T* getMatrixAddress(long x,
						long y);
	void setMatrixValue(long x,
						long y,
						const T value);
private:
	void clear();
};

template<class T> inline Matrix<T>::Matrix()
	: sizeX(0),
	  sizeY(0),
	  matrix(0),
	  initialize(false)
{
}

template<class T> inline Matrix<T>::Matrix(long  x, long  y)
{
	init(x, y);
}

template<class T> inline Matrix<T>::~Matrix()
{
	clear();
}

template<class T> inline void Matrix<T>::init(long  x, long  y)
{
	sizeX = x;
	sizeY = y;
	try{
		matrix = new T[x*y];
		memset(matrix, 0, sizeof(T)*x*y);
		initialize = true;
	}
	catch(...){
		matrix = 0;
		initialize = false;
	}
}

template<class T> inline void Matrix<T>::attach(T *data, long  x, long  y)
{
	clear();
	sizeX = x;
	sizeY = y;
	matrix = data;
	initialize = false;
}

template<class T> inline bool Matrix<T>::copy(Matrix<T>* sourceMatrix, long  fromX, long  fromY, long  fromSizeX, long  fromSizeY, long  toX, long  toY)
{
	if(!matrix || !sourceMatrix || toX + fromSizeX > sizeX || toY + fromSizeY > sizeY){
		return false;
	}

	for(long y=fromY; y<fromY+fromSizeY; y++){
		memcpy(&matrix[(toY + y - fromY) * sizeX + toX], sourceMatrix->getMatrixAddress(fromX, y), sizeof(T)*fromSizeX);
	}

	return true;
}

template<class T> inline  Matrix<T>* Matrix<T>::minimize(long  factor, long  grid)
{
	if(!matrix || sizeX == 0 || sizeY == 0 || factor <= 0 || grid <= 0){
		return 0;
	}

	long newSizeX = long(sizeX / factor);
	long newSizeY = long(sizeY / factor);
	Matrix<T>* newMatrix = new Matrix<T>(newSizeX, newSizeY);
	T* pMatrix = newMatrix->getMatrixAddress(0, 0);

	long limitX = sizeX + factor - grid;
	long limitY = sizeX + factor - grid;

	if(grid == 1){
		for(long y=0, ny=0; y<limitY; y+=factor, ny++){
			for(long x=0, nx=0; x<limitX; x+=factor, nx++){
				if(matrix[y*sizeX + x]>=0){
					pMatrix[ny*newSizeX + nx] = matrix[y*sizeX + x];
				}
				else{
					pMatrix[ny*newSizeX + nx] = 0;
				}
			}
		}
	}
	else{
		for(long y=0, ny=0; y<limitY; y+=factor, ny++){
			for(long x=0, nx=0; x<limitX; x+=factor, nx++){
				long long count = 0;
				long long sum = 0;
				for(long gy=0; gy<grid; gy++){
					for(long gx=0; gx<grid; gx++){
						if((long long)matrix[(y+gy)*sizeX + x+gx]>=0){
							sum += (long long)matrix[(y+gy)*sizeX + x+gx];
							count++;
						}
					}
				}
				if(count > 0){
					pMatrix[ny*newSizeX + nx] = (T)(sum / count);
				}
				else{
					pMatrix[ny*newSizeX + nx] = 0;
				}
			}
		}
	}

	return newMatrix;
}

template<class T> inline T Matrix<T>::getMatrixValue(long  x, long  y)
{
	return matrix[y*sizeX+x];
}

template<class T> inline T* Matrix<T>::getMatrixAddress(long  x, long  y)
{
	return &matrix[y*sizeX+x];
}

template<class T> inline void Matrix<T>::setMatrixValue(long  x, long  y, const T value)
{
/*
	if(x >= sizeX || y >= sizeY){
		return;
	}
*/
	matrix[y*sizeX+x] = value;
}

template<class T> inline void Matrix<T>::clear()
{
	sizeX = 0;
	sizeY = 0;
	if(initialize && matrix){
		delete[] matrix;
		matrix = 0;
		initialize = false;
	}
}


#endif /* MATRIX_H_ */
