# CDataFrame

Description
1. Petit projet C++ pour manipuler un DataFrame en console (création de colonnes, insertion de lignes, affichage, statistiques, tri, tests).
2. Interface interactive via menus.

Prérequis
1. Compilateur C++ (support C++17).
2. CMake.
3. CLion (recommandé) ou ligne de commande sous Windows.

Compilation et exécution
1. Avec CLion : ouvrez le projet et lancez la configuration Run.
2. Ligne de commande :
    1. `mkdir build`
    2. `cd build`
    3. `cmake ..`
    4. `cmake --build .`
    5. Lancez l'exécutable généré depuis le dossier `build` (ex. `.\VotreExecutable.exe` sous Windows) ou via CLion.

Utilisation
1. Le programme est interactif : utilisez les menus pour créer des colonnes, insérer des lignes, afficher et analyser les données.
2. Le menu "Tests" exécute plusieurs fonctions de test embarquées (classe `Column`, `CDataFrame`, tri, applyFunction, etc.).

Limitations connues
1. Les templates n'ont pas été implémentés.
2. Le chargement de fichiers CSV n'est pas pris en charge.
3. La surcharge d'opérateurs n'a pas été réalisée.
4. Certaines vérifications et conversions de types restent basiques.

Contributeurs
1. Guilleray Victor
2. Céraline Mickaël
3. Huang Christophe

Notes rapides
1. Le code est conçu pour être lu et testé via CLion (Windows).
2. Pour ajouter des améliorations envisagées : templates, import CSV, et surcharge d'opérateurs.
