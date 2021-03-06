// itemd.c
// updated by Rcwiz 2006.03.14

#pragma optimize

#pragma save_binary


#include <ansi.h>
#include <command.h>

#define GIFT_POINT   1

void create() { seteuid(getuid()); }
void reduce_consistence(object item, object me, object victim, int damage);

// 返回爆物品几率
// 1 为正常，2 为双倍
int gift_point(){ return GIFT_POINT; }

// 返回项链内功加成点及内力加成点
// 1 为内功加成 ， 2为内力加成
int neck_add_damage(object me, int k)
{
		mapping neck_enlist = ([
			"蓝宝石"     :     ({ 5, 10 }),
			"红宝石"     :     ({ 10, 5 }),
			"圣龙之眼"   :     ({ 50, 10 }),
			"圣龙之魂"   :     ({ 15, 40 }),
			"神龙梦幻"   :     ({ 50, 30 }),
			"神龙无边"   :     ({ 40, 40 }),
			"妖灵·魂"    :     ({ 50, 60 }),
			"妖灵·魄"    :     ({ 60, 50 }),	
		]);

		object ob; // 项链
		int total_force, total_neili, i, count;
		string *key_enchase, *key_neck_point;
		mapping enchase;

		total_force = 0;
		total_neili = 0;

		if (! objectp(me))return 0;

		if (! objectp( ob = me->query_temp("armor/neck")))return 0;
				
		if (! ob->is_item_make())return 0;
		
		if (ob->query("enchase_all") < 1)return 0;
		
		enchase = ob->query("enchase");

		if (! sizeof(enchase))return 0;

		key_enchase = keys(enchase);
		count = sizeof(key_enchase);
		if (! count)return 0;
		
		
		for (i = 0; i < count; i ++ )
		{
			// 找到对应的镶嵌物品
			key_neck_point = neck_enlist[enchase[key_enchase[i]]["name"]];
			if (sizeof(key_neck_point))
			{
					total_force += key_neck_point[0];
					total_neili += key_neck_point[1];
			} 
		}

		if (k == 1)return total_force;
		if (k == 2)return total_neili;
		
		return 0;
						
}

// 检查项链中是否镶嵌着某两种物品
// 用于检测是否出特效
// 返回0则没有镶嵌，返回正整数为计算出特效的几率
int neck_enchase_item(object me, string item1, string item2)
{
		object ob; // 项链
		string *key_enchase;
		mapping enchase;
		int count, i, i1, i2, result;
		int have10kOB = 0; // 是否有10孔镶嵌物

		if (! objectp(me))return 0;

		if (! objectp( ob = me->query_temp("armor/neck")))return 0;
				
		if (! ob->is_item_make())return 0;
		
		if (ob->query("enchase_all") < 1)return 0;
		
		enchase = ob->query("enchase");

		if (! sizeof(enchase))return 0;

		key_enchase = keys(enchase);
		count = sizeof(key_enchase);
		if (! count)return 0;
		
		i1 = 0;
		i2 = 0;
		for (i = 0; i < count; i ++ )
		{
			// 找到对应的镶嵌物品
			if (enchase[key_enchase[i]]["name"] == item1)i1 ++;
			if (enchase[key_enchase[i]]["name"] == item2)i2 ++;	
			
			// 是否镶嵌10孔宝石
			if (enchase[key_enchase[i]]["spe_data"]["luhua-zhijing"])have10kOB = 1;
		}

		if (i1 == 0 || i2 == 0)return 0;
		
		// 计算几率
		result = i1 < i2 ? i1 : i2;

		if (COMBAT_D->enchased_neck_item(me, "七彩神珠"))result += 2;
		
		if (COMBAT_D->enchased_neck_item(me, "圣石") &&
			item1 == "蓝宝石" || item1 == "红宝石")result += 6;

		if (COMBAT_D->enchased_neck_item(me, "水灵") &&
			item1 == "圣龙之眼" || item1 == "圣龙之魂")result += 5;

		if (COMBAT_D->enchased_neck_item(me, "百炼寒精") &&
			item1 == "神龙梦幻" || item1 == "神龙无边")result += 5;
		
		if (COMBAT_D->enchased_neck_item(me, "妖灵之翼") &&
			item1 == "妖灵·魂" || item1 == "妖灵·魄")result += 5;
		
		if (have10kOB)result += 5;
		
		return result;

}

// 发挥项链特殊攻击
int do_neck_attack(object me, object target)
{
		mapping special_attack = ([
			"无双合击"    :  ({"蓝宝石", "红宝石"}),
			"圣龙之怒"    :  ({"圣龙之眼", "圣龙之魂"}),
			"龙咆哮"      :  ({"神龙梦幻", "神龙无边"}),
			"妖灵合击"    :  ({"妖灵·魂", "妖灵·魄"}),
		]);

		string *key_special_attack, *need_item, msg;
		string skill; // 最终选定的技能
		int count, point;

		key_special_attack = keys(special_attack);

		count = sizeof(key_special_attack);

		skill = key_special_attack[random(count)];

		// 该技能出现几率
		need_item = special_attack[skill];
		point = neck_enchase_item(me, need_item[0], need_item[1]);

		if (point > 2 && userp(target))point /= 2;
				
		if (! point)return 0;
		
		if (! objectp(me) || ! objectp(target))return 0;		

		// 满足几率
		if (1 + random(100) <= point)
		{
			if (skill == "无双合击")
			{
				msg = HIY "$N胸前数道光芒闪过，猛然间犹如着魔一般，竟然施展出「无双合击」，连连攻向$n" HIY "。\n" NOR;
				message_combatd(msg, me, target);
				COMBAT_D->do_attack(me, target, 0, 0);
				COMBAT_D->do_attack(me, target, 0, 0);
			}
			if (skill == "圣龙之怒")
			{
				msg = HIC "刹那间，$N胸前项链发出刺眼的蓝光，并不停的颤抖，神技「圣龙之怒」已然发出，顷刻间，一道蓝芒已射向$n" HIC "！\n" NOR;
				message_combatd(msg, me, target);
				if (target->query("neili") > 0)								
					target->add("neili", -1 * target->query("neili") / 10 );
				if (target->query("qi") > 0)								
					target->add("qi", -1 * target->query("qi") / 10 );
				if (target->query("eff_qi") > 0)								
					target->add("eff_qi", -1 * target->query("eff_qi") / 20 );
				
				if (userp(target))target->start_busy(5 + random(5));
				else target->start_busy(8 + random(8));
				
			}
			if (skill == "龙咆哮")
			{
				msg = HIM "突然，$N胸前项链发出震天巨响，犹如千万条巨龙咆哮，「龙咆哮」已奔啸而出，响彻天地！\n" NOR;
				message_combatd(msg, me, target);
				if (userp(target))
					target->receive_wound("qi", me->query("jiali") * 2 + random(me->query("jiali") * 3));

				else
					target->receive_wound("qi", me->query("jiali") * 3 + random(me->query("jiali") * 6));

			}
			if (skill == "妖灵合击")
			{
				msg = HIG "阴风狂起，两道寒光自$N胸前瀑出，涌向四周，绝技「妖灵合击」却已发出，将$n" HIG "笼罩！\n" NOR;
				COMBAT_D->do_attack(me, target, 0, 0);
				COMBAT_D->do_attack(me, target, 0, 0);
				message_combatd(msg, me, target);
				if (userp(target))
					target->receive_wound("qi", me->query_skill("force") * 2 + random(me->query_skill("force") * 3));
				
				else
					target->receive_wound("qi", me->query_skill("force") * 3 + random(me->query_skill("force") * 6));
			}

			// 降低耐久度
			if (objectp(me->query_temp("armor/neck")))
			{
				reduce_consistence(me->query_temp("armor/neck"), me, target, 0);
				reduce_consistence(me->query_temp("armor/neck"), me, target, 0);
				reduce_consistence(me->query_temp("armor/neck"), me, target, 0);
				reduce_consistence(me->query_temp("armor/neck"), me, target, 0);
				reduce_consistence(me->query_temp("armor/neck"), me, target, 0);
				reduce_consistence(me->query_temp("armor/neck"), me, target, 0);
			}
		}
		
		return point;
		
}


// huya
void huya_remove_effect(object me, int ef_point, int en_heishuijing)
{
		me->delete_temp("enchase/special/huya", 1); // 取消标志
		me->add_temp("apply/damage", -1 * (ef_point + ef_point / 2 * en_heishuijing));
		me->add_temp("apply/str", -1 * (ef_point / 20 + ef_point / 40 *  en_heishuijing));
		me->add_temp("apply/force", -1 * (ef_point / 20 + ef_point / 20 * en_heishuijing));
		me->add_temp("apply/blade", -1 * (ef_point / 10 + ef_point / 20 * en_heishuijing));
		me->add_temp("apply/staff", -1 * (ef_point / 10 + ef_point / 20 * en_heishuijing));
		me->add_temp("apply/whip", -1 * (ef_point / 10 + ef_point / 20 * en_heishuijing));
		me->add_temp("apply/club", -1 * (ef_point / 10 + ef_point / 20 * en_heishuijing));	
		
		tell_object(me, HIG "「虎啸」的效果消失了。\n" NOR);
}

// yingyan
void yingyan_remove_effect(object me, int ef_point, int en_heishuijing)
{
		me->delete_temp("enchase/special/yingyan", 1); // 取消标志
		me->add_temp("apply/damage", -1 * (ef_point + ef_point / 2 * en_heishuijing));
		me->add_temp("apply/dex", -1 * (ef_point / 20 + ef_point / 40 *  en_heishuijing));
		me->add_temp("apply/finger", -1 * (ef_point / 10 + ef_point / 20 * en_heishuijing));
		me->add_temp("apply/strike", -1 * (ef_point / 10 + ef_point / 20 * en_heishuijing));
		me->add_temp("apply/unarmed", -1 * (ef_point / 10 + ef_point / 20 * en_heishuijing));
		me->add_temp("apply/cuff", -1 * (ef_point / 10 + ef_point / 20 * en_heishuijing));
		me->add_temp("apply/hand", -1 * (ef_point / 10 + ef_point / 20 * en_heishuijing));
		me->add_temp("apply/claw", -1 * (ef_point / 10 + ef_point / 20 * en_heishuijing));

		tell_object(me, HIG "「鹰之歌」的效果消失了。\n" NOR);
}

// xuejingling
void xuejingling_remove_effect(object me, int ef_point, int en_heishuijing)
{
		me->delete_temp("enchase/special/xuejingling", 1); // 取消标志
		me->add_temp("apply/con",  -1 * (ef_point / 20 + ef_point / 40 *  en_heishuijing));
		me->add_temp("apply/dex",  -1 * (ef_point / 20 + ef_point / 40 *  en_heishuijing));
		me->add_temp("apply/str",  -1 * (ef_point / 20 + ef_point / 40 *  en_heishuijing));
		me->add_temp("apply/dodge",  -1 * (ef_point / 10 + ef_point / 20 * en_heishuijing));
		me->add_temp("apply/parry",  -1 * (ef_point / 10 + ef_point / 20 * en_heishuijing));
		me->add_temp("apply/force",  -1 * (ef_point / 10 + ef_point / 20 * en_heishuijing));

		tell_object(me, HIG "「雪地圣音」的效果消失了。\n" NOR);
}

