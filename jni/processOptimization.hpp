#include "tombstone/HeadFile.h"
#include "tombstone/Freezer.hpp"
#include "configuration.h"

namespace whitelist
{
    //读取pid包名
    void PidCmdline(const char *pid, char *cmdline)
    {
        char buffer[FILE_CACHE_ZX], cmdlinePath[FILE_CACHE_ZX];
        sprintf(cmdlinePath, "/proc/%d/cmdline", atoi(pid));
        FILE *fp = fopen(cmdlinePath, "r");
        fgets(buffer, sizeof(buffer), fp);
        strcpy(cmdline, buffer);
    }

    //判断字符串是否为数字 是返回true 否返回false
    bool IsNumber(const char *str)
    {
        if (str == NULL)
        {
            return false;
        }
        while (*str)
        {
            if (!isdigit(*str))
            {
                return false;
            }
            str++;
        }
        return true;
    }

    // c++20 查找字符串中是否包含某个字符串
    bool Contains(const std::string &str, const std::string &substr)
    {
        return str.find(substr) != std::string::npos;
    }

    int *getNext(char *str)
    {
        int len = strlen(str);
        int *next = (int *)calloc(len, sizeof(int));
        next[0] = -1;
        int i = 0, k = -1;

        while (i < len - 1)
        { //这里注意-1

            if (k == -1 || str[i] == str[k])
            {
                i++;
                k++;
                next[i] = (str[i] == str[k] ? next[k] : k);
            }
            else
            {
                k = next[k];
            }
        }

        return next;
    }

    int KMP(char *chang, char *duan)
    {

        int *next = getNext(duan);
        int c_len = strlen(chang);
        int d_len = strlen(duan);
        int i = 0, k = 0;

        while (i < c_len && k < d_len)
        {

            if (k == -1 || chang[i] == duan[k])
            {
                i++;
                k++;
            }
            else
            {
                k = next[k];
            }
        }

        return k < d_len ? -1 : i - k;
    }

    //判断字符串是否为字母
    bool IsLetter(const char *str)
    {
        if (str == NULL)
        {
            return false;
        }
        while (*str)
        {
            if (!isalpha(*str))
            {
                return false;
            }
            str++;
        }
        return true;
    }

    int My_strncmp(const char *str1, const char *str2, int count)
    {
        assert((str1 != NULL) && (str2 != NULL));
        if (NULL == str1 || NULL == str2)

            return -2;
        // 1.两个值不相等；2，相等但是是'\0';3,需要比较的个数count 已经比较完毕
        // 如果写成count--  则会让循环体代码多执行一次
        while (--count && (*str1 == *str2) && (*str1 != '\0') && (*str2 != '\0'))
        {
            str1++;
            str2++;
        }
        int tmp = *str1 - *str2;
        if (tmp == 0)
        {
            return 0;
        }
        else if (tmp > 0)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }
} // namespace whitelist