// jue.c  回风拂柳剑法「绝剑」

#include <ansi.h>
#include <combat.h>

inherit F_SSERVER;

string name() { return HIW "绝剑" NOR; }

int perform(object me, object target)
{
        int damage;
        string msg;
        object weapon;
        string wname;
        int ap, dp;

        if (! target) target = offensive_target(me);

        if (! target || ! me->is_fighting(target))
                return notify_fail(name() + "只能对战斗中的对手使用。\n");

        if( !objectp(weapon=query_temp("weapon", me)) ||
            query("skill_type", weapon) != "sword" )
                return notify_fail("你所使用的武器不对，无法施展" + name() + "。\n");

        if (me->query_skill_mapped("sword") != "huifeng-jian")
                return notify_fail("你没有激发回风拂柳剑法，无法施展" + name() + "。\n");

        if ((int)me->query_skill("huifeng-jian", 1) < 120)
                return notify_fail("你回风拂柳剑法不够娴熟，无法施展" + name() + "。\n");

        if ((int)me->query_skill("force") < 200)
                return notify_fail("你内功火候不够，无法施展" + name() + "。\n");

        if( query("neili", me)<500 )
                return notify_fail("你现在真气不够，无法施展" + name() + "。\n");

        if (! living(target))
                return notify_fail("对方都已经这样了，用不着这么费力吧？\n");

        wname = weapon->name();

        damage = damage_power(me, "sword");

        // 第一招
        ap = attack_power(me, "sword") + me->query_str();
        dp = defense_power(target, "parry") + target->query_str();
        msg = HIC "$N" HIC "一声暴喝，手中的" + wname + HIC "对准$n" HIC "直劈而"
              "下，正是回风拂柳剑「" NOR + HIW "绝剑" NOR + HIC "」绝技。\n" NOR;
        if (ap / 2 + random(ap) > dp)
        {
                msg += COMBAT_D->do_damage(me, target, WEAPON_ATTACK, damage, 50,
                                           HIR "$p" HIR "奋力抵挡，却哪里招架得住，被$P"
                                           HIR "这一剑劈下，登时鲜血迸流！\n" NOR);
        } else
        {
                msg += CYN "$p" CYN "嘿的一声，硬声声将$P"
                       CYN "这一剑架开，丝毫无损。\n" NOR;
        }

        // 第二招
        ap = attack_power(me, "sword") + me->query_int();
        dp = defense_power(target, "parry") + target->query_int();
        msg += "\n" HIC "$N" HIC "随即一声冷笑，手中的" + wname +
               HIC "伸伸缩缩，吞吞吐吐，变幻莫测，层层剑光笼罩$n" HIC "。\n" NOR;
        if (ap / 2 + random(ap) > dp)
        {
                msg += COMBAT_D->do_damage(me, target, WEAPON_ATTACK, damage, 50,
                                           HIR "$p" HIR "大惑不解，一时难以勘透其"
                                           "中奥妙，一连中了几招，身披数创！\n" NOR);
        } else
        {
                msg += CYN "$p" CYN "淡淡一笑，浑然不将$P"
                CYN "玄妙的招数放在眼中，随意将之架开。\n" NOR;
        }

        // 第三招
        ap = attack_power(me, "sword") + me->query_con();
        dp = defense_power(target, "force") + target->query_con();
        msg += "\n" HIC "$N" HIC "深深吸了一口气，手中的" + wname +
               HIC "幻化出一层浅浅的光芒，人剑合一扑向$n" HIC "而去。\n" NOR;
        if (ap / 2 + random(ap) > dp)
        {
                msg += COMBAT_D->do_damage(me, target, WEAPON_ATTACK, damage, 55,
                                           HIR "$p" HIR "运气抵挡，只觉得一股透体凉意"
                                           "袭了上来，一个激凌，丹田立刻变得空空荡荡！\n" NOR);
        } else
        {
                msg += CYN "$p" CYN "一口气自丹田运了上来，$P"
                CYN "附体剑芒虽然厉害，却未能伤$p" CYN "分毫。\n" NOR;
        }

        // 第四招
        ap = attack_power(me, "sword") + me->query_dex();
        dp = defense_power(target, "dodge") + target->query_dex();
        msg += "\n" HIC "$N" HIC "脚步一晃，忽然近上身来，身法奇快，手中的" + wname +
               HIC "应声而起，刺向$n" HIC "。\n" NOR;
        if (ap / 2 + random(ap) > dp)
        {
                msg += COMBAT_D->do_damage(me, target, WEAPON_ATTACK, damage, 60,
                                           HIR "$p" HIR "只是慢了一步，登时被$P"
                                           HIR "这一剑刺了个正中，鲜血四处飞射！\n" NOR);
        } else
        {
                msg += CYN "$p" CYN "身法却是更快，只是一退，让$P"
                CYN "这一招堪勘不能奏效。\n" NOR;
        }

        // 第五招
        ap = attack_power(me, "sword");
        dp = defense_power(target, "parry");
        msg += "\n" HIC "$N" HIC "一声长叹，眼神变得寒冷无情，" + wname +
               HIC "悠悠而转，犹如轻风拂柳，说不清的缠绵之意。\n" NOR;
        if (ap / 2 + random(ap) > dp)
        {
                msg += COMBAT_D->do_damage(me, target, WEAPON_ATTACK, damage, 65,
                                           HIR "$p" HIR "只觉得沐浴在风中一般，对这招竟然是无可抵挡，被"
                                           HIR "剑光削了个血肉模糊！\n" NOR);
        } else
        {
                msg += CYN "$p" CYN "冷静非凡，丝毫不为这奇幻的招数"
                       "所动，凝神抵挡，不漏半点破绽！\n" NOR;
        }

        me->start_busy(3 + random(5));
        addn("neili", -400-random(100), me);
        message_combatd(msg, me, target);
        return 1;
}
