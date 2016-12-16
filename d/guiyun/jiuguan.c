// Room: /d/guiyun/jiuguan.c
// Last Modified by winder on Jul. 9 2001

#define KICK_TO        __DIR__"road2"

inherit ROOM;

int do_sit(string arg);
int do_stand();
int do_order(string arg);
int do_eat(string arg);
int do_drink(string arg);
int do_pay();
int do_buy();
int do_quit();
string look_sign();
string look_table();
void quarter_pass();

mapping *food = ({
([        "name" : "����",
        "unit" : "��",
        "short" : "youtiao",
        "value" : 10,
        "time" : "��",
        "remaining" : 1,
        "supply" : 30
]),
([        "name" : "С����",
        "unit" : "��",
        "short" : "baozi",
        "value" : 20,
        "time" : "��",
        "remaining" : 2,
        "supply" : 30
]),
([        "name" : "�׷�",
        "unit" : "��",
        "short" : "mifan",
        "value" : 40,
        "time" : "��",
        "remaining" : 4,
        "supply" : 30
]),
([        "name" : "������˿",
        "unit" : "��",
        "short" : "rousi",
        "value" : 120,
        "time" : "��",
        "remaining" : 6,
        "supply" : 50
]),
([        "name" : "��������",
        "unit" : "��",
        "short" : "liyu",
        "value" : 200,
        "time" : "��",
        "remaining" : 9,
        "supply" : 50
]),
([        "name" : "�׷�",
        "unit" : "��",
        "short" : "mifan",
        "value" : 40,
        "time" : "��",
        "remaining" : 4,
        "supply" : 30
]),
([        "name" : "������˿",
        "unit" : "��",
        "short" : "rousi",
        "value" : 120,
        "time" : "��",
        "remaining" : 6,
        "supply" : 50
]),
([        "name" : "��������",
        "unit" : "��",
        "short" : "liyu",
        "value" : 200,
        "time" : "��",
        "remaining" : 9,
        "supply" : 50
]),
([        "name" : "��������",
        "unit" : "��",
        "short" : "zhouzi",
        "value" : 250,
        "time" : "��",
        "remaining" : 8,
        "supply" : 60
])
});

mapping *liquid = ({
([        "name" : "����",
        "unit" : "��",
        "short" : "miantang",
        "value" : 10,
        "time" : "��",
        "remaining" : 2,
        "supply" : 12
]),
([        "name" : "ϡ��",
        "unit" : "��",
        "short" : "xizhou",
        "value" : 16,
        "time" : "��",
        "remaining" : 2,
        "supply" : 18
]),
([        "name" : "ϡ��",
        "unit" : "��",
        "short" : "xizhou",
        "value" : 16,
        "time" : "��",
        "remaining" : 2,
        "supply" : 18
]),
([        "name" : "�׾�",
        "unit" : "̳",
        "short" : "mijiu",
        "value" : 130,
        "time" : "��",
        "remaining" : 9,
        "supply" : 40
]),
([        "name" : "������",
        "unit" : "��",
        "short" : "sanxiantang",
        "value" : 70,
        "time" : "��",
        "remaining" : 8,
        "supply" : 24
]),
([        "name" : "ϡ��",
        "unit" : "��",
        "short" : "xizhou",
        "value" : 16,
        "time" : "��",
        "remaining" : 2,
        "supply" : 18
]),
([        "name" : "�׾�",
        "unit" : "̳",
        "short" : "mijiu",
        "value" : 130,
        "time" : "��",
        "remaining" : 9,
        "supply" : 40
]),
([        "name" : "������",
        "unit" : "��",
        "short" : "sanxiantang",
        "value" : 70,
        "time" : "��",
        "remaining" : 8,
        "supply" : 24
]),
([        "name" : "���Ϻ�����",
        "unit" : "��",
        "short" : "hailuotang",
        "value" : 90,
        "time" : "��",
        "remaining" : 8,
        "supply" : 28
])
});

