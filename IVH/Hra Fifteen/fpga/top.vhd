library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
use work.vga_controller_cfg.all;
use work.game_pack.all;

architecture main of tlv_pc_ifc is

    -- Signaly pro cell
    signal STATE : std_logic_vector(63 downto 0);
    signal INIT_STATE : std_logic_vector(63 downto 0) :=
        "1010011100010100011000101000101100000011010111001001111111011110";
    signal KEYS : std_logic_vector(3 downto 0);

    -- Pomocne signaly na "reset" hraci plochy

    -- 10 7 1 4
    -- 6 2 8 11
    -- 0 3 5 12
    -- 9 15 13 14
    signal STATE_A : std_logic_vector(63 downto 0) := 
        "1010011100010100011000101000101100000011010111001001111111011110";

    -- 0 3 5 12
    -- 6 1 10 13
    -- 7 8 2 15
    -- 4 9 11 14
    signal STATE_B : std_logic_vector(63 downto 0) := 
        "0000001101011100011000011010110101111000001011110100100110111110";

    -- 6 3 7 12
    -- 5 0 11 13
    -- 1 8 4 15
    -- 2 9 10 14
    signal STATE_C : std_logic_vector(63 downto 0) := 
        "0110001101111100010100001011110100011000010011110010100110101110";

    -- 1 9 3 0
    -- 12 4 7 15
    -- 10 2 5 8
    -- 6 14 13 11
    signal STATE_D : std_logic_vector(63 downto 0) := 
        "0001100100110000110001000111111110100010010110000110111011011011";
    
    -- Signal pro porovnani jestli jdou hodnota za sebou
    signal STATE_WIN : std_logic_vector(63 downto 0) :=
        "0001001000110100010101100111100010011010101111001101111011110000";

    -- Signal pro resetovani cell a naslednou inicializaci
    signal RESET_CELL : std_logic;

    -- Signaly pro vga rezim, default 640x480x60
    signal vga_mode : std_logic_vector(60 downto 0);
    signal row : std_logic_vector(11 downto 0);
    signal col : std_logic_vector(11 downto 0);
    signal irgb : std_logic_vector(8 downto 0);

    -- Pomocny signal pro vycentrovani na stred
    signal row_cen : std_logic_vector(11 downto 0);
    signal col_cen : std_logic_vector(11 downto 0);
    
    -- Pomocny signal pro adresovani z rom pameti daneho symbolu
    signal rom_address : std_logic_vector(3 downto 0);

    -- Pomocne signaly pro cernou, bilou a modrou, aditivni michani barev R - 3b, G - 3b, B - 3b
    signal black : std_logic_vector(8 downto 0) := "000000000";
    signal white : std_logic_vector(8 downto 0) := "111111111";
    signal blue : std_logic_vector(8 downto 0) := "000000111";

    -- Signal pro vykresleni pixelu cerny nebo bily
    signal pixel_draw : std_logic;

    -- Signal pro urceni hranic hraci plochy
    signal inx : std_logic := '0';
    signal iny : std_logic := '0';

    -- Signaly pro klavesnici
    signal k_data_out : std_logic_vector(15 downto 0);
    signal k_data_vld : std_logic;

    -- Signal pro klavesnici, protoze 25Mhz je prilis vysoka frekvence pro snimani klavesnice
    -- A zpusobilo to okamzite "prejeti" bunky na jednu z hran
    -- Proto jsem si vypujcil z vga4 engen, kde se deli signal
    signal CLK_ENGEN : std_logic;

    -- Aliasy pro kratsi zapis pro vypocet rom_address
    alias tmp_x is col_cen(7 downto 6);
    alias tmp_y is row_cen(7 downto 6);

