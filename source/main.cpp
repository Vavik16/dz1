#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <cstring>
#include <bitset>

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
        std::ifstream ifin;
        ifin.open(file_namearg);
        if (ifin) {
            std::string enc1;
            std::getline(ifin, enc1);
            ifin.close();
            srand(time(0));
            int a = rand()%9000 + 1000;
            std::string key1 = std::to_string(a);
            char key = key1[2];
            std::bitset<8> m(key);
            std::ofstream ofin;
            ofin.open(file_namearg, std::ofstream::out | std::ofstream::trunc);
            for (size_t i = 0; i < enc1.size(); i++) {
                std::bitset<8> x(enc1[i]);
                x = x ^ m;
                x = (x >> 2) | (x << 6);
                unsigned long ii = x.to_ulong();
                unsigned char c = static_cast<unsigned char>(ii);
                enc1[i] = c;
                ofin << enc1[i];
            }
            char pass = 'X';
            for (size_t i = 0; i < key1.size(); i++) {
                std::bitset<8> x(key1[i]);
                std::bitset<8> y(pass);
                x = x ^ y;
                x = (x >> 2) | (x << 6);
                unsigned long ii = x.to_ulong();
                unsigned char c = static_cast<unsigned char>(ii);
                key1[i]=c;
            }
            ofin << key1;
            std::cout << "Encryption finished!" << std::endl;
            ofin.close();
        } else std::cerr << "Error: Write correct filename" << std::endl;
    } else if (mode == "decryption") {
        std::ifstream ifin;
        ifin.open(file_namearg);
        if (ifin) {
            std::string dec;
            std::getline(ifin, dec);
            char pass = 'X';
            std::bitset<8> y(pass);
            std::string key;
            int m = dec.size();
           for(int i = m-1; i >= m-4; --i) {
                std::bitset<8> x(dec[i]);
                x = (x << 2) | (x >> 6);
                x = x ^ y;
                unsigned long ii = x.to_ulong();
                unsigned char c = static_cast<unsigned char>(ii);
                key += c;
                dec.erase(i);
            }
            std::bitset<8> y1(key[1]);
            std::ofstream ofin;
            ofin.open(file_namearg, std::ofstream::out | std::ofstream::trunc);
            for (size_t i = 0; i < dec.size(); i++) {
                std::bitset<8> x(dec[i]);
                x = (x << 2) | (x >> 6);
                x = x ^ y1;
                unsigned long ii = x.to_ulong();
                unsigned char c = static_cast<unsigned char>(ii);
                dec[i] = c;
                ofin << dec[i];
            }
            std::cout << "Decryption finished! Your phrase is: " << dec << " Also you can see it in your file!" << std::endl;
        } else std::cerr << "Error: Write correct filename" << std::endl;
    }
}