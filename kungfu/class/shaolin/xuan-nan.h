// xuan-nan.h for fight and get letter from Xuan

#include <ansi.h>
#include "xuan.h"

string *names = ({
        "chengxin luohan",
        "chengyi luohan",
        "chengsi luohan",
        "chengshi luohan",
        "chengzhi luohan",
        "chengxin luohan",
        "chengling luohan",
        "chengyu luohan",
        "chengshang luohan",
});

string *checks = ({
        "finger",
        "claw",
        "hand",
        "strike",
        "cuff",
        "dodge",
        "parry",
        "force",
});

string ask_me()
{
        mapping fam, my_fam, skl; 
        object fighter, me, room, monk;
        string *sname;
        int i, j;

        fighter = this_player();
        me = this_object();
        my_fam=query("family", me);

        if (my_fam && my_fam["family_name"] != "������")
                return RANK_D->query_respect(fighter) +
                       "�ֲ������������µ��ˣ������޺�����ʲô��";

        skl = fighter->query_skills();
        if (sizeof(skl) <= 1)
                return RANK_D->query_respect(fighter) + 
                "���������������ʸ��޺�����";
        sname  = keys(skl);

        for (i = 0; i < sizeof(skl); i++)
        {
                if (member_array(sname[i], checks) != -1 &&
                    skl[sname[i]] < 80) 
                        return RANK_D->query_respect(fighter) + 
                               "���������������ʸ��޺�����";
        }

        if( query("luohan_winner", fighter) )
                return RANK_D->query_respect(fighter) + 
                "��Ȼ�����޺����󣬿ɲ�Ҫ�����Ŀ������Ц��";

        if( mapp(fam=query("family", fighter)) && 
            fam["family_name"] == "������" &&
            fam["generation"] > (my_fam["generation"] + 2))
                return RANK_D->query_respect(fighter) + 
                "���ݲ��ϣ������ʸ��޺�����";

        if( query("guilty", fighter)>0 )
                return RANK_D->query_respect(fighter) + 
                "���۷����䣬��������������׼���㴳�޺�����";

        if( query("assigned_fighter", me) )
                return RANK_D->query_respect(fighter) +  
                "��������������ս�޺��������һ��ʱ�������ɡ�"; 

        set_temp("xuannan-asked", 1, fighter);

        say("\n����˵�����ðɣ������ټ������ó�����������"
            "�䳡���ϣ����Ժ����������䳡�ϵ��㡣\n");

        set("assigned_fighter",query("id",  fighter), me);
        
        if( !query_temp("xuanku-asked", fighter) )
        say("\n�����ֵ���������ȥ��֪�޺��������ʦ�����������м�"
            "���������޺��������䳡���ϡ�\n");

        message_vision("\n���Ѵ�ʦ�����뿪��\n\n", fighter);

        for (j = 1; j < 10; j++) 
        {
                if (! (room = find_object("/d/shaolin/banruo" + j)))
                        room = load_object("/d/shaolin/banruo" + j);
                
                me->move(room);
                
                if (! objectp(monk = present(names[j-1], room)))
                {
                        me->move("/d/shaolin/luohan5");
                        delete("assigned_fighter", me);
                        delete_temp("xuannan-asked", fighter);
                        message_vision("\n���Ѵ�ʦ���˹�����\n\n", fighter);
                        return "���ǶԲ��𣬰����������˲��ڣ��޷���"
                               "���޺�����\n";        
                }

                message("vision", "\n���Ѵ�ʦ���˹�������" +
                        query("name", monk)+"��������˵�˼��䡣\n",
                        room, monk);

                monk->move("/d/shaolin/wuchang1");
                message("vision",query("name", monk)+
                        "�޺����˵�ͷ���첽���˳�ȥ��\n", room, monk);
        }
        
        me->move("/d/shaolin/wuchang");

        call_out("waiting", 1, me);

        return "�ã���Ҷ������ˡ�\n";
}

int waiting(object me)
{
        object fighter;
        int wait_time;

        if (wait_time == 300)
        {
                say( "����˵�����������������ˣ����ǻ�ȥ�գ�\n\n");
                call_out("do_back", 0, me);                
        }

        if( !objectp(fighter=present(query("assigned_fighter", me),environment(me))) )
        {
                wait_time++;
                call_out("waiting", 1, me);
        } else
        if( !present("xuanku dashi",environment(me)) || !query_temp("xuanku-asked", fighter) )
        {
                if( random(10) == 0 ) 
                say("\n����˵����" + RANK_D->query_respect(fighter) + "ȥ���������ʦû�У� ���������ڸõ��˰գ�\n");

                wait_time++;
                call_out("waiting", 1, me);
        }
        else         call_out("preparing", 0, me, fighter);

        return 1;
}

