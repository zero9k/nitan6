#include <ansi.h>

inherit EQUIP_SOUL;

void create()
{
        set_name(HIM "头盔之魂" NOR, ({"head soul2", "head", "soul2"}));
        set_weight(100);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("long", HIM "这是一个头盔之魂。\n" NOR);
                set("unit", "个");
                set("value", 200000);
                set("no_sell", 1);
        }
        set("type", "head");
        set("status", 2);
        set("auto_load", 1);
        
        setup();
}

