//
//  Dct.h
//  DEM2JSON-OSX
//
//  Created by mm on 2013/12/30.
//  Copyright (c) 2013年 Individual. All rights reserved.
//

#ifndef __DEM2JSON_OSX__Dct__
#define __DEM2JSON_OSX__Dct__

#include "CommonInclude.h"
#include "Matrix.h"
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

class Dct{
public:
	Dct();
	virtual ~Dct();

private:
    Matrix<short>   *originMatrix;
	long            (*resultMatrix)[MATRIX_SIZE+4];
    
    double          (*xDctResult)[MATRIX_SIZE+4];
    double          (*yDctResult)[MATRIX_SIZE+4];
    double          (*xIDctResult)[MATRIX_SIZE+4];
    
    static double   factor0;
    static double   factor1;
    static double   dctFactor[MATRIX_SIZE+4][MATRIX_SIZE+4];
    static double   iDctFactor[MATRIX_SIZE+4][MATRIX_SIZE+4];

public:
    static void prepare();
    void doDCT(Matrix<short> *originMatrix,
               long          (*resultMatrix)[MATRIX_SIZE+4],
               long          dctRadius);

private:
    void dct();
    void lpf(long dctRadius);
    void idct();
};

#endif /* defined(__DEM2JSON_OSX__Dct__) */
