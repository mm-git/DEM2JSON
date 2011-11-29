//============================================================================
// Name        : DEMMatrix.cpp
// Author      : mm
// Copyright   : This codes are licensed under the MIT license.
// Create      : 2011/11/04
// Description :
//============================================================================
#include "DEMMatrix.h"
#include "AltitudeMatrix.h"
#include <boost/format.hpp>
#include <cctype>
#include <fstream>
#include "zlib.h"
#include "unzip.h"

DEMMatrix::DEMMatrix() {
	for(long i=0; i<180; i++){
		for(long j=0; j<360; j++){
			demFileMatrix[i][j].pTiffData = 0;
		}
	}
	demFileCount = 0;
}

DEMMatrix::~DEMMatrix() {
}

bool DEMMatrix::readDEMFolder(std::string& inputFolder) {
	boost::filesystem::directory_iterator end;

	for (boost::filesystem::directory_iterator it(inputFolder); it != end; it++) {
		if (boost::filesystem::is_directory(*it)) {
			continue;
		}

		std::string filename = it->path().filename().generic_string();
		std::transform(filename.begin(), filename.end(), filename.begin(), (int(*)(int))std::toupper);boost
		::regex reg("ASTGTM\\d?_[SN]\\d{2}[WE]\\d{3}\\.ZIP");
		if (!boost::regex_match(filename, reg)) {
			std::cout << "[WARNING] invalid file detected. (" << filename << ")" << std::endl;
			continue;
		}

		// 0-180:S90-S01,N00-N90 , 0-360:W180-W001,E000-E179
		long latPos = filename.find("_");

		long lat;
		if (filename[latPos+1] == 'S') {
			lat = 90 - atoi(filename.substr(latPos+2, 2).c_str());
		} else {
			lat = 90 + atoi(filename.substr(latPos+2, 2).c_str());
		}
		if (lat < 0 || lat >= 180) {
			std::cout << "[WARNING] invalid file detected. (" << filename << ")"	<< std::endl;
			continue;
		}
		long lng;
		if (filename[latPos+4] == 'W') {
			lng = 180 - atoi(filename.substr(latPos+5, 3).c_str());
		} else {
			lng = 180 + atoi(filename.substr(latPos+5, 3).c_str());
		}
		if (lng < 0 || lng >= 360) {
			std::cout << "[WARNING] invalid file detected. (" << filename << ")" << std::endl;
			continue;
		}
		demFileMatrix[lat][lng].fileName = filename;
		demFileCount++;
	}

	std::cout << "The number of GDEM files : " << demFileCount << std::endl;

	return true;
}

