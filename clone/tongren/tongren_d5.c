#include <ansi.h>

inherit ITEM;

int is_tongren() { return 1; }
void create()
{
        set_name(YEL "冲穴铜人·阳维脉·头维穴" NOR, ({ "tongren d5", "tongren" }));
        set_weight(50);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("unit", "个");
                set("long", NOR + YEL "这是一个冲穴铜人，上面绘制了一些经络穴位图案，以及冲穴使用方法。\n" NOR);
                set("value", 10000);
                set("material", "steal");
        }
        set("jingmai_name", "阳维脉");
        set("xuewei_name", "头维穴");
        set("chongxue_xiaoguo", "POT:400:JING:200");
        set("neili_cost", "3000");
        setup();
}

