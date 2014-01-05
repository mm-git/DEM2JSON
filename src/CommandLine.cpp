//============================================================================
// Name        : CommandLine.cpp
// Author      : mm
// Copyright   : This tool and Javascript code are licensed under the MIT license.
// Description :
//============================================================================

#include "CommandLine.h"

CommandLine* CommandLine::pInstance = 0;

CommandLine::CommandLine() {
	inputPath = "";
	outputPath = "";
	smoothingCount = 0;
    dctRadius = -1;
	fullConvert = false;
}

CommandLine::~CommandLine() {
}

bool CommandLine::parse(
	int			argc,
	char*		argv[])
{
	struct SArgDef{
		const char*	parameter;
		bool		hasValue;
		bool		must;
		bool		help;
		bool		(CommandLine::*pfunc)(const char*);
	} arglist[] = {
		{"-D",    true,  true,  false, &CommandLine::setInputPath},
		{"-J",    true,  true,  false, &CommandLine::setOutputPath},
		{"-S",    true,  false, false, &CommandLine::setSmoothingCount},
		{"-C",    true,  false, false, &CommandLine::setDCTRadius},
		{"-F",    false, false, false, &CommandLine::setFullConvert},
		{"-H",    false, false, true,  0},
		{"-HELP", false, false, true,  0},
		{"", false, 0}
	};
	SArgDef* pArglist;

	for(int i=1; i<argc; i++){
		pArglist = arglist;

		for(;;){
			if(pArglist->parameter[0] == 0){
				std::cout << "[ERROR] invalid argument." << std::endl;
				help();
				return false;
			}

			if(strcmp(argv[i], pArglist->parameter) == 0){
				if(pArglist->help){
					help();
					return false;
				}

				if(pArglist->hasValue){
					if(i+1 >= argc){
						std::cout << "[ERROR] argument " << argv[i] << " needs a value." << std::endl;
						help();
						return false;
					}
					if((this->*(pArglist->pfunc))(argv[i+1]) == false){
						help();
						return false;
					}
					i++;
				}
				else{
					(this->*(pArglist->pfunc))(0);
				}

				pArglist->must = false;
				break;
			}

			pArglist++;
		}
	}

	// checking must parameter to be assigned
	bool result = true;
	pArglist = arglist;
	while(pArglist->parameter[0] != 0){
		if(pArglist->must){
			std::cout << "[ERROR] The argument " << pArglist->parameter << " was not specified." << std::endl;
			result = false;
		}
		pArglist++;
	}
	if(!result){
		help();
	}

	return result;
}

bool CommandLine::setInputPath(const char* value){
	if(!boost::filesystem::is_directory(boost::filesystem::path(value))){
		std::cout << "[ERROR] Input folder is not valid." << std::endl;
		return false;
	}

	inputPath = value;
	return true;
};

bool CommandLine::setOutputPath(const char* value){
	try{
		boost::filesystem::is_empty(boost::filesystem::path(value));

		if(!boost::filesystem::is_directory(boost::filesystem::path(value))){
			std::cout << "[ERROR] Outnput folder is not folder." << std::endl;
			return false;
		}
	}
	catch(...){
		boost::filesystem::create_directories(boost::filesystem::path(value));
	}

	outputPath = value;
	if(outputPath[outputPath.length()-1] != '/'){
		outputPath += "/";
	}
	return true;
};

bool CommandLine::setSmoothingCount(const char* value){
    size_t len = strlen(value);
    
    for(int i=0; i<len; i++){
        if(value[i] < '0' || value[i] > '9'){
			std::cout << "[ERROR] '-S' parameter is not decimal." << std::endl;
            return false;
        }
    }
	
    smoothingCount = atoi(value);
	return true;
};

bool CommandLine::setDCTRadius(const char* value){
    size_t len = strlen(value);
    
    for(int i=0; i<len; i++){
        if(value[i] < '0' || value[i] > '9'){
			std::cout << "[ERROR] '-C' parameter is not decimal." << std::endl;
            return false;
        }
    }

    dctRadius = atoi(value);
    return true;
}

bool CommandLine::setFullConvert(const char* value){
	fullConvert = true;
	return true;
};

void CommandLine::help()
{
	std::cout << std::endl ;
	std::cout << "Usage: DEM2JSON -D path/to/ASTER -J path/to/output [-S]" << std::endl ;
	std::cout << "  options" << std::endl;
	std::cout << "    -H  --help    Prints this help " << std::endl;
	std::cout << "    -D            ASTER DEM data folder (input folder)" << std::endl ;
	std::cout << "    -J            JSON data folder (output folder)" << std::endl ;
	std::cout << "    -S n          Make a output JSON data smoothed by moving average method.(optional)" << std::endl ;
	std::cout << "                  n is the number of smoothing executions." << std::endl ;
	std::cout << "    -C r          Make a output JSON data smoothed by DCT.(optional)" << std::endl ;
	std::cout << "                  r is the radius of low pass filter." << std::endl ;
	std::cout << "    -F            To convert all data.(optional)" << std::endl ;
	std::cout << std::endl ;
}
