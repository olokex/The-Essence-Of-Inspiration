/**
 * Autor: Adam Sedlacek
 * Login: xsedla1e
 * Predmet: EVO 2019/20
 * Projekt: Evoluce obvodu pro priblizne vypocty
 * 
 * Popis:
 *  Muj cil byl nacist CGP reprezentaci obvodu,
 *  ktery jsem se pak snazil deaktivaci hradel aproximovat.
 */
#include <algorithm>
#include <bitset>
#include <cmath>
#include <iostream>
#include <regex>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <time.h>
#include <vector>

/**
 * Konstanty pro bitset, ktery vyzaduje pri prekladu mit danou velikost.
 * Primo tedy pro 3-bitovou nasobicku.
 * 
 * Pri nacteni jineho obvodu je treba upravit,
 *  aby odpovidalo (velikosti) pravdivostni tabulce daneho obvodu.
 */

/* multi 3-bit */
constexpr auto BIT_VECTOR_SIZE = 64;
constexpr auto INPUTS_COUNT = 6;
constexpr auto OUTPUTS_COUNT = 6;

/* 3-bit nasobicka jen s jinym poctem hradel */
constexpr auto CS = "{6,6, 80,1, 2,1,40}([6]2,4,2)([7]1,5,2)([8]3,0,4)([9]4,7,2)"
                    "([10]1,6,7)([11]3,10,5)([12]5,2,2)([13]4,1,2)([14]3,9,4)"
                    "([15]13,14,2)([16]4,15,4)([17]4,5,7)([18]12,2,5)([19]6,11,3)"
                    "([20]18,8,3)([21]18,7,5)([22]6,8,6)([23]20,20,7)([24]16,21,2)"
                    "([25]11,24,6)([26]7,6,4)([27]20,0,5)([28]17,22,6)([29]24,11,6)"
                    "([30]4,24,7)([31]22,15,2)([32]3,30,0)([33]2,14,7)([34]13,2,4)"
                    "([35]22,24,6)([36]25,35,4)([37]13,36,3)([38]30,20,5)([39]31,21,0)"
                    "([40]12,5,6)([41]14,33,3)([42]23,10,5)([43]38,36,5)([44]38,0,2)"
                    "([45]27,31,6)([46]7,38,1)([47]43,42,5)([48]36,45,4)([49]43,18,3)"
                    "([50]49,49,4)([51]1,40,0)([52]27,5,6)([53]17,38,3)([54]34,48,7)"
                    "([55]41,0,7)([56]33,25,2)([57]3,26,3)([58]26,1,4)([59]21,31,3)"
                    "([60]50,5,3)([61]34,55,7)([62]39,35,2)([63]31,4,1)([64]32,0,4)"
                    "([65]55,54,6)([66]43,55,7)([67]1,64,5)([68]40,2,5)([69]45,42,3)"
                    "([70]43,44,2)([71]38,66,7)([72]53,46,4)([73]59,36,2)([74]35,46,4)"
                    "([75]50,38,6)([76]63,65,1)([77]44,67,4)([78]64,5,7)([79]3,44,6)"
                    "([80]65,64,3)([81]66,62,3)([82]74,66,2)([83]71,79,6)([84]1,3,0)"
                    "([85]52,61,7)(70,83,48,85,26,12)";

