// caodi2.c �ݵ�
// Java Oct.10 1998

inherit ROOM;

void create()
{
        set("short", "�ݵ�");
        set("long", @LONG
������һƬ�ݵأ��ݺ��������û��С�ȡ��ݼ�ɢ����һЩ�滨��
΢�紵�����ҷ��˱ǡ�������ʱ���С������ˡ���������������һЩ��
ͷ���İ�ɫ���۷��ڲݵ��Ϸ���������
LONG        );
        set("outdoors", "gumu");
        set("exits", ([
                "west"      : __DIR__"caodi",
                "southeast" : __DIR__"hanshuitan",
        ]));
        set("objects", ([
                __DIR__"npc/yufeng" : random(3),
        ]));
        set("coor/x", -3210);
        set("coor/y", 40);
        set("coor/z", 90);
        setup();
        replace_program(ROOM);
}