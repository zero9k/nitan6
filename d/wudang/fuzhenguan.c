inherit ROOM;

void create()
{
        set("short", "��������¥");
        set("long", @LONG
��������¥�������أ������ʮ�������ʽ����ص����������
һ��֧�ţ��������ġ�һ��ʮ�������������︩�Ⱥɽ�������룬Զ
���𶥣�һ�����죬Ϊ�۾�ʤ�ء�
LONG );
        set("exits", ([
                "west"     : __DIR__"taizipo",
        ]));
        set("no_clean_up", 0);
        set("coor/x", -360);
        set("coor/y", -240);
        set("coor/z", 60);
        setup();
        replace_program(ROOM);
}