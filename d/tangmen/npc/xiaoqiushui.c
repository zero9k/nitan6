// xiao qiushui.c ����ˮ

#include <ansi.h>

inherit NPC;

string ask_me();
void create()
{
        set_name("����ˮ", ({ "xiao qiushui", "xiao" }));
        set("nickname", HIY "����" NOR);
        set("long", 
                "ԭ�����������Ʒ�����������Ϊ���Ʒ��������ŵ�����ˮ��\n"
                "���ڽ������������壬����Զ�����˳ơ���������\n"
                "����Լ��ʮ���࣬��Ȼ�ܾ���ĥ����Ȼ����һ��������\n");
        set("gender", "����");
        set("age", 20);
set("attitude", "peaceful");        
        set("shen_type", 1);
        set("str", 28);
        set("int", 30);
        set("con", 30);
        set("dex", 32);
        
set("max_qi", 300000);        
set("max_jing", 150000);        
set("neili", 5000000);        
        set("max_neili", 3000);
set("jiali", 5000);        
set("combat_exp", 500000000);        
        set("score", 700000);

set_skill("force", 2000);        
set_skill("biyun-xinfa",1200);        
set_skill("dodge", 1800);        
set_skill("qiulin-shiye", 1200);        
set_skill("unarmed", 1200);        
set_skill("biye-wu", 1300);        
set_skill("parry", 1200);        
set_skill("sword", 1400);        
set_skill("wuzhan-mei", 1300);        
set_skill("literate", 2580);            

        map_skill("force", "biyun-xinfa");
        map_skill("dodge", "qiulin-shiye");
map_skill("unarmed", "biye-wu");        
        map_skill("parry", "wuzhan-mei");
        map_skill("sword", "wuzhan-mei");
        create_family("����", 7, "����");
        set("class", "tangmen");
        
        
set("chat_chance_combat", 200);        
        set("chat_msg_combat", ({
(: perform_action, "sword.yumei" :),        
                (: perform_action, "sword.wumei" :),
(: perform_action, "unarmed.huimeng" :),        
                (: exert_function, "recover" :),
        }) );
        set("inquiry", 
                ([
                        "�Ʒ�" : (: ask_me :)
                ]));
        set("yu_count", 1);

        setup();
carry_object("/clone/weapon/changjian")->wield();        
    carry_object("/clone/cloth/cloth")->wear();
}
string ask_me()
{object ob;
 object me;
 me=this_player();
        if (query("yu_count") < 1){return "���Ʒ����������ҵģ���ϧ�ҵ������Ѿ����������ˡ�\n";}
        command("touch"+query("id", this_player()));
        command("say ���Ʒ����������ҵģ�\n");
        command("sigh" );
        command("say �Դӵ��մ�������֮���Ҿͺ��Ʒ���������ˡ�\n");
        ob=new("/d/tangmen/obj/yupei");
        ob->move(this_player());
        message_vision("$N����$nһ�����塣\n", me, ob);
        addn("yu_count", -1);
        return"���������彻���Ʒ��ɡ�\n";
        

                                                        

}