#include <iostream>
#include <vector>
#include <fstream>

class DICOM
{
public:
    DICOM();
    ~DICOM();

    std::vector<uint8_t> buffer;

    std::vector<uint8_t> load_file(std::string file_name);
    void read_header(std::string file_name);
    bool is_dicom(std::string file_name);
};