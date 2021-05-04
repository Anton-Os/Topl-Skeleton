#include "FileIO.hpp"

std::string readFile(const char* source, bool isBinaryFile){
    std::ifstream file = (!isBinaryFile)? std::ifstream(source, std::ios::in) : std::ifstream(source, std::ios::in | std::ios::binary);
	if(!file.is_open()) return std::string(); // error occured, empty string returned

    // file.ignore(std::numeric_limits<std::streamsize>::max());
    file.clear();
    file.seekg(0, std::ios_base::beg);

    std::stringstream strStream;
    strStream << file.rdbuf();
    file.close();

    return strStream.str();
}

std::string getParentDir(const char* str){
    const char* strEnd = str + strlen(str); // Traverse to the end of the string
    while(strEnd != str && nullptr == strchr("\\/", *(--strEnd)));

    std::size_t length = strEnd - str; // Gets length of string
    std::string parentDir(str, length); // Gets a subset of the string

    return parentDir;
}

void logToFile(const char* fileName, std::string logMessage){
    std::ofstream file(fileName, std::ofstream::out | std::ofstream::app);
    file << logMessage << "\n\n";
    file.close();
}

bool checkFormatObj(const char* source) {
	std::string fileContent = readFile(source, false); // ascii read
	std::string magicKey("OBJFILE.mtl");
	return (fileContent.find(magicKey) != std::string::npos) ? true : false;
}

float getFloatFromStr(const std::string& source, size_t startOffset){
    if(!isdigit(source.at(startOffset)) && source.at(startOffset) != '-'){ // minus symbol is permitted 
        fprintf(stderr, "Invalid offset provided obtaining float value. Location: %d", startOffset);
        return FLT_MAX;
    }

    std::string currentStrVal;
    while(isdigit(source.at(startOffset)) || source.at(startOffset) == '.' || source.at(startOffset) == '-'){ // minus and dot symbol permitted
        currentStrVal += source.at(startOffset);
        startOffset++;
    }

    return (float)atof(currentStrVal.c_str());
}

int getIntFromStr(const std::string& source, size_t startOffset){
    if(!isdigit(source.at(startOffset)) && source.at(startOffset) != '-'){ // minus symbol is permitted 
        fprintf(stderr, "Invalid offset provided obtaining float value. Location: %d", startOffset);
		return INT_MAX;
        // return FLT_MAX;
    }

    std::string currentStrVal;
    while(isdigit(source.at(startOffset)) || source.at(startOffset) == '-'){ // minus and permitted
        currentStrVal += source.at(startOffset);
        startOffset++;
    }

    return (int)atoi(currentStrVal.c_str());
}