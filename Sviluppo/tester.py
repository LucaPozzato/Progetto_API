import os
from datetime import datetime

file = open("/Users/luca/Documents/Progetto-API/Sviluppo/results.txt", "w")

for i in range(1, 112):
    print("test number: " + str(i))
    start_time = datetime.now()
    os.system("./progetto /Users/luca/Documents/Progetto-API/Test/archivio_test_aperti/open_" + str(i) + ".txt")
    file.write("Test file: " + str(i) + str(' -> time: {}\n'.format(datetime.now() - start_time)))
    os.system("diff output.txt /Users/luca/Documents/Progetto-API/Test/archivio_test_aperti/open_" + str(i) + ".output.txt")