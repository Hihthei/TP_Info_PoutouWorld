# TP_Info_PoutouWorld


## __Contrôles__

- clavier :
		
	- flèche directionelle (*Déplacements*)
	- barre espace (*jump*)
		-> maintenue : le jump sera plus long et une fois en l'air le joueur planera (après un sauf uniquement)
		-> appuyé 1 fois : le jump sera plus court

	- La touche echap permet d'accéder au menu

- manette : 
	- joystick L (*Déplacements*)
	- bouton 3 (*jump*)
		-> maintenue : le jump est plus long et une fois en l'air le joueur plane
		-> appuyé : le jump s'arrête

	- La touche start permet d'accéder au menu



## __Features__

- Player :
	- Gestion de la physique :
		- Si le joueur avance dans la même direction, il accélère. Il récupère sa vitesse de base si il change de direction.
		- Comme tout oiseau qui se respecte, le joueur peut grimper sur les murs.
		- Si une pente est trop raide, alors le joueur ne peut pas la grimper et glisse.
		- Si le joueur maintient son saut, il va commencer à planer. Il retombe normalement si il lâche le saut.

	- Système de vie et de dégâts :
		- Le joueur va se prendre des dégâts quand il rentre en collision avec un ennemi ou un pique.
		- Si le joueur n'a plus de coeur, il perd une vie. Lorsqu'il n'a plus de vie, le niveau s'arrête et le joueur retourne au menu.
		  Le joueur respawn avec 3 coeurs actifs.
		- Si le joueur obtient 5 coeurs, alors il gagne une vie et retourne à 3 coeurs (donc 1 coeurs bonus).
		- Quand le joueur se prend des dégâts, il obient un état invincible pendant 2 secondes et perd sa colisions avec les noisettes.

	- Animations :
		- Quand le joueur se déplace, il va s'orienter vers là où il avance.
		- Si le joueur quitte le sol (n'est plus en contact direct), il aura l'animation de falling.
		  Pareil si le joueur est dans une pente trop raide, il ne pourra plus la grimper et donc sera en falling.
		- Si le joueur est en état invincible, il clignotera (adapté avec toutes les animations précédentes).
		- Quand le joueur meurt il est dans une animation de mort.
		- Quand le joueur accélère, son animation va aller plus vite. Elle reprend sa vitesse initiale lorsque le joueur ralenti.


- Ennemies :

	Notre jeu possède 2 ennemis différents

	- Noisette 1 : 	Fonce en direction du joueur et va le suivre jusqu'à la mort.
			Se désactive si elle est en dehors du champ de vision.
	- Noisette 2 : 	Rebondit sur les murs et entres les noisettes.

	- Boss : Le boss jettes des noisettes sur le joueur et se déplace dans les air.


- Objets collectibles :
	- Firefly :
		- Le joueur peut récupérer des lucioles. S'il meurt, toutes ses lucioles s'échappent de leurs cage.
		- Au bout de 30 lucioles collectées, il les sacrifie à satan pour gagner une vie, il retombe à 0 luciole.
		- Les lucioles ont une animation qui leur fait faire une sorte de demi-cercle (en aller et retour).
		- Lorsque le niveau restart, les lucioles respawn à leurs positions de départ.

	- Heart :
		- Le joueur peut récupérer des coeurs.
		- Au bout de 5 coeurs actifs, il gagne 1 vie et retombe à 3 coeurs.
		- Lorsque le niveau restart, les lucioles respawn à leurs positions de départ.



- Terrain :
	- Plateformes mouvantes :
	Il y a 4 types de plateformes :
		- Une petite platforme qui se déplace de droite à gauche en commençant par la droite.
		- Une grande platforme qui se déplace de droite à gauche en commençant par la droite.
		- Une petite platforme qui se déplace de droite à gauche en commençant par la gauche.
		- Une grande platforme qui se déplace de droite à gauche en commençant par la gauche.
		- Une petite platforme qui va tomber si le joueur marche dessus, remonte une fois arriver en bas.
		- Une grande platforme qui va tomber si le joueur marche dessus, remonte une fois arriver en bas.

	- Bloc cassable

	- Bloc de glace

	- Pentes :
	Il y a 2 types de pentes :
		- Une pente très raide qui va empêcher le joueur de grimper
		- Une pente plus souple qui va laisser le joueur grimper

	- Checkpoint

	- Bloc bonus
		- Ce bloc contiendra un coeur à l'avenir


- Affichage :
	- Affichage du nombre de vies restantes

	- Affichage du nombre de coeurs restants

	- Affichage du nombre de lucioles récupérées

	- La caméra ne peut désormais plus depasé de la map
