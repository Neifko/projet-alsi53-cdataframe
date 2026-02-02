#include <iostream>
#include <limits>
#include <string>
#include <sstream>
#include "Column.h"
#include "CDataFrame.h"

// ==================== DECLARATIONS DES TESTS ====================
void test_column_class();
void test_dataframe_class();
void test_part2_multitype();
void test_section6_tri();
void test_applyFunction_columns();
void test_dataframe_sorting();
void test_additional_methods();

// ==================== UTILITAIRES ====================
void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void waitForEnter() {
    std::cout << "\nAppuyez sur Entree pour continuer...";
    std::cin.get();
}

int getIntInput(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            clearInput(); // Toujours vider le buffer apres une lecture reussie
            return value;
        }
        std::cout << "Entree invalide. Veuillez entrer un nombre entier.\n";
        clearInput();
    }
}

int getIntInputInRange(const std::string& prompt, int minVal, int maxVal) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            clearInput();
            if (value >= minVal && value <= maxVal) {
                return value;
            }
            std::cout << "Choix invalide. Veuillez entrer un nombre entre "
                      << minVal << " et " << maxVal << ".\n";
        } else {
            std::cout << "Entree invalide. Veuillez entrer un nombre entier.\n";
            clearInput();
        }
    }
}

double getDoubleInput(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            clearInput();
            return value;
        }
        std::cout << "Entree invalide. Veuillez entrer un nombre.\n";
        clearInput();
    }
}

std::string getStringInput(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    return value;
}

ColumnType selectColumnType() {
    std::cout << "\nTypes disponibles:\n";
    std::cout << "  1. INT (entier 32 bits)\n";
    std::cout << "  2. UINT (entier non signe 32 bits)\n";
    std::cout << "  3. FLOAT (flottant simple precision)\n";
    std::cout << "  4. DOUBLE (flottant double precision)\n";
    std::cout << "  5. STRING (chaine de caracteres)\n";
    std::cout << "  6. SHORT (entier 16 bits)\n";
    std::cout << "  7. LONG (entier 64 bits)\n";

    int choice = getIntInputInRange("Choisissez le type (1-7): ", 1, 7);
    switch (choice) {
        case 1: return ColumnType::INT;
        case 2: return ColumnType::UINT;
        case 3: return ColumnType::FLOAT;
        case 4: return ColumnType::DOUBLE;
        case 5: return ColumnType::STRING;
        case 6: return ColumnType::SHORT;
        case 7: return ColumnType::LONG;
        default: return ColumnType::INT;
    }
}

ColumnValue inputValueForType(ColumnType type) {
    switch (type) {
        case ColumnType::INT: {
            int val = getIntInput("Entrez une valeur entiere (ou -999999 pour NULL): ");
            if (val == -999999) return std::monostate{};
            return val;
        }
        case ColumnType::UINT: {
            int val = getIntInput("Entrez une valeur entiere positive (ou -1 pour NULL): ");
            if (val < 0) return std::monostate{};
            return static_cast<uint32_t>(val);
        }
        case ColumnType::FLOAT: {
            double val = getDoubleInput("Entrez une valeur decimale (ou -999999 pour NULL): ");
            if (val == -999999) return std::monostate{};
            return static_cast<float>(val);
        }
        case ColumnType::DOUBLE: {
            double val = getDoubleInput("Entrez une valeur decimale (ou -999999 pour NULL): ");
            if (val == -999999) return std::monostate{};
            return val;
        }
        case ColumnType::STRING: {
            std::string val = getStringInput("Entrez une chaine (ou 'NULL' pour valeur nulle): ");
            if (val == "NULL" || val == "null") return std::monostate{};
            return val;
        }
        case ColumnType::SHORT: {
            int val = getIntInput("Entrez une valeur entiere courte (ou -999999 pour NULL): ");
            if (val == -999999) return std::monostate{};
            return static_cast<int16_t>(val);
        }
        case ColumnType::LONG: {
            int val = getIntInput("Entrez une valeur entiere longue (ou -999999 pour NULL): ");
            if (val == -999999) return std::monostate{};
            return static_cast<int64_t>(val);
        }
        default:
            return std::monostate{};
    }
}

