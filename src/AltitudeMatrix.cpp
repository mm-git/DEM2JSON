//============================================================================
// Name        : AltitudeMatrix.cpp
// Author      : mm
// Copyright   : This tool and Javascript code are licensed under the MIT license.
// Description :
//============================================================================
#include "AltitudeMatrix.h"
#include "GeoTiffReader.h"
#include <boost/format.hpp>
#include <cctype>
#include <fstream>

AltitudeMatrix::AltitudeMatrix() {
	masterMatrix.init(MATRIX_SIZE+4, MATRIX_SIZE+4);
	memset(matrix, 0, sizeof(matrix));
	smoothing = false;
	baseFileName = "";
}


AltitudeMatrix::~AltitudeMatrix() {
}

void AltitudeMatrix::clear()
{
	masterMatrix.init(MATRIX_SIZE+4, MATRIX_SIZE+4);
	memset(matrix, 0, sizeof(matrix));
	smoothing = false;
}

bool AltitudeMatrix::setDEMMatrix(
	Matrix<short>* pMatrix11, Matrix<short>* pMatrix12, Matrix<short>* pMatrix13,
	Matrix<short>* pMatrix21, Matrix<short>* pMatrix22, Matrix<short>* pMatrix23,
	Matrix<short>* pMatrix31, Matrix<short>* pMatrix32, Matrix<short>* pMatrix33)
{
	if(!pMatrix22){
		return false;
	}

	masterMatrix.copy(pMatrix31, MATRIX_SIZE-2, MATRIX_SIZE-2, 2,           2,           0,             0);
	masterMatrix.copy(pMatrix32, 0,             MATRIX_SIZE-2, MATRIX_SIZE, 2,           2,             0);
	masterMatrix.copy(pMatrix33, 0,             MATRIX_SIZE-2, 2,           2,           MATRIX_SIZE+2, 0);
	masterMatrix.copy(pMatrix21, MATRIX_SIZE-2, 0,             2,           MATRIX_SIZE, 0,             2);
	masterMatrix.copy(pMatrix22, 0,             0,             MATRIX_SIZE, MATRIX_SIZE, 2,             2);
	masterMatrix.copy(pMatrix23, MATRIX_SIZE-2, 0,             2,           MATRIX_SIZE, MATRIX_SIZE+2, 2);
	masterMatrix.copy(pMatrix11, MATRIX_SIZE-2, 0,             2,           2,           0,             MATRIX_SIZE+2);
	masterMatrix.copy(pMatrix12, 0,             0,             MATRIX_SIZE, 2,           2,             MATRIX_SIZE+2);
	masterMatrix.copy(pMatrix13, 0,             0,             2,           2,           MATRIX_SIZE+2, MATRIX_SIZE+2);

	if(!pMatrix12){
		masterMatrix.copy(&masterMatrix, 2, 2, MATRIX_SIZE, 1, 2, 0);
		masterMatrix.copy(&masterMatrix, 2, 2, MATRIX_SIZE, 1, 2, 1);
	}
	if(!pMatrix21){
		masterMatrix.copy(&masterMatrix, 2, 2, 1, MATRIX_SIZE, 0, 2);
		masterMatrix.copy(&masterMatrix, 2, 2, 1, MATRIX_SIZE, 1, 2);
	}
	if(!pMatrix23){
		masterMatrix.copy(&masterMatrix, MATRIX_SIZE+1, 2, 1, MATRIX_SIZE, MATRIX_SIZE+2, 2);
		masterMatrix.copy(&masterMatrix, MATRIX_SIZE+1, 2, 1, MATRIX_SIZE, MATRIX_SIZE+3, 2);
	}
	if(!pMatrix32){
		masterMatrix.copy(&masterMatrix, 2, MATRIX_SIZE+1, MATRIX_SIZE, 1, 2, MATRIX_SIZE+2);
		masterMatrix.copy(&masterMatrix, 2, MATRIX_SIZE+1, MATRIX_SIZE, 1, 2, MATRIX_SIZE+3);
	}

	if(!pMatrix11){
		masterMatrix.setMatrixValue(0, 1, masterMatrix.getMatrixValue(0, 2));
		masterMatrix.setMatrixValue(1, 1, masterMatrix.getMatrixValue(2, 2));
		masterMatrix.setMatrixValue(1, 0, masterMatrix.getMatrixValue(2, 0));
		masterMatrix.setMatrixValue(0, 0, (masterMatrix.getMatrixValue(0, 1) + masterMatrix.getMatrixValue(1, 1) + masterMatrix.getMatrixValue(1, 0))/3);
	}
	if(!pMatrix13){
		masterMatrix.setMatrixValue(0, MATRIX_SIZE+2, masterMatrix.getMatrixValue(0, MATRIX_SIZE+1));
		masterMatrix.setMatrixValue(1, MATRIX_SIZE+2, masterMatrix.getMatrixValue(2, MATRIX_SIZE+1));
		masterMatrix.setMatrixValue(1, MATRIX_SIZE+3, masterMatrix.getMatrixValue(2, MATRIX_SIZE+3));
		masterMatrix.setMatrixValue(0, MATRIX_SIZE+3, (masterMatrix.getMatrixValue(0, MATRIX_SIZE+2) + masterMatrix.getMatrixValue(1, MATRIX_SIZE+2) + masterMatrix.getMatrixValue(1, MATRIX_SIZE+3))/3);
	}
	if(!pMatrix31){
		masterMatrix.setMatrixValue(MATRIX_SIZE+2, 0, masterMatrix.getMatrixValue(MATRIX_SIZE+1, 0));
		masterMatrix.setMatrixValue(MATRIX_SIZE+2, 1, masterMatrix.getMatrixValue(MATRIX_SIZE+1, 2));
		masterMatrix.setMatrixValue(MATRIX_SIZE+3, 1, masterMatrix.getMatrixValue(MATRIX_SIZE+3, 2));
		masterMatrix.setMatrixValue(MATRIX_SIZE+3, 0, (masterMatrix.getMatrixValue(MATRIX_SIZE+2, 0) + masterMatrix.getMatrixValue(MATRIX_SIZE+2, 1) + masterMatrix.getMatrixValue(MATRIX_SIZE+3, 1))/3);
	}
	if(!pMatrix33){
		masterMatrix.setMatrixValue(MATRIX_SIZE+2, MATRIX_SIZE+3, masterMatrix.getMatrixValue(MATRIX_SIZE+1, MATRIX_SIZE+3));
		masterMatrix.setMatrixValue(MATRIX_SIZE+2, MATRIX_SIZE+2, masterMatrix.getMatrixValue(MATRIX_SIZE+1, MATRIX_SIZE+1));
		masterMatrix.setMatrixValue(MATRIX_SIZE+3, MATRIX_SIZE+2, masterMatrix.getMatrixValue(MATRIX_SIZE+3, MATRIX_SIZE+1));
		masterMatrix.setMatrixValue(MATRIX_SIZE+3, MATRIX_SIZE+3, (masterMatrix.getMatrixValue(MATRIX_SIZE+2, MATRIX_SIZE+3) + masterMatrix.getMatrixValue(MATRIX_SIZE+2, MATRIX_SIZE+2) + masterMatrix.getMatrixValue(MATRIX_SIZE+3, MATRIX_SIZE+2))/3);
	}

	return true;
}

