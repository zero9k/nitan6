// Room: /d/suzhou/xidajie2.c
// Date: May 31, 98  Java

inherit ROOM;

void create()
{
        set("short", "�����");
        set("long", @LONG
������������ϣ��е�����Ľ���Ҫ�ȱ𴦵ĸɾ������ࡣ�����
Ϊ�������š���Ӫ���ܱ�����Ե�ʣ������Ƕ�ϲ��ס��������߾���
�ĵ�û�м������ˣ�������������ͨ�����⡣�ϱ�������ۣ�������һ
�����á�
LONG );
        set("outdoors", "suzhou");
        set("no_clean_up", 0);
        set("exits", ([
                "east"      : __DIR__"xidajie1",
                "west"      : __DIR__"ximen",
                "north"     : __DIR__"shuchang",
                "south"     : __DIR__"xuanmiao",
        ]));
        set("objects", ([
                "/clone/npc/walker"  : 1,
        ]));
	set("coor/x", 1090);
	set("coor/y", -1060);
	set("coor/z", 0);
	setup();
        replace_program(ROOM);
}