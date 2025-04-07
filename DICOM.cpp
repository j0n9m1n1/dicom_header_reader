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
    return false;
}