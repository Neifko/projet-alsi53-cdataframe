#include <iostream>
#include "Column.h"
#include "CDataFrame.h"
#include <type_traits>
#include <string>

void test_column_class();

void test_dataframe_class();

void test_part2_multitype();

void test_section6_tri();

void test_applyFunction_columns();

void test_additional_methods();


int main() {
    test_column_class();
    test_dataframe_class();
    test_part2_multitype();
    test_section6_tri();
    test_applyFunction_columns();

    // Added additional tests for methods not previously exercised
    test_additional_methods();

    return 0;
}

void test_column_class() {
    Column col(ColumnType::INT);

    col.insertValue(10);
    col.insertValue(20);
    col.insertValue(10);

    col.print();

    std::cout << "Occurrences de 10: " << col.valueCount(10) << std::endl;
    std::cout << "Superieurs a 15: " << col.countValuesGreaterThan(15) << std::endl;
    //int t = col.getValueAt(3); // to test out_of_range exception
}

void test_part2_multitype() {
    std::cout << "\n\n=== PROJET PARTIE 2 : DATAFRAME MULTI-TYPES ===" << std::endl;

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
    std::cout << "=== TEST CDATAFRAME ===" << std::endl;
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
    std::cout << "\n\n=== TEST SECTION 6 : TRI ===" << std::endl;

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
    std::cout << "\n\n=== TEST APPLYFUNCTION (PARTIE 2) ===" << std::endl;

    // ===== TEST 1 : applyFunction sur UNE colonne =====
    std::cout << "\n--- Test applyFunction sur une colonne (somme) ---" << std::endl;

    Column col(ColumnType::INT, "ColA");
    col.insertValue(10);
    col.insertValue(20);
    col.insertValue(30);
    col.insertValue(std::monostate{}); // test NULL

    col.print();

    auto sumFunc = [](int acc, int v) {
        return acc + v;
    };

    int sum = col.applyFunction<decltype(sumFunc), int>(sumFunc);

    std::cout << "Somme des valeurs : " << sum << std::endl;

    // ===== TEST 2 : applyFunction sur DEUX colonnes =====
    std::cout << "\n--- Test applyFunction sur deux colonnes (addition) ---" << std::endl;

    Column col1(ColumnType::INT, "Col1");
    Column col2(ColumnType::INT, "Col2");

    col1.insertValue(5);
    col1.insertValue(10);
    col1.insertValue(15);

    col2.insertValue(2);
    col2.insertValue(4);
    col2.insertValue(std::monostate{}); // NULL volontaire

    col1.print();
    col2.print();

    auto colResult = Column::applyFunction(
        col1,
        col2,
        ColumnType::INT,
        [](int a, int b) {
            return a + b;
        }
    );

    if (colResult) {
        std::cout << "\n--- Colonne resultat (Col1 + Col2) ---" << std::endl;
        colResult->print();
    } else {
        std::cout << "Erreur : colonnes incompatibles" << std::endl;
    }

    // ===== TEST 3 : comparaison (bool) =====
    std::cout << "\n--- Test applyFunction (comparaison >) ---" << std::endl;

    auto colCompare = Column::applyFunction(
        col1,
        col2,
        ColumnType::INT,
        [](int a, int b) {
            return a > b ? 1 : 0;
        }
    );

    if (colCompare) {
        colCompare->print();
    }
}

// New: helper to convert ColumnValue -> string (used in tests)
static auto columnValueToString = [](const ColumnValue& v) -> std::string {
    return std::visit([](auto&& arg) -> std::string {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::monostate>) {
            return "NULL";
        } else if constexpr (std::is_same_v<T, std::string>) {
            return arg;
        } else {
            return std::to_string(arg);
        }
    }, v);
};

// New: tests for DataFrame and Column methods not covered previously
void test_additional_methods() {
    std::cout << "\n\n=== TEST ADDITIONAL DF/COLUMN METHODS ===" << std::endl;

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
    df.printRows(1, 3); // prints rows 1 and 2

    std::cout << "\n--- printColumns(1,3) ---" << std::endl;
    df.printColumns(1, 3); // prints columns B and C

    std::cout << "\n--- setColumnNames(mismatch) ---" << std::endl;
    df.setColumnNames({"X", "Y"}); // should warn

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
        std::cout << "Value at (0,1): " << columnValueToString(cv) << std::endl;
    } catch (const std::exception &e) {
        std::cout << "getValue exception: " << e.what() << std::endl;
    }

    std::cout << "\n--- countValuesLessThan(3) ---" << std::endl;
    std::cout << "count < 3: " << df.countValuesLessThan(3) << std::endl;

    // Column-specific extras
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

    // Insert a value and call updateIndex (no-op if index empty)
    col.insertValue(2);
    col.updateIndex();
    std::cout << "Final column content:" << std::endl;
    col.print();
}
