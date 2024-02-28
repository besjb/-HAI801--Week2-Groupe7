#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <ctime>
#include <cstdlib>

using namespace std;

bool estVictoire(vector<vector<char>> &grille, char joueur) {
    // Vérifie les lignes, colonnes et diagonales pour une victoire
    for (int i = 0; i < 3; ++i) {
        if (grille[i][0] == joueur && grille[i][1] == joueur && grille[i][2] == joueur) {
            return true;
        }
        if (grille[0][i] == joueur && grille[1][i] == joueur && grille[2][i] == joueur) {
            return true;
        }
    }
    if (grille[0][0] == joueur && grille[1][1] == joueur && grille[2][2] == joueur) {
        return true;
    }
    if (grille[0][2] == joueur && grille[1][1] == joueur && grille[2][0] == joueur) {
        return true;
    }
    return false;
}

bool estPartieTerminee(vector<vector<char>> &grille) {
    // Vérifie si la grille est pleine ou s'il y a un gagnant
    if (estVictoire(grille, 'X') || estVictoire(grille, 'O')) {
        return true;
    }
    for (auto ligne : grille) {
        if (find(ligne.begin(), ligne.end(), '-') != ligne.end()) {
            return false;
        }
    }
    return true;
}


int scoreEtat(vector<vector<char>> &grille) {
    // Évalue l'état actuel du jeu
    if (estVictoire(grille, 'X')) {
        return 10;
    } else if (estVictoire(grille, 'O')) {
        return -10;
    } else {
        return 0;
    }
}

int minimax(vector<vector<char>> &grille, int profondeur, bool estMaximisant) {
    if (estPartieTerminee(grille)) {
        return scoreEtat(grille);
    }

    if (estMaximisant) {
        int meilleurScore = -1000;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (grille[i][j] == '-') {
                    grille[i][j] = 'X';
                    meilleurScore = max(meilleurScore, minimax(grille, profondeur + 1, !estMaximisant));
                    grille[i][j] = '-';
                }
            }
        }
        return meilleurScore;
    } else {
        int meilleurScore = 1000;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (grille[i][j] == '-') {
                    grille[i][j] = 'O';
                    meilleurScore = min(meilleurScore, minimax(grille, profondeur + 1, !estMaximisant));
                    grille[i][j] = '-';
                }
            }
        }
        return meilleurScore;
    }
}

pair<int, int> meilleurCoup(vector<vector<char>> &grille) {
    int meilleurScore = -1000;
    pair<int, int> meilleurCoup = make_pair(-1, -1);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (grille[i][j] == '-') {
                grille[i][j] = 'X';
                int score = minimax(grille, 0, false);
                grille[i][j] = '-';
                if (score > meilleurScore) {
                    meilleurScore = score;
                    meilleurCoup = make_pair(i, j);
                }
            }
        }
    }
    return meilleurCoup;
}

pair<int, int> choisirCaseVide(vector<vector<char>> &grille) {
    vector<pair<int, int>> cases_vides;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (grille[i][j] == '-') {
                cases_vides.push_back(make_pair(i, j));
            }
        }
    }
    return cases_vides[rand() % cases_vides.size()];
}

int jouerPartie(vector<vector<char>> grille, char symbol) {
    char joueur_actuel = symbol;
    while (!estPartieTerminee(grille)) {
        pair<int, int> coup;
        if (joueur_actuel == 'X') {
            coup = meilleurCoup(grille);
        } else {
            coup = choisirCaseVide(grille);
        }
        grille[coup.first][coup.second] = joueur_actuel;
        joueur_actuel = (joueur_actuel == 'X') ? 'O' : 'X';
    }
    if (estVictoire(grille, 'X')) {
        return 1;
    } else if (estVictoire(grille, 'O')) {
        return -1;
    } else {
        return 0;
    }
}

vector<vector<char>> genererGrilleDepart() {
    vector<vector<char>> grille(3, vector<char>(3, '-'));
    int coups_a_jouer = rand() % 10;
    char symboles[] = {'X', 'O'};
    char symbole_actuel = symboles[rand() % 2];

    for (int i = 0; i < coups_a_jouer; ++i) {
        int ligne = rand() % 3;
        int colonne = rand() % 3;
        if (grille[ligne][colonne] == '-') {
            grille[ligne][colonne] = symbole_actuel;
            symbole_actuel = (symbole_actuel == 'X') ? 'O' : 'X';
        }
    }

    return grille;
}

void afficherGrille(vector<vector<char>> &grille) {
    cout << "\n---------" << endl;
    for (auto ligne : grille) {
        for (auto cellule : ligne) {
            cout << " | " << cellule;
        }
        cout << " | " << endl << "---------" << endl;
    }
}

vector<pair<char, vector<vector<char>>>> parse_grids_from_file(string file_path) {
    vector<pair<char, vector<vector<char>>>> grids;
    ifstream file(file_path);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            if (line.length() < 10) {
                line.append(10 - line.length(), '-');
            }
            char symbol = line[0];
            line.erase(0, 1);
            for (char& c : line) {
                if (c == ' ') {
                    c = '-';
                }
            }
            vector<vector<char>> grid(3, vector<char>(3));
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    grid[i][j] = line[i * 3 + j];
                }
            }
            grids.push_back(make_pair(symbol, grid));
        }
        file.close();
    }
    return grids;
}


int main() {
    srand(time(nullptr));
    string file_path = "data/dataset.txt"; 
    vector<pair<char, vector<vector<char>>>> grids = parse_grids_from_file(file_path);

    int nblose = 0;
    int nbwin = 0;
    int nbdraw = 0;
    int nbline = 4519;
    double total_duration = 0;

    for (auto &entry : grids) {
        char symbol = entry.first;
        vector<vector<char>> grid = entry.second;

        clock_t start = clock();
        int r = jouerPartie(grid, symbol);
        clock_t end = clock();
        total_duration += (double)(end - start) / CLOCKS_PER_SEC;

        if (r == -1) {
            nblose++;
        } else if (r) {
            nbwin++;
        } else {
            nbdraw++;
        }
    }

    cout << "Winrate  = " << (double)(nbwin * 100) / nbline << " %" << endl;
    cout << "Loserate = " << (double)(nblose * 100) / nbline << " %" << endl;
    cout << "Drawrate = " << (double)(nbdraw * 100) / nbline << " %" << endl;
    cout << "AVG duration = " << total_duration / nbline << " s" << endl;
    cout << "Full duration = " << total_duration << " s" << endl;

    return 0;
}