std::string columnTypeToString(ColumnType type) {
    switch (type) {
        case ColumnType::NULLVAL: return "NULL";
        case ColumnType::UINT: return "UINT";
        case ColumnType::INT: return "INT";
        case ColumnType::USHORT: return "USHORT";
        case ColumnType::SHORT: return "SHORT";
        case ColumnType::ULONG: return "ULONG";
        case ColumnType::LONG: return "LONG";
        case ColumnType::UCHAR: return "UCHAR";
        case ColumnType::CHAR: return "CHAR";
        case ColumnType::FLOAT: return "FLOAT";
        case ColumnType::DOUBLE: return "DOUBLE";
        case ColumnType::STRING: return "STRING";
        default: return "UNKNOWN";
    }
}

// ==================== MENUS ====================

void menuAlimentation(CDataFrame& df, std::vector<ColumnType>& columnTypes);
void menuAffichage(CDataFrame& df);
void menuOperations(CDataFrame& df, std::vector<ColumnType>& columnTypes);
void menuStatistiques(CDataFrame& df);
void menuTri(CDataFrame& df);
void menuTests();

void afficherMenuPrincipal() {
    std::cout << "\n";
    std::cout << "+============================================================+\n";
    std::cout << "|           CDATAFRAME - MENU PRINCIPAL                      |\n";
    std::cout << "+============================================================+\n";
    std::cout << "|  1. Alimentation (creation, remplissage)                   |\n";
    std::cout << "|  2. Affichage                                              |\n";
    std::cout << "|  3. Operations usuelles                                    |\n";
    std::cout << "|  4. Analyse et statistiques                                |\n";
    std::cout << "|  5. Tri et recherche                                       |\n";
    std::cout << "|  6. Executer les tests                                     |\n";
    std::cout << "|  0. Quitter                                                |\n";
    std::cout << "+============================================================+\n";
}

// ==================== MENU ALIMENTATION ====================
void menuAlimentation(CDataFrame& df, std::vector<ColumnType>& columnTypes) {
    int choix;
    do {
        std::cout << "\n";
        std::cout << "+------------------------------------------+\n";
        std::cout << "|         ALIMENTATION                     |\n";
        std::cout << "+------------------------------------------+\n";
        std::cout << "|  1. Ajouter une colonne                  |\n";
        std::cout << "|  2. Inserer une ou plusieurs lignes      |\n";
        std::cout << "|  3. Charger donnees exemple              |\n";
        std::cout << "|  0. Retour au menu principal             |\n";
        std::cout << "+------------------------------------------+\n";

        choix = getIntInputInRange("Votre choix (0-3): ", 0, 3);

        switch (choix) {
            case 1: {
                std::string titre = getStringInput("Nom de la colonne: ");
                ColumnType type = selectColumnType();
                df.addColumn(titre, type);
                columnTypes.push_back(type);
                std::cout << "Colonne '" << titre << "' ajoutee avec succes.\n";
                break;
            }
            case 2: {
                if (df.getColumnsCount() == 0) {
                    std::cout << "Erreur: Ajoutez d'abord des colonnes.\n";
                    break;
                }
                int nbLignes = getIntInputInRange("Combien de lignes voulez-vous inserer? (1-100): ", 1, 100);

                for (int ligne = 0; ligne < nbLignes; ++ligne) {
                    std::cout << "\n--- Ligne " << (ligne + 1) << "/" << nbLignes << " ---\n";
                    std::vector<ColumnValue> row;
                    for (size_t i = 0; i < columnTypes.size(); ++i) {
                        std::cout << "  Colonne '" << i << "' (" << columnTypeToString(columnTypes[i]) << "): ";
                        row.push_back(inputValueForType(columnTypes[i]));
                    }
                    if (df.insertRow(row)) {
                        std::cout << "  Ligne inseree avec succes.\n";
                    } else {
                        std::cout << "  Erreur lors de l'insertion de la ligne.\n";
                    }
                }
                break;
            }
            case 3: {
                if (df.getColumnsCount() == 0) {
                    std::cout << "Creation d'un dataframe exemple...\n";
                    df.addColumn("ID", ColumnType::INT);
                    df.addColumn("Nom", ColumnType::STRING);
                    df.addColumn("Age", ColumnType::INT);
                    df.addColumn("Score", ColumnType::FLOAT);
                    columnTypes = {ColumnType::INT, ColumnType::STRING, ColumnType::INT, ColumnType::FLOAT};

                    df.insertRow({1, std::string("Alice"), 25, 85.5f});
                    df.insertRow({2, std::string("Bob"), 30, 92.0f});
                    df.insertRow({3, std::string("Charlie"), 22, 78.5f});
                    df.insertRow({4, std::string("Diana"), 28, 95.0f});
                    df.insertRow({5, std::string("Eve"), 35, 88.5f});
                    std::cout << "Dataframe exemple cree avec 5 lignes.\n";
                } else {
                    std::cout << "Le dataframe contient deja des colonnes.\n";
                    std::cout << "Utilisez l'option 2 pour ajouter des lignes.\n";
                }
                break;
            }
            case 0:
                break;
        }
    } while (choix != 0);
}