int preparing(object me, object fighter)
{
        object monk, room, ob;

        if (! (room = find_object("/d/shaolin/wuchang")))
                room = load_object("/d/shaolin/wuchang");

        say("����˵��������ɵ��ˣ��粻���ֳ���Ħ�޺������뼴���뿪�˴���\n");

        message("vision", HIY "\n���Ѵ�ʦ�������������޺������Χ��\n\n" NOR,
                room);
        command("chat"+query("title", fighter)+query("name", fighter)+
                "�ڽ���" + NATURE_D->game_time() + "��ս����ʮ���޺�����");

        delete_temp("xuannan-asked", fighter);
        delete_temp("beat_count", fighter);
        set_temp("fighting", 1, fighter);

        call_out("fighting",  10, me, fighter, 0);

        return 1;
}

int fighting(object me, object fighter, int count)
{
        object monk1, monk2, room1, room2;

        if (! (room1 = find_object("/d/shaolin/wuchang1")))
                room1 = load_object("/d/shaolin/wuchang1");

        if (! (room2 = find_object("/d/shaolin/wuchang")))
                room2 = load_object("/d/shaolin/wuchang");

        if (count <= 8)
        {
                message("vision", "\n�޺������ٵ���ת�ţ�һ���������ս���\n" NOR, room2);

                monk1 = present(names[count], room1);
                if (objectp(monk1))
                        monk1->move(room2);
        }
                
        if (count >= 1 && count <= 9)
        {
                monk2 = present(names[count-1], room2);
                if (objectp(monk2))
                        monk2->move(room1);
        }

        if( !find_player(query("assigned_fighter", me)) || 
            !living(fighter) || query("qi", fighter) <= 1 )
        {
                if( objectp(present(query("assigned_fighter", me),environment(me))) )
                {
                        delete_temp("fighting", fighter);
                        delete_temp("beat_count", fighter);
                }
                if (objectp(monk1))
                        monk1->move(room1);

                say("����˵�����ǰ�����ϧѽ ... ����\n\n");
                command("sigh");
                call_out("do_back", 5, me );
        } else
        if (count >= 9)
        {
                if( query_temp("beat_count", fighter) >= 18 )
                        call_out("do_recruit", 5, me, fighter);
                else
                        call_out("do_back", 5, me );
        } else
        {
                count++;
                call_out("fighting", 2, me, fighter, count);
        }
 
        return 1;
}

int do_recruit(object me, object fighter)
{
        mapping ft_fam, my_fam;

        delete_temp("beat_count", fighter);
        delete_temp("fighting", fighter);
        addn("combat_exp", 7500, fighter);
        set("luohan_winner", 1, fighter);

        if( !(ft_fam=query("family", fighter)) || ft_fam["family_name"] != "������" )
        {
                command("smile");
                say("����˵����" + RANK_D->query_respect(fighter) + "�书׿���������������飬Ʒ�¸��ǹ���һ����Ĳ�ʤ���֮����\n");
                call_out("do_back", 5, me );
        }
        else
        {
                say("\n����˵�����ã��ã��ã�"+query("name", fighter)+"����Ը����ҵ�Ϊʦ��\n");
                call_out("do_back", 30, me );
        }

        return 1;
}

int do_back(object me)
{
        object monk, room1, room2;
        int i;

        message("vision", "\n���Ѵ�ʦҲ���˻��֣� �����ó���"
                "���漴Ҳ�뿪���䳡��\n", environment(me));
        
        if (! (room1 = find_object("/d/shaolin/wuchang")))
                room1 = load_object("/d/shaolin/wuchang");

        set("exits/south", "/d/shaolin/houdian", room1);
        set("exits/north", "/d/shaolin/fzlou", room1);
        set("exits/east", "/d/shaolin/wuchang2", room1);
        set("exits/west", "/d/shaolin/wuchang1", room1);

        if (! (room1 = find_object("/d/shaolin/wuchang1")))
                room1 = load_object("/d/shaolin/wuchang1");

        for (i = 1; i < 10; i++)
        {
                if (! (room2 = find_object("/d/shaolin/banruo" + i)))
                        room2 = load_object("/d/shaolin/banruo" + i);

                if (objectp(monk = present(names[i-1], room1)))
                        monk->move(room2);
        }

        delete("assigned_fighter", me);
        me->move("/d/shaolin/banruo5");

        return 1;
}