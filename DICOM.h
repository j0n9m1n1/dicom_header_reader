#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <cstdint>
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
    std::vector<uint8_t> buffer;
    std::map<std::pair<uint16_t, uint16_t>, size_t> tag_positions;

    std::vector<uint8_t> load_file(std::string file_path);

    void DICOM::read_tag(std::vector<uint8_t> &buffer, size_t offset, uint16_t group, uint16_t element);
    uint16_t DICOM::read_group(std::vector<uint8_t> &buffer, size_t offset);
    uint16_t DICOM::read_element(std::vector<uint8_t> &buffer, size_t offset);
    std::string DICOM::read_vr(std::vector<uint8_t> &buffer, size_t offset);
    uint32_t DICOM::read_length(std::vector<uint8_t> &buffer, size_t offset, std::string &vr);

    bool is_dicom(std::string file_name);
    bool is_exist_tag(std::string file_name, uint16_t group, uint16_t element);

    void read_header(std::string file_name);
};