/*
  Author: Katerina Fortova (xforto00)
  Project: ISA - Whois whisperer
  Date: October - November 2019
*/

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

using namespace std;

#define WHOIS_PORT 43
#define WHOIS_MESSAGE_LENGTH 100
#define WHOIS_BUFFER_LENGTH 1500
#define DNS_BUFFER_LENGTH 1500

struct input_data
{
  string scanned_ipv4;
  string scanned_ipv6;
  string scanned_hostname;
  string whois_ipv4;
  string whois_ipv6;
  string whois_hostname;
  string dns_ipv4;
  string dns_ipv6;
  string dns_hostname;
};

/*
Function for trimming whitespaces from input string
Source:
***************************************************************************************
  *    Title: Trim whitespace from a String
  *    Author: user of stackoverflow with nickname "Anthony Kong" -> https://stackoverflow.com/users/58129/anthony-kong
  *    Date: 14.9.2014
  *    Code version: 1
  *    Availability: https://stackoverflow.com/questions/25829143/trim-whitespace-from-a-string/25829178
**************************************************************************************
*/

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

/*
Function for making reverse string
Source:
***************************************************************************************
  *    Title: C++ reversing a string
  *    Author: user of stackoverflow with nickname "JMan Mousey" -> https://stackoverflow.com/users/3472761/jman-mousey
  *    Date: 15.9.2017
  *    Code version: 1
  *    Availability: https://stackoverflow.com/questions/46248315/c-reversing-a-string
**************************************************************************************
*/
void reverseStringInPlace(string &stringToReverse)
{
    int numCharacters = stringToReverse.length();
    for (int i=0; i<numCharacters/2; i++)
    {
      swap(stringToReverse[i], stringToReverse[numCharacters-i-1]);
    }
}

/*
Function for validate whether input is IPv4, IPv6 or hostname
*/
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

/*
Function for converting hostname to IPv4
Source:
***************************************************************************************
 *    Title: Networking, Part 2: Using getaddrinfo
 *    Availability: https://github.com/angrave/SystemProgramming/wiki/Networking,-Part-2:-Using-getaddrinfo
**************************************************************************************
*/
string ConvertHostname(string hostname)
{
  struct addrinfo hints, *infoptr;
  char *host;
  memset(&hints, 0, sizeof(struct addrinfo));

  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  hints.ai_protocol = 0;

  host = (char *)hostname.c_str();
  int converted_ip = getaddrinfo(host, NULL, &hints, &infoptr);
  if (converted_ip != 0)
  {
    cerr << "Error - IPv4 for hostname doesn't exist! Check name of hostname.\n";
    exit(EXIT_FAILURE);
  }

  char converted_ip_array[256] = { 0 };
  for(struct addrinfo *p = infoptr; p != NULL; p = p->ai_next)
  {
    if (p->ai_family == AF_INET)
    {
      getnameinfo(p->ai_addr, p->ai_addrlen, converted_ip_array, sizeof (converted_ip_array), NULL, 0, NI_NUMERICHOST);
      break;
    }
  }

  freeaddrinfo(infoptr);
  return converted_ip_array;
}

/*
Function for converting hostname to IPv6
Source:
***************************************************************************************
 *    Title: Networking, Part 2: Using getaddrinfo
 *    Availability: https://github.com/angrave/SystemProgramming/wiki/Networking,-Part-2:-Using-getaddrinfo
**************************************************************************************
*/
string ConvertHostnameIPv6(string hostname)
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
    cerr << "Error - IPv6 for hostname doesn't exist!\n";
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
  return converted_ip_array;
}


/*
Function for converting IPv6 to valid hostname
Source:
***************************************************************************************
 *    Title: getnameinfo()
 *    Availability: https://beej.us/guide/bgnet/html/multi/getnameinfoman.html
**************************************************************************************
*/
string ConvertIPv6toHostname(struct input_data i_data, string ipv6_address)
{
  struct sockaddr_in6 sa;
  sa.sin6_family = AF_INET6;
  inet_pton(AF_INET6, ipv6_address.c_str(), &sa.sin6_addr);

  char host[1024];
  char service[20];

  getnameinfo((struct sockaddr*)&sa, sizeof sa, host, sizeof host, service, sizeof service, 0);
  string host_string = host;

  return host_string;
}

