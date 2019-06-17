#!/usr/bin/env python3.6

# autor: xsedla1e
# IPP interpret 2019

import sys
import re
import xml.etree.ElementTree as ET
from os import path, _exit

source_file = None
input_file = None

# TODO smazat pomocny print na xml
def printtree(t, indent=0):
    print("  "*indent, t, t.items())
    for c in t.getchildren():
        printtree(c, indent+1)

# Lorem Ipsum
def print_error(msg, code):
    sys.stderr.write(msg + "\n")
    _exit(code) # spadne :)

# Lorem Ipsum
def print_help():
    print(f"Uziti: {sys.argv[0]} --help")
    print(f"Uziti: {sys.argv[0]} --source=FILE")
    print(f"Uziti: {sys.argv[0]} --input=FILE")
    print(f"Prepinac help se NESMI kombinovat.")
    print(f"Pokud INPUT nebo SOURCE chybi nacita se z STDIN")
    sys.exit(0)

# Lorem Ipsum
if len(sys.argv) == 2 and sys.argv[1] == "--help":
    print_help()

# Lorem Ipsum
for each in sys.argv:
    if re.search("^--input=.+$", each):
        input_file = re.sub("^--input=", "", each, 0)
    elif re.search("^--source=.+$", each):
        source_file = re.sub("^--source=", "", each, 0)
    elif each == "--help":
        print_error("Neplatny --help", 10)
    elif each != sys.argv[0]:
        print_error("Spatny prepinac", 10)

# Lorem Ipsum
if not input_file and not source_file:
    print_error("Alespon jeden prepinac na soubory musi byt zadan", 10)

# Lorem Ipsum
def read_file(filename, errmsg):
    error = False
    fp = None
    try:
        fp = open(filename, 'r') if filename else sys.stdin
        text = fp.read()
    except:
        error = True
    finally:
        # TODO udelat nacitani pres stdin na file_input
        if fp and fp is not sys.stdin:
            fp.close()
    if error:
        print_error(errmsg, 11)
    return text

# Lorem Ipsum
source_text = read_file(source_file, 'Neplatny soubor source')

# Lorem Ipsum
input_fh = sys.stdin
if input_file:
    fileerr = False
    # try:
    #     input_fh = open(input_file, 'r')
    # except:
    #     fileerr = True
    # if fileerr:
    #     print_error('Nejde otevrit vstupni soubor!!!', 11)

    input_fh = open(input_file, 'r')

#    fileerr = True
    if fileerr:
        print_error('Nejde otevrit vstupni soubor!!!', 11)

# Lorem Ipsum
try:
    parsed_xml = ET.fromstring(source_text)
except Exception:
    print_error("Nevalidni format xml", 31)

if parsed_xml.tag.upper() != "PROGRAM":
    print_error("Nebyl nalezen atribut program", 32)

# Lorem Ipsum
try:
    if parsed_xml.attrib["language"].upper() != "IPPCODE19":
        print_error("Nesedi programovy kod IPPcode19", 32)
except Exception:
    print_error("Nevalidni atribut language", 32)

# sort the first layer
try:
    parsed_xml[:] = sorted(parsed_xml, key=lambda child: (child.tag, int(child.get('order'))))
    #sort the second layer
    for c in parsed_xml:
        c[:] = sorted(c, key=lambda child: (child.tag))
except Exception:
    print_error("Chybny atribut order", 32)


# xmlstr = ET.tostring(parsed_xml, encoding="utf-8", method="xml")
# print(xmlstr.decode("utf-8"))
try:
    for each, m in parsed_xml.items():
        if each not in ["language", "name", "description"]:
            print_error("Spatny atribut", 32)
    
    counter = 1
    for i in range(0, len(parsed_xml)):
        if parsed_xml[i].tag != "instruction":
            print_error("Chyba xml tagu instruction", 32)

        a = parsed_xml[i].attrib

        if set(a.keys()) != set(['opcode', 'order']):
            print_error("neznamy atribut v instruction", 32)

        if int(parsed_xml[i].get("order")) != counter:
            print_error("spatne serazeny order", 32)
        counter += 1

        for n in range(0, len(parsed_xml[i])):
            k = n + 1
            
            a = parsed_xml[i][n].items()
            for each, m in a:
                if each != "type":
                    print_error("Pouze atribut type je povoleny", 32)
                if m not in ["string", "label", "var", "nil", "bool", "int", "type"]:
                    print_error('Chybny type="X"', 32)

            if f"arg{k}" != parsed_xml[i][n].tag:
                #print(f"arg{k}", parsed_xml[i][n].tag, k)
                print_error("spatne ocislovany argumenty", 32)
except Exception:
    print_error("neco spatne", 32)

class Argument(object):
    def __init__(self, type, value):
        self.type = type
        self.value = value

    def __str__(self):
        return self.type + ":" + self.value

