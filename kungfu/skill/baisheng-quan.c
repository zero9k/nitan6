inherit SKILL;

mapping *action = ({
([      "action": "$N����һ��������΢��������һ����һ�С�è�ڡ���һȭֱͱ$n��$l",
        "force" : 130,
        "dodge" : 5,
        "parry" : 7,
        "attack": 14,
        "damage": 4,
        "lvl"   : 0,
        "skill_name" : "è��",
        "damage_type": "����"
]),
([      "action": "$N����һЦ����ȭ�������ϣ���ȭƽƽ������һ�С��ù������������$n",
        "force" : 145,
        "dodge" : 18,
        "parry" : 17,
        "attack": 16,
        "damage": 6,
        "lvl"   : 20,
        "skill_name" : "�ù�",
        "damage_type": "����"
]),
([      "action": "$N��$nһ����ȣ�ʹһ�С�ӥ��������ȭ�������漴��ȭ���ϣ�������������$n��$l",
        "force" : 157,
        "dodge" : 16,
        "parry" : 19,
        "attack": 17,
        "damage": 11,
        "lvl"   : 40,
        "skill_name" : "ӥ��",
        "damage_type": "����"
]),
([      "action": "$N�ƺ�һ����˫ȭ���Ϸֿ���һ�ǡ����ӷ�������ȭ�����ߣ�����ͬʱ����$n��$l",
        "force" : 165,
        "dodge" : 24,
        "parry" : 21,
        "attack": 19,
        "damage": 14,
        "lvl"   : 60,
        "skill_name" : "���ӷ���",
        "damage_type": "����"
]),
([      "action": "$Nʩ����ϸ���ɡ���һ�����һȭ��մ����ȭ��ֱ��$n��$l",
        "force" : 185,
        "dodge" : 24,
        "parry" : 28,
        "attack": 33,
        "damage": 19,
        "lvl"   : 80,
        "skill_name" : "ϸ����",
        "damage_type": "����"
]),
([      "action": "$Nһ����Х��˫ȭ����������һ�С����ӽš���ȭ���ܲ�$n��ǰ������",
        "force" : 197,
        "dodge" : 28,
        "parry" : 30,
        "attack": 36,
        "damage": 21,
        "lvl"   : 100,
        "skill_name" : "���ӽ�",
        "damage_type": "����"
]),
([      "action": "$Nŭ��һ������շ���һʽ�������项��˫ȭ�Ӹ����£����봷��$n",
        "force" : 285,
        "dodge" : 24,
        "parry" : 21,
        "attack": 47,
        "damage": 24,
        "lvl"   : 120,
        "skill_name" : "������",
        "damage_type": "����"
]),
});

int valid_enable(string usage) { return usage == "cuff" || usage == "parry"; }

int valid_learn(object me)
{
        if( query_temp("weapon", me) || query_temp("secondary_weapon", me) )
                return notify_fail("����ʤ��ȭ������֡�\n");

        if ((int)me->query_skill("force") < 50)
                return notify_fail("����ڹ���򲻹����޷�ѧ��ʤ��ȭ��\n");

        if( query("max_neili", me)<300 )
                return notify_fail("�������̫�����޷�����ʤ��ȭ��\n");

        if ((int)me->query_skill("cuff", 1) < 40)
                return notify_fail("��Ļ���ȭ�Ż��̫ǳ��\n");

        if ((int)me->query_skill("cuff", 1) < (int)me->query_skill("baisheng-quan", 1))
                return notify_fail("��Ļ���ȭ��ˮƽ���ޣ��޷���������İ�ʤ��ȭ��\n");

        return 1;
}

string query_skill_name(int level)
{
        int i;
        for (i = sizeof(action) - 1; i >= 0; i--)
            if (level >= action[i]["lvl"])
                return action[i]["skill_name"];
}

mapping query_action(object me, object weapon)
{
/* d_e=dodge_effect p_e=parry_effect f_e=force_effect m_e=damage_effect */
        int d_e1 = 20;
        int d_e2 = 40;
        int p_e1 = -35;
        int p_e2 = 15;
        int f_e1 = 200;
        int f_e2 = 300;
        int i, lvl, seq, ttl = sizeof(action);

        lvl = (int) me->query_skill("baisheng-quan", 1);
        for(i = ttl; i > 0; i--)
                if(lvl > action[i-1]["lvl"])
                {
                        seq = i; /* �������������� */
                        break;
                }
        seq = random(seq);       /* ѡ������������ */
        return ([
                "action"      : action[seq]["action"],
                "dodge"       : d_e1 + (d_e2 - d_e1) * seq / ttl,
                "parry"       : p_e1 + (p_e2 - p_e1) * seq / ttl,
                "force"       : f_e1 + (f_e2 - f_e1) * seq / ttl,
                "damage_type" : random(2) ? "����" : "����",
        ]);
}

int learn_bonus() { return 15; }
int practice_bonus() { return 15; }
int success() { return 15; }
int power_point(object me) { return 1.0; }

int practice_skill(object me)
{
        if( query("qi", me)<60 )
                return notify_fail("������������������Ϣһ�������ɡ�\n");

        if( query("neili", me)<50 )
                return notify_fail("������������ˡ�\n");

        me->receive_damage("qi", 50);
        addn("neili", -34, me);

        return 1;
}

string perform_action_file(string action)
{
        return __DIR__"baisheng-quan/" + action;
}