void create()
{
        set("short", "С�ƹ�");
        set("long", @LONG
����̫���ߵ�һ��С�ƹݡ���һ�߽��������ŵ�һ�ɾƲ˵���ζ��
����ʳ�ѡ����ڰ��ż��Ű�����(table)�� ��������ذڷ�����꣬��
�ܷ���Щ����(chair)�� һ���Ʊ�����æ����ͣ����Ҫ����������ƹ�
��ҥ�ԣ������Ǹ������ڡ���̨������һ����Ŀ��(sign)��
LONG
        );

        set("exits", ([
                "north" : KICK_TO,
        ]));

        set("item_desc", ([
                "sign" : (: look_sign :),
                "table" : (: look_table :),
                "chair" : "ľ�Ƶ����Σ�����������ܽ�ʵ��\n",
        ]) );

        set("objects",([
                __DIR__"npc/zhanggui" : 1,
                __DIR__"npc/jiubao" : 1,
        ]) ); 

        set("coor/x", 290);
        set("coor/y", -870);
        set("coor/z", 0);
        setup();
        quarter_pass();
}

void init()
{
        add_action("do_quit", "quit");
        add_action("do_buy", "buy");
        add_action("do_pay", "pay");
        add_action("do_pay", "jiezhang");
        add_action("do_eat", "eat");
        add_action("do_eat", "chi");
        add_action("do_drink", "drink");
        add_action("do_drink", "he");
        add_action("do_order", "order");
        add_action("do_order", "dian");
        add_action("do_stand", "stand");
        add_action("do_stand", "zhan");
        add_action("do_sit", "sit");
        add_action("do_sit", "zuo");
}

void kick_all()
{
        object *ob;
        int i, pay;

        set_temp("serve", "��");
        tell_room(this_object(), "�Ʊ�����һ����������������\n", ({}));
        ob = all_inventory(this_object());
        for(i = 0; i < sizeof(ob); i++)
        {
                if (userp(ob[i]))
                {
                        if( pay=query_temp("total_pay", ob[i]) )
                        {
                                message_vision("�Ʊ���$N�ȵ������������ˣ���\n", ob[i]);
                                if( !query_temp("to_pay", ob[i]) )
                                {
                                        message_vision("�Ʊ���$N����λ��ק��������������̨ǰ��\n", ob[i]);
                                        message_vision("�ƹ񷭳��˵���������һ�����̺����$N����һ��" + chinese_number(pay) + "��Ǯ����\n", ob[i]);
                                        delete_temp("marks/sit", ob[i]);
                                        set_temp("to_pay", 1, ob[i]);
                                }
                        }
                        else
                        {
                                if (!living(ob[i])) message_vision("�Ʊ���$N�ӳ��˾Ƶꡣ\n", ob[i]);
                                else message_vision("�Ʊ���$N�Ƴ��˾Ƶꡣ\n", ob[i]);
                                delete_temp("marks/sit", ob[i]);
                                delete_temp("cai_count", ob[i]);
                                delete_temp("cai_1", ob[i]);
                                delete_temp("cai_2", ob[i]);
                                delete_temp("cai_3", ob[i]);
                                delete_temp("cai_4", ob[i]);
                                delete_temp("cai_5", ob[i]);
                                ob[i]->move(KICK_TO);
                                if (!living(ob[i])) tell_room(KICK_TO, ob[i]->name() + "���ӳ��˾Ƶꡣ\n", ({ob[i]}));
                                else tell_room(KICK_TO, ob[i]->name() + "���Ƴ��˾Ƶꡣ\n", ({ob[i]}));
                        }
                }
        }
}