// xuejingling 分身消除
void fenshen_remove_effect(object me)
{
		me->delete_temp("hiding_kuihua");	    
		tell_object(me, HIG "「雪地圣音」·分身的效果消失了。\n" NOR);
}

// 发挥护腕特殊攻击
int do_wrists_attack(object me, object victim, string en9)
{
	mapping enchase;
	int i_en, i, i_temp, j;
	string item_type, ran_item_enchase, s_temp;
	string ran_enchase_affect;

	string *k_enchase, *k_affect_list;
	mapping affect_list;
	string ran_affect;
	int en_zipili = 0; // 是否镶嵌九孔紫霹雳
	int en_heishuijing = 0; // 是否镶嵌九孔黑水晶
	int i_ef;

	object item;

	int ef_point, ef_ran, n_ef;
	int ran_result;
	string msg;

	mapping list = ([
		"鹰眼"   :  ([
					"yingyan" : ([   "ran"    :  1 + random(2),
                                     "point"  :  500 + random(501)]),
		]),
		"虎牙"   :  ([
					"huya" : (["ran"    :  1 + random(2),
                               "point"  :  500 + random(501)]),
		]),
		"雪精灵"   :  ([
					"xuejingling"    : (["ran"    :  1 + random(2),
                                         "point"  :  500 + random(501)]),
		]),
		"碧水融华"   :  ([
					"bishuironghua"  : (["ran"    :  1 + random(2),
                                         "point"  :  300 + random(201)]),
		]),
	]);

	if (! objectp( item = me->query_temp("armor/wrists")))return 0;

    // 不是自造兵器则返回
	if (! item->is_item_make())return 0;

	// 没有镶嵌则返回
	enchase = item->query("enchase");
	
	if (! mapp(enchase))return 0;
	i_en = sizeof(enchase);
	if (! i_en)return 0;

	// 几率计算 平均?% + 几个累加%
	// 同一时间随机选取一种特效
	k_enchase = keys(enchase);

    // 从已镶嵌的物品中随机选择一个
	ran_item_enchase = enchase[k_enchase[random(i_en)]]["name"];

	// 从选取的物品中随机选择一种特效
	affect_list = list[ran_item_enchase];
	if (! affect_list)return 0;

	k_affect_list = keys(affect_list);

	i_ef = sizeof(k_affect_list);
	if (! i_ef)return 0;

	ran_affect = k_affect_list[random(i_ef)];

	// 特效选取好后则计算特效几率及威力（时间等）
	// 几率计算 平均?% + 几个累加%
	// 威力计算，当前获取的随机镶嵌物品的该特效威力
	
	// 先计算威力
	ef_point = list[ran_item_enchase][ran_affect]["point"];

	// 计算几率
	// 先计算几率总和及该特效个数
	n_ef = 0;
	ef_ran = 0;
	for (i = 0; i < i_en; i ++)
	{
		// 分别依次获得已镶嵌的物品
		s_temp = enchase[k_enchase[i]]["name"];

		// 如果效果定义中无该镶嵌物品则跳过
		if (member_array(s_temp,keys(list)) == -1)continue;

		// 如果该镶嵌物品存在此特效
		if (mapp(list[s_temp][ran_affect]))
		{
			n_ef += 1;
			ef_ran += list[s_temp][ran_affect]["ran"];
		}
	}

	// 得到最后计算的几率
	if (n_ef <= 0)ran_result = 2;  //以防万一
	else
		ran_result = ef_ran / n_ef + n_ef - 1;


	// 是否镶嵌紫霹雳和黑水井
	for (j = 0; j < sizeof(k_enchase); j ++ )
	{
		if (enchase[k_enchase[j]]["name"] == "黑水晶")
		{
			en_heishuijing = 1;
		}
		if (enchase[k_enchase[j]]["name"] == "紫霹雳")
		{
			en_zipili = 1;
		}
	}

    // 外部调用，返回是否镶嵌某种物品
	if (stringp(en9))
	{
		if (en9 == "heishuijing")
			return en_heishuijing;

		if (en9 == "zipili")
			return en_heishuijing;
	}

	// 随机判断是否发出该特效
	if (1 + random(100) > ran_result)return 0;
	
	// 发出特效
	switch(ran_affect)
	{
		case "yingyan":

			if (me->query_temp("enchase/special/yingyan"))return 0;

			message_combatd(HIG + "$N" HIG "的「" + ran_item_enchase + "」发出一道奇异的光芒！\n" NOR, me);

			msg = HIR "乌云散去，一声尖厉的啸叫自天空传来，" + me->name() + HIR "指手向天，数到奇光划过将" + me->name() +
				  HIR "\n包围，然后渐渐侵入其体内，令人匪夷所思。\n" NOR;
			tell_object(me, msg);
			
			me->set_temp("enchase/special/yingyan", 1); // 设置标志，以免重复累加效果
			me->add_temp("apply/damage", ef_point + ef_point / 2 * en_heishuijing);
			me->add_temp("apply/dex", ef_point / 20 + ef_point / 40 *  en_heishuijing);
			me->add_temp("apply/finger", ef_point / 10 + ef_point / 20 * en_heishuijing);
			me->add_temp("apply/strike", ef_point / 10 + ef_point / 20 * en_heishuijing);
			me->add_temp("apply/unarmed", ef_point / 10 + ef_point / 20 * en_heishuijing);
			me->add_temp("apply/cuff", ef_point / 10 + ef_point / 20 * en_heishuijing);
			me->add_temp("apply/hand", ef_point / 10 + ef_point / 20 * en_heishuijing);
			me->add_temp("apply/claw", ef_point / 10 + ef_point / 20 * en_heishuijing);
			me->start_call_out((: call_other, __FILE__, "yingyan_remove_effect",
				                  me, ef_point, en_heishuijing :), 180 + 180 / 2 * en_zipili);
			return 0;

		case "huya":

			if (me->query_temp("enchase/special/huya"))return 0;

			message_combatd(HIG + "$N" HIG "的「" + ran_item_enchase + "」发出一道奇异的光芒！\n" NOR, me);

			msg = HIY "猛然间，一声凄厉的嚎叫由" + item->name() + HIY "震天般传出，摄人心魄，但见" + me->name() + HIY
				  "\n仰天望月，一道金黄色的光芒将其全身笼罩，刹那间威风无比，犹如猛虎。\n" NOR;
			tell_object(me, msg);

			me->set_temp("enchase/special/huya", 1); // 设置标志，以免重复累加效果
			me->add_temp("apply/damage", ef_point + ef_point / 2 * en_heishuijing);
			me->add_temp("apply/str", ef_point / 20 + ef_point / 40 *  en_heishuijing);
			me->add_temp("apply/force", ef_point / 20 + ef_point / 20 * en_heishuijing);
			me->add_temp("apply/blade", ef_point / 10 + ef_point / 20 * en_heishuijing);
			me->add_temp("apply/staff", ef_point / 10 + ef_point / 20 * en_heishuijing);
			me->add_temp("apply/whip", ef_point / 10 + ef_point / 20 * en_heishuijing);
			me->add_temp("apply/club", ef_point / 10 + ef_point / 20 * en_heishuijing);

			me->start_call_out((: call_other, __FILE__, "huya_remove_effect",
				                  me, ef_point, en_heishuijing :), 180 + 180 / 2 * en_zipili);

			return 0;

		case "xuejingling":

			if (me->query_temp("enchase/special/xuejingling"))return 0;

			message_combatd(HIG + "$N" HIG "的「" + ran_item_enchase + "」发出一道奇异的光芒！\n" NOR, me);

			msg = HIW "天色突变，漫天飞雪狂舞，无数雪精自天而下，纷纷落向" + me->name() + HIW +"，然后侵入" + me->name() +
				  HIW "\n四肢，慢慢消失。\n" NOR;
			tell_object(me, msg);

			me->set_temp("enchase/special/xuejingling", 1); // 设置标志，以免重复累加效果
			me->add_temp("apply/con", ef_point / 20 + ef_point / 40 *  en_heishuijing);
			me->add_temp("apply/dex", ef_point / 20 + ef_point / 40 *  en_heishuijing);
			me->add_temp("apply/str", ef_point / 20 + ef_point / 40 *  en_heishuijing);
			me->add_temp("apply/dodge", ef_point / 10 + ef_point / 20 * en_heishuijing);
			me->add_temp("apply/parry", ef_point / 10 + ef_point / 20 * en_heishuijing);
			me->add_temp("apply/force", ef_point / 10 + ef_point / 20 * en_heishuijing);
			
			me->set_temp("hiding_kuihua", 1); // 分身化影

			me->start_call_out((: call_other, __FILE__, "xuejingling_remove_effect",
				                  me, ef_point, en_heishuijing :), 180 + 180 / 2 * en_zipili);

			me->start_call_out((: call_other, __FILE__, "fenshen_remove_effect",
				                  me :), 45 + 22 / 2 * en_zipili);

			return 0;

		case "bishuironghua":

			if (time() - victim->query_temp("enchase/special/bishuironghua") < 180)return 0;			

			message_combatd(HIG + "$N" HIG "的「" + ran_item_enchase + "」发出一道奇异的光芒！\n" NOR, me);

			msg = HIG "一道绿光闪过，" + victim->name() + HIG "似乎中了邪一般，痴痴地望着前方，绿光却毫无忌惮地穿过" + 
				  victim->name() + HIG "身体，令其苦不堪言。\n" NOR;
			tell_object(me, msg);

			victim->set_temp("enchase/special/bishuironghua", time());
			victim->start_busy(2 + random(2));
			victim->affect_by("bishuihan_poison",
                              ([ "level" : 300 + 300 / 2 * en_heishuijing,
                                 "id"    : me->query("id"),
                                 "duration" : 100 + 100 / 2 * en_zipili ]));

			return 0;
	}

	return 0;
		
}

// 可以用来浸透的物品列表：必须是物品的base_name
string *imbue_list = ({
        "/clone/fam/gift/int3",
        "/clone/fam/gift/con3",
        "/clone/fam/gift/str3",
        "/clone/fam/gift/dex3",
        "/clone/fam/item/bless_water",
        "/clone/fam/etc/lv7d",
        "/clone/fam/etc/lv7c",
});

// 浸入的次数的随机界限：如果每次IMBUE以后取0-IMBUE次数的随机
// 数大于这个数值，则IMBUE最终成功。
#define RANDOM_IMBUE_OK         105

// 每次浸入需要圣化的次数
#define SAN_PER_IMBUE           5

