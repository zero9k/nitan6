//Room: /d/ruzhou/hongniang-zhaung.c
// by llm 99/06/12

#include <ansi.h>

inherit ROOM;

string look_sign(object me);

void create()
{
        set("short", "������");
   set("long", @LONG
����������·򸾶�Լ��Ե�ĺ����ã�ֻ��������޽��Լ���ǽ��
��Լ���ܵõ����˵ĳ��ϡ�������һ����������������Ǿ���һ������֮
���Ե�ʮ�ֵ����ɡ��������й���һ������ֵ���ľ����(sign)��
LONG
        );
        set("no_fight", 1);
        set("exits", ([
                "west" : __DIR__"hong-zoulang",
                "east" : __DIR__"yinyuan-tang",
   ]) );

   set("item_desc", ([
           "sign": (: look_sign :),
   ]) );

   set("objects", ([
        //__DIR__"npc/meipo" : 1,
      ]) );
	set("coor/x", -6730);
	set("coor/y", 1850);
	set("coor/z", 0);
	setup();
}

string look_sign(object me)
{
        return HIM"Ȱ�Ͳ�Ȱ��\n"
   "��Ե�����\n"
   "����ǰ��ϵ\n"
   "����һ����\n"NOR;
}

int valid_leave(object me, string dir)
{
         if( dir == "west" && query_temp("pending/pro", me) )
      delete_temp("pending/pro", me);
      return ::valid_leave(me, dir);
}