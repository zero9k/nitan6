// Code of JHSH
// mutong.c ľͰ

#include <ansi.h>

#define max_volume 3
inherit ITEM;

void create()
{
        set_name("ľͰ", ({"mu tong", "tong", "bucket"}));
        set_weight(1000);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("long", "һ������װˮ��ľͰ��\n");
                set("unit", "��");
                set("amount", 0);
                set("no_drop",1);
        }

}

string extra_long()
{        
        string str;

        switch(query("amount"))
        {
                case 0: 
                        str="����ʲôҲû�С�\n";
                        break;
                case 1:
                        str="�����������ı�ˮ��\n";
                        break;
                case 2:
                        str="����ı�ˮ�����ˡ�\n";
                        break;
                case 3:
                        str="����װ���˱�ˮ��\n";
                        break;
        }
        
        return str;
}

void init()
{
        add_action("do_fill","fill");
        add_action("do_fill","zhuang");
        add_action("do_pour","pour");
        add_action("do_pour","dao");
}

int do_fill(string arg)
{
        object me=this_player();

        if (!arg) return 0;

        if (arg!="tong"&& arg!="bucket") return 0;

        if (me->is_busy()||me->is_fighting())
                return notify_fail("����æ���أ�\n");

        if( query("short", environment(me)) != "��ˮ��̶" )
                return notify_fail("����û����ˮ��\n");

        if (query("amount") >= max_volume)
                return notify_fail("ľͰ�Ѿ�װ��ˮ�ˡ�\n");

        if( query("jing", me)<50 || query("qi", me)<50 )
                return notify_fail("���Ѿ���ƣ�����ˡ�\n");

        message_vision("$NС�������شӱ�ˮ��̶��Ҩ��Щ��ˮ������\n",me);
        addn("amount",1);
        if ( random(me->query_con()) > 20)
        {
                message_vision("һ�ɺ�����̶��ֱð����������$N���˸����£�\n",me);
                me->receive_damage("qi",random(10) + 5);
        }
        addn("qi", -30, me);
        me->start_busy(3);

        return 1;
}

int do_pour(string arg)
{
        object gang;
        object me=this_player();
        string item,target;
        int amount;

        if (!arg) return 0;

        if (sscanf(arg, "%s into %s", item, target) != 2)
                return 0;

        if (me->is_busy() || me->is_fighting() )
                return notify_fail("����æ���ء�\n");

        if ((item != "ˮ" && item!="water") || (target!="ˮ��" && target!="gang"))
                return notify_fail("��Ҫ��ʲô�����������\n");

        if (!(gang=present("shui gang",environment(me))))
                return notify_fail("����û��ˮ�ס�\n");
        
        if (!(amount = query("amount")))
                return notify_fail("ľͰ��ʲô��û�С�\n");

        message_vision("$N��ľͰ���ˮȫ��������ˮ���\n",me);
        set("amount",0);
        addn("amount", amount, gang);
        addn_temp("water_amount", amount, me);
        if( query_temp("water_amount", me) >= 15 )
                tell_object(me,HIG"�������ʹ������������Ӧ�ÿ��Ի�ȥ�����˰ɣ�\n"NOR);
        return 1;
}