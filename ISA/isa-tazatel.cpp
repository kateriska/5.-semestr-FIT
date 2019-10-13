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
    string reply_from_server = "";
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

    while (recv(socket_whois_ipv4 , buffer , sizeof(buffer),0))
    {
      //cout << buffer << endl;
      reply_from_server = reply_from_server + buffer;
      memset(buffer, 0, sizeof(buffer));
    }
    cout << reply_from_server+ "\n";

    close(socket_whois_ipv4);

    return reply_from_server;


}
string WhoisConnectIPV6(struct input_data i_data)
{
    struct sockaddr_in6 sw6;
    int socket_whois_ipv6 = 0;
    char message[WHOIS_MESSAGE_LENGTH];
    char buffer[WHOIS_BUFFER_LENGTH];
    string reply_from_server = "";
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

    while (recv(socket_whois_ipv6 , buffer , sizeof(buffer),0))
    {
      //cout << buffer << endl;
      reply_from_server = reply_from_server + buffer;
      memset(buffer, 0, sizeof(buffer));
    }
    cout << reply_from_server+ "\n";

    close(socket_whois_ipv6);


    return reply_from_server;


}

void ProcessResponseFromWhois(string whois_server_response)
{
  istringstream response_stream{whois_server_response};
  string line;

  printf("=== WHOIS ===\n");
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

  bool parent_server_is_found = false;

  while (std::getline(stream, line))
  {
    //cout << line+ "\n";
    size_t found_parent_server_line = line.find("whois.");
    size_t found_refer = line.find("refer:");
    //size_t found_resource = line.find("resource:");
    size_t found_resource_link = line.find("ResourceLink:");
    if ((found_parent_server_line!=std::string::npos) && ((found_refer!=std::string::npos)  || (found_resource_link!=std::string::npos)))
    {
      refer_hostname = line.substr(line.find("whois."));
      cout << refer_hostname;
      refer_hostname = TrimWhitespaces(refer_hostname);
      //cout << refer_hostname;
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

  bool parent_server_is_found = false;

  while (std::getline(stream, line))
  {
    //cout << line+ "\n";
    size_t found_parent_server_line = line.find("whois.");
    size_t found_refer = line.find("refer:");
    //size_t found_resource = line.find("resource:");
    size_t found_resource_link = line.find("ResourceLink:");
    if ((found_parent_server_line!=std::string::npos) && ((found_refer!=std::string::npos)  || (found_resource_link!=std::string::npos)))
    {
      refer_hostname = line.substr(line.find("whois."));
      cout << refer_hostname;
      refer_hostname = TrimWhitespaces(refer_hostname);
      //cout << refer_hostname;
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

  else
  { // rodic - okamzite zpracovani
    ProcessResponseFromWhois(whois_answer);
  }
}

int DNSConnectIPV4(struct input_data i_data)
{
    printf("=== DNS ===\n");

    res_init();
    _res.nsaddr_list[0].sin_addr.s_addr = inet_addr(i_data.dns_ipv4.c_str());
  ///////////////////////////// A //////////////////////////////////////
    u_char buffer_a[DNS_BUFFER_LENGTH];
    ns_msg msg_a;
    int query_a = res_query(i_data.skenned_hostname.c_str(), ns_c_in, ns_t_a, buffer_a, sizeof(buffer_a));
    ns_initparse(buffer_a, query_a, &msg_a);

    ns_rr rr_a;

    int msg_count_a = ns_msg_count(msg_a, ns_s_an);
    for (int i = 0; i < msg_count_a; i++)
    {
        ns_parserr(&msg_a,ns_s_an,i,&rr_a);

        if (ns_rr_type(rr_a) == ns_t_a)
        {
          struct in_addr result_a_struct;
          memcpy(&result_a_struct.s_addr, ns_rr_rdata(rr_a), sizeof(result_a_struct.s_addr));
          char *result_a = inet_ntoa(result_a_struct);
          string result_a_string = result_a;
          cout << "A:              " + result_a_string + "\n";
          //fprintf(stderr, "%s IN A %s\n", ns_rr_name(rr_a), inet_ntoa(in));
        }
    }

///////////////////////////// AAAA ////////////////////////////////
    u_char buffer_aaaa[DNS_BUFFER_LENGTH];
    ns_msg msg_aaaa;
    int query_aaaa = res_query(i_data.skenned_hostname.c_str(), ns_c_in, ns_t_aaaa, buffer_aaaa, sizeof(buffer_aaaa));
    ns_initparse(buffer_aaaa, query_aaaa, &msg_aaaa);

    ns_rr rr_aaaa;

    int msg_count_aaaa = ns_msg_count(msg_aaaa, ns_s_an);
    for (int i = 0; i < msg_count_aaaa; i++)
    {
        ns_parserr(&msg_aaaa,ns_s_an,i,&rr_aaaa);

        if (ns_rr_type(rr_aaaa) == ns_t_aaaa)
        {
          struct in6_addr result_aaaa_struct;
          const void *src;
          char dst[46];

          memcpy(&result_aaaa_struct.s6_addr, ns_rr_rdata(rr_aaaa), sizeof(result_aaaa_struct.s6_addr));
          memcpy(&src, result_aaaa_struct.s6_addr , sizeof(result_aaaa_struct.s6_addr));
          const char *result_aaaa = inet_ntop(AF_INET6, &src, dst, sizeof(dst));
          //cout << dst;
          string result_aaaa_string = result_aaaa;
          cout << "AAAA:           " + result_aaaa_string + "\n";
          //fprintf(stderr, "%s IN A %s\n", ns_rr_name(rr_a), inet_ntoa(in));
        }
    }

    ///////////////////////////// SOA ////////////////////////////////
    u_char buffer_soa[1024];
    u_char response_soa[1024];
    ns_msg msg_soa;
    int query_soa = res_mkquery(ns_o_query, i_data.skenned_hostname.c_str(), ns_c_in, ns_t_soa, NULL,0, NULL,buffer_soa, sizeof(buffer_soa));
    int response_length_soa = res_send(buffer_soa, sizeof(buffer_soa),response_soa, sizeof(response_soa) );
    cout << query_soa;
    printf("blblblbl\n");
    cout << response_length_soa;

    int soa_parsing = ns_initparse(response_soa, response_length_soa, &msg_soa);
    printf("blblblbl\n");
    cout << soa_parsing;
    printf("blblblbl\n");

    ns_rr rr_soa;
    if (!ns_msg_getflag(msg_soa, ns_f_aa))
    {
      printf("neco spatnen\n");
    }


    int msg_count_soa = ns_msg_count(msg_soa, ns_s_an);
    cout << msg_count_soa;
    /*
    for (int i = 0; i < msg_count_soa; i++)
    {
        ns_parserr(&msg_soa,ns_s_an,i,&rr_soa);
        if (ns_rr_type(rr_soa) == ns_t_soa)
        {
          printf("SOA info found\n");
          char result_soa[25000];
          memcpy(&result_soa, ns_rr_rdata(rr_soa), sizeof(result_soa));
          string result_soa_string = result_soa;
          cout << "SOA:           " + result_soa_string + "\n";
          //fprintf(stderr, "%s IN A %s\n", ns_rr_name(rr_a), inet_ntoa(in));
        }
    }
    */


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
    DNSConnectIPV4(i_data);


}
