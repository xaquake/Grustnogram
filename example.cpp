#include <iostream>
#include <string>
#include <curl/curl.h>
#include "grustnogram.hpp"
using namespace std;

int main()
{
    Grustnogram GC;
    GC.login("", "");
    cout << "token: " << GC.token << "\n";
    cout << "id: " << GC.get_me()["id"] << "\n";
    auto posts = GC.get_hands();
    for(string const& x : posts["intros"])
    {
        cout << x << endl;
    }
    return 0;
}
