#include <string>
#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>

#ifndef FILEMANAGER_H
#define FILEMANAGER_H
class CFileManager
{
    std::string _inputDir;
    std::string _tempDir;
    std::string _outputDir;
    std::vector<std::string> filesInDirectory;
    std::vector<std::string> linesInFile;

public:
    void setInputFileDirectory(std::string inputDir);
    void setTempFileDirectory(std::string tempDir);
    void setOuputFileDirectory(std::string outputDir);

    std::string getInputFileDirectory();
    std::string getTempFileDirectory();
    std::string getOuputFileDirectory();

    bool isValidDirectory(std::string filenamePath);
    std::vector<std::string> readLinesInFile(std::string fileName);
    void readDirectory(std::string filenamePath);
    std::vector<std::string> getFilesInDirectory();
    void writeOutputFile(std::string filenamePath, std::string strline);

};
#endif
