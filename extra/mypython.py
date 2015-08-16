#!/usr/bin/env python

#**************************************************************
# Author: Tara Massey
# Assignment: Python Exploration
# Description: Creates three files in the directory the program
# is running in. prints exactly 10 random lowercase characters
# to each file. Then produces two random integers between 1-42
# and provides the product of them.
#*************************************************************
import string, random

alphabet = 'abcdefghijklmnopqrstuvwxyz'

# write files one through three
loop = 0
while loop < 3:
	#determine the file to be written to
	if loop == 0:
		filename = "fileOne.txt"

	if loop == 1:
		filename = "fileTwo.txt"

	if loop == 2:
		filename = "fileThree.txt"

	# open it
	myFile = open(filename, 'w')

	# put exactly 10 random lowercase chars in it
	# citation for line producing 10 random characters:
	# sth, StackOverflow. "Random strings in Python."
	# stackoverflow.com/questions/2030053/random-strings-in-Python
	print '\nWriting the following 10 random characters to file ' + filename + ': '
	i = 0
	word = ''.join(random.choice(string.lowercase) for i in range(10))
	print word
	myFile.write(word);
	
	#close the file
	myFile.close()

	# loop to next
	loop += 1

# file contents printed and stored, now print two random integers 1-42
num = random.randint(1, 42)
num2 = random.randint(1, 42)
print '\nFirst random integer (1-42): '
print num

print '\nSecond random integer (1-42): '
print num2

print '\nProduct of two numbers: '
print num * num2