/*
constexpr auto CS = "([6]1,4,5)([7]1,5,2)([8]0,4,2)([9]0,5,2)([10]2,4,5)"
                    "([11]3,1,2)([12]11,11,6)([13]6,2,4)([14]5,2,2)([15]3,2,5)([16]11,10,2)"
                    "([17]6,15,5)([18]9,17,2)([19]7,10,7)([20]8,8,6)([21]2,19,4)([22]20,8,3)"
                    "([23]5,7,2)([24]16,20,7)([25]8,13,0)([26]15,16,5)([27]1,3,5)([28]11,8,2)"
                    "([29]24,18,5)([30]10,6,6)([31]8,3,0)([32]15,9,4)([33]23,23,2)([34]10,19,3)"
                    "([35]28,23,1)([36]3,29,4)([37]27,7,2)([38]19,18,5)([39]14,11,2)([40]0,30,6)"
                    "([41]9,3,6)([42]36,28,4)([43]9,13,4)([44]28,42,3)([45]31,17,1)([46]15,2,5)"
                    "([47]13,27,0)([48]17,31,3)([49]21,17,4)([50]5,30,1)([51]25,32,2)([52]42,46,0)"
                    "([53]40,20,3)([54]42,50,2)([55]18,23,0)([56]52,22,0)([57]46,34,7)([58]44,3,2)"
                    "([59]45,40,3)([60]46,34,6)([61]46,57,3)([62]41,28,6)([63]24,60,6)([64]44,48,1)"
                    "([65]41,56,6)([66]52,47,3)([67]52,3,4)([68]55,1,1)([69]52,60,2)([70]41,60,1)"
                    "([71]50,38,1)([72]60,41,1)([73]36,57,5)([74]43,57,4)([75]0,5,2)([76]40,42,6)"
                    "([77]72,43,5)([78]46,47,0)([79]72,57,6)([80]65,73,0)([81]64,44,3)([82]49,62,5)"
                    "([83]55,63,7)([84]67,68,2)([85]52,72,0)(58,76,83,74,19,14)";
*/

/**
 * Pomocna struktura pro referencni bity, pro dany obvod.
 *  (pro spravnou funkcnost je potreba upravit konstanty vyse)
 */
struct Ref_bits {
    std::vector<std::bitset<BIT_VECTOR_SIZE>> input_bits;
    std::vector<std::bitset<BIT_VECTOR_SIZE>> output_bits;
    Ref_bits() {
        input_bits.resize(INPUTS_COUNT);
        for (auto &x : input_bits) x = 0;
        output_bits.resize(OUTPUTS_COUNT);
        for (auto &x : output_bits) x = 0;
    }
};

/**
 * Vyctovy typ, ktery uchovava dane logicke funkce hradel.
 * Musi odpovidat dane CGP reprezentaci (napr. AND => 2).
 */
enum class Function {
    In,
    Not,
    And,
    Or,
    Xor,
    Nand,
    Nor,
    Xnor,
    NumberOfFunctions
};

/**
 * Pomocna struktura, ktera odpovida jednomu hradlu v obvodu.
 * Zaroven drzi v sobe hodnotu bitu pro evaluaci.
 */
struct Cell {
    int input1 = 0;
    int input2 = 0;
    Function function = Function::NumberOfFunctions;
    std::bitset<BIT_VECTOR_SIZE> output = 0;
};

/**
 * Struktura pro parametry s vychozimy parametry (viz. help).
 */
struct Param {
    unsigned long int generations = 150000;
    unsigned long int lambda = 10;
    unsigned long int error = ((OUTPUTS_COUNT * BIT_VECTOR_SIZE) / 100.0) * 10;
    unsigned long int seed = time(NULL);
    unsigned long int mutation = 100;
    int lookup = -1;
    bool print_stats = false;
    std::string path = "";
};

/**
 * Vlastni prevod z duvodu overeni zda nejsou na vstupu "nesmyslne" funkce.
 */
Function cast_fun(std::string fun)
{
    switch (std::stoi(fun)) {
    case 0:
        return Function::In;
    case 1:
        return Function::Not;
    case 2:
        return Function::And;
    case 3:
        return Function::Or;
    case 4:
        return Function::Xor;
    case 5:
        return Function::Nand;
    case 6:
        return Function::Nor;
    case 7:
        return Function::Xnor;
    default:
        std::cerr << "Gate function isn't valid" << std::endl;
        std::abort();
    }
}

/**
 * Funkce na proparsovani/zjisteni vystupu obvodu.
 */
