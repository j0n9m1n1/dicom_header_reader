#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <cstdint>
#include <variant>
using DicomValue = std::variant<
    uint16_t,   // US
    int16_t,    // SS
    uint32_t,   // UL
    int32_t,    // SL
    float,      // FL
    double,     // FD
    std::string // ST, LT, UT 등 문자열 타입
    >;
struct DicomTagDef
{
    uint16_t group;
    uint16_t element;
    std::string name;
    std::string vr;
};
struct TagInfo
{
    uint16_t group;
    uint16_t element;
    std::string vr;
    uint32_t length;
    size_t value_offset;
    const uint8_t *data_ptr;
};

class DICOM
{

public:
    DICOM();
    ~DICOM();

    enum ByteOrder
    {
        LITTLE_ENDIAN,
        BIG_ENDIAN
    };

    std::map<std::string, DicomTagDef> tag_dictionary;
    std::map<std::pair<uint16_t, uint16_t>, std::string> tag_lookup;

    bool preamble = false;

    std::vector<uint8_t> buffer;
    std::map<std::pair<uint16_t, uint16_t>, TagInfo> tag_map;

    std::vector<uint8_t> load_file(std::string file_path);

    uint16_t read_group(std::vector<uint8_t> &buffer, size_t offset);
    uint16_t read_element(std::vector<uint8_t> &buffer, size_t offset);
    std::string read_vr(std::vector<uint8_t> &buffer, size_t offset);
    uint32_t read_length(std::vector<uint8_t> &buffer, size_t offset, std::string &vr);
    size_t get_value_offset(size_t tag_offset, const std::string &vr);
    TagInfo read_tag(std::vector<uint8_t> &buffer, size_t offset, uint16_t group, uint16_t element);
    DicomValue get_value(const TagInfo &taginfo);
    bool is_dicom(std::string file_name);
    bool is_exist_tag(std::string file_name, uint16_t group, uint16_t element);

    void read_header(std::string file_name);
};