// Room: /d/huijiang/shamo2.c
// Last Modified by winder on Sep. 12 2001

inherit ROOM;

void create()
{
        set("short", "ɳĮ");
        set("long", @LONG
����ɫ��ɳ�����࣬��������һƬ�������У����ϲ������񣬵���
�������ޡ�ֻ��ɳĮ�е��е�����ʬ�������㣬�����й������ĺۼ���
LONG);
        set("outdoors", "huijiang");
        set("exits", ([ /* sizeof() == 2 */
                "east"  : __FILE__,
                "west"  : __DIR__"shamo3",
                "north" : __DIR__"shamo1",
                "south" : __DIR__"gebi2",
        ]));
        set("no_clean_up", 0);
        set("coor/x", -50010);
        set("coor/y", 9010);
        set("coor/z", 0);
        setup();
        replace_program(ROOM);
}