std::vector<int> get_outputs() {
    std::vector<int> out_idx;
    int opening_bracket;
    std::string str_cgp = std::string(CS);
    std::string num = "";
    for (opening_bracket = str_cgp.size(); str_cgp[opening_bracket] != '('; opening_bracket--);
    for (int i = opening_bracket + 1; i < str_cgp.size() - 1; i++) {
        if (str_cgp[i] == ',') {
            out_idx.push_back(std::stoi(num));
            num = "";
            continue;
        }
        num += str_cgp[i];
    }
    out_idx.push_back(std::stoi(num));
    return out_idx;
}

/**
 * Pomocna struktura, ktera drzi index hradla a jeho puvodni funkci,
 *  vyuzito pri mutaci, aby se znovu aktivovalo hradlo do puvodniho stavu.
 */
struct Gate_info {
    int cell_idx;
    Function fun;
};

/**
 * Hlavni trida, ktera drzi jeden cely obvod, a tedy, i jeho hradla s funkcemi a vystupy.
 */
struct Circuit {
    /* Vektor pro vystupy */
    std::vector<int> idx_output;
    /* Vektor bunek - hradel */
    std::vector<Cell> cells;
    /* Fitness daneho obvodu */
    unsigned long int circuit_fitness = INT32_MAX;
    /* Citac vyuzitych hradel */
    int used_gates = INT32_MAX;
    /* Vektor indexů danych hradel, ktere jsou aktivni */
    std::vector<int> gates;
    /* Vektor deaktivovanych hradel a jejich funkce */
    std::vector<Gate_info> removed_gates_vector;
    /* Vektor vektoru pro dane sloupce hradel */
    std::vector<std::vector<int>> components_col;
    /* Pomocny vektor, ktery drzi funkcni hradla ve zvolenem sloupci */
    std::vector<int> functional_gates_given_col;
    /* Vektor, ktery je vyuzit pri rekurzi */
    std::vector<int> components;

    /**
     * Metoda, ktery nacte jednotliva hradla s jejich funkci.
     */
    void Load_circuit() {
        Cell cell;
        std::regex rgx(R"(\(\[.*?\](\d+),(\d+),(\d+)\) ?)");
        std::smatch m;
        std::string x = std::string(CS);
        while (std::regex_search(x, m, rgx)) {
            cell.input1 = std::stoi(std::string(m[1]));
            cell.input2 = std::stoi(std::string(m[2]));
            cell.function = cast_fun(std::string(m[3]));
            cells.push_back(cell);
            x = m.suffix();
        }
        idx_output = get_outputs();
    }

    /**
     * Evaluace pro dany obvod.
     * Kazde hradlo, i pokud je neaktivni se vyhodnoti.
     * Je vyhodnocovano od prvniho hradla az po posledni,
     *  tak aby se zajilo spravne poradi.
     * Vyuzito paralelni simulace diky bitset.
     */
    void Evaluate(Ref_bits* loaded_bits) {
        std::vector<std::bitset<BIT_VECTOR_SIZE>> bit_holder;
        std::bitset<BIT_VECTOR_SIZE> tmp_out, in1, in2;

        for (int i = 0; i < INPUTS_COUNT; i++) {
            bit_holder.push_back(loaded_bits->input_bits[i]);
        }

        for (int i = 0; i < cells.size(); i++) {
            in1 = bit_holder[cells[i].input1];
            in2 = bit_holder[cells[i].input2];

            switch (cells[i].function) {
            case (Function::In): tmp_out = in1; break;
            case (Function::Not): tmp_out = ~in1; break;
            case (Function::And): tmp_out = in1 & in2; break;
            case (Function::Or): tmp_out = in1 | in2; break;
            case (Function::Xor): tmp_out = in1 ^ in2; break;
            case (Function::Nand): tmp_out = ~(in1 & in2); break;
            case (Function::Nor): tmp_out = ~(in1 | in2); break;
            case (Function::Xnor): tmp_out = ~(in1 ^ in2); break;
            }
            cells[i].output = tmp_out;
            bit_holder.push_back(tmp_out);
        }
    }