void quarter_pass()
{
//        mixed *local;
        object *ob;
        int i, paytime;
        string time = NATURE_D->game_time();
/*  ʱ�䱻����
        local = localtime(time() * 60);
        if (local[2] < 6) kick_all();
        else if (local[2] < 9) set_temp("serve", "��");
        else if (local[2] < 11) set_temp("serve", "��");
        else if (local[2] < 14) set_temp("serve", "��");
        else if (local[2] < 17) set_temp("serve", "��");
        else if (local[2] < 21) set_temp("serve", "��");
        else kick_all();
*/
        if( strsrch(time, "îʱ") >= 0) set_temp("serve", "��");
        else if( strsrch(time, "��ʱ") >= 0) set_temp("serve", "��");
        else if( strsrch(time, "��ʱ") >= 0) set_temp("serve", "��");
        else if( strsrch(time, "��ʱ") >= 0) set_temp("serve", "��");
        else if( strsrch(time, "δʱ") >= 0) set_temp("serve", "��");
        else if( strsrch(time, "��ʱ") >= 0) set_temp("serve", "��");
        else if( strsrch(time, "��ʱ") >= 0) set_temp("serve", "��");
        else kick_all();

        ob = all_inventory(this_object());
        for(i = 0; i < sizeof(ob); i++)
        {
                if( paytime=query_temp("to_pay", ob[i]) )
                {
                        paytime++;
                        if( paytime == 6 && query_temp("total_pay", ob[i])){
                                message_vision("�Ʊ���$N���һ��������ô�������ʣ�����԰����Ͱ�����\n", ob[i]);
                                message_vision("�Ʊ����������󺺣�ָ��$N˵�������Ҵ򣡡�\n", ob[i]);
                                message_vision("����Χס$N����ȭ���¡�\n", ob[i]);
                                delete_temp("total_pay", ob[i]);
                                delete_temp("to_pay", ob[i]);
                                delete_temp("cai_count", ob[i]);
                                delete_temp("cai_1", ob[i]);
                                delete_temp("cai_2", ob[i]);
                                delete_temp("cai_3", ob[i]);
                                delete_temp("cai_4", ob[i]);
                                delete_temp("cai_5", ob[i]);
                                ob[i]->unconcious();
                                message_vision("�Ʊ�ƲƲ�죬˵��������ȥ����\n", ob[i]);
                                message_vision("����̧��$N���ӳ��˵��š�\n", ob[i]);
                                ob[i]->move(KICK_TO);
                                tell_room(KICK_TO, "ֻ���ƹݴ���һ���������󺺽�һ�����Բ��ѵļһﶪ�˳�����\n", ({ob[i]}));
                        }
                        else set_temp("to_pay", paytime, ob[i]);
                }
        }
        call_out("quarter_pass", 30);
}

int do_sit(string arg)
{
        object me = this_player();

        if (me->is_busy() || me->is_fighting())
                return notify_fail("����æ���أ�\n");
        if (arg == "table" || arg == "zhuozi")
                return notify_fail("��Ҫ���������ϣ����Ȱ����������ٶ������ɡ�\n");        
        if ( !arg || (arg != "chair" && arg != "yizi") )
                return notify_fail("��Ҫ���ڵذ��ϣ��и��ԣ����������Ļ�������ᱻ����ƹ��ˡ�\n");        
        if( query_temp("marks/sit", me) )
                return notify_fail("���Ѿ�������λ���ˡ�\n");        
        if( query_temp("total_pay", me) )
                return notify_fail("�ȰѾ��ʽ����ˣ�\n");        
        remove_call_out("check_payed");
        message_vision("$N���˸���λ���£�׼���Է���\n", me);        
        if (query_temp("serve") == "��")
        {
                message_vision("�Ʊ��߹�����$N˵�����Բ������ģ����ڲ����ò�ʱ�䡣��\n", me);        
                message_vision("$N���ε���վ��������\n", me);        
                return 1;
        }
        set_temp("marks/sit", 1, me);
        return 1;
}

int do_stand()
{
        object me = this_player();

        if( !query_temp("marks/sit", me) )
                return notify_fail("���Ѿ�վ���ˣ��ǲ���վ�����е㷢�谡��\n");        
        if( query_temp("total_pay", me) )
                return notify_fail("û��Ǯ(pay)��������\n");        
        message_vision("$N����ƨ�ɣ�վ��������\n", me);        
        delete_temp("marks/sit", me);
        return 1;
}

