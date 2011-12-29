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
	bool					smoothing;
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

	bool isSmoothing(){
		return smoothing;
	};

	bool isFullConvert(){
		return fullConvert;
	}

	void help();

private:
	bool setInputPath(const char* value);
	bool setOutputPath(const char* value);
	bool setSmoothing(const char* value);
	bool setFullConvert(const char* value);
};

#endif /* COMMANDLINE_H_ */
