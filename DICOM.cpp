#include "DICOM.h"

DICOM::DICOM()
{
}
DICOM::~DICOM()
{
}

void DICOM::set_file_name(std::string file_name)
{
    this->file_name = file_name;
}

bool DICOM::is_dicom()
{
    std::ifstream file(this->file_name, std::ios::binary);

    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << this->file_name << std::endl;
        return false;
    }
    else
    {
        // file.seekg(0, std::ios::end);
        std::vector<uint8_t> buffer(132);
        file.read(reinterpret_cast<char *>(buffer.data()), 132);
        file.close();
        // 128, 129, 130, 131

        buffer;
        return true;
    }
}

void DICOM::read_header()
{
}