    /**
     * Metoda pro moznost vytisknuti bitovych hodnot (slouzi pro kontrolu).
     */
    void Print(Ref_bits* loaded_bits) {
        std::vector<std::bitset<BIT_VECTOR_SIZE>> bit_holder;

        for (int i = 0; i < INPUTS_COUNT; i++) {
            bit_holder.push_back(loaded_bits->input_bits[i]);
        }
        for (int i = 0; i < cells.size(); i++) {
            bit_holder.push_back(cells[i].output);
        }
        for (int i = 0; i < bit_holder.size(); i++) {
            if (i < INPUTS_COUNT) {
                std::cout << "input: " << i << " hodnota: " << bit_holder[i].to_string() << std::endl;
            } else {
                std::cout << "komponenta: " << i << " funkce: " << static_cast<int>(cells[i - INPUTS_COUNT].function) << " hodnota: " << bit_holder[i].to_string() << std::endl;
            }
        }
    }

    /**
     * Vypocet fitness, kdy se vezme vektor vystupu,
     *  a procykli se cell.output, ktery drzi bity.
     * Pokud dany bit nesedi, tak se pricte, hodnota 0 => plne funkcni obvod.
     */
    void Fitness(Ref_bits* loaded_bits) {
        circuit_fitness = 0;
        std::vector<std::bitset<BIT_VECTOR_SIZE>> bit_holder;

        for (int i = 0; i < INPUTS_COUNT; i++) {
            bit_holder.push_back(loaded_bits->input_bits[i]);
        }
        for (int i = 0; i < cells.size(); i++) {
            bit_holder.push_back(cells[i].output);
        }

        for (int i = 0; i < OUTPUTS_COUNT; i++) {
            for (int y = 0; y < BIT_VECTOR_SIZE; y++) {
                if (bit_holder[idx_output[i]][y] != loaded_bits->output_bits[i][y]) {
                    circuit_fitness++;
                }
            }
        }
    }

    /**
     * Metoda, ktera zjisti funkcni hradla v obvode.
     * Zjisteno pomoci rekurze, ktera je nize.
     */
    void Gates_in_use() {
        std::vector<int> tmp;
        for (int i = 0; i < INPUTS_COUNT; i++) {
            Cell cell;
            cells.insert(cells.begin(), cell);
        }
        for (int i = 0; i < idx_output.size(); i++) {
            find_path(idx_output[i]);
        }
        for (int i = 0; i < components.size(); i++) {
            if(std::count(tmp.begin(), tmp.end(), components[i]) == 0) {
                tmp.push_back(components[i]);
            }
        }
        used_gates = tmp.size();
        cells.erase(cells.begin(), cells.begin() + INPUTS_COUNT);
        gates = tmp;
        components.clear();
    }

    /**
     * Rekurze, tak aby se prosla vsechna hradla od vystupu az do vstupu.
     */
    void find_path(int comp_num) {
        if (comp_num < INPUTS_COUNT) return;
        if (cells[comp_num].function != Function::In) {
            components.push_back(comp_num);
        }
        find_path(cells[comp_num].input1);
        if (cells[comp_num].function != Function::In
            &&
            cells[comp_num].function != Function::Not) {
            find_path(cells[comp_num].input2);
        }
    }