// 杀了人以后的奖励
void killer_reward(object me, object victim, object item)
{
        int exp;
        mapping o;
        string my_id;

        if (! me || ! victim)
                return;

        if (! victim->query("can_speak"))
                // only human does effect
                return;

        // record for this weapon
	if (victim->is_not_bad())  item->add("combat/WPK_NOTBAD", 1);
	if (victim->is_not_good()) item->add("combat/WPK_NOTGOOD", 1);
	if (victim->is_good())     item->add("combat/WPK_GOOD", 1);
	if (victim->is_bad())      item->add("combat/WPK_BAD", 1);

        if (userp(victim))
                item->add("combat/PKS", 1);
        else
                item->add("combat/MKS", 1);

        exp = victim->query("combat_exp");
        if (exp < 10000 || me->query("combat_exp") < exp * 4 / 5)
                return;

        exp /= 10000;
        if (exp > 250) exp = 100 + (exp - 250) / 16; else
        if (exp > 50) exp = 50 + (exp - 50) / 4;
        my_id = me->query("id");
        o = item->query("owner");
        if (! o) o = ([ ]);
        if (! undefinedp(o[my_id]) || sizeof(o) < 12)
                o[my_id] += exp;
        else
        {
                // Too much owner, I must remove one owner
                int i;
                int lowest;
                string *ks;

                lowest = 0;
                ks = keys(o);
                for (i = 1; i < sizeof(ks); i++)
                        if (o[ks[lowest]] > o[ks[i]])
                                lowest = i;
                map_delete(o, ks[lowest]);
                o += ([ my_id : exp ]);
        }
        item->set("owner", o);
}

// 召唤物品
int receive_summon(object me, object item)
{
        object env;
        object temp;
        int type;
        
/* 在/adm/daemons/biwud.c 做设置
	// 特殊：针对挑战系统，非我的兵器不能召唤 
	if (! playerp(me) && item->item_owner() != me->query("id"))return 1;
*/	
        if ((env = environment(item)) && env == me)
        {
                tell_object(me, item->name() + "不就在你身上"
                            "嘛？你召唤个什么劲？\n");
                return 1;
        }

        if (me->query("jingli") < 200)
        {
                tell_object(me, "你试图呼唤" + item->name() +
                            "，可是难以进入境界，看来是精力不济。\n");
                return 0;
        }
        me->add("jingli", -200);

        if (item->query("id") == "qingtian")
        {
              message_sort(HIM "\n只见四周金光散布，祥云朵朵，远处有凤凰盘绕，麒麟逐戏。耳边"
                           "传来阵阵梵音。$N" HIM "一声长啸，" + item->query("name") + HIM
                           "破空而来 ……。\n\n" NOR, me); 
        }
        else

              message_vision(HIW "$N" HIW "突然大喝一声，伸出右手凌空"
                             "一抓，忽然乌云密布，雷声隐隐。\n\n" NOR, me);

        if (env == environment(me))
        {
                message_vision(HIW "只见地上的" + item->name() +
                               HIW "应声而起，飞跃至$N" HIW
                               "的掌中！\n\n" NOR, me);
        } else
        {
                type = random(3);
                if (env)
                {
                        if (env->is_character() && environment(env))
                                        env = environment(env);

                        switch (type)
                        {
                        case 0:
                                message("vision", HIW "天空中传来隐隐的雷声"
                                        "，忽然电闪雷鸣，" + item->name() +
					HIW "腾空而起，"
                                        "消失不见！\n\n" NOR, env);
                                break;
                        case 1:
                                message("vision", HIC "一道神光从天而降"
                                        "，罩定了" + item->name() + HIC "，只见" +
					item->name() + HIC "化作长虹破空而"
					"走。\n\n" NOR, env);
                                break;
                        default:
                                message("vision", HIY "忽然间麝香遍地，氤氲弥漫，" +
                                        item->name() + HIY "叮呤呤的抖动数下，化作一"
					"道金光转瞬不见！\n\n" NOR, env);
                                break;
                        }

                        if (interactive(env = environment(item)))
                        {
                                tell_object(env, HIM + item->name() +
                                                 HIM "忽然离你而去了！\n" NOR);
                        }
                }

                switch (type)
                {
                case 0:
                        message_vision(HIW "一声" HIR "霹雳" HIW "，"
                                       "闪电划破长空，" + item->name() + HIW
                                       "从天而降，飞入$N" HIW "的手中！\n\n" NOR, me);
                        break;
                case 1:
                        if (item->query("id") == "qingtian")break;

                        message_vision(HIW "一道" HIY "长虹" HIW "扫过"
                                       "天空，只见" + item->name() + HIW
					"落入了$N" HIW "的掌中！\n\n" NOR, me);
                        break;
                default:
                        message_vision(HIW "只见" + item->name() + HIW "呤呤作响，大"
                                       "放异彩，挟云带雾，突现在$N"
                                       HIW "的掌中！\n\n" NOR, me);
                        break;
                }
        }

        // 取消no_get属性
        item->delete_temp("stab_by");
        item->delete("no_get");

        item->move(me, 1);
        if (environment(item) != me)
                return 1;

        if (item->query("armor_type"))
        {
                // is armor
                temp = me->query_temp("armor/" + item->query("armor_type"));
                if (temp) temp->unequip();
                WEAR_CMD->do_wear(me, item);
        } else
        if (item->query("skill_type"))
        {
                if (temp = me->query_temp("weapon"))
                        temp->unequip();
                if (temp = me->query_temp("secondary_weapon"))
                        temp->unequip();
                WIELD_CMD->do_wield(me, item);
        }

        return 1;
}

// 隐藏物品
int hide_anywhere(object me, object item)
{
        if (item->item_owner() != me->query("id"))
                return 0;

        if (me->query("jingli") < 100)
        {
                tell_object(me, "你试图令" + item->name() +
                            "遁去，可是精力不济，难以发挥它的能力。\n");
                return 0;
        }
        me->add("jingli", -100);

        message_vision(HIM "$N" HIM "轻轻一旋" + item->name() +
                       HIM "，已然了无踪迹。\n", me);
        destruct(item);
        return 1;
}

// 追寻物品
int receive_miss(object me, object item)
{
        object env, me_env;
	string bn, bn2;

        env = environment(item);

        if (env == environment(me))
        {
                write("你瞪着" + item->name() + "，看啥？\n");
                return 0;
        }

        if (env == me)
        {
                write("你摸着" + item->name() + "，发了半天的呆。\n");
                return 0;
        }

        if (! objectp(env) || userp(env) || environment(env))
        {
                write("你试图感应" + item->name() + "，但是感觉非常的渺茫。\n");
                return 0;
        }

        if (! wizardp(me) && ! env->query("outdoors"))
        {
                write("冥冥中你感应到" + item->name() + "，但是似乎难以到达那里。\n");
                return 0;
        }

        message("vision", me->name() + "在凝神思索，不知道要做些什么。\n",
                environment(me), ({ me }));
        if (me->query("jingli") < 400)
        {
                write("你觉得" + item->name() + "的感觉相当"
                      "飘忽，看来精力不济，难以感应。\n");
                return 0;
        }

	bn = base_name(env);
	me_env = environment(me);
	if (sscanf(bn, "/kungfu/class/sky%s", bn2) == 1)
	{
		write("你感觉 " + item->name() + NOR " 似乎不存在于人界！\n" NOR);
		return 0;
	}
	if (sscanf(bn, "/d/newbie/%s", bn2) == 1)	
	{
		log_file("horse", me->query("id") + "于" + ctime(time()) + " 试图利用兵器回到新手村。\n");
		return notify_fail("你感觉 " + item->name() + NOR " 似乎不能感应到你的呼唤！\n" NOR);
	}
	if (bn->query("no_flyto"))
	{
		write("你无法追寻到你兵器的所在地。\n");
		return 0;
	}
	
	// 蓬莱岛上不能MISS
	if (sscanf(base_name(me_env), "/d/penglai/%s", bn2) == 1)
	{
		write("无法感应到你的兵器所在！\n" NOR);
		return 0;
	}
		
	// 神龙岛上不能MISS
	if (sscanf(bn, "/d/shenlong/%s", bn2) == 1)
	{
		write("无法感应到你的兵器所在！\n" NOR);
		return 0;
	}

	if (sscanf(base_name(me_env), "/d/shenlong/%s", bn2) == 1)
	{
		write("此处无法感应到你的兵器。\n");
		return 0;
	}

        // 消耗精力
        me->add("jingli", -300 - random(100));
        message_vision(HIM "$N" HIM "口中念念有词，转瞬天际一道长虹划"
                       "过，$N" HIM "驾彩虹而走。\n" NOR, me);
        tell_object(me, "你追寻" + item->name() + "而去。\n");
        me->move(environment(item));
        message("vision", HIM "一道彩虹划过天际，" + me->name() +
                HIM "飘然落下，有若神仙。\n" NOR, environment(me), ({ me }));
        tell_object(me, HIM "你追寻到了" + item->name() +
                    HIM "，落下遁光。\n" NOR);
        return 1;
}

// 插在地上
int do_stab(object me, object item)
{
        if (! item->is_weapon() && item->query("armor_type") != "hands")
                return notify_fail(item->name() + "也能插在地上？\n");

        if (! environment(me)->query("outdoors") &&
            ! wizardp(me))
                return notify_fail("在这里乱弄什么！\n");

        item->set("no_get", bind((: call_other, __FILE__, "do_get_item", item :), item));
        item->set_temp("stab_by", me->query("id"));

        message_vision(WHT "\n$N" WHT "随手将" + item->name() + NOR +
                       WHT "往地上一插，发出「嚓愣」一声脆响。\n\n" NOR, me);
        item->move(environment(me));
        return 1;
}

// 把取物品时检查
mixed do_get_item(object item)
{
        object me;

        if (! objectp(me = this_player()))
                return 1;

        if (me->query("id") != item->query_temp("stab_by") &&
            me->query("id") != item->item_owner())
                return "你试图将" + item->name() + "拔起，却"
                       "发现它仿佛是生长在这里一般，无法撼动。\n";

        message_vision(HIW "\n$N" HIW "随手拂过" + item->name() +
                       HIW "脊处，顿时只听「嗤」的一声，扬起一阵"
                       "尘土。\n\n" NOR, me);
        item->delete_temp("stab_by");
        item->delete("no_get");
        item->move(me, 1);
        return "";
}

