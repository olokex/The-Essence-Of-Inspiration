-- cpu.vhd: Simple 8-bit CPU (BrainF*ck interpreter)
-- Copyright (C) 2018 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): xsedla1e
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet ROM
   CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
   CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
   CODE_EN   : out std_logic;                     -- povoleni cinnosti
   
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni z pameti (DATA_RDWR='1') / zapis do pameti (DATA_RDWR='0')
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA obsahuje stisknuty znak klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna pokud IN_VLD='1'
   IN_REQ    : out std_logic;                     -- pozadavek na vstup dat z klavesnice
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- pokud OUT_BUSY='1', LCD je zaneprazdnen, nelze zapisovat,  OUT_WE musi byt '0'
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

	signal pc_addr: std_logic_vector(11 downto 0) := (others => '0');
	signal pc_inc: std_logic := '0';
	signal pc_dec: std_logic := '0';

	signal ram_ptr: std_logic_vector(9 downto 0) := (others => '0');
	signal ram_ptr_inc: std_logic := '0';
	signal ram_ptr_dec: std_logic := '0';

	signal bracket_cnt: std_logic_vector(7 downto 0) := (others => '0');
	signal bracket_inc: std_logic := '0';
	signal bracket_dec: std_logic := '0';
	signal bracket_reset: std_logic := '0';

	signal sel: std_logic_vector(1 downto 0) := "00";
	signal tmp_hexa: std_logic_vector(7 downto 0) := (others => '0');

	type fsm is (
		fetch, p_decode,p_others,
		p_ram_ptr_inc, p_ram_ptr_dec,
		p_cell_inc, p_cell_inc_2, p_cell_dec, p_cell_dec_2,
		p_putchar, p_putchar_2, p_getchar, 
		p_while_start, p_while_start_2, p_while_start_3, p_while_start_4,
		p_while_end, p_while_end_2, p_while_end_3, p_while_end_4, p_while_end_5, p_while_end_6,
		p_comment1, p_comment2, p_comment3,
		p_hexa_char1, p_hexa_char2,
		p_null

	);

	signal state: fsm;
	signal future_state: fsm;

