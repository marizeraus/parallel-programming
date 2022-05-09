import random
ff=open("file.txt","w+")
for _ in range(100000):
    ff.write(str(random.randrange(0,100)))
    ff.write("\n")
ff.seek(0,0)
while True:
    aa=ff.readline()
    if not aa:
        print("End")
        break
    else:
        if int(aa)%2==0:
            print(int(aa))
           
ff.close()
