1 задача
Написать метод, который заменит все пробелы в строке на "%20" inplace.
На вход подается строка с зарезервированными под расширение символами (гарантируется, что resize до разумных размеров не будет аллоцировать память).

-----------------------
void urlify(std::string& s);

std::string s0 = "my url";
s0.reserve(20);
urlify(s0);
assert(s0 == "my%20url");
-------------------

void urlify(std::string& s)
{
    int count = 0;
    for (const auto& c : s)
    {
        if (c == ' ')
        {
            ++count;
        }
    }
    
    int i = s.length() - 1;
    s.resize(s.length() + 1 + count * 2, '');
    
    int j = s.length() - 1;
    while (i >= 0)
    {
        if (s[i] == ' ')
        {
            s[j--] = '0';
            s[j--] = '2';
            s[j--] = '%';
            --i;
        }
        else
        {
            s[j--] = s[i--];
        }
    }
}