bool AltitudeMatrix::setDEMMatrix(Matrix<short>* pDemMatrix)
{
	if(!pDemMatrix){
		return false;
	}

	masterMatrix.copy(pDemMatrix, 0, 0, MATRIX_SIZE, MATRIX_SIZE, 2, 2);

	return true;
}

void AltitudeMatrix::doSmoothing() {
	long i;
	long j;

	std::cout << "Smoothing ..." << std::endl;

	for(i=0; i<MATRIX_SIZE+4; i++){
		for(j=0; j<MATRIX_SIZE+4; j++){
			matrix[eFactor4][i][j] = (long)masterMatrix.getMatrixValue(j, i) * 4;
			matrix[eFactor6][i][j] = (long)masterMatrix.getMatrixValue(j, i) * 6;
			matrix[eFactor16][i][j] = (long)masterMatrix.getMatrixValue(j, i) * 16;
			matrix[eFactor24][i][j] = (long)masterMatrix.getMatrixValue(j, i) * 24;
			matrix[eFactor36][i][j] = (long)masterMatrix.getMatrixValue(j, i) * 36;
		}
	}

	for(i=2; i<MATRIX_SIZE+2; i++){
		for(j=2; j<MATRIX_SIZE+2; j++){
			matrix[eResult][i][j] =
				(((long)masterMatrix.getMatrixValue(j-2, i-2) + matrix[eFactor4][i-1][j-2]  + matrix[eFactor6][i][j-2]  + matrix[eFactor4][i+1][j-2]  + (long)masterMatrix.getMatrixValue(j-2,i+2) +
				  matrix[eFactor4][i-2][j-1]                  + matrix[eFactor16][i-1][j-1] + matrix[eFactor24][i][j-1] + matrix[eFactor16][i+1][j-1] + matrix[eFactor4][i+2][j-1] +
				  matrix[eFactor6][i-2][j]                    + matrix[eFactor24][i-1][j]   + matrix[eFactor36][i][j]   + matrix[eFactor24][i+1][j]   + matrix[eFactor6][i+2][j] +
				  matrix[eFactor4][i-2][j+1]                  + matrix[eFactor16][i-1][j+1] + matrix[eFactor24][i][j+1] + matrix[eFactor16][i+1][j+1] + matrix[eFactor4][i+2][j+1] +
				  (long)masterMatrix.getMatrixValue(j+2, i-2) + matrix[eFactor4][i-1][j+2]  + matrix[eFactor6][i][j+2]  + matrix[eFactor4][i+1][j+2]  + (long)masterMatrix.getMatrixValue(j+2,i+2))
				/ 256);
		}
	}

	smoothing = true;
}

