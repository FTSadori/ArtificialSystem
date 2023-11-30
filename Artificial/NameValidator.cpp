#include "NameValidator.h"

namespace Memory
{
    bool NameValidator::is_name_valid(const std::string& name)
    {
        for (char c : name)
            if (c == '/' || c == '\\' || c == '|' || c == '*'
                || c == '?' || c == '"' || c == '<' || c == '>'
                || c == ':')
                return false;
        if (name == "..")
            return false;
        return true;
    }
}
