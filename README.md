## Known Limitations

The check is blinded by `std::make_unique`.
Example: the following should produce a warning, but currently does not

```c++
#include <filesystem>
#include <string>
#include <memory>

class C
{
public:
    C(const std::string&) {}
};

int main()
{
    std::filesystem::path p("p");
    std::make_unique<C>(p);  // Should warn here
}
```