class Instruction(object):
    def __init__(self, opcode):
        self.opcode = opcode
        self.args = []

    def add_arg(self, type, value):
        self.args.append(Argument(type, value))

    def __str__(self):
        return self.opcode + " " + " ".join(str(a) for a in self.args)

    def len(self):
        return len(self.args)

# instruction = Instruction('ADD')
# instruction.add_arg('SYMBOL', 'neco')
# instruction.add_arg('SYMBOL', 'necojinyho')
# instructions.append(instruction)

every_instruction = ["DEFVAR", "POPS", "WRITE", "PUSHS", "EXIT", "DPRINT", "CALL",
                     "LABEL", "JUMP", "MOVE", "INT2CHAR", "STRLEN", "TYPE", "READ",
                     "BREAK", "RETURN", "CREATEFRAME", "PUSHFRAME", "POPFRAME",
                     "ADD", "SUB", "MUL", "IDIV", "LT", "AND", "STRI2INT", "CONCAT",
                     "GETCHAR", "SETCHAR", "OR", "NOT", "GT", "EQ", "JUMPIFEQ", "JUMPIFNEQ"
                    ]

# Lorem Ipsum
instructions = []

# Lorem Ipsum
def preg_match(pattern, string):
    return bool(re.match(pattern, string))

# Lorem Ipsum
reg_var = r"^(LF|GF|TF)@[a-zA-Z_+\-$&%*!?]+[\w_+\-$&%*!?]*$"
reg_label = r"^[a-zA-Z_+\-$&%*!?]+[\w_+\-$&%*!?]*$"
reg_string = r"^(\\\d{3,}|[^\\\s])*$"
reg_int = r"^[+-]?[\d]+$"
reg_nil = r"^nil$"
reg_bool = r"^true|false$"

# Lorem Ipsum
def fits(type, val):
    types = ["var", "nil", "bool", "int", "string"]
    match_reg = [reg_var, reg_nil, reg_bool, reg_int, reg_string]
    try:
        for i in range(0, len(types)):
            if type == types[i] and preg_match(match_reg[i], val):
                return True
        return False
    except:
        if val == None and type == "string":
            return True

# Lorem Ipsum
for i in range(0, len(parsed_xml)):
    args = parsed_xml[i]
    op_code = parsed_xml[i].attrib["opcode"].upper()

    if op_code not in every_instruction:
        print_error("neni instrukce platna", 32)

    if op_code in ["BREAK", "RETURN", "CREATEFRAME", "PUSHFRAME", "POPFRAME"] and len(args) == 0:
        instructions.append(Instruction(op_code))
        continue
    elif op_code in ["DEFVAR", "POPS"] and len(args) == 1:
        arg1_type = args[0].attrib["type"]
        arg1_val = args[0].text
        if arg1_type == "var":
            if preg_match(reg_var, arg1_val):
                ins = Instruction(op_code)
                ins.add_arg(arg1_val, "None")
                instructions.append(ins)
                continue
    elif op_code in ["CALL", "LABEL", "JUMP"] and len(args) == 1:
        arg1_type = args[0].attrib["type"]
        arg1_val = args[0].text
        if arg1_type == "label":
            if preg_match(reg_label, arg1_val):
                ins = Instruction(op_code)
                ins.add_arg(arg1_type, arg1_val)
                instructions.append(ins)
                continue
    elif op_code in ["WRITE", "PUSHS", "EXIT", "DPRINT"] and len(args) == 1:
        arg1_type = args[0].attrib["type"]
        arg1_val = args[0].text
        if fits(arg1_type, arg1_val):
            ins = Instruction(op_code)
            ins.add_arg(arg1_type, arg1_val)
            instructions.append(ins)
            continue
    elif op_code in ["MOVE", "INT2CHAR", "STRLEN", "TYPE", "NOT"] and len(args) == 2:
        arg1_type = args[0].attrib["type"]
        arg1_val = args[0].text
        arg2_type = args[1].attrib["type"]
        arg2_val = args[1].text
        if arg1_type == "var" and fits(arg2_type, arg2_val):
            if preg_match(reg_var, arg1_val):
                ins = Instruction(op_code)
                if arg1_val == None:
                    arg1_val = ""
                if arg2_val == None:
                    arg2_val = ""
                ins.add_arg(arg1_type, arg1_val)
                ins.add_arg(arg2_type, arg2_val)
                instructions.append(ins)
                continue
    elif op_code == "READ" and len(args) == 2:
        arg1_type = args[0].attrib["type"]
        arg1_val = args[0].text
        arg2_type = args[1].attrib["type"]
        arg2_val = args[1].text
        if arg1_type == "var" and preg_match(reg_var, arg1_val):
            if arg2_type == "type" and arg2_val in ["string", "int", "bool"]:
                ins = Instruction(op_code)
                ins.add_arg(arg1_type, arg1_val)
                ins.add_arg(arg2_type, arg2_val)
                instructions.append(ins)
                continue
    elif op_code in ["ADD", "SUB", "MUL", "IDIV", "LT", "AND", "STRI2INT", "CONCAT",
                     "GETCHAR", "SETCHAR", "OR", "GT", "EQ"] and len(args) == 3:
        arg1_type = args[0].attrib["type"]
        arg1_val = args[0].text
        arg2_type = args[1].attrib["type"]
        arg2_val = args[1].text
        arg3_type = args[2].attrib["type"]
        arg3_val = args[2].text
        if arg1_type == "var" and preg_match(reg_var, arg1_val):
            if fits(arg2_type, arg2_val) and fits(arg3_type, arg3_val):
                ins = Instruction(op_code)
                if arg2_val == None:
                    arg2_val = ""
                if arg3_val == None:
                    arg3_val = ""
                ins.add_arg(arg1_type, arg1_val)
                ins.add_arg(arg2_type, arg2_val)
                ins.add_arg(arg3_type, arg3_val)
                instructions.append(ins)
                continue
    elif op_code in ["JUMPIFNEQ", "JUMPIFEQ"] and len(args) == 3:
        arg1_type = args[0].attrib["type"]
        arg1_val = args[0].text
        arg2_type = args[1].attrib["type"]
        arg2_val = args[1].text
        arg3_type = args[2].attrib["type"]
        arg3_val = args[2].text
        if arg1_type == "label" and preg_match(reg_label, arg1_val):
            if fits(arg2_type, arg2_val) and fits(arg3_type, arg3_val):
                ins = Instruction(op_code)
                if arg2_val == None:
                    arg2_val = ""
                if arg3_val == None:
                    arg3_val = ""
                ins.add_arg(arg1_type, arg1_val)
                ins.add_arg(arg2_type, arg2_val)
                ins.add_arg(arg3_type, arg3_val)
                instructions.append(ins)
                continue
    print_error("Chybna syntaxe", 32)