int do_order(string arg)
{
        object me = this_player();
        string serve;
        int i, count;

        if (me->is_busy() || me->is_fighting())
                return notify_fail("����æ���أ�\n");
        if( !query_temp("marks/sit", me) )
                return notify_fail("�Ʊ��ú����Ƽ�һ������һ��������������㣺��վ�ųԺȣ���λ�Ӳ�������������ѽ����\n");
        if (!arg)
                return notify_fail("�Ʊ����ͷ���˵�����㾿��Ҫʲô������\n");
                        
        serve = query_temp("serve");
        if (serve == "��")
                return notify_fail("�Ʊ���Ц�������͹ټ��£������Ѿ�ͣ���ˣ���\n");
        if( !query_temp("total_pay", me))set_temp("total_pay", 0, me);
        if( !(count=query_temp("cai_count", me)) )
                set_temp("cai_count", 0, me);
        for (i = 0; i < sizeof(food); i++)
                if ((arg == food[i]["name"] || arg == food[i]["short"]) &&
                        serve == food[i]["time"])
                {
                        message_vision("$N��Ʊ�����������" + food[i]["unit"] + food[i]["name"] +"����\n", me);
                        if( query_temp("cai_count", me) >= 5 )
                        {
                                message_vision("�Ʊ���ֵ���$N���������㿪��ѽ�����Ȱ����ϵĳ�������˵����\n", me);
                                return notify_fail("");
                        }
                        message_vision("�Ʊ����ţ�����������������$N������" + food[i]["unit"] + food[i]["name"] +"��\n", me);
                        addn_temp("total_pay", food[i]["value"], me);
                        addn_temp("cai_count", 1, me);
                        set_temp("cai_"+count+"/name", food[i]["name"], me);
                        set_temp("cai_"+count+"/short", food[i]["short"], me);
                        set_temp("cai_"+count+"/unit", food[i]["unit"], me);
                        set_temp("cai_"+count+"/remaining", food[i]["remaining"], me);
                        set_temp("cai_"+count+"/supply", food[i]["supply"], me);
                        set_temp("cai_"+count+"/type", "food", me);
                        return 1;
                }
        for (i = 0; i < sizeof(liquid); i++)
                if ((arg == liquid[i]["name"] ||
                        arg == liquid[i]["short"]) &&
                        serve == liquid[i]["time"])
                {
                        message_vision("$N��Ʊ�����������" + liquid[i]["unit"] + liquid[i]["name"] +"����\n", me);
                        if( query_temp("cai_count", me) >= 5 )
                        {
                                message_vision("�Ʊ���ֵ���$N���������㿪��ѽ�����Ȱ����ϵĳ�������˵����\n", me);
                                return notify_fail("");
                        }
                        message_vision("�Ʊ����ţ�����������������$N������" + liquid[i]["unit"] + liquid[i]["name"] +"��\n", me);
                        addn_temp("total_pay", liquid[i]["value"], me);
                        addn_temp("cai_count", 1, me);
                        set_temp("cai_"+count+"/name", liquid[i]["name"], me);
                        set_temp("cai_"+count+"/short", liquid[i]["short"], me);
                        set_temp("cai_"+count+"/unit", liquid[i]["unit"], me);
                        set_temp("cai_"+count+"/remaining", liquid[i]["remaining"], me);
                        set_temp("cai_"+count+"/supply", liquid[i]["supply"], me);
                        set_temp("cai_"+count+"/type", "liquid", me);
                        return 1;
                }
        return notify_fail("�Ʊ����ͷ���˵����û�����ˣ���\n");
}