#define TIF(x) if(!x.pTiffData){ x.pTiffData = new GeoTiffReader(x.fileName); }
#define DELETE_POINTER(x) if(x){delete x; x=0;}
bool DEMMatrix::convertToJson(
	std::string&	inputFolder,
	std::string&	outputFolder,
	bool			smoothing)
{
	long i;
	long j;
	long j1;
	long j2;
	long j3;

	std::string tempFolder(TEMP_FOLDER);
	boost::filesystem::create_directories(boost::filesystem::path(tempFolder));

	AltitudeMatrix* pAltitude = new AltitudeMatrix();

	// Actual latitude range is between -80 to 80, so checking 'i' omit.
	for(i=0; i<180; i++){
		for(j=0; j<360; j++){
			try{
				if(smoothing){
					if(demFileMatrix[i][j].fileName != ""){
						j1 = (j == 0 ? 359 : j-1);
						j2 = j;
						j3 = (j == 359 ? 0 : j+1);

						Unzip(inputFolder, demFileMatrix[i-1][j1].fileName, tempFolder);
						Unzip(inputFolder, demFileMatrix[i-1][j2].fileName, tempFolder);
						Unzip(inputFolder, demFileMatrix[i-1][j3].fileName, tempFolder);
						Unzip(inputFolder, demFileMatrix[i][j1].fileName, tempFolder);
						Unzip(inputFolder, demFileMatrix[i][j2].fileName, tempFolder);
						Unzip(inputFolder, demFileMatrix[i][j3].fileName, tempFolder);
						Unzip(inputFolder, demFileMatrix[i+1][j1].fileName, tempFolder);
						Unzip(inputFolder, demFileMatrix[i+1][j2].fileName, tempFolder);
						Unzip(inputFolder, demFileMatrix[i+1][j3].fileName, tempFolder);

						if(demFileMatrix[i][j2].fileName != ""){
							TIF(demFileMatrix[i-1][j1]);
							TIF(demFileMatrix[i-1][j2]);
							TIF(demFileMatrix[i-1][j3]);
							TIF(demFileMatrix[i][j1]);
							TIF(demFileMatrix[i][j2]);
							TIF(demFileMatrix[i][j3]);
							TIF(demFileMatrix[i+1][j1]);
							TIF(demFileMatrix[i+1][j2]);
							TIF(demFileMatrix[i+1][j3]);

							pAltitude->setDEMMatrix(
								demFileMatrix[i-1][j1].pTiffData->getMiniMatrix(),
								demFileMatrix[i-1][j2].pTiffData->getMiniMatrix(),
								demFileMatrix[i-1][j3].pTiffData->getMiniMatrix(),
								demFileMatrix[i][j1].pTiffData->getMiniMatrix(),
								demFileMatrix[i][j2].pTiffData->getMiniMatrix(),
								demFileMatrix[i][j3].pTiffData->getMiniMatrix(),
								demFileMatrix[i+1][j1].pTiffData->getMiniMatrix(),
								demFileMatrix[i+1][j2].pTiffData->getMiniMatrix(),
								demFileMatrix[i+1][j3].pTiffData->getMiniMatrix()
							);
							pAltitude->doSmoothing();
							pAltitude->setBaseFile(demFileMatrix[i][j].fileName);
							pAltitude->verify(demFileMatrix[i][j2].pTiffData->getMatrix());
							pAltitude->writeJSON(outputFolder);
						}

						DELETE_POINTER(demFileMatrix[i-1][j1].pTiffData);
						DELETE_POINTER(demFileMatrix[i][j1].pTiffData);
						DELETE_POINTER(demFileMatrix[i+1][j1].pTiffData);
						if(j == 359){
							DELETE_POINTER(demFileMatrix[i-1][j2].pTiffData);
							DELETE_POINTER(demFileMatrix[i][j2].pTiffData);
							DELETE_POINTER(demFileMatrix[i+1][j2].pTiffData);
							DELETE_POINTER(demFileMatrix[i-1][j3].pTiffData);
							DELETE_POINTER(demFileMatrix[i][j3].pTiffData);
							DELETE_POINTER(demFileMatrix[i+1][j3].pTiffData);
						}
						else{
							if(j == 358 && demFileMatrix[i][j3].fileName == ""){
									DELETE_POINTER(demFileMatrix[i-1][j2].pTiffData);
									DELETE_POINTER(demFileMatrix[i][j2].pTiffData);
									DELETE_POINTER(demFileMatrix[i+1][j2].pTiffData);
									DELETE_POINTER(demFileMatrix[i-1][j3].pTiffData);
									DELETE_POINTER(demFileMatrix[i][j3].pTiffData);
									DELETE_POINTER(demFileMatrix[i+1][j3].pTiffData);
							}
							else{
								if(demFileMatrix[i][j3].fileName == ""){
									DELETE_POINTER(demFileMatrix[i-1][j2].pTiffData);
									DELETE_POINTER(demFileMatrix[i][j2].pTiffData);
									DELETE_POINTER(demFileMatrix[i+1][j2].pTiffData);
									if(demFileMatrix[i][j+2].fileName == ""){
										DELETE_POINTER(demFileMatrix[i-1][j3].pTiffData);
										DELETE_POINTER(demFileMatrix[i][j3].pTiffData);
										DELETE_POINTER(demFileMatrix[i+1][j3].pTiffData);
									}
								}
							}
						}
					}
				}
				else{
					if(demFileMatrix[i][j].fileName != ""){
						if(Unzip(inputFolder, demFileMatrix[i][j].fileName, tempFolder)){
							TIF(demFileMatrix[i][j]);
							pAltitude->setDEMMatrix(demFileMatrix[i][j].pTiffData->getMiniMatrix());
							pAltitude->setBaseFile(demFileMatrix[i][j].fileName);
							pAltitude->writeJSON(outputFolder);
							DELETE_POINTER(demFileMatrix[i][j].pTiffData);
						}
					}
				}
			}
			catch(...){
				std::cout << "[ERROR] Converting failed. (" << i << "," << j << ")" << std::endl;
			}
		}
	}
	delete pAltitude;

	boost::filesystem::remove_all(boost::filesystem::path(tempFolder));

	return true;
}

