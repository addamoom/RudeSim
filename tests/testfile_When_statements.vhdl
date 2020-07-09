LIBRARY ieee ;
USE ieee.std_logic_1164.ALL ;

ENTITY testThing IS
    PORT(
        a:      IN  STD_LOGIC;
        b:      IN  STD_LOGIC;
        c:      OUT STD_LOGIC;
        d:      OUT STD_LOGIC;
        i:      OUT STD_LOGIC_VECTOR(5 DOWNTO 0)
        );
END ENTITY testThing ;

ARCHITECTURE test OF testThing IS
    SIGNAL e : STD_LOGIC ;
    SIGNAL f : STD_LOGIC ;
    SIGNAL g : STD_LOGIC := '1';
    SIGNAL h : STD_LOGIC_VECTOR(5 DOWNTO 0) := "000000";
    SIGNAL l : STD_LOGIC_VECTOR(5 DOWNTO 0);
    SIGNAL m : STD_LOGIC_VECTOR(5 DOWNTO 0);
    SIGNAL j : INTEGER := 4;
    SIGNAL k : INTEGER;
    SIGNAL n : INTEGER;

BEGIN
    e <= '0', '1' AFTER 2 ps;
    f <= '1';
    g <= '1' WHEN e < f ELSE '0' ;
    h <= "000111", "000000" AFTER 2 ps;
    l <= "000011";
    m <= "111111" WHEN h < l ELSE "010101" ;
    j <= 2, 3 AFTER 2 ps, 4 AFTER 3 ps;
    k <= 3;
    n <= 10 WHEN j < k ELSE 20 WHEN j = k ELSE 30 WHEN j > k ELSE 40 ;
    
END ARCHITECTURE test ;


