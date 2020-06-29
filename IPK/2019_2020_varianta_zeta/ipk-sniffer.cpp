/*
 * Autor:   Adam Sedlacek
 * Login:   xsedla1e
 * Rok:     2019/2020
 * Predmet: IPK
 * Projekt: Sniffer packetu (proj. c. 2)
 * 
 */

// Hlavickove soubory, ktere jsou nezbytne nutne pro spravny chod.
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include "pcap.h"
#include <string.h>
#include <stdlib.h>
#include <netinet/ether.h>
#include <pcap.h>
#include <errno.h>
#include <netinet/tcp.h>
#include <netinet/if_ether.h> 
#include <time.h>
#include <pcap/pcap.h>

// Velikost ETHERNET ramce, vyuzito pozdeji pro kalkulace.
#define SIZE_ETHERNET (14)

/*
 * Promenne, ktere jsou vyuzity hlavne pri parsovani argumentu.
 * Slouzi zaroven k uchovani rozhrani a poctu packetu, ktere se maji zpracovat.
 */
bool udp_opt;
bool tcp_opt;
bool data_only = false;
int port = -1;
std::string nic = "";
int num_packet = 1;

/* 
 * Pomocna funkce pro ukonceni programu s vypisem chybove hlasky.
 * Pokud se nezmeni _msg_ je vyuzito vychozi "usage", jak spravne spoustet program.
 */
void fail(std::string msg="Usage: ./ipk-sniffer -i rozhrani [-p ­­port] [--tcp|-t] [--udp|-u] [-n num]") {
    std::cerr << msg << std::endl;
    exit(1);
}

// Funkce na vytisknuti vsech dostupnych rozhrani pomoci pcap.
void print_interface() {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevsp;
    if (pcap_findalldevs(&alldevsp, errbuf) == -1) {
        fail("Unable to find interface/s.");
    }
    std::cout << "Available interface:" << std::endl;
    while (alldevsp->next != NULL) {
        printf("%s\n", alldevsp->name);
        alldevsp = alldevsp->next;
    }
    exit(0);
}

/*
 * V teto funkci se zpracuje cela radka pri spusteni programu.
 * Dle zadani se vypise bud interface.
 * Muze se zvolit port na kterem se sniffuje.
 * Zvolit interface na kterem se bude sniffovat.
 * Protokol, ktery se ma zpracovat UDP/TCP, vychozi oba.
 * Kolik packetu se ma odchytit, vychozi 1.
 * Pokud uzivatel cokoliv zada spatne je nasledne upozornen.
 * Akceptuji se vicenasobne volby, vzdy se bere posledni.
 */
void parse_arguments(int argc, char *argv[]) {
    std::string tmp;
    if (argc <= 2) {
        if (argc == 2 && std::string(argv[1]) != "-i") {
            fail();
        }
        print_interface();
    }
    if (argc < 3) { fail(); }
    for (int i = 1; i < argc; i++) {
        tmp = std::string(argv[i]);
        if (tmp == "-i") {
            nic = std::string(argv[i + 1]);
            i++;
        } else if (tmp == "--tcp" || tmp == "-t") {
            tcp_opt = true;
        } else if (tmp == "--udp" || tmp == "-u") {
            udp_opt = true;
        } else if (tmp == "-p") {
            try {
                int myInt(std::stoi(std::string(argv[i + 1])));
                i++;
                if (myInt < 0) {
                    fail("Port range is out of index.");
                }
                port = myInt;
            } catch (std::exception) {
                fail("Invalid port.");
            }
        } else if (tmp == "-n") {
            try {
                num_packet = std::stoi(std::string(argv[i + 1]));
                i++;
            } catch (std::exception) {
                fail("Packet number is incorrect.");
            }
            if (num_packet <= 0) { fail("Packet number is incorrect."); }
        } else if (tmp == "data_only") {
            data_only = true;
        } else {
            fail();
        }
    }
    if (nic == "") {
        fail("Missing interface.");
    }
    if (udp_opt == false && tcp_opt == false) {
        udp_opt = true;
        tcp_opt = true;
    }
}

// Pomocna funkce na tisk casu, ktery se ziskava ze systemu.
void print_time() {
    time_t rawtime;
    struct tm *info;
    time(&rawtime);
    info = localtime(&rawtime);
    printf("%02d:%02d:%02d ", info->tm_hour, info->tm_min, info->tm_sec);
}

