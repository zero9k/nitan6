inherit ROOM;

void create()
{
        set("short", "������");
        set("long", @LONG
�������Ǽ���۹ٴɵ��̵꣬���Ҳ����Щ�黭��Ʒ��������һ
�£�����������ľ�����ϴ�С����Ĵ���������Ǹ����˼��õġ���
�ִɣ�����֮�ණ����ֻ�ܴӵ�̯���ˡ�
LONG );
        set("objects", ([
                  __DIR__"npc/liuzhanggui" : 1,
        ]));

        set("no_clean_up", 0);
        set("exits", ([
                  "west" : __DIR__"road3",
        ]));

	set("coor/x", -5050);
	set("coor/y", 2220);
	set("coor/z", 0);
	setup();
        replace_program(ROOM);
}