#pragma once
#include <stdio.h>
#include <string.h>
//#include <string>
#include <stdlib.h>
//#include <memory.h>
#include <stdarg.h>
#include <regex>

#define EMPTY ""

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef unsigned int uint;
typedef unsigned char byte;
typedef int8_t int8;
typedef int16_t int16;
typedef int64_t int64;
typedef char32_t ustring;
typedef wchar_t wstring;

namespace libstr
{

class Object
{
  public:
    virtual const char *toString() = 0;
};

class string : public Object
{

    char *str;
    uint size;

  public:
    inline uint length() const { return size; }

    inline string(const char *s) : size(strlen(s))
    {
        uint len = size + 1;
        str = (char *)malloc(len);
        memcpy(str, s, len);
    }

    inline string(const string &s) : size(s.size)
    {
        uint len = size + 1;
        str = (char *)malloc(len);
        memcpy(str, s.str, len);
    }

    inline bool operator==(const char *s) const
    {
        return !strcmp(str, s);
    }

    inline bool operator!=(const char *s)
    {
        return strcmp(str, s);
    }

    inline bool operator==(const string &s) const
    {
        return strcmp(str, s.str) == 0;
    }

    char operator[](uint pos) const
    {
        return size <= pos ? -1 : str[pos];
    }

    char charAt(uint pos) const
    {
        return size <= pos ? -1 : str[pos];
    }

    string &operator+(const char *s)
    {
        size = size + strlen(s);
        str = (char *)realloc(str, size + 1);
        strcat(str, s);
        return *this;
    }

    string &operator+(const string &s)
    {
        size = size + s.size;
        str = (char *)realloc(str, size + 1);
        strcat(str, s.str);
        return *this;
    }

    string &operator+=(const char *s)
    {
        size = size + strlen(s);
        str = (char *)realloc(str, size + 1);
        strcat(str, s);
        return *this;
    }

    string &operator+=(const string &s)
    {
        size = size + s.size;
        str = (char *)realloc(str, size + 1);
        strcat(str, s.str);
        return *this;
    }

    bool isEmpty() const
    {
        return !str;
    }

    int indexOf(const char chr)
    {
        char *pos = strchr(str, chr);
        return pos ? pos - str : -1;
    }

    int indexOf(const char *s)
    {
        char *pos = strstr(str, s);
        return pos ? pos - str : -1;
    }

    int indexOf(const string &s)
    {
        char *pos = strstr(str, s.str);
        return pos ? pos - str : -1;
    }

    string &concat(const char **strings)
    {
        int argc = 0;
        while (*strings++)
        {
            strcat(str, strings[argc]);
            argc++;
        }
        return *this;
    }

    string &substring(uint start, uint end = 0U)
    {

        if (end != 0 && (start > end) || (start > size || end > size))
            return *this;

        if (end == 0)
            end = size;

        const char *p = str + start;
        memmove(str, p, strlen(p) + 1);

        uint len = (end - start) + 1;
        char *tmp = (char *)malloc(len);
        strncpy(tmp, str, len);
        tmp[len] = '\0';
        //free(str);
        //str = tmp;
        memmove(str, tmp, len + 1);
        //free(tmp);

        return *this;
    }

    string **split(const char *separtor)
    {
        //string s = new(4);
        //token = strtok(str, s);

        /* walk through other tokens */
        /*while (token != NULL)
            {
                printf(" %s\n", token);

                token = strtok(NULL, s);
            }*/
        return nullptr;
    }

    string &toLowerCase()
    {
        _strlwr(str);
        return *this;
    }

    string &toUpperCase()
    {
        _strupr(str);
        return *this;
    }

    string &replace(const char *search, char replace)
    {

        char *s = _strdup(str),
             *buffer = NULL,
             *p = NULL;

        uint searchlen = strlen(search),
             replen = 1U;

        if (!(p = strstr(s, search)))
        { // No se encontro nada
            return *this;
        }

        if (searchlen == replen)
        {
            buffer = (char *)malloc(size + 1);
        }
        else if (searchlen > replen)
        {
            buffer = (char *)malloc(size - (searchlen - replen) + 1);
        }
        else
        {
            buffer = (char *)malloc(size + (replen - searchlen) + 1);
        }

        strncpy(buffer, s, p - s); // Copy characters from 'str' start to 'orig' st$
        buffer[p - s] = '\0';

        sprintf(buffer + (p - s), "%c%s", replace, p + strlen(search));

        free(str);
        str = buffer;
        size = strlen(str);
        return *this;
    }