// 发挥特殊功能
mixed do_touch(object me, object item)
{
        string msg;
        object ob;
        object *obs;
        mapping my;
        int ran_n; 

        if (me->query("id") != item->item_owner())
        {
                message_vision(HIR "\n$N轻轻触碰" + item->name() +
                               HIR "，突然间全身一震，连退数步，如"
                               "遭受电击。\n" NOR, me);
                me->receive_damage("qi", 50 + random(50));
                return 1;
        }

		

        if (me->query("jingli") < 160)
        {
                me->set("jingli", 1);
                return notify_fail(CYN "\n你凝视" + item->name() +
                                   CYN "许久，悠悠一声长叹。\n" NOR);
        }
		else
                me->add("jingli", -150);

        switch (random(3))
        {
        case 0:
                msg = CYN "\n$N" CYN "轻轻一弹$n" CYN "，长吟"
                      "道：「别来无恙乎？」\n" NOR;
                break;
        case 1:
                msg = CYN "\n$N" CYN "轻轻抚过$n" CYN "，作古"
                      "风一首，$n" CYN "铃铃作响，似以和之。\n" NOR;
                break;
        default:
                msg = CYN "\n$N" CYN "悠然一声长叹，轻抚$n"
                      CYN "，沉思良久，不禁感慨万千。\n" NOR;
                break;
        }

        switch (random(3))
        {
        case 0:
                msg += HIM "忽然只见$n" HIM "闪过一道光华，"
                       "飞跃而起，散作千百流离。\n" NOR;
                break;
        case 1:
                msg += HIM "顿听$n" HIM "一声龙吟，悠悠不绝"
                       "，直沁入到你的心肺中去。\n" NOR;
                break;
        default:
                msg += HIM "霎时间$n" HIM "光芒四射，如蕴琉"
                       "璃异彩，逼得你难以目视。\n" NOR;
                break;
        }

        msg = replace_string(msg, "$n", item->name());
        message_vision(msg, me);

	ran_n = 1000;
	if (me->is_fighting())ran_n = 3000;
	
        if (random(ran_n) == 1 || wizardp(me))
        {
                obs = filter_array(all_inventory(environment(me)), (: userp :));
                foreach (ob in obs)
                {
                        my = ob->query_entire_dbase();
                        my["jing"]   = my["eff_jing"] = my["max_jing"];
                        my["qi"]     = my["eff_qi"]   = my["max_qi"];
                        my["neili"]  = my["max_neili"];
                        my["jingli"] = my["max_jingli"];
                        ob->set_temp("nopoison", 1);
                }
                tell_object(obs, HIC "你感到一股温和的热浪袭来，便似"
                                "获得重生一般。\n" NOR);
        } else
        if (me->query("neili") < me->query("max_neili") || item->query("enchase_all") >= 9 )
        {
			    // 九孔兵器特效
			    if (item->query("enchase_all") >= 9 
					&& me->query("qi") > 500 
					&& time() - me->query_temp("last_touch_jl") > 180)
			    {
						me->set("eff_jingli", me->query("max_jingli"));
						me->set("jingli", me->query("max_jingli"));

						me->receive_damage("qi", 400);
						me->set_temp("last_touch_jl", time());
			    }

                me->set("neili", me->query("max_neili"));
                tell_object(me, HIC "你只觉一股热气至丹田冉冉升起，"
                                "说不出的舒服。\n" NOR);
        }
        return 1;
}

// 圣化物品
int do_san(object me, object item)
{
        string my_id;
        int count;
        int san;

        if (! item->query("skill_type") && item->query("armor_type") != "hands")
        {
                // 是装备类？
                return notify_fail("装备现在还无法圣化。\n");
        }

        // 武器类的圣化
        if (item->query("magic/power") > 0)
                return notify_fail("现在" + item->name() + "的威力"
                                   "已经得到了充分的发挥了。\n");

        if (item->query("magic/imbue_ok"))
                return notify_fail("现在" + item->name() + "的潜力"
                                   "已经充分挖掘了，现在只是需要最"
                                   "后一步融合。\n");

        my_id = me->query("id");

        count = sizeof(item->query("magic/do_san"));
        if (item->query("magic/imbue_ob"))
                return notify_fail("现在" + item->name() + "已经被充分的圣"
                                   "化了，需要浸入神物以进一步磨练。\n");

        if (item->query("magic/do_san/" + my_id))
                return notify_fail("你已经为" + item->name() + "圣化过了，"
                                   "非凡的能力还无法被它完全吸收。\n你"
                                   "有必要寻求他人帮助以继续圣化。\n");

        if (item->item_owner() == my_id)
        {
                if (! count)
                        return notify_fail("你应该先寻求四位高手协助你先行圣化" +
                                           item->name() + "。\n");

                if (count < SAN_PER_IMBUE - 1)
                        return notify_fail("你应该再寻求" +
                                           chinese_number(SAN_PER_IMBUE - 1 - count) +
                                           "位高手先行圣化" + item->name() + "。\n");
        } else
        {
                if (count >= SAN_PER_IMBUE - 1)
                        return notify_fail("最后需要剑的主人为它圣化，不劳你费心了。\n");
        }

        if (me->query("neili") < me->query("max_neili") * 9 / 10)
                return notify_fail("你现在内力并不充沛，怎敢贸然运用？\n");

        if (me->query("jingli") < me->query("max_jingli") * 9 / 10)
                return notify_fail("你现在精力不济，怎敢贸然运用？\n");

        if (me->query_skill("force") < 300)
                return notify_fail("你的内功根基不够扎实，不能贸然圣化。\n");

        if (me->query("max_neili") < 8000)
                return notify_fail("你尝试运了一下内力，无法顺"
                                   "利运足一个周天，难以施展你的能力。\n");

        if (me->query("max_jingli") < 1000)
                return notify_fail("你试图凝神运用精力，但是感觉尚有欠缺。\n");

        message_sort(HIM "\n$N" HIM "轻轻抚过$n" HIM "，两指点于其上，同"
                     "时运转丹田内力，经由奇经八脉源源由体内流出，注"
                     "入$n" HIM "。忽的只见氤氲紫气从$n" HIM
                     "上腾然升起，弥漫在四周。\n" NOR, me, item);

        if (me->query("max_neili") < me->query_neili_limit() - 400)
        {
                if (random(2) == 1)
                {
                        // 内力未满警告
                        message_vision(HIR "$N" HIR "脸色忽然变了变。\n" NOR,
                                       me);
                        tell_object(me, HIC "你忽然觉得丹田气息有些错乱。\n" NOR);
                } else
                {
                        message_vision(HIR "$N" HIR "忽然闷哼一声，脸"
                                       "上刹时大汗淋漓！\n" NOR, me);
                        tell_object(me, HIC "你感到可能是你的内力尚未锻炼"
                                    "到极至，结果损伤了你的内功根基。\n" NOR);
                        tell_object(me, HIC "你的基本内功下降了。\n");
                        me->set_skill("force", me->query_skill("force", 1) - 10 - random(5));
                        return 1;
                }
        }

        // 统计IMBUE过的次数，并用来计算本次MAX_NEILI/JINGLI的消耗
        san = item->query("magic/imbue");

        me->add("max_neili", -(san + 5));
        me->add("neili", -(san * 10 + 100));
        me->add("max_jingli", -(san * 5 + 50));
        me->add("jingli", -(san * 5 + 50));
        item->set("magic/do_san/" + my_id, me->name(1));
        me->start_busy(1);

        if (item->item_owner() == my_id)
        {
                tell_object(me, HIW "你凝神片刻，觉得" + item->name() +
                            HIW "似乎有了灵性，跳跃不休，不禁微微一笑。\n" NOR);
                message("vision", HIW + me->name() + HIW "忽然"
                        "微微一笑。\n" HIW, environment(me), ({ me }));

                // 选定一个需要imbue的物品
                item->set("magic/imbue_ob", imbue_list[random(sizeof(imbue_list))]);
        }
        return 1;
}

// 浸透物品
int do_imbue(object me, object item, object imbue)
{
        if (item->query("magic/power") > 0)
                return notify_fail("现在" + item->name() + "的威力"
                                   "已经得到了充分的发挥了。\n");

        if (item->query("magic/imbue_ok"))
                return notify_fail("现在" + item->name() + "的潜力"
                                   "已经充分挖掘了，现在只是需要最"
                                   "后一步融合。\n");

        if (sizeof(item->query("magic/do_san")) < SAN_PER_IMBUE)
                return notify_fail("你必须先对" + item->name() +
                                   "进行充分的圣化才行。\n");

        if (base_name(imbue) != item->query("magic/imbue_ob"))
                return notify_fail(item->name() + "现在不需要用" +
                                   imbue->name() + "来浸入。\n");

        message_sort(HIM "$N" HIM "深吸一口气，面上笼罩了一层白霜，只手握住$n" +
                     imbue->name() +
                     HIM "，忽然间融化在掌心，晶莹欲透！$N"
                     HIM "随手一挥，将一汪清液洒在$n" HIM
                     "上，登时化做雾气，须臾成五彩，奇光闪烁。\n" NOR,
                     me, item);

        tell_object(me, "你将" + imbue->name() + "的效力浸入了" +
                    item->name() + "。\n");
        item->delete("magic/do_san");
        item->delete("magic/imbue_ob");
        imbue->add_amount(-1);
        if (imbue->query_amount() < 1)
                destruct(imbue);

        me->start_busy(1);

        item->add("magic/imbue", 1);
        if (random(item->query("magic/imbue")) >= RANDOM_IMBUE_OK)
        {
                // 浸透完成
                tell_object(me, HIG "你忽然发现手中的" + item->name() +
                            HIG "有一种跃跃欲试的感觉，似乎期待着什么。\n" NOR);
                item->set("magic/imbue_ok", 1);
        }

        return 1;
}

// 镶嵌物品
int do_enchase(object me, object item, object tessera)
{
        if (item->query("magic/power") > 0)
                return notify_fail("现在" + item->name() + "的威力"
                                   "已经得到了充分的发挥了。\n");

        if (! item->query("magic/imbue_ok"))
                return notify_fail("现在" + item->name() + "的潜力"
                                   "还没有充分的激发，未到镶嵌的时候。\n");

        if (! tessera->query("can_be_enchased"))
                return notify_fail(tessera->name() + "好象没"
                                   "法用来镶嵌吧。\n");

        if (! mapp(tessera->query("magic")))
                return notify_fail(tessera->name() + "不能发挥魔力，"
                                   "没有必要镶嵌在" + item->name() + "上面。\n");

        if (me->query_skill("certosina", 1) < 100)
                return notify_fail("你觉得你的镶嵌技艺还不够"
                                   "娴熟，不敢贸然动手。\n");

        message_sort(HIM "“喀啦”一声，$N" HIM "将" + tessera->name() +
                     HIM "镶嵌到了$n" HIM "上面，只见$n" HIM
                     "上面隐隐的显过了一道奇异的光芒，随"
                     "即变得平静，说不出的平凡。\n" NOR, me, item);
        tell_object(me, HIC "你感受" + item->name() + HIC "发生了"
                    "不可言喻的变化。\n" NOR);
        item->set("magic/power", tessera->query("magic/power"));
       	item->set("magic/type", tessera->query("magic/type"));
        item->set("magic/tessera", tessera->name());
        item->save();
        destruct(tessera);

        // 发布消息
        CHANNEL_D->do_channel(this_object(), "rumor",
                              "听说神品" + item->name() + HIM +
                              "来到了人间。");

        me->start_busy(1);
        return 1;
}

