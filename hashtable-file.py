# HashTable - interface
# Made by Grzegorz Wyszynski
# email: wyszynskig@gmail.com
# version: 0.1


import hashtable


tablein=hashtable.HashTable(10)
print("welcome")
filein=open("input.txt")
for line in filein:
	elements=line.split('\t')
	tablein.append((elements[0],elements[1]))
output=tablein.getall()
print(output)