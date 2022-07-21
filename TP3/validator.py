import argparse

def validate_number_of_lines(r, n, path):
    expected_number_of_lines = r * n 
    file = open(path)
    lines = file.readlines()

    if len(lines) == expected_number_of_lines:
        print(
            f"NÚMERO DE LINHAS: OK")
    else:
        print(
            f"NÚMERO DE LINHAS: ERRO")


def validate_number_of_writes(path, n, r):
    processes = {}
    file = open(path)
    lines = file.readlines()

    for line in lines:
        pid = int(line.split("  |  ")[1].split(": ")[1])
        if processes.get(pid):
            processes[pid] = processes[pid]+1
        else:
            processes[pid] = 1
    for k,v in processes.items():
        if v != r :
            print (f"NÚMERO DE ESCRITAS POR PROCESSO: ERRO")
            return 1
    print (f"NÚMERO DE ESCRITAS POR PROCESSO: OK")
    return 0


def validate_increasing(path):
    #Valida se o tempo está aumentando entre as linhas
    file = open(path)
    lines = file.readlines()
    last = -1
    for l in lines:
        new = int(l.split("  |  ")[0].split(": ")[1])
        if new < last:
            print(f"EVOLUÇÃO DO RELÓGIO DO SISTEMA: ERRO")
            return
        last = new
    print(f"EVOLUÇÃO DO RELÓGIO DO SISTEMA: OK")


def validate_execution(path):
    file = open(path)
    lines = file.readlines()
    requests = []
    last_granted = 0
    for l in lines:
        splt = l.split(' ')
        if splt[1] == '2\n':
            last_granted = int(splt[0])
        elif splt[1] == '1\n':
            requests.append(int(splt[0]))
        else:
            if int(splt[0]) != requests[0] or int(splt[0]) != last_granted:
                print(f"LOG GERADO PELO COORDENADOR: ERRO")
                return
            requests = requests[1:]
    print(f"LOG GERADO PELO COORDENADOR: OK")


parser = argparse.ArgumentParser(description='Process some integers.')
parser.add_argument('r', type=int, help='r')
parser.add_argument('n', type=int, help='n')
parser.add_argument('coordpath', help='coordpath')
parser.add_argument('resultpath', help='resultpath')
args = parser.parse_args()

r = args.r
n = args.n
coordpath = args.coordpath
resultpath = args.resultpath

validate_number_of_lines(r, n, resultpath)
validate_number_of_writes(resultpath, n, r)
validate_increasing(resultpath)
validate_execution(coordpath)

