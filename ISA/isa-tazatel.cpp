/******************************************************************************/
/*                                                                            */
/* projekt: isa-tazatel                                                       */
/* predmet: ISA                                                               */
/* rok: 2019/2020                                                             */
/* autor: Adam Sedlacek | xsedla1e                                            */
/*                                                                            */
/******************************************************************************/

/*
 * C++ includy
 */
#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <algorithm>
#include <regex>

/*
 * C includy
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/nameser.h>
#include <arpa/inet.h>
#include <resolv.h>

/*
 * Struktura pro funkci na parsovani argumentu.
 * Zaroven slouzi jako pomocna struktura na predani argumentu.
 */
struct Params {
    std::string q = "";
    std::string w = "";
    std::string d = "";
    bool q_switch = false;
    bool w_switch = false;
    bool d_switch = false;
    bool verbose_switch = false;
    bool verbose = false;
};

/*
 * Trida pro ulehceni prace se socketem.
 */
class Socket {
    int sock_fd = 0;

public:
    Socket() { }

    // Navazani spojeni pro dany socket
    // AF_UNSPEC - pokud neni specifikovano bere jak IPv4 tak i IPv6
    void connect(std::string address, std::string service) {
        struct timeval tv;
        tv.tv_sec = 2;
        tv.tv_usec = 0;
        
        struct addrinfo hints = {};
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

        struct addrinfo *addr_info;
        if (getaddrinfo(address.c_str(), service.c_str(), &hints, &addr_info)) {
            throw std::runtime_error("Invalid address");
        }

        int sock_fd = 0;
        for (struct addrinfo *a = addr_info; a != NULL; a = a->ai_next) {

            sock_fd = socket(a->ai_family, a->ai_socktype, a->ai_protocol);
            if (!sock_fd) {
                continue;
            }

            if (setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
                close(sock_fd);
                continue;
            }

            if (setsockopt(sock_fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) < 0) {
                close(sock_fd);
                continue;
            }

            if (::connect(sock_fd, reinterpret_cast<struct sockaddr *>(a->ai_addr), a->ai_addrlen) < 0) {
                close(sock_fd);
                sock_fd = 0;
                continue;
            }

            break;
        }

        freeaddrinfo(addr_info);
        
        if (!sock_fd) {
            throw std::runtime_error("Cannot connect");
        }
        this->sock_fd = sock_fd;
    }

    // Zapis do socketu - poslani dat
    void write(std::vector<char> data) {
        unsigned offset = 0;
        for (;;) {
            int ret = send(sock_fd, data.data() + offset, data.size() - offset, 0); 
            if (ret < 0) {
                throw std::runtime_error("send() failed");
            }
            offset += ret;
            if (offset == data.size()) {
                break;
            }
        }
    }

    void writes(std::string s) {
        write(std::vector<char>(s.begin(), s.end()));
    }

    // Nacita data co nam druha strana posila
    std::vector<char> read() {
        std::vector<char> buffer(1024);
        unsigned offset = 0;
        for (;;) {
            int ret = recv(sock_fd, buffer.data() + offset, buffer.size() - offset, 0);
            if (ret < 0) {
                throw std::runtime_error("recv() failed");
            }
            if (ret == 0) {
                break;
            }
            offset += ret;
            if (offset == buffer.size()) {
                buffer.resize(buffer.size() + 1024);
            }
        }
        buffer.resize(offset);
        return buffer;
    }

    std::string reads() {
        std::vector<char> data = read();
        return std::string(data.begin(), data.end());
    }

    ~Socket() {
        close(sock_fd);
    }
};

/*
 * Pomocna funkce, ktera odmaze vse na strane oddelovace
 *  vcetne oddelovace, string je pozmenem do podoby bez oddelovace
 * 
 * Navraci oddelenou cast bez oddelovace
 */
std::string cut_one(std::string &s, std::string delim) {
    auto pos = s.find(delim);
    if (pos == std::string::npos) {
        std::string ret(s.begin(), s.end());
        s.erase(s.begin(), s.end());
        return ret;
    } else {
        std::string ret(s.begin(), s.begin() + pos);
        s.erase(s.begin(), s.begin() + pos + 1);
        return ret;
    }
}

/*
 * Pomocna funkce, ktera "odsekne" prebytecne mezery
 */