// ==================== MENU AFFICHAGE ====================
void menuAffichage(CDataFrame& df) {
    int choix;
    do {
        std::cout << "\n";
        std::cout << "+------------------------------------------+\n";
        std::cout << "|         AFFICHAGE                        |\n";
        std::cout << "+------------------------------------------+\n";
        std::cout << "|  1. Afficher tout le dataframe           |\n";
        std::cout << "|  2. Afficher l'en-tete                   |\n";
        std::cout << "|  3. Afficher les 10 premieres lignes     |\n";
        std::cout << "|  4. Afficher les 10 dernieres lignes     |\n";
        std::cout << "|  5. Afficher une plage de lignes         |\n";
        std::cout << "|  6. Afficher une plage de colonnes       |\n";
        std::cout << "|  7. Afficher les informations            |\n";
        std::cout << "|  0. Retour au menu principal             |\n";
        std::cout << "+------------------------------------------+\n";

        choix = getIntInputInRange("Votre choix (0-7): ", 0, 7);

        switch (choix) {
            case 1:
                std::cout << "\n=== DATAFRAME COMPLET ===\n";
                df.print();
                break;
            case 2:
                std::cout << "\n=== EN-TETE ===\n";
                df.printHeader();
                break;
            case 3:
                std::cout << "\n=== 10 PREMIERES LIGNES ===\n";
                df.printHead();
                break;
            case 4:
                std::cout << "\n=== 10 DERNIERES LIGNES ===\n";
                df.printTail();
                break;
            case 5: {
                size_t debut = static_cast<size_t>(getIntInput("Ligne de debut: "));
                size_t fin = static_cast<size_t>(getIntInput("Ligne de fin (exclusive): "));
                std::cout << "\n=== LIGNES " << debut << " A " << fin-1 << " ===\n";
                df.printRows(debut, fin);
                break;
            }
            case 6: {
                size_t debut = static_cast<size_t>(getIntInput("Colonne de debut: "));
                size_t fin = static_cast<size_t>(getIntInput("Colonne de fin (exclusive): "));
                std::cout << "\n=== COLONNES " << debut << " A " << fin-1 << " ===\n";
                df.printColumns(debut, fin);
                break;
            }
            case 7:
                std::cout << "\n";
                df.info();
                break;
            case 0:
                break;
        }
    } while (choix != 0);
}

