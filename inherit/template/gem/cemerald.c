// $file_name.c                                                 /* COMMENT_ENTRY */
// Create by TEMPLATE_D. $time.                                 /* COMMENT_ENTRY */
// skull.c
// Written by Lonely                                            /* COMMENT_EXIT  */

#include <ansi.h>

inherit TESSERA;

void create()
{
//**    set_name("$name", ({ "$id", }));                        /* NAME_ENTRY */
        set_name(HIG "ϸ��������" NOR, ({ "chipped emerald" }) ); /* NAME_EXIT  */
        set_weight(20);
        if( clonep() )
                set_default_object(__FILE__);
        else {
//**            set("$dbase_key", "$dbase_value\n");            /* DBASE_ENTRY */
                set("long", HIG "һƬ��Ө��͸���������Ƭ��\n" NOR); /* DBASE_EXIT  */
                set("value", 20000);
                set("unit", "��");
                set("level", 1);
                set("material", "cemerald");
                set("can_be_enchased", 1);
                set("magic/type", "wood");
                set("magic/power", 70);
                set("auto_load", 1);
        }
        set("enchase/weapon_prop/add_wood", 1);
        set("enchase/rings_prop/add_wood", 1);
        set("enchase/armor_prop/reduce_earth", 1);
        setup();
}

