// Code of JHSH
// wuzhuan.c 奇门五转

#include <ansi.h>

//inherit F_CLEAN_UP;
inherit F_DBASE;
inherit F_SSERVER;

int exert(object me, object target)
{
        string msg;
        int i, extra;

        if( query_temp("wuzhuan", me) )
                return notify_fail("你正在运用「奇门五转」心法！\n");

        if( me->query_skill_mapped("force") != "bibo-shengong")
                return notify_fail("你所用的内功与「奇门五转」心法相悖！\n");

        if( me->query_skill("bibo-shengong", 1) < 100 )
                return notify_fail("你的内功修为火候未到，还未领悟「奇门五转」！\n");

        if( query("neili", me)<500 )
                return notify_fail("你的内力不够，劲力不足以施展「奇门五转」！\n");
        
        if( me->query_skill("qimen-wuxing", 1) < 120 )
                return notify_fail("你的奇门五行修为不够，还未领悟「奇门五转」！\n");

        msg = HIG "$N脸上青气大盛，脚下缓缓踩着九宫八卦方位，头顶冒出腾腾白气，";
        msg += "四面隐隐传来潮涌之声，碧波神功已发挥到极致！\n"NOR;

        addn("neili", -200, me);
        
        extra = (int)me->query_skill("qimen-wuxing",1);
        addn_temp("apply/attack", extra/2, me);
        addn_temp("apply/defense", extra/2, me);
        addn_temp("apply/dodge", extra/30, me);

        set_temp("wuzhuan", 1, me);
        set_temp("wuzhuan_hit", extra/60+random(2), me);

        
        call_out("remove_effect", extra/2, me, extra);

        message_vision(msg, me);

        return 1;
}

void remove_effect(object me, int extra)
{
        if (!me) return;

        addn_temp("apply/attack", -extra/2, me);
        addn_temp("apply/defense", -extra/2, me);
        addn_temp("apply/dodge", -extra/30, me);

        delete_temp("wuzhuan_hit", me);
        delete_temp("wuzhuan", me);
        message_vision(CYN"$N只觉真气运转不畅，不得不放慢步法，调息吐纳。\n"NOR,me);
}