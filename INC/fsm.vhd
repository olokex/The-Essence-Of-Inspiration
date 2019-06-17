-- fsm.vhd: Finite State Machine
-- Author(s): xsedla1e
--
library ieee;
use ieee.std_logic_1164.all;
-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity fsm is
port(
   CLK         : in  std_logic;
   RESET       : in  std_logic;

   -- Input signals
   KEY         : in  std_logic_vector(15 downto 0);
   CNT_OF      : in  std_logic;

   -- Output signals
   FSM_CNT_CE  : out std_logic;
   FSM_MX_MEM  : out std_logic;
   FSM_MX_LCD  : out std_logic;
   FSM_LCD_WR  : out std_logic;
   FSM_LCD_CLR : out std_logic
);
end entity fsm;

-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of fsm is
   type t_state is (
		-- KOD1: 1752705731
		-- KOD2: 17521645848
		STAV1, STAV2, STAV3, STAV4, STAV5, 								-- protoze prvni ctyri stavy jsou shodne
		STAV1A, STAV2A, STAV3A, STAV4A, STAV5A,			 			-- pokracovani pro KOD1
		STAV1B, STAV2B, STAV3B, STAV4B, STAV5B, STAV6B,			   -- pokracovani pro KOD2
		MSG_ERR, MSG_OK, ENTER, FAIL, FINISH							-- stavy pro vypis
	);
   signal present_state, next_state : t_state;