string get_random_key(string key_id)
{
	key_id += sprintf("%d", random(20));
	return key_id;
}
// 扩展镶嵌
int do_enchase2(object me, object item, object tessera)
{
		string key_id;
		mapping enchases;
		string *k_enchases;
		int i;
	
        if (! tessera->query("enchase/point"))
                return notify_fail(tessera->name() + "好象没"
                                   "法用来镶嵌吧。\n");

        if (me->query_skill("certosina", 1) < 100)
                return notify_fail("你觉得你的镶嵌技艺还不够"
                                   "娴熟，不敢贸然动手。\n");

        if (item->query("enchase_all") - sizeof(item->query("enchase")) <= 0)
		return notify_fail("你的 " + item->name() + NOR " 上已没有足够的槽位。\n" );

		// 九孔镶嵌物品只能镶嵌到第九孔上，且每把兵器只能镶嵌一个
		if (tessera->query("nine_object"))
		{
			enchases = item->query("enchase");
			if (mapp(enchases))
			{
				k_enchases = keys(enchases);
			}

			if (item->query("enchase_all") < 9)
			{
				return notify_fail("这块宝石只能镶嵌到第九孔上。\n");
			}

			for (i = 0; i < sizeof(k_enchases); i ++ )
			{
				if (enchases[k_enchases[i]]["name"] == "海蓝石" ||
					enchases[k_enchases[i]]["name"] == "红眼恶魔")
				{
					return notify_fail("不能在一把兵器上同时镶嵌两个九孔宝石。\n");
				}
				if (enchases[k_enchases[i]]["name"] == "古松残镢" ||
					enchases[k_enchases[i]]["name"] == "龙凰紫珠")
				{
					return notify_fail("不能在一把兵器上同时镶嵌两个九孔宝石。\n");
				}
				if (enchases[k_enchases[i]]["name"] == "流星赶月" ||
					enchases[k_enchases[i]]["name"] == "九天玲珑珠")
				{
					return notify_fail("不能在一把兵器上同时镶嵌两个九孔宝石。\n");
				}	
				if (enchases[k_enchases[i]]["name"] == "黑水晶" ||
					enchases[k_enchases[i]]["name"] == "紫霹雳")
				{
					return notify_fail("不能在一把兵器上同时镶嵌两个九孔宝石。\n");
				}							
			}
		}

	log_file("beset", me->query("id") + " beset " + tessera->name() + NOR " to " + base_name(item) + "(" + 
	                  item->name() + NOR + ") at " + ctime(time()) + "\n");
        message_sort(HIG "“喀啦”一声，$N" HIG "将" + tessera->name() +
                     HIG "镶嵌到了$n" HIG "上面，只见$n" HIG
                     "上面隐隐的显过了一道奇异的光芒，随"
                     "即变得平静，说不出的平凡。\n" NOR, me, item);
        tell_object(me, HIC "你感受" + item->name() + HIC "发生了"
                    "不可言喻的变化。\n" NOR);

        key_id = tessera->query("id");
	if (item->query("enchase/" + key_id))
	{
		while (1)
		{			
			key_id = get_random_key(tessera->query("id"));
			if (! item->query("enchase/" + key_id))break;
		}
	}

        item->set("enchase/" + key_id + "/name", tessera->query("enchase/name"));
       	item->set("enchase/" + key_id + "/point", tessera->query("enchase/point"));
       	item->set("enchase/" + key_id + "/fy", tessera->query("enchase/fy"));
       	item->set("enchase/" + key_id + "/qy", tessera->query("enchase/qy"));
       	item->set("enchase/" + key_id + "/type", tessera->query("enchase/type"));
       	item->set("enchase/" + key_id + "/cur_firm", tessera->query("enchase/cur_firm"));
       	item->set("enchase/" + key_id + "/spe_data", tessera->query("enchase/spe_data"));

	// 武器镶嵌提高伤害50点
	if (item->query("skill_type") || item->query("armor_type") == "hands")
	{
		if (tessera->query("enchase/spe_data/luhua-zhijing"))
			item->set("enchase/" + key_id + "/point", 2500); //itemmake.c point * 2
	}
	
	// 护身符
	if (item->query("armor_type") == "myheart" || item->query("armor_type") == "myheart2" )
	{
		if (tessera->query("enchase/spe_data/luhua-zhijing"))
		{
			item->set("enchase/" + key_id + "/fy", 3);
       			item->set("enchase/" + key_id + "/qy", 3);
		}
	}
	// 护腕镶嵌提高伤害40点 itemmake.c
	if (item->query("armor_type") == "wrists")
	{
		if (tessera->query("enchase/spe_data/luhua-zhijing"))
			item->set("enchase/" + key_id + "/point", 1);
	}
	// 披风镶嵌提高伤害40点 itemmake.c
	if (item->query("armor_type") == "surcoat")
	{
		if (tessera->query("enchase/spe_data/luhua-zhijing"))
			item->set("enchase/" + key_id + "/point", 1);
	}
	// 转世勋章镶嵌提高point 21点
	if (item->query("armor_type") == "medal2")
	{
		if (tessera->query("enchase/spe_data/luhua-zhijing"))
			item->set("enchase/" + key_id + "/point", 21);
	}
	// 防具镶嵌提高防护+50
	if (item->query("armor_type") == "armor")
	{
		if (tessera->query("enchase/spe_data/luhua-zhijing"))
			item->set("enchase/" + key_id + "/point", 2500); // 2500*2.4
	}
	
        item->save();
	tell_object(me, HIM + item->name() + HIM " 突然离你而去 ……\n" NOR);
	destruct(item);
        destruct(tessera);

        me->start_busy(1);

        return 1;
}

// 10级兵器攻击对手
mixed weapon10lv_hit_ob(object me, object victim, object weapon, int damage_bonus)
{
        mapping magic;
        int jingjia;
        int power, resistance;
        int damage;
        string msg;

		int point;

        // 计算魔法效果
        magic = weapon->query("magic");
        power = magic["power"];
        damage = 0;
        resistance = 0;
        //jingjia = me->query("jiajing");
        jingjia = me->query("jiali") / 3;
      
        switch (magic["type"])
        {
        case "lighting":
                // 闪电攻击：伤害内力++和气+
                resistance = victim->query_temp("apply/resistance/lighting");
                damage = (power + jingjia) * 200 / (100 + resistance);
                switch (random(3))
                {
                case 0:
                        msg = HIY + weapon->name() + HIY "迸发出几道明亮的闪光，兹兹作响，让$n"
                              HIY "不由为之酥麻。\n" NOR;
                        break;
                case 1:
                        msg = HIY "一道电光闪过，" + weapon->name() + HIY
                              "变得耀眼夺目，令$n" HIY "无法正视，心神俱废。\n" NOR;
                        break;
                default:
                        msg = HIY "天际隐隐响起几声闷雷，紧接着一道霹雳直下，" +
                              weapon->name() + HIY "忽明忽暗，五彩缤纷，震得$n"
                              HIY "酸软无力。\n" NOR;
                        break;
                }

                if (victim->query("neili") > damage)
                        victim->add("neili", -damage);
                else
                        victim->set("neili", 0);

                victim->receive_damage("qi", damage);
                victim->receive_wound("qi", damage / 4);
                break;

        case "cold":
                // 冷冻攻击：伤害精++和气+
                resistance = victim->query_temp("apply/resistance/cold");
                damage = (power + jingjia) * 200 / (100 + resistance);
                victim->receive_damage("jing", damage / 3);
                victim->receive_wound("jing", damage / 6);
                victim->receive_damage("qi", damage / 2);
                victim->receive_wound("qi", damage / 5);
                switch (random(3))
                {
                case 0:
                        msg = HIB + weapon->name() + HIB "闪过一道冷涩的蓝光，让$n"
                              HIB "不寒而栗。\n" NOR;
                        break;
                case 1:
                        msg = HIB "忽然间" + weapon->name() + HIB
                              "变得透体通蓝，一道道冰冷的寒光迸发出来，$n"
                              HIB "浑身只是一冷。\n" NOR;
                        break;
                default:
                        msg = HIB "一道光圈由" + weapon->name() + HIB "射出，"
                              "森然盘旋在$n" HIB "四周，悄然无息。\n" NOR;
                        break;
                }
                break;

        case "fire":
                // 火焰攻击：伤害精+和气++
                resistance = victim->query_temp("apply/resistance/fire");
                damage = (power + jingjia) * 300 / (100 + resistance);
                victim->receive_damage("jing", damage / 5);
                victim->receive_wound("jing", damage / 8);
                victim->receive_damage("qi", damage);
                victim->receive_wound("qi", damage / 2);
                switch (random(3))
                {
                case 0:
                        msg = HIR + weapon->name() + HIR "蓦的腾起一串火焰，将$n"
                              HIR "接连逼退了数步，惨叫连连。\n" NOR;
                        break;
                case 1:
                        msg = HIR "一道火光从" + weapon->name() + HIR
                              "上迸出，迅捷无伦的击中$n" HIR "，令人避无可避！\n" NOR;
                        break;
                default:
                        msg = HIR "一串串火焰从" + weapon->name() + HIR "上飞溅射出，"
                              "四下散开，接连击中$n" HIR "！\n" NOR;
                        break;
                }
                break;

        case "magic":
                // 魔法攻击：吸取气+
                resistance = victim->query_temp("apply/resistance/magic");
                damage = (power + jingjia) * 200 / (100 + resistance);
                victim->receive_damage("qi", damage);
                victim->receive_wound("qi", damage / 4);
                switch (random(3))
                {
                case 0:
                        msg = HIM + weapon->name() + HIM "响起一阵奇异的声音，犹如龙吟，令$n"
                              HIM "心神不定，神情恍惚。\n" NOR;
                        break;
                case 1:
                        msg = HIM "“啵”的一声，" + weapon->name() + HIM
                              "如击败革，却见$n" HIM "闷哼一声，摇晃不定！\n" NOR;
                        break;
                default:
                        msg = HIM + weapon->name() + HIM "上旋出一道道五彩缤纷的"
                              "光圈，笼罩了$n" HIM "，四下飞舞。\n" NOR;
                        break;
                }
                break;

        default:
                msg = "";
                break;
        }

        // 使用perform
        if (random(2)) return msg;
        damage = 200 + random(400);


		if (do_wrists_attack(me, victim, "zipili"))		
			point = 6;		
		else
			point = 8;
		
        switch (random(point))
        {
        case 0:
                victim->receive_wound("jing", damage / 6 + random(damage / 6), me);
                return msg + HIM "$N" HIM "嘿然冷笑，抖动" + weapon->name() +
                       HIM "，数道光华一起射出，将$n" HIM "困在当中，无法自拔。\n" NOR;

        case 1:
                victim->receive_wound("jing", damage / 5 + random(damage / 5), me);
                return msg + HIC "$N" HIC "手中的" + weapon->name() + HIC "射出各种光芒，"
                       "眩目夺人，一道道神采映射得天地尽情失色，让$n"
                       HIC "目瞪口呆！\n" NOR;
        case 2:
                victim->receive_wound("qi", damage / 4 + random(damage / 4), me);
                return msg + HIY "$N" HIY "举起" + weapon->name() +
                       HIY "，只见天空一道亮光闪过，$n" HIY "连吐几口鲜血！\n" NOR;

        case 3:
                victim->receive_wound("qi", damage / 3 + random(damage / 3), me);
                return msg + HIG "$N" HIG "随手划动" + weapon->name() + HIG "，一圈圈碧芒"
                       "围向$n" HIG "，震得$n吐血连连！\n" NOR;

        case 4:
                victim->receive_wound("qi", damage / 2 + random(damage / 2), me);
                return msg + HIW "$N" HIW "一声长叹，" + weapon->name() + HIW "轻轻递出，"
                       "霎时万籁俱静，$n" HIW "只觉得整个人都跌进了地狱中去！\n" NOR;

        default:
                if (me->query_temp("weapon_performing"))
                        break;
                me->start_call_out(bind((: call_other, __FILE__, "continue_attack",
                                           me, victim, weapon, 10 + random(4) :), me), 0);
        }
}

