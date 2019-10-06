#include <iostream>
#include <string>
#include <vector>
#include <getopt.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include<unistd.h>
#include <sstream>
#include<sys/socket.h>
using namespace std;


/*
#include<netinet/in.h>
#include<netinet/in.h> //sockaddr_in
#include<sys/socket.h>	//socket
#include <regex>
#include <regex.h>
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

// https://stackoverflow.com/questions/25829143/trim-whitespace-from-a-string/25829178
string TrimWhitespaces(const string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}


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
  // https://www.geeksforgeeks.org/c-program-display-hostname-ip-address/
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

// https://www.geeksforgeeks.org/socket-programming-cc/
std::string WhoisConnectIPV4(struct input_data i_data)
{
    struct sockaddr_in sw4;
    int socket_whois_ipv4 = 0;
    char message[WHOIS_MESSAGE_LENGTH];
    char buffer[WHOIS_BUFFER_LENGTH];
    std::string reply_from_server;
    std::string sent_message_string;
    int count_respond = 0;

    socket_whois_ipv4 = socket(AF_INET , SOCK_STREAM , 0);
    memset(buffer, 0, sizeof(sw4));
    sw4.sin_family = AF_INET;
    sw4.sin_addr.s_addr = inet_addr((i_data.whois_ipv4).c_str());
    sw4.sin_port = htons(WHOIS_PORT);
    connect(socket_whois_ipv4, (struct sockaddr *)&sw4, sizeof(sw4));
    //std::cout << "Quering for: " + i_data.skenned_ipv4+ "\n";
    sprintf(message , "%s\r\n", (i_data.skenned_ipv4).c_str());
    send(socket_whois_ipv4 , message , strlen(message) , 0);

    count_respond = read(socket_whois_ipv4 , buffer , sizeof(buffer));
    std::cout << count_respond+ "\n";
    reply_from_server = buffer;
    std::cout << reply_from_server+ "\n";
    count_respond = read(socket_whois_ipv4 , buffer , sizeof(buffer));
    std::cout << count_respond+ "\n";
    reply_from_server = buffer;
    std::cout << reply_from_server+ "\n";
    count_respond = read(socket_whois_ipv4 , buffer , sizeof(buffer));
    std::cout << count_respond+ "\n";
    reply_from_server = buffer;
    std::cout << reply_from_server+ "\n";
  

    close(socket_whois_ipv4);


    return reply_from_server;


}

void ProcessResponseFromWhois(std::string whois_server_response)
{
  std::istringstream response_stream{whois_server_response};
  std::string line;
  /*
  std::size_t found_intetnum = line.find("inetnum:");
  std::size_t found_netname = line.find("netname:");
  std::size_t found_descr = line.find("descr:");
  std::size_t found_country = line.find("country:");
  std::size_t found_admin = line.find("admin-c:");
  std::size_t found_adress = line.find("adress:");
  std::size_t found_phone = line.find("phone:");
  */
  while (std::getline(response_stream, line))
  {
    std::size_t found_intetnum = line.find("inetnum:");
    std::size_t found_netname = line.find("netname:");
    std::size_t found_descr = line.find("descr:");
    std::size_t found_country = line.find("country:");
    std::size_t found_admin = line.find("admin-c:");
    std::size_t found_adress = line.find("adress:");
    std::size_t found_phone = line.find("phone:");
    if (found_intetnum!=std::string::npos)
    {
      std::cout << line + "\n";
    }
    if (found_netname!=std::string::npos)
    {
      std::cout << line + "\n";
    }
    if (found_descr!=std::string::npos)
    {
      std::cout << line + "\n";
    }
    if (found_country!=std::string::npos)
    {
      std::cout << line + "\n";
    }
    if (found_admin!=std::string::npos)
    {
      std::cout << line + "\n";
    }
    if (found_adress!=std::string::npos)
    {
      std::cout << line + "\n";
    }
    if (found_phone!=std::string::npos)
    {
      std::cout << line + "\n";
    }
  }
}

