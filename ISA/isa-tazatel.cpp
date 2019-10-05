#include <iostream>
#include <string>
#include <vector>
#include <getopt.h>
#include <regex.h>
#include <string.h>
#include <regex>
#include <arpa/inet.h>
#include <netdb.h>
#include<unistd.h>

/*
#include<netinet/in.h>
#include<netinet/in.h> //sockaddr_in
#include<sys/socket.h>	//socket
*/

#define WHOIS_PORT 43
#define WHOIS_IPV4_LENGTH 32
#define WHOIS_MESSAGE_LENGTH 100
#define WHOIS_BUFFER_LENGTH 1500

struct input_data
{
  std::string skenned_ipv4;
  std::string skenned_ipv6;
  std::string skenned_hostname;
  std::string whois_ipv4;
  std::string whois_ipv6;
  std::string whois_hostname;
  std::string dns_ipv4;
  std::string dns_ipv6;
  std::string dns_hostname;
};


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
std::string ConvertHostname(std::string hostname)
{
  std::string converted_ip;
  struct hostent *host;
  char *converted_ip_array;
  host = gethostbyname((char *)hostname.c_str());
  if (host == NULL)
  {
      std::cerr << "Error - Hostname doesn't exist!\n";
    exit(EXIT_FAILURE);
  }
  converted_ip_array = inet_ntoa(*((struct in_addr*) host->h_addr_list[0]));
  converted_ip = converted_ip_array;
  //std::cout << converted_ip + "\n";
  return converted_ip;
}

void PrintInputData(struct input_data i_data)
{
  printf("--------Input data:--------\n");

  std::cout << "Skenned IPV4: " + i_data.skenned_ipv4 + "\n";
  std::cout << "Skenned IPV6: " + i_data.skenned_ipv6 + "\n";
  std::cout << "Skenned hostname: " + i_data.skenned_hostname + "\n";

  std::cout << "Whois IPV4: " + i_data.whois_ipv4 + "\n";
  std::cout << "Whois IPV6: " + i_data.whois_ipv6 + "\n";
  std::cout << "Whois hostname: " + i_data.whois_hostname + "\n";

  std::cout << "DNS IPV4: " + i_data.dns_ipv4 + "\n";
  std::cout << "DNS IPV6: " + i_data.dns_ipv6 + "\n";
  std::cout << "DNS hostname: " + i_data.dns_hostname + "\n";

}


std::string WhoisConnectIPV4(struct input_data i_data)
{
    struct sockaddr_in sw4;
    int socket_whois_ipv4 = 0;
    char message[WHOIS_MESSAGE_LENGTH];
    char buffer[WHOIS_BUFFER_LENGTH];
    int read_size = 0;
    int total_size = 0;
    std::string reply_from_server;
    std::string sent_message_string;
    std::string skenned_ipv4_resolve = i_data.skenned_ipv4;
    char* message_char = const_cast<char*>(skenned_ipv4_resolve.c_str());
    //std::string request = "--resource " + i_data.skenned_ipv4;


    socket_whois_ipv4 = socket(AF_INET , SOCK_STREAM , IPPROTO_TCP);
    //memset(buffer, 0, sizeof(sw4));
    memset( &sw4 , 0 , sizeof(sw4) );
    sw4.sin_family = AF_INET;
    sw4.sin_addr.s_addr = inet_addr((i_data.whois_ipv4).c_str());
    sw4.sin_port = htons(WHOIS_PORT);
    connect(socket_whois_ipv4, (struct sockaddr *)&sw4, sizeof(sw4));
    //std::cout << "Quering for: " + i_data.skenned_ipv4+ "\n";
    //sprintf(message , "%s\r\n" , message_char);
    sprintf(message , "77.65.12.44\r\n");
    //--resource 77.65.12.44
    //sprintf(message , "%s\r\n" , (request).c_str());
    //sent_message_string = message;
  //  std::cout << "Send message is: " +sent_message_string + "\n";
    if( send(socket_whois_ipv4 , message , strlen(message) , 0) < 0)
    {
      perror("send failed");
    }
    //read_size = recv(socket_whois_ipv4 , buffer , sizeof(buffer) , 0);
    /*
    while (read_size = recv(socket_whois_ipv4 , buffer , sizeof(buffer), 0))
    {
      printf("Bytes received: %d\n", read_size);
    }
    */
    recv(socket_whois_ipv4 , buffer , sizeof(buffer), 0);
    reply_from_server = buffer;
    std::cout << reply_from_server+ "\n";
    recv(socket_whois_ipv4 , buffer , sizeof(buffer), 0);
    reply_from_server = buffer;
    std::cout << reply_from_server+ "\n";

    //reply_from_server = buffer;
    //std::cout << reply_from_server+ "\n";
    close(socket_whois_ipv4);
    //std::cout << reply_from_server+ "\n";
    return reply_from_server;


}




