// fubiting.c

inherit ROOM;

void create()
{
        set("short", "����ͤ");
        set("long", @LONG
����԰���߽������, ���Ͻ�ͤ, ������Ǹ���ͤ. ͤ�ӳ��ϱ���, 
ͤ�����к���, �����о�. ����, ��ˮ�峺, ����Ϸ���ںɻ�֮��. ��
��, ��ˮ���, ������뾮����. 
LONG
);
        set("outdoors", "huanggong");
        set("exits", ([
                "southwest" : __DIR__"yuhuayuan",
        ]) );
        set("no_clean_up", 0);
	set("coor/x", -2810);
	set("coor/y", 7731);
	set("coor/z", 0);
	setup();
        replace_program(ROOM);
}