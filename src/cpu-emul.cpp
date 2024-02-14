#include "cpu-emul.hpp"
#include <Stack/Stack.hpp>

using namespace std;

int main()
{
    stack::Stack<std::string> a;
    a.push("21");
    std::cout << a.top() << std::endl;
    std::string n{ "12" };
    a.push(n);
    std::cout << a.top() << std::endl;
    a.pop();
    std::cout << a.top() << std::endl;
	return 0;
}
