#include <iostream>

#include "config.hpp"
#include "config_parser.hpp"
#include "http_request_parser.hpp"
#include "logger.hpp"
#include "server.hpp"

int main(int ac, char **av) {
  Logger::SetHandler(new FileStreamWrapper("log.txt"));
  std::string config_file;
  if (ac == 1)
    config_file = ConfigParser::default_file_;
  else if (ac == 2)
    config_file = av[1];
  else {
    Logger::Error() << "Invalid argument." << std::endl;
    return 1;
  }
  Logger::Info() << "Reading " << config_file << std::endl;
  try {
    const Config config = ConfigParser::Parse(config_file);
    const std::map<std::string, std::string> &v = config.GetErrorPage();
    for (std::map<std::string, std::string>::const_iterator it = v.begin();
         it != v.end(); ++it) {
      Logger::Info() << it->first << " : " << it->second << std::endl;
    }
    const std::vector<ServerContext> &m = config.GetServer();
    for (std::vector<ServerContext>::const_iterator it = m.begin();
         it != m.end(); ++it) {
      Logger::Info() << *it << std::endl;
    }
    if (!Server::Run(config)) {
      Logger::Error() << "Server::Run() failed." << std::endl;
      return 1;
    }
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
    std::exit(1);
  }
}
