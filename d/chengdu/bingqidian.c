// Room: bingqidian.c
// Date: Feb.14 1998 by Java

inherit ROOM;
void create()
{
        set("short", "������");
        set("long", @LONG
������һ��СС����ı����̣��������ı����ɲ�Ѱ���������
Ȼ�����ѵ�һ�������õ��˸���ϡ�١��ƹ�����������ֱ��ë��
��ת���Ϳ���ǽ�ϵ�����(zhaopai)��
LONG );
        set("item_desc", ([
                "zhaopai": "���� \"list\" �г��������\"buy\" ���ϰ幺�\n",
        ]));
        set("objects", ([
                __DIR__"npc/tanghuai" : 1,
        ]));
//         set("no_clean_up", 0);
        set("exits", ([
            "southeast" : __DIR__"northroad1",
        ]));

        set("coor/x", -15240);
	set("coor/y", -1800);
	set("coor/z", 0);
	setup();
        replace_program(ROOM);
}