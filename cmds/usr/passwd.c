// passwd.c

#include <getconfig.h>
#include <ansi.h>

inherit F_CLEAN_UP;


int main(object me, string arg)
{
        object ob;

        if (me != this_player(1)) return 0;

        if (arg == "?")
        {
                write("���⹦�ܣ�SHUTDOWN��ADMIN��\n");
                return 1;
        }

        if (stringp(arg))
        {
                if (! SECURITY_D->valid_grant(me, "(admin)"))
                {
                        write("��û��Ȩ���޸ı��˵Ŀ��\n");
                        return 1;
                }

                if (wiz_level(me) < wiz_level(arg))
                {
                        write("��û��Ȩ���޸�����˵Ŀ��\n");
                        return 1;
                }

                seteuid(getuid());
                ob = find_player(arg);
                if (! ob)
                {
                        ob = new(LOGIN_OB);
                        set("id", arg, ob);
                        if (! ob->restore())
                        {
                                destruct(ob);
                                return notify_fail("û�������ҡ�\n");
                        }
                        set_temp("create_temp", 1, ob);
                } else
                {
                        ob=query_temp("link_ob", ob);
                        while (ob && ob->is_character())
                                ob=query_temp("link_ob", ob);
                        if (! ob)
                        {
                                ob = new(LOGIN_OB);
                                set("id", arg, ob);
                                if (! ob->restore())
                                {
                                        destruct(ob);
                                        return notify_fail("�������ȱ��������Ϣ��������LOGIN��\n");
                                }
                                set_temp("create_temp", 1, ob);
                        }
                }

                write("������("+query("id", ob)+")���¹������룺");
                input_to("get_new_ad_pass", 1, ob);
                return 1;
        }

        ob=query_temp("link_ob", me);
        if (! ob)
                return notify_fail("�������ȱ��������Ϣ��������LOGIN��\n");

        while(ob && ob->is_character())ob=query_temp("link_ob", ob);

        write("Ϊ�˰�ȫ����������������������룺");
        input_to("get_old_pass", 1, ob);
        return 1;
}

protected void get_old_pass(string pass, object ob)
{
        string old_pass;

        if (! objectp(ob))
        {
                write("�޷��ҵ����Ӷ��󣬴˴β�����ֹ�ˡ�\n");
                return;
        }

        write("\n");
        old_pass=query("ad_password", ob);
        if (! stringp(old_pass) || crypt(pass, old_pass) != old_pass)
        {
                write(HIR "���������ע�⣺����Ҫ������ǹ������롣\n" NOR);
                return;
        }
        write("��ѡ������һ��������\n"
              "1. �޸Ĺ�������\n"
              "2. �޸���ͨ����\n"
              "3. ���޸ġ�\n"
              "��ѡ��(����㲻�����������֣���������select1��select2)��");
        
        input_to("select_fun", ob);
}

protected void select_fun(string fun, object ob)
{
        if (! objectp(ob))
        {
                write("�޷��ҵ����Ӷ��󣬴˴β�����ֹ�ˡ�\n");
                return;
        }

        switch (fun)
        {
        case "1":
        case "select1":
                write("���������µĹ������룺");
                input_to("get_new_ad_pass", 1, ob);
                return;

        case "2":
        case "select2":
                write("���������µ���ͨ���룺");
                input_to("get_new_pass", 1, ob);
                return;

        case "":
        case "3":
                write("������ϡ�\n");
                return;

        default:
                write("û������ܡ�\n");
                return;
        }
}


string trans_char(int c)
{
        return sprintf("%c ", c);
}

protected void get_new_pass(string pass, object ob)
{
        string old_pass;

        if (! objectp(ob))
        {
                write("�޷��ҵ����Ӷ��󣬴˴β�����ֹ�ˡ�\n");
                return;
        }

        if (pass == "")
        {
                write("����ȡ���ˡ�\n");
                return;
        }

        if (strlen(pass) < 3)
        {
                write("�Բ��������ͨ���볤�ȱ��������λ�����������룺");
                input_to("get_new_pass", 1, ob);
                return;
        }

        old_pass=query("ad_password", ob);
        if (stringp(old_pass) && crypt(pass, old_pass) == old_pass)
        {
                write(HIR "\nΪ�˰�ȫ�������ͨ����͹������벻��һ����\n\n" NOR);
                write("���������������ͨ���룺");
                input_to("get_new_pass", 1, ob);
                return;
        }

        write("\n��������һ���µ���ͨ���룺");
        input_to("confirm_new_pass", 1, ob, crypt(pass, 0));
}

