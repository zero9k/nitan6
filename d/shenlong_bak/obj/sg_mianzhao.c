// sg_mianzhao.c 面罩
// Last Modified by winder on Jul. 12 2002

#include <armor.h>
inherit HEAD;

#define SGCANGKU "/d/shenlong/cangku"

void do_copy(object, object);
void do_flee(object);
int eff_fam(object);

#include "/d/shenlong/masters.h"
#include "/d/shenlong/safeplaces.h"

void create()
{
        set_name("面罩", ({ "mianzhao", "mask" }) );
        set_weight(2000);
        if( clonep() )
                set_default_object(__FILE__);
        else {
                set("unit", "个");
                set("long", "这是个神龙教特有的黑布头套，能把整个脑袋套起来，只露出两个眼睛。\n你每次为神龙教办案后，务必在尸体上写下(sign)“逆神龙教者杀！”。\n");
                set("material", "cloth");
                set("armor_prop/armor", 1);
                set("value", 10);
                set("no_get", "这样东西不能离开那儿。\n");
                set("no_drop", "这样东西不能离开你。\n");
        }
        setup();
}

void init()
{
        add_action("do_wear", "wear");
        add_action("do_remove", "remove");
        add_action("do_sign", "sign");
        add_action("do_forcejoin", "forcejoin");
        add_action("do_search", "search");
        add_action("do_open", "open");
        add_action("do_job", "job");
        add_action("do_jobtime", "jobtime");
}

int do_wear(string arg)
{
        if (!arg || (arg != "mianzhao" && arg != "mask") ) return 0;

        set_temp("apply/name", ({"蒙面人"}), this_player());
        set_temp("apply/short", ({"蒙面人(Mengmianren)"}), this_player());
        set_temp("apply/long", ({"一个黑布蒙面，神秘兮兮的家伙。\n"}), this_player());

        return 0;
}

int do_remove(string arg)
{
        if (!arg || (arg != "mianzhao" && arg != "mask") ) return 0;

        delete_temp("apply/name", this_player());
        delete_temp("apply/short", this_player());
        delete_temp("apply/long", this_player());

        return 0;
}

int do_sign(string arg)
{
        object obj;
        object me = this_player();
        string v_id, v_name;
        mapping pkjob;
        int mygain;

       if( !query("sg/spy",me) && !mapp(query("sgjob_join",me)) ) 
                return 0;

        if( !arg || arg != "corpse" || !(obj = present(arg, environment(me))) )
                return 0;

        if( query("my_killer", obj) != query("id", me) )
                return 0;

        v_name=query("victim_name", obj);
        if( query("signed", obj) || 
                (v_name != query("sgjob/victim_name", me) && 
                v_name != query("sgjob_join/victim_name", me)) )
                return 0;

        if( v_id=query("sgjob_join/victim_id", me) )
        {
                delete("sgjob_join", me);
                set("sg_ok/join", 1, me);
        }
        else
        {
                v_id=query("sgjob/victim_id", me);
                delete("sgjob", me);
                set("sg_ok/pk", 1, me);
                mygain=query("combat_exp", obj)/1000;
                addn("sg/exp", 100+mygain*(mygain/2000+1)/40, me);
                if( mygain > 1000 ) mygain = 1000;
                addn("combat_exp", mygain, me);
        }

        set("signed", 1, obj);
        log_file("test/ShenlongPker",sprintf("%s于%s时杀了%s(%s).\n",query("name", me),ctime(time()),v_name,v_id));

        set("sg_victim/"+time(), v_id, me);
        tell_room(environment(me), "蒙面人在尸体旁写下“逆神龙教者杀！”几个大字。", ({me}));
        tell_object(me, "你在尸体旁写下“逆神龙教者杀！”几个大字。\n");
        return 1;
}

