// Room: /d/nanyang/yidao2.c
// Last Modified by winder on Nov. 20 2001

inherit ROOM;

void create()
{
        set("short", "�����");
        set("long", @LONG
������һ����������Ĵ�����ϡ��������������������ӵ����̣�
���Ŵ󳵵������Ͼ��Ͽ��������������������ǳ����֡���ʱ������
������������߷ɳ۶���������һ·���������ϵ��˸������ž�װ����
���������еĻ���������ŵ�������������Щϰ��Ľ����ˡ���·����
����������������֡�
LONG );
        set("exits", ([
                "south" : __DIR__"yidao1",
                "north" : __DIR__"hanshui1",
                "east"  : "/d/room/xiaoyuan",
        ]));
        set("no_clean_up", 0);
        set("outdoors", "nanyang");
        set("coor/x", 0);
	set("coor/y", 60);
	set("coor/z", 0);
	setup();
        replace_program(ROOM);
}