protected void confirm_new_pass(string pass, object ob, string new_pass)
{
        object me;
        object body;
        string email;
        string msg;

        if (! objectp(ob))
        {
                write("�޷��ҵ����Ӷ��󣬴˴β�����ֹ�ˡ�\n");
                return;
        }

        write("\n");
        if (crypt(pass, new_pass) != new_pass)
        {
                write("�Բ�������������Ĳ�����ͬ�����������������ͨ���룺");
                input_to("get_new_pass", 1, ob);
                return;
        }

        seteuid(getuid());
        if( !set("password",new_pass,ob) )
        {
                write("��ͨ������ʧ�ܣ�\n");
                return;
        }       
#ifdef DB_SAVE 
        if (! DATABASE_D->query_db_status())
                DATABASE_D->connect_to_database();   

        DATABASE_D->db_set_player(query("id", ob),"password",pass);
#endif          
        ob->save();
        me = this_player();
        log_file("static/passwd", sprintf("%s %s's normal passwd changed by %s(%s)\n",
                                          log_time(),
                                          query("id", ob),
                                          geteuid(me),
                                          interactive(me) ? query_ip_name(me)
                                                          : 0,
                                          ctime(time())));

        write("��ͨ�������ɹ���\n");
}

protected void get_new_ad_pass(string pass, object ob)
{
        string old_pass;

        if (! objectp(ob))
        {
                write("�޷��ҵ����Ӷ��󣬴˴β�����ֹ�ˡ�\n");
                return;
        }

        if (pass == "")
        {
                write("����ȡ���ˡ�\n");
                return;
        }

        if (strlen(pass) < 5)
        {
                write(HIR "\n�Բ���Ϊ�˰�ȫ����������ͨ���볤�ȱ��������λ��\n\n" NOR);
                write("�����������µĹ������룺");
                input_to("get_new_ad_pass", 1, ob);
                return;
        }

        old_pass=query("password", ob);
        if (stringp(old_pass) && crypt(pass, old_pass) == old_pass)
        {
                write(HIR "\nΪ�˰�ȫ����������������ͨ���벻��һ����\n\n" NOR);
                write("������������Ĺ������룺");
                input_to("get_new_ad_pass", 1, ob);
                return;
        }

        write("\n��������һ���µĹ������룺");
        input_to("confirm_new_ad_pass", 1, ob, crypt(pass, 0));
}


protected void confirm_new_ad_pass(string pass, object ob, string new_pass)
{
        object me;
        object body;
        string email;
        string msg;
        string passwd;

        if (! objectp(ob))
        {
                write("�޷��ҵ����Ӷ��󣬴˴β�����ֹ�ˡ�\n");
                return;
        }

        write("\n");
        if (crypt(pass, new_pass) != new_pass)
        {
                write("�Բ�������������Ĳ�����ͬ��������������Ĺ������룺");
                input_to("get_new_ad_pass", 1, ob);
                return;
        }

        seteuid(getuid());
        if( !set("ad_password",new_pass,ob) )
        {
                write("����������ʧ�ܣ�\n");
                return;
        }
#ifdef DB_SAVE 
        if (! DATABASE_D->query_db_status())
                DATABASE_D->connect_to_database();  
                
        DATABASE_D->db_set_player(query("id", ob),"ad_password",pass);
#endif        
        ob->save();
        me = this_player();
        log_file("static/passwd", sprintf("%s %s's super passwd changed by %s(%s)\n",
                                          log_time(),
                                          query("id", ob),
                                          geteuid(me),
                                          interactive(me) ? query_ip_name(me)
                                                          : 0,
                                          ctime(time())));

        // ���Ҳ�����mail
        if( geteuid(me) == query("id", ob) )
        {
                // �Ǳ������޸�
                write("�����������ɹ���\n");
                return;
        } else
        {
                // ���������޸�
                set("password", "55AA", ob);
                write("����û�ԭ�е���ͨ���롣\n");
                ob->save();
        }

        body = LOGIN_D->make_body(ob);
        {
                // ����mail
                body->restore();
                email=query("email", body);
                destruct(body);
                msg = @LONG
Hello, %id.

��л������������Ϸ%MUD_NAME������ʹ�õ��˺�(%id)�Ĺ���������
�Ѿ���%me�޸ĳ�Ϊ%passwd���´ε�¼��ʱ������ʹ���µĹ�������
��¼�����������õ�¼ʹ�õ���ͨ���롣�Դ���ɵĲ��㾴��ԭ�¡�

                             %data
LONG ;
                msg=replace_string(msg,"%id",query("id", ob));
                msg = replace_string(msg, "%MUD_NAME", LOCAL_MUD_NAME());
                msg = replace_string(msg, "%me", me->name(1) + "(" + geteuid(me) + ")");
                msg = replace_string(msg, "%passwd", pass);
                msg = replace_string(msg, "%data", ctime(time()));
                //MAIL_D->queue_mail(me, 0, email, "Password chanaged", msg);
        }

        write("��ɹ����޸����û�("+query("id", ob)+")�Ĺ������롣\n");

        if( query_temp("create_temp", ob) )
                destruct(ob);
}

int help(object me)
{
        write(@HELP
ָ���ʽ : passwd <���>
 
���ָ������޸�����������롣�������ʦ������ʹ�����������
�޸����˵Ĺ������룬�޸��Ժ�ϵͳ���Զ����ŵ������ע������ͨ
֪�µĹ������롣

HELP );
        return 1;
}