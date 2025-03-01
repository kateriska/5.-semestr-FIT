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
#include <resolv.h>
using namespace std;

#define WHOIS_PORT 43
#define DNS_PORT 53
#define WHOIS_MESSAGE_LENGTH 100
#define WHOIS_BUFFER_LENGTH 1500
#define DNS_MESSAGE_LENGTH 100
#define DNS_BUFFER_LENGTH 1500

struct input_data
{
  string skenned_ipv4;
  string skenned_ipv6;
  string skenned_hostname;
  string whois_ipv4;
  string whois_ipv6;
  string whois_hostname;
  string dns_ipv4;
  string dns_ipv6;
  string dns_hostname;
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


string IpValidate(string input_ip)
{
  struct sockaddr_in sa;
  string input_validate;

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
string ConvertHostname(string hostname)
{
  string converted_ip;
  struct hostent *host;
  char *converted_ip_array;
  host = gethostbyname((char *)hostname.c_str());
  if (host == NULL)
  {
    cerr << "Error - Hostname doesn't exist!\n";
    exit(EXIT_FAILURE);
  }
  // https://www.geeksforgeeks.org/c-program-display-hostname-ip-address/
  converted_ip_array = inet_ntoa(*((struct in_addr*) host->h_addr_list[0]));
  converted_ip = converted_ip_array;
  return converted_ip;
}

void PrintInputData(struct input_data i_data)
{
  printf("--------Input data:--------\n");

  cout << "Skenned IPV4: " + i_data.skenned_ipv4 + "\n";
  cout << "Skenned IPV6: " + i_data.skenned_ipv6 + "\n";
  cout << "Skenned hostname: " + i_data.skenned_hostname + "\n";

  cout << "Whois IPV4: " + i_data.whois_ipv4 + "\n";
  cout << "Whois IPV6: " + i_data.whois_ipv6 + "\n";
  cout << "Whois hostname: " + i_data.whois_hostname + "\n";

  cout << "DNS IPV4: " + i_data.dns_ipv4 + "\n";
  cout << "DNS IPV6: " + i_data.dns_ipv6 + "\n";
  cout << "DNS hostname: " + i_data.dns_hostname + "\n";

}

// https://www.geeksforgeeks.org/socket-programming-cc/
string WhoisConnectIPV4(struct input_data i_data)
{
    struct sockaddr_in sw4;
    int socket_whois_ipv4 = 0;
    char message[WHOIS_MESSAGE_LENGTH];
    char buffer[WHOIS_BUFFER_LENGTH];
    string reply_from_server;
    string sent_message_string;
    int count_respond = 0;

    socket_whois_ipv4 = socket(AF_INET , SOCK_STREAM , 0);
    memset(buffer, 0, sizeof(sw4));
    sw4.sin_family = AF_INET;
    sw4.sin_addr.s_addr = inet_addr((i_data.whois_ipv4).c_str());
    sw4.sin_port = htons(WHOIS_PORT);
    connect(socket_whois_ipv4, (struct sockaddr *)&sw4, sizeof(sw4));
    sprintf(message , "%s\r\n", (i_data.skenned_ipv4).c_str());
    send(socket_whois_ipv4 , message , strlen(message) , 0);

    count_respond = read(socket_whois_ipv4 , buffer , sizeof(buffer));
    //cout << count_respond+ "\n";
    reply_from_server = buffer;
    //cout << reply_from_server+ "\n";
    count_respond = read(socket_whois_ipv4 , buffer , sizeof(buffer));
    //cout << count_respond+ "\n";
    reply_from_server = buffer;
    //cout << reply_from_server+ "\n";

    close(socket_whois_ipv4);

    return reply_from_server;


}

void ProcessResponseFromWhois(string whois_server_response)
{
  istringstream response_stream{whois_server_response};
  string line;

  printf("=====Printing output information========\n");
  while (getline(response_stream, line))
  {

    if ((line.find("inetnum:")!=std::string::npos) || (line.find("NetRange:")!=std::string::npos))
    {
      cout << line + "\n";
    }
    else if ((line.find("netname:")!=std::string::npos) || (line.find("NetName:")!=std::string::npos))
    {
      cout << line + "\n";
    }
    else if ((line.find("descr:")!=std::string::npos) || (line.find("Organization:")!=std::string::npos))
    {
      cout << line + "\n";
    }
    else if ((line.find("country:")!=std::string::npos) || (line.find("Country:")!=std::string::npos))
    {
      cout << line + "\n";
    }
    else if ((line.find("admin-c:")!=std::string::npos) || (line.find("OrgTechHandle:")!=std::string::npos))
    {
      cout << line + "\n";
    }
    else if ((line.find("address:")!=std::string::npos) || (line.find("Address:")!=std::string::npos))
    {
      cout << line + "\n";
    }
    else if ((line.find("phone:")!=std::string::npos) || (line.find("OrgTechPhone:")!=std::string::npos))
    {
      cout << line + "\n";
    }

  }
  return;
}

void ProcessParentServer(struct input_data i_data, string whois_answer)
{
  istringstream stream{whois_answer};
  string line;
  string refer_hostname;
  size_t found_parent_server = whois_answer.find("whois.");

  bool parent_server_is_found = false;

  if (found_parent_server!=std::string::npos)
  {
    //printf("Tohle byl detsky server - je potreba zjistit matersky\n");
    while (std::getline(stream, line))
    {
      //cout << line+ "\n";
      size_t found_parent_server_line = line.find("whois.");
      if (found_parent_server_line!=std::string::npos)
      {
        refer_hostname = line.substr(line.find("whois."));
        //cout << refer_hostname+ "\n";
        refer_hostname = TrimWhitespaces(refer_hostname);
        //cout << refer_hostname+ "\n";
        refer_hostname = ConvertHostname(refer_hostname);
        i_data.whois_ipv4 = refer_hostname;
        parent_server_is_found = true;
        break;
      }

    }
    if (parent_server_is_found == true)
    {
      //printf("Zpracovavani materskeho serveru\n");
      whois_answer = WhoisConnectIPV4(i_data);
      ProcessParentServer(i_data, whois_answer);
    }

  }
  else
  { // rodic - okamzite zpracovani
    ProcessResponseFromWhois(whois_answer);
  }
}

string DNSConnectIPV4(struct input_data i_data)
{
  struct sockaddr_in sd4;
  int socket_dns_ipv4 = 0;
  char message_dns[DNS_MESSAGE_LENGTH];
  char buffer_dns[DNS_BUFFER_LENGTH];
  string reply_from_server_dns4;
  string sent_message_string;
  int count_respond_dns = 0;
  socket_dns_ipv4 = socket(AF_INET , SOCK_STREAM , 0);
  memset(buffer_dns, 0, sizeof(sd4));
  sd4.sin_family = AF_INET;
  sd4.sin_addr.s_addr = inet_addr((i_data.dns_ipv4).c_str());
  sd4.sin_port = htons(DNS_PORT);
  if ((connect(socket_dns_ipv4, (struct sockaddr *)&sd4, sizeof(sd4))) < 0)
  {
    printf("Connection to DNS server failed\n");
  }
  sprintf(message_dns , "%s\r\n", (i_data.skenned_ipv4).c_str());
  if ((send(socket_dns_ipv4 , message_dns , strlen(message_dns) , 0)) < 0)
  {
    printf("Send message to DNS server failed\n");
  }

  count_respond_dns = read(socket_dns_ipv4 , buffer_dns , sizeof(buffer_dns));
  cout << count_respond_dns+ "\n";
  reply_from_server_dns4 = buffer_dns;
  cout << reply_from_server_dns4+ "\n";
  count_respond_dns = read(socket_dns_ipv4 , buffer_dns , sizeof(buffer_dns));
  cout << count_respond_dns+ "\n";
  reply_from_server_dns4 = buffer_dns;
  cout << reply_from_server_dns4+ "\n";

  close(socket_dns_ipv4);

  return reply_from_server_dns4;
}




int main(int argc, char **argv)
{
  int opt;
  string ipq_input;
  string whois_input;
  string dns_input;
  bool q = false; // mandatory option
  bool w = false; // mandatory option
  bool d = false; // chosen option
  string input_validate_ipq;
  string input_validate_whois;
  string input_validate_dns;
  struct input_data i_data;
  string ipq_converted;
  string whois_converted;
  string dns_converted;
  string whois_server_response;
  string dns_server_response;

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
      //cout << ipq_converted + "\n";
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
      //cout << whois_converted + "\n";
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

    if (d == true)
    {
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
        //cout << dns_converted + "\n";
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
  }

    PrintInputData(i_data);
    whois_server_response = WhoisConnectIPV4(i_data);
    ProcessParentServer(i_data, whois_server_response);
    dns_server_response = DNSConnectIPV4(i_data);

}
