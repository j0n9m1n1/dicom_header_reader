#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include "DICOM.h"

int main()
{
    DICOM dcm;
    std::string file_name = "D:\\48744_DX_I00010002.dcm";

    std::cout << "is DICOM: " << dcm.is_dicom(file_name) << std::endl;
    std::cout << "is exist tag: " << dcm.is_exist_tag(file_name, 0x0008, 0x0060) << std::endl;
}