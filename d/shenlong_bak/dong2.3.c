inherit DEMONROOM;

void create()
{
        set("short", "��������");
        set("long",@LONG
����һ���������������ס�
LONG);

        set("exits", ([
                "north"   : __DIR__"dong2.2",
                "south"   : __DIR__"dong2.4",

        ]));

                set("n_time", 60); 
                set("n_npc", 2); 
                set("n_max_npc", 4);
                set("s_npc", "/clone/dream_npc/shenlongdao/xixue-bianfu");

        setup();
}