std::string trim_space(std::string s) {
    auto left = s.begin();
    while (*left == ' ' && left < s.end()) {
        left++;
    }
    auto right = s.end() - 1;
    while (*right == ' ' && right >= s.begin()) {
        right--;
    }
    return left < right ? std::string(left, right + 1) : std::string();
}

/*
 * Pomocna funkce, ktera prevede vse na male pismena
 */
std::string to_lowercase(std::string s) {
    for (auto &c : s) { 
        c = std::tolower(c);
    }
    return s;
}

/*
 * Najde REFER server, ktery se nachazi v zaznamu
 * Navraci primo string, ktery je "ocisteny" o zbytecne znaky okolo
 */
std::string find_refer(std::string data) {
    while (!data.empty()) {
        std::string line = cut_one(data, "\n");
        auto field_name = to_lowercase(trim_space(cut_one(line, ":")));
        if (field_name == "refer") {
            return trim_space(line);
        }
    }
    return std::string();
}

/*
 * Vraci nazev statu z ktereho zaznam pochazi
 * 
 * Disclaimer - vetsina zaznamu nema primo "country: X",
 *  proto vraci az 4 zaznam z address, ktery dle testovani
 *  se ukazal jako "vzdy" oznaceni zeme
 */
std::string get_country(std::string data) {
    int i = 1;
    std::regex address ("address:.*", std::regex_constants::icase);
    std::smatch match;

    while (std::regex_search(data, match, address)) {
        if (i == 4) {
            return std::regex_replace(match.str(0), std::regex("address"), "country");
        }
        data = match.suffix().str();
        i++;
    }
    return std::string();
}

/*
 * Vraci celou adresu, tedy dva prvni zaznamy
 */
std::string get_full_address(std::string data) {
    int i = 1;
    std::regex address ("address:.*", std::regex_constants::icase);
    std::smatch match;
    std::string tmp = "";

    while (std::regex_search(data, match, address)) {
        tmp += match.str(0) + '\n';
        if (i == 2) {
            tmp.pop_back();
            return tmp;
        }
        data = match.suffix().str();
        i++;
    }
    return tmp;
}


/*
 * "Byla muj uplne nejvic nejlepsi pritel. Muj jedniny pritel."
 *
 *                                              Forrest Gump
 */
std::string get_value_with_the_help_of_holy_regular_expressions(std::string data, std::string pattern) {
    std::regex address (pattern, std::regex_constants::icase);
    std::smatch match;
    std::regex_search(data, match, address);
    return match.str();
}

/*
 * Parsuje odpoved od serveru
 * Odstraneni prebytecnych \r a \n znaku, totez pro komentare %
 * 
 * Zaroven funkce i projede odpoved danymi regexy, ktere jsou case-insensitive
 * 
 * Obdobne vola pomocne funkce pro ziskani adresy a zeme
 * 
 * Mensi "vedlejsi efekt", tiskne pak rovnou, misto aby se hodnota vratila do whois funkce
 */
void parse_response(std::string data) {
    std::string tmp = "";
    bool comment = false;
    bool new_line = false;
    for (int i = 0; i < data.size(); i++) {
        if (data[i] == '\r' ) {
            comment = false;
            continue;
        }
        if (data[i] == '\n' && new_line) {
            continue;
        }
        if (data[i] != '\n') new_line = false;
        if (data[i] == '\n') new_line = true;
        if (data[i] == '%') comment = true;
        if (comment) continue;
        tmp += data[i];
    }

    if (tmp.empty()) {
        std::cout << "Record not found" << std::endl;
        return;
    }

    std::vector<std::string> whois_info;
    std::vector<std::string> regex_patterns = {
        "domain name:.*", "netname:.*", "inetnum:.*", "descr:.*",
        "country:.*", "admin-c:.*", "phone:.*", "org:.*", "netrange:.*",
        "net range:.*", "telephone:.*", "inet6num:.*", "organisation:.*"
    };

    for (auto pattern : regex_patterns) {
        whois_info.emplace_back(get_value_with_the_help_of_holy_regular_expressions(tmp, pattern));
    }

    whois_info.emplace_back(get_full_address(tmp));
    whois_info.emplace_back(get_country(tmp));

    for (auto each : whois_info) {
        if (!each.empty()) std::cout << each << std::endl;
    }
}