    /**
     * Vytvori vektor vektoru hradel, tak ze jsou rozzarena do jednotlivych sloupcu.
     * Vektor je "cisty" tj. nejsou zadna hradla pridana vicekrat.
     *
     * Je zavolano pouze jednou tudiz se jedna o relativne "nepekny" kod.
     */
    void Make_cols() {
        components_col.emplace_back();
        int col = 0;
        int cnt = 0;

        for (int i = 0; i < cells.size(); i++) {
            if (cells[i].input1 < INPUTS_COUNT && cells[i].input2 < INPUTS_COUNT) {
                components_col[col].push_back(i + INPUTS_COUNT);
                cnt++;
            } else if ((cells[i].function == Function::Not || cells[i].function == Function::In)
                    && cells[i].input1 < INPUTS_COUNT) {
                components_col[col].push_back(i + INPUTS_COUNT);
                cnt++;
            }
        }

        while (cnt < cells.size()) {
            components_col.emplace_back();
            col++;
            for (int i = 0; i < cells.size(); i++) {
                Cell c = cells[i];
                bool in1 = false;
                bool in2 = false;
                if (c.function == Function::In || c.function == Function::Not) {
                    for (int k = 1; k < components_col.size(); k++) {
                        if (std::count(components_col[col - k].begin(), components_col[col - k].end(), c.input1)) {
                            bool contains = false;
                            for (int k = 0; k < components_col.size(); k++) {
                                if (std::count(components_col[k].begin(), components_col[k].end(), i + INPUTS_COUNT)) {
                                    contains = true;
                                    break;
                                }
                            }
                            if (contains == false) {
                                components_col[col].push_back(i + INPUTS_COUNT);
                                cnt++;
                            }
                            break;
                        }
                    }
                    continue;
                }
                for (int k = 1; k < components_col.size(); k++) {
                    if (std::count(components_col[col - k].begin(), components_col[col - k].end(), c.input1)) {
                        in1 = true;
                    }
                    if (std::count(components_col[col - k].begin(), components_col[col - k].end(), c.input2)) {
                        in2 = true;
                    }
                }
                if (in2 == false && in1 == true) {
                    if (c.input2 < INPUTS_COUNT) {
                        in2 = true;
                    }
                }
                if (in1 == false && in2 == true) {
                    if (c.input1 < INPUTS_COUNT) {
                        in1 = true;
                    }
                }
                
                if (in1 && in2) {
                    bool contains = false;
                    for (int k = 0; k < components_col.size(); k++) {
                        if (std::count(components_col[k].begin(), components_col[k].end(), i + INPUTS_COUNT)) {
                            contains = true;
                            break;
                        }
                    }
                    if (contains == false) {
                        components_col[col].push_back(i + INPUTS_COUNT);
                        cnt++;
                    }
                }
            }
        }
    }

    /**
     * Vytiskne CGP reprezentaci, uz primo s deaktivovanimy hradly.
     * Nasledne se muze zobrazit napr. v cgpvieweru.
     */
    void Cgp_viewer() {
        int tmp = static_cast<int>(Function::NumberOfFunctions);
        printf("{%d,%d,%d,%d,2,1,%d}", INPUTS_COUNT, OUTPUTS_COUNT, cells.size(), 1, tmp);
        for (int i = 0; i < cells.size(); i++) {
            printf("([%i]%d,%d,%d)", i + INPUTS_COUNT, cells[i].input1, cells[i].input2, cells[i].function);
        }

        if (OUTPUTS_COUNT == 1) {
            printf("(%d)", idx_output[0]);
        } else {
            printf("(%d", idx_output[0]);
            for (int i = 1; i < OUTPUTS_COUNT; i++) {
                printf(",%d", idx_output[i]);
            }
            printf(")");
        }
        printf("\n\n");
    }

    /**
     * Deaktivuje nahodne hradlo v obvodu.
     * Deaktivace je udelana, tak ze dane hradlo se stane pouze "dratem".
     */
    void Disable_functional_gate() {
        int idx_dis = rand() % gates.size();
        int remove_gate = gates[idx_dis];
        Gate_info tmp = {remove_gate, cells[remove_gate - INPUTS_COUNT].function};
        removed_gates_vector.push_back(tmp);
        cells[remove_gate - INPUTS_COUNT].function = Function::In;
        gates.erase(gates.begin() + idx_dis);
        used_gates = gates.size();
    }

