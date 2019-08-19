#include "SlitScanProcessor.h"

int main(int argc, char** argv) 
{   
    std::string path, direction, nSlices;
    printf("\nPath to file: ");
    getline(std::cin, path);
    printf("\nScan direction (0-3): ");
    getline(std::cin, direction);
    printf("\nSlice number (More is smoother): ");
    getline(std::cin, nSlices);
    int direction_option = stoi(direction);
    int nSlices_option = stoi(nSlices);
    SlitScanProcessor Processor(direction_option, nSlices_option);
    std::vector<char> path_option(path.c_str(), path.c_str() + path.size() + 1);
    int success = Processor.processFile(path_option.data());
    return 0; 
}