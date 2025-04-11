#include "DicomValue.h" // DicomValue 클래스 정의가 있는 헤더

std::string DICOMValue::to_string() const
{
    if (is_empty())
        return "Empty";
    if (is_uint16())
        return std::to_string(as_uint16());
    if (is_int16())
        return std::to_string(as_int16());
    if (is_uint32())
        return std::to_string(as_uint32());
    if (is_int32())
        return std::to_string(as_int32());
    if (is_float())
        return std::to_string(as_float());
    if (is_double())
        return std::to_string(as_double());
    if (is_string())
        return as_string();
    if (is_binary())
    {
        const auto &bin = as_binary();
        std::string result = "Binary data, length: " + std::to_string(bin.size());
        return result;
    }
    return "Unknown type";
}