#include <iostream>
#include "CFileManager.h"
#include "CMap.h"
#include "CReducer.h"

int main(int argc, char* argv[])
{
   /*
    1)	Your program will accept three inputs via command-line:
    a)	Directory that holds input files.
    b)	Directory to hold output files.
    c)	Temporary directory to hold intermediate output files.
   */
    if (argc != 4)
    {
        std::cout << "Usage: wordcount <DIRECTORY FOR INPUT FILE> <DIRECTORY FOR OUTPUT FILE> <DIRECTORY FOR TEMP FILE> " << std::endl;
        return 1;
    }
  
    CFileManager FileManager;

    /* verify input directory.*/
    if (FileManager.isValidDirectory(argv[1]))
        FileManager.setInputFileDirectory(argv[1]);
    else
    {
        std::cout << "Please enter a valid Input directory" << std::endl;
        return 1;
    }

    /* verify output directory.*/
    if (FileManager.isValidDirectory(argv[2]))
        FileManager.setOuputFileDirectory(argv[2]);
    else
    {
        std::cout << "Please enter a valid Output directory" << std::endl;
        return 1;
    }

    /* verify temp directory.*/
    if (FileManager.isValidDirectory(argv[3]))
        FileManager.setTempFileDirectory(argv[3]);
    else
    {
        std::cout << "Please enter a valid Temp directory" << std::endl;
        return 1;
    }

    // clean temp directory.
    std::string cleanTempDirCmd = "del " + FileManager.getTempFileDirectory() + "\\PART0000*.txt";
    std::cout << cleanTempDirCmd << std::endl;
    system(cleanTempDirCmd.c_str());

    // clean output directory.
    std::string cleanOutputDirCmd = "del " + FileManager.getOuputFileDirectory() + "\\reduce*.txt";
    std::cout << cleanOutputDirCmd << std::endl;
    system(cleanOutputDirCmd.c_str());

    cleanOutputDirCmd = "del " + FileManager.getOuputFileDirectory() + "\\SUCCESS.txt";
    std::cout << cleanOutputDirCmd << std::endl;
    system(cleanOutputDirCmd.c_str());

   /*******************************  MAP  ***********************************/
   /*
   a)	Map: Is given data from a file (does not parse the file itself)
   */
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "Mapper is now running....." << std::endl;
    std::cout << "==========================" << std::endl;
    std::cout << std::endl;

    FileManager.readDirectory(FileManager.getInputFileDirectory());
    // read files from input directory.
    std::vector<std::string> listOfFiles = FileManager.getFilesInDirectory();

    // read files in sequence and retrieve lines per file at a time to store for processing.
    std::vector<std::string> linesInFile;

    CMap mapObj;
    // seq no to append to PART0000
    int seqno = 0;
    std::string tempfilename = "PART0000";

    for (std::size_t i = 0; i < listOfFiles.size(); ++i) {
        std::string filename = listOfFiles[i];
        //std::cout << listOfFiles[i] << std::endl;
        linesInFile.clear();
        linesInFile = FileManager.readLinesInFile(listOfFiles[i]);

        // call map function on the lines read from file and tokenize. 
        for (std::size_t j = 0; j < linesInFile.size(); ++j)
            mapObj.map(filename, linesInFile[j]);

        /* Map outputs a separate temporary file that holds (word, 1) for each occurrence of every word.  */
        std::string file_no = std::to_string(++seqno);
        std::string tmpfile;
        tmpfile = tempfilename + file_no;

        // write temp output to temp directory.
        mapObj.exportData(FileManager.getTempFileDirectory(), tmpfile);
    }

   /*******************************  REDUCER  ***********************************/
    CReducer reducer;
    reducer.clear();
    // read files from temp directory.
    FileManager.readDirectory(FileManager.getTempFileDirectory());
    std::vector<std::string> listOfFilesTemp = FileManager.getFilesInDirectory();

    for (std::size_t i = 0; i < listOfFilesTemp.size(); ++i) {
        std::string filename = listOfFilesTemp[i];
        std::cout << listOfFilesTemp[i] << std::endl;
        linesInFile.clear();

        /* Lines in file (linesInFile) will now be like below after read.
        *
            ACT - 1
            BERTRAM - 1
            BERTRAM - 1
            COUNTESS - 1
            COUNTESS - 1
        */
        linesInFile = FileManager.readLinesInFile(listOfFilesTemp[i]);

        /* Lines in file (linesInFile) will now be like below after accumulate.
        *
            ACT - 1
            BERTRAM - 1 1
            COUNTESS - 1 1
        */
        for (std::size_t j = 0; j < linesInFile.size(); ++j)
            reducer.do_accumulate_job(linesInFile[j]);
    }
         /* Reducer job will accumulate the above lines as below.
         *
             ACT - 1
             BERTRAM - 2
             COUNTESS - 2
         */


        reducer.Reduce();
        // write output file after adding counts.
        reducer.exportData(FileManager.getOuputFileDirectory());
        reducer.writeEmptySuccessfile(FileManager.getOuputFileDirectory());
}