int do_eat(string arg)
{
        object me = this_player();
        int i, count, remaining;

        if (!arg) return 0;
                        
        if( !(count=query_temp("cai_count", me)))return 0;
        for (i = 0; i < count; i++)
                if( query_temp("cai_"+i+"/type", me) == "food" && 
                        (arg == query_temp("cai_"+i+"/name", me) || 
                        arg == query_temp("cai_"+i+"/short", me)) )
                        break;
        if (i == count) return 0;
        if( !query_temp("marks/sit", me) )
        {
                write( "�Ʊ��ú����Ƽ�һ������һ��������������㣺��վ�ųԺȣ���λ�Ӳ�������������ѽ����\n");
                return 1;
        }
        if (me->is_busy()) {
                write("����һ��������û����ɡ�\n");
                return 1;
        }
        if( query("food", me) >= me->max_food_capacity() )
        {
                write("���Ѿ���̫���ˣ���Ҳ�������κζ����ˡ�\n");
                return 1;
        }

        addn("food",query_temp("cai_"+i+"/supply",  me), me);
        if (me->is_fighting()) me->start_busy(2);

        message_vision("$N����һ"+query_temp("cai_"+i+"/unit", me)+query_temp("cai_"+i+"/name", me)+"���������ڡ�\n",me);
        remaining=query_temp("cai_"+i+"/remaining", me);
        remaining--;
        if (remaining == 0)
        {
                message_vision("$N��һ"+query_temp("cai_"+i+"/unit", me)+query_temp("cai_"+i+"/name", me)+"�Ե�һ�ɶ�����\n",me);
                for (; i < count - 1; i++)
                {
                        set_temp("cai_"+i+"/name",query_temp("cai_"+(i+1,  me)+"/name"), me);
                        set_temp("cai_"+i+"/short",query_temp("cai_"+(i+1,  me)+"/short"), me);
                        set_temp("cai_"+i+"/unit",query_temp("cai_"+(i+1,  me)+"/unit"), me);
                        set_temp("cai_"+i+"/remaining",query_temp("cai_"+(i+1,  me)+"/remaining"), me);
                        set_temp("cai_"+i+"/supply",query_temp("cai_"+(i+1,  me)+"/supply"), me);
                        set_temp("cai_"+i+"/type",query_temp("cai_"+(i+1,  me)+"/type"), me);
                }
                addn_temp("cai_count", -1, me);
        }
        else
                set_temp("cai_"+i+"/remaining", remaining, me);
        return 1;
}

int do_drink(string arg)
{
        object me = this_player();
        int i, count, remaining;

        if (!arg) return 0;
                        
        if( !(count=query_temp("cai_count", me)))return 0;
        for (i = 0; i < count; i++)
                if( query_temp("cai_"+i+"/type", me) == "liquid" && 
                        (arg == query_temp("cai_"+i+"/name", me) || 
                        arg == query_temp("cai_"+i+"/short", me)) )
                        break;
        if (i == count) return 0;
        if( !query_temp("marks/sit", me) )
        {
                write( "�Ʊ��ú����Ƽ�һ������һ��������������㣺��վ�ųԺȣ���λ�Ӳ�������������ѽ����\n");
                return 1;
        }
        if (me->is_busy())
        {
                write("����һ��������û����ɡ�\n");
                return 1;
        }
        if( query("water", me) >= me->max_water_capacity() )
        {
                write("���Ѿ���̫���ˣ���Ҳ�಻���κζ����ˡ�\n");
                return 1;
        }

        addn("water",query_temp("cai_"+i+"/supply",  me), me);
        if (me->is_fighting()) me->start_busy(2);

        message_vision("$N����һ"+query_temp("cai_"+i+"/unit", me)+query_temp("cai_"+i+"/name", me)+"���������ڡ�\n",me);
        remaining=query_temp("cai_"+i+"/remaining", me);
        remaining--;
        if (remaining == 0)
        {
                message_vision("$N��һ"+query_temp("cai_"+i+"/unit", me)+query_temp("cai_"+i+"/name", me)+"�ȵ�һ�ɶ�����\n",me);
                for (; i < count - 1; i++)
                {
                        set_temp("cai_"+i+"/name",query_temp("cai_"+(i+1,  me)+"/name"), me);
                        set_temp("cai_"+i+"/short",query_temp("cai_"+(i+1,  me)+"/short"), me);
                        set_temp("cai_"+i+"/unit",query_temp("cai_"+(i+1,  me)+"/unit"), me);
                        set_temp("cai_"+i+"/remaining",query_temp("cai_"+(i+1,  me)+"/remaining"), me);
                        set_temp("cai_"+i+"/supply",query_temp("cai_"+(i+1,  me)+"/supply"), me);
                        set_temp("cai_"+i+"/type",query_temp("cai_"+(i+1,  me)+"/type"), me);
                }
                addn_temp("cai_count", -1, me);
        }
        else set_temp("cai_"+i+"/remaining", remaining, me);
        return 1;
}

