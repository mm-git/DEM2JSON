//============================================================================
// Name        : GeoTiffReader.cpp
// Author      : mm
// Copyright   : This tool and Javascript code are licensed under the MIT license.
// Description :
//============================================================================
#include "GeoTiffReader.h"
#include <cctype>
#include <fstream>

GeoTiffReader::GeoTiffReader(std::string& tiffFile)
	: demMatrix(),
	  demMatrixMini(0)
{
	readGeoTiffFile(tiffFile);
}

GeoTiffReader::~GeoTiffReader()
{
	if(demMatrixMini){
		delete demMatrixMini;
	}

}

bool GeoTiffReader::readGeoTiffFile(std::string& tiffFile)
{
	if(tiffFile == ""){
		return false;
	}

	std::cout << "Reading " << tiffFile << " ..." << std::endl;

	map = boost::interprocess::file_mapping(
		tiffFile.c_str(),
		boost::interprocess::read_only
	);
	view = boost::interprocess::mapped_region(
		map,
		boost::interprocess::read_only,
		8,
		TIFF_MATRIX_SIZE*TIFF_MATRIX_SIZE*sizeof(short));

	demMatrix.attach((short*)view.get_address(), TIFF_MATRIX_SIZE, TIFF_MATRIX_SIZE);
	demMatrixMini = demMatrix.minimize(3, 4);

	return true;
}
