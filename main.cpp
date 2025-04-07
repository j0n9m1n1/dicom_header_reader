#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

std::vector<uint8_t> load_file()
{
    std::string filename = "D:\\1.3.46.670589.33.1.63764894582333658100001.4761269290869272443.dcm";
    std::ifstream file(filename, std::ios::binary);

    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << filename << std::endl;
    }

    // seek get pointer, tell get pointer
    file.seekg(0, std::ios::end);            // 커서 끝으로 set
    std::streampos file_size = file.tellg(); // 커서 포지션?
    file.seekg(0, std::ios::beg);            // 커서 처음으로 set

    // unsigned int 8bit
    std::vector<uint8_t> buffer(file_size);
    file.read(reinterpret_cast<char *>(buffer.data()), file_size);

    uint8_t *data = buffer.data();
    if (buffer.size() > 132)
    {
        std::cout << data[128] << std::endl;
        std::cout << data[129] << std::endl;
        std::cout << data[130] << std::endl;
        std::cout << data[131] << std::endl;
    }

    return buffer;
}
int main()
{
    std::vector<uint8_t> buffer = load_file();
}