// xiaohuan-dan.c С����

#include <ansi.h>

inherit ITEM;

void create()
{
        set_name("С����", ({"xiaohuan dan", "xiaohuan", "dan"}));
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("unit", "��");
                set("long", "����һ��Ө����Բ��С�������˵���������"
                            "ҩ��������������Ч�ޱȡ�\n");
                set("value", 10000);
                set("only_do_effect", 1);
        }

        setup();
}

int do_effect(object me)
{
        if( query("neili", me) >= query("max_neili", me)*2-200 )
                return notify_fail("������û��Ҫ��С������\n");

        me->start_busy(2);

        addn("neili", 200, me);
        message_vision(HIY "$N" HIY "����һ��С������ֻ�����������࣬"
                       "��֫�ٺ�������ӯ�ޱȣ��������ڶ�����\n" NOR, me);

        destruct(this_object());
        return 1;
}