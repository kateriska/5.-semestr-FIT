#include <iostream>
#include <string>
#include <vector>
#include <getopt.h>
#include <regex.h>
#include <string.h>

void IpRegexParsing(std::string input_ip)
{
  bool ipv4_input = false;
  bool ipv6_input = false;
  bool hostname_input = false;
  std::string ipv4_regex = "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)";
  std::string ipv6_regex = "^([0-9a-fA-F]{0,4}:){1,7}[0-9a-fA-F]{0,4}$";

  if (std::regex_match(input_ip, ipv4_regex))
  {
    ipv4_input = true;
    return true;
  }
  else if (std::regex_match(input_ip, ipv6_regex))
  {
    ipv6_input = true;
    return true;
  }

  return false;
}



int main(int argc, char **argv)
{
  int opt;
  std::string ip;
  std::string host_name_whois;
  std::string host_name_dns;

  while ((opt = getopt (argc, argv, "qwd")) != -1)
  {
    switch (opt)
    {
      case 'q':
        printf("input argument -q <IP|hostname> \n");
        if (argc != 3)
        {
          std::cerr << "Error - Wrong count of arguments!\n";
          exit(EXIT_FAILURE);
        }

      break;
      case 'w':
        printf("input argument -w <IP|hostname WHOIS serveru>\n");
        if (argc != 3)
        {
          std::cerr << "Error - Wrong count of arguments!\n";
          exit(EXIT_FAILURE);
        }
      break;
      case 'd':
        printf("input argument -d <IP|hostname DNS serveru>\n");
        if (argc == 2)
        {
          host_name_dns = "1.1.1.1";
          std::cout << host_name_dns + "\n";
        }
        if (argc > 3 || argc < 2)
        {
          std::cerr << "Error - Wrong count of arguments!\n";
          exit(EXIT_FAILURE);
        }
      break;
    }
  }


}