    /**
     * Mutace je udelana tak, ze se vybere nahodne deaktivovane hradlo,
     *  a to se nasledne aktivuje a opet se nahodne vybere hradlo na deaktivaci.
     * (muze se jednat o to stejne hradlo, pokud nahoda tak padne)
     */
    void Mutate(int chance) {
        if (removed_gates_vector.size() <= 0) return;
        if (chance > rand() % 100) {
            int idx_removed = rand() % removed_gates_vector.size();
            cells[removed_gates_vector[idx_removed].cell_idx - INPUTS_COUNT].function = removed_gates_vector[idx_removed].fun;
            removed_gates_vector.erase(removed_gates_vector.begin() + idx_removed);
            Gates_in_use();
            Disable_functional_gate();
        }
    }

    /**
     * Rozzradi funkcni hradla v danem sloupecku.
     */
    void Functional_gates_selected_col(int col) {
        functional_gates_given_col.clear();
        for (int i = 0; i < components_col[col].size(); i++) {
            if (std::count(gates.begin(), gates.end(), components_col[col][i])) {
                functional_gates_given_col.push_back(components_col[col][i]);
            }
        }
    }

    /**
     * Obdoba metody vyse, jen se deaktivuje v danem sloupci.
     * Neni potreba urcit, ktery sloupec, protoze
     * _functional_gates_given_col_ jiz drzi zvoleny sloupec.
     */
    void Disable_functional_gate_col() {
        using namespace std;
        if (functional_gates_given_col.size() <= 0) return;
        int idx_dis = rand() % functional_gates_given_col.size();
        int remove_gate = functional_gates_given_col[idx_dis];

        Gate_info tmp = {remove_gate, cells[remove_gate - INPUTS_COUNT].function};
        removed_gates_vector.push_back(tmp);
        cells[remove_gate - INPUTS_COUNT].function = Function::In;
        functional_gates_given_col.erase(functional_gates_given_col.begin() + idx_dis);
        Gates_in_use();
        used_gates = gates.size();
    }

    /**
     * Pretizena metoda pro mutaci.
     * Obdoba pouze ve zvolenem sloupci.
     */
    void Mutate(int chance, int col) {
        using namespace std;
        if (removed_gates_vector.size() <= 0) return;
        if (chance > rand() % 100) {
            int idx_removed = rand() % removed_gates_vector.size();
            cells[removed_gates_vector[idx_removed].cell_idx - INPUTS_COUNT].function = removed_gates_vector[idx_removed].fun;
            removed_gates_vector.erase(removed_gates_vector.begin() + idx_removed);
            Gates_in_use();
            Functional_gates_selected_col(col);
            Disable_functional_gate_col();
        }
    }
};

/**
 * Funkce nacte ze souboru bity z pravdivostni tabulky.
 * Stejny vstup jako je pro CGP.cpp od doc. Vasicka 2012.
 */
void load_from_file(Ref_bits *ptr, std::string path)
{
    FILE* fp = fopen(path.c_str(), "r");
    if (fp == NULL) {
        printf("Wrong file path.\n");
        exit(EXIT_FAILURE);
    }

    char* line = NULL;
    size_t len = 0;
    while ((getline(&line, &len, fp)) != -1) {
        if ('#' == line[0]) {
            continue;
        } else {
            fseek(fp, ftell(fp) - strlen(line), SEEK_SET);
            break;
        }
    }

    int c;
    bool loading_input_bit = true;
    bool commentary = false;
    int col = 0;
    int row = 0;

    while ((c = fgetc(fp)) != EOF) {
        if (commentary && c != '\n') {
            continue;
        }
        if (c == '0' || c == '1') {
            if (loading_input_bit) {
                ptr->input_bits[col][row] = c == '1' ? true : false;
            }
            if (!loading_input_bit) {
                ptr->output_bits[col][row] = c == '1' ? true : false;
            }
            col++;
        } else if (c == ' ') {
            continue;
        } else if (c == ':') {
            col = 0;
            loading_input_bit = !loading_input_bit;
        } else if (c == '\n') {
            row++;
            col = 0;
            loading_input_bit = !loading_input_bit;
            commentary = false;
        } else if (c == '#') {
            commentary = true;
        }
    }
    fclose(fp);
    free(line);
}