int do_pay()
{
        object me = this_player();
        int pay;

        if( !(pay=query_temp("total_pay", me)) )
                return notify_fail("�ƹ���Ц���������������������𣿲���Ҳ���ʣ���\n");
        if( query_temp("marks/sit", me) )
        {
                message_vision("$Nվ���������ߵ���̨ǰ���ʡ�\n", me);
                message_vision("�ƹ񷭳��˵���������һ�����̺����$N����һ��" + MONEY_D->price_str(pay) + "����\n", me);
                message_vision("�ƹ���žƱ��µ�������λ�͹������ϵ�ʣ����ʰ�ɾ���\n", me);
        }
        else
        {
                message_vision("$N���ƹ��ʵ�����������٣���\n", me);
                message_vision("�ƹ��˷����ۣ�˵����������" + MONEY_D->price_str(pay) + "����\n", me);
        }
        delete_temp("marks/sit", me);
        delete_temp("cai_count", me);
        set_temp("to_pay", 1, me);
        return 1;
}

int do_buy()
{
        write("�Ʊ�˵���������겻��������������(sit)���(order)����\n");
        return 1;
}

int do_quit()
{
        if( query_temp("total_pay", this_player()) )
        {
                write("�Ա������˲���Ǯ�����˳�����\n");
                return 1;
        }
        return 0;
}

string look_sign()
{
        string serve;
        int i;

        write("������д�ţ�\n");
        serve = query_temp("serve");
        if (serve == "��")
        {
                write("    ���ò�ʱ�䡣\n");
                return "";
        }
        write("    " + serve + "�͹�Ӧ���·�ʳ���������(order)��\n");
        for (i = 0; i < sizeof(food); i++)
                if (serve == food[i]["time"]) 
                        printf("    %30s    ÿ%s�ġ�\n", food[i]["name"] + "(" + food[i]["short"] + ")", food[i]["unit"] + chinese_number(food[i]["value"]));
        for (i = 0; i < sizeof(liquid); i++)
                if (serve == liquid[i]["time"]) 
                        printf("    %30s    ÿ%s�ġ�\n", liquid[i]["name"] + "(" + liquid[i]["short"] + ")", liquid[i]["unit"] + chinese_number(liquid[i]["value"]));
        return "";
}

string look_table()
{
        object me = this_player();
        int i, j, num, count;
        string name;

        if( !(count=query_temp("cai_count", me)) )
        {
                write("�������ǿյġ�\n");
                return "";
        }
        write("����ǰ�������Ϸ��ţ�\n");
        for (i = 0; i < count; i++)
        {
                name=query_temp("cai_"+i+"/name", me);
                num = 1;
                for (j = 0; j < count; j++)
                {
                        if (j == i) continue;
                        if( name == query_temp("cai_"+j+"/name", me) )
                        {
                                if (j < i)
                                {
                                        num = 0;
                                        break;
                                }
                                num++;
                        }
                }
                if (!num) continue;
                write(""+chinese_number(num)+query_temp("cai_"+i+"/unit", me)+name+"("+query_temp("cai_"+i+"/short", me)+")��\n");
        }
        return "";
}

int valid_leave(object me, string dir)
{
        if( query_temp("marks/sit", me) )
                return notify_fail("�����������Ҳ���ȥ��\n");        
        if( query_temp("total_pay", me) )
                return notify_fail("�Ʊ���ס��˵��������Ǯ������ұⰡ����\n");        
        delete_temp("cai_count", me);
        delete_temp("cai_1", me);
        delete_temp("cai_2", me);
        delete_temp("cai_3", me);
        delete_temp("cai_4", me);
        delete_temp("cai_5", me);
        return ::valid_leave(me, dir);
}