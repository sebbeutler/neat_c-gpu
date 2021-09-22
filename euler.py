i = 2520
c = True
while c:
    h = True
    for j in range(1, 21):
        if i % j != 0:
            h = False
    if h:
        c = False
        print(i)
    i += 1