// This program is a part of NT MudLIB

#include <ansi.h>
#include <command.h>

inherit BOSS;

void random_move();
void death_msg();

void create()
{
        set_name(HIR "��͢�ӷ�" NOR, ({ "chaoting taofan", "chaoting", "taofan" }) );
        set("title", HIG "���������ġ�" NOR);
        set("gender", "����");
        set("age", 38);
        set("long", @LONG
�����˳�ͨ͢������������˵��Խ��Ǳ�ӣ��������ģ�ʮ���׺���
LONG );
        set("combat_exp", 1000000);
        set("shen_type", 0);
        set("max_neili", 6000);
        set("neili", 6000);
        set("max_jing", 8000);
        set("jing", 8000);
        set("max_qi", 150000);
        set("eff_qi", 150000);
        set("qi", 150000);
        set("jingli", 8000);
        set("max_jingli", 8000);
        set("level", 20);


        set("no_clean_up", 1);

        //set("no_nuoyi", 1); // ����Ų��Ӱ��

        set("str", 27);
        set("int", 25);
        set("con", 25);
        set("dex", 25);

        set_skill("unarmed", 120);
        set_skill("sword", 120);
        set_skill("parry", 120);
        set_skill("dodge", 120);
        set_skill("force", 120);
        set_skill("cuff", 120);

        set_skill("shaolin-xinfa", 120);
        set_skill("huashan-sword", 120);
        set_skill("luohan-quan", 120);
        set_skill("shaolin-shenfa", 120);

        map_skill("cuff", "luohan-quan");
        map_skill("dodge", "shaolin-shenfa");
        map_skill("parry", "luohan-quan");
        map_skill("force", "shaolin-xinfa");

        prepare_skill("cuff", "huashan-quan");

        set("chat_chance_combat", 120);
        set("chat_msg_combat", ({
                (: perform_action, "cuff.xiangmo" :),
                (: exert_function, "recover" :)
        }) );

        set("chat_chance", 30);
        set("chat_msg", ({ (: random_move :) }));

        set("jiali", 20);

        set("end_time", 300);
        set("death_msg", (: death_msg :));

        set("rewards", ([
                "exp"   : 10000,
                "pot"   : 3000,
                "mar"   : 150,
                "score" : 30,
                "gold"  : 5,
        ]));

        set("drops", ([
                "RA&RANDOM10"    :       100,   // �ͼ���ͨװ��
                "RA&RANDOM20"    :       40,    // �ͼ���ͨװ��
                "FI&/clone/goods/enchant-scroll" :   20,
                "FI&/clone/goods/sun"    :   20,
                "FI&/clone/goods/moon"   :   20,
                "FI&/clone/armor/gangtie-kai"               :           30,
                "FI&/clone/armor/jinsi-shoutao"             :           30,
                "FI&/clone/armor/jinsi-beixin"              :           20,
                "FI&/clone/armor/shengling-yaodai"          :           20,
                "FI&/clone/tessera/rune02"  :   1,
        ]));

        setup();

        carry_object("/clone/cloth/cloth")->wear();
        set_temp("born_time", time());
        if (clonep()) keep_heart_beat();

}

int accept_fight(object ob)
{
        kill_ob(ob);
        return -1;
}

int accept_kill(object ob)
{
        return 1;
}

int accept_hit(object ob)
{
        kill_ob(ob);
        return -1;
        return 1;
}

mixed hit_ob(object me, object ob, int damage_bouns)
{
        ob->start_busy(4 + random(8));
        me->receive_wound("qi", 100 + random(100), ob);
        return HIY "$N" HIY "���һ����ƴ�����������Ƶ�$n" HIY "��æ���ҡ�\n" NOR;
}

void heart_beat()
{
        if (query("neili") < 1 && random(100) == 1)
                set("neili", query("max_neili"));

        ::heart_beat();
}

void random_move()
{
        object env;

        if (time() - query_temp("born_time") > 1800)
        {
                env = environment(this_object());
                message_vision("$N����ææ�����ˡ�\n", this_object());

/*
                message("vision", HIG"��" HIR"����" HIG"��" HIW +
                        env->short() + HIW "(" + LOOK_CMD->locate(base_name(env)) + ")һ�����ֵ�" +
                        HIR + this_object()->short() + HIR "��ʧ�ˡ�\n" NOR, users());
*/
                CHANNEL_D->channel_broadcast("mess", "��˵" + env->short() + HIW "(" + LOOK_CMD->locate(base_name(env)) + ")һ�����ֵ�" +
                        HIR + this_object()->short() + HIG "��ʧ�ˡ�\n" NOR);

                destruct(this_object());
                return;
        }
        //::random_move();
}

void death_msg()
{
        object dob;
        int money;

        command("chat �ߣ�����ʮ���������һ���ú� ...");
        CHANNEL_D->channel_broadcast("rumor", "��˵" + name() + HIM "��������ʿ��ɱ��" NOR);

        dob = query_last_damage_from();
        // �õ���͢����
        if (objectp(dob) && environment(dob) == environment(this_object()))
        {
                money = 500 + random(300);
                addn("balance", money);
                set_name(HIY "��͢�ʰ�" NOR, ({"chaoting huangbang"}));
                CHANNEL_D->do_channel(this_object(), "chat", dob->name() + HIC "�����ӷ��й����ʹͻƽ� " +
                                      sprintf("%d", money) + " ����\n" NOR);
        }
}