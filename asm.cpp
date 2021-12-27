#include <iostream>
#include <sstream>
#include <cstdlib>
//#include <format>

//#include <stdio.h>
//#include <stdlib.h>
//#include <conio.h>

// declarations
std::string hex2ascii(std::string& strhex);
std::string my_itoa(int i);


using namespace std;

int main() {
    //std::cout << "Hello, Assembly!";
    std::cout << "Go1\n" << std::flush;


    // standard inline assembly
    //asm ("movl $0, %eax\n\t" // 
    //     "cpuid\n\t"         // 
    //     "");

    // Results:
    // eax: 20         0x14       Maximum Input Value for Basic CPUID Information.
    // ebx: 1970169159 0x756e6547 "uneG" 
    // ecx: 1818588270 0x6c65746e "letn"
    // edx: 1231384169 0x49656e69 "Ieni"
    // ebx,edx,ecx => "GenuineIntel"

    // ebx: 756e6547   uneG
    // ecx: 6c65746e   letn
    // edx: 49656e69   Ieni

    // int a=0, b;
    // asm ("movl %1, %%eax\n\t"
    //      "cpuid\n\t"
    //      "movl %%ebx, %0\n\t"
    //      :"=r"(b)        /* output */ // %0
    //      :"r"(a)         /* input */  // %1
    //      :"%eax", "%ebx"         /* clobbered register */
    //      );

    // extended inline assembly
    // HELL: Why can't we get a proper ecx out???
    // OKOK: it happens when we indicate ecx as a clobbered register
    // Still flakey.
    // NONO: We flipped %0 and %1

    // Get ebx ecx adx all in one go
    // NOTE: when using "=r" we get wrong output ("uneG" for all)
    // Use "=m" or "=rm"
    // See: https://lists.llvm.org/pipermail/llvm-dev/2018-November/127968.html
    // GCC-style inline assembly is notoriously hard to write correctly, because it is
    // the user's responsibility to tell the compiler about the requirements of the
    // assembly (inputs, output, modified registers, memory access), and getting this
    // wrong results in silently generating incorrect code. This is also dependent on
    // register allocation and scheduling decisions made by the compiler, so an inline
    // assembly statement may appear to work correctly, then silently break when
    // another change to the code or compiler upgrade causes those decisions to
    // change.
    int ai=0, a, b, c, d;
    asm ("movl %4, %%eax\n\t"
         "cpuid\n\t"
         "movl %%eax, %0\n\t"
         "movl %%ebx, %1\n\t"
         "movl %%ecx, %2\n\t"
         "movl %%edx, %3\n\t"
         :"=rm"(a), "=rm"(b), "=rm"(c), "=rm"(d)  /* output */ // %0 %1 %2 %3
         :"r"(ai)     /* input */  // %4
         :"%eax"     /* clobbered register */
         );

    int a_num = a;
    std::cout << "eax: " << std::hex << a_num << "h\t" << std::dec << a_num << "\n";
    
    std::string strbhex = my_itoa(b);
    std::cout << "ebx: " << strbhex << "\t" << hex2ascii(strbhex) << "\n";
    
    std::string strchex = my_itoa(c);
    std::cout << "ecx: " << strchex << "\t" << hex2ascii(strchex) << "\n";

    std::string strdhex = my_itoa(d);
    std::cout << "edx: " << strdhex << "\t" << hex2ascii(strdhex) << "\n";

    

    // standard inline assembly
    asm ("movl $0, %eax\n\t" // 
         "cpuid\n\t"         // 
         "");    

    // TEST: only after the inline assembly our registers will still be intact
    // after cpuid. Looking good at breakpoint here.
    int dummy = 0;

    //std::string strahex = my_itoa(a);
    
   

    std::cout << std::flush;

    //std::cout << "rbx: " << std::hex << b << "\n" << std::flush;

    std::cout << "Go2\n";     
}


// turn hex string into ascii characters
std::string hex2ascii(std::string& strhex)
{
    int len = strhex.length();
    std::string newString;
    for(int i=0; i<len; i+=2)
    {
        std::string byte = strhex.substr(i,2);
        char chr = (char)(int)strtol(byte.c_str(), NULL, 16);
        newString.push_back(chr);
    }
    return newString;
}

// turn int into a hex string
std::string my_itoa(int i) 
{
    // Alas, itoa() is non-standard
    //char hex_str[33];
    //std::itoa(b,hex_str,16); 
    //printf("hexadecimal: %s\n",hex_str);
    
    // use C++ stream crap
    std::stringstream sstream;
    sstream << std::hex << i;
    std::string strhex = sstream.str();

    // use new C++ fmt
    // Alas, only in C++20
    //std::string strbhex = std::format("{:x}", b);

    return strhex;
}



/*
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
*/

