/**
 * Убрать смайлы из строки in-place
 * Примеры смайлов: ':' + '-' + ( ')' | '(' )+
 *  :-)))))))
 *  :-(((((
 *  :-)
 * Вот тут смайл - ':-)':
 *  :-)(
 */
void ClearString(std::string& s)
{
    size_t i = 0; // write
    size_t j = 0; // scan
    
    while (j < s.size())
    {
        if (s[j] == ':' && j + 2 < s.size())
        {
            // check if it's a smile
            if (s[j+1] == '-' && (s[j + 2] == ')' || s[j + 2] == '('))
            {
                const auto bracket = s[j + 2];
                
                j = j + 3;
                for (; j < s.size() && s[j] == bracket; ++j)
                {
                    // do nothing
                }
                continue;
            }
        }
        s[i++] = s[j++];
    }
    s.resize(i);
}
