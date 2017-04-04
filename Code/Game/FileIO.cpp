#include <fstream>

#include "ServerSettings.h"
#include "FileIO.h"

ServerSettings FileIO::retrieveServerSettings()
{
    ServerSettings settings;

    std::ifstream settings_file;
    settings_file.open("Resources/server_details.txt");

    if (!settings_file.is_open())
    {
        throw std::runtime_error("Missing server_details.txt");
    }

    std::getline(settings_file, settings.ip_address);

    std::string tcp_port;
    std::getline(settings_file, tcp_port);
    settings.tcp_port = std::stoi(tcp_port);

    return settings;
}
