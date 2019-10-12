#include <iostream>
#include <string>
#include <vector>
#include <getopt.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <sstream>
#include<sys/socket.h>
#include <resolv.h>
#include <netinet/in.h>
#include <arpa/nameser.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

#define WHOIS_PORT 43
#define DNS_PORT 53
#define WHOIS_MESSAGE_LENGTH 100
#define WHOIS_BUFFER_LENGTH 1500
#define DNS_MESSAGE_LENGTH 100
#define DNS_BUFFER_LENGTH 4096

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
string ConvertHostnameIPV6(string hostname)
{
  struct addrinfo hints, *infoptr;
  char *host;
  memset(&hints, 0, sizeof(struct addrinfo));

  hints.ai_family = AF_INET6;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  hints.ai_protocol = 0;

  host = (char *)hostname.c_str();
  int converted_ip = getaddrinfo(host, NULL, &hints, &infoptr);
  if (converted_ip != 0)
  {
    cerr << "Error - IPV6 for hostname doesn't exist!\n";
    exit(EXIT_FAILURE);
  }

  char converted_ip_array[256] = { 0 };
  for(struct addrinfo *p = infoptr; p != NULL; p = p->ai_next)
  {
    if (p->ai_family == AF_INET6)
    {
      getnameinfo(p->ai_addr, p->ai_addrlen, converted_ip_array, sizeof (converted_ip_array), NULL, 0, NI_NUMERICHOST);
      break;
    }
  }

  freeaddrinfo(infoptr);
  cout << converted_ip_array;
  return converted_ip_array;
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
string WhoisConnectIPV6(struct input_data i_data)
{
    struct sockaddr_in6 sw6;
    int socket_whois_ipv6 = 0;
    char message[WHOIS_MESSAGE_LENGTH];
    char buffer[WHOIS_BUFFER_LENGTH];
    string reply_from_server;
    string sent_message_string;
    int count_respond = 0;

    socket_whois_ipv6 = socket(AF_INET6 , SOCK_STREAM , 0);
    memset(buffer, 0, sizeof(sw6));
    sw6.sin6_family = AF_INET6;
    inet_pton(AF_INET6, (i_data.whois_ipv6).c_str(), &sw6.sin6_addr);
    sw6.sin6_port = htons(WHOIS_PORT);
    connect(socket_whois_ipv6, (struct sockaddr *)&sw6, sizeof(sw6));
    sprintf(message , "%s\r\n", (i_data.skenned_ipv6).c_str());
    send(socket_whois_ipv6 , message , strlen(message) , 0);

    count_respond = read(socket_whois_ipv6 , buffer , sizeof(buffer));
    cout << count_respond+ "\n";
    reply_from_server = buffer;
    cout << reply_from_server+ "\n";
    count_respond = read(socket_whois_ipv6 , buffer , sizeof(buffer));
    cout << count_respond+ "\n";
    reply_from_server = buffer;
    cout << reply_from_server+ "\n";

    close(socket_whois_ipv6);


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

void ProcessParentServerIPV6(struct input_data i_data, string whois_answer)
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
        i_data.whois_ipv6 = refer_hostname;
        parent_server_is_found = true;
        break;
      }

    }
    if (parent_server_is_found == true)
    {
      //printf("Zpracovavani materskeho serveru\n");
      whois_answer = WhoisConnectIPV6(i_data);
      ProcessParentServerIPV6(i_data, whois_answer);
    }

  }
  else
  { // rodic - okamzite zpracovani
    ProcessResponseFromWhois(whois_answer);
  }
}

int DNSConnectIPV4(struct input_data i_data)
{
  union {
        HEADER hdr;
        u_char buf[NS_PACKETSZ];
    } response;
    int responseLen;
    res_init();
    ns_msg handle;
    responseLen =res_query("www.fit.vutbr.cz",ns_c_in,ns_t_a,(u_char *)&response,sizeof(response));
    if (responseLen<0)
        exit(-1);
    if (ns_initparse(response.buf, responseLen, &handle)<0)
    {
        fprintf(stderr, "ERROR PARSING RESPONSE....");
        perror("damn");
        exit(-1);

    }
    ns_rr rr;
    int rrnum;
    ns_sect section=ns_s_an;
    for (rrnum=0;rrnum<(ns_msg_count(handle,section));rrnum++)
    {
        if (ns_parserr(&handle,ns_s_an,rrnum,&rr)<0)
        {
            fprintf(stderr, "ERROR PARSING RRs");
            exit(-1);
        }

        if (ns_rr_type(rr)==ns_t_a)
        {
          struct in_addr in;
          memcpy(&in.s_addr, ns_rr_rdata(rr), sizeof(in.s_addr));
          fprintf(stderr, "%s IN A %s\n", ns_rr_name(rr), inet_ntoa(in));
        }
    }

    union {
          HEADER hdr;
          u_char buf[NS_PACKETSZ];
      } response_aaaa;
    ns_msg handle2;
    int responseLen_aaaa =res_query("www.fit.vutbr.cz",ns_c_in,ns_t_aaaa,(u_char *)&response_aaaa,sizeof(response_aaaa));
    if (responseLen_aaaa<0)
        exit(-1);
    if (ns_initparse(response_aaaa.buf, responseLen_aaaa, &handle2)<0)
    {
        fprintf(stderr, "ERROR PARSING RESPONSE....");
        perror("damn");
        printf("message\n");
        exit(-1);
        printf("dddd\n");

    }
    printf("dudududuud\n");
    ns_rr rr2;
    int rrnum2;
    ns_sect section2=ns_s_an;
    for (rrnum2=0;rrnum2<(ns_msg_count(handle2,section2));rrnum2++)
    {
        if (ns_parserr(&handle2,ns_s_an,rrnum2,&rr2)<0)
        {
            fprintf(stderr, "ERROR PARSING RRs");
            exit(-1);
        }

        if (ns_rr_type(rr2)==ns_t_aaaa)
        {
          struct in_addr in2;
          //memcpy(&in2.s_addr, ns_rr_rdata(rr2), sizeof(in2.s_addr));
          //cout << in2.s_addr;
          printf("aaaaaaparsing\n");
          //fprintf(stderr, "%s IN AAAA %s\n", ns_rr_name(rr2), inet_ntop(in2));
        }
    }



  return 0;
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
    if (   !((i_data.skenned_ipv6).empty()) ||  !((i_data.whois_ipv6).empty())  ||  !((i_data.dns_ipv6).empty())    )
    {
      if (!(i_data.skenned_hostname).empty())
      {
        i_data.skenned_ipv6 = ConvertHostnameIPV6(i_data.skenned_hostname);
      }
      if (!(i_data.whois_hostname).empty())
      {
        i_data.whois_ipv6 = ConvertHostnameIPV6(i_data.whois_hostname);
      }
      if (!(i_data.dns_hostname).empty())
      {
        i_data.dns_ipv6 = ConvertHostnameIPV6(i_data.dns_hostname);
      }


    }
    PrintInputData(i_data);

    if (!(i_data.whois_ipv4).empty())
    {
      printf("Processing IPV4\n");
      whois_server_response = WhoisConnectIPV4(i_data);
      ProcessParentServer(i_data, whois_server_response);
    }
    else if (!(i_data.whois_ipv6).empty())
    {
      printf("Processing IPV6dundudu\n");
      whois_server_response = WhoisConnectIPV6(i_data);
      ProcessParentServerIPV6(i_data, whois_server_response);
    }
    printf("Connecting to DNS");
    DNSConnectIPV4(i_data);


}
