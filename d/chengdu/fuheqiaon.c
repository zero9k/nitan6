// Room: fuheqiaon.c
// Date: Feb.14 1998 by Java

inherit ROOM;
void create()
{
        set("short","������");
        set("long", @LONG
�����ǳɶ������⸮���š��ɶ����Ǵ����׸���ȴ���տ��أ�һ
���з������ɶ�ƽԭ����ʹ��������ˮ�ǣ�Ҳ��ֻ�е���Է��ȱ���
ѡ���ˡ�
LONG );
        set("outdoors", "chengdu");
        set("exits",([ /* sizeof() == 1 */
            "north"  : "/d/qingcheng/qcroad1",
            "south"  : __DIR__"northgate",
        ]));
        set("no_clean_up", 0);
        set("coor/x", -15220);
	set("coor/y", -1790);
	set("coor/z", 0);
	setup();
        replace_program(ROOM);
}