# Lorem Ipsum
all_labels = {}
tf_pushed = False
frames = { 'LF': [], 'GF': {}, 'TF': {} }
stack = []

for i in range(0, len(instructions)):
    if instructions[i].opcode == "LABEL":
        val = instructions[i].args[0].value
        if val in all_labels:
            print_error(f"Redefinice navesti: {val}", 52)
        # TODO potencionalni chyba i + 1
        all_labels[val] = i

# Lorem Ipsum
import traceback
def storevar(var, value, check_defined=True):
    if type(value) != type((1,)):
        print('!!!!!!!!!!!!!!!!!!!!!!!!!!!!to neni tupla', file=sys.stderr)
        traceback.print_stack()
        raise Exception()

    varframe, varname = var.split('@')
    frame = None
    if varframe == 'TF':
        if tf_pushed:
            print_error('Nedefinovany TF ramec storevar', 55)
        frame = frames['TF']
    elif varframe == 'LF':
        if not frames['LF']:
            print_error('Nedefinovany LF ramec storevar', 55)
        frame = frames['LF'][-1]
    else: # GF
        frame = frames['GF']

    if check_defined and varname not in frame:
        print_error('Nedefinovana promenna storevar', 54)

    frame[varname] = value

# Lorem Ipsum
def getvar(var):
    varframe, varname = var.split('@')
    frame = None
    if varframe == 'TF':
        if tf_pushed:
            print_error('Nedefinovany TF ramec getvar', 55)
        frame = frames['TF']
    elif varframe == 'LF':
        if not frames['LF']:
            print_error('Nedefinovany LF ramec getvar', 55)
        frame = frames['LF'][-1]
    else: # GFskoro 
        frame = frames['GF']

    if varname not in frame:
        print_error('Nedefinovana promenna getvar', 54)

    return frame[varname]

# Lorem Ipsum
def defvar(var):
    storevar(var, (None, None), False)
#    try:
#        frame, name = var.split("@", 1)
#
#        if frame == "LF":
#            if name in frames[frame][-1]:
#                raise ZeroDivisionError
#            if len(frames[frame]) == 0:
#                raise IndexError
#            frames[frame][-1][name] = (None, None)
#        else:
#            if name in frames[frame]:
#                raise ZeroDivisionError
#            if name in frames[frame]:
#                raise IndexError
#            frames[frame][name] = (None, None)
#
#    except ZeroDivisionError:
#        print_error(f"Redefinice promenne: {name}", 52)
#    except IndexError:
#        print_error("Neexistuje LF ramec", 1)
#    except:
#        print_error("Ramec je problem", 1)

# Lorem Ipsum
def setvar(var, value):
    storevar(var.value, (value.type, value.value))
#    frame, name = var.value.split("@", 1)
#    try:
#        if frame == 'LF':
#            exist, _ = frames[frame][-1][name]
#        else:
#            exist, _ = frames[frame][name]
#
#        if frame == 'LF':
#            frames[frame][-1][name] = (value.type, value.value)
#        else:
#            frames[frame][name] = (value.type, value.value)
#    except Exception as e:
#        print_error(f"Prvne pouzij DEFVAR {var.value}", 53)

