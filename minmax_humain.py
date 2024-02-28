import math
import random
import copy
import time

def estPartieTerminee(grille):
    # Vérifie si la grille est pleine ou s'il y a un gagnant
    if estVictoire(grille, 'X') or estVictoire(grille, 'O'):
        return True
    for ligne in grille:
        if '-' in ligne:
            return False
    return True

def estVictoire(grille, joueur):
    # Vérifie les lignes, colonnes et diagonales pour une victoire
    for i in range(3):
        if all(grille[i][j] == joueur for j in range(3)):
            return True
        if all(grille[j][i] == joueur for j in range(3)):
            return True
    if all(grille[i][i] == joueur for i in range(3)) or all(grille[i][2 - i] == joueur for i in range(3)):
        return True
    return False

def scoreEtat(grille):
    # Évalue l'état actuel du jeu
    if estVictoire(grille, 'X'):
        return 10
    elif estVictoire(grille, 'O'):
        return -10
    else:
        return 0

def minimax(grille, profondeur, estMaximisant):
    if estPartieTerminee(grille):
        return scoreEtat(grille)

    if estMaximisant:
        meilleurScore = -math.inf
        for i in range(3):
            for j in range(3):
                if grille[i][j] == '-':
                    grille[i][j] = 'X'
                    score = minimax(grille, profondeur + 1, False)
                    grille[i][j] = '-'
                    meilleurScore = max(meilleurScore, score)
        return meilleurScore
    else:
        meilleurScore = math.inf
        for i in range(3):
            for j in range(3):
                if grille[i][j] == '-':
                    grille[i][j] = 'O'
                    score = minimax(grille, profondeur + 1, True)
                    grille[i][j] = '-'
                    meilleurScore = min(meilleurScore, score)
        return meilleurScore

def meilleurCoup(grille):
    meilleurScore = -math.inf
    meilleurCoup = None
    for i in range(3):
        for j in range(3):
            if grille[i][j] == '-':
                grille[i][j] = 'X'
                score = minimax(grille, 0, False)
                grille[i][j] = '-'
                if score > meilleurScore:
                    meilleurScore = score
                    meilleurCoup = (i, j)
    return meilleurCoup

def choisirCaseVide(grille):
    cases_vides = [(i, j) for i in range(3) for j in range(3) if grille[i][j] == '-']
    return random.choice(cases_vides)

def jouerPartie(grille, symbol):
    grille = copy.deepcopy(grille)  # Copie de la grille pour ne pas modifier l'originale
    joueur_actuel = symbol
    end = 0
    while not estPartieTerminee(grille):
        if joueur_actuel == 'X':
            coup = meilleurCoup(grille)
        else:
            coup = choisirCaseVide(grille)  # Utilisation de choisirCaseVide pour l'ordinateur
        grille[coup[0]][coup[1]] = joueur_actuel
        joueur_actuel = 'O' if joueur_actuel == 'X' else 'X'
    # afficherGrille(grille)
    result = 0
    if estVictoire(grille, 'X'):
        result = 1
    elif estVictoire(grille, 'O'):
        result = -1
    return result

def genererGrilleDepart():
    grille = [['-' for _ in range(3)] for _ in range(3)]
    coups_a_jouer = random.randint(0, 9)  # Nombre aléatoire de coups à jouer
    symboles = ['X', 'O']
    symbole_actuel = random.choice(symboles)  # Choisir aléatoirement le premier symbole

    for _ in range(coups_a_jouer):
        ligne, colonne = choisirCaseVide(grille)
        grille[ligne][colonne] = symbole_actuel
        symbole_actuel = 'X' if symbole_actuel == 'O' else 'O'  # Alterner entre 'X' et 'O'

    return grille

def afficherGrille(grille):
    print("\n---------")
    for ligne in grille:
        print(" | ".join(ligne))
        print("---------")

def parse_grids_from_file(file_path):
    grids = []
    with open(file_path, 'r') as file:
        for line in file:
            line = line.strip()
            if len(line) < 10:
                # Ignorer les lignes qui ne sont pas au format attendu
                line += ' ' * (10 - len(line))
            symbol = line[0]
            values = line[1:]
            values = values.replace(' ', '-')
            grid = [list(values[i:i+3]) for i in range(0, len(values), 3)]
            grids.append((symbol, grid))
    return grids


file_path = "data/dataset.txt"
grids = parse_grids_from_file(file_path)
#jouerPartie(grids[-1][1], grids[-1][0])
# Pour afficher les grilles
nblose = 0
nbwin = 0
nbdraw = 0
nbline = 4519
total_duration = 0
for symbol, grid in grids:
    start = time.time()
    r = jouerPartie(grid, symbol)
    end = time.time()
    total_duration += (end - start)

    if (r == -1):
        nblose += 1
    elif (r):
        nbwin += 1
    else:
        nbdraw += 1

print("Winrate  = " + str((nbwin/nbline)*100) + str(" %"))
print("Loserate = " + str((nblose/nbline)*100)+ str(" %"))
print("Drawrate = " + str((nbdraw/nbline)*100)+ str(" %"))
print("AVG duration = " + str(total_duration/nbline) + str(" s"))
print("Full duration = " + str(total_duration) + str(" s"))