// 9级兵器攻击对手
mixed weapon_hit_ob(object me, object victim, object weapon, int damage_bonus)
{
        int ap;
        int dp;
        int damage;
		int point;

        if (random(2)) return;

        // 计算damage：不论是空手武器还是普通兵器，统一计算
        if (weapon->is_weapon())
                damage = me->query_temp("apply/damage");
        else
                damage = me->query_temp("apply/unarmed_damage");

		if (do_wrists_attack(me, victim, "zipili"))		
			point = 5;		
		else
			point = 8;

        switch (random(point))
        {
        case 0:
                victim->receive_wound("jing", damage / 5 + random(damage / 5), me);
                return HIY "$N" HIY "抖动手中的" + weapon->name() + HIY
                       "，幻化成夜空流星，数道" HIM "紫芒" HIY "划破星"
                       "空袭向$n" HIY "。\n" NOR;
        case 1:
                victim->receive_wound("qi", damage / 3 + random(damage / 3), me);
                return HIR "$N" HIR "大喝一声，手中" + weapon->name() +
                       HIR "遥指$n" HIR "，一道杀气登时将$n" HIR "震退"
                       "数步。\n" NOR;
        case 2:
                victim->receive_wound("qi", damage / 6 + random(damage / 6), me);
                victim->receive_wound("jing", damage / 10 + random(damage / 10), me);
                return HIG "$N" HIG "蓦地回转" + weapon->name() + HIG
                       "，漾起层层碧波，宛若" NOR + HIB "星河" HIG "气"
                       "旋，将$n" HIG "圈裹其中。\n" NOR;
        case 3:
				if (do_wrists_attack(me, victim, "zipili"))
				{
					 if (me->query_temp("weapon_performing"))
			                break;
					 me->start_call_out(bind((: call_other, __FILE__, "continue_attack",
                                           me, victim, weapon, 5 + random(4) :), me), 0);					
				}
				break;
        case 4:
        case 5:
        case 6:
                break;

        default:
                if (me->query_temp("weapon_performing"))
                        break;
                me->start_call_out(bind((: call_other, __FILE__, "continue_attack",
                                           me, victim, weapon, 5 + random(4) :), me), 0);
        }
}

// 绝招：12连环攻击
void continue_attack(object me, object victim, object weapon, int times)
{
        int i;
        string msg;
        int ap, dp;

        if (! me || ! victim || ! weapon || ! me->is_fighting(victim))
                return;

        msg  = HIW "霎时只听$N" HIW "纵声长啸，人与" + weapon->name() +
               HIW "融为一体，霎时间寒芒飞散，向$n" HIW "射去。\n" NOR;

        ap = me->query_skill("martial-cognize");
        dp = victim->query_skill("parry");

        if (ap / 2 + random(ap) > dp * 2 / 3)
                msg += HIR "$n" HIR "大骇之下连忙后退，可已然不及闪避，慌乱"
                       "中不禁破绽迭出。\n" HIW "$N" HIW "盯住$n" HIW "招中"
                       "破绽，疾速旋转手中" + weapon->name() + HIW "，电光火"
                       "石间已朝$n" HIW "攻出" + chinese_number(times) + HIW
                       "招！\n" NOR;
        else
        {
                msg += CYN "可是$n" CYN "冥神抵挡，将$N"
                       CYN "此招的所有变化全然封住。\n" NOR;
                message_combatd(msg, me, victim);
                return;
        }
        message_combatd(msg, me, victim);

        me->set_temp("weapon_performing", 1);
        i = times;
        while (i--)
        {
                if (! me->is_fighting(victim))
                        break;

                if (! victim->is_busy() && random(2))
                        victim->start_busy(1);

                COMBAT_D->do_attack(me, victim, me->query_temp("weapon"), 0);
        }
        me->delete_temp("weapon_performing");
}

// 计算穿透特效点
public int chuantou_point(object me, object item)
{	
		int total;
		mapping enchase;
		string *k_enchase, *k_chuantou_item;
		int i;
		string name;

		mapping chuantou_item = ([
			"炎黄之星"       : 100,
			"炎黄·战神"      : 100,
			"炎黄·君临天下"  : 100,
			"龙女·曙光"      : 100,
			"龙女·蚀"        : 200,
		]);

		total = 0;

		total = me->query_temp("apply/chuantou");

		enchase = item->query("enchase");
		if (! mapp(enchase))return total;

		k_enchase = keys(enchase);

		if (! sizeof(k_enchase))return total;

		k_chuantou_item = keys(chuantou_item);

		for (i = 0; i < sizeof(k_enchase); i ++ )
		{
			name = enchase[k_enchase[i]]["name"];
			if (member_array(name, k_chuantou_item) != -1)
			{	
				  total = total + chuantou_item[name];	
			}
		}
		
		return total;
}