/*
Debugging function printing information about input structure
*/
void PrintInputData(struct input_data i_data)
{
  printf("--------Input data:--------\n");

  cout << "Scanned IPv4: " + i_data.scanned_ipv4 + "\n";
  cout << "Scanned IPv6: " + i_data.scanned_ipv6 + "\n";
  cout << "Scanned hostname: " + i_data.scanned_hostname + "\n";

  cout << "WHOIS IPv4: " + i_data.whois_ipv4 + "\n";
  cout << "WHOIS IPv6: " + i_data.whois_ipv6 + "\n";
  cout << "WHOIS hostname: " + i_data.whois_hostname + "\n";

  cout << "DNS IPv4: " + i_data.dns_ipv4 + "\n";
  cout << "DNS IPv6: " + i_data.dns_ipv6 + "\n";
  cout << "DNS hostname: " + i_data.dns_hostname + "\n";

}

/*
Function for connecting to WHOIS server for IPv4 address
Sources and helpful links:
***************************************************************************************
 *    Title: Socket Programming in C/C++
 *    Availability: https://www.geeksforgeeks.org/socket-programming-cc/
**************************************************************************************
***************************************************************************************
 *    Title: C code to perform IP whois
  *    Availability: https://www.binarytides.com/c-code-to-perform-ip-whois/
***************************************************************************************
*/
string WHOISConnectIPv4(struct input_data i_data)
{
    struct sockaddr_in sw4;
    int socket_whois_ipv4 = 0;
    char message[WHOIS_MESSAGE_LENGTH];
    char buffer[WHOIS_BUFFER_LENGTH];
    string reply_from_server = "";
    string sent_message_string;

    socket_whois_ipv4 = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_whois_ipv4 < 0)
    {
      cerr << "Error - Creating socket for WHOIS server failed!\n";
      exit(EXIT_FAILURE);
    }
    memset(buffer, 0, sizeof(sw4));
    sw4.sin_family = AF_INET;
    sw4.sin_addr.s_addr = inet_addr((i_data.whois_ipv4).c_str());
    sw4.sin_port = htons(WHOIS_PORT);
    int connect_whois_ipv4 = connect(socket_whois_ipv4, (struct sockaddr *)&sw4, sizeof(sw4));
    if (connect_whois_ipv4 < 0)
    {
      cerr << "Error - Connection to WHOIS server failed!\n";
      exit(EXIT_FAILURE);
    }
    sprintf(message , "%s\r\n", (i_data.scanned_ipv4).c_str());
    int send_whois_ipv4 = send(socket_whois_ipv4 , message , strlen(message) , 0);
    if (send_whois_ipv4 < 0)
    {
      cerr << "Error - Sending a message to WHOIS server failed!\n";
      exit(EXIT_FAILURE);
    }

    while (recv(socket_whois_ipv4 , buffer , sizeof(buffer),0))
    {
      reply_from_server = reply_from_server + buffer;
      memset(buffer, 0, sizeof(buffer));
    }

    int close_whois_ipv4 = close(socket_whois_ipv4);
    if (close_whois_ipv4 < 0)
    {
      cerr << "Error - Closing a socket after connection to WHOIS server failed!\n";
      exit(EXIT_FAILURE);
    }

    return reply_from_server;

}

/*
Function for connecting to WHOIS server for IPv6 address
*/
string WHOISConnectIPv6(struct input_data i_data)
{
    struct sockaddr_in6 sw6;
    int socket_whois_ipv6 = 0;
    char message[WHOIS_MESSAGE_LENGTH];
    char buffer[WHOIS_BUFFER_LENGTH];
    string reply_from_server = "";
    string sent_message_string;

    socket_whois_ipv6 = socket(AF_INET6 , SOCK_STREAM , 0);
    if (socket_whois_ipv6 < 0)
    {
      cerr << "Error - Creating socket for WHOIS server failed!\n";
      exit(EXIT_FAILURE);
    }
    memset(buffer, 0, sizeof(sw6));
    sw6.sin6_family = AF_INET6;
    inet_pton(AF_INET6, (i_data.whois_ipv6).c_str(), &sw6.sin6_addr);
    sw6.sin6_port = htons(WHOIS_PORT);
    int connect_whois_ipv6 = connect(socket_whois_ipv6, (struct sockaddr *)&sw6, sizeof(sw6));
    if (connect_whois_ipv6 < 0)
    {
      cerr << "Error - Connection to WHOIS server failed!\n";
      exit(EXIT_FAILURE);
    }
    sprintf(message , "%s\r\n", (i_data.scanned_ipv6).c_str());
    int send_whois_ipv6 = send(socket_whois_ipv6 , message , strlen(message) , 0);
    if (send_whois_ipv6 < 0)
    {
      cerr << "Error - Sending a message to WHOIS server failed!\n";
      exit(EXIT_FAILURE);
    }

    while (recv(socket_whois_ipv6 , buffer , sizeof(buffer),0))
    {
      reply_from_server = reply_from_server + buffer;
      memset(buffer, 0, sizeof(buffer));
    }

    int close_whois_ipv6 = close(socket_whois_ipv6);
    if (close_whois_ipv6 < 0)
    {
      cerr << "Error - Closing a socket after connection to WHOIS server failed!\n";
      exit(EXIT_FAILURE);
    }


    return reply_from_server;

}

