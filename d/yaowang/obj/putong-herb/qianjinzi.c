#include "herb.h"

void create()
{
        set_name(HIY "ǧ����" NOR, ({ "qian jinzi", "qian", "jinzi", "herb_qianjinzi" }));
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("long", HIY "ǧ�����ǳ�������ҩҩ�ġ�\n" NOR);
                set("base_unit", "��");
                set("base_value", 3000);
                set("base_weight", 20);
                set("no_sell", "ҩ���ȵĵ�ҩ�ǳ�����������ͨ��ҩ��ض��а���"); 
                set("yaowang", 1); 
        }
        setup();
}