# Lorem Ipsum
def writevar(var):
    if var.type == "var":
        value = getvar(var.value)[1]
#        frame, name = var.value.split("@", 1)
#        try:
#            if frame == "GF":
#                value = frames[frame][name][1]
#            elif frame == "TF" and tf_pushed == False:
#                value = frames[frame][name][1]
#            else:
#                value = frames[frame][0][name][1]
#        except:
#            print_error("Spatny ramec promenne", 55)
        
        if value == None:
           print_error("Neexistujici promenna writevar", 55)
    else:
        value = var.value
        if value == "nil":
            value = ""
    print(value, end="")
    sys.stdout.flush()

# Lorem Ipsum
def getint(x):
    hodnota = False
    try:
        xtype, xvalue = getvar(x.value)
        if xtype == "int":
            return int(xvalue)
        hodnota = True
    except:
        print_error(f"Promenna neexistuje {name}", 2)
    if hodnota:
        raise NameError

# Lorem Ipsum
def add(store, arg1, arg2):
    try:
        if arg1.type == "int":
            a = int(arg1.value)
        if arg2.type == "int":
            b = int(arg2.value)
        if arg1.type == "var":
            a = getint(arg1)
        if arg2.type == "var":
            b = getint(arg2)
        value = a + b
        storevar(store.value, ("int", str(value)))
    except ValueError:
        print_error("Nevalidni typ pro add", 53)
    except NameError:
        print_error("Nejedna se o typ INT", 53)
    except:
        print_error("Neexistujici promena +", 54)

# Lorem Ipsum
def sub(store, arg1, arg2):
    frame, name = store.value.split("@", 1)
    try:
        if arg1.type == "int":
            a = int(arg1.value)
        if arg2.type == "int":
            b = int(arg2.value)
        if arg1.type == "var":
            a = getint(arg1)
        if arg2.type == "var":
            b = getint(arg2)
        value = a - b
        storevar(store.value, ("int", str(value)))
    except ValueError:
        print_error("Nevalidni typ pro add", 53)
    except NameError:
        print_error("Nejedna se o typ INT", 53)
    except:
        print_error("Neexistujici promena -", 54)

# Lorem Ipsum
def mul(store, arg1, arg2):
    frame, name = store.value.split("@", 1)
    try:
        if arg1.type == "int":
            a = int(arg1.value)
        if arg2.type == "int":
            b = int(arg2.value)
        if arg1.type == "var":
            a = getint(arg1)
        if arg2.type == "var":
            b = getint(arg2)
        value = a * b
        storevar(store.value, ("int", str(value)))
    except ValueError:
        print_error("Nevalidni typ pro add", 53)
    except NameError:
        print_error("Nejedna se o typ INT", 53)
    except:
        print_error("Neexistujici promena *", 54)

# Lorem Ipsum
def div(store, arg1, arg2):
    frame, name = store.value.split("@", 1)
    try:
        if arg1.type == "int":
            a = int(arg1.value)
        if arg2.type == "int":
            b = int(arg2.value)
        if arg1.type == "var":
            a = getint(arg1)
        if arg2.type == "var":
            b = getint(arg2)
        value = a // b
        storevar(store.value, ("int", str(value)))
    except ValueError:
        print_error("Nevalidni typ pro add", 53)
    except NameError:
        print_error("Nejedna se o typ INT", 53)
    except ZeroDivisionError:
        print_error("Deleni nulou bohuzel neni podporovano :(", 57)
    except:
        print_error("Neexistujici promena /", 54)

# Lorem Ipsum
def pushs(arg):
    if arg.type == "var":
        try:
            frame, name = arg.value.split("@", 1)
            typ, val = getvar(arg.value)
            if val == None:
                print_error("Promenna bez hodnoty pro pushs", 54)
            stack.append((typ, val))
        except KeyError:
            print_error("Neni frame", 55)
    else:
        stack.append((arg.type, arg.value))

# Lorem Ipsum
def pops(arg):
    try:
        storevar(arg.type, stack.pop())
    except KeyError:
        print_error("Nejedna se o platny ramec", 55)
    except Exception as e:
        print_error("Zasobnik je prazdny", 56)

# Lorem Ipsum
def eq(store, arg1, arg2):
    try:
        if arg1.type == "var":
            frame, name = arg1.value.split("@", 1)
            a = getvar(arg1.value)
            atype = a[0]
            aval = a[1]
        else:
            atype = arg1.type
            aval = arg1.value

        if arg2.type == "var":
            frame, name = arg2.value.split("@", 1)
            b = getvar(arg2.value)
            btype = b[0]
            bval = b[1]
        else:
            btype = arg2.type
            bval = arg2.value
        
        if atype == btype:
            val = aval == bval
        elif atype == "nil" or btype == "nil":
            val = False
        else:
            raise ZeroDivisionError;
        
        if val:
            val = ("bool", "true")
        else:
            val = ("bool", "false")
        storevar(store.value, val)
    except ZeroDivisionError:
        print_error("Nejedna se o kompatibilni typy pro EQ", 53)
    except:
        print_error("Problem s promenyma ramcu EQ", 55)

