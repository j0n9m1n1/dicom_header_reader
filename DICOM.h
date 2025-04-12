#include "DICOMValue.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <cstdint>
#include <variant>
#include <string>
#include <cstring>
#include <variant>
#include <string>
#include <vector>
#include <cstring> // memcpy 사용을 위한 헤더
#include <iomanip>

struct DicomTagDict
{
    uint16_t group;
    uint16_t element;
    std::string name;
    std::string vr;
};

struct TransferSyntaxDict
{
    std::string name;
    bool is_explicit_vr;
    bool is_little_endian;
    bool is_compressed;
    std::string compression_type;
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

    bool preamble = false;

    std::map<std::string, DicomTagDict> tag_dictionary;
    std::map<std::pair<uint16_t, uint16_t>, std::string> tag_lookup;

    std::map<std::string, TransferSyntaxDict> transfer_syntax_dictionary;

    std::vector<uint8_t> buffer;
    std::map<std::pair<uint16_t, uint16_t>, TagInfo> tag_map;

    std::vector<uint8_t> load_file(std::string file_path);

    bool is_dicom(std::string file_name);
    bool find_tag(std::string file_name, uint16_t group, uint16_t element);
    bool find_tag(std::string file_path, std::string tag_name);
    DICOMValue get_value(TagInfo &tag);
    DICOMValue get_value(std::string &file_path, uint16_t group, uint16_t element);
    DICOMValue get_value(std::string &file_path, std::string tag_name);
    void read_header(std::string file_name);
    std::string get_tag_name(uint16_t group, uint16_t element);
    void print(std::string file_path);

private:
    void init();
    void init2();
    TagInfo read_tag(std::vector<uint8_t> &buffer, size_t offset, uint16_t group, uint16_t element);
    uint16_t read_group(std::vector<uint8_t> &buffer, size_t offset);
    uint16_t read_element(std::vector<uint8_t> &buffer, size_t offset);
    std::string read_vr(std::vector<uint8_t> &buffer, size_t offset);
    uint32_t read_length(std::vector<uint8_t> &buffer, size_t offset, std::string &vr);
    size_t get_value_offset(size_t tag_offset, const std::string &vr);
};