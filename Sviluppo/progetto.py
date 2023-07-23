import sys

# file = open(sys.argv[1], "r")
file = open("/Users/luca/Documents/Progetto-API/Test/archivio_test_aperti/open_50.txt", "r")
output = open("output.txt", "w")
lines = file.readlines()

highway = {}
for i in lines:
    commands = i.split()
    if(commands[0] == "aggiungi-stazione"):
        if int(commands[1]) in highway:
            output.write("non aggiunta\n")            
        else:
            # cars = [0]*int(commands[2])
            # for j in range(int(commands[2])):
            #     cars[j] = int(commands[j+3])
            # cars.sort()
            highway[int(commands[1])] = {}
            highway[int(commands[1])]["cars"] = [int(commands[j+3]) for j in range(int(commands[2]))]
            highway[int(commands[1])]["stations_dx"] = [] 
            highway[int(commands[1])]["stations_sx"] = [] 
            # len_cars = len(highway[int(commands[1])]["cars"])
            # if len_cars > 0:
            #     car_max = highway[int(commands[1])]["cars"][len_cars-1]
            # # calcola grafo
            # max_distance = int(commands[1]) + car_max
            # for station in highway:
            #     if station <= max_distance and station > int(commands[1]):
            #         highway[int(commands[1])]["stations"].append(station)
            output.write("aggiunta\n")
    if(commands[0] == "demolisci-stazione"):
        if int(commands[1]) in highway:
            highway.pop(int(commands[1]))
            output.write("demolita\n")
        else:
            output.write("non demolita\n")
    if(commands[0] == "aggiungi-auto"):
        if int(commands[1]) in highway:
            # len_cars = len(highway[int(commands[1])]["cars"])
            # if len_cars > 0:
            #     car_max = highway[int(commands[1])]["cars"][len_cars-1] 
            highway[int(commands[1])]["cars"].append(int(commands[2]))
            # highway[int(commands[1])]["cars"].sort()
            output.write("aggiunta\n")
            # se macchina nuova < macchina max niente altrimeni ric grafo
            # if int(commands[2]) <= car_max:
            #     pass
            # else:
            #     highway[int(commands[1])]["stations"] = []
            #     for station in highway:
            #         if station <= (int(commands[1])+int(commands[2])) and station > int(commands[1]):
            #             highway[int(commands[1])]["stations"].append(station) 
            #     # ricalcola grafo
        else:
            output.write("non aggiunta\n")
    if(commands[0] == "rottama-auto"):
        if int(commands[1]) in highway and int(commands[2]) in highway[int(commands[1])]["cars"]: 
            # len_cars = len(highway[int(commands[1])]["cars"])
            # car_max = highway[int(commands[1])]["cars"][len_cars-1] 
            highway[int(commands[1])]["cars"].remove(int(commands[2]))
            output.write("rottamata\n")
            # # se macchina rottamata = macchina max ric grafo
            # if int(commands[2]) == car_max:
            #     len_cars = len(highway[int(commands[1])]["cars"])
            #     car_max = highway[int(commands[1])]["cars"][len_cars-1] 
            #     highway[int(commands[1])]["stations"] = []
            #     for station in highway:
            #         if station <= (int(commands[1])+car_max) and station > int(commands[1]):
            #             highway[int(commands[1])]["stations"].append(station) 
            #     # ricalcola grafo
            # else:
            #     pass
        else:
            output.write("non rottamata\n")
    if(commands[0] == "pianifica-percorso"):
        if int(commands[1]) < int(commands[2]):
            node_st = int(commands[1])
            node_end = int(commands[2])
        else:
            node_st = int(commands[2])
            node_end = int(commands[1])
        
        for k in highway:
            highway[k]["stations_dx"] = []
            highway[k]["stations_sx"] = []

        for stop in highway:
            len_cars = len(highway[stop]["cars"])
            if len_cars > 0:
                car_max = max(highway[stop]["cars"])
                for station in highway:
                    if station <= stop+car_max and station > stop:
                        highway[stop]["stations_dx"].append(station)
                    if station >= stop-car_max and station < stop:
                        highway[station]["stations_sx"].append(stop)
            highway[stop]["stations_dx"].sort()
        
        for stop in highway:
            highway[stop]["stations_sx"].sort()

        paths = []
        path_list = [[node_st]]
        path_index = 0

        previous_nodes = [node_st]

        if node_st == node_end:
            output.write(node_st)

        while path_index < len(path_list):
            current_path = path_list[path_index]
            last_node = current_path[-1]
            if int(commands[1]) < int(commands[2]):
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
            # best_path = paths[0]
            # flag = 0
            # if int(commands[1]) > int(commands[2]):
            #     for i in range(len(paths[0])-1, 0, -1):
            #         current_station = paths[0][i]
            #         for path in paths:
            #             if len(path) <= len(paths[0]):
            #                 if path[i] < current_station:
            #                     current_station = path[i]
            #                     best_path = path
            #                     flag = 1
            #         if flag == 1:
            #             break
            string = ""
            if int(commands[1]) < int(commands[2]):
                for i in paths[0]:
                    string += str(i) + " "
                output.write(string.strip() + "\n")
            else:
                for i in range(len(paths[0])-1, -1, -1):
                    string += str(paths[0][i]) + " "
                output.write(string.strip() + "\n")
        else:
            output.write("nessun percorso\n")

    # highway = dict(sorted(highway.items())) # non necessario

# print(json.dumps(highway))
file.close()
output.close()