# Lorem Ipsum
def lt(store, arg1, arg2):
    try:
        if arg1.type == "var":
            a = getvar(arg1.value)
            atype = a[0]
            aval = a[1]
        else:
            atype = arg1.type
            aval = arg1.value

        if arg2.type == "var":
            b = getvar(arg2.value)
            btype = b[0]
            bval = b[1]
        else:
            btype = arg2.type
            bval = arg2.value
        
        if atype == btype and atype != "nil":
            val = aval < bval
        elif atype == btype and atype == "bool":
            val = aval > bval
        elif atype == "nil" or btype == "nil":
            raise ZeroDivisionError;
        else:
            print_error("Nejedna se o kompatibilni typy pro LT", 53)
        
        if val:
            val = ("bool", "true")
        else:
            val = ("bool", "false")
        storevar(store.value, val)
    except ZeroDivisionError:
        print_error("Nesmi se porovnavat s NIL LT", 53)
    except:
        print_error("Problem s promenyma ramcu LT", 55)

# Lorem Ipsum
def gt(store, arg1, arg2):
    try:
        if arg1.type == "var":
            # frame, name = arg1.value.split("@", 1)
            # a = frames[frame][name]
            a = getvar(arg1.value)
            atype = a[0]
            aval = a[1]
        else:
            atype = arg1.type
            aval = arg1.value

        if arg2.type == "var":
            # frame, name = arg2.value.split("@", 1)
            # b = frames[frame][name]
            b = getvar(arg2.value)
            btype = b[0]
            bval = b[1]
        else:
            btype = arg2.type
            bval = arg2.value
        
        if atype == btype and atype != "nil":
            val = aval > bval
        elif atype == btype and atype == "bool":
            val = aval < bval
        elif atype == "nil" or btype == "nil":
            raise ZeroDivisionError;
        else:
            print_error("Nejedna se o kompatibilni typy pro GT", 53)
        
        if val:
            val = ("bool", "true")
        else:
            val = ("bool", "false")
        # frame, name = store.value.split("@", 1)
        # frames[frame][name] = val
        # a tohle nahrad
        storevar(store.value, val)
    except ZeroDivisionError:
        print_error("Nesmi se porovnavat s NIL GT", 53)
    except:
        print_error("Problem s promenyma ramcu GT", 55)

# Lorem Ipsum
def booland(store, arg1, arg2):
    try:
        if arg1.type == "var":
            # frame, name = arg1.value.split("@", 1)
            # a = frames[frame][name]
            a = getvar(arg1.value)
            atype = a[0]
            aval = a[1]
        else:
            atype = arg1.type
            aval = arg1.value

        if arg2.type == "var":
            # frame, name = arg2.value.split("@", 1)
            # b = frames[frame][name]
            b = getvar(arg2.value)
            btype = b[0]
            bval = b[1]
        else:
            btype = arg2.type
            bval = arg2.value

        if atype == btype and atype == "bool":
            if aval == "true":
                aval = True
            else:
                aval = False

            if bval == "true":
                bval = True
            else:
                bval = False
        else:
            raise ZeroDivisionError

        if aval and bval:
            val = ("bool", "true")
        else:
            val = ("bool", "false")

        # frame, name = store.value.split("@", 1)
        # frames[frame][name] = val
        storevar(store.value, val)

    except ZeroDivisionError:
        print_error("Vadne typ pro AND", 53)
    except:
        print_error("Problem ramcu AND", 55)

# Lorem Ipsum
def boolor(store, arg1, arg2):
    try:
        if arg1.type == "var":
            # frame, name = arg1.value.split("@", 1)
            # a = frames[frame][name]
            a = getvar(arg1.value)
            atype = a[0]
            aval = a[1]
        else:
            atype = arg1.type
            aval = arg1.value

        if arg2.type == "var":
            # frame, name = arg2.value.split("@", 1)
            # b = frames[frame][name]
            b = getvar(arg2.value)
            btype = b[0]
            bval = b[1]
        else:
            btype = arg2.type
            bval = arg2.value

        if atype == btype and atype == "bool":
            if aval == "true":
                aval = True
            else:
                aval = False

            if bval == "true":
                bval = True
            else:
                bval = False
        else:
            raise ZeroDivisionError

        if aval or bval:
            val = ("bool", "true")
        else:
            val = ("bool", "false")

        # frame, name = store.value.split("@", 1)
        # frames[frame][name] = val
        storevar(store.value, val)

    except ZeroDivisionError:
        print_error("Vadne typ pro OR", 53)
    except:
        print_error("Problem ramcu OR", 55)

