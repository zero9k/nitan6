// wuji4.c

inherit BOOK;

string* titles = ({
        "����ָ��",
        "���ư˴�",
        "����ָ��",
        "����Ҷָ",
        "һ����ɢ",
});

string* skills = ({
        "xiuluo-zhi",
        "shenzhang-bada",
        "wuxiang-zhi",
        "duoluoye-zhi",
        "yipai-liangsan",
});

void create()
{
        int i = random(sizeof(titles));

        set_name(titles[i], ({ "shaolin wuji", "wuji" }));
        set_weight(200);
        set("unit", "��");
        set("long", "����һ��" + titles[i] + "\n");
        set("value", 500);
        set("material", "paper");
        set("skill", ([
                "name": skills[i],      // name of the skill
                "exp_required": 10000,  // minimum combat experience required
                "jing_cost": 35 + random(30),// jing cost every time study this
                "difficulty":   30,     // the base int to learn this skill
                "max_skill":    99      // the maximum level you can learn
        ]) );
}