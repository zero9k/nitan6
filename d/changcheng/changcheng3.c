// Room: /d/changcheng/changcheng3.c
// Last Modified by Winder on Aug. 25 2000

#include <ansi.h>
inherit ROOM;

void create()
{
        set("short", "长城");
        set("long", WHT @LONG
这里就是举世闻名的──长城。站在长城上，你可以看到四周群山
环绕，山上到处都是矮矮的灌木丛，举目望去，你脚下的长城顺着山脊
象一条巨龙般蜿蜒的向东和向西延伸出去。从这里向东你隐隐约约可以
看到长城上著名的居庸关。
LONG
NOR );
        set("outdoors", "changcheng");
        set("exits", ([
                "westup"   :__DIR__"changcheng4",
                "eastdown" :__DIR__"changcheng2",
        ]));
        set("no_clean_up", 0);
        set("coor/x", -2000);
        set("coor/y", 5000);
        set("coor/z", 30);
        setup();
        replace_program(ROOM);
}