/* 
 * Pomocna funkce, tiskne data z packetu ve HEXa podobe.
 * Kvuli hezcimu vypisu se vyplni pripadne mezery.
*/
void print_hex(u_char *data, int start, int end) {
    printf(" ");
    for (int i = start; i < end; i++) {
        printf("%02x ", data[i]);
    }
    int rest = end - start;
    if (rest < 16) {
        for (int i = 0; i < 16 - rest; i++) {
            printf("   ");
        }
    }
    printf(" ");
}

/* 
 * Tisk bajtu ascii hodnotou, ktery se vezme z packetu. 
 * Pokud se nejedna o tisknutelny znak nahradi se teckou.
 */
void print_ascii(u_char *data, int start, int end) {
    for(int i = start; i < end; i++) {
        if(isprint(data[i])) {
            printf("%c", data[i]);
        } else {
            printf(".");
        }
    }
}

/*
 * Funkce, ktera zajistuje prostrednika pro tisk dat v zadane forme:
 *  pocet vypsanych bajtu (16 soustava) | data v hexa forme | data v ascii forme.
 *  Pokud nejsou zadna data k dispozici, vytiskne se "No data".
 */
void print_data(u_char *data, int length) {    
    if (length == 0) { 
        printf("No data.\n");
        return; 
    }
    const int S = 16;
    int i = 0;
    printf("\n");
    for (; i < length - S; i += S) {
        printf("0x%04x ", i);
        print_hex(data, i, S + i);
        print_ascii(data, i, S + i);
        printf("\n");
    }
    printf("0x%04x ", length - S);
    print_hex(data, i, length);
    print_ascii(data, i, length);
    printf("\n");
}
/*
 * Pomocna funkce, ktera ziska domenove jmeno z dane adresy.
 * Pokud nelze ziskat, tak se vrati klasicka IP adresa.
 * Pri selhani knihovny se vytiskne chybova hlaska (nemelo by nastat).
 */
std::string get_addr(char *my_ip) {
    struct addrinfo *result;
    struct addrinfo *res;
    int error;
 
    error = getaddrinfo(my_ip, NULL, NULL, &result);
    if (error != 0) {   
        fail("Getaddrinfo() error");
    }   
 
    for (res = result; res != NULL; res = res->ai_next) {   
        char hostname[NI_MAXHOST] = "";
        error = getnameinfo(res->ai_addr, res->ai_addrlen, hostname, NI_MAXHOST, NULL, 0, 0); 
        if (error != 0) {
            fail("Getnameinfo() error");
            continue;
        }
        if (*hostname != '\0')
            freeaddrinfo(result);
            return std::string(hostname);
    }
}

/*
 * Veskere zpracovani packetu se odehrava zde.
 * Vyuziti pcap_loop, moznosti vyuzit callback pri kazdem packetu.
 * Vytiskne se systemovy cas, kdy byl packet odchytnut.
 * Zjisti se zda se jedna o IPv4 (teoreticke rozsireni do budoucnosti o IPv6.
 * Udela se pointerova aritmerika, tak aby se ziskaly data z headeru.
 * Zde se overi ip adresa a prevede na domenovou pokud je to mozne.
 * Podle UDP/TCP se pak zjisti prislusny port a vytiskne se.
 * Zavolani pomocne funkce na tisk dat.
 * 
 * IP hlavicka, i TCP hlavicka maji promenlivou delku.
 * S tim by si pointerova aritmetika mela poradit.
 */