int do_forcejoin(string arg)
{
        object *inv, target, me = this_player();
        int fp, dp;
        mixed busy;

        if( !query("sg/spy", me) )
                return notify_fail("你不是神龙教的人，来什么劲？\n");
        if( query("sg_ok/forcejoin", me) )
                return notify_fail("你先回去复命吧。\n");
        if( query("no_fight", environment(me)) )
                return notify_fail("这里不是强迫的地方。\n");
        if ( busy = me->query_busy() )
        {
                if (intp(busy)) me->start_busy(busy+1);
                        return notify_fail("你现在正忙着！\n");
        }
        if( !arg || !(target = present(arg, environment(me))) )
                return notify_fail("你要逼谁入神龙教？\n");

        if( query("sgjob/forcejoin", me) != query("id", target) )
                return notify_fail("你找错人了。\n");

        if( query("sg/spy", target) || 
                query("family/family_name", target) == "神龙教" )
                return notify_fail(target->name()+ "已经加入神龙了，不用再费心了。\n");
        if( !living(target) )
                return notify_fail("你现在再逼"+target->name()+"也没有用。\n");
        if( query_temp("forcejoining", me) )
                return notify_fail("你已经在逼迫人了。\n");
        if( query("jing", me)*100/query("max_jing", me)<40 || 
                query("jingli", me)*100/query("max_jingli", me)<40 )
                return notify_fail("你太累了，休息一会儿再劝吧。\n");

        me->receive_damage("jing", 60);
        me->receive_damage("qi", 60);

        message_vision("\n$N对$n大声喝道：" + RANK_D->query_rude(target) + "快加入神龙教，否则格杀无论！！！\n", me, target);

        inv = all_inventory(environment(target));
        for(int i = 0; i < sizeof(inv); i++)
        {
                if( living(inv[i]) && inv[i] != me && inv[i] != target &&
                        !query("sg/spy", inv[i]) && 
                        query("family/family_name", inv[i]) && 
                        query("family/family_name", inv[i]) == query("family/family_name", target))inv[i]->kill_ob(me);
        }
        fp = (int)me->query_skill("force") * 3 +
                (int)me->query_skill("parry") * 2 +
                (int)me->query_skill("dodge");
        fp*=query("sg/exp", me)+
                query("combat_exp", me)/1000+1;
        dp=query("jing", target)+query("max_jing", target)+
                query("qi", target)+query("max_qi", target)+
                query("jingli", target)*2+
                query("neili", target)*3;
        if ( dp < 1 ) dp = 1;
        dp*=query("combat_exp", target)/1000+1;

        set_temp("forcejoining",query("id",  target), me);
        call_out("complete_forcejoin", 8+random(10), me, target, fp, dp);
        return 1;
}

private void complete_forcejoin(object me, object target, int fp, int dp)
{
        object ob, *inv;
        int t_shen, t_exp, bonus = 2, times, record;

        if( !present(target, environment(me)) ) return ;
        if( !objectp(me) )  return;
        delete_temp("forcejoining", me);
        addn_temp("forcetimes", 1, target);

        if( !objectp(target) ) return;
        if( environment(target) != environment(me) ) return;

        if( !living(target) )
        {
                tell_object(me, "你现在再逼也没有用。\n");
                return;
        }
        t_shen=query("shen", target);
        while ( t_shen > 10 )
        {
                bonus += 1;
                t_shen = t_shen/16;
        }
        if( member_array(query("id", target),masters) != -1)bonus*=30;
        dp *= bonus;
        dp *= 4 * eff_fam(target);
        if( target->query_condition("embedded") ) dp /= 2;
        if( (int)target->query_condition("snake_poison") > 0 ) dp /= 2;
        if( (int)target->query_condition("insect_poison") > 0 ) dp /= 2;
        if( (int)target->query_condition("xx_poison") > 0 ) dp /= 2;
        if( query_temp("hgmz", target)>0)dp/=4;
        if( (int)target->query_condition("hyz_damage") > 0 ) dp /= 2;
        if( (int)target->query_condition("hsf_poison") > 0 ) dp /= 2;
        if( dp < 1 ) dp = 1;
        if( random(fp+dp)  > dp )
        {
                message_vision("$N别无它法，只得答应$n加入神龙教。\n", target, me);
                if( member_array(query("id", target),masters) != -1 )
                        bonus += 100;
                bonus+=query("sg/exp", me)*2000/query("combat_exp", me);
                bonus *= eff_fam(target);
                if(bonus>300)bonus = 300;

                record = bonus/3 + random(bonus/3);
                addn("combat_exp", record, me);
                addn("sg/exp", 1, me);
                addn("shen", -query("combat_exp", target)/100, me);

                log_file("test/ForceJoin",sprintf("%s于%s时逼%s入神龙教获得%s经验点\n",query("name", me),ctime(time()),query("name", target),chinese_number(record)));

                bonus /= 4;
                addn("potential", bonus/3+random(bonus/3), me);
                if( query("potential", me)>query("max_potential", me) )
                set("potential",query("max_potential",  me), me);

                delete("sgjob", me);
                set("sg_ok/forcejoin", 1, me);

                inv = all_inventory(target);
                for(int i = 0; i < sizeof(inv); i++)
                        if( inv[i]->is_unique() ||
                                query("weapon_prop/damage", inv[i])>100 || 
                                query("armor_prop/armor", inv[i])>100 )
                        {
                                inv[i]->move(me);
                                message_vision("$N将" + inv[i]->name() + "双手奉上递给$n。\n", target, me);
                        }
                set("sg/spy", 1, target);
                delete_temp("forcetimes", target);
                set("attitude", "heroism", target);
                set("shen_type", -1, target);
                target->remove_killer(me);
                target->start_busy(1);
                me->remove_killer(target);
                me->start_busy(1);
        }
        else
        {
                if( random(query_temp("forcetimes", target))>2 )
                {
                        "/cmds/std/halt"->main(target);
                        if( !random(3) ) do_flee(target);
                        else target->random_move();
                       if( !mapp(query("exits", environment(target))) ) 
                               target->return_home(query("startroom",target)); 
                }
                else
                {
                        target->kill_ob(me);
                        message_vision("\n$N不屑地撇撇嘴：" + RANK_D->query_rude(me) + "，我先杀了你这狂徒！！！\n", target);
                }
                me->start_busy(1+random(3));
        }
}

