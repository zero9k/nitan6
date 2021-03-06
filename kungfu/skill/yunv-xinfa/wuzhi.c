// Written by Lonely@nitan.org
// 所向无滞 转世技能
// wuzhi.c

#include <ansi.h>
inherit F_CLEAN_UP;

void remove_effect(object me, int skill);

int exert(object me, object target)
{
        int skill;

        /*
        if( query("family/family_name", me) != "古墓派" )
                return notify_fail("你不是古墓弟子，无法使用所向无滞。\n");
        */

        if( !query("reborn/times", me) )
                return notify_fail("你尚未转世重生，无法使用所向无滞！\n");

        if( (skill = me->query_skill("yunv-xinfa", 1)) < 1000 )
                return notify_fail("你的玉女心法还不够精熟，无法使用所向无滞！\n");

        if( query_temp("wuzhi", me) )
                return notify_fail("你已经在运起所向无滞了。\n");

        if( query("jingli", me)<2000 )
                return notify_fail("你的真气不够。\n");

        if( query("neili", me)<5000 )
                return notify_fail("你的内力不够。\n");

        addn("neili", -1000, me);
        me->receive_damage("qi", 0);

        skill += me->query_skill("martial-cognize", 1);
        skill /= 100;

        message_combatd(HIY "只见$N" HIY "心静如水，内息再无停滞，心中武学如流水般挥洒开来。\n" NOR, me);

        addn_temp("apply/ap_power", skill, me);
        addn_temp("apply/add_weak", skill, me);
        addn_temp("apply/add_busy", skill/6, me);
        addn_temp("apply/double_damage", skill, me);
        set_temp("wuzhi", 1, me);

        me->start_call_out((: call_other, __FILE__, "remove_effect", me, skill :), skill*50);

        if( me->is_fighting() ) me->start_busy(2);

        return 1;
}

void remove_effect(object me, int skill)
{
        if( query_temp("wuzhi", me)){
                addn_temp("apply/ap_power", -skill, me);
                addn_temp("apply/add_weak", -skill, me);
                addn_temp("apply/add_busy", -skill/6, me);
                addn_temp("apply/double_damage", -skill, me);
                delete_temp("wuzhi", me);
                tell_object(me, "你的玉女心法运行完毕，将内力收回丹田。\n");
        }
}
