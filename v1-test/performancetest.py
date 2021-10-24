#!C:\Python27\python.exe -u


import time
t0 = time.time()
a=0
i=0
while (i<1000000.000001):
    a+=10.000001
    i+=1
    
t1 = time.time() 
print (t1-t0)