void mypcap_handler(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
    struct ip *my_ip;               // pointr na zacetek IP hlavicky.
    struct ether_header *eptr;      // pointr na ETHERNET ramec, ktery se nevyuziva, 
                                    //  mohlo by se pro MAC adresu napr.
    const struct tcphdr *my_tcp;    // pointr, ktery se pak vyuzije na TCP.
    const struct udphdr *my_udp;    // pointr, ktery se pak vyuzije na UDP.
    u_int size_ip;                  // Pomocna promenna na velikost IP hlavicky.
    u_char *data;                   // Pomocna promenna na odkaz na data.
    u_int size_tcp;                 // Pomocna promenna na vypocet promenlive delky TCP hlavicky.
    std::string src_addr;           // Pomocna promenna na zdrojovou adresu.
    std::string dst_addr;           // Pomocna promenna na cilovou adresu.

    eptr = (struct ether_header *) packet;
    print_time();

    switch (ntohs(eptr->ether_type)){                           // Ziskani ethernet typu, zde se jedna o IP.
    case ETHERTYPE_IP:                                          // IPv4 packet.
        my_ip = (struct ip*)(packet + SIZE_ETHERNET);           // "Preskoceni" ethernet ramce.
        
        src_addr = get_addr(inet_ntoa(my_ip->ip_src));
        dst_addr = get_addr(inet_ntoa(my_ip->ip_dst));
        
        size_ip = my_ip->ip_hl*4;                               // Vypocet promenlive delky IP hlavicky.
        switch (my_ip->ip_p){                                   // Zjisteni prislusneho protocolu.
        case 6: {                                               // 6 - TCP protokol.
            my_tcp = (struct tcphdr *) (packet + SIZE_ETHERNET + size_ip); // Aritmetika pro ziskani TCP ukazatele na hlavicku.
            std::cout << src_addr << " : " << ntohs(my_tcp->th_sport) << " > " << dst_addr << " : " << ntohs(my_tcp->th_dport) << std::endl;
            if (data_only) {
                size_tcp = (my_tcp->doff)*4;                        // Vypocet promenlive delky TCP hlavicky.
                data = (u_char *)(packet + SIZE_ETHERNET + size_ip + size_tcp);
                print_data(data, static_cast<u_int>(ntohs(my_ip->ip_len)) - size_tcp - size_ip);
            } else {
                data = (u_char *)packet;
                print_data(data, header->len);
            }
        } break;
        case 17:                                                 // 17 - UDP protokol.
            my_udp = (struct udphdr *) (packet + SIZE_ETHERNET + size_ip); // Ziskani ukazatele na UDP hlavicku.
            std::cout << src_addr << " : " << ntohs(my_udp->uh_sport) << " > " << dst_addr << " : " << ntohs(my_udp->uh_dport) << std::endl;
            if (data_only) {
                data = (u_char *)(packet + SIZE_ETHERNET + size_ip + 8);    // 8 = Velikost UDP, která není promenliva (vzdy 8).
                print_data(data, ntohs(my_udp->len) - 8);
            } else {
                data = (u_char *)packet;
                print_data(data, header->len);
            }
            break;
        default:
            // TOHLE BY SE NIKDY NEMELO STAT.
            fail("WHAT IS HAPPENING???");
        }
        break;
    /* maybe in the future as bonus
    case ETHERTYPE_IPV6:
        printf("hello ipv6\n");
        break;
    */
    }
    std::cout << std::endl;
}

/*
 * Funkce na vytvoreni filtru pro pcap.
 */
std::string create_filter() {
    std::string filter = "";
    if (port == -1) {
        if (tcp_opt && udp_opt) {
            filter = "tcp || udp";
        } else if (tcp_opt) {
            filter = "tcp";
        } else if (udp_opt) {
            filter = "udp";
        }
    } else {
        std::string p = std::to_string(port);
        if (tcp_opt && udp_opt) {
            filter = "(tcp port " + p + ") || (udp port " + p + ")";
        } else if (tcp_opt) {
            filter = "tcp port " + p;
        } else if (udp_opt) {
            filter = "udp port " + p;
        }
    }
    return filter;
}

/*
 * Hlavni magie se odehrava zde.
 * Zavolani zpracovani argumentu programu.
 * Zjisteni prislusneho filtru pro pcap
 * Veskery main je vesmes jen volani pcap knihovny.
 */
int main(int argc, char *argv[]) {
    parse_arguments(argc, argv);
    std::string filter = create_filter();
    pcap_t *handle;
    char errbuf[PCAP_ERRBUF_SIZE];	    // Error string.
    struct bpf_program fp;		        // Struktura zkompilovaneho filtru (rychlejsi).
    bpf_u_int32 mask;		            // Nase maska.
    bpf_u_int32 net;		            // Nase IP adresa.
    struct pcap_pkthdr header;	        // Header, ktery nam poskytne pcap.
    const u_char *packet;

    // Zjisteni IP adresy a masky pro nas interface.
    if (pcap_lookupnet(nic.c_str(), &net, &mask, errbuf) == -1) {
        fail("Problem with interface and netmask.");
    }
    // Otevre se v promiskuitnim modu.
    handle = pcap_open_live(nic.c_str(), BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) {
        fail("Couldn't be opened interface for sniffing.");
    }
    // Zkompilovani naseho filtru.
    if (pcap_compile(handle, &fp, filter.c_str(), 0, net) == -1) {
        fail("Unexpected fail within compiling filter. Port range from 0 to 65536 only.");
    }
    // Aplikace filtru.
    if (pcap_setfilter(handle, &fp) == -1) {
        fail("Unexpected fail with applying filter.");
    }
    // Smycka pro odchyt packetu, kde pak je callback na nas handler.
    if (pcap_loop(handle, num_packet, mypcap_handler, NULL) == -1) {
        fail("Pcap_loop() has failed");
    }
    // Uvolneni pcap.
    pcap_close(handle);
    return 0;
}