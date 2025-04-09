#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include "DICOM.h"

std::string find_dicom_tag(std::vector<uint8_t> &buffer, uint16_t group, uint16_t element, size_t &position)
{
    // DICOM 태그를 찾는 함수
    for (size_t i = 0; i < buffer.size() - 4; i++)
    {
        if (buffer[i] == (group >> 8) && buffer[i + 1] == (group & 0xFF) &&
            buffer[i + 2] == (element >> 8) && buffer[i + 3] == (element & 0xFF))
        {
            position = i;
            return std::string(reinterpret_cast<const char *>(&buffer[i]), 4);
        }
    }
    return "";
}
std::vector<uint8_t> load_file()
{
    std::string filename = "D:\\1.3.46.670589.33.1.63766538606917383700001.5372385953498057149.dcm";
    std::ifstream file(filename, std::ios::binary);

    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << filename << std::endl;
    }

    // seek get pointer, tell get pointer
    file.seekg(0, std::ios::end);    // 커서 끝으로 set
    size_t file_size = file.tellg(); // 커서 포지션?
    file.seekg(0, std::ios::beg);    // 커서 처음으로 set

    // unsigned int 8bit
    std::vector<uint8_t> buffer(file_size);
    file.read(reinterpret_cast<char *>(buffer.data()), file_size);

    uint8_t *data = buffer.data();
    if (buffer.size() > 132)
    {
        std::cout << data[128] << std::endl;
        std::cout << data[129] << std::endl;
        std::cout << data[130] << std::endl;
        std::cout << data[131] << std::endl;
    }
    std::string sop_instance_uid = find_dicom_tag(buffer, 0x0020, 0x000d, file_size);
    std::cout << sop_instance_uid << std::endl;
    for (size_t i = 0; i < file_size - 8; i++)
    {
        if (data[i] == 0x20 && data[i + 1] == 0x00 &&
            data[i + 2] == 0x0D && data[i + 3] == 0x00)
        {
            if (data[i + 4] == 'U' && data[i + 5] == 'I')
            {
                // 길이 읽기
                uint16_t length = data[i + 6] | (data[i + 7] << 8);

                // 값이 유효한지 확인
                if (length > 0 && i + 8 + length <= file_size)
                {
                    // UID 값 추출
                    std::cout << std::string(reinterpret_cast<const char *>(&data[i + 8]), length);
                }
            }
        }
        if (data[i] == 0x20 && data[i + 1] == 0x00 &&
            data[i + 2] == 0x0D && data[i + 3] == 0x00)
        {
            std::cout << data[i] << std::endl;
        }
    }
    return buffer;
}

int main()
{
    DICOM dcm;
    std::string file_name = "D:\\48744_DX_I00010002.dcm";

    std::cout << "is DICOM: " << dcm.is_dicom(file_name) << std::endl;
    std::cout << "is exist tag: " << dcm.is_exist_tag(file_name, 0x0008, 0x0060) << std::endl;
}