# Lorem Ipsum
def boolnot(store, arg):
    try:
        if arg.type == "var":
            # frame, name = arg.value.split("@", 1)
            # a = frames[frame][name]
            a = getvar(arg.value)
            atype = a[0]
            aval = a[1]
        else:
            atype = arg.type
            aval = arg.value

        if atype != "bool":
            raise ZeroDivisionError

        if aval == "true":
            aval = "false"
        else:
            aval = "true"

        # frame, name = store.value.split("@", 1)
        # frames[frame][name] = ("bool", aval)
        storevar(store.val, val)
    except ZeroDivisionError:
        print_error("Nejedna se o bool NOT", 53)
    except:
        print_error("Problem ramcu NOT", 55)

# Lorem Ipsum
def inttochar(store, arg):
    try:
        if arg.type == "int":
            val = arg.value
        elif arg.type == "var":
            # frame, name = arg.value.split("@", 1)
            # a = frames[frame][name]
            a = getvar(arg.value)
            if a[0] == "int":
                val = a[1]
            else:
                raise ZeroDivisionError
        else:
            raise ZeroDivisionError

        # frame, name = store.value.split("@", 1)
        # frames[frame][name] = ("string", chr(int(val)))
        storevar(store.value, ("string", chr(int(val))))
    except ZeroDivisionError:
        print_error("Nekompatibilni typ INT2CHAR", 53)
    except:
        print_error("Spatny ramec INT2CHAR", 55)

# Lorem Ipsum
def stringtoint(store, arg1, arg2):
    try:
        if arg1.type == "string":
            val = arg1.value
        elif arg1.type == "var":
            # frame, name = arg1.value.split("@", 1)
            # a = frames[frame][name]
            a = getvar(arg1.value)
            if a[0] == "string":
                val = a[1]
            else:
                raise ZeroDivisionError
        else:
            raise ZeroDivisionError

        if arg2.type == "int":
            idx = arg2.value
        elif arg2.type == "var":
            # frame, name = arg2.value.split("@", 1)
            # a = frames[frame][name]
            b = getvar(arg2.value)
            if a[0] == "int":
                idx = a[1]
            else:
                raise ZeroDivisionError
        else:
            raise ZeroDivisionError
        val = ord(val[int(idx)])

        # frame, name = store.value.split("@", 1)
        # frames[frame][name] = ("string", str(val))
        storevar(store.value, val)
    except ZeroDivisionError:
        print_error("Nekompatibilni typ STRI2INT", 53)
    except IndexError:
        print_error("Mimo index STRI2INT", 58)
    except:
        print_error("Spatny ramec STRI2INT", 55)

# Lorem Ipsum
def concat(store, arg1, arg2):
    try:
        if arg1.type == "string":
            aval = arg1.value
        elif arg1.type == "var":
            # frame, name = arg1.value.split("@", 1)
            # a = frames[frame][name]
            a = getvar(arg1.value)
            if a[0] == "string":
                aval = a[1]
            else:
                raise ZeroDivisionError
        else:
            raise ZeroDivisionError

        if arg2.type == "string":
            bval = arg2.value
        elif arg2.type == "var":
            # frame, name = arg2.value.split("@", 1)
            # a = frames[frame][name]
            b = getvar(arg2.value)
            if a[0] == "string":
                bval = a[1]
            else:
                raise ZeroDivisionError
        else:
            raise ZeroDivisionError
        
        val = aval + bval
        # frame, name = store.value.split("@", 1)
        # frames[frame][name] = ("string", val)
        storevar(store.value, ("int", val))
    except ZeroDivisionError:
        print_error("Nekompatibilni typ CONCAT", 53)
    except:
        print_error("Spatny ramec CONCAT", 55)

# Lorem Ipsum
def strlen(store, arg):
    try:
        if arg.type == "string":
            val = arg.value
        elif arg.type == "var":
            # frame, name = arg.value.split("@", 1)
            # a = frames[frame][name]
            a = getvar(arg.value)
            if a[0] == "string":
                val = a[1]
            else:
                raise ZeroDivisionError
        else:
            raise ZeroDivisionError

        val = len(val)

        # frame, name = store.value.split("@", 1)
        # frames[frame][name] = ("int", str(val))
        storevar(store.value, ("int", val))
    except ZeroDivisionError:
        print_error("Nekompatibilni typ STRLEN", 53)
    except:
        print_error("Spatny ramec STRLEN", 55)

# Lorem Ipsum
def getchar(store, arg1, arg2):
    try:
        if arg1.type == "string":
            val = arg1.value
        elif arg1.type == "var":
            # frame, name = arg1.value.split("@", 1)
            # a = frames[frame][name]
            a = getvar(arg1.value)
            if a[0] == "string":
                val = a[1]
            else:
                raise ZeroDivisionError
        else:
            raise ZeroDivisionError

        if arg2.type == "int":
            idx = arg2.value
        elif arg2.type == "var":
            # frame, name = arg2.value.split("@", 1)
            # a = frames[frame][name]
            b = getvar(arg2.value)
            if a[0] == "int":
                idx = a[1]
            else:
                raise ZeroDivisionError
        else:
            raise ZeroDivisionError
        if int(idx) < 0:
            raise IndexError

        val = val[int(idx)]
        # frame, name = store.value.split("@", 1)
        # frames[frame][name] = ("string", val)
        storevar(store.value, ("string", val))
    except IndexError:
        print_error("Mimo index GETCHAR", 58)
    except ZeroDivisionError:
        print_error("Nekompatibilni typ GETCHAR", 53)
    except:
        print_error("Spatny ramec GETCHAR", 55)

