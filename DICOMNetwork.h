#include <string>
#include <iostream>

class DICOMNetwork
{
public:
    DICOMNetwork();
    ~DICOMNetwork();

    void connect(const std::string &host, int port);
    void disconnect();
    bool isConnected() const;
};