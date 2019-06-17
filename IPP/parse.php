<?php
	/*
	 * Autor: Adam Sedlacek | xsedla1e
	 * Rok: 2019
	 * Projekt: parser do IPP
	 *
	 */

	// id citac
	$order_counter = 1;

	// promenne pro pocitani u rozsireni
	$comment_cnt = 0;
	$jupms_cnt = 0;
	$labels_cnt = 0;
	$loc_cnt = 0;

	// unikatni pole labelu, rozsireni
	$uniq_labels = array();

	// promenne pro DocDocument na vytvareni XML
	$program;
	$xml;
	$ins;

	// pomocne pole pro overeni validity instrukce
	$all_instructions = array("DEFVAR", "POPS", "WRITE", "PUSHS", "EXIT", "DPRINT", "CALL",
								"LABEL", "JUMP", "MOVE", "INT2CHAR", "STRLEN", "TYPE", "READ",
								"BREAK", "RETURN", "CREATEFRAME", "PUSHFRAME", "POPFRAME",
								"ADD", "SUB", "MUL", "IDIV", "LT", "AND", "STRI2INT", "CONCAT",
								"STRLEN", "GETCHAR", "SETCHAR", "OR", "NOT", "GT", "EQ", "JUMPIFEQ", "JUMPIFNEQ"
							);

	// pomocna funkce, ktera tiskne na STDERRR zpravu a kod, ktery se preda v parametru
	function print_error($msg, $error_code) {
		fwrite(STDERR, $msg . PHP_EOL);
		exit($error_code);
	}

	// pomocna funkce na overeni hlavicky
	function verify_header($line) {
		if (strtolower(trim($line)) != ".ippcode19") {
			print_error("Spatna hlavicka (.IPPcode19)", 21);
		}
	}

	// pomocna funkce, protoze domdocument sam neprevadi spatne znaky...
	function replace_all_bad_characters($bad) {
		$a = str_replace("&", "&amp;", $bad);
		$a = str_replace('"', "&quot;", $a);
		$a = str_replace("'", "&apos;", $a);
		$a = str_replace("<", "&lt;", $a);
		$a = str_replace(">", "&gt;", $a);
		return $a;
	}

	/* pomocna funkce na vytvoreni dalsiho XML elementu
		byla by zbytecne rozkopirovavana 4krat,
		takto se vytvori prvek a nasledne se pridaji atributy
	*/
	function do_another_xml_element($opcode) {
		global $program, $xml, $order_counter, $ins;
		$ins = $xml->createElement("instruction");
		$ins->setAttribute("order", $order_counter);
		$ins->setAttribute("opcode", $opcode);
		$order_counter++;
	}

	// funkce na zpracovani pouze jedine instrukce, vsechny instrukce jsou v poli
	function single_instruction($opcode) {
		global $all_instructions;
		$valid_opcode = array("BREAK", "RETURN", "CREATEFRAME", "PUSHFRAME", "POPFRAME");
		if (in_array($opcode, $valid_opcode)) {
			global $program, $xml, $order_counter, $ins;
			do_another_xml_element($opcode);
			$program->appendChild($ins);
		} elseif (in_array($opcode, $all_instructions)) {
			print_error("Nevalidni instrukce.", 23);
		} else {
			print_error("Nevalidni instrukce.", 22);
		}
	}

	/* Funkce prijima jiz rozzarene instrukce se spravnym poradi operandu
		funkce na vytvoreni XML pro instrukce co prjimaji jeden operand
	*/
	function double_instruction_xml($opcode, $arg, $co) {
		global $program, $xml, $order_counter, $ins;
		do_another_xml_element($opcode);
		if ($co == "string") {
			$tmp_arg = $xml->createElement("arg1", explode("string@", $arg)[1]);
			$tmp_arg->setAttribute("type", "string");
		}
		if ($co == "label") {
			$tmp_arg = $xml->createElement("arg1", $arg);
			$tmp_arg->setAttribute("type", "label");
		}
		if ($co == "var") {
			$tmp_arg = $xml->createElement("arg1", $arg);
			$tmp_arg->setAttribute("type", "var");
		}
		if ($co == "nil") {
			$tmp_arg = $xml->createElement("arg1", "nil");
			$tmp_arg->setAttribute("type", "nil");
		}
		if ($co == "true") {
			$tmp_arg = $xml->createElement("arg1", "true");
			$tmp_arg->setAttribute("type", "bool");
		}
		if ($co == "false") {
			$tmp_arg = $xml->createElement("arg1", "false");
			$tmp_arg->setAttribute("type", "bool");
		}
		if ($co == "int") {
			$tmp_arg = $xml->createElement("arg1", explode("int@", $arg)[1]);
			$tmp_arg->setAttribute("type", "int");
		}
		$ins->appendChild($tmp_arg);
		$program->appendChild($ins);
	}

	/* Funkce prijima jiz rozzarene instrukce se spravnym poradi operandu
		funkce na vytvoreni XML pro instrukce co prjimaji dva operandy
	*/
	function triple_instruction_xml($opcode, $arg, $arg2) {
		global $program, $xml, $order_counter, $ins;
		do_another_xml_element($opcode);
		$tmp_arg = $xml->createElement("arg1", $arg);
		$tmp_arg->setAttribute("type", "var");
		$tmp = explode("@", $arg2, 2);
		if ($tmp[0] == "string") {
			$tmp_arg2 = $xml->createElement("arg2", $tmp[1]);
			$tmp_arg2->setAttribute("type", "string");
		} elseif ($tmp[0] == "int") {
			$tmp_arg2 = $xml->createElement("arg2", $tmp[1]);
			$tmp_arg2->setAttribute("type", "int");
		} elseif ($tmp[0] == "nil") {
			$tmp_arg2 = $xml->createElement("arg2", $tmp[1]);
			$tmp_arg2->setAttribute("type", "nil");
		} elseif ($tmp[0] == "bool") {
			$tmp_arg2 = $xml->createElement("arg2", $tmp[1]);
			$tmp_arg2->setAttribute("type", "bool");
		} elseif ($tmp[0] == "LF" or $tmp[0] == "TF" or $tmp[0] == "GF") {
			$tmp_arg2 = $xml->createElement("arg2", $arg2);
			$tmp_arg2->setAttribute("type", "var");
		}
		$ins->appendChild($tmp_arg);
		$ins->appendChild($tmp_arg2);
		$program->appendChild($ins);
	}

	/* Funkce prijima pole o dvou prvich
		funkce ktera nam zpracuje instrukce, ktere prijimaji jeden operand nebo label
	*/
	function double_instruction($neco) {
		global $all_instructions;
		global $jupms_cnt;
		global $uniq_labels;
		global $labels_cnt;
		$a = strtoupper($neco[0]);
		if ($a == "DEFVAR" or $a == "POPS") {
			$b = preg_match("/^(LF|GF|TF)@[a-zA-Z_+\-$&%*!?]+[\w_+\-$&%*!?]*$/", $neco[1]);
			$neco[1] = replace_all_bad_characters($neco[1]);
			if ($b) {
				double_instruction_xml($a, $neco[1], "var");
				return;
			}
		} elseif ($a == "CALL" or $a == "LABEL" or $a == "JUMP") {
			$b = preg_match("/^[a-zA-Z_+\-$&%*!?]+[\w_+\-$&%*!?]*$/", $neco[1]);
			$neco[1] = replace_all_bad_characters($neco[1]);
			if ($b) {
				double_instruction_xml($a, $neco[1], "label");
				if ($a == "JUMP") {
					$jupms_cnt++;
				}
				if ($a == "LABEL") {
					if (!in_array($neco[1], $uniq_labels)) {
						array_push($uniq_labels, $neco[1]);
					}
				}
				return;
			}
		} elseif ($a == "WRITE" or $a == "PUSHS" or $a == "EXIT" or $a == "DPRINT") {
			
			if (preg_match("/^(LF|GF|TF)@[a-zA-Z_+\-$&%*!?]+[\w_+\-$&%*!?]*$/", $neco[1])) {
				$neco[1] = replace_all_bad_characters($neco[1]);
				double_instruction_xml($a, $neco[1], "var");
				return;
			}
			if (preg_match("/^string@(\\\\\d{3,}|[^\\\\\s])*$/", $neco[1])) {
				$neco[1] = replace_all_bad_characters($neco[1]);
				double_instruction_xml($a, $neco[1], "string");
				return;
			}
			if ($neco[1] == 'nil@nil') {
				double_instruction_xml($a, $neco[1], "nil");
				return;
			}
			if ($neco[1] == 'bool@true') {
				double_instruction_xml($a, $neco[1], "true");
				return;
			}
			if ($neco[1] == 'bool@false') {
				double_instruction_xml($a, $neco[1], "false");
				return;
			}
			if (preg_match("/^int@[+-]?[\d]+$/", $neco[1])) {
				double_instruction_xml($a, $neco[1], "int");
				return;
			}
		} elseif (!in_array($a, $all_instructions)) {
			print_error("Chyba opcode", 22);
		}
		print_error("Chyba syntaxe", 23);
	}

	/* Funkce prijima pole o trech prvich
		funkce na zpracovani instrukce s dvemi operandy
	*/
	function triple_instruction($neco) {
		global $all_instructions;
		$ab = strtoupper($neco[0]);
		if ($ab == "MOVE" or $ab == "INT2CHAR" or $ab == "STRLEN" or $ab == "TYPE" or $ab == "NOT") {
			$b = preg_match("/^(LF|GF|TF)@[a-zA-Z_+\-$&%*!?]+[\w_+\-$&%*!?]*$/", $neco[1]);
			$c = preg_match("/^(((LF|GF|TF)@[a-zA-Z_+\-$&%*!?]+[\w_+\-$&%*!?]*)|(int@[+-]?[\d]+)|(bool@(true|false))|(nil@nil)|(string@(\\\\\d{3,}|[^\\\\\s])*))$/", $neco[2]);
			if ($b and $c) {
				$neco[1] = replace_all_bad_characters($neco[1]);
				$neco[2] = replace_all_bad_characters($neco[2]);
				triple_instruction_xml($ab, $neco[1], $neco[2]);
				return;
			}
		} elseif ($ab == "READ") {
			$b = preg_match("/^(LF|GF|TF)@[a-zA-Z_+\-$&%*!?]+[\w_+\-$&%*!?]*$/", $neco[1]);
			if ($b and ($neco[2] == "int" or $neco[2] == "bool" or $neco[2] == "string")) {
				$neco[1] = replace_all_bad_characters($neco[1]);
				global $program, $xml, $order_counter, $ins;
				do_another_xml_element($ab);
				$tmp_arg = $xml->createElement("arg1", $neco[1]);
				$tmp_arg->setAttribute("type", "var");
				$tmp_arg2 = $xml->createElement("arg2", $neco[2]);
				$tmp_arg2->setAttribute("type", "type");
				$ins->appendChild($tmp_arg);
				$ins->appendChild($tmp_arg2);
				$program->appendChild($ins);
				return;
			}
		} elseif (!in_array($ab, $all_instructions)) {
			print_error("Chyba opcode", 22);
		}
		print_error("Chyba syntaxe", 23);
	}

	/* Funkce prijima jiz rozzarene instrukce se spravnym poradi operandu
		funkce pro vytoreni XML, pro instrukce, ktere maji tri argumenty
	*/
	function quadro_instruction_xml($opcode, $arg, $arg2, $arg3) {
		global $program, $xml, $order_counter, $ins;
		do_another_xml_element($opcode);
		$kk = explode("@", $arg, 2);
		if ($kk[0] == "GF" or $kk[0] == "TF" or $kk[0] == "LF") {
			$tmp_arg = $xml->createElement("arg1", $arg);
			$tmp_arg->setAttribute("type", "var");
		} else {
			$tmp_arg = $xml->createElement("arg1", $arg);
			$tmp_arg->setAttribute("type", "label");
		}

		// cyklus nebo vytvoreni tridy je zbytecne, rychlejsi je rozkopirovani
		$tmp = explode("@", $arg2, 2);
		if ($tmp[0] == "string") {
			$tmp_arg2 = $xml->createElement("arg2", $tmp[1]);
			$tmp_arg2->setAttribute("type", "string");
		} elseif ($tmp[0] == "int") {
			$tmp_arg2 = $xml->createElement("arg2", $tmp[1]);
			$tmp_arg2->setAttribute("type", "int");
		} elseif ($tmp[0] == "nil") {
			$tmp_arg2 = $xml->createElement("arg2", $tmp[1]);
			$tmp_arg2->setAttribute("type", "nil");
		} elseif ($tmp[0] == "bool") {
			$tmp_arg2 = $xml->createElement("arg2", $tmp[1]);
			$tmp_arg2->setAttribute("type", "bool");
		} elseif ($tmp[0] == "LF" or $tmp[0] == "TF" or $tmp[0] == "GF") {
			$tmp_arg2 = $xml->createElement("arg2", $arg2);
			$tmp_arg2->setAttribute("type", "var");
		}

		$tmp = explode("@", $arg3, 2);
		if ($tmp[0] == "string") {
			$tmp_arg3 = $xml->createElement("arg3", $tmp[1]);
			$tmp_arg3->setAttribute("type", "string");
		} elseif ($tmp[0] == "int") {
			$tmp_arg3 = $xml->createElement("arg3", $tmp[1]);
			$tmp_arg3->setAttribute("type", "int");
		} elseif ($tmp[0] == "nil") {
			$tmp_arg3 = $xml->createElement("arg3", $tmp[1]);
			$tmp_arg3->setAttribute("type", "nil");
		} elseif ($tmp[0] == "bool") {
			$tmp_arg3 = $xml->createElement("arg3", $tmp[1]);
			$tmp_arg3->setAttribute("type", "bool");
		} elseif ($tmp[0] == "LF" or $tmp[0] == "TF" or $tmp[0] == "GF") {
			$tmp_arg3 = $xml->createElement("arg3", $arg3);
			$tmp_arg3->setAttribute("type", "var");
		}

		$ins->appendChild($tmp_arg);
		$ins->appendChild($tmp_arg2);
		$ins->appendChild($tmp_arg3);
		$program->appendChild($ins);
	}

	/* Funkce prijima pole o 4 prvcich, idealni pripad je spravna instrukce s 3 operandy
		nejslozitejsi funkce, ktera zpracovava temer vetsinu instrukci
		musi overovat vsechny typy co se mohou objevit
		zpracovava instrukce o trech argumentech
	*/
	function quadro_instruction($neco) {
		global $all_instructions;
		global $jupms_cnt;
		$a = strtoupper(trim($neco[0]));
		$prvni = array("ADD", "SUB", "MUL", "IDIV", "LT", "AND", "STRI2INT", "CONCAT", "STRLEN", "GETCHAR", "SETCHAR", "OR", "GT", "EQ");
		if (in_array($a, $prvni)) {
			$b = preg_match("/^(LF|GF|TF)@[a-zA-Z_+\-$&%*!?]+[\w_+\-$&%*!?]*$/", $neco[1]);
			$c = preg_match("/^(((LF|GF|TF)@[a-zA-Z_+\-$&%*!?]+[\w_+\-$&%*!?]*)|(int@[+-]?[\d]+)|(bool@(true|false))|(nil@nil)|(string@(\\\\\d{3,}|[^\\\\\s])*))$/", $neco[2]);
			$d = preg_match("/^(((LF|GF|TF)@[a-zA-Z_+\-$&%*!?]+[\w_+\-$&%*!?]*)|(int@[+-]?[\d]+)|(bool@(true|false))|(nil@nil)|(string@(\\\\\d{3,}|[^\\\\\s])*))$/", $neco[3]);
			if ($d and $b and $c) {
				$neco[1] = replace_all_bad_characters($neco[1]);
				$neco[2] = replace_all_bad_characters($neco[2]);
				$neco[3] = replace_all_bad_characters($neco[3]);
				quadro_instruction_xml($a, $neco[1], $neco[2], $neco[3]);
				return;
			}
		} elseif ($a == "JUMPIFNEQ" or $a == "JUMPIFEQ") {
			$b = preg_match("/[a-zA-Z_+\-$&%*!?]+[\w_+\-$&%*!?]*$/", $neco[1]);
			$c = preg_match("/^(((LF|GF|TF)@[a-zA-Z_+\-$&%*!?]+[\w_+\-$&%*!?]*)|(int@[+-]?[\d]+)|(bool@(true|false))|(nil@nil)|(string@(\\\\\d{3,}|[^\\\\\s])*))$/", $neco[2]);
			$d = preg_match("/^(((LF|GF|TF)@[a-zA-Z_+\-$&%*!?]+[\w_+\-$&%*!?]*)|(int@[+-]?[\d]+)|(bool@(true|false))|(nil@nil)|(string@(\\\\\d{3,}|[^\\\\\s])*))$/", $neco[3]);
			if ($a and $b and $c) {
				$neco[1] = replace_all_bad_characters($neco[1]);
				$neco[2] = replace_all_bad_characters($neco[2]);
				$neco[3] = replace_all_bad_characters($neco[3]);
				quadro_instruction_xml($a, $neco[1], $neco[2], $neco[3]);
				$jupms_cnt++;
				return;
			}
		} elseif (!in_array($a, $all_instructions)) {
			print_error("Chyba opcode", 22);
		}
		print_error("Chyba syntaxe", 23);
	}

	/* funkce prijima $line, coz je jeden nacteny radek a uz upraveny na spravny pocet mezer a bez komentaru
		pomocna funkce, podle ktere se rozhodneme do ktere funkce skocime na zpracovani instrukci,
		protoze v zadani je primo receno, ze instrukce musi byt s nenulovym poctem oddelovacu,
		tak jsem prvne pres regex nahradil vsechny bile znaky za jednu mezeru.
		Nasledne splitnu a spocitam kolik prvku mam v poli
	*/
	function process_line($line) {
		global $loc_cnt;
		$tmp = explode(" ", $line);
		switch (count($tmp)) {
			case 1:
				if ($tmp[0] == "") {
					break;
				}
				single_instruction(strtoupper($tmp[0]));
				$loc_cnt++;
				break;
			case 2:
				double_instruction($tmp);
				$loc_cnt++;
				break;
			case 3:
				triple_instruction($tmp);
				$loc_cnt++;
				break;
			case 4:
				quadro_instruction($tmp);
				$loc_cnt++;
				break;
			default:
				print_error("Chyba syntaxe", 23);
				break;
		}
	}

	/* pridana funkce main, protoze se takto udelalo lehceji rozsireni, kdy kod uz byl moc neprehledny
		funkce neprijima zadne parametry
	*/
	function main() {
		global $program, $xml, $order_counter, $ins;
		global $comment_cnt, $jupms_cnt, $labels_cnt, $loc_cnt;
		// nacteni prvniho radku jestli mame hlavicku
		$line = fgets(STDIN);
		// zbaveni se pripadneho komentare
		$line = explode("#", trim($line));
		if (count($line) == 2) {
				$comment_cnt++;
			}
		verify_header($line[0]);

		// vytvoreni XML dle pozadavku zadani
		$xml = new DomDocument('1.0', 'UTF-8');
		$xml->formatOutput = true;
		$program = $xml->createElement("program");
		$program->setAttribute("language", "IPPcode19");
		$xml->appendChild($program);

		// cyklus na nacteni jednotlivych radku
		while (FALSE !== ($line = fgets(STDIN))) {
			// nahrazeni bilych znaku za jednu mezeru
			// aby jsme meli vzdy "instrukce operand operand"
			$line = preg_replace('/\s+/', ' ', $line);
			// odstraneni komentare
			$a = explode("#", trim($line), 2);
			if (count($a) == 2) {
				$comment_cnt++;
			}
			process_line(trim($a[0]));
		}
		// vytisknuti vysledneho XML
		echo $xml->saveXML();
	}

	// vypsani help
	if (in_array("--help", $argv) && $argc == 2) {
		echo "Zadej: ./$argv[0] < IPPcode19\n";
		exit(0);
	}

	// pokus by se nahodou zadal help s vice argumenty
	if (in_array("--help", $argv)) {
		print_error("Spatne pouziti.\nZadej: ./$argv[0] --help", 10);
	}

	// pouze cteme instrukce ze STDIN, statistiky nas nezajimaji
	if ($argc == 1) {
		main();
	}

	$ano = False;
	$file;
	$all_possible_switches = array('--comments', '--loc', '--jumps', '--labels', $argv[0]);
	// hledani souboru pomoci regexu
	foreach ($argv as $key => $value) {
		if (preg_match('/^--stats=.+$/', $value)) {
			$ano = True;
			$file = explode("=", $value, 2)[1];
			array_push($all_possible_switches, $value);
		}
	}

	// pokud mame soubor, tak overime zda jde otevrit
	if ($ano) {
		$file = @fopen($file, "w");
		if ($file === false) {
			print_error("Nedostatecna prava na vytvoreni/zapsani do souboru", 12);
		}
		// overeni zda mame spravne argumenty zadany
		foreach ($argv as $key => $value) {
			if (!in_array($value, $all_possible_switches)) {
				print_error("Chyba argumentu", 10);
			}
		}
		main();
		// proste vypsani statistik ve spravnem poradi
		foreach ($argv as $key => $value) {
			switch ($value) {
				case '--comments':
					fwrite($file, "$comment_cnt\n");
					break;
				case '--loc':
					fwrite($file, "$loc_cnt\n");
					break;
				case '--jumps':
					fwrite($file, "$jupms_cnt\n");
					break;
				case '--labels':
					fwrite($file, count($uniq_labels) . "\n");
					break;
			}
		}
	} elseif ($argc > 1) {
		print_error("Chyba stats", 10);
	}
?>