/*
Function for parsing response from WHOIS, finding relevant information from response
*/
void WHOISParseResponse(string whois_server_response)
{
  istringstream response_stream{whois_server_response};
  string line;
  int relevant_info_found = 0;

  printf("=== WHOIS ===\n");
  while (getline(response_stream, line))
  {

    if ((line.find("inetnum:")!=string::npos) || (line.find("NetRange:")!=string::npos))
    {
      string inetnum_value = line.substr(line.find(":") + 1);
      inetnum_value = TrimWhitespaces(inetnum_value);
      relevant_info_found++;
      cout << "inetnum:        " + inetnum_value + "\n";
    }
    else if (line.find("inet6num:")!=string::npos)
    {
      string inet6num_value = line.substr(line.find(":") + 1);
      inet6num_value = TrimWhitespaces(inet6num_value);
      relevant_info_found++;
      cout << "inet6num:       " + inet6num_value + "\n";
    }
    else if ((line.find("netname:")!=string::npos) || (line.find("NetName:")!=string::npos))
    {
      string netname_value = line.substr(line.find(":") + 1);
      netname_value = TrimWhitespaces(netname_value);
      relevant_info_found++;
      cout << "netname:        " + netname_value + "\n";
    }
    else if ((line.find("descr:")!=string::npos) || (line.find("Organization:")!=string::npos))
    {
      string descr_value = line.substr(line.find(":") + 1);
      descr_value = TrimWhitespaces(descr_value);
      relevant_info_found++;
      cout << "descr:          " + descr_value + "\n";
    }
    else if ((line.find("country:")!=string::npos) || (line.find("Country:")!=string::npos))
    {
      string country_value = line.substr(line.find(":") + 1);
      country_value = TrimWhitespaces(country_value);
      relevant_info_found++;
      cout << "country:        " + country_value + "\n";
    }
    else if ((line.find("admin-c:")!=string::npos) || (line.find("OrgTechHandle:")!=string::npos))
    {
      string admin_value = line.substr(line.find(":") + 1);
      admin_value = TrimWhitespaces(admin_value);
      relevant_info_found++;
      cout << "admin-c:        " + admin_value + "\n";
    }
    else if ((line.find("address:")!=string::npos) || (line.find("Address:")!=string::npos))
    {
      string address_value = line.substr(line.find(":") + 1);
      address_value = TrimWhitespaces(address_value);
      relevant_info_found++;
      cout << "address:        " + address_value + "\n";
    }
    else if ((line.find("phone:")!=string::npos) || (line.find("OrgTechPhone:")!=string::npos))
    {
      string phone_value = line.substr(line.find(":") + 1);
      phone_value = TrimWhitespaces(phone_value);
      relevant_info_found++;
      cout << "phone:          " + phone_value + "\n";
    }

  }
  if (relevant_info_found == 0)
  {
    cout << "Parent WHOIS server didn't find any relevant information about searched IP!\n";
  }
  return;
}

