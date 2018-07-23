import os
path='./'
cnt = 0;
for i in os.walk(path):
    for j in i[2]:
		myfile = open(j) 
		lines = len(myfile.readlines()) 
		cnt = cnt + lines
		print(j,lines)
	
print(cnt)