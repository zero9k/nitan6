

#include <armor.h>
#include <ansi.h>

inherit CLOTH;


void create()
{
        set_name(HIR "���ս��" NOR, ({ "chiyou zhanpao2", "chiyou", "zhanpao2" }) );
        set_weight(3000);
        if( clonep() )
                set_default_object(__FILE__);
        else {
                set("unit", "��");
                set("long", HIR "һ����������ĳ��ۣ����������ħ����ȵı�־��\n" NOR);
                set("material", "steel");
                set("value", 800000);
                set("armor_prop/armor", 600);
                set("armor_prop/str", 6);
                set("armor_prop/con", 6);
                set("limit", ([
                        "exp"  :  10000000,
                        "int"  :  40,
                        "dex"  :  60,
                ]));
                
        }
        setup();
}

int query_autoload()
{
        return 1;
}