/*
 * Tiskne informace, ktere se ziskaly z daneho whois serveru
 * Vola rekurzivne samo-sebe, pokud v zaznamu se najde REFER
 *  primarne se bere zaznam od refera
 *
 * Verbose vytiskne se tak, jak byla odpoved serveru
 *  pokud neni zadano, tak se projete regexy a snazi se najit pouze
 *  informace, ktere maji pro uzivatele smysl
 */
void whois(std::string lookup_name, std::string whois_server, bool verbose) {
    std::cout << "[Querying " << whois_server << "]" << std::endl; 
    
    Socket sock;
    sock.connect(whois_server, "whois");
    sock.writes(lookup_name + "\r\n");
    auto data = sock.reads();
    auto refer = find_refer(data);
    if (!refer.empty()) {
        std::cout << "[Redirected to " << refer << "]" << std::endl;
        whois(lookup_name, refer, verbose);
        return;
    }

    std::cout << "[Response from " << whois_server << "]" << std::endl;
    std::cout << std::endl;
    std::cout << "=== WHOIS ===" << std::endl;
    if (verbose) {
        std::cout << data << std::endl;
    } else {
        parse_response(data);
    }
}

/*
 * Validace zadanych argumentu programu
 * Tisk napovedy, ktera by mela uzivatele nasmerovat na pouziti
 *
 * Prepinace muzou byt zadany v libovolnem poradi
 * Nesmi byt prepinac zadany vicekrat, pokud se tak stane,
 *  je vyvolana vyjimka s prislusnou chybou
 */
Params parameter_validaton(int argc, char *argv[]) {
    Params par_tmp;
    bool err_par = false;

    if (argc == 2) {
        if (std::string(argv[1]) == "-h") {
            std::cout << "[USAGE]" << std::endl;
            std::cout << "-q IP address or hostname - REQUIRED" << std::endl;
            std::cout << "-w IP address or hostname of whois server - REQUIRED" << std::endl;
            std::cout << "-d IP address of DNS - OPTIONAL" << std::endl;
            std::cout << "-v true/false (verbose) - OPTIONAL (default is FALSE)" << std::endl;
            std::cout << "-h for help" << std::endl;
            std::cout << std::endl;
            std::cout << "[EXAMPLE]" << std::endl;
            std::cout << argv[0] << " -q seznam.cz -w whois.nic.cz -v true" << std::endl;
            exit(EXIT_SUCCESS);
        }
    }

    if (argc != 5 && argc != 7 && argc != 9) {
        throw std::runtime_error("Wrong arguments count. Use -h");
    }

    for (int i = 1; i < argc; i += 2) {
        std::string tmp = argv[i];
        std::string tmp2 = argv[i + 1];
        if (tmp == "-d") {
            if (par_tmp.d_switch == true) {
                err_par = true;
            }
            par_tmp.d_switch = true;
            par_tmp.d = tmp2;
        }
        if (tmp == "-w") {
            if (par_tmp.w_switch == true) {
                err_par = true;
            }
            par_tmp.w_switch = true;
            par_tmp.w = tmp2;
        }
        if (tmp == "-q") {
            if (par_tmp.q_switch == true) {
                err_par = true;
            }
            par_tmp.q_switch = true;
            par_tmp.q = tmp2;
        }
        if (tmp == "-v") {
            if (par_tmp.verbose_switch == true) {
                err_par = true;
            }
            par_tmp.verbose_switch = true;
            if (tmp2 == "true") {
                par_tmp.verbose = true;
            } else if (tmp2 != "false") {
                err_par = true;
            }
        }
    }

    if (err_par) {
        throw std::runtime_error("This switch usage is not allowed");
    }

    if (par_tmp.q_switch == false || par_tmp.w_switch == false) {
        throw std::runtime_error("-q and -w is REQUIRED");
    }

    return par_tmp;
}

/*
 * Veskere "tahani dat" z DNS se deje zde
 * Funkce prijima pozadovanou domenu a typ zaznamu, ktery nasledne vrati,
 *  pokud existuje, kdyz ne, navraci prazdny string
 * 
 * Funkce vyuzivat resolvs ze systemu, a "novejsi" Nkovou knihovnu,
 *  která zajišťuje bezpečnou práci i v multithreadu
 * 
 * Inspirace jak dane funkce pracuji:
 * https://docstore.mik.ua/orelly/networking_2ndEd/dns/ch15_02.htm
 * 
 * Pripadne manualove stranky v systemu
 */
