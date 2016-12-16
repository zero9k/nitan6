// insect.c ����

#ifndef __INSECT__
#define __INSECT__

// A normal insect is at least 1 g weight
#define BASE_WEIGHT 1
#endif

inherit F_DBASE;

mapping *combat_action_crawl = ({
([
        "action":                "$N������������$n��$l�ݺݵ�һҧ",
        "damage":                30,
        "damage_type":        "ҧ��",
]),
([
        "action":                "$NԾ�����͵���$n��$l����һ��",
        "damage":                10,
        "damage_type":        "����",
]),
([
        "action":                "$N��ת����β�ͼ��׼$n��$lһ��",
        "damage":                20,
        "damage_type":        "����",
]),  
});

mapping *combat_action_fly = ({
([
        "action":                "$N�ɹ���������$n��$l�ݺݵ�һҧ",
        "damage":                30,
        "damage_type":        "ҧ��",
]),
([
        "action":                "$N�������͵���$n��$l����һ��",
        "damage":                10,
        "damage_type":        "����",
]),
([
        "action":                "$N��ת����β�ͼ��׼$n��$lһ��",
        "damage":                20,
        "damage_type":        "����",
]),  
});

void create()
{
        seteuid(getuid());
        set("attitude", "heroism");
}

void setup_insect(object ob)
{
        mapping my;

        my = ob->query_entire_dbase();

        ob->set_default_action(__FILE__, "query_action");
	set("default_actions", (: call_other, __FILE__, "query_action" :), ob);
        
        my["unit"] = "ֻ";
        
        if( undefinedp(my["gender"]) ) my["gender"] = (random(2))?"����":"����";
        if( undefinedp(my["age"]) ) my["age"] = random( 5) + 5;

        if( undefinedp(my["str"]) ) my["str"] = random( 5) + 5;
        if( undefinedp(my["int"]) ) my["int"] = random( 5) + 5;
        if( undefinedp(my["per"]) ) my["per"] = random( 5) + 5;
        if( undefinedp(my["con"]) ) my["con"] = random(10) + 5;
        if( undefinedp(my["dex"]) ) my["dex"] = random(15) + 5;
        if( undefinedp(my["kar"]) ) my["kar"] = random(10) + 5;

        if( undefinedp(my["max_jing"]) ) {
                if( my["age"] <= 5 ) my["max_jing"] = my["age"] * 5;
                else my["max_jing"]  = 25;
        }
        if( undefinedp(my["max_qi"]) ) {
                if( my["age"] <= 5 ) my["max_qi"] = my["age"] * 5;
                else my["max_qi"]  = 25;
        }
        if( undefinedp(my["max_jingli"]) ) {
                if( my["age"] <= 5 ) my["max_jingli"] = my["age"] * 5;
                else my["max_jingli"]  = 25;
        }
        ob->set_default_object(__FILE__);
        if( !ob->query_weight() ) ob->set_weight(BASE_WEIGHT + (my["str"] - 5)* 100);

        if( query("subrace", ob) == "����"){
                set("dead_message",       "\n$N�Ľų���ط��˹��������ˡ�\n\n"        );
                set("unconcious_message", "\n$N�Ľų���ط��˹����������ˡ�\n\n"      );
                set("revive_message",     "\n$Nһ�·������������ݵ��Ĵ�����������\n\n");
                set("leave_msg",          "�Ĳݴ������˳�ȥ"            );
                set("arrive_msg",         "���˹���"                              );
                set("fleeout_message",    "�Ĳݴ������˳�ȥ"            );
                set("fleein_message",     "�������˹�����������Щ����"            );

                set("limbs", ({"ͷ��",        "����",        "����", "����", "ǰ��",        "����", "β��"}) );
        }

        if( query("subrace", ob) == "�ɳ�"){
                set("dead_message",       "\n$N���ڵ��ϣ������˼��¾����ˡ�\n\n"      );
                set("unconcious_message", "\n$N�˵ĵ��ڵ��ϣ����˹�ȥ��\n\n"          );
                set("revive_message",     "\n$N�ſ����������һ�£��ַ���������\n\n"  );
                set("leave_msg",          "�����˵ؽ��ŷ��˳�ȥ"              );
                set("arrive_msg",         "���˵ؽ��ŷ��˹���"                    );
                set("fleeout_message",    "���˵ؽ��ŷ��˳�ȥ"                );
                set("fleein_message",     "�������˹�����������Щ����"            );

                set("limbs", ({"ͷ��",        "����",        "����",        "���", "ǰ��",        "����", "β��"}) );
        }
}

mapping query_action(object me)
{
        if( query("subrace", me) == "����")return combat_action_crawl[random(sizeof(combat_action_crawl))];
        if( query("subrace", me) == "�ɳ�")return combat_action_fly[random(sizeof(combat_action_fly))];
}