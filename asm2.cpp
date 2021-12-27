#include <iostream>
#include <sstream>
#include <cstdlib>
//#include <format>

//#include <stdio.h>
//#include <stdlib.h>
//#include <conio.h>


using namespace std;

int main() {
    //std::cout << "Hello, Assembly!";
    std::cout << "Go1\n" << std::flush;


    // standard inline assembly
    asm ("movl $0, %eax\n\t" // 
         "cpuid\n\t"            // 
         "");

    // Results:
    // rax: 20         0x14       Maximum Input Value for Basic CPUID Information.
    // rbx: 1970169159 0x756E6547 "uneG" 
    // rcx: 1818588270
    // rdx: 1231384169

    int a=0, b;
    asm ("movl %0, %%eax\n\t"
         "cpuid\n\t"
         "movl %%ebx, %1\n\t"
         :"=r"(b)        /* output */
         :"r"(a)         /* input */
         :"%eax", "%ebx"         /* clobbered register */
         );

    // turn int into a hex string
    // Alas, itoa() is non-standard
    //char hex_str[33];
    //std::itoa(b,hex_str,16); 
    //printf("hexadecimal: %s\n",hex_str);
    
    // use C++ stream crap
    std::stringstream sstream;
    sstream << std::hex << b;
    std::string strbhex = sstream.str();

    // use new C++ fmt
    // Alas, only in C++20
    //std::string strbhex = std::format("{:x}", b);


    // turn hex string into ascii characters
    int len = strbhex.length();
    std::string newString;
    for(int i=0; i<len; i+=2)
    {
        std::string byte = strbhex.substr(i,2);
        char chr = (char)(int)strtol(byte.c_str(), NULL, 16);
        newString.push_back(chr);
    }


    std::cout << "rbx: " << strbhex << "\t" << newString << "\n" << std::flush;

    //std::cout << "rbx: " << std::hex << b << "\n" << std::flush;

    std::cout << "Go2\n";     
}



// See: https://stackoverflow.com/questions/3790613/how-to-convert-a-string-of-hex-values-to-a-string

// C++98 guarantees that '0', '1', ... '9' are consecutive.
// It only guarantees that 'a' ... 'f' and 'A' ... 'F' are
// in increasing order, but the only two alternative encodings
// of the basic source character set that are still used by
// anyone today (ASCII and EBCDIC) make them consecutive.
unsigned char hexval(unsigned char c)
{
    if ('0' <= c && c <= '9')
        return c - '0';
    else if ('a' <= c && c <= 'f')
        return c - 'a' + 10;
    else if ('A' <= c && c <= 'F')
        return c - 'A' + 10;
    else abort();
}

void hex2ascii(const string& in, string& out)
{
    out.clear();
    out.reserve(in.length() / 2);
    for (string::const_iterator p = in.begin(); p != in.end(); p++)
    {
       unsigned char c = hexval(*p);
       p++;
       if (p == in.end()) break; // incomplete last digit - should report error
       c = (c << 4) + hexval(*p); // + takes precedence over <<
       out.push_back(c);
    }
}


