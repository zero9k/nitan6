inherit ROOM;

void create()
{
      set("short", "��ˮ��");
        set("long", @LONG
��ˮ��λ�ڱ��Ʒ��£���ǰ�д��ʯ����ҺȪ��Ȫ��ʯ���峺��
�ȣ����겻�ԡ�һ��������������ٲ��䣬������Χ�ߴ�ʮ���ɡ�
�Ա��и����֣��ɴ������з��¶��´����
LONG );
      set("exits", ([
          "eastdown"  : __DIR__"chunyangdian",
          "westup"    : __DIR__"zhongfengsi",
          "south"     : __DIR__"fushouan",
      ]));
      set("outdoors", "emei");
      set("no_clean_up", 0);
        set("coor/x", -440);
        set("coor/y", -230);
        set("coor/z", 70);
        setup();
      replace_program(ROOM);
}