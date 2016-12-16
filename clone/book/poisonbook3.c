// poisonbook3.c

inherit ITEM;

void create()
{
        set_name("������ƪ", ({ "jing", "book" }));
        set_weight(50);
        if( clonep() )
                set_default_object(__FILE__);
        else {
                set("unit", "��");
                set("long", "����һ��������С�ᣬ������������ļ����˸����ö��ķ��ţ�������(kan)��һ����\n");
                set("no_drop", "�������������뿪�㡣\n");
                set("value", 0);
                set("material", "silk");
        }
}
void init()
{
        add_action("do_read", "kan");
        add_action("do_read", "see");
        add_action("do_read", "du");

//        this_object()->set("created", 1);
        remove_call_out ("destroy_book");
        call_out ("destroy_book", 28800); // 8 hours

}

void destroy_book()
{
        message_vision("һ��紵����"+query("name", this_object())+"ƬƬ���䣬����ȥ�ˡ�\n",environment(this_object()));
        destruct(this_object());
}

int do_read(string arg)
{
        object me;
        int jing_cost, gain;
        me=this_player();
        if( !arg||!( arg == "jing"||arg =="book"))
                return notify_fail("ʲô��\n");
        if (!present(this_object(), this_player()))
                return 0;

        if( me->is_busy() )
                return notify_fail("������æ���أ����п��ܾ����ж�...\n");
        if( me->is_fighting() )
                return notify_fail("���ڲ��뵽���ж�����̫���˰ɣ�\n");
        if( query("family/family_name", me) != "�嶾��"){
                remove_call_out ("destroy_book");
                call_out ("destroy_book", 0);
        }
        if( (int)me->query_skill("five-poison",1)<150
         || query("combat_exp", me)<300000 )
                return notify_fail("�㿴����ȥ������û�����Ȿ����д�Ķ���ɶ��\n");
        if( (int)me->query_skill("five-poison",1)>300 )
                return notify_fail("��Ķ����Ѿ��������裬�Ȿ�鲻�����㳤�����١�\n");

        jing_cost=35+(20-query("int", me));
        if( query("jing", me)<jing_cost )
                return notify_fail("������ͷ�����ͣ�����Ϣ��Ϣ�ˡ�\n");
        me->receive_damage("jing", jing_cost);

        gain = (int)me->query_skill("five-poison", 1)/4+(int)me->query_skill("literate",1)/2;
        me->improve_skill("five-poison", gain);

        message_vision("$N���ž����ϸ�Ķ�,ֻ���������ص������ö����˵�\n������������δ�ţ����ſ��Ž���ס����һ���亹��\n", me);

        return 1;
}