/*
Function for finding parent WHOIS server when we didn't receive relevant data or process parent server immediately when we get them
Works for IPv4 adressess
*/
void WHOISParentServerIPv4(struct input_data i_data, string whois_answer)
{
  istringstream stream{whois_answer};
  string line;
  string refer_hostname;

  bool parent_server_is_found = false;

  while (getline(stream, line))
  {
    // search for key words indicating answer on another WHOIS server
    size_t found_parent_server_line = line.find("whois.");
    size_t found_refer = line.find("refer:");
    size_t found_resource_link = line.find("ResourceLink:");
    if ((found_parent_server_line!=string::npos) && ((found_refer!=string::npos)  || (found_resource_link!=string::npos)))
    {
      refer_hostname = line.substr(line.find("whois.")); // cut address of WHOIS server
      refer_hostname = TrimWhitespaces(refer_hostname);
      refer_hostname = ConvertHostname(refer_hostname);
      i_data.whois_ipv4 = refer_hostname;
      parent_server_is_found = true;
      break;
    }

  }

  if (parent_server_is_found == true)
  {
    whois_answer = WHOISConnectIPv4(i_data);
    WHOISParentServerIPv4(i_data, whois_answer);
  }

  else
  {
    // parent server - connect and process immediately
    WHOISParseResponse(whois_answer);
  }
}

/*
Function for finding parent WHOIS server when we didn't receive relevant data or process parent server immediately when we get them
Works for IPv6 adressess
*/
void WHOISParentServerIPv6(struct input_data i_data, string whois_answer)
{
  istringstream stream{whois_answer};
  string line;
  string refer_hostname;

  bool parent_server_is_found = false;

  while (getline(stream, line))
  {
    size_t found_parent_server_line = line.find("whois.");
    size_t found_refer = line.find("refer:");
    size_t found_resource_link = line.find("ResourceLink:");
    size_t found_resource_whois = line.find("whois:");
    if ((found_parent_server_line!=string::npos) && ((found_refer!=string::npos)  || (found_resource_link!=string::npos) || (found_resource_whois!=string::npos)  ))
    {
      refer_hostname = line.substr(line.find("whois."));
      refer_hostname = TrimWhitespaces(refer_hostname);
      refer_hostname = ConvertHostnameIPv6(refer_hostname);
      i_data.whois_ipv6 = refer_hostname;
      parent_server_is_found = true;
      break;
    }

  }

  if (parent_server_is_found == true)
  {
    whois_answer = WHOISConnectIPv6(i_data);
    WHOISParentServerIPv6(i_data, whois_answer);
  }

  else
  {
    WHOISParseResponse(whois_answer);
  }
}

