#include <ansi.h>
#include <combat.h>

#define LUAN "��" HIW "�ٻ�����" NOR "��"

inherit F_SSERVER;


int perform(object me, object target)
{
        int skill, ap, dp, damage;
        string msg;

        if( userp(me) && !query("can_perform/baihua-cuoquan/luan", me) )
                return notify_fail("�㻹û���ܵ�����ָ�㣬����������" LUAN "��\n");

        if (! target)
        {
                me->clean_up_enemy();
                target = me->select_opponent();
        }

        if (! target || ! me->is_fighting(target))
                return notify_fail(LUAN "ֻ�ܶ�ս���еĶ���ʹ�á�\n");

        skill = me->query_skill("baihua-cuoquan", 1);

        if (skill < 120)
                return notify_fail("��İٻ���ȭ�ȼ�����������ʩչ" LUAN "��\n");

        if( query("neili", me)<200 )
                return notify_fail("�����������������ʩչ" LUAN "��\n");
 
        if (me->query_skill_mapped("unarmed") != "baihua-cuoquan")
                return notify_fail("��û�м����ٻ���ȭ������ʩչ" LUAN "��\n");

        if (me->query_skill_prepared("unarmed") != "baihua-cuoquan")
                return notify_fail("������û��׼��ʹ�ðٻ���ȭ���޷�ʹ��" LUAN "��\n");

        if (target->is_busy())
                return notify_fail(target->name() + "Ŀǰ���Թ˲�Ͼ���ŵ������ɡ�\n");

        if (! living(target))
                return notify_fail("�Է����Ѿ������ˣ��ò�����ô�����ɣ�\n");

        msg = HIW "$N" HIW "�ٲ�������˫�Ƴ�$n" HIW "����������Ʒ�ȭӰ��"
              "�ص���������һ�С��ٻ����ҡ���\n" NOR;
        addn("neili", -50, me);

        ap = attack_power(me, "unarmed");
        dp = defense_power(target, "parry");
        if (ap / 2 + random(ap) > dp)
        {
                addn("neili", -150, me);
                damage = damage_power(me, "unarmed");
                msg += COMBAT_D->do_damage(me, target, UNARMED_ATTACK, damage, 50,
                                           HIW "$nֻ�е�ͷ��Ŀѣ��ֻ��$N���ơ���צ��"
                                           "��ȭ����ָ����ǵص����Լ�������λϮ����\n"
                                           "ֻһ˲�䣬ȫ�����Ѷ�����ʮ���˺ۣ�" 
                                           HIR "��Ѫ" HIW "��к��ֹ��\n" NOR);
                me->start_busy(1);
                if (! target->is_busy())
                        target->start_busy(ap / 500 + 2);
        } else
        {
                msg += CYN "$n" CYN "ֻ��$N" CYN "ȭ����ӿ���������ӣ���æ�����"
                       "�����������⿪����\n" NOR;
                addn("neili", -80, me);
                me->start_busy(3);
        }
        message_combatd(msg, me, target);

        return 1;
}