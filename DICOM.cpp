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
    init();
    init2();
}
DICOM::~DICOM()
{
}

// https://www.dicomlibrary.com/dicom/dicom-tags/
void DICOM::init()
{
    tag_dictionary["FileMetaInformationGroupLength"] = {0x0002, 0x0000, "FileMetaInformationGroupLength", "UL"};
    tag_dictionary["FileMetaInformationVersion"] = {0x0002, 0x0001, "FileMetaInformationVersion", "OB"};
    tag_dictionary["MediaStorageSOPClassUID"] = {0x0002, 0x0002, "MediaStorageSOPClassUID", "UI"};
    tag_dictionary["MediaStorageSOPInstanceUID"] = {0x0002, 0x0003, "MediaStorageSOPInstanceUID", "UI"};
    tag_dictionary["TransferSyntaxUID"] = {0x0002, 0x0010, "TransferSyntaxUID", "UI"};
    tag_dictionary["ImplementationClassUID"] = {0x0002, 0x0012, "ImplementationClassUID", "UI"};
    tag_dictionary["ImplementationVersionName"] = {0x0002, 0x0013, "ImplementationVersionName", "SH"};
    tag_dictionary["SourceApplicationEntityTitle"] = {0x0002, 0x0016, "SourceApplicationEntityTitle", "AE"};
    tag_dictionary["SendingApplicationEntityTitle"] = {0x0002, 0x0017, "SendingApplicationEntityTitle", "AE"};
    tag_dictionary["ReceivingApplicationEntityTitle"] = {0x0002, 0x0018, "ReceivingApplicationEntityTitle", "AE"};
    tag_dictionary["PrivateInformationCreatorUID"] = {0x0002, 0x0100, "PrivateInformationCreatorUID", "UI"};
    tag_dictionary["PrivateInformation"] = {0x0002, 0x0102, "PrivateInformation", "OB"};

    tag_dictionary["StudyDate"] = {0x0008, 0x0020, "StudyDate", "DA"};
    tag_dictionary["StudyTime"] = {0x0008, 0x0030, "StudyTime", "TM"};
    tag_dictionary["Modality"] = {0x0008, 0x0060, "Modality", "CS"};
    tag_dictionary["SOPClassUID"] = {0x0008, 0x0016, "SOPClassUID", "UI"};
    tag_dictionary["SOPInstanceUID"] = {0x0008, 0x0018, "SOPInstanceUID", "UI"};

    tag_dictionary["PatientName"] = {0x0010, 0x0010, "PatientName", "PN"};
    tag_dictionary["PatientID"] = {0x0010, 0x0020, "PatientID", "LO"};
    tag_dictionary["PatientBirthDate"] = {0x0010, 0x0030, "PatientBirthDate", "DA"};
    tag_dictionary["PatientSex"] = {0x0010, 0x0040, "PatientSex", "CS"};
    tag_dictionary["StudyInstanceUID"] = {0x0020, 0x000D, "StudyInstanceUID", "UI"};
    tag_dictionary["SeriesInstanceUID"] = {0x0020, 0x000E, "SeriesInstanceUID", "UI"};

    tag_dictionary["Rows"] = {0x0028, 0x0010, "Rows", "US"};
    tag_dictionary["Columns"] = {0x0028, 0x0011, "Columns", "US"};
    tag_dictionary["BitsAllocated"] = {0x0028, 0x0100, "BitsAllocated", "US"};
    tag_dictionary["BitsStored"] = {0x0028, 0x0101, "BitsStored", "US"};
    tag_dictionary["HighBit"] = {0x0028, 0x0102, "HighBit", "US"};
    tag_dictionary["PixelRepresentation"] = {0x0028, 0x0103, "PixelRepresentation", "US"};

    tag_dictionary["PixelData"] = {0x7FE0, 0x0010, "PixelData", "OW"};

    for (const auto &entry : tag_dictionary)
    {
        tag_lookup[{entry.second.group, entry.second.element}] = entry.first;
    }
}

