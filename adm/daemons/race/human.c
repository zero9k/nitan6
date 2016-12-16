// human.c
// From ES2
// Modified by Xiang@XKX

// A normal human is at least 40 kg weight
#define BASE_WEIGHT 40000

inherit F_DBASE;
inherit F_SKILL;

mapping *combat_action = ({
([      "action":               "$N��ȭ����$n��$l",
        "damage_type":  "����",
]),
([      "action":               "$N��$n��$lһץ",
        "damage_type":  "ץ��",
]),
([      "action":               "$N��$n��$l�ݺݵ�����һ��",
        "damage_type":  "����",
]),
([      "action":               "$N����ȭͷ��$n��$l��ȥ",
        "damage_type":  "����",
]),
([      "action":               "$N��׼$n��$l�����ӳ�һȭ",
        "damage_type":  "����",
]),
});

void create()
{
        seteuid(getuid());
}

void setup_human(object ob)
{
        mapping my;
        int s;
        int x, y;
        int limit;
        int xism_age;
        int skill;
        int jing_age;
        int add_point;
        int die_point;
        int r;

        my = ob->query_entire_dbase();
        ob->set_default_action(__FILE__, "query_action");
	set("default_actions", (: call_other, __FILE__, "query_action" :), ob);
        if (! stringp(my["unit"])) my["unit"] = "λ";
        if (! stringp(my["gender"])) my["gender"] = "����";
        if (/*undefinedp*/!(my["can_speak"])) my["can_speak"] = 1;
        if (! stringp(my["attitude"])) my["attitude"] = "peaceful";
        if (! pointerp(my["limbs"])) my["limbs"] = ({
                "ͷ��", "����", "�ؿ�", "����", "���", "�Ҽ�", "���",
                "�ұ�", "����", "����", "����", "����", "����", "С��",
                "����", "����", "����", "���", "�ҽ�", "���", "�Ҷ�"
        });

        if (/*undefinedp*/!(my["age"])) my["age"] = 14;
        if (/*undefinedp*/!(my["str"])) my["str"] = 10 + random(21);
        if (/*undefinedp*/!(my["con"])) my["con"] = 10 + random(21);
        if (/*undefinedp*/!(my["dex"])) my["dex"] = 10 + random(21);
        if (/*undefinedp*/!(my["int"])) my["int"] = 10 + random(21);
        if (/*undefinedp*/!(my["per"])) my["per"] = 10 + random(21);
        if (/*undefinedp*/!(my["kar"])) my["kar"] = 10 + random(21);

        if (!userp(ob) && /*undefinedp*/!(my["max_jingli"]))
        {
                limit=query("combat_exp", ob)/1000;
                my["max_jingli"] = limit;
                my["jingli"] = limit;
        }

        if (userp(ob) || /*undefinedp*/!(my["max_jing"]))
        {
                s = ob->query_int()*5 + ob->query_con()+ob->query_dex()+ob->query_str();
                my["max_jing"] = 100;
                if (/*undefinedp*/!(my["born"]))
                        ; else
                if (my["age"] < 11)
                        my["max_jing"] = 50 + my["age"] * s * 2 / 3;
                else
                if (my["age"] < 24)
                        my["max_jing"] += (my["age"] - 11) * s * 2 / 3;
                else
                        my["max_jing"] += (24 - 11) * s * 2 / 3;

                if ((int)my["max_jingli"] > 0)
                        my["max_jing"] += (int)my["max_jingli"] / 3;

                // ���������������ǰ������������󳤾�
                if (userp(ob) && mapp(my["family"])) {
                        if ( my["family"]["family_name"] == "������" )
                                xism_age = (int)ob->query_skill("mahayana", 1);
                        else if ( my["family"]["family_name"] == "������" )
                                xism_age = (int)ob->query_skill("buddhism", 1);
                        else if ( my["family"]["family_name"] == "���ϻ���" )
                                xism_age = (int)ob->query_skill("buddhism", 1);
                        else if ( my["family"]["family_name"] == "ѩɽ��" || my["family"]["family_name"] == "Ѫ����" )
                                xism_age = (int)ob->query_skill("lamaism", 1);
                }
                else
                        xism_age = 0;

                if (xism_age > 250) xism_age = 250;

                if (xism_age > 39) {
                        xism_age = xism_age/2;
                        if (my["age"] <= 30) xism_age -= my["age"];
                        else xism_age -= 30;

                        skill = ob->query_skill("force");
                        if (xism_age > 0 && skill > 0) {
                                skill = xism_age * (skill/20);
                                if ( my["family"]["family_name"] == "������" )
                                {
                                        my["max_jing"] += skill;
                                        //my["max_qi"] += skill/2;
                                }
                                else if ( my["family"]["family_name"] == "������" )
                                {
                                        my["max_jing"] += skill;
                                        //my["max_qi"] += skill/2;
                                }
                                else if ( my["family"]["family_name"] == "���ϻ���" )
                                {
                                        my["max_jing"] += skill*2/3;
                                        //my["max_qi"] += skill/2;
                                }
                                else
                                {
                                        my["max_jing"] += skill*3/4;
                                        //my["max_qi"] += skill/2;
                                }
                        }
                }

                // ��ɲ���꣺ÿ��һ�Σ�ؤ�ﾫ������ֵ
                if (userp(ob) && mapp(my["family"]) && my["family"]["family_name"] == "ؤ��")
                {
                        add_point = my["combat_exp"]/50000;
                        if (mapp(my["combat"]))
                        {
                                die_point = my["combat"]["dietimes"];
                                if (die_point > add_point) die_point = add_point;

                                if (die_point < 100)
                                        my["max_jing"] += ob->query_con() * die_point * 10;
                                else
                                        my["max_jing"] += ob->query_con() * 100 * 10;
                        }
                }

                // ��ɽ��ϼ�񹦣�������ǰ������������󳤾�
                if (userp(ob) && mapp(my["family"]) && my["family"]["family_name"] == "��ɽ��"
                && (jing_age = (int)ob->query_skill("zixia-shengong", 1)/3) > 39)
                {
                        if (jing_age > 250) jing_age = 250;
                        jing_age = jing_age/2;

                        if (my["age"] <= 30) jing_age -= my["age"];
                        else jing_age -= 30;

                        skill = (int)ob->query_skill("force");

                        if ( jing_age > 0 )
                                my["max_jing"] += jing_age * (skill/30);
                }

                // ��Ĺ��Ů�ķ���������ǰ������������󳤾�
                if (userp(ob) && mapp(my["family"]) && my["family"]["family_name"] == "��Ĺ��"
                        && (jing_age =(int)ob->query_skill("yunv-xinjing", 1)) > 39)
                {
                        if (jing_age > 250) jing_age = 250;
                        jing_age = jing_age/2;

                        if (my["age"] <= 30) jing_age -= my["age"];
                        else jing_age -= 30;

                        skill = (int)ob->query_skill("force");

                        if ( jing_age > 0 )
                                my["max_jing"] += jing_age * (skill/30);
                }

                // �һ����������У�������ǰ������������󳤾�����Ч��С
                if (userp(ob) && mapp(my["family"]) && my["family"]["family_name"] == "�һ���"
                        && (xism_age=(int)ob->query_skill("qimen-wuxing", 1)/3) > 39)
                {
                        if (xism_age > 250) xism_age = 250;
                        xism_age = xism_age/2;

                        if (my["age"] <= 30) xism_age -= my["age"];
                        else xism_age -= 30;

                        skill = (int)ob->query_skill("force");

                        if (xism_age > 0 ) my["max_jing"] += xism_age * (skill/35);
                }

                // ����ʥ����ڤ��������ǰ������������󳤾�����Ч��С
                if (userp(ob) && mapp(my["family"]) && my["family"]["family_name"] == "����"
                        && (xism_age=(int)ob->query_skill("shenghuo-shengong", 1)/3) > 39)
                {
                        if (xism_age > 250) xism_age = 250;
                        xism_age = xism_age/2;

                        if (my["age"] <= 30) xism_age -= my["age"];
                        else xism_age -= 30;

                        skill = (int)ob->query_skill("force");

                        if (xism_age > 0 ) my["max_jing"] += xism_age * (skill/35);
                }

                // ����������¹⻪��������ǰ������������󳤾�����Ч��С
                if (userp(ob) && mapp(my["family"]) && my["family"]["family_name"] == "�������"
                        && (xism_age=(int)ob->query_skill("riyue-guanghua", 1)/3) > 39)
                {
                        if (xism_age > 250) xism_age = 250;
                        xism_age = xism_age/2;

                        if (my["age"] <= 30) xism_age -= my["age"];
                        else xism_age -= 30;

                        skill = (int)ob->query_skill("force");

                        if (xism_age > 0 ) my["max_jing"] += xism_age * (skill/35);
                }

                // �����������������������ǰ������������󳤾�����Ч��С
                if (userp(ob) && mapp(my["family"]) && my["family"]["family_name"] == "�������"
                        && (xism_age=(int)ob->query_skill("dispel-poison", 1)) > 39)
                {
                        if (xism_age > 250) xism_age = 250;
                        xism_age = xism_age/2;

                        if (my["age"] <= 30) xism_age -= my["age"];
                        else xism_age -= 30;

                        skill = (int)ob->query_skill("force");

                        if (xism_age > 0 ) my["max_jing"] += xism_age * (skill/35);
                }

                if (userp(ob) && my["breakup"])
                        my["max_jing"] += my["max_jing"];

                if (userp(ob) && my["animaout"])
                        my["max_jing"] += my["max_jing"];

#ifdef DB_SAVE
                if (userp(ob) && MEMBER_D->is_valid_member(ob))
                        my["max_jing"] += my["max_jing"] / 2;
#endif

                if( userp(ob) && (r=query("reborn/times", ob)) )
                        my["max_jing"] += my["max_jing"] * r;

                if( userp(ob) && query("jingmai/jing", ob) )
                        my["max_jing"] += query("jingmai/jing", ob);

                if( userp(ob) && query("jingmai/finish", ob) )
                        my["max_jing"] += ZHOUTIAN_D->query_jingmai_effect("jing"); 

                if( query_temp("apply/max_jing", ob) )
                        my["max_jing"]+=query_temp("apply/max_jing", ob);
        }

        if (userp(ob) || /*undefinedp*/!(my["max_qi"]))
        {
                s = ob->query_con()*5 + ob->query_str() + ob->query_con() + ob->query_dex();
                my["max_qi"] = 100;
                if (/*undefinedp*/!(my["born"]))
                        ; else
                if (my["age"] < 11)
                        my["max_qi"] = 60 + my["age"] * my["con"] / 2;
                else
                if (my["age"] < 27)
                        my["max_qi"] += (my["age"] - 11) * s * 2 / 3;
                else
                        my["max_qi"] += (27 - 11) * s * 2 / 3;

                if ((int)my["max_neili"] > 0)
                        my["max_qi"] += (int)my["max_neili"] / 4;

                // �䵱̫������
                if (userp(ob) && mapp(my["family"]) && my["family"]["family_name"] == "�䵱��" &&
                    (x = (int)ob->query_skill("taoism", 1)) > 39 &&
                    (y = (int)ob->query_skill("taiji-shengong", 1)) > 39)
                {
                        if (x > 350) x = (x - 350) / 2 + 350;
                        if (y > 350) y = (y - 350) / 2 + 350;
                        if (x > 200) x = (x - 200) / 2 + 200;
                        if (y > 200) y = (y - 200) / 2 + 200;

                        my["max_qi"] += (x + 100 ) * (y + 100) / 100;
                }

                //ȫ�����칦����
                if (userp(ob) && mapp(my["family"]) && my["family"]["family_name"] == "ȫ���" &&
                    (x = (int)ob->query_skill("taoism", 1)) > 39 &&
                    (y = (int)ob->query_skill("xiantian-gong", 1)) > 39)
                {
                        if (x > 350) x = (x - 350) / 2 + 350;
                        if (y > 350) y = (y - 350) / 2 + 350;
                        if (x > 200) x = (x - 200) / 2 + 200;
                        if (y > 200) y = (y - 200) / 2 + 200;

                        my["max_qi"] += (x + 100 ) * (y + 100) / 100;
                }

                // ���޾۶�������������ǰ���������������
                if (userp(ob) &&  mapp(my["family"]) && my["family"]["family_name"] == "������"
                && (xism_age=(int)ob->query_skill("poison", 1)/3) > 39)
                {
                        if (xism_age > 250) xism_age = 250;
                        xism_age = xism_age/2;

                        if (my["age"] <= 30) xism_age -= my["age"];
                        else xism_age -= 30;

                        skill = (int)ob->query_skill("huagong-dafa",1);

                        if (xism_age > 0 )
                                my["max_qi"] += xism_age * (skill/30);
                }

                // ����ɽׯ�۶�������������ǰ���������������
                if (userp(ob) && mapp(my["family"]) && my["family"]["family_name"] == "ŷ������"
                     && (xism_age=(int)ob->query_skill("poison", 1)/3) > 39)
                {
                        if (xism_age > 250) xism_age = 250;
                        xism_age = xism_age/2;
                        if (my["age"] <= 30) xism_age -= my["age"];
                        else xism_age -= 30;

                        skill = (int)ob->query_skill("hamagong",1);

                        if (xism_age > 0 )
                                my["max_qi"] += xism_age * (skill/30);
                }

                // ��ң����ң��ѧ������������ǰ���������������
                if (userp(ob) && mapp(my["family"]) && my["family"]["family_name"] == "��ң��"
                     && (xism_age=(int)ob->query_skill("xiaoyao-qixue", 1)) > 39)
                {
                        if (xism_age > 250) xism_age = 250;
                        xism_age = xism_age/2;

                        if (my["age"] <= 30) xism_age -= my["age"];
                        else xism_age -= 30;

                        skill = (int)ob->query_skill("force");

                        if (xism_age > 0 )
                                my["max_qi"] += xism_age * (skill/40);
                }

                // ���չ��˻��񹦣�������ǰ���������������
                if (userp(ob) && mapp(my["family"]) && my["family"]["family_name"] == "���չ�"
                     && (xism_age=(int)ob->query_skill("bahuang-gong", 1)/3) > 39)
                {
                        if (xism_age > 250) xism_age = 250;
                        xism_age = xism_age/2;

                        if (my["age"] <= 30) xism_age -= my["age"];
                        else xism_age -= 30;

                        skill = (int)ob->query_skill("force",1);

                        if (xism_age > 0 )
                                my["max_qi"] += xism_age * (skill/40);
                }

                // Ľ�������ϻ��ķ�������������ǰ���������������
                if (userp(ob) && mapp(my["family"]) && my["family"]["family_name"] == "Ľ������"
                     && (xism_age=(int)ob->query_skill("zihui-xinfa", 1)/3) > 39)
                {
                        if (xism_age > 250) xism_age = 250;
                        xism_age = xism_age/2;

                        if (my["age"] <= 30) xism_age -= my["age"];
                        else xism_age -= 30;

                        skill = (int)ob->query_skill("force");

                        if (xism_age > 0 )
                                my["max_qi"] += xism_age * (skill/40);
                }

                // ��������������������������ǰ���������������
                if (userp(ob) && mapp(my["family"]) && my["family"]["family_name"] == "������"
                     && (xism_age=(int)ob->query_skill("shenlong-mixin", 1)) > 39)
                {
                        if (xism_age > 250) xism_age = 250;
                        xism_age = xism_age/2;

                        if (my["age"] <= 30) xism_age -= my["age"];
                        else xism_age -= 30;

                        skill = (int)ob->query_skill("force");

                        if (xism_age > 0 )
                                my["max_qi"] += xism_age * (skill/50);
                }

                if (userp(ob) && my["breakup"])
                        my["max_qi"] += my["max_qi"] * 2;

#ifdef DB_SAVE
                if (userp(ob) && MEMBER_D->is_valid_member(ob))
                        my["max_qi"] += my["max_qi"] / 2;
#endif

                if( userp(ob) && (r=query("reborn/times", ob)) )
                        my["max_qi"] += my["max_qi"] * r;

                if( userp(ob) && query("jingmai/qi", ob) )
                        my["max_qi"]+=query("jingmai/qi", ob);

                if( userp(ob) && query("jingmai/finish", ob) )
                        my["max_qi"] += ZHOUTIAN_D->query_jingmai_effect("qi"); 

                // ����superҩƷ
                if( userp(ob) && query("drug_addqi", ob) )
                        my["max_qi"] += query("drug_addqi", ob);

                if( query_temp("apply/max_qi", ob) )
                        my["max_qi"]+=query_temp("apply/max_qi", ob);
        }

        if (! ob->query_weight())
                ob->set_weight((BASE_WEIGHT + (my["str"] - 10) * 2000) * 7 / 10);
}

mapping query_action()
{
        return combat_action[random(sizeof(combat_action))];
}