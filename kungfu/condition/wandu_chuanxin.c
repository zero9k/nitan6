#include <ansi.h>
#include <condition.h>

inherit F_CLEAN_UP;
inherit POISON;

string name() { return "qianzhu_wandushou"; }

string chinese_name() { return "穿心剧毒"; }

string update_msg_others()
{
       return HIB "只见$N" HIB "毒气上涌，脸浮现出一层黑气，不住的颤抖。\n" NOR;
}

string update_msg_self()
{
        return HIB "你只觉毒气攻心，头晕目眩，一阵说不出的恶心。\n" NOR;
}

int qi_damage(object me, mapping cnd)
{
        return cnd["level"] / 5;
}

int dispel(object me, object ob, int duration)
{
        if (me != ob)
        {
                tell_object(ob, "你发觉" + me->name() + "的内力源源涌进，使丹"
                            "田处燃烧得更为厉害，便如身陷洪炉，眼前登时一黑。\n");
                tell_object(me, "你试图帮助" + ob->name() + "化解异种真气，却"
                            "发现对方真气便似洪炉爆裂，不由大惊，连忙住手。\n");

                if (living(ob))
                        ob->unconcious();

                return -1;
        }


        tell_object(me, "你觉得全身真气犹如洪炉爆裂，内息"
                        "根本无法凝聚。\n");
        return -1;
}
