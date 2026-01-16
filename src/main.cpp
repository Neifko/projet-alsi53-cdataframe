#include <iostream>
#include "Column.h"
#include "CDataFrame.h"

void test_column_class();
void test_dataframe_class();
void test_part2_multitype();



int main() {
	test_column_class();
	test_dataframe_class();
	test_part2_multitype();
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

    myDataframe.insertRow({ std::string("Alice"), 20, 15.5f });
    myDataframe.insertRow({ std::string("Bob"), 22, 12.0f });
    myDataframe.insertRow({ std::string("Charlie"), 20, 18.5f });

    myDataframe.insertRow({ std::string("David"), std::monostate{}, 9.5f });

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

	myDataframe.insertRow({ 1, 20, 100 });
	myDataframe.insertRow({ 2, 21, 200 });
	myDataframe.insertRow({ 3, 22, 300 });
	myDataframe.insertRow({ 4, 20, 150 });

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
