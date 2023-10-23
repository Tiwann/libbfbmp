#include "bfbmp/bfbmp.hpp"
#include <iostream>


int main()
{
    const std::string input_file(R"(C:\Users\Tiwann\Desktop\Danganronpa.bfbmp)");
    const std::string output_file(R"(C:\Users\Tiwann\Desktop\Danganronpa_encoded.bfbmp)");
    
    bfbmp::beatmap beatmap;
    if(!beatmap.decode(input_file))
    {
        std::cerr << "Failed to read bfbmp file!\n";
        return -1;
    }
    
    if(!beatmap.encode(output_file))
    {
        std::cerr << "Failed to encode bfbmp to file " << output_file << "!\n";
        return -1;
    }

    if(beatmap.decode(output_file))
    {
        std::cout << "This is a success!\n";
    }
    return 0;
}