begin

	multiplexor: process(IN_DATA, DATA_RDATA, sel, tmp_hexa)
	begin

		case sel is
			when "00" => DATA_WDATA <= IN_DATA;
			when "01" => DATA_WDATA <= tmp_hexa;
			when "10" => DATA_WDATA <= DATA_RDATA - 1;
			when "11" => DATA_WDATA <= DATA_RDATA + 1;
			when others =>
		end case;

	end process;

	reg_pc: process(CLK, RESET, pc_addr, pc_inc, pc_dec)
	begin
		if RESET = '1' then
			pc_addr <= (others => '0');
		elsif rising_edge(CLK) then
			if pc_inc = '1' then
				pc_addr <= pc_addr + 1;
			elsif pc_dec = '1' then
				pc_addr <= pc_addr - 1;
			end if;
		end if;
	end process;

	CODE_ADDR <= pc_addr;

	ram_pointer: process(CLK, RESET, ram_ptr, ram_ptr_inc, ram_ptr_dec)
	begin
		if RESET = '1' then
			ram_ptr <= (others => '0');
		elsif rising_edge(CLK) then
			if ram_ptr_inc = '1' then
				ram_ptr <= ram_ptr + 1;
			elsif ram_ptr_dec = '1' then
				ram_ptr <= ram_ptr - 1;
			end if;
		end if;
	end process;

	DATA_ADDR <= ram_ptr;

	bracket_counter: process(CLK, RESET, bracket_inc, bracket_dec)
	begin 
		if RESET = '1' then
			bracket_cnt <= (others => '0');
		elsif rising_edge(CLK) then
			if bracket_inc = '1' then
				bracket_cnt <= bracket_cnt + 1;
			elsif bracket_dec = '1' then
				bracket_cnt <= bracket_cnt - 1;
			elsif bracket_reset = '1' then
				bracket_cnt <= "00000001";
			end if;
		end if;	
	end process;

	fsm_start: process(CLK, RESET)
	begin
		if RESET = '1' then
			state <= fetch;
		elsif rising_edge(CLK) then
			if EN = '1' then
				state <= future_state;
			end if;
		end if;
	end process;

	fsm_future_state: process(CODE_DATA, IN_VLD, OUT_BUSY, DATA_RDATA, bracket_cnt, state)
	begin

		-- inicializace
		CODE_EN <= '1';
		DATA_EN <= '0';
		OUT_WE <= '0';
		IN_REQ <= '0';
		DATA_RDWR <= '0';
		pc_inc <= '0';
		pc_dec <= '0';
		ram_ptr_inc <= '0';
		ram_ptr_dec <= '0';
		bracket_inc <= '0';
		bracket_dec <= '0';
		bracket_reset <= '0';
		sel <= "00";
				
		case state is
			when fetch =>
				CODE_EN <= '1';
				future_state <= p_decode;

			when p_decode =>
				case CODE_DATA is
					when X"3E" => future_state <= p_ram_ptr_inc;
					when X"3C" => future_state <= p_ram_ptr_dec;
					when X"2B" => future_state <= p_cell_inc;
					when X"2D" => future_state <= p_cell_dec;
					when X"5B" => future_state <= p_while_start;
					when X"5D" => future_state <= p_while_end;
					when X"2E" => future_state <= p_putchar;
					when X"2C" => future_state <= p_getchar;
					when X"23" => future_state <= p_comment1;

					when 
                        X"30" | X"31" | X"32" | X"33" | 
                        X"34" | X"35" | X"36" | X"37" | 
                        X"38" | X"39" => future_state <= p_hexa_char1;

                  	when 
                  		X"41" | X"42" | X"43" |
                  		X"44" | X"45" | X"46" => future_state <= p_hexa_char2;

					when X"00" => future_state <= p_null;
					when others => future_state <= p_others;
				end case;

			-- >
			when p_ram_ptr_inc =>
				ram_ptr_inc <= '1';
				pc_inc <= '1';
				future_state <= fetch;
			
			-- <
			when p_ram_ptr_dec =>
				ram_ptr_dec <= '1';
				pc_inc <= '1';
				future_state <= fetch;
			
			-- +
			when p_cell_inc =>
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				future_state <= p_cell_inc_2;
			
			when p_cell_inc_2 =>
				sel <= "11";
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				pc_inc <= '1';
				future_state <= fetch;

			-- -
			when p_cell_dec =>
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				future_state <= p_cell_dec_2;
			
			when p_cell_dec_2 =>
				sel <= "10";
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				pc_inc <= '1';
				future_state <= fetch;

			-- .
			when p_putchar =>
				if OUT_BUSY = '1' then
					future_state <= p_putchar;
				else
					DATA_EN <= '1';
					DATA_RDWR <= '1';
					future_state <= p_putchar_2;
				end if;
			
			when p_putchar_2 =>
				OUT_WE <= '1';
				OUT_DATA <= DATA_RDATA;
				pc_inc <= '1';
				future_state <= fetch;

			-- ,
			when p_getchar =>
				IN_REQ <= '1';
				if IN_VLD = '0' then
					future_state <= p_getchar;
				else
					sel <= "00";
					DATA_EN <= '1';
					DATA_RDWR <= '0';
					pc_inc <= '1';
					future_state <= fetch;
				end if;

			-- [
			when p_while_start =>
				pc_inc <= '1';
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				future_state <= p_while_start_2;
			
			when p_while_start_2 =>
				if DATA_RDATA = X"00" then
					bracket_reset <= '1';
					future_state <= p_while_start_3;
				else
					future_state <= fetch;
				end if;
			
			when p_while_start_3 =>
				if bracket_cnt = X"00" then
					future_state <= fetch;
				else
					CODE_EN <= '1';
					future_state <= p_while_start_4;
				end if;
			
			when p_while_start_4 =>
				if CODE_DATA = X"5B" then
					bracket_inc <= '1';
				elsif CODE_DATA = X"5D" then
					bracket_dec <= '1';
				end if;
				pc_inc <= '1';
				future_state <= p_while_start_3;

			-- ]
			when p_while_end =>
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				future_state <= p_while_end_2;
			
			when p_while_end_2 =>
				if DATA_RDATA = X"00" then
					pc_inc <= '1';
					future_state <= fetch;
				else
					future_state <= p_while_end_3;
				end if;
			
			when p_while_end_3 =>
				bracket_reset <= '1';
				pc_dec <= '1';
				future_state <= p_while_end_4;
			
			when p_while_end_4 =>
				if bracket_cnt = X"00" then
					future_state <= fetch;
				else
					CODE_EN <= '1';
					future_state <= p_while_end_5;
				end if;
			
			when p_while_end_5 =>
				if CODE_DATA = X"5D" then
					bracket_inc <= '1';
				elsif CODE_DATA = X"5B" then
					bracket_dec <= '1';
				end if;
				future_state <= p_while_end_6;
			
			when p_while_end_6 =>
				if bracket_cnt = X"00" then
					pc_inc <= '1';
				else
					pc_dec <= '1';
				end if;
				future_state <= p_while_end_4;

			-- #
			when p_comment1 =>
				pc_inc <= '1';
				future_state <= p_comment2;
			
			when p_comment2 =>
				--DATA_EN <= '1';
				CODE_EN <= '1';
				future_state <= p_comment3;
			
			when p_comment3 =>
				if CODE_DATA = X"23" then
					pc_inc <= '1';
					future_state <= fetch;
				else
					future_state <= p_comment1;
				end if;

			-- 0-9
			when p_hexa_char1 =>
				DATA_EN <= '1';
				pc_inc <= '1';
				sel <= "01";
				tmp_hexa <= CODE_DATA(3 downto 0) & X"0";
				future_state <= fetch;

			-- A-F
			when p_hexa_char2 =>
				DATA_EN <= '1';
				pc_inc <= '1';
				sel <= "01";
				--tmp_hexa <= (CODE_DATA(3 downto 0) + X"9") & X"0";
				tmp_hexa <= (CODE_DATA(3 downto 0) + 9) & X"0";
				future_state <= fetch;

			-- _
			when p_null =>
				future_state <= p_null;

			-- ¯\_(ツ)_/¯
			when p_others =>
				pc_inc <= '1';
				future_state <= fetch;

		end case;
	end process;
end behavioral;