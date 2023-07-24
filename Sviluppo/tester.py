import os
# from datetime import datetime

for i in range(1, 50):
    print("test number: " + str(i))
    os.system("echo 'test number: " + str(i) +"' >>results_test.txt && /usr/bin/time -h ./progetto /Users/luca/Documents/Progetto-API/Test/archivio_test_aperti/open_" + str(i) + ".txt >>results_test.txt 2>&1")
    # file.write("Test file: " + str(i) + str(' -> time: {}\n'.format(datetime.now() - start_time)))
    os.system("diff output.txt /Users/luca/Documents/Progetto-API/Test/archivio_test_aperti/open_" + str(i) + ".output.txt >>results_test.txt 2>&1 && echo '\n' >>results_test.txt")