// https://www.dicomlibrary.com/dicom/transfer-syntax/
/*
    std::string name;
    bool is_explicit_vr;
    bool is_little_endian;
    bool is_compressed;
    std::string compression_type;
*/
void DICOM::init2()
{
    transfer_syntax_dictionary["1.2.840.10008.1.2"] = {"Implicit VR Endian", false, true, false, ""};
    transfer_syntax_dictionary["1.2.840.10008.1.2.1"] = {"Explicit VR Little Endian", true, true, false, ""};
    transfer_syntax_dictionary["1.2.840.10008.1.2.2"] = {"Explicit VR Big Endian", true, false, false, ""};
    transfer_syntax_dictionary["1.2.840.10008.1.2.1.99"] = {"Deflated Explicit VR Little Endian", true, true, false, ""};

    transfer_syntax_dictionary["1.2.840.10008.1.2.4.50"] = {"JPEG Baseline (Process 1)", true, true, true, "JPEG"};
    transfer_syntax_dictionary["1.2.840.10008.1.2.4.51"] = {"JPEG Baseline (Process 2 & 4))", true, true, true, "JPEG"};
    // 1.2.840.10008.1.2.4.52	JPEG Extended (Processes 3 & 5)	Retired
    // 1.2.840.10008.1.2.4.53	JPEG Spectral Selection, Nonhierarchical (Processes 6 & 8)	Retired
    // 1.2.840.10008.1.2.4.54	JPEG Spectral Selection, Nonhierarchical (Processes 7 & 9)	Retired
    // 1.2.840.10008.1.2.4.55	JPEG Full Progression, Nonhierarchical (Processes 10 & 12)	Retired
    // 1.2.840.10008.1.2.4.56	JPEG Full Progression, Nonhierarchical (Processes 11 & 13)	Retired
    transfer_syntax_dictionary["1.2.840.10008.1.2.4.57"] = {"JPEG Lossless, Nonhierarchical (Processes 14)", true, true, true, "JPEG Loseless"};
    // 1.2.840.10008.1.2.4.58	JPEG Lossless, Nonhierarchical (Processes 15)	Retired
    // 1.2.840.10008.1.2.4.59	JPEG Extended, Hierarchical (Processes 16 & 18)	Retired
    // 1.2.840.10008.1.2.4.60	JPEG Extended, Hierarchical (Processes 17 & 19)	Retired
    // 1.2.840.10008.1.2.4.61	JPEG Spectral Selection, Hierarchical (Processes 20 & 22)	Retired
    // 1.2.840.10008.1.2.4.62	JPEG Spectral Selection, Hierarchical (Processes 21 & 23)	Retired
    // 1.2.840.10008.1.2.4.63	JPEG Full Progression, Hierarchical (Processes 24 & 26)	Retired
    // 1.2.840.10008.1.2.4.64	JPEG Full Progression, Hierarchical (Processes 25 & 27)	Retired
    // 1.2.840.10008.1.2.4.65	JPEG Lossless, Nonhierarchical (Process 28)	Retired
    // 1.2.840.10008.1.2.4.66	JPEG Lossless, Nonhierarchical (Process 29)	Retired
    transfer_syntax_dictionary["1.2.840.10008.1.2.4.70"] = {"JPEG Lossless, Nonhierarchical, First- Order Prediction", true, true, true, "JPEG Loseless"};

    transfer_syntax_dictionary["1.2.840.10008.1.2.4.80"] = {"JPEG-LS Lossless", true, true, true, "JPEG-LS"};
    transfer_syntax_dictionary["1.2.840.10008.1.2.4.81"] = {"JPEG-LS Lossy", true, true, true, "JPEG-LS"};

    transfer_syntax_dictionary["1.2.840.10008.1.2.4.90"] = {"JPEG 2000 (Lossless Only)", true, true, true, "JPEG 2000"};
    transfer_syntax_dictionary["1.2.840.10008.1.2.4.91"] = {"JPEG 2000", true, true, true, "JPEG 2000"};

    transfer_syntax_dictionary["1.2.840.10008.1.2.5"] = {"RLE Lossless", true, true, true, "RLE"};
}
std::string DICOM::get_tag_name(uint16_t group, uint16_t element)
{
    std::string tag_name = "Unknown";
    auto it = tag_lookup.find({group, element});
    if (it != tag_lookup.end())
    {
        tag_name = it->second;
    }
    return tag_name;
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

bool DICOM::find_tag(std::string file_path, uint16_t group, uint16_t element)
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
        int offset = is_dicom(file_path) ? 132 : 0;

        while (offset < buffer.size())
        {
            TagInfo tag = read_tag(buffer, offset, 0x0002, 0x0010);

            if (tag.group == group && tag.element == element)
            {

                // std::cout << "Transfer Syntax UID" << std::endl;
                // DICOMValue value = get_value(tag);
                // std::cout << "Value: " << value.to_string() << std::endl;
                return true;
            }
            offset += tag.value_offset - offset + tag.length;

            if (offset >= buffer.size())
                break;
        }
    }
    return false;
}

bool DICOM::find_tag(std::string file_path, std::string tag_name)
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
        int offset = is_dicom(file_path) ? 132 : 0;

        uint16_t group = tag_dictionary[tag_name].group;
        uint16_t element = tag_dictionary[tag_name].element;

        if (group == 0x0000 && element == 0x0000)
        {
            std::cerr << "Error: Invalid tag name" << std::endl;
            return false;
        }
        while (offset < buffer.size())
        {
            TagInfo tag = read_tag(buffer, offset, 0x0002, 0x0010);

            if (tag.group == group && tag.element == element)
            {

                // std::cout << "Transfer Syntax UID" << std::endl;
                // DICOMValue value = get_value(tag);
                // std::cout << "Value: " << value.to_string() << std::endl;
                return true;
            }
            offset += tag.value_offset - offset + tag.length;

            if (offset >= buffer.size())
                break;
        }
    }
    return false;
}

