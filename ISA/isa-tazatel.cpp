/*
TO - DO resolve segfault when you enter numbers instead of strings (f.e. -q 55555)
*/
#include <iostream>
#include <string>
#include <vector>
#include <getopt.h>
#include <regex.h>
#include <string.h>
#include <regex>
#include <arpa/inet.h>
#include <netdb.h>



std::string IpValidate(std::string input_ip)
{
  struct sockaddr_in sa;
  std::string input_validate;
  //int test_ipv4 = inet_pton(AF_INET, input_ip.c_str(), &(sa.sin_addr)); // 1 - network is ipv4 or ipv6, 0 - error
  if (inet_pton(AF_INET, input_ip.c_str(), &(sa.sin_addr)) == 1)
  {
    input_validate = "ipv4_input";
  }
  else if (inet_pton(AF_INET6, input_ip.c_str(), &(sa.sin_addr)) == 1)
  {
    input_validate = "ipv6_input";
  }
  else
  {
    input_validate = "hostname_input";
  }

  return input_validate;
}

struct input_data
{
  //vector<int> udp_ports;
  //vector<int> tcp_ports;
  //string interface;
  std::string skenned_ipv4;
  std::string skenned_ipv6;
  std::string skenned_hostname;
  std::string whois_ipv4;
  std::string whois_ipv6;
  std::string skenned_whois;
  std::string dns_ipv4;
  std::string dns_ipv6;
  std::string dns_hostname;
  //string domain;
};



int main(int argc, char **argv)
{
  int opt;
  std::string ipq_input;
  std::string whois_input;
  std::string dns_input;
  bool q = false; // mandatory option
  bool w = false;
  std:: string input_validate_ipq;
  std:: string input_validate_whois;
  std:: string input_validate_dns;

  struct input_data i_data;

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

    input_validate_ipq = IpValidate(ipq_input);
    std::cout << input_validate_ipq + "\n";
    if (input_validate_ipq == "hostname_input")
    {
      struct hostent *host;
      char *IPbuffer; 
      host = gethostbyname((char *)ipq_input.c_str());
      if (host == NULL)
      {
        printf("Error:Hostname is wrong\n");
      }
      IPbuffer = inet_ntoa(*((struct in_addr*) host->h_addr_list[0]));
      printf("Host IP: %s", IPbuffer);
    }
    input_validate_whois = IpValidate(whois_input);
    std::cout << input_validate_whois + "\n";
    input_validate_dns = IpValidate(dns_input);
    std::cout << input_validate_dns + "\n";



}
