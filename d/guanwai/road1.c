// /guanwai/road1.c
inherit ROOM;

void create()
{
        set("short", "�ٵ�");
        set("long", @LONG
������ͨ������Ĺٵ������׿��Ĵ����ȫ���ɻ����̳ɣ��ɲ�������
������������ӵ���������ǽ����ؿڵıؾ�֮·��·��������Ⱥ�����˶���
��������Ƥ����ҩ�ĵ������ˡ�ż�������Źٱ������ɳ۶�����
LONG );
        set("exits", ([
                "northeast" : __DIR__"road2",
                "southwest" : __DIR__"shanhaiguan",
        ]));
        set("objects",([
                __DIR__"npc/xiaoyuanshan" : 1,
        ]));
//        set("no_clean_up", 0);
         set("outdoors", "guanwai");
        set("coor/x", 3950);
	set("coor/y", 9130);
	set("coor/z", 0);
	setup();
        replace_program(ROOM);
}