// feather.c

#include <armor.h>
#include <ansi.h>
#include <mudlib.h>

inherit CLOTH;

void create()
{
        set_name(HIW "��ȸ����" NOR, ({ "kongque yuyi", "kongque", "yuyi" }) ); 
        set_weight(600);

        if (clonep())
                set_default_object(__FILE__);
        else {
                set("long", HIW "����һ�������칬֯Ů������õ���"
                            "ɴ���¡���ʽ��ӱ���£�\n����Ʈ�ݵġ�"
                            "�������м�ֱ�����������һ�����μ�Ʒ��\n" NOR);

                set("unit", "��");
                set("value", 100000);
                set("material", "feather");
                set("armor_prop/per", 10);
                set("armor_prop/armor", 10);
                set("armor_prop/armor_vs_force", 10);
                set("armor_prop/attack", -2); 
                set("wear_msg", HIW "$N" HIW "�����չ��һ�������ѩ��Ʈ��ɴ"
                                "�������������ϣ�ӳ�ĵ�\n$P" HIW "������"
                                "ѩһ�㣬һ��ӯӯЦ�����ˮܽ�أ�"
                                "�����⶯��ҡ��\n" NOR);

        }

        setup();
}