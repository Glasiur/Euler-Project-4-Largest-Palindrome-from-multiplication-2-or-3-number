import matplotlib.pyplot as plt

tab = []
with open("github_euler/statistic") as brut:
    for raw in brut:

        tab.append(raw.split(',')[0:-1])

x = [int(line[0]) for line in tab]
y1 = [int(line[1]) for line in tab]
y2 = [int(line[2]) for line in tab]

fig = plt.figure()

ax = fig.add_subplot(111)
ax2 = ax.twinx()

ax.scatter(x, y1, color="goldenrod", label='pal',s=1)
ax2.scatter(x, y2, color="slateblue", label='c',s=1)
ax.set_xlabel("lim")
ax.set_ylabel("pal")
ax2.set_ylabel("c")
plt.legend()


plt.show()





"""plt.scatter(x, y1, color="goldenrod", label='pal')"""