/*
Function for connecting to DNS and resolving relevant data
Sources and helpful links:
***************************************************************************************
 *    Title: C Programming with the Resolver Library Routines
 *    Availability: https://docstore.mik.ua/orelly/networking_2ndEd/dns/ch15_02.htm
***************************************************************************************
***************************************************************************************
  *    Title: How to query a server and get the MX, A, NS records
  *    Author: user of stackoverflow with nickname "Dima00782" -> https://stackoverflow.com/users/1818004/dima00782
  *    Author: user of stackoverflow with nickname "Code Painters" -> https://stackoverflow.com/users/483173/code-painters
  *    Date: 18.3.2013
  *    Code version: 1
  *    Availability: https://stackoverflow.com/questions/15476717/how-to-query-a-server-and-get-the-mx-a-ns-records
***************************************************************************************
***************************************************************************************
  *    Title: res_query(3) - Linux man page
  *    Availability: https://linux.die.net/man/3/res_query
***************************************************************************************
***************************************************************************************
  *    Title: How to query a server and get the MX, A, NS records
  *    Author: user of stackoverflow with nickname "Dima00782" -> https://stackoverflow.com/users/1818004/dima00782
  *    Author: user of stackoverflow with nickname "Code Painters" -> https://stackoverflow.com/users/483173/code-painters
  *    Date: 18.3.2013
  *    Code version: 1
  *    Availability: https://stackoverflow.com/questions/15476717/how-to-query-a-server-and-get-the-mx-a-ns-records
***************************************************************************************
***************************************************************************************
  *    Title: make a reverse ipv6 for DNSBL in c++
  *    Author: user of stackoverflow with nickname "Michael Hampton" -> https://stackoverflow.com/users/1068283/michael-hampton
  *    Date: 14.3.2017
  *    Code version: 1
  *    Availability: https://stackoverflow.com/questions/42774904/make-a-reverse-ipv6-for-dnsbl-in-c
***************************************************************************************
***************************************************************************************
  *    Title: Reverse DNS lookup
  *    Availability: https://en.wikipedia.org/wiki/Reverse_DNS_lookup
***************************************************************************************
*/
int DNSConnect(struct input_data i_data, bool entered_dns, bool reverse_lookup)
{
    printf("=== DNS ===\n");

    res_init();

    if (entered_dns == true) // we entered DNS server, overwrite the structure
    {
      _res.nsaddr_list[0].sin_addr.s_addr = inet_addr(i_data.dns_ipv4.c_str());
      _res.nscount = 1;
    }

    if (reverse_lookup == true) // provide reverse DNS lookup for IPv6 or IPv4
    {
      string reverse_request = "";
      if (!i_data.scanned_ipv6.empty())
      {
        // SOURCE: https://stackoverflow.com/questions/42774904/make-a-reverse-ipv6-for-dnsbl-in-c
        struct in6_addr addr;
        inet_pton(AF_INET6,i_data.scanned_ipv6.c_str(),&addr);
        char str2[48];
        string ipv6_hexa_i;

        sprintf(str2,"%02x.%02x.%02x.%02x.%02x.%02x.%02x.%02x.%02x.%02x.%02x.%02x.%02x.%02x.%02x.%02x",
        addr.s6_addr[15], addr.s6_addr[14],
        addr.s6_addr[13], addr.s6_addr[12],
        addr.s6_addr[11], addr.s6_addr[10],
        addr.s6_addr[9], addr.s6_addr[8],
        addr.s6_addr[7], addr.s6_addr[6],
        addr.s6_addr[5], addr.s6_addr[4],
        addr.s6_addr[3], addr.s6_addr[2],
        addr.s6_addr[1], addr.s6_addr[0]);
        string retorno = str2;
        for (unsigned int i = 0; i < retorno.length(); ++i)
        {
          if (retorno[i] == '.')
          {
            retorno[i] = '\n';
          }
        }
        stringstream ipv6_reverse_stream{retorno};
        string reverse_ipv6 = "";
        while (getline(ipv6_reverse_stream, ipv6_hexa_i))
        {
          reverseStringInPlace(ipv6_hexa_i);
          string hexa1 = ipv6_hexa_i.substr(0,1);
          string hexa2 = ipv6_hexa_i.substr(1);
          reverse_ipv6 = reverse_ipv6 + "." + hexa1 + "." + hexa2;

        }

        reverse_ipv6 = reverse_ipv6.substr(1);
        reverse_request = reverse_ipv6 + ".ip6.arpa";
      }

      else if (!i_data.scanned_ipv4.empty())
      {
      string input_ip = i_data.scanned_ipv4;
      string oct1 = input_ip.substr(0, input_ip.find("."));
      string oct2 = input_ip.substr(input_ip.find(".") + 1, input_ip.find("."));
      input_ip = input_ip.substr(input_ip.find(".") + 1);
      input_ip = input_ip.substr(input_ip.find(".") + 1);
      string oct3 = input_ip.substr(0, input_ip.find("."));
      string oct4 = input_ip.substr(input_ip.find(".") + 1);

      reverse_request = oct4 + "." + oct3 + "." + oct2 + "." + oct1 + ".in-addr.arpa";
      }

      ///////////////// PTR ////////////////////////////////////////////////
      u_char buffer_ptr[DNS_BUFFER_LENGTH];
      ns_msg msg_ptr;
      int query_ptr = res_query(reverse_request.c_str(), ns_c_in, ns_t_ptr, buffer_ptr, sizeof(buffer_ptr));
      ns_initparse(buffer_ptr, query_ptr, &msg_ptr);

      ns_rr rr_ptr;

      int msg_count_ptr = ns_msg_count(msg_ptr, ns_s_an);


      for (int i = 0; i < msg_count_ptr; i++)
      {
          ns_parserr(&msg_ptr,ns_s_an,i,&rr_ptr);
          if (ns_rr_type(rr_ptr) == ns_t_ptr)
          {
            char result_ptr[DNS_BUFFER_LENGTH];
            ns_sprintrr(&msg_ptr, &rr_ptr, NULL, NULL, result_ptr, sizeof(result_ptr));
            string result_ptr_string = result_ptr;
            result_ptr_string = result_ptr_string.substr(result_ptr_string.find("PTR") + 3);
            result_ptr_string = result_ptr_string.substr(0, result_ptr_string.size()-1);
            result_ptr_string = TrimWhitespaces(result_ptr_string);
            i_data.scanned_hostname = result_ptr_string;
          }
      }

    }

    ///////////////////////////// A //////////////////////////////////////
    u_char buffer_a[DNS_BUFFER_LENGTH];
    ns_msg msg_a;
    int query_a = res_query(i_data.scanned_hostname.c_str(), ns_c_in, ns_t_a, buffer_a, sizeof(buffer_a)); // make a query, specify our searched hostname
    ns_initparse(buffer_a, query_a, &msg_a); // first routine, has to be called

    ns_rr rr_a;

    int msg_count_a = ns_msg_count(msg_a, ns_s_an);
    for (int i = 0; i < msg_count_a; i++) // cycle through records
    {
        ns_parserr(&msg_a,ns_s_an,i,&rr_a); // extracts information about record

        if (ns_rr_type(rr_a) == ns_t_a) // find relevant record
        {
          struct in_addr result_a_struct;
          memcpy(&result_a_struct.s_addr, ns_rr_rdata(rr_a), sizeof(result_a_struct.s_addr)); // copy memory block to our destination
          char *result_a = inet_ntoa(result_a_struct);
          string result_a_string = result_a; // conversion to string
          result_a_string = TrimWhitespaces(result_a_string);
          cout << "A:              " + result_a_string + "\n";
        }
    }

    ///////////////////////////// AAAA ////////////////////////////////
    u_char buffer_aaaa[DNS_BUFFER_LENGTH];
    ns_msg msg_aaaa;
    int query_aaaa = res_query(i_data.scanned_hostname.c_str(), ns_c_in, ns_t_aaaa, buffer_aaaa, sizeof(buffer_aaaa));
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
          string result_aaaa_string = result_aaaa;
          result_aaaa_string = TrimWhitespaces(result_aaaa_string);
          cout << "AAAA:           " + result_aaaa_string + "\n";
        }
    }

    ///////////////////////////// SOA ////////////////////////////////
    u_char buffer_soa[DNS_BUFFER_LENGTH];
    ns_msg msg_soa;
    int query_soa = res_query(i_data.scanned_hostname.c_str(), ns_c_in, ns_t_soa, buffer_soa, sizeof(buffer_soa));
    ns_initparse(buffer_soa, query_soa, &msg_soa);
    string soa_message;

    ns_rr rr_soa;

    int msg_count_soa = ns_msg_count(msg_soa, ns_s_an);


    for (int i = 0; i < msg_count_soa; i++)
    {
        ns_parserr(&msg_soa,ns_s_an,i,&rr_soa);
        if (ns_rr_type(rr_soa) == ns_t_soa)
        {
          char result_soa[DNS_BUFFER_LENGTH];
          ns_sprintrr(&msg_soa, &rr_soa, NULL, NULL, result_soa, sizeof(result_soa));
          string result_soa_string = result_soa;
          result_soa_string = result_soa_string.substr(result_soa_string.find("SOA"), result_soa_string.find("("));
          result_soa_string = result_soa_string.substr(4, result_soa_string.find("("));
          result_soa_string = result_soa_string.substr(0, result_soa_string.find("("));

          for (unsigned int i = 0; i < result_soa_string.length(); ++i)
          {
            if (result_soa_string[i] == ' ')
            result_soa_string[i] = '\n';
          }
          stringstream soa_stream{result_soa_string};
          while (getline(soa_stream, soa_message))
          {
            soa_message = soa_message.substr(0, soa_message.size()-1);
            soa_message = TrimWhitespaces(soa_message);
            if (soa_message == "")
            {
              continue;
            }
            cout << "SOA:            " + soa_message + "\n";
          }
        }
    }

    /////////////////////// MX ///////////////////////////////////////

    u_char buffer_mx[DNS_BUFFER_LENGTH];
    ns_msg msg_mx;
    int query_mx = res_query(i_data.scanned_hostname.c_str(), ns_c_in, ns_t_mx, buffer_mx, sizeof(buffer_mx));
    ns_initparse(buffer_mx, query_mx, &msg_mx);

    ns_rr rr_mx;

    int msg_count_mx = ns_msg_count(msg_mx, ns_s_an);


    for (int i = 0; i < msg_count_mx; i++)
    {
        ns_parserr(&msg_mx,ns_s_an,i,&rr_mx);
        if (ns_rr_type(rr_mx) == ns_t_mx)
        {
          char result_mx[DNS_BUFFER_LENGTH];
          ns_sprintrr(&msg_mx, &rr_mx, NULL, NULL, result_mx, sizeof(result_mx));
          string result_mx_string = result_mx;
          result_mx_string = result_mx_string.substr(result_mx_string.find("MX"));
          result_mx_string = result_mx_string.substr(5);
          result_mx_string = result_mx_string.substr(0, result_mx_string.size()-1);
          result_mx_string = TrimWhitespaces(result_mx_string);
          cout << "MX:             " + result_mx_string + "\n";
        }
    }

    ///////////////////////////////// CNAME ////////////////////////////////////////
    u_char buffer_cname[DNS_BUFFER_LENGTH];
    ns_msg msg_cname;
    int query_cname = res_query(i_data.scanned_hostname.c_str(), ns_c_in, ns_t_cname, buffer_cname, sizeof(buffer_cname));
    ns_initparse(buffer_cname, query_cname, &msg_cname);

    ns_rr rr_cname;

    int msg_count_cname = ns_msg_count(msg_cname, ns_s_an);


    for (int i = 0; i < msg_count_cname; i++)
    {
        ns_parserr(&msg_cname,ns_s_an,i,&rr_cname);
        if (ns_rr_type(rr_cname) == ns_t_cname)
        {
          char result_cname[DNS_BUFFER_LENGTH];
          ns_sprintrr(&msg_cname, &rr_cname, NULL, NULL, result_cname, sizeof(result_cname));
          string result_cname_string = result_cname;
          result_cname_string = result_cname_string.substr(result_cname_string.find("CNAME"));
          result_cname_string = result_cname_string.substr(5);
          result_cname_string = result_cname_string.substr(0, result_cname_string.size()-1);
          result_cname_string = TrimWhitespaces(result_cname_string);
          cout << "CNAME:          " + result_cname_string + "\n";
        }
    }

    ////////////////////////////////// NS ///////////////////////////////////////
    u_char buffer_ns[DNS_BUFFER_LENGTH];
    ns_msg msg_ns;
    int query_ns = res_query(i_data.scanned_hostname.c_str(), ns_c_in, ns_t_ns, buffer_ns, sizeof(buffer_ns));
    ns_initparse(buffer_ns, query_ns, &msg_ns);

    ns_rr rr_ns;

    int msg_count_ns = ns_msg_count(msg_ns, ns_s_an);


    for (int i = 0; i < msg_count_ns; i++)
    {
        ns_parserr(&msg_ns,ns_s_an,i,&rr_ns);
        if (ns_rr_type(rr_ns) == ns_t_ns)
        {
          char result_ns[DNS_BUFFER_LENGTH];
          ns_sprintrr(&msg_ns, &rr_ns, NULL, NULL, result_ns, sizeof(result_ns));
          string result_ns_string = result_ns;
          result_ns_string = result_ns_string.substr(result_ns_string.find("NS"));
          result_ns_string = result_ns_string.substr(2);
          result_ns_string = result_ns_string.substr(0, result_ns_string.size()-1);
          result_ns_string = TrimWhitespaces(result_ns_string);
          cout << "NS:         " + result_ns_string + "\n";
        }
    }

    ///////////////// PTR ////////////////////////////////////////////////
    if ((!i_data.scanned_hostname.empty()) && (reverse_lookup == true))
    {
      cout << "PTR:            " + i_data.scanned_hostname + "\n";
    }

  return 0;
}


