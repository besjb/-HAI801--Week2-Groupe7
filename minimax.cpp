#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>

using namespace std;

bool estPartieTerminee(vector<vector<char>>& grille);
bool estVictoire(vector<vector<char>>& grille, char joueur);
int scoreEtat(vector<vector<char>>& grille);
int minimax(vector<vector<char>>& grille, int profondeur, bool estMaximisant);
pair<int, int> meilleurCoup(vector<vector<char>>& grille);
pair<int, int> choisirCaseVide(vector<vector<char>>& grille);
void jouerPartie(vector<vector<char>>& grille, char symbol);
vector<vector<char>> genererGrilleDepart();
void afficherGrille(vector<vector<char>>& grille);
vector<pair<char, vector<vector<char>>>> parse_grids_from_file(string file_path);

bool estPartieTerminee(vector<vector<char>>& grille) {
    if (estVictoire(grille, 'X') || estVictoire(grille, 'O')) {
        return true;
    }
    for (auto& ligne : grille) {
        if (find(ligne.begin(), ligne.end(), '-') != ligne.end()) {
            return false;
        }
    }
    return true;
}

bool estVictoire(vector<vector<char>>& grille, char joueur) {
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

int scoreEtat(vector<vector<char>>& grille) {
    if (estVictoire(grille, 'X')) {
        return 10;
    } else if (estVictoire(grille, 'O')) {
        return -10;
    } else {
        return 0;
    }
}

int minimax(vector<vector<char>>& grille, int profondeur, bool estMaximisant) {
    if (estPartieTerminee(grille)) {
        return scoreEtat(grille);
    }

    if (estMaximisant) {
        int meilleurScore = -INFINITY;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (grille[i][j] == '-') {
                    grille[i][j] = 'X';
                    int score = minimax(grille, profondeur + 1, false);
                    grille[i][j] = '-';
                    meilleurScore = max(meilleurScore, score);
                }
            }
        }
        return meilleurScore;
    } else {
        int meilleurScore = INFINITY;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (grille[i][j] == '-') {
                    grille[i][j] = 'O';
                    int score = minimax(grille, profondeur + 1, true);
                    grille[i][j] = '-';
                    meilleurScore = min(meilleurScore, score);
                }
            }
        }
        return meilleurScore;
    }
}

pair<int, int> meilleurCoup(vector<vector<char>>& grille) {
    int meilleurScore = -INFINITY;
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

pair<int, int> choisirCaseVide(vector<vector<char>>& grille) {
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

void jouerPartie(vector<vector<char>>& grille, char symbol) {
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
    afficherGrille(grille);
}

vector<vector<char>> genererGrilleDepart() {
    vector<vector<char>> grille(3, vector<char>(3, '-'));
    int coups_a_jouer = rand() % 10; // Nombre aléatoire de coups à jouer
    char symboles[] = {'X', 'O'};
    char symbole_actuel = symboles[rand() % 2]; // Choisir aléatoirement le premier symbole

    for (int i = 0; i < coups_a_jouer; ++i) {
        pair<int, int> case_vide = choisirCaseVide(grille);
        grille[case_vide.first][case_vide.second] = symbole_actuel;
        symbole_actuel = (symbole_actuel == 'O') ? 'X' : 'O'; // Alterner entre 'X' et 'O'
    }

    return grille;
}

void afficherGrille(vector<vector<char>>& grille) {
    cout << "\n---------" << endl;
    for (auto& ligne : grille) {
        for (char c : ligne) {
            cout << " | " << c;
        }
        cout << " |" << endl;
        cout << "---------" << endl;
    }
}

vector<pair<char, vector<vector<char>>>> parse_grids_from_file(string file_path) {
    vector<pair<char, vector<vector<char>>>> grids;
    ifstream file(file_path);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            if (line.length() < 10) {
                continue;
            }
            char symbol = line[0];
            string values = line.substr(1);
            replace(values.begin(), values.end(), ' ', '-');
            vector<vector<char>> grid;
            for (size_t i = 0; i < values.length(); i += 3) {
                grid.push_back(vector<char>(values.begin() + i, values.begin() + i + 3));
            }
            grids.push_back(make_pair(symbol, grid));
        }
        file.close();
    }
    return grids;
}

int main() {
    string file_path = "data/dataset.txt"; // Remplacez "votre_fichier.txt" par le chemin de votre fichier
    vector<pair<char, vector<vector<char>>>> grids = parse_grids_from_file(file_path);
    for (auto& grid : grids) {
        jouerPartie(grid.second, grid.first);
    }
    return 0;
}