bool AltitudeMatrix::setBaseFile(std::string & demFileName)
{
	baseFileName = boost::filesystem::path(demFileName).stem().generic_string();

	long latPos = baseFileName.find("_");
	baseFileName = baseFileName.substr(latPos+1, 7);
	return true;
}

void AltitudeMatrix::verify(Matrix<short > *pOrgMatrix)
{
	for(long i=2; i<MATRIX_SIZE+2; i++){
		for(long j=2; j<MATRIX_SIZE+2; j++){
			short orgAltitude = pOrgMatrix->getMatrixValue((j-2)*3, (i-2)*3);
			short resultAltitude = smoothing ? matrix[eResult][i][j] : masterMatrix.getMatrixValue(j, i);

			if(abs(orgAltitude - resultAltitude) > HEIGHT_THRESHOLD && resultAltitude==0 && orgAltitude>0){
				std::cout << baseFileName << " x=" << (j-2) << " y=" << (i-2) << " org=" << orgAltitude << " smooth=" << resultAltitude << std::endl;
				if(smoothing){
					matrix[eResult][i][j] = orgAltitude;
				}
				else{
					masterMatrix.setMatrixValue(j, i, orgAltitude);
				}
			}
		}
	}
}

bool AltitudeMatrix::writeJSON(std::string& outputFolder)
{
	std::cout << "Writing " << baseFileName << " ..." <<std::endl;

	for(long k=0; k<3; k++){
		for(long l=0; l<3; l++){
			std::string outputFileName = outputFolder + baseFileName + (boost::format("M%1%%2%.json") % k % l).str();

			std::ofstream outStream(outputFileName.c_str(), std::ios::out |  std::ios::binary );
			if(!outStream){
				return false;
			}
			outStream << "{\"h\" : [" << std::endl;

			for(long i=0; i<MATRIX_SIZE/3; i++){
				short previous = 0;
				short diff = 0;
				short count = 0;

				std::string lineString = "\"";

				for(long j=0; j<MATRIX_SIZE/3; j++){
					short current;
					if(smoothing){
						current = matrix[eResult][(3-k)*(MATRIX_SIZE/3)-i+1][l*MATRIX_SIZE/3+j+2];
					}
					else{
						current = masterMatrix.getMatrixValue(l*MATRIX_SIZE/3+j+2, (3-k)*(MATRIX_SIZE/3)-i+1);
					}

					diff = current - previous;
					if(diff == 0){
						if(count == 0){
							lineString += ":";
						}
						count++;
					}
					else{
						if(count>0){
							lineString += heightEncode(count);
							count = 0;
						}

						diff = diff << 1;
						if(diff & 0x8000){
							diff = ~diff;
						}
						lineString += heightEncode(diff);
					}
					previous = current;
				}
				if(count>0){
					lineString += heightEncode(count);
				}
				if(i == MATRIX_SIZE/3-1){
					lineString += "\"";
				}
				else{
					lineString += "\",";
				}
				outStream << lineString << std::endl;
			}
			outStream << "]}" << std::endl;
			outStream.close();
		}
	}
	return true;
}

char* AltitudeMatrix::heightEncode(short height)
{
	static char caEncodeString[10];
	memset(caEncodeString, 0, sizeof(caEncodeString));

	long i=0;
	while(height >= 0x20){
		caEncodeString[i] = (char)((0x20 | (height & 0x1f)) + 0x3f);
		height >>= 5;
		i++;
	}
	if(height + 0x3f == 0x5c){
		caEncodeString[i] = 0x5c;
		caEncodeString[i+1] = 0x5c;
	}
	else{
		caEncodeString[i] = (char)(height + 0x3f);
	}

	return caEncodeString;
}
