Exercice 1 : Chargement dynamique de bibliothèques
- Les codes du programme principal et ses bibliothèques sont souvent séparés en des fichiers .c (le programme principal cron_func.c), .h (les headers de fonction), .c (l'implémentation des fonctions de la bibliothèque). 

- Par défaut, gcc compile tous ces fichiers et les embarque dans un fichier d'exécution unique après les compile en fichier .o par un outil qui s'appelle linker; 

- Problématique: pour chaque fois, si on veut changer le comportement d'un fonction de la bibliothèque, on devrait recompiler tous les codes. 
=> Idée: à partir les fichiers header .h et les fichiers d'implémentation .c de la fonction, on va créer des bibliothèques qui seront chargées dynamiquement au moment où on exécute le programme principal. Cette utilité du noyau s'appelle ldd qui sera chargé, juste avant d'éxécuter le main(), de charger les différentes bibliothèques nécessaires. Ces bibliothèques dynamiques sont formalisées sous forme des fichiers .so (Exemple: libfunc.so). 

- Deux étapes pour créer un fichier de la bibliothèque dynamique: 
	+ compiler le fichier .h et .c en .o grâce à l'option -fPIC (Position Independant Code) de gcc. Cette option indique que le résultat de la compilation est indépendant
	+ faire packaging des fichiers .o dans un fichier .so qui a des métadonnées supplémentaires qui permettent de savoir où sont les différents symboles (de fonctions, variables, ...). Pour cela, il faut indiquer que le fichier .so généré est une bibliothèque partagée, on ajoute l'option -shared pour dire à linker que c'est partagé. 
	- un fichier .so peut être constitué de pleins de fichiers .o
	- le programme principal est compilé avec une fonction dans une bibliothèque par défaut. Pour chaque changement, on n'a pas besoin de recompiler ce programme, ms on devrait refaire les tables de liaisons, car on ne va plus lier le programme avec la fonction au début mais avec d'autre version de la fonction grâce à l'option -L  
	
- Attention : Quand on exécute notre binaire, ldd va chercher libfunc.so. Sauf que par défaut dans tous les Unix, il ne regarde pas dans le répertoire courant (alors que libfunc y est). Pour cela il existe une variable d'environnement nommée LD_LIBRARY_PATH, qui permet de rajouter des chemins vers des répertoires qui contiennent des bibliothèques à charger. En faisant LD_LIBRARY_PATH=., on dit que notre variable d'environnement contient le chemin .. Et au moment où on va charger le programme en mémoire, ldd va lire cette variable d'environnement et comprendre que le répertoire courant contient aussi des librairies et va donc essayer de voir s'il y trouve la libfunc.so.
	LD_LIBRARY_PATH=. ./cron_func
	
- LD_PRELOAD : on peut avoir plusieurs répertoires qui contiennent des bibliothèques (/usr/lib, /lib, /usr/local/lib/...). Tout cela sera dans LD_LIBRARY_PATH. Quand on exécute un programme, on peut en fait avoir plusieurs version d'un même programme (une dans /usr/bin, une dans /bin, une dans /usr/local/bin). Le système va regarder la variable d'environnement PATH et exécuter la première version qu'il trouve. Donc si on exécute par exemple le programme echo, s'il est à la fois dans /bin et dans /usr/bin, il va exécuter celui qui est dans /bin. C'est pareil pour les bibliothèques, donc si le programme a besoin de libfunc.so et qu'il est dans tous ces répertoires, il va prendre la première qu'il trouve. La variable d'environnement LD_PRELOAD permet d'indiquer directement une bibliothèque, donc son principe est d'indiquer de charger une bibliothèque précise, cela le force à charger une version particulière.
LD_PRELOAD=libfunc.so ./cron_func

Par exemple, dans la libc on a la fonction read(). Qu'est-ce que l'on pourrait faire avec LD_PRELOAD ? On pourrait très bien coder notre propre read() (c'est le sujet de la question 3) et faire ce que l'on souhaite dedans. En résumé, on peut surcharger avec LD_PRELOAD les fonctions du système. C'est un moyen de faire à ce qui ressemble à du piratage, puisqu'on peut faire des keyloggers, le principe étant de détourner read() pour enregistrer tout ce qui est tapé au clavier (y compris les mots de passe par exemple) puis d'exécuter la fonction d'origine pour que l'utilisateur n'y voit que du feu.

LD_PRELOAD=libread.so ./cron_func

#include <dlfcn.h>

void *dlopen(const char *filename, int flag);

char *dlerror(void);

void *dlsym(void *handle, const char *symbol);

int dlclose(void *handle);

Link with -ldl.

dlopen() prends un filename et retourne un "handle" opaque

dlsym prends le "handle" d'une lib. dyn. et un nom de symbole.
retourne NULL si non trouvé
RTLD_DEFAULT et RTLD_NEXT sont des pseudo-handler

dlclose() prends un handler et décrémente un cpt de ref et décharge la lib. si
nécéssaire