void ProcessParentServer(struct input_data i_data, std::string whois_answer)
{
  std::istringstream stream{whois_answer};
  std::string line;
  std::string refer_hostname;
  std::size_t found_parent_server = whois_answer.find("whois.");
  //std::size_t found_parent_server_line = line.find("refer:");
  bool parent_server_is_found = false;

  if (found_parent_server!=std::string::npos)
  {
    printf("Tohle byl detsky server - je potreba zjistit matersky\n");
    while (std::getline(stream, line)) // nacitam radek
    {
      printf("printing line\n");
      std::cout << line+ "\n";
      std::size_t found_parent_server_line = line.find("whois."); // hledam
      if (found_parent_server_line!=std::string::npos)
      {
        printf("Processing paren server hostname\n");
        refer_hostname = line.substr(line.find("whois."));
        std::cout << refer_hostname+ "\n";
        /*
        if (refer_hostname.find(" ")!=std::string::npos)
        {
          refer_hostname = refer_hostname.substr(refer_hostname.find(" "));
        }
        */
        printf("===Processing parent hostname===\n");
        refer_hostname = TrimWhitespaces(refer_hostname);
        std::cout << refer_hostname+ "\n";
        printf("===Processing parent hostname===\n");
        refer_hostname = ConvertHostname(refer_hostname);
        i_data.whois_ipv4 = refer_hostname;
        parent_server_is_found = true; // najdu rodice u serveru
        break;
      }

    }
    if (parent_server_is_found == true)
    {
      printf("Zpracovavani materskeho serveru\n");
      whois_answer = WhoisConnectIPV4(i_data);
      ProcessParentServer(i_data, whois_answer);
      //ProcessResponseFromWhois(whois_server_response);
    }

  }
  else
  { // rodic - okamzite zpracovani
    ProcessResponseFromWhois(whois_answer);
  }
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
    whois_server_response = WhoisConnectIPV4(i_data);
    ProcessParentServer(i_data, whois_server_response);
    /*
    std::istringstream stream{whois_server_response};
    std::string line;
    std::string refer_hostname;
    std::size_t found_parent_server = whois_server_response.find("whois.");
    //std::size_t found_parent_server_line = line.find("refer:");
    if (found_parent_server!=std::string::npos)
    {
      printf("Tohle byl detsky server - je potreba zjistit matersky\n");
      while (std::getline(stream, line))
      {
        printf("printing line\n");
        std::cout << line+ "\n";
        std::size_t found_parent_server_line = line.find("whois.");
        if (found_parent_server_line!=std::string::npos)
        {
          printf("Processing paren server hostname\n");
          refer_hostname = line.substr(line.find(":") + 1);
          printf("===Processing parent hostname===\n");
          refer_hostname = TrimWhitespaces(refer_hostname);
          std::cout << refer_hostname+ "\n";
          printf("===Processing parent hostname===\n");
          refer_hostname = ConvertHostname(refer_hostname);
          i_data.whois_ipv4 = refer_hostname;
          break;
        }

      }
      printf("Zpracovavani materskeho serveru\n");
      whois_server_response = WhoisConnectIPV4(i_data);
      ProcessResponseFromWhois(whois_server_response);

    }
    else
    {
      ProcessResponseFromWhois(whois_server_response);
    }

    /*
    std::istringstream stream{whois_server_response};
    std::string line;
    std::string refer_hostname;
    std::size_t found = line.find("refer:");



    while (std::getline(stream, line))
    {
        //printf("===Processing line===\n");
        //std::cout << line+ "\n";

        if (found!=std::string::npos)
        {
          printf("Tohle byl detsky server - je potreba zjistit matersky\n");
          refer_hostname = line.substr(14);
          printf("===Processing parent hostname===\n");
          std::cout << refer_hostname+ "\n";
          refer_hostname = ConvertHostname(refer_hostname);
          i_data.whois_ipv4 = refer_hostname;
          printf("Zpracovavani materskeho serveru\n");
          whois_server_response = WhoisConnectIPV4(i_data);
          ProcessResponseFromWhois(whois_server_response);

        }
        else
        {
          ProcessResponseFromWhois(whois_server_response);
          break;
        }
    }
*/








}
