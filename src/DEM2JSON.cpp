//============================================================================
// Name        : DEM2JSON.cpp
// Author      : mm
// Copyright   : This tool and Javascript code are licensed under the MIT license.
// Description :
//============================================================================

#include "CommandLine.h"
#include "DEMMatrix.h"

int main(int argc, char* argv[])
{
	std::cout << "This is a DEM(ASTER) to JSON converter." << std::endl;

	CommandLine* pParam = CommandLine::getInstance();
	if(!pParam){
		std::cout << "Fatal error. (Initializing command line parser failed.)" << std::endl;
		return -1;
	}
	if(!pParam->parse(argc, argv)){
		return -1;
	}

	try {
		DEMMatrix* pDEM = new DEMMatrix();
		pDEM->readDEMFolder(pParam->getInputPath());
		pDEM->convertToJson(pParam->getInputPath(), pParam->getOutputPath(), pParam->isSmoothing());
		pDEM->writeArea(pParam->getOutputPath());
		
		delete pDEM;
	}
	catch(...){
		std::cout << "Fatal error. (Initializing DEM file reader failed.)" << std::endl;
		return -1;
	}

	std::cout << "Converting finished." << std::endl;
	return 0;
}