/*
*** MAIN ***
*/
int main(int argc, char **argv)
{
  int opt;
  string ipq_input;
  string whois_input;
  string dns_input;
  bool q = false; // compulsory option
  bool w = false; // compulsory option
  bool d = false; // optional option
  string input_validate_ipq;
  string input_validate_whois;
  string input_validate_dns;
  struct input_data i_data;
  string ipq_converted;
  string whois_converted;
  string dns_converted;
  string whois_server_response;
  string dns_server_response;
  bool reverse_lookup = false;

  if ((argc == 2) && (strcmp(argv[1], "--help") == 0))
  {
    cout << "Whois whisperer - HELP\n";
    cout << "Project finds information about given hostname or IP with use of WHOIS server and DNS server\n";
    cout << "Run project with these arguments:\n";
    cout << "./isa-tazatel -q INPUT_ADDRESS [-d DNS_SERVER] -w WHOIS_SERVER\n";
    cout << "INPUT_ADDRES is IPv4, IPv6 or hostname\n";
    cout << "DNS_SERVER is IPv4. Optional argument, default is DNS server configured in operation system\n";
    cout << "WHOIS_SERVER is IPv4, IPv6 or hostname\n";
    exit(EXIT_SUCCESS);

  }

  if ((argc < 5 ) || (argc > 7))
  {
        cerr << "Error - Wrong count of arguments!\n";
        exit(EXIT_FAILURE);
  }
  // validate parametres
  while ((opt = getopt (argc, argv, ":q:w:d:")) != -1)
  {
    switch (opt)
    {
      case 'q':
        ipq_input = optarg;
        q = true;
      break;

      case 'w':
        whois_input = optarg;
        w = true;
      break;

      case 'd':
        dns_input = optarg;
        d = true;
      break;

      default:
        cerr << "Error - Bad parametres!\n";
        exit(EXIT_FAILURE);
      break;

    }
  }

    // check mandatory parameters
    if ((w == false) || (q == false))
    {
      cerr << "Error - Missing mandatory options!\n";
      exit(EXIT_FAILURE);
    }

    // filled structure correctly with IPv4, IPv6 or hostname
    // validate -q input
    input_validate_ipq = IpValidate(ipq_input);
    if (input_validate_ipq == "ipv6_input" )
    {
      i_data.scanned_ipv6 = ipq_input;
    }
    else if ( input_validate_ipq == "ipv4_input" )
    {
      i_data.scanned_ipv4 = ipq_input;
    }
    else
    {
      i_data.scanned_hostname = ipq_input;
      ipq_converted = ConvertHostname(ipq_input);
      input_validate_ipq = IpValidate(ipq_converted);
      if (input_validate_ipq == "ipv6_input" )
      {
        i_data.scanned_ipv6 = ipq_converted;
      }
      else if ( input_validate_ipq == "ipv4_input" )
      {
        i_data.scanned_ipv4 = ipq_converted;
      }
    }

    // validate -w input
    input_validate_whois = IpValidate(whois_input);
    if (input_validate_whois == "ipv6_input" )
    {
      i_data.whois_ipv6 = whois_input;
    }
    else if ( input_validate_whois == "ipv4_input" )
    {
      i_data.scanned_ipv4 = whois_input;
    }
    else
    {
      i_data.whois_hostname = whois_input;
      whois_converted = ConvertHostname(whois_input);
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

    // validate -d input
    if (d == true)
    {
      input_validate_dns = IpValidate(dns_input);
      if (input_validate_dns == "ipv6_input" )
      {
        i_data.dns_ipv6 = dns_input;
        i_data.dns_hostname = ConvertIPv6toHostname(i_data, i_data.dns_ipv6);
        i_data.dns_ipv4 = ConvertHostname(i_data.dns_hostname);

      }
      else if ( input_validate_dns == "ipv4_input" )
      {
        i_data.dns_ipv4 = dns_input;
      }
      else
      {
        cerr << "Error - Enter only IPv4 or IPv6 address for DNS!\n";
        exit(EXIT_FAILURE);
      }
  }

    // convert addresses to IPV6 if neccessary
    if (!((i_data.scanned_ipv6).empty()) ||  !((i_data.whois_ipv6).empty())  ||  !((i_data.dns_ipv6).empty()))
    {
      if (!(i_data.scanned_hostname).empty())
      {
        i_data.scanned_ipv6 = ConvertHostnameIPv6(i_data.scanned_hostname);
      }
      if (!(i_data.whois_hostname).empty())
      {
        i_data.whois_ipv6 = ConvertHostnameIPv6(i_data.whois_hostname);
      }
      if (!(i_data.dns_hostname).empty())
      {
        i_data.dns_ipv6 = ConvertHostnameIPv6(i_data.dns_hostname);
      }

    }

    // convert hostname to IPV4 if neccessary
    if ((i_data.scanned_hostname).empty())
    {
      reverse_lookup = true;
    }

    DNSConnect(i_data, d, reverse_lookup);

    if (!(i_data.whois_ipv6).empty())
    {
      whois_server_response = WHOISConnectIPv6(i_data);
      WHOISParentServerIPv6(i_data, whois_server_response);
    }
    else if (!(i_data.whois_ipv4).empty())
    {
      whois_server_response = WHOISConnectIPv4(i_data);
      WHOISParentServerIPv4(i_data, whois_server_response);
    }


}
