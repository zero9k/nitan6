// foxiang.c

#include <ansi.h>

inherit ITEM;

void create()
{
        set_name(YEL"大佛像"NOR, ({ "dafo xiang", "xiang", "dafo" }) );
        set_weight(3000);
        set_max_encumbrance(5000);
        if( clonep() )
                set_default_object(__FILE__);
        else {
                set("unit", "尊");
                set("long", "一尊面目慈祥的如来佛像。好象是用黄金铸就的。\n");
                set("value", 1000);
                set("material", "gold");
                set("no_get",1);
                set("no_drop",1);
        }
        setup();
}

int is_container() { return 1; }

void init()
{
        object ob;
        object me = this_player();
        if( query("combat_exp", me) <= 5
         && query("amount", this_object()) >= 10
        &&  random(2) == 1 )
        {
                ob=new("/clone/money/silver");
                ob->set_amount(10);
                ob->move(this_object());
                addn("amount", -10, this_object());
                tell_object(me, "\n你忽然看到功德箱里有什麽东西在闪闪发光！\n\n");
                addn("combat_exp", 1, me);
        }
        add_action("do_put", "put");
}

int do_put(string arg)
{
        object me, obj;
        string item, target; 
        int amount;

        me = this_player();

        if(!arg) return notify_fail("你要将什么东西放进哪里？\n");

        if( sscanf(arg, "%s in %s", item, target)!=2 
        ||  sscanf(item, "%d %s", amount, item)!=2
        ||  !objectp(obj = present(item, me)) )

        return notify_fail("你要给谁什么东西？\n");

        if( query("money_id", obj) == "silver"
        &&  obj->query_amount() >= 5 && amount >= 5 ) 
        {
                if( query("begger", me)>0){
                        message_vision( sprintf(HIY "$N将一%s%s放进%s。\n" NOR,
                                query("unit", obj),obj->name(),
                                this_object()->name()),me );
                        obj->set_amount(obj->query_amount() - amount);;
                        addn("begger", (-1)*(amount/5), me);
                        if( query("begger", me)<0)set("begger", 0, me);
                        addn("amount", amount, this_object());
                        return 1;
                }
        }

        return 0;
}