// 发挥镶嵌物品的特效
public void do_enchase_attack(object item, object me, object victim, int damage)
{
	mapping enchase;
	int i_en, i, i_temp, j;
	string item_type, ran_item_enchase, s_temp;
	string ran_enchase_affect;

	string *k_enchase, *k_affect_list;
	mapping affect_list;
	string ran_affect;
	int i_ef;

	int ef_point, ef_ran, n_ef;
	int ran_result;
	string msg;

	object shenlong; // 碧海神龙

	mapping list = ([
		"天魔·幽灵"   :  ([
					"suck_neili" : (["ran"    :  1 + random(3),
                                     "point"  :  30 + random(21)]),
		]),
		"天魔·梦幻"   :  ([
					"add_damage" : (["ran"    :  1 + random(3),
                                     "point"  :  30 + random(21)]),
		]),
		"天魔·鬼煞"   :  ([
					"suck_qi"    : (["ran"    :  1 + random(3),
                                     "point"  :  10 + random(21)]),
		]),
		"天魔·妖晶"   :  ([
					"add_wound"  : (["ran"    :  1 + random(3),
                                     "point"  :  30 + random(21)]),
		]),
		"阴月·乱世"   :  ([
					"busy"  :      (["ran"    :  2 + random(2),
                                     "point"  :  2 + random(2)]),
		]),
		"阴月·冲天"   :  ([
					"add_damage"  : (["ran"   :  2 + random(2),
                                      "point" :  30 + random(21)]),
		]),
		"阴月·落霞"   :  ([
					"jing_wound"  : (["ran"   :  1 + random(3),
                                      "point" :  10 + random(11)]),
		]),
		"阴月·圣光"   :  ([
					"suck_neili"  : (["ran"   :  2 + random(2),
                                      "point" :  50]),
		]),
		"天娇·伏魔"   :  ([
					"add_wound"  : (["ran"    :  2 + random(2),
                                     "point"  :  50]),
		]),
		"天娇·沉沦"   :  ([
					"add_damage"  : (["ran"   :  3 + random(3),
                                      "point" :  50]),
		]),
		"天娇·神泣"   :  ([
					"jing_wound"  : (["ran"   :  2 + random(2),
                                      "point" :  15 + random(6)]),
		]),
		"天轿·觉醒"   :  ([
					"suck_qi"    : (["ran"    :  2 + random(2),
                                     "point"  :  30]),
		]),
		"轮回·炼狱"   :  ([
					"poison_lianyu"    : ([ "ran"    :  2 + random(2),
                                            "point"  :  me->query_temp("apply/damage")]),
		]),
		"轮回·饿鬼"   :  ([
					"suck_neili" : (["ran"    :  3 + random(3),
                                     "point"  :  50 + random(31)]),
		]),
		"轮回·修罗"   :  ([
					"add_wound"  : (["ran"    :  3 + random(3),
                                     "point"  :  50 + random(31)]),
		]),
		"轮回·烈火"   :  ([
					"no_exert"   : (["ran"    :  2 + random(2),
                                     "point"  :  10 + random(6)]),
		]),
		"轮回·裁决"   :  ([
					"no_perform"   : (["ran"    :  2 + random(2),
                                       "point"  :  10 + random(6)]),
		]),
		"轮回·重生"   :  ([ // 特别地
					"lunhui-chongsheng" : (["ran"    :  3 + random(3),
                                            "point"  :  0]),
		]),
		"轮回·灭世"   :  ([ // 特别地 ：致命攻击
					"unconcious" : (["ran"    :  1,
                                     "point"  :  0]),
		]),
		
		"炎黄之星"   :   ([ // 多种复合特效
					"suck_neili" : (["ran"    :  5,
                                     "point"  :  150]),
					"suck_qi"    : (["ran"    :  5,
                                     "point"  :  150]),
		]),
		"龙女之泪"   :   ([ // 多种复合特效
					"suck_neili" : (["ran"    :  5,
                                     "point"  :  100]),
					"suck_qi"    : (["ran"    :  5,
                                     "point"  :  100]),
					"reduce_def" :(["ran"   :  5, // 化解对方防御等级1
						            "point" :  100]),
					"reduce_dam" :(["ran"   :  5, // 化解对方伤害等级1
						            "point" :  100]),
		]),
		"炎黄·君临天下":  ([
					"jing_wound"  : (["ran"   :  3 + random(3),
                                      "point" :  50 + random(31)]),
					"busy"  :       (["ran"    :  5,
                                      "point"  :  5]),
		]),		
		"炎黄·国色天香":   ([ // 多种复合特效
					"suck_neili" : (["ran"    :  5,
                                     "point"  :  100]),
					"suck_qi"    : (["ran"    :  5,
                                     "point"  :  100]),
					"busy"  :       (["ran"    :  5,
                                      "point"  :  5]),
					"no_exert"   : (["ran"    :  3,
                                     "point"  :  15]),
					"no_perform"   : (["ran"    :  3,
                                       "point"  :  15]),
		]),		
		"炎黄·战神":  ([ // 特别地 ：致命攻击
					"unconcious" : (["ran"    :  2,
                                     "point"  :  0]),
					"add_damage"  : (["ran"   :  5,
                                      "point" :  200]),
		]),		
		"龙女·碧海神龙":  ([ // 特别地 ：召唤神龙
					"summon_shenlong": (["ran"   :  5,
                                         "point" :  0]),
					"add_damage"  : (["ran"   :  5,
                                      "point" :  200]),					
		]),
		"龙女·曙光"   :  ([
					"add_wound"  : (["ran"   :  3 + random(3),
                                      "point" :  100 + random(101)]),
					"jing_wound" : (["ran"   :  3 + random(3),
                                      "point" :  35 + random(26)]),
		]),
		"龙女·蚀":       ([
					"add_damage"  : (["ran"   :  3,
                                      "point" :  300]),					
		]),
	]);


    // 发挥项链的特殊攻击技能
	do_neck_attack(me, victim);

	// 发挥护腕特殊攻击效果
	do_wrists_attack(me, victim, "");

    // 不是自造兵器则返回
	if (! item->is_item_make())return;

	if (item->query("skill_type"))item_type = "weapon";
	if (item->query("armor_type") == "hands")item_type = "weapon";

    // 不是兵器则返回
	if (item_type != "weapon")return;

	// 没有镶嵌则返回
	enchase = item->query("enchase");
	
	if (! mapp(enchase))return;
	i_en = sizeof(enchase);
	if (! i_en)return;

	// 几率计算 平均?% + 几个累加%
	// 同一时间随机选取一种特效
	k_enchase = keys(enchase);

	// 发挥穿透特效
	victim->receive_wound("qi", chuantou_point(me, item), me);	

    // 从已镶嵌的物品中随机选择一个
	ran_item_enchase = enchase[k_enchase[random(i_en)]]["name"];

	// 从选取的物品中随机选择一种特效
	affect_list = list[ran_item_enchase];
	if (! affect_list)return;

	k_affect_list = keys(affect_list);

	i_ef = sizeof(k_affect_list);
	if (! i_ef)return;

	ran_affect = k_affect_list[random(i_ef)];

	// 特效选取好后则计算特效几率及威力（时间等）
	// 几率计算 平均?% + 几个累加%
	// 威力计算，当前获取的随机镶嵌物品的该特效威力
	
	// 先计算威力
	ef_point = list[ran_item_enchase][ran_affect]["point"];

	// 计算几率
	// 先计算几率总和及该特效个数
	n_ef = 0;
	ef_ran = 0;
	for (i = 0; i < i_en; i ++)
	{
		// 分别依次获得已镶嵌的物品
		s_temp = enchase[k_enchase[i]]["name"];

		// 如果效果定义中无该镶嵌物品则跳过
		if (member_array(s_temp,keys(list)) == -1)continue;

		// 如果该镶嵌物品存在此特效
		if (mapp(list[s_temp][ran_affect]))
		{
			n_ef += 1;
			ef_ran += list[s_temp][ran_affect]["ran"];
		}
	}

	// 得到最后计算的几率
	if (n_ef <= 0)ran_result = 3;  //以防万一
	else
		ran_result = ef_ran / n_ef + n_ef - 1;

	// 如果镶嵌了宝石--红眼恶魔则提高1%～2%的几率
	for (j = 0; j < sizeof(k_enchase); j ++ )
	{
		if (enchase[k_enchase[j]]["name"] == "红眼恶魔")
		{
			ran_result = ran_result + 1 + random(2);
		}
	}

	// 随机判断是否发出该特效
	if (1 + random(100) > ran_result)return;
	
	message_combatd(HIG + "$N" HIG "的「" + ran_item_enchase + "」发出一道奇异的光芒！\n" NOR, me);

	// 发出特效
	switch(ran_affect)
	{
		case "suck_neili":
			if (victim->query("neili") < ef_point)return;

			msg = HIY "「" + ran_item_enchase + "」吸取「" + victim->name() + HIY +
                              "」内力：" + sprintf("%d", ef_point) + "点\n" NOR;
			tell_object(me, msg);			
			victim->add("neili", -1 * ef_point);
			if (victim->query("neili") < 0)victim->set("neili", 0);
			me->add("neili", ef_point);
			if (me->query("neili") > me->query("max_neili"))
				me->set("neili", me->query("max_neili"));

			return;

		case "suck_qi":
			if (victim->query("qi") < ef_point)return;

			msg = HIY "「" + ran_item_enchase + "」吸取「" + victim->name() + HIY +
                              "」气血：" + sprintf("%d", ef_point) + "点\n" NOR;
			tell_object(me, msg);
			victim->receive_damage("qi", ef_point, me);
			if (victim->query("qi") < 0)victim->set("qi", 0);
			me->add("qi", ef_point);
			if (me->query("qi") > me->query("eff_qi"))
				me->set("qi", me->query("eff_qi"));

			return;

		case "add_damage":
			msg = HIR "「" + ran_item_enchase + "」追加「" + victim->name() + HIY +
                              "」伤害：" + sprintf("%d", ef_point) + "点\n" NOR;
			tell_object(me, msg);
			victim->receive_damage("qi", ef_point, me);

			return;

		case "add_wound":
			msg = HIR "「" + ran_item_enchase + "」追加「" + victim->name() + HIY +
                              "」伤害上限：" + sprintf("%d", ef_point) + "点\n" NOR;
			tell_object(me, msg);
			victim->receive_wound("qi", ef_point, me);

			return;
		
		case "busy":
			msg = HIR "「" + ran_item_enchase + "」追加「" + victim->name() + HIY +
                              "」忙乱效果：" + sprintf("%d", ef_point) + "秒\n" NOR;
			tell_object(me, msg);
			if (! victim->is_busy())victim->start_busy(ef_point);

			return;

		case "no_exert":
			msg = HIR "「" + ran_item_enchase + "」追加「" + victim->name() + HIY +
                              "」内息紊乱：" + sprintf("%d", ef_point) + "秒\n" NOR;
			tell_object(me, msg);
			if (! victim->query_temp("no_exert"))victim->set_temp("no_exert", 1);
			tell_object(victim, HIM "你被" + me->name() + "的" + HIY + ran_item_enchase + HIM "打中，顿觉内息紊乱！\n" NOR);

			// 消除效果
			call_out("remove_effect", ef_point, victim, "内息紊乱");
			return;

		case "no_perform":
			msg = HIR "「" + ran_item_enchase + "」追加「" + victim->name() + HIY +
                              "」力道涣散：" + sprintf("%d", ef_point) + "秒\n" NOR;
			tell_object(me, msg);
			if (! victim->query_temp("no_perform"))victim->set_temp("no_perform", 1);
			tell_object(victim, HIM "你被" + me->name() + "的" + HIY + ran_item_enchase + HIM "打中，顿觉力道涣散！\n" NOR);

			// 消除效果
			call_out("remove_effect", ef_point, victim, "力道涣散");
			return;		

        // 轮回·重生的特殊效果
		// 削弱战力
		case "lunhui-chongsheng":
			if (victim->query_temp("lunhui-chongsheng/xueruo"))return;

			msg = HIR "「" + ran_item_enchase + "」追加「" + victim->name() + HIY +
                              "」削弱战力：削弱攻、防、回避、命中各30%。\n" NOR;
			
			tell_object(me, msg);
			
			victim->set_temp("lunhui-chongsheng/xueruo", 1);
			victim->set_temp("apply/damage", victim->query_temp("apply/damage") / 10 * 7);
			victim->set_temp("apply/unarmed_damage", victim->query_temp("apply/unarmed_damage") / 10 * 7);
			victim->set_temp("apply/defense", victim->query_temp("apply/defense") / 10 * 7);
			victim->set_temp("apply/attack", victim->query_temp("apply/attack") / 10 * 7);

			return;

        // 致命一击
		case "unconcious":
			if (userp(victim))return;
						
			msg = HIR "「" + ran_item_enchase + "」追加「" + victim->name() + HIY +
                              "」致命一击！\n" NOR;
			
			tell_object(me, msg);

			victim->receive_wound("qi", 1, me);
			victim->unconcious(me);

			return;
		
		// 化解防护
		case "reduce_def":
			if (userp(victim))return;
			
			if (victim->query_temp("apply/defense") <= 0)return;
						
			msg = HIR "「" + ran_item_enchase + "」追加「" + victim->name() + HIY +
                              "」化解防护：" + sprintf("%d", ef_point) + "点\n" NOR;
			tell_object(me, msg);

			if (victim->query_temp("apply/defense") < ef_point)
					victim->set_temp("apply/defense", 0);
			else
					victim->add_temp("apply/defense", -1 * ef_point);

			return;
			
		// 化解伤害
		case "reduce_dam":
			if (userp(victim))return;
						
			msg = HIR "「" + ran_item_enchase + "」追加「" + victim->name() + HIY +
                              "」化解伤害：" + sprintf("%d", ef_point) + "点\n" NOR;
			tell_object(me, msg);

			if (victim->query_temp("apply/damage") < ef_point)
					victim->set_temp("apply/damage", 0);
			else
					victim->add_temp("apply/damage", -1 * ef_point);

			if (victim->query_temp("apply/unarmed_damage") < ef_point)
					victim->set_temp("apply/unarmed_damage", 0);
			else
					victim->add_temp("apply/unarmed_damage", -1 * ef_point);

			return;

        // 召唤神龙
		case "summon_shenlong":

			if (time() - me->query_temp("last_summon_shenlong") < 180)return;

			if (! victim)return;

			msg = HIG "「" + ran_item_enchase + "」光芒万丈，一条神龙伴随着光芒从天而降。\n" NOR;
			message_combatd(msg, me);
			
			shenlong = new("/kungfu/class/misc/shenlong");
			
			if (! objectp(shenlong))return;
						
			me->set_temp("last_summon_shenlong", time());

			shenlong->move(environment(me));
			shenlong->set("long", HIC + me->name() + "的宝物龙女·碧海神龙的力量所召唤的神龙，威风无比。\n" NOR);
			shenlong->kill_ob(victim);
			shenlong->force_me("guard " + me->query("id") );

			return;

	}

	return;
}

// 消除特效
void remove_effect(object victim, string eff)
{

	if (! objectp(victim))return;
	
	switch(eff)
	{
		case "内息紊乱":
			victim->delete_temp("no_exert");			
			return;

		case "力道涣散":
			victim->delete_temp("no_perform");
		    return;
	}
	return;
}

// 降低耐久度
void reduce_consistence(object item, object me, object victim, int damage)
{
        int st;
        int con;
		int i, en_num, flag, cur_firm;
		mapping enchases;
		string* en_keys;

        // 调用镶嵌物品攻击特效	
		if (! objectp(item))return;
		if (item->query("armor_type") != "neck" && item->query("armor_type") != "armor")
			do_enchase_attack(item, me, victim, damage);

        st = item->query("stable");

        if (st < 2)
                return;

        con = item->query("consistence");

		// 降低镶嵌物品的耐久度
		flag = 0;
        enchases = item->query("enchase");
        en_num = sizeof(enchases);
        if (en_num && mapp(enchases))
		{
		en_keys = keys(enchases);
		for (i = 0; i < en_num; i ++)
		{
			cur_firm = item->query("enchase/" + en_keys[i] + "/cur_firm");
			if (enchases[en_keys[i]]["cur_firm"] == 100)continue;
			if (random(100) >= 20)/*random(st) > ((100 - cur_firm / 2) / 10) )*/continue;
			if (item->add("enchase/" + en_keys[i] + "/cur_firm", -1) > 0)
				continue;
			flag = 1;
        	if (environment(item))
			{
             			tell_object(environment(item), HIG "你的" +
                         	            item->query("enchase/" + en_keys[i] + "/name") + HIG "已经彻底损坏了。\n" NOR);

			}
			item->set("enchase/" + en_keys[i] + "/cur_firm", 0);
                        item->delete("enchase/" + en_keys[i]);
			item->save();

		}
		if (flag)
		{
			tell_object(environment(item), HIM "突然间，" + item->name() + HIM "离你而去！\n" NOR);
			destruct(item);
			return;
		}
	}

        // 寰宇天晶炼制的武器永不磨损
        if (item->query("material") == "tian jing")
                return;

        if (random(st) > (100 - con) / 13 || random(2))
                return;

        if (item->add("consistence", -1) > 0)
                return;

        item->set("consistence", 0);

        if (environment(item))
                tell_object(environment(item), HIG "你的" +
                            item->name() + HIG "已经彻底损坏了。\n" NOR);

        item->unequip();
}

