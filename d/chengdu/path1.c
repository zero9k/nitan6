// Room: path1.c
// Date: Feb.14 1998 by Java

inherit BUILD_ROOM;
string look_gaoshi();
void create()
{
        set("short", "Ϫ��С·");
        set("long", @LONG
������佻�Ϫ����С·�ϣ�佻�Ϫˮ��Ө��͸����������ԶԶ
�����ŵ�ˮ�������㡣����˯��ɢ����Ϫ��������ˮ��Ʈ����������
��Ƽ����Ƽ���в����޺��������ȥ��Ϫ��С·��Ϫ���������ƣ�
����û�㣬��Ʈ�ƺ���ʹ�˲���������������ν "���д���" ���ϲ�
���ۡ�
LONG );
        set("outdoors", "chengdu");
        set("exits", ([
                "north"  : __DIR__"path2",
                "east"   : __DIR__"nanheqiaow",
        ]));
        set("max_room", 4);
        set("no_clean_up", 0);
        set("coor/x", -15270);
	set("coor/y", -1830);
	set("coor/z", 0);
	setup();
}