// Code of ShenZhou
// shegu.c �߹�
// maco 7/15/2000

#include <ansi.h>
inherit ROOM;

void create()
{
        set("short", "�߹�����");
        set("long", @LONG
���ǰ���ɽ���߹�֮�У���ľ�������ݴ��в�ʱ����ɳɳ֮����
�ƺ���ʲ���ڲ������شܶ���
LONG );

        set("exits", ([ 
            "northeast" : __DIR__"shegu9",
            "west" : __DIR__"shegu11",
        ]));

        set("hide_snake", random(3));
        set("grass", 1);

        set("cost", 2);
        set("outdoors","baituo");
        set("shegu", 1);
        set("coor/x", -50030);
        set("coor/y", 20160);
        set("coor/z", 20);
        setup();
}

#include "snake_room.h"