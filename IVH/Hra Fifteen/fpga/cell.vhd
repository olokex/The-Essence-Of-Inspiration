-- Author: xsedla1e
-- Year 2018

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
USE work.game_pack.ALL; -- reseni prvniho ukolu - bude pouzito spravne reseni
use ieee.std_logic_unsigned.all;

entity cell is -- v definici entity neni dovoleno provadet zadne zmeny
   GENERIC (
      MASK           : mask_t := (others => '1') -- mask_t definovano v baliku game_pack
   );
   Port ( 
      STATE     	 : out  STD_LOGIC_VECTOR(3 downto 0);
      INIT_STATE	 : in   STD_LOGIC_VECTOR(3 downto 0);
      KEYS           : in   STD_LOGIC_VECTOR(3 downto 0);
      NEIG_TOP		 : in   STD_LOGIC_VECTOR(3 downto 0);
      NEIG_LEFT      : in   STD_LOGIC_VECTOR(3 downto 0);
      NEIG_RIGHT     : in   STD_LOGIC_VECTOR(3 downto 0);
      NEIG_BOTTOM    : in   STD_LOGIC_VECTOR(3 downto 0);
      CLK            : in   STD_LOGIC;
      RESET          : in   STD_LOGIC
   );
end cell;

architecture Behavioral of cell is
  constant IDX_TOP    : NATURAL := 0; -- index klavesy nahoru
  constant IDX_LEFT   : NATURAL := 1; -- ... totez        ...                vlevo
  constant IDX_RIGHT  : NATURAL := 2; -- ... totez        ...                vpravo
  constant IDX_BOTTOM : NATURAL := 3; -- ... totez        ...                dole
  signal CUR_STATE : STD_LOGIC_VECTOR(3 downto 0);
	
begin
	STATE <= CUR_STATE;
	process (CLK, RESET)
	begin
		if (rising_edge(CLK)) then
			if (RESET = '1') then
				CUR_STATE <= INIT_STATE;
			elsif (CUR_STATE = "0000") then
				if ((KEYS(IDX_TOP) = '1') and (MASK.bottom = '1'))then
					CUR_STATE <= NEIG_BOTTOM;
				elsif ((KEYS(IDX_BOTTOM) = '1') and (MASK.top = '1')) then
					CUR_STATE <= NEIG_TOP;
				elsif ((KEYS(IDX_LEFT) = '1') and (MASK.right = '1')) then
					CUR_STATE <= NEIG_RIGHT;
				elsif ((KEYS(IDX_RIGHT) = '1') and (MASK.left = '1')) then
					CUR_STATE <= NEIG_LEFT;
				end if;
			else
				if ((KEYS(IDX_BOTTOM) = '1') and (NEIG_BOTTOM = "0000") and (MASK.bottom = '1')) then
					CUR_STATE <= NEIG_BOTTOM;
				elsif ((KEYS(IDX_TOP) = '1') and (NEIG_TOP = "0000") and (MASK.top = '1')) then
					CUR_STATE <= NEIG_TOP;
				elsif ((KEYS(IDX_LEFT) = '1') and (NEIG_LEFT = "0000") and (MASK.left = '1')) then
					CUR_STATE <= NEIG_LEFT;
				elsif ((KEYS(IDX_RIGHT) = '1') and (NEIG_RIGHT = "0000") and (MASK.right = '1')) then
					CUR_STATE <= NEIG_RIGHT;
				end if;
			end if;
		end if;
	end process;
end Behavioral;