int eff_fam(object ob)
{
        switch(query("family/family_name", ob) )
        {
                case "武当派"   :
                case "全真教"   :
                case "少林派"   :
                case "南少林派" :
                case "云龙门"   :
                case "红花会"   :
                case "大理段家" :  return 10; break;
                case "峨眉派"   :
                case "昆仑派"   :
                case "古墓派"   :
                case "嵩山派"   :  return  9; break;
                case "泰山派"   :
                case "华山派"   :
                case "衡山派"   :
                case "恒山派"   :  return  8; break;
                case "青城派"   :
                case "姑苏慕容" :
                case "桃花岛"   :  return  7; break;
                case "逍遥派"   :
                case "凌霄城"   :  return  6; break;
                case "明教"     :
                case "丐帮"     :  return  5; break;
                case "雪山寺"   :
                case "黑木崖"   :  return  4; break;
                case "五毒教"   :
                case "白陀山派" :  return  3; break;
                case "铁掌帮"   :
                case "星宿派"   :  return  2; break;
                default:
                        if( query("attitude", ob) == "aggressive" || 
                                query("attitude", ob) == "heroism" )
                                return 6;
                        else return 3;
                        break;
        }
        return  5;
}

void do_flee(object ob)
{
        message("vision", ob->name() + "一转眼便消失了。\n", environment(ob), ({ob}));

        if( mapp(query("family", ob)) && 
                member_array(query("id", ob),masters) == -1 )
        {
                switch(query("family/family_name", ob) )
                {
                case "武当派"   : ob->move("/d/wudang/sanqingdian"); break;
                case "全真教"   : ob->move("/d/quanzhen/datang1"); break;
                case "少林派"   : ob->move("/d/shaolin/dxbdian"); break;
                case "南少林派" : ob->move("/d/nanshaolin/pingtai"); break;
                case "云龙门"   : ob->move("/d/beijing/dating"); break;
                case "红花会"   : ob->move("/d/huijiang/zongduo"); break;
                case "大理段家" : ob->move("/d/dali/wangfugate"); break;
                case "峨眉派"   : ob->move("/d/emei/hcaguangchang"); break;
                case "昆仑派"   : ob->move("/d/kunlun/qianting"); break;
                case "古墓派"   : ob->move("/d/gumu/zhongting"); break;
                case "嵩山派"   : ob->move("/d/songshan/chanyuan"); break;
                case "泰山派"   : ob->move("/d/taishan/riguan"); break;
                case "华山派"   : ob->move("/d/huashan/buwei3"); break;
                case "衡山派"   : ob->move("/d/henshan/zhurongdian"); break;
                case "恒山派"   : ob->move("/d/hengshan/baiyunan"); break;
                case "青城派"   : ob->move("/d/qingcheng/songfengguan"); break;
                case "姑苏慕容" : ob->move("/d/yanziwu/lanyue"); break;
                case "桃花岛"   : ob->move("/d/taohua/dating"); break;
                case "逍遥派"   : ob->move("/d/xiaoyao/qingcaop"); break;
                case "凌霄城"   : ob->move("/d/lingxiao/dadian"); break;
                case "明教"     : ob->move("/d/mingjiao/square"); break;
                case "丐帮"     : ob->move("/d/yueyang/gaibangtang"); break;
                case "雪山寺"   : ob->move("/d/xueshan/dadian"); break;
                case "黑木崖"   : ob->move("/d/heimuya/chengdedian"); break;
                case "五毒教"   : ob->move("/d/wudujiao/dating"); break;
                case "白陀山派" : ob->move("/d/baituo/dating"); break;
                case "铁掌帮"   : ob->move("/d/tiezhang/guangchang"); break;
                case "星宿派"   : ob->move("/d/xingxiu/riyuedong1"); break;
                default  : 
                        ob->move(safeplaces[random(sizeof(safeplaces))]); break;
                }
        }
        else ob->move(safeplaces[random(sizeof(safeplaces))]);

        message("vision", ob->name() + "走了过来。\n", environment(ob), ({ob}));
}      

