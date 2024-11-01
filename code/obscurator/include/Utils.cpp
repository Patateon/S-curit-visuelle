#pragma once

#define TERMINAL_UNDERLINE "\033[4m"
#define TERMINAL_ERROR     "\e[1;31m"
#define TERMINAL_INFO      "\033[94m"
#define TERMINAL_OK        "\033[92m"
#define TERMINAL_RESET     "\033[0m"
#define TERMINAL_TIMER     "\033[93m"
#define TERMINAL_FILENAME  "\033[95m"
#define TERMINAL_WARNING   "\e[38;5;208m"
#define TERMINAL_NOTIF     "\e[1;36m"

#define FILE_ERROR_MESSAGE(filename, message) std::cerr \
    << TERMINAL_ERROR << "Error \xBF loading file " \
    << TERMINAL_TIMER << filename \
    << TERMINAL_ERROR << " from "\
    << TERMINAL_UNDERLINE << __PRETTY_FUNCTION__ \
    << TERMINAL_RESET << TERMINAL_ERROR << " at file " << __FILE__ << ":" << __LINE__ \
    << "\n      \xC0\xC4\xC4 "\
    << TERMINAL_WARNING << message \
    << TERMINAL_RESET << "\n\n";

#define WARNING_MESSAGE(message) std::cerr \
    << TERMINAL_WARNING << "WARNING \xBF from " \
    << TERMINAL_UNDERLINE << __PRETTY_FUNCTION__ \
    << TERMINAL_RESET << TERMINAL_WARNING << " at file " << __FILE__ << ":" << __LINE__ \
    << "\n        \xC0\xC4\xC4 "\
    << TERMINAL_WARNING << message \
    << TERMINAL_RESET << "\n\n";

#define ERROR_MESSAGE(message) std::cerr \
    << TERMINAL_ERROR << "ERROR : from " \
    << TERMINAL_UNDERLINE << __PRETTY_FUNCTION__ \
    << TERMINAL_RESET << TERMINAL_ERROR << " at file " << __FILE__ << ":" << __LINE__ \
    << "\n        ==> "\
    << TERMINAL_ERROR << message \
    << TERMINAL_RESET << "\n\n";

#define ARGCCHECK(nb, usage) \
    if(argc != nb+1){ERROR_MESSAGE("Usage [" << argv[0] << "]" << usage); exit(1);} 

#include <string>

std::string getFileExtension(const std::string &fileName);
const char* getFileExtensionC(const char* fileName);
std::string getFileNameFromPath(const char *path);
std::string getNameOnlyFromPath(const char *path);

