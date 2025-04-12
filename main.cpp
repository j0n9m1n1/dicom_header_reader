#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include "DICOM.h"

int main()
{
    DICOM dcm;
    // std::string file_name = "D:\\48744_DX_I00010002.dcm";
    std::string file_name = "G:\\1.2.410.200067.100.3.20220811.1127130623.32304.1.1.dcm";
    // std::cout << "is DICOM: " << dcm.is_dicom(file_name) << std::endl;
    std::cout << "is exist tag: " << dcm.find_tag(file_name, 0x0008, 0x0060) << std::endl;
    std::cout << "value: " << dcm.get_value(file_name, 0x0008, 0x0060).to_string() << std::endl;
    std::cout << "is exist tag: " << dcm.find_tag(file_name, "TransferSyntaxUID") << std::endl;
    std::cout << "value: " << dcm.get_value(file_name, "TransferSyntaxUID").to_string() << std::endl;
    dcm.print(file_name);
}