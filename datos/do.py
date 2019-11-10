import csv

# lineas = {
#   'belgranosur': 1,
#   'mitre': 2,
#   'Mitre': 2,
#   'roca': 3,
#   'Roca': 3,
#   'sanmartin': 4,
#   'San Martín': 4,
#   'sarmiento': 5,
#   'Sarmiento': 5,
# }
# 
# estaciones = {}
# for k, v in csv.reader(open('estaciones.txt')):
#     estaciones[v] = k
# 
# f = open('trenes.txt')
# g = open('trenes_normalizada.txt', 'w')
# r = csv.reader(f)
# w = csv.writer(g)
# print(next(r))
# for x in r:
#     x[2] = str(lineas[x[2]])
#     x[3] = estaciones[x[3].lower()]
#     w.writerow(x)
# f.close()
 
#r = csv.reader(open('viajes.txt'))
#w = csv.writer(open('viajes_cantidad.txt', 'w'))
#print(next(r))
#for x in r:
#    w.writerow([x[0], x[4]])