begin
-- -------------------------------------------------------
sync_logic : process(RESET, CLK)
begin
   if (RESET = '1') then
      present_state <= STAV1;
   elsif (CLK'event AND CLK = '1') then
      present_state <= next_state;
   end if;
end process sync_logic;

-- -------------------------------------------------------
next_state_logic : process(present_state, KEY, CNT_OF)
begin
   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when STAV1 =>
      next_state <= STAV1;
      if (KEY(15) = '1') then
         next_state <= MSG_ERR;
		elsif (KEY(1) = '1') THEN
			next_state <= STAV2;
		elsif(KEY(14 downto 0) /= "000000000000000") then
			next_state <= FAIL;
      end if;
		
	when STAV2 =>
		next_state <= STAV2;
		if (KEY(15) = '1') then
			next_state <= MSG_ERR;
		elsif (KEY(7) = '1') then
			next_state <= STAV3;
		elsif(KEY(14 downto 0) /= "000000000000000") then
			next_state <= FAIL;
		end if;
		
	when STAV3 =>
		next_state <= STAV3;
		if (KEY(15) = '1') then
			next_state <= MSG_ERR;
		elsif (KEY(5) = '1') then
			next_state <= STAV4;
		elsif(KEY(14 downto 0) /= "000000000000000") then
			next_state <= FAIL;
		end if;
		
	when STAV4 =>
		next_state <= STAV4;
		if (KEY(15) = '1') then
			next_state <= MSG_ERR;
		elsif (KEY(2) = '1') then
			next_state <= STAV5;
		elsif(KEY(14 downto 0) /= "000000000000000") then
			next_state <= FAIL;
		end if;
		
	when STAV5 =>
		next_state <= STAV5;
		if (KEY(15) = '1') then
			next_state <= MSG_ERR;
		elsif (KEY(7) = '1') then
			next_state <= STAV1A;
		elsif (KEY(1) = '1') then
			next_state <= STAV1B;
		elsif(KEY(14 downto 0) /= "000000000000000") then
			next_state <= FAIL;
		end if;
		
	when STAV1A =>
		next_state <= STAV1A;
		if (KEY(15) = '1') then
			next_state <= MSG_ERR;
		elsif (KEY(0) = '1') then
			next_state <= STAV2A;
		elsif(KEY(14 downto 0) /= "000000000000000") then
			next_state <= FAIL;
		end if;

	when STAV2A =>
		next_state <= STAV2A;
		if (KEY(15) = '1') then
			next_state <= MSG_ERR;
		elsif (KEY(5) = '1') then
			next_state <= STAV3A;
		elsif(KEY(14 downto 0) /= "000000000000000") then
			next_state <= FAIL;
		end if;

	when STAV3A =>
		next_state <= STAV3A;
		if (KEY(15) = '1') then
			next_state <= MSG_ERR;
		elsif (KEY(7) = '1') then
			next_state <= STAV4A;
		elsif(KEY(14 downto 0) /= "000000000000000") then
			next_state <= FAIL;
		end if;

	when STAV4A =>
		next_state <= STAV4A;
		if (KEY(15) = '1') then
			next_state <= MSG_ERR;
		elsif (KEY(3) = '1') then
			next_state <= STAV5A;
		elsif(KEY(14 downto 0) /= "000000000000000") then
			next_state <= FAIL;
		end if;

	when STAV5A =>
		next_state <= STAV5A;
		if (KEY(15) = '1') then
			next_state <= MSG_ERR;
		elsif (KEY(1) = '1') then
			next_state <= ENTER;
		elsif(KEY(14 downto 0) /= "000000000000000") then
			next_state <= FAIL; 
		end if;
	
	-- konec KOD1
		
	when STAV1B =>
		next_state <= STAV1B;
		if (KEY(15) = '1') then
			next_state <= MSG_ERR;
		elsif (KEY(6) = '1') then
			next_state <= STAV2B;
		elsif(KEY(14 downto 0) /= "000000000000000") then
			next_state <= FAIL;
		end if;

	when STAV2B =>
		next_state <= STAV2B;
		if (KEY(15) = '1') then
			next_state <= MSG_ERR;
		elsif (KEY(4) = '1') then
			next_state <= STAV3B;
		elsif(KEY(14 downto 0) /= "000000000000000") then
			next_state <= FAIL;
		end if;

	when STAV3B =>
		next_state <= STAV3B;
		if (KEY(15) = '1') then
			next_state <= MSG_ERR;
		elsif (KEY(5) = '1') then
			next_state <= STAV4B;
		elsif(KEY(14 downto 0) /= "000000000000000") then
			next_state <= FAIL;
		end if;

	when STAV4B =>
		next_state <= STAV4B;
		if (KEY(15) = '1') then
			next_state <= MSG_ERR;
		elsif (KEY(8) = '1') then
			next_state <= STAV5B;
		elsif(KEY(14 downto 0) /= "000000000000000") then
			next_state <= FAIL;
		end if;

	when STAV5B =>
		next_state <= STAV5B;
		if (KEY(15) = '1') then
			next_state <= MSG_ERR;
		elsif (KEY(4) = '1') then
			next_state <= STAV6B;
		elsif(KEY(14 downto 0) /= "000000000000000") then
			next_state <= FAIL;
		end if;

	when STAV6B =>
		next_state <= STAV6B;
		if (KEY(15) = '1') then
			next_state <= MSG_ERR;
		elsif (KEY(8) = '1') then
			next_state <= ENTER;
		elsif(KEY(14 downto 0) /= "000000000000000") then
			next_state <= FAIL;
		end if;
		
	when FAIL =>
		next_state <= FAIL;
		if (KEY(14 downto 0) /= "000000000000000") then
			next_state <= FAIL;
		elsif (KEY(15) = '1') then
			next_state <= MSG_ERR;
		end if;
		
	-- konec KOD2
	
	when ENTER =>
		next_state <= ENTER;
		if (KEY(15) = '1') then
			next_state <= MSG_OK;
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= FAIL;
		end if;
	
	-- vyhodnoceni jaka zprava se vytiskne
	
	 when MSG_OK =>
      next_state <= MSG_OK;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
	
	 when MSG_ERR =>
		next_state <= MSG_ERR;
		if (CNT_OF = '1') then
			next_state <= FINISH;
		end if;
		
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      next_state <= FINISH;
      if (KEY(15) = '1') then
         next_state <= STAV1; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
      next_state <= STAV1;
   end case;
end process next_state_logic;

-- -------------------------------------------------------
output_logic : process(present_state, KEY)
begin
   FSM_CNT_CE     <= '0';
   FSM_MX_MEM     <= '0';
   FSM_MX_LCD     <= '0';
   FSM_LCD_WR     <= '0';
   FSM_LCD_CLR    <= '0';

   case (present_state) is

   when MSG_ERR =>
      FSM_CNT_CE  <= '1';
      FSM_MX_LCD  <= '1';
      FSM_LCD_WR  <= '1';
		-- MX_MEM <= 0 neni treba, protoze je to vychozi hodnota

   when MSG_OK =>
		FSM_CNT_CE  <= '1';
      FSM_MX_MEM  <= '1';
      FSM_MX_LCD  <= '1';
      FSM_LCD_WR  <= '1';

   when FINISH =>
      if (KEY(15) = '1') then
         FSM_LCD_CLR <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   
	-- tisk * na display
	
	when others =>
		if (KEY(14 downto 0) /= "000000000000000") then
			FSM_LCD_WR <= '1';
		elsif(KEY(15) = '1') then
			FSM_LCD_CLR <=  '1';
		end if;
   end case;
end process output_logic;

end architecture behavioral;