private int is_spy(object ob)
{
        if( !query("sg/spy", ob))return 0;
        return 1;
}

int do_search(string arg)
{
        string str, *option;
        object *list;
        int i, j, ppl_cnt, total;

        if( !arg || !(arg == "shenlong" || arg == "神龙教") ||
                query("sg/exp", this_player()) < 1000 ) 
                return 0;

        if( !wizardp(this_player()) && query("jing", this_player())<30 )
                return notify_fail("你的精神太差了，没有办法得知本教组织的详细资料。\n");

        this_player()->receive_damage("jing", 30);
        seteuid(getuid());

        str = "◎ " + MUD_NAME + "\n";
        str += "─────────────────────────────────────\n";

        list = filter_array(users(), "is_spy", this_object());   
        total = sizeof(list);

        list = sort_array(list, "sort_user", this_object());
        j = sizeof(list);
        while( j-- )
        {
// Skip those users in login limbo.
                if(!environment(list[j]) ) continue;
                if(this_player() && !this_player()->visible(list[j]) ) continue;
                str = sprintf("%s%12s %s\n", str, RANK_D->query_rank(list[j]), list[j]->short(1) );
        }

        str += "─────────────────────────────────────\n";
        str = sprintf("%s共有 %d 位神龙教成员连线中。\n", str, total);

        this_player()->start_more(str);
        return 1;
}

int sort_user(object ob1, object ob2)
{
        return (int)query("sg/exp",ob2) - (int)query("sg/exp",ob1); 
}

int do_job()
{
        string job;

        if( query("sg_ok/pk", this_player()) || 
                this_player(query("sg_ok/join", )) )
        {
                write("你最近完成了一件追杀任务。\n");
                return 1;
        }
        if( query("sg_ok/stuff", this_player()) )
        {
                write("你最近完成了一件找东西的任务。\n");
                return 1;
        }
        if( query("sg_ok/forcejoin", this_player()) )
        {
                write("你最近完成了一件逼迫入教的的任务。\n");
                return 1;
        }
        if( !mapp(query("sgjob",this_player())) && 
                !mapp(query("sgjob_join",this_player())) ) 
        {
                write("你现在没有教务缠身。\n");
                return 1;
        }
        if( (job=query("sgjob/forcejoin", this_player())) )
        {
                write("你当前的任务是逼迫" + job + "入教。\n");
                return 1;
        }
        if( (job=query("sgjob/victim_name", this_player())) )
        {
                write("你当前的任务是追杀"+job+"("+query("sgjob/victim_id", this_player())+")。\n");
                return 1;
        }
        if( (job=query("sgjob/sgjob", this_player())) )
        {
                write("你当前的任务是找" + job + "。\n");
                return 1;
        }
        write("你现在没有教务缠身。\n");
        return 1;
}

int do_jobtime()
{
        int start_time, time, s, m;

        if( mapp(query("sg_ok",this_player())) ) 
                return notify_fail("你已经完成任务了，无需在乎时间。\n");
        if( !(start_time=query("sgjob/assigntime", this_player())) )
        if( !(start_time=query("sgjob_join/assigntime", this_player())) )
                return notify_fail("你现在没有教务缠身。\n");
        time = time() - start_time;
        if( time > 1800 )
        {
                write("已经过了半小时，可以去问新任务了。\n");
                return 1;
        }
        s = time % 60;
        m = time / 60;
        write("加油！加油！加油！已经过了" + chinese_number(m) + "分" + chinese_number(s) + "秒\n");
        return 1;
}