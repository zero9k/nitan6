inherit ROOM;
void create()
{
        set("short", "����"); 
        set("long", @LONG
������Բ��ɽׯ��԰һƬ���ܵ���ľ�֣�����һƬ�ž���������
�ı�ض��ǣ�����������С·�̳�������������̺����֪����Ұ��͵
͵�ӹ���С·�Ծ������������ʵ�СϪ��Ϫˮ��Ư���������Ҷ����
��֦Ҷ����ϡ�ɿ���Ϫˮ�����羵���峺���ס�
LONG );
        set("outdoors", "wansong");
        set("type","forest");
        set("exits",([
                "east":__DIR__"grass1",
                "northwest":__DIR__"senling2",
        ]) );
        
        set("objects", ([
                CLASS_D("mojiao/fuhongxue") : 1,
        ]));
        set("coor/x",-610);
        set("coor/y",240);
        set("coor/z",80);
        setup();
        replace_program(ROOM);
}   