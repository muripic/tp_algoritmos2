
f = open('interurbano.txt')
g = open('interurbano2.txt', 'w')
n = 0
for l in f:
    g.write('%u,' % (n,) + l)
    n += 1
g.close()
f.close()
