// Room: /d/xiangyang/bingying1.c
// Date: Jan. 8 1999 by Winder

inherit ROOM;

void create()
{
        set("short", "��Ӫ");
        set("long", @LONG
�����Ǳ�Ӫ���������鵽�����ǹٱ����е����佫��ָ�����ж�
�������еĶ������������е����š�����������Ϣ����ǽ��������˧��
������ɫ��Ѳ�������ܡ����������������ȫ���ɴ��۾������㡣
LONG );
        set("item_desc", ([
                "dong" : "\n",
        ]));

        set("exits", ([
                "east" : __DIR__"northjie",
        ]));
        set("objects", ([
                __DIR__"npc/pian" : 1,
                __DIR__"npc/zuo"  : 1,
                __DIR__"npc/bing" : 2,
        ]));
        set("coor/x", -7820);
	set("coor/y", -730);
	set("coor/z", 0);
	setup();
        replace_program(ROOM);
}