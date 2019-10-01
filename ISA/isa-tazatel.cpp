/*
TO - DO resolve segfault when you enter numbers instead of strings (f.e. -q 55555)
*/
#include <iostream>
#include <string>
#include <vector>
#include <getopt.h>
#include <regex.h>
#include <string.h>

/*

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
*/



int main(int argc, char **argv)
{
  int opt;
  std::string ipq_input;
  std::string whois_input;
  std::string dns_input;
  bool q = false; // mandatory option
  bool w = false;

  if ((argc < 5 ) || (argc > 7))
  {
        std::cerr << "Error - Wrong count of arguments!\n";
        exit(EXIT_FAILURE);
  }

  while ((opt = getopt (argc, argv, ":q:w:d:")) != -1)
  {
    switch (opt)
    {
      case 'q':
        printf("input argument -q <IP|hostname> \n");
        ipq_input = optarg;
        std::cout << ipq_input + "\n";
        q = true;
      break;

      case 'w':
        printf("input argument -w <IP|hostname WHOIS serveru>\n");
        whois_input = optarg;
        std::cout << whois_input + "\n";
        w = true;
      break;

      case 'd':
        printf("input argument -d <IP|hostname DNS serveru>\n");
        std::cout << optarg;
        dns_input = optarg;
        std::cout << dns_input + "\n";
      break;

      default:
        printf("usage default\n");
        std::cerr << "Error - Bad parametres!\n";
        exit(EXIT_FAILURE);
      break;
      

    }
  }

    // Check mandatory (povinne) parameters:
    if ((w == false) || (q == false))
    {
      std::cerr << "Error - Missing mandatory options!\n";
      exit(EXIT_FAILURE);
    }


}
