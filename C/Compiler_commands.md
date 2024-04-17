Voici les commandes importantes pour utiliser le compilateur ainsi que valgrind :

## Compilation du C
1. Compiler les 'modules' :
```bash
gcc -c my_module.c
```
2. Compiler le code source :
```bash
gcc my_file.c my_module.o -o exectuable_name
```
Ou plutôt pour avoir tous les avertissements de compilateur et le débuggage :
```bash
gcc -g -std=c99 -Wall test.c -o test
```

### Flags
Voici une liste des flags utiles :
1. `-o` : permet de renommer le fichier de sortie. Exemple `gcc my_file.c -o exectuable_name`
2. `-std=c99` : informe le compilateur de la version de C utilisée dans le code.
3. `-c` : compile le fichier mais pas l'exécutable.
4. `-Wall` : permet au compilateur d'indiquer toutes les messages **Warning**.
5. `-g` : permet des informations de débuggage supplémentaires lorsque l'on utilise valgrind.
6. `-O` ou `-O1` : active l'optimisation. Peut-être utilisé en `-O2` pour encore plus d'optimisation. Pour les autres flags d'optimisation, allez voir [ici](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html).
7. `-I` : ajoute un dossier de fichiers headers. Exemple : `gcc my_file.c -I ./headers/ -o exectuable_name`


## Debuggage
Ajouter le flag `-g` :
`gcc -g my_file.c -o exectuable_name`

Puis utiliser [valgrind](https://doc.ubuntu-fr.org/valgrind) : (ne pas oublier tldr pour plus de commandes)
`valgrind executable_name`
Ou encore :
```
valgrind --tool=memcheck --leak-check=yes|no|full|summary --leak-resolution=low|med|high --show-reachable=yes ./executable_name
```
Où le paramètre `leak-check` peut prendre les valeurs suivantes :
- `no` : ne fait rien.
- `full` / `yes` : donne des détails sur chaque fuite mémoire.
- `summary` : indique le nombre de fuites mémoires.

Où le paramètre `show-reachable` étend la recherche aux zones encore pointées mais non désallouées.

Où le paramètre `leak-resolution` indique le niveau de recherche des fuites mémoires.



### Commande préfaite :
```
valgrind --tool=memcheck --leak-check=full --track-origins=yes ./executable_name
```