std::string dns_query(std::string req_domain, __ns_type type) {
    res_state rstate = static_cast<res_state>(malloc(sizeof(*rstate)));
    if (res_ninit(rstate) != 0) {
        throw std::runtime_error("res_ninit failed");
    }

    const char *domain = req_domain.c_str();
    const int buflen = 4096;
    unsigned char buf[buflen];
    ns_msg msg;
    
    int r = res_nquery(rstate, domain, ns_c_in, type, buf, buflen);
    
    if (r < 0) {
        return std::string();
    }

    if (ns_initparse(buf, r, &msg) < 0) {
        throw std::runtime_error("ns_res_ninit failed");
    }

    int rrnum;
    ns_rr rr;
    constexpr int SIZE = 512;
    char response[SIZE] = {};
    std::string tmp = "";

    for (rrnum = 0; rrnum < ns_msg_count(msg, ns_s_an); rrnum++) {
        if (ns_parserr(&msg, ns_s_an, rrnum, &rr)) {
            throw std::runtime_error("ns_parserr failed");
        }

        switch (ns_rr_type(rr)) {
            case ns_t_a:
                inet_ntop(AF_INET, ns_rr_rdata(rr), response, SIZE);
                tmp += "A:\t" + std::string(response) + '\n';
                break;
            case ns_t_aaaa:
                inet_ntop(AF_INET6, ns_rr_rdata(rr), response, SIZE);
                tmp += "AAAA:\t" + std::string(response) + '\n';
                break;
            case ns_t_ns:
                ns_name_uncompress(buf, &buf[sizeof(buf)], ns_rr_rdata(rr), response, SIZE);
                tmp += "NS:\t" + std::string(response) + '\n';
                break;
            case ns_t_soa:
                ns_name_uncompress(buf, &buf[sizeof(buf)], ns_rr_rdata(rr), response, SIZE);
                tmp += "SOA:\t" + std::string(response);
                break;
            case ns_t_cname:
                ns_name_uncompress(buf, &buf[sizeof(buf)], ns_rr_rdata(rr), response, SIZE);
                tmp += "CNAME:\t" + std::string(response);
                break;
            case ns_t_ptr:
                // TODO dodelat reverse query pro dns ptr
                ns_name_uncompress(buf, &buf[sizeof(buf)], ns_rr_rdata(rr), response, SIZE);
                tmp += "PTR:\t" + std::string(response) + '\n';
                break;
            case ns_t_mx:
                ns_sprintrr(&msg, &rr, NULL, NULL, response, SIZE);
                tmp += "MX:\t" + std::string(response) + '\n';
                break;
        }
    }

    if (rrnum > 1) tmp.pop_back();

    res_nclose(rstate);
    free(rstate);
    return tmp;
}

/*
 * Pomocna funkce, ktera zajistuje tisk vsech pozadovanych DNS zaznamu
 * Pokud dany zaznam chyby, tak se nevytiskne
 */
void dns(std::string dom) {
    std::cout << std::endl;
    std::cout << "=== DNS ===" << std::endl;
    std::vector<std::string> respon;
    std::vector<__ns_type> record_type = {
        ns_t_ns, ns_t_a, ns_t_aaaa, ns_t_cname,
        ns_t_ptr, ns_t_soa, ns_t_mx
    };

    for (auto rec : record_type) {
        respon.emplace_back(dns_query(dom, rec));
    }

    for (auto x : respon) {
        if (!x.empty())
            std::cout << x << std::endl;
    }
}

/*
 * Hlavni funkce, veskera "magie" se skryva pod touto poklickou
 * Overeni argumentu
 * Zavolani whois funkce
 * Zavolani dns funkce
 * V pripade nastani vyjimky, se odchyti a vytiskne na standardni chybovy vystup,
 *  a ukonci se s neuspesnou navratovou hodnotou
 */
int main(int argc, char *argv[]) {
    try {
        Params par = parameter_validaton(argc, argv);
        whois(par.q, par.w, par.verbose);
        dns(par.q);
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}