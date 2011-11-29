//============================================================================
// Name        : GeoTiffReader.h
// Author      : mm
// Copyright   : This tool and Javascript code are licensed under the MIT license.
// Description :
//============================================================================
#ifndef GEOTIFFREADER_H_
#define GEOTIFFREADER_H_

#include "CommonInclude.h"
#include "Matrix.h"
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

#define TIFF_MATRIX_SIZE 3601

class GeoTiffReader  
{
public:
	GeoTiffReader(std::string& tiffFile);
	virtual ~GeoTiffReader();

private:
	Matrix<short>		demMatrix;
	Matrix<short>*		demMatrixMini;
	boost::interprocess::file_mapping map;
	boost::interprocess::mapped_region view;

public:
	Matrix<short>* getMatrix(){
		return &demMatrix;
	}
	Matrix<short>* getMiniMatrix(){
		return demMatrixMini;
	}

private:
	bool readGeoTiffFile(std::string &tiffFile);
};

#endif
