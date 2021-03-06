// you.c 幽冥一击
 
#include <ansi.h>
#include <combat.h>
 
inherit F_SSERVER;
 
int perform(object me, object target)
{
        string msg;
        int ap, dp, bs;
        int damage;
        object weapon;

        if (! target)
        {
                me->clean_up_enemy();
                target = me->select_opponent();
        }

        if( query("gender", me) != "无性" && !query("tianmo_jieti", me) )
                return notify_fail("你的性别与日月内功相斥，无法使用此绝招！\n");  

        if( userp(me) && !query("can_perform/pixie-jian/you", me) )
                return notify_fail("你还不会使用「幽冥一击」。\n");

        if (! target || ! me->is_fighting(target))
                return notify_fail("幽冥一击只能对战斗中的对手使用。\n");
 
        if( !objectp(weapon=query_temp("weapon", me)) || 
            query("skill_type", weapon) != "sword" )
                return notify_fail("你没有装备剑，不能用这一招。\n");

        if( me->query_skill_mapped("sword") != "pixie-jian") 
                return notify_fail("你并没有使用辟邪剑法，无法使用此绝招！\n"); 

        if (me->query_skill("pixie-jian", 1) < 400)
                return notify_fail("你的辟邪剑法还不够娴熟，不能使用幽冥一击！\n");

        if( query("neili", me)<300 )
                return notify_fail("你的内力不够，不能使用幽冥一击！\n");
 
        msg = HIR "$N" HIR "忽的扑上前去，霎时便逼近$n" HIR
              "，手中" + weapon->name() + HIR "猛然刺向$p的要害！\n" NOR;
 
        ap=me->query_skill("pixie-jian",1)*3/2+query("dex", me)*20+
             me->query_skill("martial-cognize", 1);
        dp=target->query_skill("parry")+query("dex", target)*20+
             target->query_skill("martial-cognize", 1);

        me->start_busy(2);
        addn("neili", -40, me);

        if (ap * 2 / 3 + random(ap) < dp || !living(target))
        {
                msg += HIG "然而$n" HIG "抵挡得法，将$N" HIG
                       "的攻势一一化解。\n" NOR;
        } else
        {
                target->start_busy(1);
                addn("neili", -60, me);
                damage = ap / 4 + random(ap / 4) - random(80);
                msg += COMBAT_D->do_damage(me, target, WEAPON_ATTACK, damage, 40,
                                           HIR "$n" HIR "急忙后退，阵脚已乱，结果连中$N"
                                           HIR "数招，鲜血溅了一地！\n" NOR);
        }

        message_combatd(msg, me, target);
        return 1;
}
