// This program is a part of NITAN MudLIB
// maxsuck.c

#include <ansi.h>

inherit F_SSERVER;
int exert(object me, object target)
{
        int ap, dp, suck;

        if (target == me) target = offensive_target(me);

        if (me->is_busy()) return notify_fail("你正忙着呢。\n");

        if( query("no_fight", environment(me)) )
                return notify_fail("在这里不能攻击他人。\n");

        if( !objectp(target) || query("id", target) == "muren" )
                return notify_fail("你要吸取谁的丹元？\n");
                
        if( query("race", target) != "人类" )
                return notify_fail("搞错了！只有人才能有丹元！\n");
       
        if( query_temp("sucked", me) )
                return notify_fail("你刚刚吸取过丹元！\n");
        if( objectp(query_temp("weapon", me)) )
                return notify_fail("你必须空手才能施用北冥神功吸人丹元！\n");

        if ((int)me->query_skill("beiming-shengong", 1) < 90)
                return notify_fail("你的北冥神功功力不够，不能吸取对方的丹元！\n");

        if( query("neili", me)<20 )
                return notify_fail("你的内力不够，不能使用化功大法。\n");

        if( me->query_skill("beiming-shengong",1)<query("max_neili", me)/20 )
                return notify_fail("你的北冥神功太低了，再吸取也是徒劳。\n");
                
        if( query("max_neili", me)*2 >= me->query_neili_limit()*3 )
                return notify_fail("你的内力修为似乎已经达到了瓶颈，再吸取也是徒劳。\n"); 

        if( query("max_neili", target)<100 || query("no_suck", target) )
                return notify_fail(target->name() +
                        "丹元涣散，功力未聚，你无法从他体内吸取任何东西！\n");

        if( query("max_neili", target)<query("max_neili", me)/18 )
                return notify_fail(target->name() +
                        "的内功修为远不如你，你无法从他体内吸取丹元！\n");

        message_combatd(
                HIG "$N朝$n微微一笑，突然翻掌变爪，扣向$n的手腕！\n\n" NOR,
                me, target );
                
        me->want_kill(target);               
        if (living(target))
        {
                if (! target->is_killing(me))
                        target->kill_ob(me);
        }
        set_temp("sucked", 1, me);

        ap = attack_power(me, "force");
        dp = defense_power(target, "dodge");
             
        if (ap / 2 + random(ap) > dp || ! living(target))
        {
                tell_object(target, HIR "你只觉全身乏力，全身功力如融雪般消失得无影无踪！\n" NOR);
                tell_object(me, HIG "你觉得" + target->name() + "的丹元自手掌源源不绝地流了进来。\n" NOR);
                
                suck = 100 + (me->query_skill("beiming-shengong", 1) - 90) / 2;
                
                addn("max_neili", -suck, target);
                addn("max_neili", suck, me);
                if( query("max_neili", target)<1 )
                        set("max_neili", 0, target);

                me->start_busy(7);
                if (! target->is_busy())
                target->start_busy(7);
                addn("neili", -10, me);
                // remove_call_out("del_sucked");
                call_out("del_sucked", 10, me);
        }
        else
        {       
                message_combatd(HIY "可是$p看破了$P的企图，机灵地溜了开去。\n" NOR, me, target);
                me->start_busy(2 + random(5));
                // remove_call_out("del_sucked");
                call_out("del_sucked", 15, me);
        }

        return 1;
}

void del_sucked(object me)
{
        if (! me) return;
        
        if( query_temp("sucked", me) )
                delete_temp("sucked", me);
}

