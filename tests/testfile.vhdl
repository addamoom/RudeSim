LIBRARY ieee ;
USE ieee.std_logic_1164.ALL ;

ENTITY testThing IS
    PORT(
        a:      IN  STD_LOGIC;
        b:      IN  STD_LOGIC;
        c:      OUT STD_LOGIC;
        d:      OUT STD_LOGIC
        );
END ENTITY testThing ;

ARCHITECTURE test OF testThing IS
    SIGNAL e : STD_LOGIC ;
    SIGNAL f : STD_LOGIC ;
BEGIN
    f <= '1' ;
    c <= f ;
    e <= '0' ;
    d <= NOT e;
END ARCHITECTURE test ;