    bool match(const char *search, std::regex_constants::syntax_option_type option = std::regex_constants::ECMAScript | std::regex_constants::icase)
    {
        try
        {
            return std::regex_search(str, std::regex(search, option));
        }
        catch (std::regex_error &e)
        {
            return false;
        }
    }

    bool search(const char *search, std::regex_constants::syntax_option_type option = std::regex_constants::ECMAScript | std::regex_constants::icase)
    {
        try
        {
            return std::regex_search(str, std::regex(search, option));
        }
        catch (std::regex_error &e)
        {
            return false;
        }
    }

    std::smatch imatch(const char *search, std::regex_constants::syntax_option_type option = std::regex_constants::ECMAScript | std::regex_constants::icase)
    {
        try
        {
            std::smatch sm;
            std::string s(str);
            std::regex_search(s, sm, std::regex(search, option));
            return sm;
        }
        catch (std::regex_error &e)
        {
            return std::smatch();
        }
    }

    string &replace(const char *search, const char *replace, int8 count = -1)
    {

        char *buffer = NULL,
             *p = NULL;
    replacebegin:

        uint searchlen = strlen(search),
             replen = strlen(replace);

        if (!(p = strstr(str, search)))
        { // No se encontro nada
            return *this;
        }

        if (searchlen == replen)
        {
            buffer = (char *)malloc(size + 1);
        }
        else if (searchlen > replen)
        {
            buffer = (char *)malloc(size - (searchlen - replen) + 1);
        }
        else
        {
            buffer = (char *)malloc(size + (replen - searchlen) + 1);
        }

        strncpy(buffer, str, p - str); // Copy characters from 'str' start to 'orig' st$
        buffer[p - str] = '\0';

        sprintf(buffer + (p - str), "%s%s", replace, p + strlen(search));

        free(str);
        str = buffer;
        size = strlen(str);

        if (count == -1)
            goto replacebegin;

        if (count > 0)
        {
            count--;
            goto replacebegin;
        }

        return *this;
    }

    string &replaceMatch(const string &regex, const char *replace, uint8 count = -1)
    {
        //return string(std::regex_replace(str, std::regex(regex.str), replace).c_str());
        char *buffer = NULL,
             *p = NULL;

    replacebegin:
        uint searchlen = regex.length(),
             replen = strlen(replace);

        auto match = imatch(regex.str);

        if (!match.size())
        {
            return *this;
        }

        //printf("%i\n", match.position(0));

        auto ss = string(*this);
        auto s = ss.substring(match.position(0)).str;
        printf("%s | %i\n", s, match.position(0));

        if (!(p = s))
        { // No se encontro nada
            return *this;
        }

        if (searchlen == replen)
        {
            buffer = (char *)malloc(size + 1);
        }
        else if (searchlen > replen)
        {
            buffer = (char *)malloc(size - (searchlen - replen) + 1);
        }
        else
        {
            buffer = (char *)malloc(size + (replen - searchlen) + 1);
        }

        strncpy(buffer, s, p - s);
        buffer[p - s] = '\0';

        sprintf(buffer + (p - s), "%s%s", replace, p + strlen(regex.str));

        free(str);
        str = buffer;
        size = strlen(str);

        if (count == -1)
            goto replacebegin;

        if (count > 0)
        {
            count--;
            goto replacebegin;
        }

        return *this;
    }

    string &trim()
    {
        char *end, *s = _strdup(str);

        while (isspace(*s))
            s++;

        if (*s == 0) // sin espacios
            return *this;

        end = s + strlen(s) - 1;
        while (end > s && isspace(*end))
            end--;

        *(end + 1) = 0;

        memmove(str, s, strlen(s) + 1);

        return *this;
    }

    void clear()
    {
        if (str)
        {
            //printf("cad = %s - %p\n", str, str);
            free(str);
            str = NULL;
        }
    }

    const char *toString() noexcept
    {
        return str ? str : EMPTY;
    }

    ~string()
    {
        //printf("Destructor\n");
        clear();
    }
};
}