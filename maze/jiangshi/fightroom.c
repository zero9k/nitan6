inherit ROOM; 

void create() 
{ 
        set("short", "߱����"); 
        set("long", @LONG 
������30��BOSS��ս����

LONG
); 
                        
        set("outdoors", "boss"); 
        set("no_magic",1);
        set("exits", ([ /* sizeof() == 3 */ 
                "out": "/d/city/wumiao",
        ])); 
        
        set("objects", ([
                __DIR__"npc/daozhang" : 1,
        ]));
        
        setup(); 
} 
