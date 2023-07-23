array = [1,7,19,21,3,4,5,10,12,8]
i = -1
j = -1
for w in range(0, len(array)):
    for k in range(0, len(array)):
        if i == -1:
            i = array.pop(0)
        elif j == -1:
            j = array.pop(0)
        if j != -1 and i != -1:
            if i > j:
                array.append(j)
                j = -1
            else:
                array.append(i)
                i = -1
    if i == -1:
        array.append(j)
        j = -1
    elif j == -1:
        array.append(i)
        i = -1
print(array)