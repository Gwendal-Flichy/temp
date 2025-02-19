#include <iostream>

int fetchValueFromDatabase()
{
    std::cout << "Coucou ";

    return 42;
}

int main()
{
    int toto = 5;
    int res = SQUARE(fetchValueFromDatabase());

    std::cout << res << "\n";
    std::cout << toto << "\n";
}
