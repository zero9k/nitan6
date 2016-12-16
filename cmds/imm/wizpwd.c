// Copyright (C) 2003, by Lonely. All rights reserved.
// This software can not be used, copied, or modified 
// in any form without the written permission from authors.
// wizpwd.c

inherit F_CLEAN_UP;

int main(object me, string arg)
{
        object ob;
        
        if (! SECURITY_D->valid_grant(me, "(immortal)")) 
                return 0; 

        if( me != this_player(1) ) return 0;

        ob=query_temp("link_ob", me);
        if( !ob ) return 0;
        while(ob && ob->is_character())ob=query_temp("link_ob", ob);

        write("Ϊ�˰�ȫ���������������ԭ�������룺");
        input_to("get_old_pass", 1, ob);
        return 1;
}

protected void get_old_pass(string pass, object ob)
{
        string old_pass;

        write("\n");
        old_pass=query("wizpwd", ob);
        if( query("wizpwd", ob) )
        {
            if(crypt(pass, old_pass)!=old_pass ) {
                write("�������\n");
                return;
            }
        }
        write("�������µ����룺");
        input_to("get_new_pass", 1, ob );
}

protected void get_new_pass(string pass, object ob)
{
        write("\n��������һ���µ����룺");
        input_to("confirm_new_pass", 1, ob, crypt(pass,0));
}

protected void confirm_new_pass(string pass, object ob, string new_pass)
{
        write("\n");
        if( crypt(pass, new_pass)!=new_pass ) {
                write("�Բ�������������벢����ͬ������ʹ��ԭ�������롣\n");
                return;
        }
        seteuid(getuid());
        if( !set("wizpwd",new_pass,ob) ){
                write("������ʧ�ܣ�\n");
                return;
        }
         
        set("wizpwd", new_pass, ob);
        ob->save();
        write("�������ɹ���\n");
}

int help(object me)
{
        write(@HELP
ָ���ʽ : passwd
 
���ָ������޸�wiz�ĵڶ������롣
 
HELP
        );
        return 1;
}