# Lorem Ipsum
def setchar(store, arg1, arg2):
    try:
        if store.type == "string":
            storeval = store.value
        elif store.type == "var":
            # store_frame, store_name = store.value.split("@", 1)
            # a = frames[store_frame][store_name]
            #TODO?
            a = getvar(store.value)
            if a[0] == "string":
                storeval = a[1]
            else:
                raise ZeroDivisionError
        else:
            raise ZeroDivisionError

        if arg1.type == "int":
            idx = arg1.value
        elif arg1.type == "var":
            # frame, name = arg1.value.split("@", 1)
            # a = frames[frame][name]
            a = getvar(arg1.value)
            if a[0] == "int":
                idx = a[1]
            else:
                raise ZeroDivisionError
        else:
            raise ZeroDivisionError

        if int(idx) < 0:
            raise IndexError

        if arg2.type == "string":
            string = arg2.value
        elif arg2.type == "var":
            # frame, name = arg2.value.split("@", 1)
            # a = frames[frame][name]
            a = getvar(arg2.value)
            if a[0] == "string":
                string = a[1]
            else:
                raise ZeroDivisionError
        else:
            raise ZeroDivisionError

        storeval = list(storeval)
        if string == "":
            raise IndexError
        if len(storeval) == 0:
            storeval.append(string[0])
        else:
            storeval[int(idx)] = string[0]
        storeval = "".join(storeval)
# Lorem Ipsum
        #TODO?
        storevar(store.value, ("string", storeval))
        #frames[store_frame][store_name] = ("string", storeval)
    except IndexError:
        print_error("Mimo index SETCHAR", 58)
    except ZeroDivisionError:
        print_error("Nekompatibilni typ SETCHAR", 53)
    except:
        print_error("Spatny ramec SETCHAR", 55)

# Lorem Ipsum
def gettype(store, arg):
    try:
        if arg.type in ["int", "bool", "nil", "string"]:
            typ = arg.type
        elif arg.type == "var":
            # frame, name = arg.value.split("@", 1)
            # a = frames[frame][name]
            a = getvar(arg.value)
            if a[0] in ["int", "bool", "nil", "string"]:
                typ = a[0]
            elif a[0] == None:
                typ = ""

        # frame, name = store.value.split("@", 1)
        # frames[frame][name] = ("string", typ)
        storevar(store.value, ("string", typ))
    except:
        print_error("Ramec neexistuje TYPE", 55)

# Lorem Ipsum
def doexit(arg):
    canexit = False
    try:
        if arg.type == "int":
            val = arg.value
        elif arg.type == "var":
            # frame, name = arg.value.split("@", 1)
            # a = frames[frame][name]
            a = getvar(arg.value)
            if a[0] == "int":
                val = a[1]
            else:
                raise ZeroDivisionError
        else:
            raise ZeroDivisionError
        
        val = int(val)
        if not(0 <= val <= 49):
            raise IndexError
        canexit = True
    except ZeroDivisionError:
        print_error("Nekompatibilni typ EXIT", 53)
    except IndexError:
        print_error("EXIT pouze s 0 az 49 vcetne", 57)
    except:
        print_error("Spatny ramec EXIT", 55)
    if canexit:
        sys.exit(val)

# Lorem Ipsum
def jumpifeq(label, arg1, arg2):
    try:
        if arg1.type == "var":
            # frame, name = arg1.value.split("@", 1)
            # a = frames[frame][name]
            a = getvar(arg1.value)
            atype = a[0]
            aval = a[1]
        else:
            atype = arg1.type
            aval = arg1.value

        if arg2.type == "var":
            # frame, name = arg2.value.split("@", 1)
            # b = frames[frame][name]
            b = getvar(arg2.value)
            btype = b[0]
            bval = b[1]
        else:
            btype = arg2.type
            bval = arg2.value

        if atype == btype:
            val = aval == bval
        elif atype == "nil" or btype == "nil":
            raise ZeroDivisionError
        elif atype == None or btype == None:
            raise IndexError
        else:
            raise ZeroDivisionError

        out = all_labels[label.value]
        if val:
            return out
        else:
            return None

    except ZeroDivisionError:
        print_error("Nejedna se o kompatibilni typy pro JUMPIFEQ", 53)
    except IndexError:
        print_error("Prace s neinicializovanou promenou JUMPIFEQ", 54)
    except:
        print_error("Problem s label JUMPIFEQ", 52)