DICOMValue DICOM::get_value(TagInfo &tag)
{
    if (tag.vr == "US")
    {
        uint16_t value = tag.data_ptr[0] | (tag.data_ptr[1] << 8);
        return value;
    }
    else if (tag.vr == "SS")
    {
        uint16_t value = static_cast<int16_t>(tag.data_ptr[0] | (tag.data_ptr[1] << 8));
        return value;
    }
    else if (tag.vr == "UL")
    {
        uint32_t value = tag.data_ptr[0] |
                         (tag.data_ptr[1] << 8) |
                         (tag.data_ptr[2] << 16) |
                         (tag.data_ptr[3] << 24);
        return value;
    }
    else if (tag.vr == "SL")
    {
        int32_t value = static_cast<int32_t>(tag.data_ptr[0] |
                                             (tag.data_ptr[1] << 8) |
                                             (tag.data_ptr[2] << 16) |
                                             (tag.data_ptr[3] << 24));
        return value;
    }
    else if (tag.vr == "FL")
    {
        float value;
        std::memcpy(&value, tag.data_ptr, sizeof(float));
        return value;
    }
    else if (tag.vr == "FD")
    {
        double value;
        std::memcpy(&value, tag.data_ptr, sizeof(double));
        return value;
    }
    else if (tag.vr == "ST" || tag.vr == "LT" || tag.vr == "UT" ||
             tag.vr == "PN" || tag.vr == "LO" || tag.vr == "CS" ||
             tag.vr == "UI" || tag.vr == "DA" || tag.vr == "TM")
    {
        return std::string(reinterpret_cast<const char *>(tag.data_ptr), tag.length);
    }
    else if (tag.vr == "AT")
    {
        uint16_t group = tag.data_ptr[0] | (tag.data_ptr[1] << 8);
        uint16_t element = tag.data_ptr[2] | (tag.data_ptr[3] << 8);
        // 이거 return을 뭘로 할 지 생각좀 해봐야 할 듯
        return std::string(std::to_string(group) + ", " + std::to_string(element));
    }
    else if (tag.vr == "OB" || tag.vr == "OW" || tag.vr == "OF" || tag.vr == "UN")
    {
        // 바이너리 데이터는 길이 정보와 함께 저장
        std::vector<uint8_t> value(tag.data_ptr, tag.data_ptr + tag.length);
        return value;
    }
    return DICOMValue{}; // 기본값 (에러 처리 필요 시 예외 던지기 가능)
}

DICOMValue DICOM::get_value(std::string &file_path, uint16_t group, uint16_t element)
{
    std::vector<uint8_t> buffer = load_file(file_path);
    if (buffer.size() == 0)
    {
        std::cerr << "Error loading file: " << file_path << std::endl;
        return DICOMValue{}; // 에러 처리 필요
    }
    else
    {
        int offset = is_dicom(file_path) ? 132 : 0;
        while (offset < buffer.size())
        {
            TagInfo tag = read_tag(buffer, offset, group, element);
            if (tag.group == group && tag.element == element)
            {
                return get_value(tag);
            }

            offset += tag.value_offset - offset + tag.length;
            if (offset >= buffer.size())
                break;
        }
        return DICOMValue{}; // 에러 처리 필요
    }
}

DICOMValue DICOM::get_value(std::string &file_path, std::string tag_name)
{
    std::vector<uint8_t> buffer = load_file(file_path);
    if (buffer.size() == 0)
    {
        std::cerr << "Error loading file: " << file_path << std::endl;
        return DICOMValue{}; // 에러 처리 필요
    }
    else
    {
        int offset = is_dicom(file_path) ? 132 : 0;

        uint16_t group = tag_dictionary[tag_name].group;
        uint16_t element = tag_dictionary[tag_name].element;
        if (group == 0x0000 && element == 0x0000)
        {
            std::cerr << "Error: Invalid tag name" << std::endl;
            return false;
        }
        while (offset < buffer.size())
        {
            TagInfo tag = read_tag(buffer, offset, group, element);
            if (tag.group == group && tag.element == element)
            {
                return get_value(tag);
            }

            offset += tag.value_offset - offset + tag.length;
            if (offset >= buffer.size())
                break;
        }
        return DICOMValue{}; // 에러 처리 필요
    }
}

void DICOM::print(std::string file_path)
{
    std::vector<uint8_t> buffer = load_file(file_path);
    if (buffer.size() == 0)
    {
        std::cerr << "Error loading file: " << file_path << std::endl;
    }
    else
    {
        int offset = is_dicom(file_path) ? 132 : 0;
        while (offset < buffer.size())
        {
            TagInfo tag = read_tag(buffer, offset, 0x0000, 0x0000);
            std::cout << "(0x" << std::hex << std::setw(4) << std::setfill('0') << tag.group
                      << ", 0x" << std::setw(4) << std::setfill('0') << tag.element << ") "
                      << tag.vr << " "
                      << get_tag_name(tag.group, tag.element) << " "
                      << ": " << get_value(tag).to_string() << std::endl;

            offset += tag.value_offset - offset + tag.length;
            if (offset >= buffer.size())
                break;
        }
    }
    std::cout << std::dec;
}
void DICOM::read_header(std::string file_path)
{
}
