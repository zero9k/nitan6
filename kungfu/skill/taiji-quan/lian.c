// This is player's own perform (Write by Lonely@nt2)
//Createby小刀(Xiaodao)atTue Aug 19 17:52:34 2014
// 连字诀(lian)
#include <ansi.h>
#include <combat.h>

inherit F_SSERVER;

string name() { return HIW "连字诀" NOR; }

string *weapon_sk = ({
        "sword", "blade", "staff", "whip", "club", "hammer", "axe"
});

int perform(object me, object target)
{
        object weapon;
        string msg;
        int ap, dp;
        int count;
        int i, attack_time;

        if (! target)
        {
                me->clean_up_enemy();
                target = me->select_opponent();
        }

        if (! target || ! me->is_fighting(target))
                return notify_fail(name() + "只能对战斗中的对手使用。\n");

        if (member_array("cuff", weapon_sk) != -1)
        {
                if( !objectp(weapon=query_temp("weapon", me) )
                    || query("skill_type", weapon) != "cuff" )
                        return notify_fail("你所使用的武器不对，难以施展" + name() + "。\n");
        } else
        {
                if( query_temp("weapon", me) || query_temp("secondary_weapon", me) )
                        return notify_fail(name() + "只能空手使用。\n");
        }

        if ((int)me->query_skill("taiji-quan", 1) < 400)
                return notify_fail("你" + to_chinese("taiji-quan") + "不够娴熟，难以施展" + name() + "。\n");

        if (member_array("cuff", weapon_sk) == -1)
        {
                if (me->query_skill_prepared("cuff") != "taiji-quan")
                        return notify_fail("你没有准备" + to_chinese("taiji-quan") + "，难以施展" + name() + "。\n");
        }

        if( query("neili", me)<300 )
                return notify_fail("你现在的真气不够，难以施展" + name() + "。\n");

        if (! living(target))
                return notify_fail("对方都已经这样了，用不着这么费力吧？\n");

        msg = HIW "$N一声怒嚎，将太极神功提运极至，双拳顿时携着雷霆万钧之势接二连三得攻向$n。。" + "\n" + NOR;

        ap = attack_power(me, "cuff");
        dp = defense_power(target, "dodge");
        attack_time = 6;

        if (ap * 2 / 3 + random(ap) > dp)
        {
                msg += HIM "结果$n闪避不及，$N的拳力掌劲顿时透体而入，口中鲜血狂喷，连退数步。" + "\n" NOR;
                count = ap / 10;
                addn_temp("apply/attack", count, me);
                addn_temp("apply/damage", count, me);
        } else
        {
                msg += NOR + CYN "可是$p识破了$P这一招，斜斜一跃避开。" + "\n" NOR;
                count = 0;
        }

        message_sort(msg, me, target);

        attack_time+=query("jieti/times", me)*2;
        if (attack_time > 13)
                attack_time = 13;

        addn("neili", -attack_time*20, me);

        for (i = 0; i < attack_time; i++)
        {
                if (! me->is_fighting(target))
                        break;

                if (random(2) && ! target->is_busy()) target->start_busy(1);

                COMBAT_D->do_attack(me, target, (member_array("cuff", weapon_sk) == -1 ? 0 : weapon), 0);
        }
        addn_temp("apply/attack", -count, me);
        addn_temp("apply/damage", -count, me);
        me->start_busy(3 + random(attack_time/ 3));

        return 1;
}
