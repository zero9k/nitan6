inherit ROOM;

void create()
{
        set("short", "���׵�");
        set("long", @LONG
��ҿ��׵�ͽֶ���������궼��ͬһ���ϰ忪�ģ�֮���԰����Ƿ�
����ֻ��Ϊ�˷���˿͹����Լ���Ҫ�Ķ����������ı��Ϲ����˸�ʽ����
�Ŀ��ף����Ƶģ����Ƶģ�ͭ�Ƶģ����Ƶģ�Ƥ�Ƶģ�Ʒ����ȫ��Ӧ�о�
�С�
LONG);
        set("no_fight", 1);
        set("no_clean_up", 0);
        set("exits", ([
                  "south" : __DIR__"eroad1",
        ]));
        set("objects", ([
                __DIR__"npc/zhu" : 1,
        ]));
	set("coor/x", -6980);
	set("coor/y", 2190);
	set("coor/z", 0);
	setup();
        replace_program(ROOM);
}