// ==================== MENU OPERATIONS ====================
void menuOperations(CDataFrame& df, std::vector<ColumnType>& columnTypes) {
    int choix;
    do {
        std::cout << "\n";
        std::cout << "+------------------------------------------+\n";
        std::cout << "|         OPERATIONS USUELLES              |\n";
        std::cout << "+------------------------------------------+\n";
        std::cout << "|  1. Supprimer une ligne                  |\n";
        std::cout << "|  2. Supprimer une colonne                |\n";
        std::cout << "|  3. Renommer une colonne                 |\n";
        std::cout << "|  4. Modifier une valeur                  |\n";
        std::cout << "|  5. Acceder a une valeur                 |\n";
        std::cout << "|  6. Verifier l'existence d'une valeur    |\n";
        std::cout << "|  7. Renommer toutes les colonnes         |\n";
        std::cout << "|  0. Retour au menu principal             |\n";
        std::cout << "+------------------------------------------+\n";

        choix = getIntInputInRange("Votre choix (0-7): ", 0, 7);

        switch (choix) {
            case 1: {
                if (df.getRowsCount() == 0) {
                    std::cout << "Le dataframe ne contient aucune ligne.\n";
                    break;
                }
                size_t idx = static_cast<size_t>(getIntInput("Index de la ligne a supprimer: "));
                if (idx >= df.getRowsCount()) {
                    std::cout << "Erreur: Index " << idx << " invalide. Le dataframe contient "
                              << df.getRowsCount() << " lignes (indices 0 a " << df.getRowsCount()-1 << ").\n";
                } else {
                    df.deleteRow(idx);
                    std::cout << "Ligne " << idx << " supprimee avec succes.\n";
                }
                break;
            }
            case 2: {
                if (df.getColumnsCount() == 0) {
                    std::cout << "Le dataframe ne contient aucune colonne.\n";
                    break;
                }
                std::cout << "Colonnes disponibles: ";
                df.printHeader();
                std::string nom = getStringInput("Nom de la colonne a supprimer: ");
                bool found = df.deleteColumn(nom);
                if (found) {
                    // Mettre a jour columnTypes
                    for (size_t i = 0; i < columnTypes.size(); ++i) {
                        // On ne peut pas facilement retrouver l'index, on laisse tel quel
                    }
                    std::cout << "Colonne '" << nom << "' supprimee avec succes.\n";
                } else {
                    std::cout << "Erreur: La colonne '" << nom << "' n'existe pas.\n";
                }
                break;
            }
            case 3: {
                if (df.getColumnsCount() == 0) {
                    std::cout << "Le dataframe ne contient aucune colonne.\n";
                    break;
                }
                std::cout << "Colonnes disponibles: ";
                df.printHeader();
                std::string ancien = getStringInput("Nom actuel de la colonne: ");
                std::string nouveau = getStringInput("Nouveau nom: ");
                bool found = df.renameColumn(ancien, nouveau);
                if (found) {
                    std::cout << "Colonne '" << ancien << "' renommee en '" << nouveau << "'.\n";
                } else {
                    std::cout << "Erreur: La colonne '" << ancien << "' n'existe pas.\n";
                }
                break;
            }
            case 4: {
                if (df.getColumnsCount() == 0 || df.getRowsCount() == 0) {
                    std::cout << "Le dataframe est vide.\n";
                    break;
                }
                size_t row = static_cast<size_t>(getIntInput("Index de la ligne: "));
                size_t col = static_cast<size_t>(getIntInput("Index de la colonne: "));
                if (row >= df.getRowsCount()) {
                    std::cout << "Erreur: Index de ligne invalide.\n";
                    break;
                }
                if (col >= columnTypes.size()) {
                    std::cout << "Erreur: Index de colonne invalide.\n";
                    break;
                }
                ColumnValue newVal = inputValueForType(columnTypes[col]);
                try {
                    df.replaceValue(row, col, newVal);
                    std::cout << "Valeur modifiee avec succes.\n";
                } catch (const std::exception& e) {
                    std::cout << "Erreur: " << e.what() << "\n";
                }
                break;
            }
            case 5: {
                if (df.getColumnsCount() == 0 || df.getRowsCount() == 0) {
                    std::cout << "Le dataframe est vide.\n";
                    break;
                }
                size_t row = static_cast<size_t>(getIntInput("Index de la ligne: "));
                size_t col = static_cast<size_t>(getIntInput("Index de la colonne: "));
                try {
                    const ColumnValue& val = df.getValue(row, col);
                    std::string str = std::visit([](auto&& arg) -> std::string {
                        using T = std::decay_t<decltype(arg)>;
                        if constexpr (std::is_same_v<T, std::monostate>) return "NULL";
                        else if constexpr (std::is_same_v<T, std::string>) return arg;
                        else return std::to_string(arg);
                    }, val);
                    std::cout << "Valeur a (" << row << ", " << col << "): " << str << "\n";
                } catch (const std::exception& e) {
                    std::cout << "Erreur: " << e.what() << "\n";
                }
                break;
            }
            case 6: {
                if (df.getColumnsCount() == 0) {
                    std::cout << "Le dataframe est vide.\n";
                    break;
                }
                std::cout << "Type de valeur a rechercher:\n";
                std::cout << "  1. Entier\n";
                std::cout << "  2. Decimal\n";
                std::cout << "  3. Chaine\n";
                int typeChoix = getIntInputInRange("Choix (1-3): ", 1, 3);
                ColumnValue searchVal;
                if (typeChoix == 1) {
                    searchVal = getIntInput("Valeur entiere: ");
                } else if (typeChoix == 2) {
                    searchVal = static_cast<float>(getDoubleInput("Valeur decimale: "));
                } else {
                    searchVal = getStringInput("Chaine: ");
                }
                bool exists = df.valueExists(searchVal);
                std::cout << "La valeur " << (exists ? "EXISTE" : "N'EXISTE PAS") << " dans le dataframe.\n";
                break;
            }
            case 7: {
                if (df.getColumnsCount() == 0) {
                    std::cout << "Le dataframe ne contient aucune colonne.\n";
                    break;
                }
                std::vector<std::string> noms;
                std::cout << "Vous allez renommer les " << df.getColumnsCount() << " colonnes.\n";
                std::cout << "Colonnes actuelles: ";
                df.printHeader();
                for (size_t i = 0; i < df.getColumnsCount(); ++i) {
                    std::string nom = getStringInput("Nouveau nom pour la colonne " + std::to_string(i) + ": ");
                    noms.push_back(nom);
                }
                df.setColumnNames(noms);
                std::cout << "Noms des colonnes mis a jour avec succes.\n";
                std::cout << "Nouveaux noms: ";
                df.printHeader();
                break;
            }
            case 0:
                break;
        }
    } while (choix != 0);
}

