//HSQCHANGLANG.C

inherit ROOM;

void create()
{
        set("short", "��ˮ�쳤��");
        set("long", @LONG
��������һ������֮�ϣ� �����������ۣ� �������续���� С
�Ӵ����¾������ʣ� ����֮��Ʈ���ŵ��������̣� ������ɴ������
�����־���������֣� �ƺ�û��һ��������
LONG );
        set("exits", ([
                "east"  : __DIR__"cangshui",
                "north" : __DIR__"hsqdating",
                "out"   : __DIR__"hsqmen",
        ]));
        set("outdoors", "mingjiao");
        set("no_clean_up", 0);
        setup();
        replace_program(ROOM);
}