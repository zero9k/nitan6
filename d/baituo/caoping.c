inherit ROOM;

void create()
{
        set("short","��ƺ");
        set("long", @LONG
������һ�鷼��ƺ���̲����𣬺������裬���˸е���⡣����
ż��Ҳ�������˻��ߡ�
LONG );
        set("exits", ([
                "west" : __DIR__"shijie",
        ]));

        set("objects" , ([
                "/clone/beast/caihuashe" : random(2),
        ]));

        set("quarrys", ([
                "caihuashe"  : 100000,
                "dushe"      : 20000,
                "jinshe"     : 20000,
                "qingshe"    : 20000,
        ]));

        set("outdoors", "baituo");
        set("coor/x", -49970);
        set("coor/y", 19990);
        set("coor/z", 10);
        setup();
        replace_program(ROOM);
}