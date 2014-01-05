//============================================================================
// Name        : CommandLine.h
// Author      : mm
// Copyright   : This tool and Javascript code are licensed under the MIT license.
// Description :
//============================================================================

#ifndef COMMANDLINE_H_
#define COMMANDLINE_H_

#include "CommonInclude.h"

class CommandLine;

class CommandLine {
private:
	CommandLine();
	virtual ~CommandLine();

private:
	static CommandLine*		pInstance;
	std::string				inputPath;
	std::string				outputPath;
	unsigned long           smoothingCount;
    long                    dctRadius;      // -1 means not to do DCT
	bool					fullConvert;

public:
	static CommandLine* getInstance(){
		if( !pInstance ){
			try{
				pInstance = new CommandLine();
			}
			catch(...){
				return 0;
			}
		}
		return pInstance;
	};

	bool parse(
		int			argc,
		char*		argv[]
	);

	std::string& getInputPath(){
		return inputPath;
	};

	std::string& getOutputPath(){
		return outputPath;
	};

	unsigned long getSmoothingCount(){
		return smoothingCount;
	};

    long getDCTRadius(){
        return dctRadius;
    };
    
	bool isFullConvert(){
		return fullConvert;
	}

	void help();

private:
	bool setInputPath(const char* value);
	bool setOutputPath(const char* value);
	bool setSmoothingCount(const char* value);
    bool setDCTRadius(const char* value);
	bool setFullConvert(const char* value);
};

#endif /* COMMANDLINE_H_ */
