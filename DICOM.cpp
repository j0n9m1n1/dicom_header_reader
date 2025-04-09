#include "DICOM.h"

DICOM::DICOM()
{
}
DICOM::~DICOM()
{
}

std::vector<uint8_t> DICOM::load_file(std::string file_path)
{
    std::ifstream file(file_path, std::ios::binary);

    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << file_path << std::endl;
        return std::vector<uint8_t>();
    }
    else
    {
        file.seekg(0, std::ios::end);
        size_t file_size = file.tellg();
        file.seekg(0, std::ios::beg); // 이걸 안해주면 read에서 읽지를 못 함
        std::vector<uint8_t> buffer(file_size);
        file.read(reinterpret_cast<char *>(buffer.data()), file_size);
        file.close();
        return buffer;
    }
}

bool DICOM::is_dicom(std::string file_path)
{
    std::vector<uint8_t> buffer = load_file(file_path);
    if (buffer.size() == 0)
    {
        std::cerr << "Error loading file: " << file_path << std::endl;
        return false;
    }
    else
    {
        if (buffer[128] == 0x44 && buffer[129] == 0x49 &&
            buffer[130] == 0x43 && buffer[131] == 0x4D)
        {
            preamble = true;
            return true;
        }
        else
        {
            preamble = false;
            return false;
        }
    }
}

/*
2바이트 Length 필드를 사용하는 VR:
AE, AS, AT, CS, DA, DS, DT, FL, FD, IS, LO, LT, PN, SH, SL, SS, ST, TM, UI, UL, US

4바이트 Length 필드를 사용하는 VR:
OB, OW, OF, SQ, UT, UN
*/
uint16_t DICOM::read_group(std::vector<uint8_t> &buffer, size_t offset)
{
}
uint16_t DICOM::read_element(std::vector<uint8_t> &buffer, size_t offset)
{
}
std::string DICOM::read_vr(std::vector<uint8_t> &buffer, size_t offset)
{
}
uint32_t DICOM::read_length(std::vector<uint8_t> &buffer, size_t offset, std::string &vr)
{
    if (vr == "OB" || vr == "OW" || vr == "OF" || vr == "SQ" || vr == "UT" || vr == "UN")
    {
        uint32_t length = 0;
        buffer[offset + 3] buffer[offset + 4] buffer[offset + 5] buffer[offset + 6]
    }
}

void DICOM::read_tag(std::vector<uint8_t> &buffer, size_t offset, uint16_t group, uint16_t element)
{
    read_group(buffer, offset);
    read_element(buffer, offset);
    read_vr(buffer, offset);
    read_length(buffer, offset, vr);
}

bool DICOM::is_exist_tag(std::string file_path, uint16_t group, uint16_t element)
{
    std::vector<uint8_t> buffer = load_file(file_path);
    if (buffer.size() == 0)
    {
        std::cerr << "Error loading file: " << file_path << std::endl;
        return false;
    }
    else
    {
        // 0x0002, 0x0010 Trasfer Syntax UID
        for (int i = 0; i < buffer.size(); i++)
        {
            read_tag(buffer, i, 0x0002, 0x0010);

            if (buffer[i] == 0x02 && buffer[i + 1] == 0x00 &&
                buffer[i + 2] == 0x10 && buffer[i + 3] == 0x00)
            {
                std::cout << "Transfer Syntax UID" << std::endl;

                // char vr[3] = {0};
                // vr[0] = buffer[i + 5];
                // vr[1] = buffer[i + 4];
                std::string vr(reinterpret_cast<char *>(&buffer[i + 4]), 2);
                std::cout << "VR: " << vr << std::endl;
                // // length
                // buffer[i + 7];
                // buffer[i + 6];
                [""] std::string legnth(reinterpret_cast<char *>(&buffer[i + 4]), 2);
            }
        }
        // preamble을 찾았으면 132부터~-4
        for (int i = 0; i < buffer.size(); i++)
        {
            // 0x1234 >> 8 = 0x0012, 0x1234 & 0xFF = 0x0034
            if (buffer[i] == (group >> 8) && buffer[i + 1] == (group & 0xFF) &&
                buffer[i + 2] == (element >> 8) && buffer[i + 3] == (element & 0xFF))
            {
                return true;
            }
        }
    }
    return false;
}
void DICOM::read_header(std::string file_path)
{
}