/**
 * Funkce, ktera najde nejlepsiho jedince v populaci.
 */
Circuit &find_best(std::vector<Circuit> &pop) {
    Circuit &best = pop[0];
    for (int i = 0; i < pop.size(); i++) {
        if (best.circuit_fitness >= pop[i].circuit_fitness) {
            best = pop[i];
        }
    }
    return best;
}

/**
 * Samotna evoluce je zde, pomoci argumentu programu se da ruzne upravovat.
 * Tedy i lehce se muze pozmenit strategie, z lambda + 5, ktera je typicka pro CGP
 *  se muze pozmenit na ES + 1.
 * Muze se urcit i mira mutace.
 * Jak velky error k aproximaci.
 * Je zavedeny elitismus.
 */
void evolution(Param par, Ref_bits *loaded_bits) {
    std::vector<Circuit> population;
    int cnt = 0;
    srand(par.seed);
    std::cout << "seed: " << par.seed << std::endl;

    for (int i = 0; i < par.lambda + 1; i++) {
        Circuit tmp;
        tmp.Load_circuit();
        tmp.Evaluate(loaded_bits);
        tmp.Fitness(loaded_bits);
        tmp.Make_cols();
        tmp.Gates_in_use();
        population.push_back(tmp);
    }

    Circuit best = find_best(population);
    population.pop_back();
    std::cout << "gates given: " << best.used_gates << std::endl;

    while (cnt < par.generations) {
        Circuit offspring;
        for (int i = 0; i < par.lambda; i++) {
            offspring = best;
            offspring.Gates_in_use();
            offspring.Disable_functional_gate();
            offspring.Mutate(par.mutation);
            offspring.Evaluate(loaded_bits);
            offspring.Fitness(loaded_bits);
            population[i] = offspring;
        }

        cnt += par.lambda;
        offspring = find_best(population);

        if (offspring.circuit_fitness <= par.error) {
            best = offspring;
        }

        if (cnt % 1000 == 0 && par.print_stats) {
            std::cout << "iteration: " << cnt << 
            " fitness: " << best.circuit_fitness << 
            std::endl;
        }
    }
    std::cout << "disabled gates: " << best.removed_gates_vector.size() << std::endl;
    best.Cgp_viewer();
}

/**
 * Pomocna funkce na vytisknuti help.
 * ./aprox -h
 */
void print_help() {
    using namespace std;
    cout << "All switches are optional" << endl;
    cout << "Description: " << endl;
    cout << endl;
    cout << "generations terminating condition" << endl;
    cout << "lambda how many inviduals can be in one population" << endl;
    cout << "error percentage interpretation of incorrect bits in a circuit (0-100, otherwise 10 %)" << endl;
    cout << "mutation chance to reactive disabled gate and deactive randomly any gate (0-100, otherwise 100 %)" << endl;
    cout << "lookup selected column to be deactivating gates (default all)" << endl;
    cout << "print out ongoing statistics (default false)" << endl;
    cout << "path location to the file of truth table for certain circuit" << endl;
    cout << endl;
    cout << "Example of usage with all parameters:" << endl;
    cout << endl;
    cout << "./aprox generations 1000 lambda 10 error 10 seed 1516171820 mutation 80 lookup 3 print true path ./multi3x3" << endl;
    exit(0);
}

/**
 * Funkce na proparsovani argumentu.
 * Viz. help.
 */
