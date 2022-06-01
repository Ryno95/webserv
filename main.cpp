#include <sstream>
#include <iostream>

int main(void)
{
    std::stringstream strStream;
    char buffer[BUFSIZ];

    std::cout << strStream.read(buffer, 1) << std::endl;
    
    if (strStream.eof())
        return 3;
    else if (strStream.good())
        return 2;

    
    return (0);
}