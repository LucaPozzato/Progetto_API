x = 3
array = [[4,5],[2,x]]
n = len(array[0])
i = 0
j = 0
flag = 0
while i < n and j < n:
    if array[i][j] == x:
        flag = 1
    if array[i][j] < x:
        j += 1
    else:
        i += 1
if flag:
    print("\ntrovato\n")
else:
    print("\nnon trovato\n")
