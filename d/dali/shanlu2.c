//Room: /d/dali/shanlu2.c
//Date: June. 29 1998 by Java

inherit ROOM;
void create()
{
        set("short","ɽ·");
        set("long",@LONG
һ��ɽ·���������ï�ܵ����ֺ����಻���Ĵ�ɽ����·���Լ�
��ɼ�һЩ����Ĵ�������һƬ��ɽ�������ƣ�ɽ�߰�ѩ��������
��������·������ɽ��һ����·���ϱ������š�
LONG);
        set("outdoors", "daliw");
        set("exits",([ /* sizeof() == 1 */
            "east"      : __DIR__"shanlu3",
            "south"     : __DIR__"shanlu4",
            "northup"   : __DIR__"nianhuasi",
            "westup"    : __DIR__"biluoxueshan",
        ]));
        set("objects", ([
                "/clone/npc/walker"  : 1,
        ]));
        set("no_clean_up", 0);
	set("coor/x", -19160);
	set("coor/y", -6890);
	set("coor/z", 0);
	setup();
        replace_program(ROOM);
}