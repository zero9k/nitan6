inherit DREAM_NPC;
#include <ansi.h>

int is_snake(){return 1;}

void create()
{
        set_name(HIY "毒蛇王" NOR, ({ "dushe wang", "dushe", "wang" }));
        set("long", RED "这是毒蛇中的王者，体形巨大，剧毒无比。\n" NOR);

        set("age", 90);
        set("str", 300);
        set("dex", 200);
        set("max_qi", 1992200);
        set("eff_qi", 1992200);
        set("max_ging", 1192200);
        set("eff_jing", 1100000);
        set("max_jing", 1100000);
        set("jingli", 1100000);
        set("max_jingli", 1100000);
        set("jingli", 1100000);
        set("eff_jingli", 1100000);
        set("combat_exp", 5000000000);
        set("max_neili", 650000);
        set("neili", 650000);
         set("no_nuoyi", 1); // 不被挪移影响

        set("snake_poison", ([
                "level"  : 900,
                "perhit" : 460,
                "remain" : 400,
                "maximum": 400,
                "supply" : 350,
        ]));

        set_temp("apply/parry", 150);
        set_temp("apply/dodge", 350);
        set_temp("apply/attack", 150);
        set_temp("apply/defense", 380);
        set_temp("apply/unarmed_damage", 580);
        set_temp("apply/armor", 380);

                set("gift/exp", 500);
                set("gift/pot", 220);
                set("oblist", ([
                        "/clone/fam/gift/str3" : 2,
                        "/clone/fam/gift/lan-feihun3" : 2,
                        "/clone/armor/moling-zhiyi3" : 3,                        
                        "/clone/armor/shenwu-xiangquan":20,
                        "/clone/armor/shenwu-xiangquan2":20,
                        "/clone/armor/shenwu-xiangquan3":30,
                        "/clone/armor/yinyang-erhuan3":8,
                        "/clone/fam/item/wannian-hong":10,
                ]));
        setup();
            add_money("gold", 3);
}

mixed hit_ob(object me, object ob, int damage)
{
        mapping p;
        string  msg;
        int force;
        int dur;

        p=query("snake_poison", me);
        if (! p) return;
        if (damage / 3 + random(damage * 2 / 3) <
            query_temp("apply/armor", ob) )
        {
                // Defeated by armor
                return;
        }

        msg = "";
        dur = p["perhit"];
        if (dur > (int)p["remain"]) dur = p["remain"];
        p["remain"] = (int)p["remain"] - dur;
        this_object()->apply_condition("poison-supply", 1);
        if (! dur) return;


        force = (int)ob->query_skill("force");
        if (random(force / 2) + force / 2 >= (int)p["level"] &&
            query("neili", ob)>damage/5 )
        {
                if( query("qi", ob)<150 )
                {
                        msg = HIR "你觉得伤口有些发麻，连忙运功化解，但"
                              "是一时体力不支，难以施为。\n" NOR;
                } else
                if( query("jing", ob)<60 )
                {
                        msg = HIR "你觉得伤口有些发麻，连忙运功化解，但"
                              "是一时精神不济，难以施为。\n" NOR;
                } else
                if( query("neili", ob)<damage/5+50 )
                {
                        msg = HIR "你觉得伤口有些发麻，连忙运功化解，但"
                              "是一时内力不足，难以施为。\n" NOR;
                } else
                {
                        addn("neili", -damage/5, ob);
                        ob->receive_damage("qi", 20);
                        ob->receive_damage("jing", 10);
                        return HIM "你觉得被咬中的地方有些发麻，连忙运功"
                               "化解毒性。\n" NOR;
                }
        }

        if (ob->affect_by("poison", ([ "level" : (int)p["level"] / 2,
                                       "name"  : "蛇毒",
                                       "id"    : "nature poison",
                                       "duration" : dur / 2, ])))
        {
                msg += HIR "$n" HIR "脸色一变，只觉被咬中的地方一阵麻木。\n" NOR;
        }
        return msg;
}
