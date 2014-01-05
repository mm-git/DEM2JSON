//
//  Dct.cpp
//  DEM2JSON-OSX
//
//  Created by mm on 2013/12/30.
//  Copyright (c) 2013年 Individual. All rights reserved.
//

#include "Dct.h"

double Dct::factor0;
double Dct::factor1;
double Dct::dctFactor[MATRIX_SIZE+4][MATRIX_SIZE+4];
double Dct::iDctFactor[MATRIX_SIZE+4][MATRIX_SIZE+4];

Dct::Dct(){
    originMatrix = NULL;
    resultMatrix = NULL;
    
    xDctResult = new double[MATRIX_SIZE+4][MATRIX_SIZE+4];
    yDctResult = new double[MATRIX_SIZE+4][MATRIX_SIZE+4];
    xIDctResult = new double[MATRIX_SIZE+4][MATRIX_SIZE+4];
}

Dct::~Dct(){
}

void Dct::prepare(){
    long xy;
    long k;

    std::cout << "Preparing dct data..." << std::endl;
    
    factor0 = (double)sqrt(1.0/(double)MATRIX_SIZE);
    factor1 = (double)sqrt(2.0/(double)MATRIX_SIZE);

    for(xy=0;xy<MATRIX_SIZE;xy++){
        for(k=0;k<MATRIX_SIZE;k++){
            if(xy == 0){
                dctFactor[xy+2][k+2] = 1.0;
            }
            else{
                dctFactor[xy+2][k+2] = cos(M_PI * (2*k+1)*xy/(double)(2*MATRIX_SIZE));
            }
        }
    }

    for(xy=0;xy<MATRIX_SIZE;xy++){
        for(k=0;k<MATRIX_SIZE;k++){
            if(k != 0){
                iDctFactor[xy+2][k+2] = cos(M_PI * (2*xy+1)*k/(double)(2*MATRIX_SIZE));
            }
        }
    }
}

void Dct::doDCT(Matrix<short> *originMatrix, long (*resultMatrix)[MATRIX_SIZE+4], long dctRadius){
    this->originMatrix = originMatrix;
    this->resultMatrix = resultMatrix;

    dct();
    lpf(dctRadius);
    idct();
}

void Dct::dct(){
    long x;
    long y;
    long k;
    double tempValue;

    std::cout << "Doing dct x axis ..." << std::endl;
    
    for(y=0;y<MATRIX_SIZE;y++){
        for(x=0;x<MATRIX_SIZE;x++){
            tempValue = 0.0;
            for(k=0;k<MATRIX_SIZE;k++){
                if(x == 0){
                    tempValue += (double)originMatrix->getMatrixValue(k+2, y+2);
                }
                else{
                    tempValue += (double)originMatrix->getMatrixValue(k+2, y+2) * dctFactor[x+2][k+2];
                }
            }
            if(x == 0){
                xDctResult[y+2][x+2] = tempValue * factor0;
            }else{
                xDctResult[y+2][x+2] = tempValue * factor1;
            }
        }
    }
    
    std::cout << "Doing dct y axis ..." << std::endl;

    for(x=0;x<MATRIX_SIZE;x++){
        for(y=0;y<MATRIX_SIZE;y++){
            tempValue = 0.0;
            for(k=0;k<MATRIX_SIZE;k++){
                if(y == 0){
                    tempValue += xDctResult[k+2][x+2];
                }
                else{
                    tempValue += xDctResult[k+2][x+2] * dctFactor[y+2][k+2];
                }
            }
            if(y == 0){
                yDctResult[y+2][x+2] = tempValue * factor0;
            }else{
                yDctResult[y+2][x+2] = tempValue * factor1;
            }
        }
    }
}

void Dct::lpf(long dctRadius){
    long x;
    long y;

    std::cout << "Doing dct lpf ..." << std::endl;

    for(y=0; y<MATRIX_SIZE; y++){
        for(x=0; x<MATRIX_SIZE; x++){
            if(sqrt(x*x+y*y)>=dctRadius){
                yDctResult[y+2][x+2]=0;
            }
        }
    }
}

void Dct::idct(){
    long x;
    long y;
    long k;
    double tempValue;

    std::cout << "Doing idct x axis ..." << std::endl;

    for(y=0;y<MATRIX_SIZE;y++){
        for(x=0;x<MATRIX_SIZE;x++){
            tempValue = 0.0;
            for(k=0;k<MATRIX_SIZE;k++){
                if(k != 0){
                    tempValue += yDctResult[y+2][k+2] * iDctFactor[x+2][k+2];
                }
            }
            xIDctResult[y+2][x+2] = tempValue * factor1 + yDctResult[y+2][2] * factor0;
        }
    }
    
    std::cout << "Doing idct y axis ..." << std::endl;

    for(x=0;x<MATRIX_SIZE;x++){
        for(y=0;y<MATRIX_SIZE;y++){
            tempValue = 0.0;
            for(k=0;k<MATRIX_SIZE;k++){
                if(k != 0){
                    tempValue += xIDctResult[k+2][x+2] * iDctFactor[y+2][k+2];
                }
            }
            resultMatrix[y+2][x+2] = tempValue * factor1 + xIDctResult[2][x+2] * factor0;
        }
    }
    
}