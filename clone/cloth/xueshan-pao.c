// xueshan-pao.c 
// Last Modified by winder on May. 25 2001

#include <armor.h>
inherit CLOTH;

void create()
{
        set_name("ѩɽ����", ({"xueshan baipao", "baipao", "pao", "cloth"}));
        set_weight(3000);
        if( clonep() )
                set_default_object(__FILE__);
        else {
                set("long","һ��˿�ư��ۣ���������һֻ�������ѵĴ�����\n");
                set("material", "cloth");
                set("unit", "��");
                set("value", 600);
                set("armor_prop/armor", 4);
                set("armor_prop/per", 1);
        }
        setup();
}

