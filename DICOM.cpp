#include "DICOM.h"

// struct TagInfo
// {
//     uint16_t group;
//     uint16_t element;
//     std::string vr;
//     uint32_t length;
//     size_t value_offset;
//     const uint8_t *data_ptr;
// };

DICOM::DICOM()
{
    tag_dictionary["PatientName"] = {0x0010, 0x0010, "PatientName", "PN"};

    for (const auto &entry : tag_dictionary)
    {
        tag_lookup[{entry.second.group, entry.second.element}] = entry.first;
    }
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
buffer[offset] = 0x08 (하위 바이트)
buffer[offset + 1] = 0x00 (상위 바이트)
(buffer[offset + 1] << 8) = 0x0000 (0x00을 왼쪽으로 8비트 시프트)
buffer[offset] | (buffer[offset + 1] << 8) = 0x0008
*/
uint16_t DICOM::read_group(std::vector<uint8_t> &buffer, size_t offset)
{
    int byte_order = 0; // 0: little endian, 1: big endian
    if (byte_order == 0)
    {
        return buffer[offset] | (buffer[offset + 1] << 8);
    }
    else if (byte_order == 1)
    {
        return (buffer[offset] << 8) | buffer[offset + 1];
    }
    return 0;
}
uint16_t DICOM::read_element(std::vector<uint8_t> &buffer, size_t offset)
{
    int byte_order = 0; // 0: little endian, 1: big endian
    if (byte_order == 0)
    {
        return buffer[offset + 2] | (buffer[offset + 3] << 8);
    }
    else if (byte_order == 1)
    {
        return (buffer[offset + 2] << 8) | buffer[offset + 3];
    }
    return 0;
}

std::string DICOM::read_vr(std::vector<uint8_t> &buffer, size_t offset)
{
    std::string vr(reinterpret_cast<char *>(&buffer[offset + 4]), 2);
    return vr;
}
/*
2바이트 Length 필드를 사용하는 VR:
AE, AS, AT, CS, DA, DS, DT, FL, FD, IS, LO, LT, PN, SH, SL, SS, ST, TM, UI, UL, US

4바이트 Length 필드를 사용하는 VR:
OB, OW, OF, SQ, UT, UN
*/
uint32_t DICOM::read_length(std::vector<uint8_t> &buffer, size_t offset, std::string &vr)
{
    int byte_order = 0; // 0: little endian, 1: big endian

    // 6, 7은 reserved
    if (vr == "OB" || vr == "OW" || vr == "OF" || vr == "SQ" || vr == "UT" || vr == "UN")
    {
        if (byte_order == 0) // 리틀 엔디안
        {
            /*
            메모리 주소:   offset+0  offset+1  offset+2  offset+3
            저장된 값:       0x78      0x56      0x34      0x12
            buffer[offset]        = 0x78             = 0x00000078
            (buffer[offset+1]<<8) = 0x56 << 8        = 0x00005600
            (buffer[offset+2]<<16)= 0x34 << 16       = 0x00340000
            (buffer[offset+3]<<24)= 0x12 << 24       = 0x12000000
            OR 연산 결과                             = 0x12345678
            */
            return buffer[offset + 8] |
                   (buffer[offset + 9] << 8) |
                   (buffer[offset + 10] << 16) |
                   (buffer[offset + 11] << 24);
        }
        else // 빅 엔디안
        {
            return (buffer[offset + 8] << 24) |
                   (buffer[offset + 9] << 16) |
                   (buffer[offset + 10] << 8) |
                   buffer[offset + 11];
        }
    }
    else if (vr == "AE" || vr == "AS" || vr == "AT" || vr == "CS" || vr == "DA" ||
             vr == "DS" || vr == "DT" || vr == "FL" || vr == "FD" || vr == "IS" ||
             vr == "LO" || vr == "LT" || vr == "PN" || vr == "SH" || vr == "SL" ||
             vr == "SS" || vr == "ST" || vr == "TM" || vr == "UI" || vr == "UL" ||
             vr == "US")
    {
        if (byte_order == 0) // 리틀 엔디안
        {
            return buffer[offset + 6] |
                   (buffer[offset + 7] << 8);
        }
        else // 빅 엔디안
        {
            return (buffer[offset + 6] << 8) |
                   buffer[offset + 7];
        }
    }
    return 0;
}

size_t DICOM::get_value_offset(size_t tag_offset, const std::string &vr)
{
    if (vr == "OB" || vr == "OW" || vr == "SQ" || vr == "UN" || vr == "UT" || vr == "OF")
    {
        return tag_offset + 12; // 그룹(2) + 엘리먼트(2) + VR(2) + 예약(2) + 길이(4)
    }
    // 2바이트 길이 필드를 가진 VR (다른 모든 VR)
    else
    {
        return tag_offset + 8; // 그룹(2) + 엘리먼트(2) + VR(2) + 길이(2)
    }
}

DICOMValue DICOM::get_value(const TagInfo &taginfo)
{
    if (taginfo.vr == "US")
    {
        uint16_t value = taginfo.data_ptr[0] | (taginfo.data_ptr[1] << 8);
        return value;
    }
    else if (taginfo.vr == "SS")
    {
        uint16_t value = static_cast<int16_t>(taginfo.data_ptr[0] | (taginfo.data_ptr[1] << 8));
        return value;
    }
    else if (taginfo.vr == "UL")
    {
        uint32_t value = taginfo.data_ptr[0] |
                         (taginfo.data_ptr[1] << 8) |
                         (taginfo.data_ptr[2] << 16) |
                         (taginfo.data_ptr[3] << 24);
        return value;
    }
    else if (taginfo.vr == "SL")
    {
        int32_t value = static_cast<int32_t>(taginfo.data_ptr[0] |
                                             (taginfo.data_ptr[1] << 8) |
                                             (taginfo.data_ptr[2] << 16) |
                                             (taginfo.data_ptr[3] << 24));
        return value;
    }
    else if (taginfo.vr == "FL")
    {
        float value;
        std::memcpy(&value, taginfo.data_ptr, sizeof(float));
        return value;
    }
    else if (taginfo.vr == "FD")
    {
        double value;
        std::memcpy(&value, taginfo.data_ptr, sizeof(double));
        return value;
    }
    else if (taginfo.vr == "ST" || taginfo.vr == "LT" || taginfo.vr == "UT" ||
             taginfo.vr == "PN" || taginfo.vr == "LO" || taginfo.vr == "CS" ||
             taginfo.vr == "UI" || taginfo.vr == "DA" || taginfo.vr == "TM")
    {
        return std::string(reinterpret_cast<const char *>(taginfo.data_ptr), taginfo.length);
    }
    else if (taginfo.vr == "AT")
    {
        uint16_t group = taginfo.data_ptr[0] | (taginfo.data_ptr[1] << 8);
        uint16_t element = taginfo.data_ptr[2] | (taginfo.data_ptr[3] << 8);
        // 이거 return을 뭘로 할 지 생각좀 해봐야 할 듯
        return std::string(std::to_string(group) + ", " + std::to_string(element));
    }
    else if (taginfo.vr == "OB" || taginfo.vr == "OW" || taginfo.vr == "OF" || taginfo.vr == "UN")
    {
        // 바이너리 데이터는 길이 정보와 함께 저장
        std::vector<uint8_t> value(taginfo.data_ptr, taginfo.data_ptr + taginfo.length);
        return value;
    }
    return DICOMValue{}; // 기본값 (에러 처리 필요 시 예외 던지기 가능)
}

TagInfo DICOM::read_tag(std::vector<uint8_t> &buffer, size_t offset, uint16_t group, uint16_t element)
{
    TagInfo tag;

    tag.group = read_group(buffer, offset);
    tag.element = read_element(buffer, offset);
    tag.vr = read_vr(buffer, offset);
    tag.length = read_length(buffer, offset, tag.vr);
    tag.value_offset = get_value_offset(offset, tag.vr);
    tag.data_ptr = &buffer[tag.value_offset];

    return tag;
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
        int offset = 0;

        while (offset < buffer.size())
        {
            TagInfo tag = read_tag(buffer, offset, 0x0002, 0x0010);

            if (tag.group == group && tag.element == element)
            {
                std::cout << "Transfer Syntax UID" << std::endl;
                DICOMValue value = get_value(tag);
                return true;
            }
            offset += tag.value_offset - offset + tag.length;

            if (offset >= buffer.size())
                break;
        }
        // for (int i = 0; i < buffer.size(); i++)
        // {

        // get_value(tag);

        // if (buffer[i] == 0x02 && buffer[i + 1] == 0x00 &&
        //     buffer[i + 2] == 0x10 && buffer[i + 3] == 0x00)
        // {
        //     std::cout << "Transfer Syntax UID" << std::endl;
        // }
        // }
        // preamble을 찾았으면 132부터~-4
        // for (int i = 0; i < buffer.size(); i++)
        // {
        //     // 0x1234 >> 8 = 0x0012, 0x1234 & 0xFF = 0x0034
        //     if (buffer[i] == (group >> 8) && buffer[i + 1] == (group & 0xFF) &&
        //         buffer[i + 2] == (element >> 8) && buffer[i + 3] == (element & 0xFF))
        //     {
        //         return true;
        //     }
        // }
    }
    return false;
}
void DICOM::read_header(std::string file_path)
{
}
