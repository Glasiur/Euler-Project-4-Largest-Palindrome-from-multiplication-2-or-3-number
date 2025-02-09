import matplotlib.pyplot as plt

# Chargement et traitement des données
with open("github_euler/statistic") as brut:
    tab = [line.split(',')[0:-1] for line in brut]
#tab.sort(key= lambda x: x[1])

# Extraction des données principales
x = [int(line[1]) for line in tab]
max_pal = [int(line[2]) for line in tab]
iteration = [int(line[3]) for line in tab]
sqrt_or_cbrt_x = [int(line[4]) for line in tab]
start = [int(line[4])**3 for line in tab]  # cbrt tttttttttttttttt oui ou non 


# Traitement dynamique des étapes
steps_x = []
steps_y = []
for i, line in enumerate(tab):
    # Extraction des étapes pour chaque ligne
    step_values = line[0].split(';')[1:]  # Ignorer le premier élément vide
    for val in step_values:
        if val.strip():  # Vérifier que la chaîne n'est pas vide
            steps_x.append(x[i])
            steps_y.append(int(val))


# Création du graphique
fig, ax = plt.subplots(figsize=(12, 7))
ax2 = ax.twinx()

# Affichage des étapes (points verts)
ax.scatter(steps_x, steps_y, color="gray", s=0.1, alpha=1, label='Tentatives')

# Autres données
ax.scatter(x, max_pal, color="goldenrod", s=1, label='Meilleur palindrome')
ax.plot(x, x, color="black", linestyle='--', alpha=0.2, label='y = x')
ax.scatter(x, sqrt_or_cbrt_x, color="limegreen", s=1, label='Racine cubique')
ax.scatter(x, start, color="red", s=1, label='Point de départ')
ax2.scatter(x, iteration, color="slateblue", s=1, label='Itérations')

# Configuration finale
ax.set_xlabel("Limite N", fontsize=12)
ax.set_ylabel("Valeurs principales", fontsize=12)
ax2.set_ylabel("Nombre d'itérations", fontsize=12)
ax.grid(alpha=0.2)

# Gestion unique de la légende
handles, labels = ax.get_legend_handles_labels()
handles2, labels2 = ax2.get_legend_handles_labels()
fig.legend(handles + handles2, labels + labels2, 
           loc='upper center', 
           ncol=3,
           bbox_to_anchor=(0.5, 1.05),
           fontsize=9)

plt.tight_layout()
plt.show()