// ==================== MENU STATISTIQUES ====================
void menuStatistiques(CDataFrame& df) {
    int choix;
    do {
        std::cout << "\n";
        std::cout << "+------------------------------------------+\n";
        std::cout << "|         ANALYSE ET STATISTIQUES          |\n";
        std::cout << "+------------------------------------------+\n";
        std::cout << "|  1. Nombre de lignes                     |\n";
        std::cout << "|  2. Nombre de colonnes                   |\n";
        std::cout << "|  3. Compter valeurs egales a X           |\n";
        std::cout << "|  4. Compter valeurs superieures a X      |\n";
        std::cout << "|  5. Compter valeurs inferieures a X      |\n";
        std::cout << "|  6. Informations completes               |\n";
        std::cout << "|  0. Retour au menu principal             |\n";
        std::cout << "+------------------------------------------+\n";

        choix = getIntInputInRange("Votre choix (0-6): ", 0, 6);

        switch (choix) {
            case 1:
                std::cout << "Nombre de lignes: " << df.getRowsCount() << "\n";
                break;
            case 2:
                std::cout << "Nombre de colonnes: " << df.getColumnsCount() << "\n";
                break;
            case 3: {
                std::cout << "Type de valeur:\n  1. Entier\n  2. Decimal\n";
                int t = getIntInputInRange("Choix (1-2): ", 1, 2);
                ColumnValue val;
                if (t == 1) val = getIntInput("Valeur X: ");
                else val = static_cast<float>(getDoubleInput("Valeur X: "));
                std::cout << "Nombre de cellules = X: " << df.countValuesEqualTo(val) << "\n";
                break;
            }
            case 4: {
                std::cout << "Type de valeur:\n  1. Entier\n  2. Decimal\n";
                int t = getIntInputInRange("Choix (1-2): ", 1, 2);
                ColumnValue val;
                if (t == 1) val = getIntInput("Valeur X: ");
                else val = static_cast<float>(getDoubleInput("Valeur X: "));
                std::cout << "Nombre de cellules > X: " << df.countValuesGreaterThan(val) << "\n";
                break;
            }
            case 5: {
                std::cout << "Type de valeur:\n  1. Entier\n  2. Decimal\n";
                int t = getIntInputInRange("Choix (1-2): ", 1, 2);
                ColumnValue val;
                if (t == 1) val = getIntInput("Valeur X: ");
                else val = static_cast<float>(getDoubleInput("Valeur X: "));
                std::cout << "Nombre de cellules < X: " << df.countValuesLessThan(val) << "\n";
                break;
            }
            case 6:
                df.info();
                break;
            case 0:
                break;
        }
    } while (choix != 0);
}

// ==================== MENU TRI ====================
void menuTri(CDataFrame& df) {
    int choix;
    do {
        std::cout << "\n";
        std::cout << "+------------------------------------------+\n";
        std::cout << "|         TRI ET RECHERCHE                 |\n";
        std::cout << "+------------------------------------------+\n";
        std::cout << "|  1. Trier par colonne (croissant)        |\n";
        std::cout << "|  2. Trier par colonne (decroissant)      |\n";
        std::cout << "|  3. Afficher dataframe trie              |\n";
        std::cout << "|  0. Retour au menu principal             |\n";
        std::cout << "+------------------------------------------+\n";

        choix = getIntInputInRange("Votre choix (0-3): ", 0, 3);

        switch (choix) {
            case 1: {
                if (df.getColumnsCount() == 0) {
                    std::cout << "Le dataframe est vide.\n";
                    break;
                }
                std::cout << "Colonnes disponibles: ";
                df.printHeader();
                std::string col = getStringInput("Nom de la colonne: ");
                df.sort(col, true);
                std::cout << "Dataframe trie par '" << col << "' (croissant).\n";
                break;
            }
            case 2: {
                if (df.getColumnsCount() == 0) {
                    std::cout << "Le dataframe est vide.\n";
                    break;
                }
                std::cout << "Colonnes disponibles: ";
                df.printHeader();
                std::string col = getStringInput("Nom de la colonne: ");
                df.sort(col, false);
                std::cout << "Dataframe trie par '" << col << "' (decroissant).\n";
                break;
            }
            case 3:
                std::cout << "\n=== DATAFRAME (ordre actuel) ===\n";
                df.print();
                break;
            case 0:
                break;
        }
    } while (choix != 0);
}

