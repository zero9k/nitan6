// shield.c 太极护体神功
// Created by Lonely

#include <ansi.h>

inherit F_CLEAN_UP;

void remove_effect(object me, int amount);

int exert(object me, object target)
{
        int skill;

        if (target != me)
                return notify_fail("你只能用太极神功来提升自己的防御力。\n");

        if( query("neili", me)<100 )
                return notify_fail("你的真气不够。\n");

        if ((int)me->query_skill("taiji-shengong", 1) < 40)
                return notify_fail("你的太极神功等级不够。\n");

        if( query_temp("shield", me) )
                return notify_fail("你已经在运功中了。\n");

        skill = me->query_skill("force");
        addn("neili", -100, me);
        me->receive_damage("qi", 0);

        message_combatd(HIR "$N" HIR "默念太极神功的口诀：他"
                            "强由他强，清风拂山冈。他横任他横，"
                            "明月照大江……\n刹那间只见$N" 
                        HIR "全身顿时浮现出一层刚猛的劲气，将$P"
                        HIR "全全笼罩。\n" NOR, me);

        addn_temp("apply/armor", skill/2, me);
        set_temp("shield", 1, me);

        me->start_call_out((: call_other, __FILE__, "remove_effect", 
                           me, skill / 2 :), skill);

        if (me->is_fighting()) me->start_busy(2);

        return 1;
}

void remove_effect(object me, int amount)
{
        if( query_temp("shield", me) )
        {
                addn_temp("apply/armor", -amount, me);
                delete_temp("shield", me);
                tell_object(me, "你的太极神功运行完毕，将内力收回丹田。\n");
        }
}

