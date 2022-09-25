#include "Json.h"
#include <fstream>
#include <iostream>
#include <sstream>

int main()
{
    try
    {
        std::ifstream fin("../test.json");
        std::stringstream ss;
        ss << fin.rdbuf();
        
        const std::string data = ss.str();

        myJson::Json jsonTest;
        jsonTest.parse(data);
        printf("%s\n", jsonTest.str().c_str());
    }
    catch (std::exception &e)
    {
        std::cout << "catch exception: " << e.what() << std::endl;
    }

    return 0;
}