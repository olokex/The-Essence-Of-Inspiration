<?php
    /*
     * Autor: Adam Sedlacek | xsedla1e
     * Rok: 2019
     * Projekt: test do IPP
     *
     */

    // pomocne promenne na vypocet pruchozich testu
    $test_counter = 0;
    $test_passed = 0;

    // pomocne pole pro testy co ne/prosly
    $test_list_passed = [];
    $test_list_failed = [];

    // pomocna funkce, ktera ulehcuje praci pri chybovych stavech
    function print_error($msg, $error_code) {
        fwrite(STDERR, $msg . PHP_EOL);
        exit($error_code);
    }

    /*
     * Funkce bere jako argument cestu na slozku v ktere pak hleda dalsi slozky
     * Vystupem je pole slozek
     */
    function get_list_of_dir_recursion($dir) {
        $result = [$dir];
        while($dirs = glob($dir . '/*', GLOB_ONLYDIR)) {
          $dir .= '/*';
          if(!$result) {
             $result = $dirs;
           } else {
              $result = array_merge($result, $dirs);
           }
        }
        return $result;
    }

    /*
     * Vstupem je slozka v ktere se najdou soubory potrebne pro test parseru
     * "Vystupem" je pridani do pole pro testy co ne/prosly
     * Generuji postupne soubory co jsou pripadne overuji zda existuji a daji se cist
     * Porovnam navratove hodnoty a podle toho se rozhodnu zda test prosel
     */
    function do_test_parse($path) {
        global $switches, $test_counter, $test_passed;
        global $test_list_passed, $test_list_failed;
        $path_of_src = $path . "/*.src";
        // vygeneruje pole souborů ktere konci na *.src
        $files = glob($path_of_src, GLOB_BRACE);
        foreach($files as $file) {
            $test_counter++;
            if (!is_readable($file)) {
                print_error("Nedostatecne opravneni1", 11);
            }
            //$idk = exec('php7.3 ' . $switches["parse-script"] . " < " . "$file" . " 2>&1 " . " > tmp-test-xsedla1e", $output, $status);
            $bla = $switches["parse-script"];
            $testik = "php7.3 \"$bla\" < \"$file\" 2>&1 > tmp-test-xsedla1e";
            exec($testik, $output, $status);

            $rc_file_name = explode(".src", $file)[0];
            $rc_file_path = $rc_file_name . ".rc";
            if (!is_readable($rc_file_path)) {
                $rc_file = @fopen($rc_file_path, "w");
                if ($rc_file === false) {
                    print_error("Nedostatecne opravneni2", 11);
                }

                $write_sucess = @fwrite($rc_file, "0");
                if ($write_sucess === false) {
                    print_error("Nedostatecne opravneni3", 11);
                }
            }

            $rc_file = @fopen($rc_file_path, "r");
            if ($rc_file === false) {
                    print_error("Nedostatecne opravneni4", 12);
            }

            $return_code = fgets($rc_file);
            if ($return_code != 0 && $return_code == $status) {
                $out_file = $rc_file_name . ".out";
                if (!is_readable($out_file)) {
                    $out_file = @fopen($out_file, "w");
                    if ($out_file === false) {
                        print_error("Nedostatecne opravneni5", 11);
                    }
                }
                $test_passed++;
                array_push($test_list_passed, $rc_file_name);
                continue;
            }
            if ($return_code == 0 && $return_code == $status) {
                $out_file = $rc_file_name . ".out";
                if (!is_readable($out_file)) {
                    $out_file = @fopen($out_file, "w");
                    if ($out_file === false) {
                        print_error("Nedostatecne opravneni6", 11);
                    }
                }
                // TODO opravit cestu na merlin
                $idk = @exec("java -jar /pub/courses/ipp/jexamxml/jexamxml.jar tmp-test-xsedla1e " . "\"$out_file\"", $output, $status);
                
                //$idk = @exec("java -jar jexamxml.jar tmp-test-xsedla1e $out_file", $output, $status);
                if ($status == 0) {
                    $test_passed++;
                    array_push($test_list_passed, $rc_file_name);
                    continue;
                }
            }
            $out_file = $rc_file_name . ".out";
            if (!is_readable($out_file)) {
                $out_file = @fopen($out_file, "w");
                if ($out_file === false) {
                    print_error("Nedostatecne opravneni7", 11);
                }
            }
            array_push($test_list_failed, $rc_file_name);
        }

        // smazani logu z Jexamxml a meho pomocneho souboru
        $first = $path . "/tmp-test-xsedla1e";
        if (file_exists($first)) {
            @exec("rm " . $first);
        }
        $second = $path . "/tmp-test-xsedla1e.log";
        if (file_exists($second)) {
            @exec("rm " . $second);
        }
    }

    /*
     * Vstup je cesta, ze ktere se pak hledaji *.src pro interpret
     * "Vystup" jsou testy, ktere ne/prosly
     * Postupne overuji soubory a generuji je pokud nejaky chybi
     * Overim navratove kody a podle toho se rozhodnu kam test zaradim
     */
    function do_test_interpret($path) {
        global $switches, $test_counter, $test_passed;
        global $test_list_passed, $test_list_failed;
        $path_of_src = $path . "/*.src";
        $files = glob($path_of_src, GLOB_BRACE);
        foreach($files as $file) {
            $test_counter++;
            if (!is_readable($file)) {
                print_error("Nedostatecne opravneni", 11);
            }
            $path_of_in = explode(".src", $file, 2)[0];
            $path_of_in .= ".in";
            if (!is_readable($path_of_in)) {
                $path_of_in = @fopen($path_of_in, "w");
                if ($path_of_in == false) {
                    print_error("Nedostatecne opravneni1", 11);
                }
            }
            
            $path_of_rc = explode(".src", $file, 2)[0];
            $path_of_rc .= ".rc";
            if (!is_readable($path_of_rc)) {
                $path_of_in = @fopen($path_of_rc, "w");
                if ($path_of_rc == false) {
                    print_error("Nedostatecne opravneni2", 11);
                }
                $write_sucess = @fwrite($path_of_rc, "0");
                // if ($write_sucess === false) {
                //     print_error("Nedostatecne opravneni3", 11);
                // }
            }

            $rc_file = @fopen($path_of_rc, "r");
            if ($rc_file === false) {
                    print_error("Nedostatecne opravneni4", 11);
            }
            $return_code = fgets($rc_file);

            $path_int = $switches["int-script"];
            $idk = @exec("python3.6 " . "\"$path_int\"" . " --source=" . "\"$file\"" . " --input=" . "\"$path_of_in\"" . " > tmp-test-xsedla1e", $output, $status);

            if ($status == 0 && $return_code == $status) {
                $path_of_out = explode(".src", $file, 2)[0];
                $path_of_out .= ".out";
                if (!is_readable($path_of_out)) {
                    $path_out = @fopen($path_of_out, "w");
                    if ($path_out == false) {
                        print_error("Nedostatecne opravneni5", 11);
                    }
                }
                $idk = @exec("diff tmp-test-xsedla1e " . "\"$path_of_out\"", $output, $status);
                if ($status == 0) {
                    $test_passed++;
                    array_push($test_list_passed, explode(".src", $file, 2)[0]);
                    continue;
                }
            }
            if ($return_code == $status) {
                $test_passed++;
                array_push($test_list_passed, explode(".src", $file, 2)[0]);
                continue;
            }
            array_push($test_list_failed, explode(".src", $file, 2)[0]);
        }

        // smazani docasnych souboru ktere potrebuji k testovani
        $first = $path . "/tmp-test-xsedla1e";
        if (file_exists($first)) {
            @exec("rm " . $first);
        }
    }


    /*
     * Vstupem je float hodnota, kolik testu proslo
     * Funkce tiskne CSS/HTML na vystup STDIN
     */
    function print_html_header($passed, $count) {
    print('
<!DOCTYPE html>
<html>
<head>

<style>
    //@import url(\'https://fonts.googleapis.com/css?family=Source+Sans+Pro:300\');
    //@import url(\'https://fonts.googleapis.com/css?family=Source+Sans+Pro:600\');

    body {
        background-color: #f9f9f4;
    }

    h1, h2 {
        color: #333300;
        font-family: \'Source Sans Pro\', sans-serif;
        font-weight: 300;
    }

    h2 {
        font-size: 1.2em;
        border-bottom: 1px solid;
    }

    .heading {
        width: 800px;
        padding-top: 1.2em;
        margin: auto;
    }

    .main {
        background-color: rgba(77, 56, 0, 0.05);
        font-family: \'Source Sans Pro\', sans-serif;
        font-weight: 600;
        padding: 10px;
        width: 800px;
        margin: auto;
        padding-left: 0px;
        padding-right: 0px;
        border-radius: 15px;
        justify-content: space-evenly;
        border: 1px solid;
        border-color: #e5e5e0;
    }

    .bad-bold {
        color: #ff471a;

    }

    .ok-bold {
        color: #2ba52b;
    }

    .bad-bold,
    .ok-bold {
        padding-left: 5px;
        width: 100px;
    }

    .bad,
    .ok {
        font-weight: 300;
        width: 700px;
        padding-left: 10px;
    }

    .even {
        background-color: rgba(77, 56, 0, 0.1);
        margin: auto;
        padding-left: 0;
        padding-right: 0;
        //border: 1px solid;
    }

    .wrapper-lines {
        justify-content: space-between;
        display: flex;
        //border: 1px solid;
    }

</style>

</head>
<body>
    <div class="heading">
        <h1>IPP test output</h1>
        <h2>Passed: ' . $passed . '/' . $count . '</h2>
    </div>
    <div class="main">
');

    }

    // Pomocna funkce pro generovani HTML
    function bad_even_html($filename) {
        echo "<div class=\"even wrapper-lines\">\n";
        echo "<div class=\"bad\">$filename</div>\n";
        echo "<div class=\"bad-bold\">FAILED</div>\n";
        echo "</div>\n";
    }

    // Pomocna funkce pro generovani HTML
    function bad_html($filename) {
        echo "<div class=\"wrapper-lines\">\n";
        echo "<div class=\"bad\">$filename</div>\n";
        echo "<div class=\"bad-bold\">FAILED</div>\n";
        echo "</div>\n";
    }

    // Pomocna funkce pro generovani HTML
    function ok_html($filename) {
        echo "<div class=\"wrapper-lines\">\n";
        echo "<div class=\"ok\">$filename</div>\n";
        echo "<div class=\"ok-bold\">OK</div>\n";
        echo "</div>\n";
    }

    // Pomocna funkce pro generovani HTML
    function ok_even_html($filename) {
        echo "<div class=\"even wrapper-lines\">\n";
        echo "<div class=\"ok\">$filename</div>\n";
        echo "<div class=\"ok-bold\">OK</div>\n";
        echo "</div>\n";
    }

    /*
     * Funkce neprijima zadny parametr a "vystupem" je spravne vygenerovany HTML kod,
     * tak aby byl lehko citelny pro člověka, který jej kontroluje
     *
     * Pro lepsi citelnost jsem zvyraznil sude radky
     * Taktez jsou oznaceny znackou "OK" nebo "FAILED" a barvou, ktera napovi
     */
    function do_main_stuff() {
        global $test_counter, $test_passed;
        global $test_list_passed, $test_list_failed;

        // vypocet procent uspenosti testu
        if ($test_counter != 0) {
            $percent = $test_passed / $test_counter * 100;
            $percent = number_format($percent, 2);
        } else {
            $percent = 0;
        }

        print_html_header($test_passed, $test_counter);
        $big_array = array_merge($test_list_passed, $test_list_failed);
        for ($i=0; $i < $test_counter; $i++) {
         if ($i % 2 == 0) {
             if ($i < count($test_list_passed)) {
                 ok_even_html($big_array[$i]);
             } else {
                 bad_even_html($big_array[$i]);
             }
         } else {
             if ($i < count($test_list_passed)) {
                 ok_html($big_array[$i]);
             } else {
                 bad_html($big_array[$i]);
             }
         }
        }

        // tisk konce HTML
        echo "</div>\n";
        echo "</body>\n";
        echo "</html>\n";
        exit(0);
    }

    /*
     * Funkce bere argument cestu, v ktere pak vyhleda vsechny *.src soubory
     * "vystupem" je pole testu, ktere ne/prosly, ktere se rozzradi podle navratove hodnoty
     * Vyuzil jsem funkci parse a interpret, ktere jsem pouze zkopiroval a lehce upravil
     */
    function do_test_both($path) {
        global $switches, $test_counter, $test_passed;
        global $test_list_passed, $test_list_failed;

        $path_of_src = $path . "/*.src";
        $files = glob($path_of_src, GLOB_BRACE);
        foreach($files as $file) {
            $test_counter++;
            //echo $file;
            if (!is_readable($file)) {
                print_error("Nedostatecne opravneni", 11);
            }
            //$idk = exec('php7.3 ' . $switches["parse-script"] . " < " . "$file" . " 2>&1 " . " > tmp-test-xsedla1e", $output, $status);
            $bla = $switches["parse-script"];
            $testik = "php7.3 " . "\"$bla\"" . " < " . "\"$file\"" . " 2>&1 > tmp-test-xsedla1e";
            exec($testik, $output, $status);
            
            if ($status != 0) {
                $rc_file_name = explode(".src", $file)[0];
                $rc_file_path = $rc_file_name . ".rc";
                if (!is_readable($rc_file_path)) {
                    $rc_file = @fopen($rc_file_path, "w");
                    if ($rc_file === false) {
                        print_error("Nedostatecne opravneni", 11);
                    }

                    $write_sucess = @fwrite($rc_file, "0");
                    if ($write_sucess === false) {
                        print_error("Nedostatecne opravneni", 11);
                    }
                }
                
                $rc_file = @fopen($rc_file_path, "r");
                $return_code = fgets($rc_file);
                if ($return_code == $status) {
                    $test_passed++;
                    array_push($test_list_passed, $rc_file_name);
                    continue;
                }

                array_push($test_list_failed, $rc_file_name);
                continue;
            }

// tady interpret

            $path_of_in = explode(".src", $file, 2)[0];
            $path_of_in .= ".in";
            if (!is_readable($path_of_in)) {
                $path_of_in = @fopen($path_of_in, "w");
                if ($path_of_in == false) {
                    print_error("Nedostatecne opravneni", 11);
                }
            }
            
            $path_of_rc = explode(".src", $file, 2)[0];
            $path_of_rc .= ".rc";
            if (!is_readable($path_of_rc)) {
                $path_of_in = @fopen($path_of_rc, "w");
                if ($path_of_rc == false) {
                    print_error("Nedostatecne opravneni", 11);
                }
                $write_sucess = @fwrite($path_of_rc, "0");
                if ($write_sucess === false) {
                    print_error("Nedostatecne opravneni", 11);
                }
            }

            $rc_file = @fopen($path_of_rc, "r");
            if ($rc_file === false) {
                    print_error("Nedostatecne opravneni", 11);
            }
            $return_code = fgets($rc_file);
            $path_int = $switches["int-script"];
            $idk = @exec("python3.6 " . "\"$path_int\"" . " --source=tmp-test-xsedla1e " . "--input=" . "\"$path_of_in\"" . " > tmp-test-int-xsedla1e", $output, $status);
            //$idk = @exec("python3.6 " . $switches["int-script"] . " --source=tmp-test-xsedla1e < $path_of_in > tmp-test-int-xsedla1e", $output, $status);

            if ($status == 0 && $return_code == $status) {
                $path_of_out = explode(".src", $file, 2)[0];
                $path_of_out .= ".out";
                $idk = @exec("diff tmp-test-int-xsedla1e " . "\"$path_of_out\"", $output, $status);
                if ($status == 0) {
                    $test_passed++;
                    array_push($test_list_passed, explode(".src", $file, 2)[0]);
                    continue;
                }
            }
            if ($return_code == $status) {
                $test_passed++;
                array_push($test_list_passed, explode(".src", $file, 2)[0]);
                continue;
            }
            array_push($test_list_failed, explode(".src", $file, 2)[0]);
        }

        // uklid docasnych souboru, ktere vyuzivam pri testovani
        $first = $path . "/tmp-test-xsedla1e";
        if (file_exists($first)) {
            @exec("rm " . $first);
        }
        $second = $path . "/tmp-test-int-xsedla1e";
        if (file_exists($second)) {
            @exec("rm " . $second);
        }
    }

    // overeni zda se zadal pouze jeden help a vypsani napovedy
    if ($argc == 2 && $argv[1] == "--help") {
        echo "Program muze byt spusten s nasledujicimi prepinaci\n\n";
        echo "--directory=PATH hleda v danem adresari, pokud chybi bere soubory z aktualniho adresare\n";
        echo "--recursive hleda rekurzivne v podadresarich\n";
        echo "--parse-script=FILE cesta k parse.php, pokud neni zadano bere aktualni adresar\n";
        echo "--int-script=FILE cesta k interpret.py, pokud neni zadano bere aktualni adresar\n";
        echo "--parse-only bude testovat pouze parse.php (nesmi se skombinovat s --int-only\n";
        echo "--int-only bude testovat pouze interpret.py (nesmi se skombinovat s --parse-only\n";
        exit(0);
    }

    // vice prepinacu s help je chyba
    if (in_array("--help", $argv)) {
        print_error("Spatne pouziti.\nZadej: ./$argv[0] --help", 10);
    }

    // pomocne pole, do kterych ukladam hodnoty z argumentu
    $switches = [
        "directory" => getcwd(),
        "parse-script" => getcwd() . "/parse.php",
        "int-script" => getcwd() . "/interpret.py",
        "recursive" => False,
        "parse-only" => False,
        "int-only" => False
    ];

    // vlastni parsovani argumentu, dvoji zadani neberu jako chybu, a je brano posledni
    foreach ($argv as $key => $value) {
        if (preg_match('/^--directory=.*$/', $value)) {
            $switches["directory"] = explode("=", $value, 2)[1];
        }
        elseif (preg_match('/^--parse-script=.*$/', $value)) {
            $switches["parse-script"] = explode("=", $value, 2)[1];
        }
        elseif (preg_match('/^--int-script=.*$/', $value)) {
            $switches["int-script"] = explode("=", $value, 2)[1];
        }
        elseif ($value == "--recursive") {
            $switches["recursive"] = True;
        }
        elseif ($value == "--parse-only") {
            $switches["parse-only"] = True;
        }
        elseif ($value == "--int-only") {
            $switches["int-only"] = True;
        } elseif ($value != $argv[0]) {
            print_error("Nejedna se o validni prepinace", 10);
        }
    }

    //print_r($switches);

    if ($switches["int-only"] && $switches["parse-only"]) {
        print_error("Prepinace --int-only a --parse-only se nesmi kombinovat", 10);
    }

    if (!is_dir($switches["directory"])) {
        print_error("Neplatna cesta --directory=PATH", 11);
    }

    if (!is_readable($switches["parse-script"]) && file_exists($switches["parse-script"])) {
        print_error("Neplatny soubor --parse-script=FILE", 11);
    }

    if (!is_readable($switches["int-script"]) && file_exists($switches["int-script"])) {
        print_error("Neplatny soubor --int-script=FILE", 11);
    }

    if ($switches["parse-only"] && $switches["recursive"] == true) {
        $result = get_list_of_dir_recursion($switches["directory"]);
        foreach ($result as $key) {
            do_test_parse($key);
        }
        do_main_stuff();
    }

    if ($switches["parse-only"] && $switches["recursive"] == false) {
        do_test_parse($switches["directory"]);
        do_main_stuff();
    }

    if ($switches["int-only"] && $switches["recursive"] == false) {
        do_test_interpret($switches["directory"]);
        do_main_stuff();
    }

    if ($switches["int-only"] && $switches["recursive"] == true) {
       $result = get_list_of_dir_recursion($switches["directory"]);
        foreach ($result as $key) {
            do_test_interpret($key);
        }
        do_main_stuff();
    }

    if ($switches["recursive"] == true) {
        $result = get_list_of_dir_recursion($switches["directory"]);
        foreach ($result as $key) {
            do_test_both($key);
        }
        do_main_stuff();
    }

    do_test_both($switches["directory"]);
    do_main_stuff();

?>