// ==================== MENU TESTS ====================
void menuTests() {
    int choix;
    do {
        std::cout << "\n";
        std::cout << "+------------------------------------------+\n";
        std::cout << "|         EXECUTION DES TESTS              |\n";
        std::cout << "+------------------------------------------+\n";
        std::cout << "|  1. Test classe Column                   |\n";
        std::cout << "|  2. Test classe CDataFrame               |\n";
        std::cout << "|  3. Test multi-types (Partie 2)          |\n";
        std::cout << "|  4. Test tri (Section 6)                 |\n";
        std::cout << "|  5. Test applyFunction                   |\n";
        std::cout << "|  6. Test tri global dataframe            |\n";
        std::cout << "|  7. Test methodes additionnelles         |\n";
        std::cout << "|  8. Executer TOUS les tests              |\n";
        std::cout << "|  0. Retour au menu principal             |\n";
        std::cout << "+------------------------------------------+\n";

        choix = getIntInputInRange("Votre choix (0-8): ", 0, 8);

        switch (choix) {
            case 1:
                test_column_class();
                break;
            case 2:
                test_dataframe_class();
                break;
            case 3:
                test_part2_multitype();
                break;
            case 4:
                test_section6_tri();
                break;
            case 5:
                test_applyFunction_columns();
                break;
            case 6:
                test_dataframe_sorting();
                break;
            case 7:
                test_additional_methods();
                break;
            case 8:
                std::cout << "\n========== EXECUTION DE TOUS LES TESTS ==========\n";
                test_column_class();
                test_dataframe_class();
                test_part2_multitype();
                test_section6_tri();
                test_applyFunction_columns();
                test_dataframe_sorting();
                test_additional_methods();
                std::cout << "\n========== FIN DE TOUS LES TESTS ==========\n";
                break;
            case 0:
                break;
        }

        if (choix != 0) {
            waitForEnter();
        }
    } while (choix != 0);
}

// ==================== MAIN ====================
int main() {
    std::cout << "+============================================================+\n";
    std::cout << "|        BIENVENUE DANS CDATAFRAME                           |\n";
    std::cout << "|        Projet de manipulation de donnees en C++            |\n";
    std::cout << "+============================================================+\n";

    CDataFrame df;
    std::vector<ColumnType> columnTypes;

    int choix;
    do {
        afficherMenuPrincipal();
        choix = getIntInputInRange("Votre choix (0-6): ", 0, 6);

        switch (choix) {
            case 1:
                menuAlimentation(df, columnTypes);
                break;
            case 2:
                menuAffichage(df);
                break;
            case 3:
                menuOperations(df, columnTypes);
                break;
            case 4:
                menuStatistiques(df);
                break;
            case 5:
                menuTri(df);
                break;
            case 6:
                menuTests();
                break;
            case 0:
                std::cout << "\nMerci d'avoir utilise CDataFrame. Au revoir!\n";
                break;
        }
    } while (choix != 0);

    return 0;
}

// ==================== IMPLEMENTATIONS DES TESTS ====================

void test_column_class() {
    std::cout << "\n=== TEST CLASSE COLUMN ===" << std::endl;

    Column col(ColumnType::INT);
    col.insertValue(10);
    col.insertValue(20);
    col.insertValue(10);

    col.print();

    std::cout << "Occurrences de 10: " << col.valueCount(10) << std::endl;
    std::cout << "Superieurs a 15: " << col.countValuesGreaterThan(15) << std::endl;
}