Param parse_arguments(int argc, char *argv[]) {
    using namespace std;
    string arg = "";
    string val = "";
    Param par;
    if (argc == 2 && string(argv[1]) == "-h") print_help();
    for (int i = 1; i < argc; i += 2) {
        arg = string(argv[i]);
        val = string(argv[i + 1]);
        if (arg == "generations") par.generations = stoul(val);
        else if (arg == "lambda") par.lambda = stoul(val);
        else if (arg == "seed")  par.seed = stoul(val);
        else if (arg == "lookup") par.lookup = stoi(val);
        else if (arg == "print") par.print_stats = true;
        else if (arg == "path") par.path = val;
        else if (arg == "mutation") {
            par.mutation = stoul(val);
            if (par.mutation > 100) par.mutation = 10;
        }
        else if (arg == "error") {
            unsigned long int tmp = stoul(val);
            if (tmp <= 100) {
                par.error = ((OUTPUTS_COUNT * BIT_VECTOR_SIZE) / 100.0) * tmp;
            }
        }
        else {
            throw std::invalid_argument("Received wrong parameter.");
        }
    }
    return par;
}

/**
 * Funkce co vytikne rozzarene hradla do sloupecku (nevyuzito).
 */
void print_columns(Circuit cir) {
    for (int i = 0; i < cir.components_col.size(); i++) {
        std::cout << "col: " << i << " :: ";
        for (int y = 0; y < cir.components_col[i].size(); y++) {
            std::cout << cir.components_col[i][y] << " ";
        }
        std::cout << std::endl;
    }
}

/**
 * Pretizena funkce na evoluci, pouze s tim rozdelem,
 *  ze je zvolen vybrany sloupecek.
 * Pokud uzivatel zvoli prilis vysoke cislo sloupce je mu to dano najevo.
 * Cisluje se 0-n.
 */
void evolution(Param par, Ref_bits *loaded_bits, int col_lookup) {
    Circuit verify;
    verify.Load_circuit();
    verify.Make_cols();
    verify.Functional_gates_selected_col(col_lookup);
    if (verify.components_col.size() <= col_lookup) {
        std::cerr << "not too many columns in circuit" << std::endl;
        exit(1);
    }

    srand(par.seed);
    unsigned long int cnt = 0;
    std::vector<Circuit> population;
    for (int i = 0; i < par.lambda + 1; i++) {
        Circuit tmp;
        tmp.Load_circuit();
        tmp.Make_cols();
        tmp.Evaluate(loaded_bits);
        tmp.Fitness(loaded_bits);
        tmp.Gates_in_use();
        tmp.Functional_gates_selected_col(col_lookup);
        population.push_back(tmp);
    }

    Circuit best = find_best(population);
    population.pop_back();
    std::cout << "gates given: " << best.used_gates << std::endl;

    while (cnt < par.generations) {
        Circuit offspring;
        for (int i = 0; i < par.lambda; i++) {
            offspring = best;
            offspring.Gates_in_use();
            offspring.Functional_gates_selected_col(col_lookup);
            offspring.Disable_functional_gate_col();
            offspring.Mutate(par.mutation, col_lookup);
            offspring.Evaluate(loaded_bits);
            offspring.Fitness(loaded_bits);
            population[i] = offspring;
        }
        cnt += par.lambda;
        if (offspring.circuit_fitness <= par.error) {
            best = offspring;
        }

        if (cnt % 1000 == 0 && par.print_stats) {
            std::cout << "iteration: " << cnt << 
            " fitness: " << best.circuit_fitness << 
            std::endl;
        }
    }
    std::cout << "disabled gates: " << best.removed_gates_vector.size() << " of " << best.components_col[col_lookup].size() << std::endl;
    best.Cgp_viewer();
}

/**
 * Main...
 *  nacteni referencnich bitu
 *  proparsovani argumentu
 *  zavolani jednotlivych evoluci
 */
int main(int argc, char *argv[]) {
    Ref_bits loaded_bits;
    Param p;
    try {
        p = parse_arguments(argc, argv);
    } catch (std::invalid_argument &ex) {
        std::cout << ex.what() << std::endl;
        return 1;
    } catch (std::exception) {
        std::cerr << "Problem within arguments." << std::endl;
        return 1;
    }
    load_from_file(&loaded_bits, p.path);
    if (p.lookup != -1) {
        evolution(p, &loaded_bits, p.lookup);
    } else {
        evolution(p, &loaded_bits);
    }
    return 0;
}