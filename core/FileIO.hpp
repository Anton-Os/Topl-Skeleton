#include <ios>
#include <sstream>
#include <fstream>
#include <string>

#include <cstring>
#include <cstdio>
#include <cctype>
// #include <vector>
#include <climits> // For error handling
#include <cfloat> // For error handling
#include <cerrno> // For error handling

// ---------------------- Anonymous Functions ---------------------- //

std::string readFile(const char* source, bool isBinaryFile);
std::string getParentDir(const char* arg);
bool checkFormatObj(const char* source);
// bool checkFileExtension(const char* source, const char* ext);
void cleanupNumStr(std::string* str); // cleanup string so only numbers and whitespace are left
// void trim(std::string &str);
unsigned getValsCountFromStr(const std::string& source); // extracts number of consecutive values in a string without divisor
unsigned getValsCountFromStr(const std::string& source, unsigned short div); // extracts number of consecutive values in a string divided by div
float getFloatFromStr(const std::string& source, size_t startOffset); // extracts float from str relative to start offset
int getIntFromStr(const std::string& source, size_t startOffset); // extracts int from str relative to start offset

// Create a log file for debugging purposes
void logToFile(const char* fileName, std::string logMessage);