int main(int argc, char **argv)
{
  int opt;
  std::string ipq_input;
  std::string whois_input;
  std::string dns_input;
  bool q = false; // mandatory option
  bool w = false; // mandatory option
  bool d = false; // chosen option
  std:: string input_validate_ipq;
  std:: string input_validate_whois;
  std:: string input_validate_dns;
  struct input_data i_data;
  std:: string ipq_converted;
  std:: string whois_converted;
  std:: string dns_converted;
  std:: string ipq_converted_response;
  std:: string whois_converted_response;
  std:: string dns_converted_response;
  std:: string whois_server_response;

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
        ipq_input = optarg;
        //std::cout << ipq_input + "\n";
        q = true;
      break;

      case 'w':
        whois_input = optarg;
        //std::cout << whois_input + "\n";
        w = true;
      break;

      case 'd':
        //std::cout << optarg;
        dns_input = optarg;
        //std::cout << dns_input + "\n";
        d = true;
      break;

      default:
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
    // default dns
    if (d == false)
    {
      i_data.dns_ipv4 = "1.1.1.1";
    }

    input_validate_ipq = IpValidate(ipq_input);
    if (input_validate_ipq == "ipv6_input" )
    {
      i_data.skenned_ipv6 = ipq_input;
    }
    else if ( input_validate_ipq == "ipv4_input" )
    {
      i_data.skenned_ipv4 = ipq_input;
    }
    else
    {
      i_data.skenned_hostname = ipq_input;
      ipq_converted = ConvertHostname(ipq_input);
      std::cout << ipq_converted + "\n";
      input_validate_ipq = IpValidate(ipq_converted);
      if (input_validate_ipq == "ipv6_input" )
      {
        i_data.skenned_ipv6 = ipq_converted;
      }
      else if ( input_validate_ipq == "ipv4_input" )
      {
        i_data.skenned_ipv4 = ipq_converted;
      }
    }

    ////////////////////////////////////////////////////////////////////////////////////
    input_validate_whois = IpValidate(whois_input);
    if (input_validate_whois == "ipv6_input" )
    {
      i_data.whois_ipv6 = whois_input;
    }
    else if ( input_validate_whois == "ipv4_input" )
    {
      i_data.skenned_ipv4 = whois_input;
    }
    else
    {
      i_data.whois_hostname = whois_input;
      whois_converted = ConvertHostname(whois_input);
      std::cout << whois_converted + "\n";
      input_validate_whois = IpValidate(whois_converted);
      if (input_validate_whois == "ipv6_input" )
      {
        i_data.whois_ipv6 = whois_converted;
      }
      else if ( input_validate_whois == "ipv4_input" )
      {
        i_data.whois_ipv4 = whois_converted;
      }
    }

    input_validate_dns = IpValidate(dns_input);
    if (input_validate_dns == "ipv6_input" )
    {
      i_data.dns_ipv6 = dns_input;
    }
    else if ( input_validate_dns == "ipv4_input" )
    {
      i_data.dns_ipv4 = dns_input;
    }
    else
    {
      i_data.dns_hostname = dns_input;
      dns_converted = ConvertHostname(dns_input);
      std::cout << dns_converted + "\n";
      input_validate_dns = IpValidate(dns_converted);
      if (input_validate_dns == "ipv6_input" )
      {
        i_data.dns_ipv6 = dns_converted;
      }
      else if ( input_validate_dns == "ipv4_input" )
      {
        i_data.dns_ipv4 = dns_converted;
      }
    }

    PrintInputData(i_data);
    WhoisConnectIPV4(i_data);








}
