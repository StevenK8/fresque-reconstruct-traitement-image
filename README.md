# traitement-image-tp1

1. L’objectif du premier exercice est de reconstruire une image de fresque à partir d’un ensemble de
fragments, et de leurs positions correctes à l’intérieur de l’image initiale. Chaque fragment a un index
index ≥ 0, et sa représentation se trouve dans le fichier frag_eroded_<index>.png. Dans le fichier
fragments.txt on fournit ligne par ligne les positions de tous les fragments qui font réellement partie
de la fresque, dans le format :
<index> <posx> <posy> <angle>
Le fichier fragments_s.txt dévoile les indices des fragments qui ne font pas partie de la fresque, et qui
ont été rajoutés juste pour mettre en difficulté un éventuel algorithme de reconstruction automatique.
Votre premier objectif sera donc de comprendre la signification exacte des valeurs contenues en
fragments.txt, et de les utiliser pour créer à partir des fragments l’image suivante (Figure 1), qui
devra avoir la même résolution que l’image initiale de la fresque (fournie)
2. En réalité, un compétiteur qui souhaite faire une reconstruction automatique construit un fichier de
localisation des fragments solution.txt ayant la même structure que fragments.txt, sans jamais
avoir accès a fragments.txt évidemment. Implémentez un programme qui calcule la qualité de la
reconstruction de solution.txt en la confrontant au contenu de la vérité terrain qui se trouve en
fragments.txt.
Pour cela on considère qu’un fragment i qui est positionné à (x, y, α) et qui dans la vérité terrain se
trouve réellement à (¯x, y, ¯ α¯) est bien localisé si : kx − x¯k ≤ ∆x, ky − y¯k ≤ ∆y et kα − α¯k ≤ ∆α.
On calcule la précision p de la localisation comme la surface de tous les fragments bien localisés, moins
la surface des fragments positionnés mais n’appartenant pas à la fresque, divisée par la surface de tous
les fragments de la fresque.
Les valeurs ∆x, ∆y, ∆α doivent rester des paramètres de votre programme ; mais typiquement elles
pourraient prendre la valeur de 1px, 1px et 1 deg respectivement.
Testez votre programme d’évaluation avec quelques fichiers solution.txt simples.
