// Room: /d/shaolin/hantan3.c

#include <room.h>
inherit ROOM;

void create()
{
        set("short", "��ź�̶");
        set("long", @LONG
���Ǻ�̶֮�С�����ˮ�����һ��ʹ����̶ˮ�ڴ˴���ø�ϡ
��һ�������ܵı���������һ����飬����˫������͸�����������⣬
ȫ���������Ѷ������ã�ֻ������ˮ������������³�ȥ��
LONG );
        set("no_clean_up", 0);
        setup();
        replace_program(ROOM);
}