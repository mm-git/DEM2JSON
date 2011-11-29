//============================================================================
// Name        : DEMMatrix.h
// Author      : mm
// Copyright   : This codes are licensed under the MIT license.
// Create      : 2011/11/04
// Description :
//============================================================================
#ifndef DEMMATRIX_H_
#define DEMMATRIX_H_

#include "CommonInclude.h"
#include "GeoTiffReader.h"

class DEMMatrix {
public:
	DEMMatrix();
	virtual ~DEMMatrix();

private:
	struct demFileMatrix {
		std::string		fileName;
		GeoTiffReader*	pTiffData;
	};
	demFileMatrix demFileMatrix[180][360];	// 0-180:S90-S01,N00-N90 , 0-360:W180-W001,E000-E179
	long demFileCount;

public:
	bool readDEMFolder(std::string&		inputFolder);
	bool convertToJson(std::string&		inputFolder,
					   std::string&		outputFolder,
					   bool				smoothing);
	bool writeArea(std::string&			outputFolder);

private:
	bool Unzip(std::string&				inputFolder,
			   std::string&				demFile,
			   std::string&				target);
};

#endif /* DEMMATRIX_H_ */
