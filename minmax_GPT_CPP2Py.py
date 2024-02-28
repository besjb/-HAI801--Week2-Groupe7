import random
import time

def estVictoire(grille, joueur):
    # Vérifie les lignes, colonnes et diagonales pour une victoire
    for i in range(3):
        if all(grille[i][j] == joueur for j in range(3)):
            return True
        if all(grille[j][i] == joueur for j in range(3)):
            return True
    if grille[0][0] == joueur and grille[1][1] == joueur and grille[2][2] == joueur:
        return True
    if grille[0][2] == joueur and grille[1][1] == joueur and grille[2][0] == joueur:
        return True
    return False

def estPartieTerminee(grille):
    # Vérifie si la grille est pleine ou s'il y a un gagnant
    if estVictoire(grille, 'X') or estVictoire(grille, 'O'):
        return True
    for ligne in grille:
        if '-' in ligne:
            return False
    return True

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
        meilleurScore = -1000
        for i in range(3):
            for j in range(3):
                if grille[i][j] == '-':
                    grille[i][j] = 'X'
                    meilleurScore = max(meilleurScore, minimax(grille, profondeur + 1, not estMaximisant))
                    grille[i][j] = '-'
        return meilleurScore
    else:
        meilleurScore = 1000
        for i in range(3):
            for j in range(3):
                if grille[i][j] == '-':
                    grille[i][j] = 'O'
                    meilleurScore = min(meilleurScore, minimax(grille, profondeur + 1, not estMaximisant))
                    grille[i][j] = '-'
        return meilleurScore

def meilleurCoup(grille):
    meilleurScore = -1000
    meilleurCoup = (-1, -1)
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
    joueur_actuel = symbol
    while not estPartieTerminee(grille):
        coup = ()
        if joueur_actuel == 'X':
            coup = meilleurCoup(grille)
        else:
            coup = choisirCaseVide(grille)
        grille[coup[0]][coup[1]] = joueur_actuel
        joueur_actuel = 'O' if joueur_actuel == 'X' else 'X'
    return 1 if estVictoire(grille, 'X') else -1 if estVictoire(grille, 'O') else 0

def genererGrilleDepart():
    grille = [['-' for _ in range(3)] for _ in range(3)]
    coups_a_jouer = random.randint(0, 9)
    symboles = ['X', 'O']
    symbole_actuel = random.choice(symboles)

    for _ in range(coups_a_jouer):
        ligne, colonne = choisirCaseVide(grille)
        grille[ligne][colonne] = symbole_actuel
        symbole_actuel = 'X' if symbole_actuel == 'O' else 'O'
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
            if len(line) < 10:
                line = line.strip() + '-' * (10 - len(line.strip()))
            symbol = line[0]
            values = line[1:].replace(' ', '-')
            grid = [list(values[i:i+3]) for i in range(0, len(values), 3)]
            grids.append((symbol, grid))
    return grids

file_path = "data/dataset.txt"
grids = parse_grids_from_file(file_path)

nblose = 0
nbwin = 0
nbdraw = 0
nbline = 4519
total_duration = 0

for symbol, grid in grids:
    start = time.time()
    result = jouerPartie(grid, symbol)
    end = time.time()
    total_duration += end - start

    if result == -1:
        nblose += 1
    elif result == 1:
        nbwin += 1
    else:
        nbdraw += 1

print("Winrate  = {:.2f} %".format((nbwin * 100) / nbline))
print("Loserate = {:.2f} %".format((nblose * 100) / nbline))
print("Drawrate = {:.2f} %".format((nbdraw * 100) / nbline))
print("AVG duration = {:.4f} s".format(total_duration / nbline))
print("Full duration = {:.2f} s".format(total_duration))
