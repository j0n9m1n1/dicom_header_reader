#include <iostream>
#include <vector>
#include <fstream>

class DICOM
{
public:
    DICOM();
    ~DICOM();

    std::string file_name = "";
    std::vector<uint8_t> buffer;

    void set_file_name(std::string file_name);
    std::vector<uint8_t> load_file();
    bool is_dicom();
};