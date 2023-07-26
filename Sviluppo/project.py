import sys
import os
import cProfile

DEBUG = os.getenv("PYLAB_DEBUG", False) == "true"
PROFILE = os.getenv("PYLAB_PROFILE", False) == "true"

# Optimizations
PARTIAL_GRAPH = os.getenv("PYLAB_PARTIAL", False) == "true"

print(f"DEBUG: {DEBUG}")
print(f"PROFILE: {PROFILE}")

file = open(sys.argv[1], "r")
basefile = os.path.basename(sys.argv[1])
output = open("Output/"+basefile+".output", "w+")
lines = file.readlines()
highway = {}

def add_station(station, cars):
    if station in highway:
        output.write("non aggiunta\n")            
    else:
        highway[station] = {}
        highway[station]["cars"] = {}
        for c in cars:
            if c not in highway[station]["cars"].keys():
                highway[station]["cars"][c] = 1
            else:
                highway[station]["cars"][c] += 1
        highway[station]["stations_dx"] = [] 
        highway[station]["stations_sx"] = [] 
        output.write("aggiunta\n")

def del_station(station):
    if station in highway:
        highway.pop(station)
        output.write("demolita\n")
    else:
        output.write("non demolita\n")

def add_car(station, autonomy):
    if station in highway:
        if autonomy not in highway[station]["cars"].keys():
            highway[station]["cars"][autonomy] = 1
        else:
            highway[station]["cars"][autonomy] += 1
        output.write("aggiunta\n")
    else:
        output.write("non aggiunta\n")

def del_car(station, autonomy):
    if station in highway and autonomy in highway[station]["cars"].keys(): 
        highway[station]["cars"][autonomy] -= 1
        if highway[station]["cars"][autonomy] <= 0:
            highway[station]["cars"].pop(autonomy)
        output.write("rottamata\n")
    else:
        output.write("non rottamata\n")

def search_path(stn_start, stn_end):
    if stn_start < stn_end:
        node_st = stn_start
        node_end = stn_end
    else:
        node_st = stn_end
        node_end = stn_start
    
    graph_rebuild(stn_start, stn_end)

    if DEBUG:
        print(f" {stn_start} -> {stn_end}")
        sts = list(highway.keys())
        sts.sort()
        for s in sts:
            print(f"STN: {s}, {highway[s]}")
        print()

    paths = []
    path_list = [[node_st]]
    path_index = 0

    previous_nodes = [node_st]

    if node_st == node_end:
        output.write(node_st)

    while path_index < len(path_list):
        current_path = path_list[path_index]
        last_node = current_path[-1]
        if stn_start < stn_end:
            next_nodes = highway[last_node]["stations_dx"]
        else:
            next_nodes = highway[last_node]["stations_sx"]

        if node_end in next_nodes:
            current_path.append(node_end)
            paths.append(current_path)
        
        for next_node in next_nodes:
            if not next_node in previous_nodes:
                new_path = current_path[:]
                new_path.append(next_node)
                path_list.append(new_path)
                previous_nodes.append(next_node)
        path_index = path_index + 1

    if len(paths) > 0:
        string = ""
        if stn_start < stn_end:
            string = " ".join(str(i) for i in paths[0])
            if DEBUG:
                print(string)
            output.write(string.strip() + "\n")
        else:
            paths[0].reverse()
            string = " ".join(str(i) for i in paths[0])
            if DEBUG:
                print(string)
            output.write(string.strip() + "\n")
    else:
        if DEBUG:
            print("Nessun percorso\n")
        output.write("nessun percorso\n")

def graph_rebuild(stn_start, stn_end):
    for k in highway:
        highway[k]["stations_dx"] = []
        highway[k]["stations_sx"] = []

    range_start = min(stn_start, stn_end)
    range_end = max(stn_start, stn_end) + 1
    if PARTIAL_GRAPH:
        stations = [s for s in range(range_start, range_end) if s in highway.keys()]
    else:
        stations = highway.keys()

    if DEBUG:
        print(f"Rebuild graph for stations: {stations}")
    # O(nˆ2) against stations number
    for stop in stations:
        len_cars = len(highway[stop]["cars"].keys())
        if len_cars > 0:
            car_max = max(highway[stop]["cars"].keys())
            # List comprehension reduces the number of alloc calls
            highway[stop]["stations_dx"] = [ s for s in stations if s <= stop+car_max and s > stop ]
            for station in stations:
                # if station <= stop+car_max and station > stop:
                #     highway[stop]["stations_dx"].append(station)
                if station >= stop-car_max and station < stop:
                    highway[station]["stations_sx"].append(stop)
        highway[stop]["stations_dx"].sort()
    
    for stop in stations:
        highway[stop]["stations_sx"].sort()


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