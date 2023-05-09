#include <stdio.h>
#include "jsonlib.h"

const char *jsonstr = "{\"serialNum\":\"DE58025CDE44\",\"deviceNum\":\"202\",\"count\":\"1\",\"rows\":[{\"eventNum\":\"35\",\"doorId\":\"1\",\"cardID\":\"2598580497\",\"enterType\":\"2\",\"readerId\":\"1\",\"inOrOut\":\"1\",\"dealTime\":\"2023-05-09 15:37:57\",\"deviceSId\":\"52\"}]}";

JSON_OBJ_ITEM rootdata[] = {
    {"serialNum", TYPE_STR},
    {"deviceNum", TYPE_STR},
    {"count", TYPE_STR},
    {"rows", TYPE_ARRAY},
};

int main(int argc, char const *argv[])
{
    /* code */
    paserJson(jsonstr, &rootdata);
    return 0;
}