# Lorem Ipsum
def jumpifneq(label, arg1, arg2):
    try:
        if arg1.type == "var":
            # frame, name = arg1.value.split("@", 1)
            # a = frames[frame][name]
            a = getvar(arg1.value)
            atype = a[0]
            aval = a[1]
        else:
            atype = arg1.type
            aval = arg1.value

        if arg2.type == "var":
            # frame, name = arg2.value.split("@", 1)
            # b = frames[frame][name]
            b = getvar(arg2.value)
            btype = b[0]
            bval = b[1]
        else:
            btype = arg2.type
            bval = arg2.value
        
        if atype == btype:
            val = aval == bval
        elif atype == "nil" or btype == "nil":
            raise ZeroDivisionError
        elif atype == None or btype == None:
            raise IndexError
        else:
            raise ZeroDivisionError

        out = all_labels[label.value]
        if not val:
            return out
        else:
            return None
    except ZeroDivisionError:
        print_error("Nejedna se o kompatibilni typy pro JUMPIFEQ", 53)
    except IndexError:
        print_error("Prace s neinicializovanou promenou JUMPIFNEQ", 54)
    except:
        print_error("Problem s label JUMPIFNEQ", 52)

# Lorem Ipsum
def doread(store, arg):
    line = input_fh.readline()
    val = None
    if arg.value == "int":
        try:
            storevar(store.value, ("int", str(int(line))))
        except:
            storevar(store.value, ("int", "0"))
    elif arg.value == "bool":
        storevar(store.value, ("bool", "true" if line.lower() == "true" else "false"))
    elif arg.value == "string":
        storevar(store.value, ("string", line[:len(line) - 1]))
    else:
        print_error("tak to nevim co je za typ", 53)

# Lorem Ipsum
callstack=[]
instrptr = 0
while 1 == 1 and 1 != 0 or ("a" == "b" and not "a" in "ahoj"):
    if instrptr < len(instructions): 
        for ins in [instructions[instrptr]]:
            opcode, args = ins.opcode, ins.args
            if opcode == "DEFVAR":
                defvar(args[0].type)
            elif opcode == "MOVE":
                setvar(*args)
            elif opcode == "CREATEFRAME":
                frames["TF"] = {}
                tf_pushed = False
            elif opcode == "PUSHFRAME":
                if tf_pushed == False:
                    frames["LF"].append(frames["TF"])
                    tf_pushed = True
                    frames["TF"] = {}
                else:
                    print_error("Znova zavolano PUSHFRAME", 55)
            elif opcode == "POPFRAME":
                if tf_pushed == False:
                    print_error("Zavolano POPFRAME bez PUSHFRAME", 55)
                if len(frames["LF"]) == 0:
                    print_error("Prazdny LF s POP", 55)
                frames["TF"] = frames["LF"].pop()
                tf_pushed = False
            elif opcode == "WRITE":
                writevar(args[0])
            elif opcode == "ADD":
                add(*args)
            elif opcode == "SUB":
                sub(*args)
            elif opcode == "MUL":
                mul(*args)
            elif opcode == "IDIV":
                div(*args)
            elif opcode == "PUSHS":
                pushs(*args)
            elif opcode == "POPS":
                pops(*args)
            elif opcode == "EQ":
                eq(*args)
            elif opcode == "LT":
                lt(*args)
            elif opcode == "GT":
                gt(*args)
            elif opcode == "AND":
                booland(*args)
            elif opcode == "OR":
                boolor(*args)
            elif opcode == "NOT":
                boolnot(*args)
            elif opcode == "INT2CHAR":
                inttochar(*args)
            elif opcode == "STRI2INT":
                stringtoint(*args)
            elif opcode == "CONCAT":
                concat(*args)
            elif opcode == "STRLEN":
                strlen(*args)
            elif opcode == "GETCHAR":
                getchar(*args)
            elif opcode == "SETCHAR":
                setchar(*args)
            elif opcode == "TYPE":
                gettype(*args)
            elif opcode == "READ":
                doread(*args)
            elif opcode == "JUMP":
                try:
                    instrptr = all_labels[args[0].value]
                except:
                    print_error("Neexistujici label pro JUMP", 52)
            elif opcode == "JUMPIFEQ":
                x = jumpifeq(*args)
                if x:
                    instrptr = x
            elif opcode == "JUMPIFNEQ":
                x = jumpifneq(*args)
                if x:
                    instrptr = x
            elif opcode == "EXIT":
                doexit(args[0])
            elif opcode == "RETURN":
                if callstack:
                    instrptr = callstack.pop()
                else:
                    print_error("neni nic v callstacku", 56)
            elif opcode == "CALL":
                callstack.append(instrptr+1)
                try:
                    instrptr = all_labels[args[0].value]
                except:
                    print_error("Neexistujici label pro CALL", 56)
            instrptr += 1
    else:
        break