bool DEMMatrix::writeArea(std::string& outputFolder)
{
	std::cout << "Writing area file ..." << std::endl;

	std::string outputFileName = outputFolder + AREA_FILE;
	std::ofstream outStream(outputFileName.c_str(), std::ios::out |  std::ios::binary );
	if(!outStream){
		return false;
	}

	outStream << "{'a' : [" << std::endl;
	for(long i=0; i<180; i++){
		std::string lineString = "\"";
		unsigned char shift = 0x80;
		unsigned char hex = 0;

		for(long j=0; j<360; j++){
			if(demFileMatrix[i][j].fileName != ""){
				hex |= shift;
			}
			shift >>= 1;
			if(shift == 0){
				lineString += (boost::format("%|02x|") % (int)(unsigned char)(hex)).str();
				hex = 0;
				shift = 0x80;
			}
		}

		if(i == 179){
			lineString += "\"";
		}
		else{
			lineString += "\",";
		}
		outStream << lineString << std::endl;
	}
	outStream << "]}" << std::endl;
	outStream.close();

	return true;
}

bool DEMMatrix::Unzip(std::string& inputFolder, std::string& demFile, std::string& target)
{
	if(demFile == ""){
		return false;
	}
	if(stricmp(demFile.substr(demFile.length()-3, 3).c_str(), "ZIP") != 0){
		return false;
	}

	boost::filesystem::path zipFile(inputFolder);
	zipFile /= boost::filesystem::path(demFile);

	std::cout << "Unzipping " << demFile << " ..." << std::endl;
	unzFile hUnzip = unzOpen(zipFile.generic_string().c_str());
	try  {
		if (!hUnzip){
			throw 0;
		}

		do {
			char packedFilename[512];
			unz_file_info fileInfo;
			if (unzGetCurrentFileInfo(hUnzip, &fileInfo, packedFilename, sizeof(packedFilename), NULL, 0, NULL, 0) != UNZ_OK){
				throw;
			}

			// to unzip only %s_dem.tif file
			if(std::string(packedFilename).find("_dem.tif") == std::string::npos){
				continue;
			}

			std::string targetPath = target;
			targetPath += boost::filesystem::path(packedFilename).filename().generic_string();

			if (unzOpenCurrentFile(hUnzip) != UNZ_OK){
				throw;
			}
			std::ofstream outStream(targetPath.c_str(), std::ios::out | std::ios::binary);
			if(!outStream){
				unzCloseCurrentFile(hUnzip);
				throw;
			}
			char buffer[8192];
			unsigned long readSize;
			while ((readSize = unzReadCurrentFile(hUnzip, buffer, sizeof(buffer))) > 0){
				outStream.write(buffer, readSize);
			}
			outStream.close();
			unzCloseCurrentFile(hUnzip);
			break;
		} while (unzGoToNextFile(hUnzip) != UNZ_END_OF_LIST_OF_FILE);
	}
	catch(...){
		std::cout << "[ERROR] Unzip error." << std::endl;
		demFile = "";
		return false;
	}
	unzClose(hUnzip);

	std::string replace("_dem.tif");
	demFile.replace(demFile.length()-4, replace.length(), replace);
	demFile = target + demFile;

	return true;
}



