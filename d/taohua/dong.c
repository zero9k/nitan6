// Copyright (C) 2003, by Lonely. All rights reserved.
// This software can not be used, copied, or modified 
// in any form without the written permission from authors.

#include <ansi.h>
inherit ROOM;
 
int do_pick(string arg);

void create()
{
        set("short", "ɽ��");
        set("long", @LONG
��������һ��Сɽ���С�������ߺܰ���ֻ������ԼԼ����������һ
�Ѹɲݡ��ɲ��Ϻ������һ��ʯϻ(xia)����ͻȻ�ŵ�һ�����ŵ���ζ��
��Ȼ�����˳���ס������ȴ����ɨ��Ե�ʡ�
LONG );   
        set("item_desc", ([
                "xia" : @TEXT
����ϸ����һ��ʯϻ�����������ƺ�����һ���飬�ƺ����������ó���
(pick book)��
TEXT
        ]) );
        set("exits", ([
                "out"  : __DIR__"road4",
        ]));
        set("objects", ([
                // __DIR__"npc/zhou" : 1,
        ]) );    
        set("coor/x", 8990);
        set("coor/y", -2930);
        set("coor/z", 0);
        setup();
}
void init()
{
        add_action("do_pick","pick");
}
void reset()
{
        ::reset();
        set("get_available" , 1);
}
int do_pick(string arg)
{
        object me,obj;
        me = this_player();
        
        if( !arg || arg!="book" )
              return notify_fail("��Ҫ��ʲô?\n");
              
        obj = present("zhou botong",environment(me));
        if (obj && living(obj))        
        {
                message_vision(YEL "�ܲ�ͨ����һ�ɣ�����һ����������������͵͵������"
                               "���ܲ�ͨ��û���ģ�\n" NOR,me) ;
                return 1 ;
        }
        if ( query("get_available") )  
        {
                obj = new ("/clone/book/jiuyin1");
                obj->move(me);
                addn("get_available", -1);
                message_vision("$N��ʯϻ��ȡ��һ���顣\n",me );
        } else
                return notify_fail("��Ҫ�����Ǳ��飬ȴ�����㿴�����ۣ���ʵ����ʲô��û�С�\n");
        return 1;
}