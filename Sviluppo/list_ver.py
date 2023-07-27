import sys
import os
import cProfile

DEBUG = os.getenv("PYLAB_DEBUG", False) == "true"
PROFILE = os.getenv("PYLAB_PROFILE", False) == "true"

print(f"DEBUG: {DEBUG}")
print(f"PROFILE: {PROFILE}")

file = open("/Users/luca/Documents/Progetto-API/Test/archivio_test_aperti/open_100.txt", "r")
# basefile = os.path.basename("Test/archivio_test_aperti/open_100.txt")
output = open("/Users/luca/Documents/Progetto-API/sviluppo/output.txt", "w+")
lines = file.readlines()
highway = []

def add_station(station, cars):
    if len(highway) == 0:
        stn = {'id': station, 'cars': cars, 'explored': 0}
        highway.append(stn)
        output.write("aggiunta\n")
    else:
        i = 0
        while i < len(highway):
            if highway[i]['id'] < station:
                i += 1
            elif highway[i]['id'] == station:
                output.write("non aggiunta\n")
                break
            else:
                stn = {'id': station, 'cars': cars, 'explored': 0}
                highway.insert(i ,stn)
                output.write("aggiunta\n")
                break
        if i == len(highway):
            stn = {'id': station, 'cars': cars, 'explored': 0}
            highway.insert(i ,stn)
            output.write("aggiunta\n")
    return 0

def del_station(station):
    i = 0
    found = 0
    while i < len(highway):
        if highway[i]['id'] == station:
            highway.remove(highway[i])
            output.write("demolita\n")
            found = 1
            break
        i += 1
    if found == 0:
        output.write("non demolita\n")
    return 0

def add_car(station, autonomy):
    i = 0
    found = 0
    while i < len(highway):
        if highway[i]['id'] == station:
            highway[i]['cars'].append(autonomy)
            output.write("aggiunta\n")
            found = 1
            break
        i += 1
    if found == 0:
        output.write("non aggiunta\n")
    return 0

def del_car(station, autonomy):
    i = 0
    found = 0
    while i < len(highway):
        if highway[i]['id'] == station:
            if autonomy in highway[i]['cars']:
                highway[i]['cars'].remove(autonomy)
                output.write("rottamata\n")
                found = 1
                break
            else:
                break
        i += 1
    if found == 0:
        output.write("non rottamata\n")
    return 0

def search_path(stn_start, stn_end):
    if stn_start < stn_end:
        direction = 1
    else:
        direction = -1
    index = 0
    end_index = 0
    i = 0
    if direction == 1:
        while i < len(highway):
            if highway[i]['id'] == stn_start:
                index = i
            elif highway[i]['id'] == stn_end:
                end_index = i
                break
            i += 1
    else:
        while i < len(highway):
            if highway[i]['id'] == stn_end:
                end_index = i
            elif highway[i]['id'] == stn_start:
                index = i
                break
            i += 1

    queue = [[index, -1]]
    if stn_start == stn_end:
        output.write(str(stn_start) + "\n")
    else:
        for i in range(index, end_index):
            highway[i]['explored'] = 0

        found = bfs(end_index, queue, direction)
        if found != -1:
            path = []
            for i in range(0, len(queue)):
                queue[i][0] = highway[queue[i][0]]['id']
                queue[i][1] = highway[queue[i][1]]['id']
            for i in range(0, len(queue)):
                for j in range(0, len(queue)):
                    if queue[j][0] == found:
                        path.append(found)
                        found = queue[j][1]
                        break
            print(path)
            string = ""
            if stn_start > stn_end:
                string = " ".join(str(i) for i in path)
                output.write(string.strip() + "\n")
            else:
                path.reverse()
                string = " ".join(str(i) for i in path)
                output.write(string.strip() + "\n")
        else:
            output.write("nessun percorso\n")
    return 0

def bfs(end_index, queue, direction):
    next_nodes = []
    i = 0
    index = 0
    while (i < len(queue)) :
        v = queue[i]
        index = v[0]
        if direction == 1:
            next_nodes = []
            j = index + 1
            car = max(highway[index]['cars'])
            while highway[j]['id'] < highway[index]['id'] + car and j < len(highway) - 1:
                next_nodes.append(j)
                j += 1
        else:
            next_nodes = []
            j = index - 1
            car = max(highway[index]['cars'])
            while highway[j]['id'] > highway[index]['id'] - car and j >= 0:
                next_nodes.append(j)
                j += -1

        if v[0] == end_index:
            return v[0]

        for next_node in next_nodes:
            if highway[next_node]['explored'] == 0:
                highway[next_node]['explored'] = 1
                q = [next_node, v[0]]
                queue.append(q)

        i += 1
    else:
        return -1

def main():
    for i in lines:
        commands = i.split()
        if(commands[0] == "aggiungi-stazione"):
            station = int(commands[1])
            cars = [int(commands[j+3]) for j in range(int(commands[2]))]
            add_station(station, cars)

        if(commands[0] == "demolisci-stazione"):
            station = int(commands[1])
            del_station(station)

        if(commands[0] == "aggiungi-auto"):
            station = int(commands[1])
            autonomy = int(commands[2])
            add_car(station, autonomy)

        if(commands[0] == "rottama-auto"):
            station = int(commands[1])
            autonomy = int(commands[2])
            del_car(station, autonomy)

        if(commands[0] == "pianifica-percorso"):
            stn_start = int(commands[1])
            stn_end = int(commands[2])
            search_path(stn_start, stn_end)

if PROFILE:
    cProfile.run("main()")
else:
    main()

file.close()
output.close()