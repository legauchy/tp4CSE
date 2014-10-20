Gauchy Anthony
Lefevre Henry

Compte-rendu TP4 CSE
====================

## Question 1 : 
Cette implémentation respecte le fait que un écrivain et un lecteur ne peuvent pas
accéder en même temps à une ressource. Par contre un seul lecteur à la fois peut 
lire une ressource ce qui fait que cette solution ne répond pas au problème.

## Question 2 :
Solution : lectred.c
Cette solution n'est pas très équitable : 
	* Cette solution ne respecte pas l'odre FIFO, le premier à attendre une ressource peut donc être le dernier à la recevoir.
	* Cette solution est aussi particulièrement désavantageuse pour les ecrivains. En effet tant que des lecteurs font un begin_read pendant que d'autres sont encore entrain de lire ils entrent et empêchent donc les ecrivains d'accéder à la ressource.