begin

    -- Propojeni bunek pro x, y souradnice, pomocna funkce na vypocet v game_pack
    cell_x : for x in 0 to 3 generate
        cell_y : for y in 0 to 3 generate
            connnect_cell : entity work.cell
            generic map ( MASK => getmask(x, y, 4, 4) )
            port map (
                -- Jedna se o matici, kdyz x = 0 - 1 -> -1 tj. mimo nasi matici
                -- Tento problem zaobstarava operace modulu, tj -1 mod 4 -> 3
                -- Takto "preskocime" na druhou stranu matice a nevybocime nikdy
                STATE => STATE( vypocet(x, y) + 3 downto vypocet(x, y) ),
                INIT_STATE => INIT_STATE( vypocet(x, y) + 3 downto vypocet(x, y) ),
                NEIG_LEFT => STATE( vypocet(x - 1, y) + 3 downto vypocet(x - 1, y) ),
                NEIG_RIGHT => STATE( vypocet(x + 1, y) + 3 downto vypocet(x + 1, y) ),
                NEIG_TOP => STATE( vypocet(x, y - 1) + 3 downto vypocet(x, y - 1) ),
                NEIG_BOTTOM => STATE( vypocet(x, y + 1) + 3 downto vypocet(x, y + 1) ),
                KEYS => KEYS,
                CLK => CLK_ENGEN,
                RESET => RESET_CELL
            );

        end generate ; -- cell_y
    end generate; -- cell_x

    get_gen_clk : entity work.engen
    generic map ( MAXVALUE => 250 )
    port map (
            CLK => CLK,
            ENABLE => '1',
            EN => CLK_ENGEN
    ); 

    -- Nastaveni grafickeho rezimu (640x480, 60 Hz refresh)
    setmode(r640x480x60, vga_mode);

    -- Namapovani pro vga radic
    vga: entity work.vga_controller(arch_vga_controller) 
    generic map ( REQ_DELAY => 1 )
    port map (
        CLK    => CLK, 
        RST    => RESET,
        ENABLE => '1',
        MODE   => vga_mode,

        DATA_RED    => irgb(8 downto 6),
        DATA_GREEN  => irgb(5 downto 3),
        DATA_BLUE   => irgb(2 downto 0),
        ADDR_COLUMN => col,
        ADDR_ROW    => row,

        VGA_RED   => RED_V,
        VGA_BLUE  => BLUE_V,
        VGA_GREEN => GREEN_V,
        VGA_HSYNC => HSYNC_V,
        VGA_VSYNC => VSYNC_V
    );

    -- Vypocty pro zarovnani na stred
    col_cen <= col - 192;
    row_cen <= row - 112;

    -- Mezivypocet, aby jsme dostali danou hodnotu SYMBOLU ze soucasneho STATE
    -- Netusim proc muj vypocet pres alias nechce fungovat, budu moc rad za zpetnou vazbu
    -- rom_address <= STATE(vypocet(conv_integer(tmp_x), conv_integer(tmp_y)) + 3 
    --    downto vypocet(conv_integer(tmp_x), conv_integer(tmp_y)));

    rom_address <= STATE(
            (63 - 4 * (conv_integer(col_cen(7 downto 6)) mod 4) - 16 * (conv_integer(row_cen(7 downto 6)) mod 4) - 3) + 3 
        downto
            (63 - 4 * (conv_integer(col_cen(7 downto 6)) mod 4) - 16 * (conv_integer(row_cen(7 downto 6)) mod 4) - 3));

    -- Namapovani signalu pro praci se symboly z ROM pameti
    -- Protoze nase symboly jsou 8x8 a herni plocha by byla pouze 32x32
    -- Pouzili jsme bitove podeleni o 3 bity, tudiz vykreslime 64x64
    -- Takze na hraci plochu budeme mit 4x64 do radku takez i do sloupce
    -- Vznika nam tedy ctverec 256x256, kde jsou jeho souradnice vypocteny nize
    rom: entity work.symbol_rom
    port map (
        ADDRESS => rom_address,
        COLUMN => col_cen(5 downto 3),
        ROW => row_cen(5 downto 3),
        DATA => pixel_draw
    );

    -- Namapovani pro klavesnici
    kbrd_ctrl: entity work.keyboard_controller(arch_keyboard)
    generic map ( READ_INTERVAL => 1000000 )
    port map (
        CLK => CLK_ENGEN,
        RST => RESET,

        DATA_OUT => k_data_out(15 downto 0),
        DATA_VLD => k_data_vld,

        KB_KIN   => KIN,
        KB_KOUT  => KOUT
    );

    -- Pro lepsi orientaci okopirovano z cell.vhd
    -- 0 index klavesy - nahoru
    -- 1 index klavesy - vlevo
    -- 2 index klavesy - vpravo
    -- 3 index klavesy - dole

    game_logic : process( CLK_ENGEN, RESET )
    begin
        if RESET = '1' then
            RESET_CELL <= '1';

        elsif rising_edge(CLK_ENGEN) then
            RESET_CELL <= '0';
            
            -- http://merlin.fit.vutbr.cz/FITkit/docs/firmware/fpga_keyboard.html
            if k_data_vld = '1' then
                KEYS(0) <= k_data_out(4); 
                KEYS(1) <= k_data_out(1);
                KEYS(2) <= k_data_out(9);
                KEYS(3) <= k_data_out(6);

                -- Nahrani signalu_X podle stiskle klavesnice A, B, C, D 
                if k_data_out(12) = '1' then
                    INIT_STATE <= STATE_A;
                    RESET_CELL <= '1';
                end if;

                if k_data_out(13) = '1' then
                    INIT_STATE <= STATE_B;
                    RESET_CELL <= '1';
                end if;

                if k_data_out(14) = '1' then
                    INIT_STATE <= STATE_C;
                    RESET_CELL <= '1';
                end if;

                if k_data_out(15) = '1' then
                    INIT_STATE <= STATE_D;
                    RESET_CELL <= '1';
                end if;
            else
                KEYS <= "0000";
            end if;
        end if; 
    end process; -- game_logic

    -- Vykresleni hraci plochy
    -- Kde si zjistujeme, jestli jsme vne, naseho ctverce, co jsme si vypocetli
    -- Vypocet souradnic 640 / 2 - 256 / 2           -> levy okraj
    -- Vypocet souradnic 480 / 2 - 256 / 2           -> horni okraj
    -- Vypocet souradnic 640 / 2 - 256 / 2 + 256     -> pravy okraj
    -- Vypocet souradnic 480 / 2 - 256 / 2 + 256     -> dolni okraj
    --
    -- 0,0
    --  +---------------------------------------+
    --  |                                       |
    --  |                                       |
    --  |        192,112         448,112        |
    --  |           +---------------+           |
    --  |           |///////////////|           |
    --  |           |///////////////|           |
    --  |           |///////////////|           |
    --  |           |///////////////|           |
    --  |           |///////////////|           |
    --  |           +---------------+           |
    --  |        192,368         448,368        |
    --  |                                       |
    --  |                                       |
    --  +---------------------------------------+ 
    --                                      640,480

    draw_table : process( CLK )
    begin
        if ( rising_edge(CLK) ) then
            
            -- Viz. obrazek o par radku vyse
            if inx = '0' then
                if col = 192 then
                    inx <= '1';
                end if;
            else
                if col = 448 then
                    inx <= '0';
                end if;
            end if;

            if iny = '0' then
                if row = 112 then
                    iny <= '1';
                end if;
            else
                if row = 368 then
                    iny <= '0';
                end if;
            end if;

        -- Vykresleni podle toho kde se zrovna nachazime
        -- Pokud nas soucasny signal se neshoduje s "viteznym" signalem
        -- Jinak nam vznika modry ctverec, ktery ceka na nahrani noveho STATE_X
        -- Vykreslujeme bily nebo cerny pixel podle zrovna daneho bitu z ROM symbolu
        if (inx = '1') and (iny = '1') then
            if STATE_WIN = STATE then
                irgb <= blue;
            elsif pixel_draw = '1' then
                irgb <= black;
            else
                irgb <= white;
            end if;
        else
            irgb <= black;
        end if;

        end if;
    end process; -- draw_table

end main;