#include <iostream>
#include "Column.h"
#include "CDataFrame.h"

void test_column_class();
void test_dataframe_class();

int main() {
	test_column_class();
	test_dataframe_class();
	return 0;
}

void test_column_class() {
	Column col("Test");

	col.insertValue(10);
	col.insertValue(20);
	col.insertValue(10);

	col.print();

	std::cout << "Occurrences de 10: " << col.valueCount(10) << std::endl;
	std::cout << "Superieurs a 15: " << col.countValuesGreaterThan(15) << std::endl;
	//int t = col.getValueAt(3); // to test out_of_range exception
}

void test_dataframe_class() {
	std::cout << "=== TEST CDATAFRAME ===" << std::endl;
	CDataFrame myDataframe;

	myDataframe.addColumn("ID");
	myDataframe.addColumn("Age");
	myDataframe.addColumn("Score");

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
