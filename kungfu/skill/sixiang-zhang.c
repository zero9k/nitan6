inherit SKILL;

mapping *action = ({
([        "action" : "$N����������һʽ��������ɳ����˫����£���У�����$n�����",
        "force" : 70,
        "attack": 5,
        "dodge" : 38,
        "parry" : 38,
        "damage": 1,
        "lvl"   : 0,
        "skill_name" : "������ɳ",
        "damage_type" : "����"
]),
([        "action" : "$Nһʽ���׶���������˫����Ѹ�ײ����ڶ�֮�ƣ�����$n������̫��Ѩ",
        "force" : 95,
        "attack": 8,
        "dodge" : 43,
        "parry" : 43,
        "damage": 4,
        "lvl"   : 30,
        "skill_name" : "�׶�����",
        "damage_type" : "����"
]),
([        "action" : "$Nʹһʽ��ժ�ǻ�����������������˫�����֣�һ������$n�ĺ���ȥ",
        "force" : 120,
        "attack": 13,
        "dodge" : 51,
        "parry" : 51,
        "damage": 8,
        "lvl"   : 60,
        "skill_name" : "ժ�ǻ���",
        "damage_type" : "����"
]),
([        "action" : "$Nһʽ�����Ľ�ħ����˫�����÷��ã��ƶ��Ƕ��������缱����ʹ$n��������",
        "force" : 140,
        "attack": 15,
        "dodge" : 65,
        "parry" : 65,
        "damage": 12,
        "lvl"   : 100,
        "skill_name" : "���Ľ�ħ",
        "damage_type" : "����"
]),
});

int valid_enable(string usage) { return usage == "strike" ||  usage == "parry"; }

int valid_learn(object me)
{
        if( query_temp("weapon", me) || query_temp("secondary_weapon", me) )
                return notify_fail("�������Ʒ�������֡�\n");

        if (me->query_skill("force") < 20)
                return notify_fail("����ڹ���򲻹��������������Ʒ���\n");

        if( query("max_neili", me)<50 )
                return notify_fail("�������̫�����޷��������Ʒ���\n");

        if (me->query_skill("strike", 1) < me->query_skill("sixiang-zhang", 1))
                return notify_fail("��Ļ����Ʒ�̫��޷���������������Ʒ���\n");

        return 1;
}

string query_skill_name(int level)
{
        int i;
        for(i = sizeof(action)-1; i >= 0; i--)
                if(level >= action[i-1]["lvl"])
                        return action[i-1]["skill_name"];
}

mapping query_action(object me, object weapon)
{
        int i, level;
        level   = (int)me->query_skill("sixiang-zhang",1);
        for(i = sizeof(action); i > 0; i--)
                if(level > action[i-1]["lvl"])
                        return action[NewRandom(i, 20, level/5)];
}

int practice_skill(object me)
{
        if( query("qi", me)<50 )
                return notify_fail("�������̫���ˡ�\n");

        if( query("neili", me)<60 )
                return notify_fail("�������������ϰ�����Ʒ���\n");

        if (me->query_skill("sixiang-zhang", 1) < 100)
                me->receive_damage("qi", 20);
        else
                me->receive_damage("qi", 30);

        addn("neili", -40, me);
        return 1;
}
string perform_action_file(string action)
{
        return __DIR__"sixiang-zhang/"+ action;
}