inherit ROOM;

void create()
{
        set("short", "�Ƕ���¥");
        set("long", @LONG
���Ƕ���ͷ�ļ�¥�����沼����һ�ŵĶ�ڣ�һ�Ŵ��εļ�����
������ؼ����ų����һ��һ������ʱ׼����ǰ��͵Ϯ���ɹű���ӭ
ͷʹ����
LONG );
        set("indoors", "xiangyang");
        set("no_clean_up", 0);
        set("site", "east_out");
        set("no_fly",1);     
        set("exits", ([
                "east" : __DIR__"east_out1",
                "up" : __DIR__"eastlou5",  
                "down" : __DIR__"eastlou3",  
        ]));
        set("coor/x", -7780);
	set("coor/y", -770);
	set("coor/z", 40);
	setup();
}
int valid_leave(object me, string dir)
{
        if (dir == "east" )
        return notify_fail("���ȥ��ʲô����������\n");
        return ::valid_leave(me,dir);
}