#include <ansi.h>
#include <weapon.h>
inherit SWORD;

void create()
{
        set_name(NOR + HIC "���ǽ�" NOR, ({ "bingpo jian", "bingpo", "jian" }));
        set_weight(100);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("unit", "��");
                set("value", 2000);
                set("material", "steel");
                set("wield_msg", "$N��ৡ���һ�����һ��$n�������С�\n");
                set("unwield_msg", "$N�����е�$n��ؽ��ʡ�\n");
        }
        init_sword(1200);
        setup();
}
