#include "regular_exp.h"
#include <regex.h>
#include <stdio.h>

// http://c.biancheng.net/cpp/html/1428.html regex函数族说明

int regular_match(const char* in, const char* regular) {

    int ret = 0;
    int flags = REG_EXTENDED;
    regex_t regex;
    size_t nmatch;
    regmatch_t pmatch[1];

    if (NULL==in || NULL==regular)
        return -REGULAR_ERR_PARAMCHECK;

    ret = regcomp(&regex, regular, flags);
    if (0!=ret)
        return -REGULAR_ERR_COMPILED;

    ret = REGULAR_MATCH;
    if (0!=regexec(&regex, in, nmatch, pmatch, 0))
        ret = -REGULAR_ERR_NOTMATCH;

    regfree(&regex);
    return ret;
}

