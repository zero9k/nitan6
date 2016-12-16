#include <ansi.h>
#include <combat.h>

#define LIAN "��" HIG "��������" NOR "��"

inherit F_SSERVER;

int perform(object me, object target)
{
        string msg;
        int ap, dp;
        int attack_time, i;

        if( userp(me) && !query("can_perform/biluo-zhang/lian", me) )
                return notify_fail("����ʹ�õ��⹦��û�����ֹ��ܡ�\n");

        if (! target) target = offensive_target(me);

        if (! target || ! me->is_fighting(target))
                return notify_fail(LIAN "ֻ�ܶ�ս���еĶ���ʹ�á�\n");

        if( query_temp("weapon", me) || query_temp("secondary_weapon", me) )
                return notify_fail(LIAN "ֻ�ܿ���ʩչ��\n");

        if ((int)me->query_skill("biluo-zhang", 1) < 100)
                return notify_fail("������Ʒ�������죬����ʩչ" LIAN "��\n");

        if (me->query_skill_mapped("strike") != "biluo-zhang")
                return notify_fail("��û�м��������Ʒ�������ʩչ" LIAN "��\n");

        if (me->query_skill_prepared("strike") != "biluo-zhang")
                return notify_fail("��û��׼�������Ʒ�������ʩչ" LIAN "��\n");

        if (me->query_skill("force") < 120)
                return notify_fail("����ڹ���Ϊ����������ʩչ" LIAN "��\n");

        if( query("neili", me)<200 )
                return notify_fail("�����ڵ���������������ʩչ" LIAN "��\n");

        if (! living(target))
                return notify_fail("�Է����Ѿ������ˣ��ò�����ô�����ɣ�\n");

        ap = me->query_skill("strike");
        dp = target->query_skill("parry");

        msg = HIC "\n$N" HIC "˫�ƶ�Ȼ������ת��һ�С�" HIG "��������"
              HIC "����˫�ƴ��磬�ѽ�$n" HIC "�������Ʒ�֮�С�" NOR;

        message_sort(msg, me, target);

        if (random(ap) > dp / 2)
        {
                msg = HIR "���$n" HIR "Ŀ��Ͼ�ӣ���ʱ��$N" HIR "��"
                      "����������ʱ��Ŵ��ҡ�\n" NOR;
                addn_temp("apply/attack", 50, me);
        } else
        {
                msg = HIY "$n" HIY "����$N" HIY "�⼸�е���·����"
                      "�ھ��������Ʒ�Ϭ����Ҳֻ��С�ĵֵ���\n" NOR;          
        }
        message_combatd(msg, me, target);

        attack_time += 3 + random(ap / 40);

        if (attack_time > 6)
                attack_time = 6;

        addn("neili", -attack_time*20, me);

        for (i = 0; i < attack_time; i++)
        {
                if (! me->is_fighting(target))
                        break;
                COMBAT_D->do_attack(me, target, 0, 0);
        }
        addn_temp("apply/attack", -50, me);
        me->start_busy(1 + random(attack_time));

        return 1;
}