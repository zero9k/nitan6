#include <ansi.h>
#include <weapon.h>

inherit SWORD;

void create()
{
        set_name(HIY "������" NOR, ({ "huanglong jian", "huanglong", "jian", "sword" }) );
        set_weight(4000);
        if( clonep() )
                set_default_object(__FILE__);
        else {
                set("unit", "��");
                set("value", 700000);
                set("material", "steel");
        }
        init_sword(60);
        setup();
}