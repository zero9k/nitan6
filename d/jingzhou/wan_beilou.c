// This program is a part of NT MudLIB

inherit ROOM;

void create ()
{
        set ("short", "��¥");
        set ("long", @LONG
�ϵ�¥����ֻ���м俿������һ�Ŵ����������ű�īֽ����ʮ����
�飬����һ������һ���С��������������ҹ������ס�ĵط���
LONG);
        set("region", "jingzhou");
        set("exits", ([
                "down"  : __DIR__"wan_ketang",
        ]));
        set("objects", ([
                __DIR__"npc/wangui"    : 1,
                __DIR__"obj/lianpen"    : 1,
        ]));

        set("coor/x", -7110);
	set("coor/y", -2010);
	set("coor/z", 10);
	setup();
}
void init()
{
        add_action("do_put","put");
        add_action("do_put","jin");
        add_action("do_put","fang");
}
int do_put(string arg)
{
        object ob, me = this_player();
        if (arg != "tangshi book")
                return notify_fail("��Ҫ��ʲô����������ȥ��\n");
        if (! objectp(ob = present("tangshi book", me)))
                return notify_fail("��û���Ȿ�顣\n");
        message("vision", me->name()+"����ʫѡ������������ȥ������������ʾ�����������ۣ���ʫ������\n", environment(me));
        destruct(ob);
        ob = new("/clone/book/tangshi_book");
        ob->move(me);
        return 1;
}