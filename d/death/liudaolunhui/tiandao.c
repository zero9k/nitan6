#include <ansi.h>
inherit ROOM;

void create()
{
        set("short", HIW "\n\n                ��        ��        ��" NOR);
        set("long", HIW "\n"
"ãã�ƺ������ޱ߼ʡ����ܽ��ɢ�������ƶ�䣬Զ���з��\n"
"���ƣ�������Ϸ�����ߴ�����������������ãȻ��֪���롣��\n"
"�������鼰���硢ɫ�缰��ɫ�����硣�����������죬ɫ����\n"
"����ʮ���죬��ɫ�����죬���ж�ʮ���졣\n\n" NOR);

        set("objects", ([
                __DIR__"npc/tiandaofo" : 1,
        ]));
        set("no_magic", 1);
        setup();
}