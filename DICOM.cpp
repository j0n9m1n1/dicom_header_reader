#include "DICOM.h"

DICOM::DICOM()
{
}
DICOM::~DICOM()
{
}

bool DICOM::is_dicom(std::string file_name)
{
    std::ifstream file(file_name, std::ios::binary);

    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << file_name << std::endl;
        return false;
    }
    else
    {
        // file.seekg(0, std::ios::end);
        std::vector<uint8_t> buffer(132);
        file.read(reinterpret_cast<char *>(buffer.data()), 132);
        file.close();
        // 128, 129, 130, 131

        if (buffer[128] == 0x44 && buffer[129] == 0x49 &&
            buffer[130] == 0x43 && buffer[131] == 0x4D)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

void DICOM::read_header(std::string file_name)
{
}