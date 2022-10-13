#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <cstring>
#include <bitset>
#include <algorithm>

void func(std::vector<std::string> arg);

int main() {
    std::string a;
    std::vector<std::string> mode;
    std::cout << "Enter mode (encryption/decryption): " << std::endl;
    std::cin >> a;
    if (a == "encryption" || a == "decryption") {
        mode.push_back(a);
        std::cout << "Enter filename: " << std::endl;
        std::cin >> a;
        mode.push_back(a);
        func(mode);
    } else {
        std::cerr << "Error: Write correct mode!" << std::endl;
    }
    return 0;
}

void func(std::vector<std::string> arg) {
    const std::string mode = arg[0];
    const std::string file_namearg = arg[1];
    if (mode == "encryption") {
        std::string enc1, enc2;
        int pass1;
        std::cout << "Please, enter string which you want to encrypt" << std::endl;
        std::cin >> enc1;
        getline(std::cin, enc2);
        enc1 += enc2;
        if (enc1.size() > 248) std::cerr << "Error: Write correct string (<=248)" << std::endl;
        else {
            std::cout << "Please, enter password: " << std::endl;
            if (std::cin >> pass1) {
                srand(time(nullptr));
                int a = rand() % 9000 + 1000;
                std::string key1 = std::to_string(a);
                std::string pass = std::to_string(pass1);
                char key = key1[2];
                std::bitset<8> m(key);
                std::ofstream ofin;
                ofin.open(file_namearg, std::ofstream::out | std::ofstream::trunc);
                if (ofin) {
                    for (char &i: enc1) {
                        std::bitset<8> x(i);
                        x = x ^ m;
                        x = (x >> 2) | (x << 6);
                        unsigned long ii = x.to_ulong();
                        auto c = static_cast<unsigned char>(ii);
                        i = c;
                        ofin << i;
                    }
                    char passw = 'X';
                    for (char &pas: pass) {
                        std::bitset<8> x(pas);
                        std::bitset<8> y(passw);
                        x = x ^ y;
                        x = (x >> 2) | (x << 6);
                        unsigned long ii = x.to_ulong();
                        auto c = static_cast<unsigned char>(ii);
                        pas = c;
                    }
                    ofin << "PS01" << pass;
                    for (char &i: key1) {
                        std::bitset<8> x(i);
                        std::bitset<8> y(passw);
                        x = x ^ y;
                        x = (x >> 2) | (x << 6);
                        unsigned long ii = x.to_ulong();
                        auto c = static_cast<unsigned char>(ii);
                        i = c;
                    }
                    ofin << "NIx0" << key1;
                    std::cout << "Encryption finished!" << std::endl;
                    ofin.close();
                } else std::cerr << "Error: Write correct filename" << std::endl;
            } else std::cerr << "Error: Write correct password" << std::endl;
        }
    } else if (mode == "decryption") {
        std::ifstream ifin;
        ifin.open(file_namearg);
        if (ifin) {
            std::string dec;
            std::getline(ifin, dec);
            char pass = 'X';
            std::bitset<8> y(pass);
            std::string key;
            std::string password;
            std::string password_check;
            size_t pos1 = dec.find("PS01");
            size_t pos2 = dec.find("NIx0");
            size_t m = dec.size();
            for (size_t i = pos1 + 4; i < pos2; i++) {
                std::bitset<8> x(dec[i]);
                x = (x << 2) | (x >> 6);
                x = x ^ y;
                unsigned long ii = x.to_ulong();
                auto c = static_cast<unsigned char>(ii);
                password += c;
            }
            for (size_t i = pos2 + 4; i < m; i++) {
                std::bitset<8> x(dec[i]);
                x = (x << 2) | (x >> 6);
                x = x ^ y;
                unsigned long ii = x.to_ulong();
                auto c = static_cast<unsigned char>(ii);
                key += c;
            }
            dec.erase(dec.begin() + pos1, dec.end());
            std::cout << "Enter password: " << std::endl;
            std::cin >> password_check;
            if (password_check == password) {
                std::bitset<8> y1(key[2]);
                for (char & i : dec) {
                    std::bitset<8> x(i);
                    x = (x << 2) | (x >> 6);
                    x = x ^ y1;
                    unsigned long ii = x.to_ulong();
                    auto c = static_cast<unsigned char>(ii);
                    i = c;
                }
                std::cout << "Decryption finished! Your phrase is: " << dec << std::endl;
            } else std::cerr << "Error: Write correct password" << std::endl;
        } else std::cerr << "Error: Write correct filename" << std::endl;
    }
}
