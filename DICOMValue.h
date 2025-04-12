#ifndef DICOM_VALUE_H
#define DICOM_VALUE_H

#include <variant>
#include <string>
#include <vector>
#include <cstring> // memcpy 사용을 위한 헤더
#include <cstdint>
// DICOM 값을 저장하기 위한 간단한 클래스
class DICOMValue
{
private:
    // 여러 타입의 값을 저장할 수 있는 variant
    std::variant<
        std::monostate,      // 빈 값/에러 상태
        uint16_t,            // US
        int16_t,             // SS
        uint32_t,            // UL
        int32_t,             // SL
        float,               // FL
        double,              // FD
        std::string,         // 문자열 값들
        std::vector<uint8_t> // 바이너리 데이터
        >
        m_data; // data는 표준 라이브러리 함수와 충돌할 수 있으므로 m_data로 변경

public:
    // 기본 생성자
    DICOMValue() : m_data(std::monostate{}) {}

    // 다양한 타입에 대한 생성자들
    DICOMValue(uint16_t val) : m_data(val) {}
    DICOMValue(int16_t val) : m_data(val) {}
    DICOMValue(uint32_t val) : m_data(val) {}
    DICOMValue(int32_t val) : m_data(val) {}
    DICOMValue(float val) : m_data(val) {}
    DICOMValue(double val) : m_data(val) {}
    DICOMValue(const std::string &val) : m_data(val) {}
    DICOMValue(const std::vector<uint8_t> &val) : m_data(val) {}

    // 값 확인 메서드들
    bool is_empty() const { return std::holds_alternative<std::monostate>(m_data); }
    bool is_uint16() const { return std::holds_alternative<uint16_t>(m_data); }
    bool is_int16() const { return std::holds_alternative<int16_t>(m_data); }
    bool is_uint32() const { return std::holds_alternative<uint32_t>(m_data); }
    bool is_int32() const { return std::holds_alternative<int32_t>(m_data); }
    bool is_float() const { return std::holds_alternative<float>(m_data); }
    bool is_double() const { return std::holds_alternative<double>(m_data); }
    bool is_string() const { return std::holds_alternative<std::string>(m_data); }
    bool is_binary() const { return std::holds_alternative<std::vector<uint8_t>>(m_data); }

    // 값 접근 메서드들
    uint16_t as_uint16() const { return std::get<uint16_t>(m_data); }
    int16_t as_int16() const { return std::get<int16_t>(m_data); }
    uint32_t as_uint32() const { return std::get<uint32_t>(m_data); }
    int32_t as_int32() const { return std::get<int32_t>(m_data); }
    float as_float() const { return std::get<float>(m_data); }
    double as_double() const { return std::get<double>(m_data); }
    const std::string &as_string() const { return std::get<std::string>(m_data); }
    const std::vector<uint8_t> &as_binary() const { return std::get<std::vector<uint8_t>>(m_data); }

    // 문자열 표현 반환 (옵션)
    std::string to_string() const;
};

#endif // DICOM_VALUE_H