// 构造物品缺省的耐久度信息
void equip_setup(object item)
{
        int stable;
        mapping dbase;

        dbase = item->query_entire_dbase();
        if (undefinedp(dbase["consistence"]))
                dbase["consistence"] = 100;

        if (undefinedp(item->query("stable")))
        {
                switch (item->query("material"))
                {
                case "cloth":
                        // 永远不会损坏
                        stable = 0;
                        break;
                case "paper":
                        stable = 3;
                        item->set("no_repair", "这东西我可没法修理。\n");
                        break;
                case "bone":
                        stable = 8;
                        item->set("no_repear", "修理这个？可别拿我寻开心。\n");
                        break;
                case "bamboo":
                case "wood":
                        stable = 10;
                        item->set("no_repair", "这东西我咋修理？\n");
                        break;
                case "copper":
                        stable = 40;
                        break;
                case "silver":
                        stable = 40;
                        break;
                case "iron":
                        stable = 45;
                        break;
                case "gold":
                        stable = 50;
                        break;
                case "steel":
                        stable = 75;
                        break;
                case "stone":
                        stable = 80;
                        break;
                default:
                        stable = 100;
                        break;
                }

                if (! item->is_item_make())
                        stable /= 2;

                item->set("stable", stable);
        }
}


// 外部调用来自 /clone/fam/item/moye-shengfu.c
// 写入文件
public int extern_write_file(string str_file, string msg, int flag)
{
	if (! write_file(str_file, msg, flag))return 0;

	return 1;
}
2371    // 来自/feature/itemmake.c的调用，强化等级增加相应的镶嵌效果 
2372    //public int qianghua_enchase_points("WEAPON", enchase_points, query("qh_level")) 
2373    public int qianghua_enchase_points(string stype, int points, int qh_level) 
2374    { 
2375            int i; 
2376            if (points <= 0)return points; 
2377            if (qh_level < 1)return points; 
2378             
2379            switch(stype) 
2380            { 
2381            case "weapon": // 武器每级强化递增x% 
2382                    for (i = 0; i < qh_level; i ++) 
2383                            points = points + points / 10; 
2384                    return points; 
2385            break; 
2386     
2387            case "wrists": // 护腕每级增加x% 
2388                    points *= 300; // 护腕传入的是sizeof(镶嵌) 
2389                    for (i = 0; i < qh_level; i ++) 
2390                            points = points + points * 3 / 50; 
2391                    return points; 
2392            break; 
2393     
2394            case "surcoat": // 披风每级增加x%，基础为镶嵌总数*300 
2395                    points *= 300; // 披风传入的是sizeof(镶嵌） 
2396                    for (i = 0; i < qh_level; i ++) 
2397                            points = points + points / 10; 
2398                    return points; 
2399            break; 
2400     
2401            default: 
2402                    return points; 
2403            break; 
2404            } 
2405     
2406            return points; 
2407    } 
2408     
2409    // 来自外部调用，强化装备等级 
2410    public int qianghua_object(object me, object obj) 
2411    { 
2412            string *can_qianghua, str_log; 
2413            int qh_level, qhobj_count, suc_points; 
2414            object qhobj; 
2415            mapping suc_points_list; 
2416             
2417            // 开放强化的道具 
2418            can_qianghua = ({ 
2419                    "hands", "surcoat", "wrists", "head", 
2420            }); 
2421    /* 
2422            suc_points_list = ([ 
2423                    "0" :  80,  // 代表0->1 
2424                    "1" :  70, 
2425                    "2" :  50, 
2426                    "3" :  30, 
2427                    "4" :  10, 
2428                    "5" :  5,   // 5升6的时候失败就会降到4级，后面都是降到4级 
2429                    "6" :  4,   
2430                    "7" :  3, 
2431                    "8" :  2, 
2432                    "9" :  1, 
2433            ]); 
2434    */ 
2435            suc_points_list = ([ 
2436                    "0" :  80,  // 代表0->1 
2437                    "1" :  70, 
2438                    "2" :  60, 
2439                    "3" :  50, 
2440                    "4" :  45, 
2441                    "5" :  40,   // 5升6的时候失败就会降到4级，后面都是降到4级 
2442                    "6" :  35,   
2443                    "7" :  20, 
2444                    "8" :  22, 
2445                    "9" :  12, 
2446            ]); 
2447                     
2448            str_log = ctime(time()); // 记录LOG 
2449             
2450            if (! objectp(me) || ! objectp(obj)) 
2451                    return notify_fail("遇到问题，暂时不能强化！\n"); 
2452             
2453            // 非自造兵器不能强化 
2454            if (! obj->is_item_make()) 
2455                    return notify_fail("只有自造物品才能强化！\n"); 
2456             
2457            // 不是自己的道具不能强化 
2458            if (obj->item_owner() != me->query("id")) 
2459                    return notify_fail("这个好像不是你的装备吧，不能强化！\n"); 
2460     
2461            // 只开放的强化类型 
2462            if (! obj->query("skill_type") && // 武器 
2463                  member_array(obj->query("armor_type"), can_qianghua) == -1) 
2464                    return notify_fail("该类型的装备暂时还未开放强化，详见帮助文件！\n"); 
2465     
2466            // 没有镶嵌信息不能强化 
2467            if (! sizeof(obj->query("enchase"))) 
2468                    return notify_fail("这件装备似乎还没有镶嵌过任何东西，暂时不能强化！\n"); 
2469             
2470            qh_level = obj->query("qh_level"); 
2471            // 强化等级满了不能强化 
2472            if (qh_level >= 10) 
2473                    return notify_fail("该装备强化等级已满！\n"); 
2474     
2475            // 需要消耗强化物品 
2476            if (! objectp(qhobj = present("tie ying", me))) 
2477                    return notify_fail("你身上没有铁英石，不能强化装备!\n"); 
2478             
2479            // 强化物品是否正确 
2480            if (! qhobj->query("qhobj")) 
2481                    return notify_fail("你身上没有铁英石，不能强化装备。\n"); 
2482     
2483            qhobj_count = qh_level - 3; 
2484            if (qhobj_count < 1)qhobj_count = 1; 
2485            // 数量是否足够 
2486            if (qhobj->query_amount() < qhobj_count) 
2487                    return notify_fail("你身上没有足够的铁英石，当前强化需要" +  
2488                                       sprintf("%d", qhobj_count) + "个。\n"); 
2489     
2490            // 开始强化 
2491            str_log += " " + obj->name() + "(" + obj->query("id") + ") of " + obj->item_owner() + NOR " 强化："; 
2492            // 计算成功几率suc_points 
2493            suc_points = suc_points_list[sprintf("%d", qh_level)]; 
2494            str_log += sprintf("基础成功率：%d，", suc_points); 
2495            // 如果装备有成功率加成则增加 
2496            if (obj->query("suc_points_add")) 
2497                    suc_points += obj->query("suc_points_add"); 
2498            str_log += sprintf("加成后成功率：%d，", suc_points); 
2499            // 扣掉物品 
2500            qhobj->add_amount(-1*qhobj_count); 
2501            if (qhobj->query_amount() < 1)destruct(qhobj); 
2502     
2503            // 清除增加成功率 
2504            if (obj->query("suc_points_add")) 
2505            { 
2506                    obj->delete("suc_points_add"); 
2507                    obj->save(); 
2508            } 
2509             
2510            // 等级大于6时，成功率降低5% 
2511            if (qh_level > 6)suc_points -= 5; 
2512            // 9级时候再减2% 
2513            if (qh_level == 9)suc_points -= 2; 
2514             
2515            // 是否失败 
2516            if (random(100) >= suc_points) // 失败 
2517            { 
2518                    // 当前等级不超过4级则失败提示返回即可 
2519                    if (qh_level <= 4) 
2520                    { 
2521                            str_log += sprintf("强化失败等级为：%d。\n", qh_level); 
2522                            log_file("0装备强化", str_log); 
2523                            return notify_fail("很遗憾，强化失败了！\n"); 
2524                    } 
2525                    else // 超过4级，失败就会倒退到4级 
2526                    { 
2527                            // 如果有防止失败掉级 
2528                            if (obj->query("no_reduce_level")) 
2529                            { 
2530                                    obj->delete("no_reduce_level"); 
2531                                    obj->save(); 
2532                                    str_log += sprintf("强化失败等级为：%d(强化等级未损失）。\n", qh_level); 
2533                                    log_file("0装备强化", str_log); 
2534                                    return notify_fail("很遗憾，强化失败了(强化等级未损失）！\n"); 
2535                            } 
2536                            else 
2537                            { 
2538                                    obj->set("qh_level", 4); 
2539                                    obj->save(); 
2540                                    str_log += sprintf("强化失败等级为：%d(强化等级降为4级）。\n", qh_level); 
2541                                    log_file("0装备强化", str_log); 
2542                                    write(HIG "你感觉" + obj->name() + HIG "突然离你而去，似乎发生了不可言喻的变化！\n" NOR); 
2543                                    //destruct(obj); 
2544                                     
2545                                    return notify_fail(HIR "很遗憾，强化失败了(强化等级降为4级）！\n" NOR); 
2546                            } 
2547                    } 
2548            } 
2549            else // 成功！！！ 
2550            { 
2551                    obj->set("qh_level", qh_level+1); 
2552                    obj->save(); 
2553                    write(HIG "强化成功！" + obj->name() + HIG "强化等升至" + 
2554                          sprintf(HIY "%d" HIG, obj->query("qh_level")) + "级。\n" NOR); 
2555                    str_log += sprintf("强化成功！等级从%d升级到%d\n", qh_level, obj->query("qh_level")); 
2556                    log_file("0装备强化", str_log); 
2557                     
2558                    // 全屏通告 
2559                    if (obj->query("qh_level") >= 6) 
2560                    { 
2561                            CHANNEL_D->do_channel(this_object(), "chat",  
2562                                                  "听说" + HIY + me->name() + HIM "的" + obj->name() +  
2563                                                  HIM "强化至" HIR + sprintf("%d", obj->query("qh_level")) +  
2564                                                  HIM "级。\n" NOR); 
2565     
2566                    } 
2567                    if (obj->query("qh_level") == 10) 
2568                    { 
2569                            CHANNEL_D->do_channel(this_object(), "chat",  
2570                                                  HIR "恭喜" + HIY + me->name() + HIR "的" + obj->name() +  
2571                                                  HIR "强化为旷世神兵！\n" NOR); 
2572                    } 
2573                    write(HIG "你感觉" + obj->name() + HIG "突然离你而去，似乎发生了不可言喻的变化！\n" NOR); 
2574                    //destruct(obj); 
2575                    return 1; 
2576            } 
2577    } 
2578     
2579    // 来自/inherit/char/dream_npc调用 
2580    // 判断是否掉出铁英石，外部控制几率，方便随时调整 
2581    public int drop_tieyingshi(object me) 
2582    { 
2583            int ran; 
2584             
2585            if (! objectp(me))return 0; 
2586     
2587            if (me->query("service/jiutoushe")) 
2588            { 
2589                    ran = 20000; 
2590            } 
2591            else 
2592            { 
2593                    if (! MEMBER_D->is_valib_member(me->query("id")))ran = 1200000; // 非会员爆率极低 
2594                    else 
2595                            ran = 150000; 
2596            } 
2597            /* 
2598            if (random(ran) == 1) 
2599            { 
2600                    // 记录每日铁英掉落总量 
2601                    total_tieying += 1; 
2602                    if (total_tieying % 100 == 0) 
2603                    { 
2604                            DB_D->set_data("tieying_drop/" + sprintf("%d", time() / 86400), total_tieying); 
2605                    } 
2606                     
2607                    // 返回1则掉落 
2608                    return 1; 
2609            } 
2610            */ 
2611            return 0; 
2612    } 
