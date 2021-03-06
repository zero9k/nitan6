// mu.c

#include <ansi.h>
inherit NPC;
inherit F_MASTER;

string leave_kongdong( object );



void create()
{
        seteuid(getuid());
        set_name("木灵子", ({ "mu lingzi", "mu","lingzi" }) );
        set("class","taoist");
        set("gender", "男性" );
        set("age", 50);
        set("str", 30);
        set("cor", 30);
        set("cps", 30);
        set("int", 25);
        set("ker", 24);
        set("kar", 30);
        set("max_force", 1500);
        set("force", 1500);
        set("force_factor", 3);

        set("rank_info/respect", "老神仙");

        set("long",
    "木灵子红光满面，一看就是修道有成之人，颇有些仙风道骨之相。\n");

        create_family("崆峒派", 1,"掌门人");
        set("positive_score",30000);
        set("combat_exp", 1000000);
        set("social_exp", 200000);

	set_wugong("kongdong-xinfa",130);
    set_wugong("unarmed",100,100,0);
	set_wugong("shizi-zhan",100);
	set_wugong("dahong-quan",100);
	set_wugong("wulong-quan",100);
    set_wugong("qishang-quan",120,120,1);
	set_xiuwei_by_type("unarmed",2300);
	set_wugong("sword",100);
    set_wugong("fengyun-jian",100);
    set_wugong("qimen-bagua",300);
	set_wugong("kongling-jian",100);
	set_xiuwei_by_type("sword",900);
	set_wugong("jiben-qinggong",100);
	set_wugong("nieyun-bufa", 100,100,1);
	set_wugong("luofeng",100,100,1);
	set_xiuwei("dodge",900);
	
	set( "inquiry",([ /*sizeof() ==1 */
	    "出师" : (: leave_kongdong :),
	]) );

    set("env/attack", 75);
    set("force_factor", 20);

    setup();
    carry_object(__DIR__"obj/cloth1")->wear();
}

void init()
{

	add_action("give_quest", "quest" );

}

void attempt_apprentice(object ob)
{
         //Hydra 19980507 for 出师
        if( ob->query("hydra/出师/崆峒派") )
        {
            message_vision(CYN"$N对$n说道：“你既已离开师门，怎么又回来"
"了呢？”\n"NOR,this_object(),ob);
            return 0;
        }
        if( (int)ob->query("PKS") > 10 ) 
        {
                command("hmm");
                command("say 你手上血腥浓重，怎能修身得道？\n");
                return;
        }
        if ( ob->query("gender") == "无性" )
        {
                command("heihei");
                command("say 你是一个阉割之人，怎能修身得道？\n");
                return;
        }
        command("en");
        say("木灵子笑道：“这位"+RANK_D->query_respect(ob)+"真是个可造之才呀！\n"
            "将来必能发扬我崆峒派门庭！\n");
        command("recruit "+ob->query("id") );
        ob->delete("master");
        ob->set("class","taoist");
        ob->set_family("title","开山大弟子");
}


//Hydra:add for 出师 19980507
string leave_kongdong( object me )
{
    object this;
    me = this_player();
    this = this_object();

    if( me->query("hydra/出师/崆峒派"))
    {
        return "你不是已经出师了吗？";
    }
    if( me->query("family/family_name") != "崆峒派" )
    {
        command( "? "+me->query("id") );
        return "你好象不是我崆峒派门人啊？";
    }
    if( me->query_temp("出师比划"))
    {
        return "我不是让你先跟我比划一下吗？";
    }
    
    if( me->query( "class_score/崆峒派" ) < 1000 )
    {
        message_vision(CYN"$N对$n道：“你在我门下多年，为师门所做"
"未免太少了一些。”\n"NOR,this,me );
        if(me->query("class_score/崆峒派") > -50 )
            me -> add( "class_score/崆峒派",-10);
        return "你还是先为光大师门多干点事吧！\n";
    }

    if( me->query_jibie("qishang-quan") > 80 )
    {
        if( me->query("age") < 20 )
        {
            message_vision(CYN"$N对$n说道：“你年纪轻轻就出去，人家会以为我崆峒派"
"无人的。”\n"NOR,this,me);
            if(me->query("class_score/崆峒派") > -50 )
                me -> add( "class_score/崆峒派",-10);            
            return "过两年再说吧。\n";
        }
        else
        {
            me->set_temp("出师比划",1);
            return "你先空手跟为师比划一下，让我看看你拳脚功夫如何了。";
        }
    }
    else
    {
        return "以你现在的实力，难于在江湖上独挡一面，还是跟为师再学学吧！";
    }
    return "";
}        

int accept_fight( object ppl )
{
    object this;
    this = this_object();
    
    if( ppl-> query_temp( "出师比划" ))
    {
        if(this->is_fighting() )
        {
            return notify_fail( "现在木灵子不能跟你比划。\n");
        }    
        if(ppl->query_temp("weapon"))
            return notify_fail("木灵子道：“我不是让你空手跟我比划吗？”\n");
        this->set( "eff_kee",this->query("max_kee"));
        this->set( "kee",this->query("max_kee"));
        this->set( "eff_sen",this->query("max_sen"));
        this->set( "sen",this->query("max_sen"));
        this->set( "force",this->query("max_force"));
        this->set( "force_factor",100);
        this->set( "env/attack",90 );
        message_vision(CYN"$N道，“好，进招吧。”\n",this);
        return 1;
     }
     return ::accept_fight( ppl );
}

int lose_bihua( object ppl)
{
    object this;
    this=this_object();
    if( ppl->query_temp("出师比划"))
    {
        command( "chat* haha" );
        command( "chat 我崆峒终于也出了一个人才。");
        command( "say 真是青出于蓝胜于蓝啊，你的功夫已经比为师的高了。");
        command( "say 如此，你就另谋高就去吧。");  
        ppl -> delete_temp("出师比划");    
        ppl -> set("title","普通百姓");
        ppl -> delete("family");
        ppl -> delete("class");
        ppl -> set("hydra/出师/崆峒派",1);
             LOG_D->do_log(ppl,sprintf("你获得了木灵子的允许，从崆峒出师。\n"));
        log_file("chushi",sprintf("%s(%s)于%s顺利从崆峒出师。\n",ppl->query("name"),
            getuid(ppl),ctime(time()) ) );
        message_vision("$N恭恭敬敬地跪了下来，向$n磕了四个响头。\n",ppl,this);
    }
    return 1;
}
        
int win_bihua( object ppl )
{
    object this;
    this=this_object();
    if( ppl->query_temp("出师比划"))
    {
        command("pat "+ppl->query("id"));
        command("say 看来为师的功夫还略高你一些，还够你学一阵的。");
        ppl->delete_temp("出师比划");
        if(ppl->query("class_score/崆峒派") > -50 )
            ppl->add("class_score/崆峒派",-10);
    }
    return 1;
}
            
void heart_beat()
{
    object this,ob;
    string *bihua;
    this = this_object();
    bihua = this->query_bihua();
    
    foreach( string name in bihua )
    {
       if( objectp( ob=find_player(name)) && 
           ob->query_temp("出师比划") && 
           ob -> query_temp("weapon") )
       {
           if(ob->query("class_score/崆峒派") > -50 )
               ob -> add( "class_score/崆峒派",-100);
           command("heng "+ob->query("id"));
           command("say 我不是让你空手跟我打的吗？居然敢欺骗师傅！！");
           command("stop");
        }
    }
    return ::heart_beat();
}
