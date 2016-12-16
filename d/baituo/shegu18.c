// Code of ShenZhou
// shegu.c �߹�
// maco 7/15/2000

#include <ansi.h>
inherit ROOM;
string look_crack();

void create()
{
        set("short", "�߹�����");
        set("long", @LONG
���ǰ���ɽ�߹��е�һ�����֣��Ӳݴ�������ľ֦��ï�ܣ��İ�
��ɭ����ľ�̸����ڣ���Щ��������ѷ� (crack)������ϸ���������
�����������Ըߣ���һ���µء����Ϸ��Ǵ�Ƭ�ĵأ����߹�����Σ��
�ĵش���
LONG );

        set("exits", ([ 
            "east" : __DIR__"shegu17",
            "northeast" : __DIR__"shegu19",
            "southwest" : __DIR__"shegu_maze1",
        ]));

        set("item_desc", ([
        "crack" : (: look_crack :),
        ]));

        set("objects", ([
            __DIR__"npc/bt_snake" : 1,
        ]));

        set("hide_snake", 1+random(3));
        set("grass", 1);
        set("crack", 1);

        set("cost", 2);
        set("outdoors","baituo");
        set("shegu", 1);
        set("coor/x", -49950);
        set("coor/y", 20180);
        set("coor/z", 20);
        setup();
}

int valid_leave(object me, string dir)
{
        if (dir == "southwest") {
                if( query("family/family_name", me) == "����ɽ"
                 && !query_temp("shegu/warning_18", me)){
                        set_temp("shegu/warning_18", 1, me);
                        return notify_fail( HIR"���ϱ����߹��д��������û�Ļĵأ������ﰵ�Ծ��裬����С�Ľ�����\n"NOR+"��������׼���Ļ������ٴ�ǰ���ɡ�\n");
                }
        }
        return ::valid_leave(me,dir);
}


#include "snake_room.h"