void test_part2_multitype() {
    std::cout << "\n=== PROJET PARTIE 2 : DATAFRAME MULTI-TYPES ===" << std::endl;

    CDataFrame myDataframe;

    std::cout << "[INFO] Creation des colonnes..." << std::endl;
    myDataframe.addColumn("Nom", ColumnType::STRING);
    myDataframe.addColumn("Age", ColumnType::INT);
    myDataframe.addColumn("Moyenne", ColumnType::FLOAT);

    std::cout << "[INFO] Insertion des lignes..." << std::endl;

    myDataframe.insertRow({std::string("Alice"), 20, 15.5f});
    myDataframe.insertRow({std::string("Bob"), 22, 12.0f});
    myDataframe.insertRow({std::string("Charlie"), 20, 18.5f});
    myDataframe.insertRow({std::string("David"), std::monostate{}, 9.5f});

    std::cout << "\n--- Affichage du Dataframe ---" << std::endl;
    myDataframe.print();

    std::cout << "\n--- Tests Statistiques ---" << std::endl;

    int countAge20 = myDataframe.countValuesEqualTo(20);
    std::cout << "Nombre de personnes ayant 20 ans : " << countAge20 << std::endl;

    int countMoyenneSup10 = myDataframe.countValuesGreaterThan(10.0f);
    std::cout << "Nombre de moyennes > 10.0 : " << countMoyenneSup10 << std::endl;

    std::cout << "\n--- Modification ---" << std::endl;
    myDataframe.renameColumn("Moyenne", "Note_Finale");
    myDataframe.replaceValue(1, 2, 14.5f);
    myDataframe.print();
}

void test_dataframe_class() {
    std::cout << "\n=== TEST CDATAFRAME ===" << std::endl;
    CDataFrame myDataframe;

    myDataframe.addColumn("ID", ColumnType::INT);
    myDataframe.addColumn("Age", ColumnType::INT);
    myDataframe.addColumn("Score", ColumnType::INT);

    myDataframe.insertRow({1, 20, 100});
    myDataframe.insertRow({2, 21, 200});
    myDataframe.insertRow({3, 22, 300});
    myDataframe.insertRow({4, 20, 150});

    std::cout << "--- Affichage complet ---" << std::endl;
    myDataframe.print();

    std::cout << "--- Renommage 'Score' en 'Points' ---" << std::endl;
    myDataframe.renameColumn("Score", "Points");

    std::cout << "--- Remplacement val (ligne 0, col 1) 20 -> 99 ---" << std::endl;
    myDataframe.replaceValue(0, 1, 99);

    std::cout << "--- Suppression ligne 2 (ID 3) ---" << std::endl;
    myDataframe.deleteRow(2);

    myDataframe.print();

    std::cout << "--- Statistiques ---" << std::endl;
    std::cout << "Valeurs > 150 : " << myDataframe.countValuesGreaterThan(150) << std::endl;
    std::cout << "Valeurs == 200 : " << myDataframe.countValuesEqualTo(200) << std::endl;
}

void test_section6_tri() {
    std::cout << "\n=== TEST SECTION 6 : TRI ===" << std::endl;

    Column col(ColumnType::INT, "Valeurs");
    col.insertValue(52);
    col.insertValue(44);
    col.insertValue(15);
    col.insertValue(18);

    std::cout << "--- Colonne avant tri ---" << std::endl;
    col.print();

    std::cout << "--- Tri croissant ---" << std::endl;
    col.sort(true);
    col.printSorted(true);

    std::cout << "--- Tri decroissant ---" << std::endl;
    col.sort(false);
    col.printSorted(false);

    int searchVal = 40;
    int result = col.searchValue(searchVal);
    if (result == 1) {
        std::cout << "Valeur " << searchVal << " trouvee dans la colonne" << std::endl;
    } else if (result == 0) {
        std::cout << "Valeur " << searchVal << " non trouvee dans la colonne" << std::endl;
    } else {
        std::cout << "Colonne non triee, recherche impossible" << std::endl;
    }
}

void test_applyFunction_columns() {
    std::cout << "\n=== TEST APPLYFUNCTION (PARTIE 2) ===" << std::endl;

    std::cout << "\n--- Test applyFunction sur une colonne (somme) ---" << std::endl;

    Column col(ColumnType::INT, "ColA");
    col.insertValue(10);
    col.insertValue(20);
    col.insertValue(30);
    col.insertValue(std::monostate{});

    col.print();

    auto sumFunc = [](int acc, int v) { return acc + v; };
    int sum = col.applyFunction<decltype(sumFunc), int>(sumFunc);
    std::cout << "Somme des valeurs : " << sum << std::endl;

    std::cout << "\n--- Test applyFunction sur deux colonnes (addition) ---" << std::endl;

    Column col1(ColumnType::INT, "Col1");
    Column col2(ColumnType::INT, "Col2");

    col1.insertValue(5);
    col1.insertValue(10);
    col1.insertValue(15);

    col2.insertValue(2);
    col2.insertValue(4);
    col2.insertValue(std::monostate{});

    col1.print();
    col2.print();

    auto colResult = Column::applyFunction(col1, col2, ColumnType::INT,
        [](int a, int b) { return a + b; });

    if (colResult) {
        std::cout << "\n--- Colonne resultat (Col1 + Col2) ---" << std::endl;
        colResult->print();
    } else {
        std::cout << "Erreur : colonnes incompatibles" << std::endl;
    }

    std::cout << "\n--- Test applyFunction (comparaison >) ---" << std::endl;

    auto colCompare = Column::applyFunction(col1, col2, ColumnType::INT,
        [](int a, int b) { return a > b ? 1 : 0; });

    if (colCompare) {
        colCompare->print();
    }
}

