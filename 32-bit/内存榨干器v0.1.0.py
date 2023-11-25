import os
import time
import itertools as its

words = '你好'
r = its.product(words, repeat=1000)
dic = open(r"C:\Users\Public\Documents\让我看看.txt", 'a')
for i in r:
    dic.write(''.join(i))
    dic.write(''.join('\n'))
dic.close()
