// mapo-doufu.c ���Ŷ���
// Last Modified by Sir on May. 22 2001

inherit ITEM;
inherit F_FOOD;

void create()
{
        set_name("���Ŷ���", ({"mapo doufu", "doufu"}));
        set_weight(200);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("long", "һ����ɫ������ϸ�д��ۣ������ɿڵ����Ŷ�����\n");
                set("unit", "��");
                set("value", 150);
                set("food_remaining", 4);
                set("food_supply", 40);
        }
}