void test_dataframe_sorting() {
    std::cout << "\n=== TEST TRI GLOBAL DATAFRAME ===" << std::endl;

    CDataFrame df;
    df.addColumn("ID", ColumnType::INT);
    df.addColumn("Nom", ColumnType::STRING);
    df.addColumn("Note", ColumnType::FLOAT);

    df.insertRow({ 1, std::string("Charlie"), 12.5f });
    df.insertRow({ 2, std::string("Alice"), 18.0f });
    df.insertRow({ 3, std::string("Bob"), 15.0f });
    df.insertRow({ 4, std::string("Alice"), 10.0f });

    std::cout << "--- Original ---" << std::endl;
    df.print();

    std::cout << "\n--- Tri par Nom (Croissant) ---" << std::endl;
    df.sort("Nom", true);
    df.print();

    std::cout << "\n--- Tri par Note (Decroissant) ---" << std::endl;
    df.sort("Note", false);
    df.print();
}

void test_additional_methods() {
    std::cout << "\n=== TEST ADDITIONAL DF/COLUMN METHODS ===" << std::endl;

    CDataFrame df;
    df.addColumn("A", ColumnType::INT);
    df.addColumn("B", ColumnType::FLOAT);
    df.addColumn("C", ColumnType::STRING);

    df.insertRow({1, 2.5f, std::string("x")});
    df.insertRow({2, 3.5f, std::string("y")});
    df.insertRow({3, 1.5f, std::string("z")});

    std::cout << "\n--- printHeader ---" << std::endl;
    df.printHeader();

    std::cout << "\n--- printHead ---" << std::endl;
    df.printHead();

    std::cout << "\n--- printTail ---" << std::endl;
    df.printTail();

    std::cout << "\n--- printRows(1,3) ---" << std::endl;
    df.printRows(1, 3);

    std::cout << "\n--- printColumns(1,3) ---" << std::endl;
    df.printColumns(1, 3);

    std::cout << "\n--- setColumnNames(mismatch) ---" << std::endl;
    df.setColumnNames({"X", "Y"});

    std::cout << "\n--- setColumnNames(correct) ---" << std::endl;
    df.setColumnNames({"ID", "Val", "Name"});
    df.printHeader();

    std::cout << "\n--- deleteColumn('Val') ---" << std::endl;
    df.deleteColumn("Val");
    df.printHeader();
    df.print();

    std::cout << "\n--- valueExists(2) and valueExists(99) ---" << std::endl;
    std::cout << std::boolalpha
              << "exists 2: " << df.valueExists(2) << ", exists 99: " << df.valueExists(99)
              << std::noboolalpha << std::endl;

    std::cout << "\n--- getValue(0,1) ---" << std::endl;
    try {
        const ColumnValue &cv = df.getValue(0, 1);
        std::string str = std::visit([](auto&& arg) -> std::string {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, std::monostate>) return "NULL";
            else if constexpr (std::is_same_v<T, std::string>) return arg;
            else return std::to_string(arg);
        }, cv);
        std::cout << "Value at (0,1): " << str << std::endl;
    } catch (const std::exception &e) {
        std::cout << "getValue exception: " << e.what() << std::endl;
    }

    std::cout << "\n--- countValuesLessThan(3) ---" << std::endl;
    std::cout << "count < 3: " << df.countValuesLessThan(3) << std::endl;

    std::cout << "\n--- Column index/search tests ---" << std::endl;
    Column col(ColumnType::INT, "Tmp");
    col.insertValue(5);
    col.insertValue(1);
    col.insertValue(3);

    std::cout << "checkIndex (before sort): " << col.checkIndex() << std::endl;
    col.sort(true);
    std::cout << "checkIndex (after sort): " << col.checkIndex() << std::endl;
    std::cout << "searchValue(3): " << col.searchValue(3) << " (1 means found)" << std::endl;

    col.eraseIndex();
    std::cout << "checkIndex (after eraseIndex): " << col.checkIndex() << std::endl;

    col.insertValue(2);
    col.updateIndex();
    std::cout << "Final column content:" << std::endl;
    col.print();
}
